//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::CloseRel;
use crate::stream_if::geometry::DrawDirective::Curve;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::DrawDirective::MoveRel;
use crate::stream_if::geometry::Offset;
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

/// gray pen
static GRAY_THICK_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 2.0,
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

/// pink color
static PINK: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0x3c,
    blue: 0xd2,
};

/// pink thick pen
static PINK_THICK_PEN: geometry::Pen = geometry::Pen {
    color: PINK,
    width: 2.0,
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

/// The function defines the pink marker icon directives
///
fn get_pink_marker() -> [geometry::DrawDirective; 2] {
    [
        MoveRel(Offset { dx: 1.0, dy: 30.0 }),
        LineRel(Offset { dx: 30.0, dy: 0.0 }),
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

/// The function generates an edit/cut icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_edit_cut(out: &mut dyn PathRenderer) -> () {
    let left_finger_sym: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 15.5, y: 24.5 }, 3.0, 3.0);
    out.render_path(&left_finger_sym, &Some(BLACK_PEN), &None);

    let right_finger_sym: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 23.5, y: 21.5 }, 3.0, 3.0);
    out.render_path(&right_finger_sym, &Some(BLACK_PEN), &None);

    let scissors_sym: [geometry::DrawDirective; 11] = [
        MoveRel(Offset { dx: 16.0, dy: 21.5 }),
        LineRel(Offset {
            dx: 0.375,
            dy: -4.75,
        }),
        LineRel(Offset {
            dx: -3.5,
            dy: -2.125,
        }),
        LineRel(Offset {
            dx: -5.0,
            dy: -8.125,
        }),
        LineRel(Offset { dx: 9.0, dy: 8.375 }),
        LineRel(Offset {
            dx: -0.625,
            dy: -12.375,
        }),
        LineRel(Offset {
            dx: 2.875,
            dy: 9.25,
        }),
        LineRel(Offset {
            dx: -0.625,
            dy: 4.25,
        }),
        LineRel(Offset {
            dx: 3.375,
            dy: 3.125,
        }),
        LineRel(Offset {
            dx: -3.875,
            dy: -1.625,
        }),
        CloseRel,
    ];
    out.render_path(&scissors_sym, &Some(BLACK_PEN), &None);

    let pink_line: [geometry::DrawDirective; 2] = get_pink_marker();
    out.render_path(&pink_line, &Some(PINK_THICK_PEN), &None);
}

/// The function defines the draw directives for the page contour
///
fn get_page_contour( left: f32, top: f32 ) -> [geometry::DrawDirective; 8] {
    [
        MoveRel(Offset { dx: left + 6.0, dy: top + 19.0 }),
        LineRel(Offset {
            dx: 5.0,
            dy: -5.0,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: -14.0,
        }),
        LineRel(Offset {
            dx: -11.0,
            dy: 0.0,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: 19.0,
        }),
        LineRel(Offset { dx: 6.0, dy: 0.0 }),
        LineRel(Offset {
            dx: 1.0,
            dy: -4.0,
        }),
        LineRel(Offset {
            dx: 4.0,
            dy: -1.0,
        }),
    ]
}

/// The function generates an edit/copy icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_edit_copy(out: &mut dyn PathRenderer) -> () {
    let left_page_sym: [geometry::DrawDirective; 8] = get_page_contour( 3.0 + HALFLINE, 4.0 + HALFLINE );
    out.render_path(&left_page_sym, &Some(BLACK_PEN), &None);

    let right_page_sym: [geometry::DrawDirective; 8] = get_page_contour( 17.0 + HALFLINE, 4.0 + HALFLINE );
    out.render_path(&right_page_sym, &Some(BLACK_PEN), &None);

    let pink_line: [geometry::DrawDirective; 2] = get_pink_marker();
    out.render_path(&pink_line, &Some(PINK_THICK_PEN), &None);
}

/// The function generates an edit/paste icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_edit_paste(out: &mut dyn PathRenderer) -> () {
    let left_page_sym: [geometry::DrawDirective; 8] = get_page_contour( 10.0 + HALFLINE, 4.0 + HALFLINE );
    out.render_path(&left_page_sym, &Some(BLACK_PEN), &None);

    let board_sym: [geometry::DrawDirective; 6] = [
        MoveRel(Offset { dx: 7.0 + HALFLINE, dy: 26.0 }),
        LineRel(Offset {
            dx: -1.0,
            dy: -2.0,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: -22.0 - HALFLINE,
        }),
        LineRel(Offset {
            dx: 19.0,
            dy: 0.0,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: 22.0 + HALFLINE,
        }),
        LineRel(Offset {
            dx: -1.0,
            dy: 2.0,
        }),
    ];
    out.render_path(&board_sym, &Some(BLACK_PEN), &None);

    let pink_line: [geometry::DrawDirective; 2] = get_pink_marker();
    out.render_path(&pink_line, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates an edit/reset icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_edit_reset(out: &mut dyn PathRenderer) -> () {
    let pink_line: [geometry::DrawDirective; 2] = get_pink_marker();
    out.render_path(&pink_line, &Some(GRAY_THICK_PEN), &None);
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
        IconSource {
            name: "edit_cut",
            viewport: ICON_VIEW_RECT,
            generate: generate_edit_cut,
        },
        IconSource {
            name: "edit_copy",
            viewport: ICON_VIEW_RECT,
            generate: generate_edit_copy,
        },
        IconSource {
            name: "edit_paste",
            viewport: ICON_VIEW_RECT,
            generate: generate_edit_paste,
        },
        IconSource {
            name: "edit_reset",
            viewport: ICON_VIEW_RECT,
            generate: generate_edit_reset,
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
