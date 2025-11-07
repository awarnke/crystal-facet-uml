//! The module provides functions to render an icon to vector graphics.

use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;
use crate::stream_if::transform::move_points;
use crate::stream_if::transform::scale_points;
use crate::stream_if::transform::rotate_points;

/// The view rectangle of each icon
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 32.0,
};

/// icon center x
const CX: f32 = 16.0;
/// icon center y
const CY: f32 = 16.0;

/// half line width
const HALFLINE: f32 = 0.5;

/// black line color
static BLACK: geometry::Color = geometry::Color {
    red: 0x00,
    green: 0x00,
    blue: 0x00,
};

/// black bold pen
static BLACK_THICK_PEN: geometry::Pen = geometry::Pen {
    color: BLACK,
    width: 2.0,
};

/// white color
static WHITE: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0xff,
    blue: 0xff,
};

/// green color
static LIGHT_GREEN: geometry::Color = geometry::Color {
    red: 0xdd,
    green: 0xff,
    blue: 0xdd,
};

/// gray line color
static GRAY: geometry::Color = geometry::Color {
    red: 0x7f,
    green: 0x7f,
    blue: 0x7f,
};

/// gray pen
static GRAY_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 1.0,
};

/// gray bold pen
static GRAY_THICK_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 2.0,
};

/// The function generates a crystal_facet_uml icon
///
pub fn generate_crystal_facet_uml(out: &mut dyn PathRenderer) -> () {
    /* prepare */
    let deg60 = std::f32::consts::PI / 3.0;
    let six_points: [Point; 6] = [
        Point { x: 1.0, y: 0.0 },
        Point {
            x: deg60.cos(),
            y: deg60.sin(),
        },
        Point {
            x: -(deg60.cos()),
            y: deg60.sin(),
        },
        Point { x: -1.0, y: 0.0 },
        Point {
            x: -(deg60.cos()),
            y: -(deg60.sin()),
        },
        Point {
            x: deg60.cos(),
            y: -(deg60.sin()),
        },
    ];
    let small_rot = std::f32::consts::PI / -33.0;
    let big_rot = std::f32::consts::PI / -20.0;

    /* define a crystal at origin */
    let mut top_points: [Point; 6] = six_points;
    rotate_points(&mut top_points,small_rot);
    move_points(&mut top_points, Offset { dx: 0.0, dy: -2.2 });
    let mut mid_points: [Point; 6] = six_points;
    rotate_points(&mut mid_points,small_rot);
    let mut floor_point: [Point; 1] = [Point { x: 0.0, y: 18.5 },];

    scale_points(&mut top_points, 7.0, 2.6 );
    scale_points(&mut mid_points, 10.5, 3.9 );

    /* spin around */

    rotate_points(&mut top_points,big_rot);
    rotate_points(&mut mid_points,big_rot);
    rotate_points(&mut floor_point,big_rot);

    move_points(&mut top_points, Offset { dx: CX, dy: CY-6.0 });
    move_points(&mut mid_points, Offset { dx: CX, dy: CY-6.0 });
    move_points(&mut floor_point, Offset { dx: CX, dy: CY-6.0 });

    let icon_shape: [geometry::DrawDirective; 22] = [
        Move(top_points[0]),
        Line(top_points[1]),
        Line(top_points[2]),
        Line(top_points[3]),
        Line(top_points[4]),
        Line(top_points[5]),
        Close,
        Line(mid_points[0]),
        Move(top_points[1]),
        Line(mid_points[1]),
        Move(top_points[2]),
        Line(mid_points[2]),
        Move(top_points[3]),
        Line(mid_points[3]),
        Line(mid_points[2]),
        Line(mid_points[1]),
        Line(mid_points[0]),
        Line(floor_point[0]),
        Line(mid_points[3]),
        Move(mid_points[1]),
        Line(floor_point[0]),
        Line(mid_points[2]),
    ];
    out.render_path(&icon_shape, &Some(GRAY_PEN), &None);

}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[IconSource {
        name: "crystal_facet_uml",
        viewport: ICON_VIEW_RECT,
        generate: generate_crystal_facet_uml,
    }]
}

/*
 * Copyright 2025-2025 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
