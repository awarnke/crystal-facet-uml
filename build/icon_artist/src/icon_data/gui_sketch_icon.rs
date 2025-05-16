//! The module provides functions to render an icon to vector graphics.

use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::Curve;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::DrawDirective::MoveRel;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;
use crate::icon_data::shape::get_rect_abs;

/// The view rectangle of each icon
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 16.0,
};

/// gray color
static WHITE: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0xff,
    blue: 0xff,
};

/// black color
static BLACK: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0x0,
    blue: 0x0,
};

/// black pen
static BLACK_PEN: geometry::Pen = geometry::Pen {
    color: BLACK,
    width: 1.0,
};

/// The function generates an arrow to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_relate(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 14] = [
        Move(Point { x: 18.0, y: 16.0 }),
        Line(Point { x: 18.0, y: 6.5 }),
        Curve(
            Point { x: 18.0, y: 4.5 },
            Point { x: 20.5, y: 2.0 },
            Point { x: 22.5, y: 2.0 },
        ),
        Line(Point { x: 26.0, y: 2.0 }),
        Line(Point { x: 26.0, y: 0.0 }),
        Line(Point { x: 27.0, y: 0.0 }),
        Line(Point { x: 32.0, y: 3.5 }),
        Line(Point { x: 27.0, y: 7.0 }),
        Line(Point { x: 26.0, y: 7.0 }),
        Line(Point { x: 26.0, y: 5.0 }),
        Line(Point { x: 23.5, y: 5.0 }),
        Curve(
            Point { x: 22.0, y: 5.0 },
            Point { x: 21.0, y: 6.5 },
            Point { x: 21.0, y: 8.5 },
        ),
        Line(Point { x: 21.0, y: 16.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 12] = [
        Move(Point { x: 19.0, y: 15.0 }),
        Line(Point { x: 19.0, y: 7.0 }),
        Curve(
            Point { x: 19.0, y: 5.0 },
            Point { x: 21.0, y: 3.0 },
            Point { x: 23.0, y: 3.0 },
        ),
        Line(Point { x: 27.0, y: 3.0 }),
        Line(Point { x: 27.0, y: 1.0 }),
        Line(Point { x: 30.5, y: 3.5 }),
        Line(Point { x: 27.0, y: 6.0 }),
        Line(Point { x: 27.0, y: 4.0 }),
        Line(Point { x: 23.5, y: 4.0 }),
        Curve(
            Point { x: 21.5, y: 4.0 },
            Point { x: 20.0, y: 5.5 },
            Point { x: 20.0, y: 7.5 },
        ),
        Line(Point { x: 20.0, y: 15.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK));
}

/// The function generates a flower image to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_refine(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 5] =
    get_rect_abs(Rect{left:0.0,top:0.0,width:15.0,height:16.0});
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* item list and plus sign */
    let icon_segs: [geometry::DrawDirective; 16] = [
        Move(Point { x: 1.0, y: 1.5 }),
        LineRel(Offset { dx: 2.0, dy: 0.0 }),
        MoveRel(Offset { dx: 2.0, dy: 0.0 }),
        LineRel(Offset { dx: 9.0, dy: 0.0 }),
        Move(Point { x: 1.0, y: 3.5 }),
        LineRel(Offset { dx: 2.0, dy: 0.0 }),
        MoveRel(Offset { dx: 2.0, dy: 0.0 }),
        LineRel(Offset { dx: 9.0, dy: 0.0 }),
        Move(Point { x: 1.0, y: 5.5 }),
        LineRel(Offset { dx: 2.0, dy: 0.0 }),
        MoveRel(Offset { dx: 2.0, dy: 0.0 }),
        LineRel(Offset { dx: 9.0, dy: 0.0 }),
        Move(Point { x: 4.0, y: 11.5 }),
        LineRel(Offset { dx: 7.0, dy: 0.0 }),
        MoveRel(Offset { dx: -3.5, dy: -3.5 }),
        LineRel(Offset { dx: 0.0, dy: 7.0 }),
    ];
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);

    generate_sketch_relate(out);
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_create(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 5] =
    get_rect_abs(Rect{left:8.0,top:0.0,width:15.0,height:15.0});
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* item list and plus sign */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 9.0, y: 7.5 }),
        LineRel(Offset { dx: 13.0, dy: 0.0 }),
        MoveRel(Offset { dx: -6.5, dy: -6.5 }),
        LineRel(Offset { dx: 0.0, dy: 13.0 }),
    ];
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "sketch_relate",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_relate,
        },
        IconSource {
            name: "sketch_refine",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_refine,
        },
        IconSource {
            name: "sketch_create",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_create,
        },
    ]
}

/*
Copyright 2025-2025 Andreas Warnke

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
