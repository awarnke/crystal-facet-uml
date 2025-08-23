//! The module provides functions to render an icon to vector graphics.

use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::Curve;
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
    height: 32.0,
};

/// icon center x
const CX: f32 = 16.0;
/// icon center y
const CY: f32 = 16.0;

/// green fill color
static GREEN: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0xff,
    blue: 0x99,
};

/// gray color
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

/// half line width
const HALFLINE: f32 = 0.5;

/// The function defines the draw directives for the broken_circle contour
///
fn get_broken_circle_contour() -> [geometry::DrawDirective; 4] {
    [
        Move(Point {
            x: CX - 10.6,
            y: CY - 10.6,
        }),
        Curve(
            Point {
                x: CX - 16.6,
                y: CY - 4.6,
            },
            Point {
                x: CX - 16.6,
                y: CY + 4.6,
            },
            Point {
                x: CX - 10.6,
                y: CY + 10.6,
            },
        ),
        Move(Point {
            x: CX + 10.6,
            y: CY + 10.6,
        }),
        Curve(
            Point {
                x: CX + 16.6,
                y: CY + 4.6,
            },
            Point {
                x: CX + 16.6,
                y: CY - 4.6,
            },
            Point {
                x: CX + 10.6,
                y: CY - 10.6,
            },
        ),
    ]
}

/// The function defines the draw directives for the time_glass contour
///
fn get_time_glass() -> [geometry::DrawDirective; 9] {
    [
        Move(Point {
            x: CX - 1.0 - HALFLINE,
            y: CY,
        }),
        Curve(
            Point {
                x: CX - 1.0 - HALFLINE,
                y: CY - 4.0,
            },
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY - 5.0,
            },
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY - 9.0,
            },
        ),
        Curve(
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY - 12.0,
            },
            Point {
                x: CX - 5.0,
                y: CY - 14.0 - HALFLINE,
            },
            Point {
                x: CX,
                y: CY - 14.0 - HALFLINE,
            },
        ),
        Curve(
            Point {
                x: CX + 5.0,
                y: CY - 14.0 - HALFLINE,
            },
            Point {
                x: CX + 7.0 + HALFLINE,
                y: CY - 12.0,
            },
            Point {
                x: CX + 7.0 + HALFLINE,
                y: CY - 9.0,
            },
        ),
        Curve(
            Point {
                x: CX + 7.0 + HALFLINE,
                y: CY - 5.0,
            },
            Point {
                x: CX + 1.0 + HALFLINE,
                y: CY - 4.0,
            },
            Point {
                x: CX + 1.0 + HALFLINE,
                y: CY,
            },
        ),
        Curve(
            Point {
                x: CX + 1.0 + HALFLINE,
                y: CY + 4.0,
            },
            Point {
                x: CX + 7.0 + HALFLINE,
                y: CY + 5.0,
            },
            Point {
                x: CX + 7.0 + HALFLINE,
                y: CY + 9.0,
            },
        ),
        Curve(
            Point {
                x: CX + 7.0 + HALFLINE,
                y: CY + 12.0,
            },
            Point {
                x: CX + 5.0,
                y: CY + 14.0 + HALFLINE,
            },
            Point {
                x: CX,
                y: CY + 14.0 + HALFLINE,
            },
        ),
        Curve(
            Point {
                x: CX - 5.0,
                y: CY + 14.0 + HALFLINE,
            },
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY + 12.0,
            },
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY + 9.0,
            },
        ),
        Curve(
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY + 5.0,
            },
            Point {
                x: CX - 1.0 - HALFLINE,
                y: CY + 4.0,
            },
            Point {
                x: CX - 1.0 - HALFLINE,
                y: CY,
            },
        ),
    ]
}

/// The function generates an edit/undo icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_edit_undo(out: &mut dyn PathRenderer) -> () {
    /* background */
    let broken_circle_sym: [geometry::DrawDirective; 4] = get_broken_circle_contour();
    out.render_path(&broken_circle_sym, &Some(GRAY_PEN), &None);

    /* green sand */
    let sand_sym: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 1.0 - HALFLINE,
            y: CY,
        }),
        Curve(
            Point {
                x: CX - 1.0 - HALFLINE,
                y: CY - 4.0,
            },
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY - 5.0,
            },
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY - 10.0,
            },
        ),
        Line(Point {
            x: CX,
            y: CY - 9.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 7.0 + HALFLINE,
            y: CY - 10.0,
        }),
        Curve(
            Point {
                x: CX + 7.0 + HALFLINE,
                y: CY - 5.0,
            },
            Point {
                x: CX + 1.0 + HALFLINE,
                y: CY - 4.0,
            },
            Point {
                x: CX + 1.0 + HALFLINE,
                y: CY,
            },
        ),
        Close,
    ];
    out.render_path(&sand_sym, &None, &Some(GREEN));

    /* time glass symbol */
    let time_glass_sym: [geometry::DrawDirective; 9] = get_time_glass();
    out.render_path(&time_glass_sym, &Some(BLACK_PEN), &None);
}

/// The function generates an edit/undo icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_edit_redo(out: &mut dyn PathRenderer) -> () {
    /* background */
    let broken_circle_sym: [geometry::DrawDirective; 4] = get_broken_circle_contour();
    out.render_path(&broken_circle_sym, &Some(GRAY_PEN), &None);

    /* green sand */
    let sand_sym: [geometry::DrawDirective; 5] = [
        Move(Point {
            x: CX + 7.0 + HALFLINE,
            y: CY + 9.0,
        }),
        Curve(
            Point {
                x: CX + 7.0 + HALFLINE,
                y: CY + 12.0,
            },
            Point {
                x: CX + 5.0,
                y: CY + 14.0 + HALFLINE,
            },
            Point {
                x: CX,
                y: CY + 14.0 + HALFLINE,
            },
        ),
        Curve(
            Point {
                x: CX - 5.0,
                y: CY + 14.0 + HALFLINE,
            },
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY + 12.0 + HALFLINE,
            },
            Point {
                x: CX - 7.0 - HALFLINE,
                y: CY + 9.0,
            },
        ),
        Line(Point {
            x: CX,
            y: CY + 7.0 - HALFLINE,
        }),
        Close,
    ];
    out.render_path(&sand_sym, &None, &Some(GREEN));

    /* time glass symbol */
    let time_glass_sym: [geometry::DrawDirective; 9] = get_time_glass();
    out.render_path(&time_glass_sym, &Some(BLACK_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "edit_undo",
            viewport: ICON_VIEW_RECT,
            generate: generate_edit_undo,
        },
        IconSource {
            name: "edit_redo",
            viewport: ICON_VIEW_RECT,
            generate: generate_edit_redo,
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
