//! The module provides functions to render an icon to vector graphics.

use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::DrawDirective::Curve;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;

/// The view rectangle of each icon
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 24.0,
    height: 24.0,
};

/// half line width
const HALFLINE: f32 = 0.5;

/// gray line color
static GRAY: geometry::Color = geometry::Color {
    red: 0x7f,
    green: 0x7f,
    blue: 0x7f,
};

/// gray bold pen
static GRAY_THICK_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 2.0,
};

/// red line color
static RED: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0x3f,
    blue: 0x3f,
};

/// red bold pen
static RED_THICK_PEN: geometry::Pen = geometry::Pen {
    color: RED,
    width: 2.0,
};

/// green color
static GREEN: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0xaa,
    blue: 0x0,
};

/// The function generates a 5_avoid icon
///
pub fn generate_5_avoid(out: &mut dyn PathRenderer) -> () {
    let icon_avoid: [geometry::DrawDirective; 4] = [
        Move(Point { x: 2.0, y: 2.0 }),
        Line(Point { x: 22.0, y: 22.0 }),
        Move(Point { x: 2.0, y: 22.0 }),
        Line(Point { x: 22.0, y: 2.0 }),
    ];
    out.render_path(&icon_avoid, &Some(RED_THICK_PEN), &None);
}

/// The function generates a 5_improve icon
///
pub fn generate_5_improve(out: &mut dyn PathRenderer) -> () {
    let icon_improve: [geometry::DrawDirective; 12] = [
        Move(Point { x: 2.0, y: 2.0 }),
        Line(Point { x: 2.0, y: 7.0 }),
        Curve(Point { x: 2.0, y: 14.0 },Point { x: 3.0, y: 15.0 },Point { x: 10.0, y: 15.0 }),
        Line(Point { x: 14.0, y: 15.0 }),
        Line(Point { x: 14.0, y: 19.0 }),
        Line(Point { x: 22.0, y: 13.0 }),
        Line(Point { x: 14.0, y: 7.0 }),
        Line(Point { x: 14.0, y: 11.0 }),
        Line(Point { x: 10.0, y: 11.0 }),
        Curve(Point { x: 6.5, y: 11.0 },Point { x: 6.0, y: 10.5 },Point { x: 6.0, y: 7.0 }),
        Line(Point { x: 6.0, y: 2.0 }),
        Close,
    ];
    out.render_path(&icon_improve, &None, &Some(GREEN) );
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "5_avoid",
            viewport: ICON_VIEW_RECT,
            generate: generate_5_avoid,
        },
        IconSource {
            name: "5_improve",
            viewport: ICON_VIEW_RECT,
            generate: generate_5_improve,
        },
    ]
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
