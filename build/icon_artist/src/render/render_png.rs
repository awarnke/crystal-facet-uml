//! The module provides functions to generate gdk pixbuf c-source files.

use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;
use crate::stream_if::path_renderer::PathRenderer;
use raqote;

/// Defines a png renderer that produces png icons
pub struct PngRenderer<'my_lifespan> {
    /// The file that is open for writing
    output_file_name: &'my_lifespan str,
    /// The offset is applied to absolute coordinates before scaling
    offset_x: f32,
    /// The offset is applied to absolute coordinates before scaling
    offset_y: f32,
    /// The scale is applied to absolute and relative coordinates
    scale_x: f32,
    /// The scale is applied to absolute and relative coordinates
    scale_y: f32,
    /// The target canvas of the 2d drawing library
    dt: raqote::DrawTarget,
}

/// The PngRenderer struct provides some methods to write png files
impl<'my_lifespan> PngRenderer<'my_lifespan> {
    /// The function new initializes a PngRenderer
    ///
    /// # Arguments
    ///
    /// * `output_file_name` - The file name in which to write png icons data
    /// * `viewport` - The drawing rectangle bounds
    ///
    /// # Panics
    ///
    /// Depends on library calls.
    ///
    pub(super) fn new(
        output_file_name: &'my_lifespan str,
        canvas_width: i32,
        canvas_height: i32,
        offset_x: f32,
        offset_y: f32,
        scale_x: f32,
        scale_y: f32,
    ) -> PngRenderer<'my_lifespan> {
        PngRenderer {
            output_file_name: output_file_name,
            offset_x: offset_x,
            offset_y: offset_y,
            scale_x: scale_x,
            scale_y: scale_y,
            dt: raqote::DrawTarget::new(canvas_width, canvas_height),
        }
    }

    /// The function write_png converts the graphics drawing to png format
    ///
    /// # Arguments
    ///
    /// * `view` - The bounding box of the visible area
    ///
    /// # Panics
    ///
    /// This function panics if the vector graphics cannot be written to a file.
    ///
    pub(super) fn write_png(self: &mut Self) -> () {
        self.dt.write_png(self.output_file_name).unwrap();
    }
}

