//! The module provides functions to generate svg files.

use crate::model::geometry::Color;
use crate::model::geometry::DrawDirective;
use crate::model::geometry::Point;
use crate::model::geometry::Rect;
use std::fs::File;
use std::io::Write;

/// Defines a vector renderer
pub struct VecRenderer<'my_lifespan> {
    /// The left x-coordinate
    pub output_file: &'my_lifespan mut File,
    /// The default font size in points
    pub font_size: f32,
}

/// The Rect struct provides some methods
impl<'my_lifespan> VecRenderer<'my_lifespan> {
    /// The function header converts the vector graphics drawing directive header to svg format
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

            ",
            view.left, view.top, view.width, view.height
        )
        .expect("Error at writing file");
    }

    /// The function header converts the vector graphics drawing directive footer to svg format
    pub(super) fn footer(self: &mut Self) -> () {
        write!(self.output_file, "\n</svg>\n").expect("Error at writing file");
    }

    /// The function header converts the vector graphics drawing directive text to svg format
    pub(super) fn text(self: &mut Self, start: &Point, text: &str, col: &Color) -> () {
        write!(
            self.output_file,
            "\
            <text
                x=\"{}\"
                y=\"{}\"
                font-family=\"-apple-system, system-ui, BlinkMacSystemFont, Roboto\"
                dominant-baseline=\"auto\"
                text-anchor=\"middle\"
                font-size=\"{}\"
                fill=\"#{}\"
                font-weight=\"100\"
                >
                {}
            </text>
            ",
            start.x,
            start.y,
            self.font_size,
            col.to_svg(),
            text,
        )
        .expect("Error at writing file");
    }

    /// The function header converts the vector graphics drawing directive path to svg format
    pub fn path(self: &mut Self, segs: &[DrawDirective], col: &Color) -> () {
        write!(
            self.output_file,
            "\
            <path
                stroke=\"#{}\" fill=\"none\"
                d=\"",
            col.to_svg()
        )
        .expect("Error at writing file");
        for seg in segs {
            match seg {
                DrawDirective::Move(target) => {
                    write!(self.output_file, "M {},{} ", target.x, target.y)
                        .expect("Error at writing file");
                }
                DrawDirective::Line(target) => {
                    write!(self.output_file, "L {},{} ", target.x, target.y)
                        .expect("Error at writing file");
                }
                DrawDirective::Curve(p1, p2, target) => {
                    write!(
                        self.output_file,
                        "C {},{} {},{} {},{}",
                        p1.x, p1.y, p2.x, p2.y, target.x, target.y
                    )
                    .expect("Error at writing file");
                }
                DrawDirective::Close => {
                    write!(self.output_file, "Z ").expect("Error at writing file");
                }
            }
        }
        write!(self.output_file, "\"\n            />\n            ")
            .expect("Error at writing file");
    }

    /// The function header converts the vector graphics drawing directive rect to svg format
    pub(super) fn rect_fill(self: &mut Self, rect: &Rect, col: &Color) -> () {
        write!(
            self.output_file,
            "\
            <rect
                x=\"{}\"
                y=\"{}\"
                width=\"{}\"
                height=\"{}\"
                rx=\"3\"
                fill=\"#{}\"
            />
            ",
            rect.left,
            rect.top,
            rect.width,
            rect.height,
            col.to_svg()
        )
        .expect("Error at writing file");
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
