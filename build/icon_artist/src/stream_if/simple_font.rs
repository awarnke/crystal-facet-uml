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
    let descend = (0.38 * font_size).floor();  /* smaller fibonacci */
    let ascend = font_size - descend;
    for codepoint in text.chars() {
        match codepoint {
            'E' => {
                let char_width: f32 = (0.62 * ascend).ceil();
                let char_halfascend: f32 = (0.62 * ascend).floor();
                let icon_segs: [geometry::DrawDirective; 6] = [
                    Move(Point {
                        x: base_start.x - halfline,
                        y: base_start.y - char_width,
                    }),
                    Line(Point {
                        x: base_start.x - halfline,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - ascend + halfline,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - ascend + halfline,
                        y: base_start.y - char_width,
                    }),
                    Move(Point {
                        x: base_start.x - char_halfascend - halfline,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - char_halfascend - halfline,
                        y: base_start.y - char_width + 1.0,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - char_width - 2.0;
            }
            'F' => {
                let char_width: f32 = (0.62 * ascend).ceil();
                let char_halfascend: f32 = (0.62 * ascend).floor();
                let icon_segs: [geometry::DrawDirective; 5] = [
                    Move(Point {
                        x: base_start.x,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - ascend + halfline,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - ascend + halfline,
                        y: base_start.y - char_width,
                    }),
                    Move(Point {
                        x: base_start.x - char_halfascend - halfline,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - char_halfascend - halfline,
                        y: base_start.y - char_width + 1.0,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - char_width - 2.0;
            }
            'I' => {
                let char_width: f32 = (0.38 * ascend).ceil();
                let icon_segs: [geometry::DrawDirective; 2] = [
                    Move(Point {
                        x: base_start.x,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - ascend,
                        y: base_start.y - halfline,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - char_width - 2.0;
            }
            'L' => {
                let char_width: f32 = (0.62 * ascend).ceil();
                let icon_segs: [geometry::DrawDirective; 3] = [
                    Move(Point {
                        x: base_start.x - halfline,
                        y: base_start.y - char_width,
                    }),
                    Line(Point {
                        x: base_start.x - halfline,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x - ascend,
                        y: base_start.y - halfline,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - char_width - 2.0;
            }
            'V' => {
                let char_width: f32 = (0.62 * ascend).ceil();
                let char_halfwidth: f32 = (0.5 * char_width).ceil();
                let icon_segs: [geometry::DrawDirective; 3] = [
                    Move(Point {
                        x: base_start.x - ascend,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x,
                        y: base_start.y - char_halfwidth + halfline,
                    }),
                    Line(Point {
                        x: base_start.x - ascend,
                        y: base_start.y - char_width + halfline,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - char_width - 2.0;
            }
            'W' => {
                let char_width: f32 = ascend;
                let char_halfwidth: f32 = (0.5 * char_width).ceil();
                let char_quarterwidth: f32 = (0.2 * char_width).ceil();
                let char_halfascend: f32 = (0.62 * ascend).floor();
                let icon_segs: [geometry::DrawDirective; 5] = [
                    Move(Point {
                        x: base_start.x - ascend,
                        y: base_start.y - halfline,
                    }),
                    Line(Point {
                        x: base_start.x,
                        y: base_start.y - char_halfwidth + char_quarterwidth + halfline,
                    }),
                    Line(Point {
                        x: base_start.x - char_halfascend,
                        y: base_start.y - char_halfwidth + halfline,
                    }),
                    Line(Point {
                        x: base_start.x,
                        y: base_start.y - char_halfwidth - char_quarterwidth + halfline,
                    }),
                    Line(Point {
                        x: base_start.x - ascend,
                        y: base_start.y - char_width + halfline,
                    }),
                ];
                out.render_path(&icon_segs, &Some(stroke), &None);
                base_start.y = base_start.y - char_width - 2.0;
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