/// The PngRenderer struct provides some methods that implement a PathRenderer
impl<'my_lifespan> PathRenderer for PngRenderer<'my_lifespan> {
    /// The function render_path converts the vector graphics drawing directive path
    /// to raqote DrawTarget
    ///
    /// # Arguments
    ///
    /// * `segs` - The segments of the path
    /// * `stroke` - The foreground color and width by which the path is stroked
    /// * `fill` - The background color by which the path is filled
    ///
    /// # Panics
    ///
    /// Depends on library calls.
    ///
    fn render_path(
        self: &mut Self,
        segs: &[DrawDirective],
        stroke: &Option<geometry::Pen>,
        fill: &Option<geometry::Color>,
    ) -> () {
        let mut section_start = Point { x: 0.0, y: 0.0 };
        let mut cursor = Point { x: 0.0, y: 0.0 };
        let mut direction = Offset { dx: 0.0, dy: 0.0 };
        let mut pb = raqote::PathBuilder::new();
        for seg in segs {
            match seg {
                DrawDirective::Move(target) => {
                    pb.move_to(
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = Offset::delta(cursor, *target);
                    cursor = *target;
                    section_start = *target;
                }
                DrawDirective::MoveRel(offset) => {
                    let target = Point::add(cursor, *offset);
                    pb.move_to(
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = *offset;
                    cursor = target;
                    section_start = target;
                }
                DrawDirective::Line(target) => {
                    pb.line_to(
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = Offset::delta(cursor, *target);
                    cursor = *target;
                }
                DrawDirective::LineRel(offset) => {
                    let target = Point::add(cursor, *offset);
                    pb.line_to(
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = *offset;
                    cursor = target;
                }
                DrawDirective::Continue(target) => {
                    /* assume last operation was a line */
                    pb.line_to(
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = Offset::delta(cursor, *target);
                    cursor = *target;
                }
                DrawDirective::ContinueRel(offset) => {
                    /* assume last operation was a line */
                    let target = Point::add(cursor, *offset);
                    pb.line_to(
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = *offset;
                    cursor = target;
                }
                DrawDirective::Curve(p1, p2, target) => {
                    pb.cubic_to(
                        (p1.x + self.offset_x) * self.scale_x,
                        (p1.y + self.offset_y) * self.scale_y,
                        (p2.x + self.offset_x) * self.scale_x,
                        (p2.y + self.offset_y) * self.scale_y,
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = Offset::delta(*p2, *target);
                    cursor = *target;
                }
                DrawDirective::CurveRel(o_p1, o_p2, offset) => {
                    let p1 = Point::add(cursor, *o_p1);
                    let p2 = Point::add(cursor, *o_p2);
                    let target = Point::add(cursor, *offset);
                    pb.cubic_to(
                        (p1.x + self.offset_x) * self.scale_x,
                        (p1.y + self.offset_y) * self.scale_y,
                        (p2.x + self.offset_x) * self.scale_x,
                        (p2.y + self.offset_y) * self.scale_y,
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = Offset::delta(p2, target);
                    cursor = target;
                }
                DrawDirective::Symmetric(p2, target) => {
                    let p1 = Point::add(cursor, direction);
                    pb.cubic_to(
                        (p1.x + self.offset_x) * self.scale_x,
                        (p1.y + self.offset_y) * self.scale_y,
                        (p2.x + self.offset_x) * self.scale_x,
                        (p2.y + self.offset_y) * self.scale_y,
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    direction = Offset::delta(*p2, *target);
                    cursor = *target;
                }
                DrawDirective::SymmetricRel(o_p2, offset) => {
                    let p1 = Point::add(cursor, direction);
                    let p2 = Point::add(cursor, *o_p2);
                    let target = Point::add(cursor, *offset);
                    pb.cubic_to(
                        (p1.x + self.offset_x) * self.scale_x,
                        (p1.y + self.offset_y) * self.scale_y,
                        (p2.x + self.offset_x) * self.scale_x,
                        (p2.y + self.offset_y) * self.scale_y,
                        (target.x + self.offset_x) * self.scale_x,
                        (target.y + self.offset_y) * self.scale_y,
                    );
                    /* update cursor and direction */
                    cursor = target;
                    direction = Offset::delta(p2, target);
                }
                DrawDirective::Close => {
                    pb.close();
                    /* update cursor and direction */
                    direction = Offset::delta(cursor, section_start);
                    cursor = section_start;
                }
                DrawDirective::CloseRel => {
                    pb.close();
                    /* update cursor and direction */
                    direction = Offset::delta(cursor, section_start);
                    cursor = section_start;
                }
            }
        }
        let path = pb.finish();
        match fill {
            Some(ground_color) => {
                let options = raqote::DrawOptions {
                    blend_mode: raqote::BlendMode::SrcOver,
                    alpha: 1.0,
                    antialias: raqote::AntialiasMode::None,
                };
                self.dt.fill(
                    &path,
                    &raqote::Source::Solid(raqote::SolidSource {
                        r: ground_color.red,
                        g: ground_color.green,
                        b: ground_color.blue,
                        a: 0xff,
                    }),
                    &options,
                );
            }
            None => {}
        }
        match stroke {
            Some(pen) => {
                let options = raqote::DrawOptions {
                    blend_mode: raqote::BlendMode::SrcOver,
                    alpha: 1.0,
                    antialias: raqote::AntialiasMode::Gray,
                };
                self.dt.stroke(
                    &path,
                    &raqote::Source::Solid(raqote::SolidSource {
                        r: pen.color.red,
                        g: pen.color.green,
                        b: pen.color.blue,
                        a: 0xff,
                    }),
                    &raqote::StrokeStyle {
                        cap: raqote::LineCap::Round,
                        join: raqote::LineJoin::Round,
                        width: pen.width,
                        miter_limit: 0.0,
                        dash_array: vec![],
                        dash_offset: 0.0,
                    },
                    &options,
                );
            }
            None => {}
        }
    }
}

/*
Copyright 2024-2025 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
