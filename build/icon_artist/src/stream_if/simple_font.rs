//! The module transform provides a couple of simple 2D transformations.

use super::path_renderer::PathRenderer;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::Pen;
use crate::stream_if::geometry::Point;

/// The function moves a slice of points by an offset
///
/// # Arguments
///
/// * `points` - The absolute coordinates of a slice of points
/// * `offset` - The offset by which to move the point
///
pub fn draw_string_upwards(
    text: &str,
    base_line_start: Point,
    stroke: Pen,
    font_size: f32,
    out: &mut dyn PathRenderer,
) -> () {
    let halfline = stroke.width / 2.0;
    let mut base_start = base_line_start;
    for codepoint in text.chars() {
        match codepoint {
            'F' => {
                let char_f_y: f32 = base_start.y;
                let char_width: f32 = (0.6 * font_size).ceil();
                let char_halfheight: f32 = (0.5 * font_size).ceil();
                let icon_segs: [geometry::DrawDirective; 5] = [
                    Move(Point {
                        x: base_start.x,
                        y: char_f_y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - font_size + halfline,
                        y: char_f_y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - font_size + halfline,
                        y: char_f_y - char_width,
                    }),
                    Move(Point {
                        x: base_start.x - char_halfheight - halfline,
                        y: char_f_y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - char_halfheight - halfline,
                        y: char_f_y - char_width + 1.0,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - font_size;
            }
            'I' => {
                let char_i_y: f32 = base_start.y;
                let icon_segs: [geometry::DrawDirective; 2] = [
                    Move(Point {
                        x: base_start.x,
                        y: char_i_y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - font_size,
                        y: char_i_y - halfline,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - 0.5 * font_size;
            }
            'L' => {
                let char_l_y: f32 = base_start.y;
                let char_width: f32 = (0.6 * font_size).ceil();
                let icon_segs: [geometry::DrawDirective; 3] = [
                    Move(Point {
                        x: base_start.x - halfline,
                        y: char_l_y - char_width,
                    }),
                    Line(Point {
                        x: base_start.x - halfline,
                        y: char_l_y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - font_size,
                        y: char_l_y - halfline,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - font_size;
            }
            'E' => {
                let char_e_y: f32 = base_start.y;
                let char_width: f32 = (0.6 * font_size).ceil();
                let char_halfheight: f32 = (0.5 * font_size).ceil();
                let icon_segs: [geometry::DrawDirective; 6] = [
                    Move(Point {
                        x: base_start.x - halfline,
                        y: char_e_y - char_width,
                    }),
                    Line(Point {
                        x: base_start.x - halfline,
                        y: char_e_y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - font_size + halfline,
                        y: char_e_y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - font_size + halfline,
                        y: char_e_y - char_width,
                    }),
                    Move(Point {
                        x: base_start.x - char_halfheight - halfline,
                        y: char_e_y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - char_halfheight - halfline,
                        y: char_e_y - char_width + 1.0,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - font_size;
            }
            _ => {}
        }
    }
}

/*
Copyright 2026-2026 Andreas Warnke

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
