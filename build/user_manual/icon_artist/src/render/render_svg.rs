//! The module provides functions to generate svg files.

use crate::model::geometry::Color;
use crate::model::geometry::DrawDirective;
use crate::model::geometry::Rect;
use std::fs::File;
use std::io::Write;

/// The rounding unit for absolute coordinates
const ABS_UNIT: f32 = 0.125;

/// The rounding unit for relative coordinates
const REL_UNIT: f32 = 0.0625;

/// Defines a vector renderer
pub struct VecRenderer<'my_lifespan> {
    /// The file that is open for writing
    pub output_file: &'my_lifespan mut File,
    /// default colors are black stroke and white fill if requested
    pub force_colors: bool,
}

/// The Rect struct provides some methods
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
        write!(self.output_file, "\n</svg>\n").expect("Error at writing file");
    }

    /// The function path converts the vector graphics drawing directive path to svg format
    /// # Arguments
    ///
    /// * `segs` - The segments of the path
    /// * `fg_col` - The foreground color by which the path is stroked
    /// * `bg_col` - The background color by which the path is filled
    ///
    /// # Panics
    ///
    /// This function panics if the vector graphics cannot be written to a file.
    ///
    pub fn path(
        self: &mut Self,
        segs: &[DrawDirective],
        fg_col: &Option<Color>,
        bg_col: &Option<Color>,
    ) -> () {
        write!(self.output_file, "<path ").expect("Error at writing file");
        match (fg_col, bg_col, self.force_colors) {
            (Some(color), _, _) => {
                write!(self.output_file, "stroke=\"#{}\" ", color.to_svg())
                    .expect("Error at writing file");
            }
            (None, Some(_), _) => {
                write!(self.output_file, "stroke=\"none\" ").expect("Error at writing file");
            }
            (None, None, true) => {
                write!(self.output_file, "stroke=\"black\" ").expect("Error at writing file");
            }
            (None, None, false) => {}
        }
        match (bg_col, self.force_colors) {
            (Some(color), _) => {
                write!(self.output_file, "fill=\"#{}\" ", color.to_svg())
                    .expect("Error at writing file");
            }
            (None, true) => {
                write!(self.output_file, "fill=\"none\" ").expect("Error at writing file");
            }
            (None, false) => {}
        }
        write!(self.output_file, "d=\"").expect("Error at writing file");
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
        write!(self.output_file, "\" />").expect("Error at writing file");
    }
}

/*
Copyright 2023-2023 Andreas Warnke

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
