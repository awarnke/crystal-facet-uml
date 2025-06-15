//! The module provides functions to render an icon to vector graphics.

use super::shape::get_rect_abs;
use super::shape::get_rounded_rect_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;

/// The view rectangle of each icon
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 24.0,
};

/// gray line color
static GRAY: geometry::Color = geometry::Color {
    red: 0x7f,
    green: 0x7f,
    blue: 0x7f,
};

/// half line width
const HALFLINE: f32 = 0.5;

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

/// The function generates a type_feat_property
///
pub fn generate_type_feat_property(out: &mut dyn PathRenderer) -> () {
    /* object */
    /*
    let icon_object: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 16.0 + HALFLINE,
        top: 1.0 + HALFLINE,
        width: 14.0,
        height: 21.0,
    });
    */
    let icon_object: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: 16.0 + HALFLINE,
            y: 8.0,
        }),
        Line(Point {
            x: 16.0 + HALFLINE,
            y: 1.0 + HALFLINE,
        }),
        Line(Point {
            x: 30.0 + HALFLINE,
            y: 1.0 + HALFLINE,
        }),
        Line(Point {
            x: 30.0 + HALFLINE,
            y: 22.0 + HALFLINE,
        }),
        Line(Point {
            x: 16.0 + HALFLINE,
            y: 22.0 + HALFLINE,
        }),
        Line(Point {
            x: 16.0 + HALFLINE,
            y: 17.0,
        }),
    ];
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    let icon_feature: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 15.0,
        top: 9.0,
        width: 16.0,
        height: 7.0,
    });
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_feat_operation
///
pub fn generate_type_feat_operation(out: &mut dyn PathRenderer) -> () {
    /* object */
    /*
    let icon_object: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 16.0 + HALFLINE,
        top: 1.0 + HALFLINE,
        width: 14.0,
        height: 21.0,
    });
    */
    let icon_object: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: 16.0 + HALFLINE,
            y: 14.0,
        }),
        Line(Point {
            x: 16.0 + HALFLINE,
            y: 1.0 + HALFLINE,
        }),
        Line(Point {
            x: 30.0 + HALFLINE,
            y: 1.0 + HALFLINE,
        }),
        Line(Point {
            x: 30.0 + HALFLINE,
            y: 14.0,
        }),
    ];
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    let icon_feature: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: 15.0,
            top: 15.0,
            width: 16.0,
            height: 7.0,
        },
        3.0,
    );
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "type_feat_property",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_property,
        },
        IconSource {
            name: "type_feat_operation",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_operation,
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
