//! The module provides functions to generate svg files.

use crate::stream_if::geometry::Color;
use crate::stream_if::geometry::DrawDirective;
use crate::stream_if::geometry::Pen;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;
use std::fs::File;
use std::io::Write;

/// This is the rounding unit value for absolute coordinates.
/// This unit has an exact representation in both binary and decimal.
const ABS_UNIT: f32 = 0.125;

/// This is the rounding unit value for relative coordinates.
/// It has higher precision than ABS_UNIT to keep the sum of errors low.
/// This unit has an exact representation in both binary and decimal.
const REL_UNIT: f32 = 0.0625;

/// Defines a vector renderer
pub struct VecRenderer<'my_lifespan> {
    /// The file that is open for writing
    pub output_file: &'my_lifespan mut File,
    /// default colors are black stroke and white fill if requested
    pub force_colors: bool,
}

/// The VecRenderer struct provides some methods to write svg header and footer
impl<'my_lifespan> VecRenderer<'my_lifespan> {
    /// The function header converts the vector graphics drawing directive header to svg format
    ///
    /// # Arguments
    ///
    /// * `view` - The bounding box of the visible area
    ///
    /// # Panics
    ///
    /// This function panics if the vector graphics cannot be written to a file.
    ///
    pub(super) fn header(self: &mut Self, view: &Rect) -> () {
        write!(
            self.output_file,
            "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<svg
    width=\"100%\"
    height=\"100%\"
    viewBox=\"{} {} {} {}\"
    preserveAspectRatio=\"xMidYMid meet\"
    xmlns=\"http://www.w3.org/2000/svg\"
    version=\"1.1\"
    >
\
            ",
            view.left, view.top, view.width, view.height
        )
        .expect("Error at writing file");
    }

    /// The function footer converts the vector graphics drawing directive footer to svg format
    ///
    /// # Panics
    ///
    /// This function panics if the vector graphics cannot be written to a file.
    ///
    pub(super) fn footer(self: &mut Self) -> () {
        write!(self.output_file, "</svg>\n").expect("Error at writing file");
    }
}

/// The VecRenderer struct provides some methods that implement a PathRenderer
impl<'my_lifespan> PathRenderer for VecRenderer<'my_lifespan> {
    /// The function render_path converts the vector graphics drawing directive path
    /// to svg format
    /// # Arguments
    ///
    /// * `segs` - The segments of the path
    /// * `stroke` - The foreground color and width by which the path is stroked
    /// * `fill` - The background color by which the path is filled
    ///
    /// # Panics
    ///
    /// This function panics if the vector graphics cannot be written to a file.
    ///
    fn render_path(
        self: &mut Self,
        segs: &[DrawDirective],
        stroke: &Option<Pen>,
        fill: &Option<Color>,
    ) -> () {
        write!(self.output_file, "    <path").expect("Error at writing file");
        match (stroke, fill, self.force_colors) {
            (Some(pen), _, _) => {
                write!(self.output_file, " stroke=\"#{}\"", pen.color.to_svg())
                    .expect("Error at writing file");
                /*
                 * Note that the current version of crstal-facet-uml does not allow stroke-width in stereotype images.
                write!(self.output_file, " stroke-width=\"{}\"", pen.width)
                    .expect("Error at writing file");
                */
            }
            (None, Some(_), _) => {
                write!(self.output_file, " stroke=\"none\"").expect("Error at writing file");
            }
            (None, None, true) => {
                write!(self.output_file, " stroke=\"black\"").expect("Error at writing file");
            }
            (None, None, false) => {}
        }
        match (fill, self.force_colors) {
            (Some(color), _) => {
                write!(self.output_file, " fill=\"#{}\"", color.to_svg())
                    .expect("Error at writing file");
            }
            (None, true) => {
                write!(self.output_file, " fill=\"none\"").expect("Error at writing file");
            }
            (None, false) => {}
        }

        write!(self.output_file, "\n        d=\"").expect("Error at writing file");
        for seg in segs {
            match seg {
                DrawDirective::Move(target) => {
                    write!(
                        self.output_file,
                        "M {},{} ",
                        target.round_x(ABS_UNIT),
                        target.round_y(ABS_UNIT)
                    )
                }
                DrawDirective::MoveRel(offset) => {
                    write!(
                        self.output_file,
                        "m {},{} ",
                        offset.round_dx(REL_UNIT),
                        offset.round_dy(REL_UNIT)
                    )
                }
                DrawDirective::Line(target) => {
                    write!(
                        self.output_file,
                        "L {},{} ",
                        target.round_x(ABS_UNIT),
                        target.round_y(ABS_UNIT)
                    )
                }
                DrawDirective::LineRel(offset) => {
                    write!(
                        self.output_file,
                        "l {},{} ",
                        offset.round_dx(REL_UNIT),
                        offset.round_dy(REL_UNIT)
                    )
                }
                DrawDirective::Continue(target) => {
                    write!(
                        self.output_file,
                        "{},{} ",
                        target.round_x(ABS_UNIT),
                        target.round_y(ABS_UNIT)
                    )
                }
                DrawDirective::ContinueRel(offset) => {
                    write!(
                        self.output_file,
                        "{},{} ",
                        offset.round_dx(REL_UNIT),
                        offset.round_dy(REL_UNIT)
                    )
                }
                DrawDirective::Curve(p1, p2, target) => {
                    write!(
                        self.output_file,
                        "C {},{} {},{} {},{} ",
                        p1.round_x(ABS_UNIT),
                        p1.round_y(ABS_UNIT),
                        p2.round_x(ABS_UNIT),
                        p2.round_y(ABS_UNIT),
                        target.round_x(ABS_UNIT),
                        target.round_y(ABS_UNIT)
                    )
                }
                DrawDirective::CurveRel(o_p1, o_p2, offset) => {
                    write!(
                        self.output_file,
                        "c {},{} {},{} {},{} ",
                        o_p1.round_dx(REL_UNIT),
                        o_p1.round_dy(REL_UNIT),
                        o_p2.round_dx(REL_UNIT),
                        o_p2.round_dy(REL_UNIT),
                        offset.round_dx(REL_UNIT),
                        offset.round_dy(REL_UNIT)
                    )
                }
                DrawDirective::Symmetric(p2, target) => {
                    write!(
                        self.output_file,
                        "S {},{} {},{} ",
                        p2.round_x(ABS_UNIT),
                        p2.round_y(ABS_UNIT),
                        target.round_x(ABS_UNIT),
                        target.round_y(ABS_UNIT)
                    )
                }
                DrawDirective::SymmetricRel(o_p2, offset) => {
                    write!(
                        self.output_file,
                        "s {},{} {},{} ",
                        o_p2.round_dx(REL_UNIT),
                        o_p2.round_dy(REL_UNIT),
                        offset.round_dx(REL_UNIT),
                        offset.round_dy(REL_UNIT)
                    )
                }
                DrawDirective::Close => {
                    write!(self.output_file, "Z ")
                }
                DrawDirective::CloseRel => {
                    write!(self.output_file, "z ")
                }
            }
            .expect("Error at writing file");
        }
        write!(self.output_file, "\"\n    />\n").expect("Error at writing file");
    }
}

/*
Copyright 2023-2026 Andreas Warnke

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
