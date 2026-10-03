//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::Curve;
use crate::stream_if::geometry::DrawDirective::CurveRel;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::DrawDirective::MoveRel;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;
use crate::stream_if::simple_font;

/// The view rectangle of each icon
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 32.0,
};

/// The view rectangle of section icons
const SECT_ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 12.0,
    height: 32.0,
};

/// half line width
const HALFLINE: f32 = 0.5;

/// section icon font baseline and size
const SECT_BASELINE_X: f32 = 10.0;
const SECT_BASELINE_Y: f32 = 30.0;
const SECT_FONT_SIZE: f32 = 12.0;

/// gray line color
static GRAY: geometry::Color = geometry::Color {
    red: 0x7f,
    green: 0x7f,
    blue: 0x7f,
};

/// light gray fill color
static SILVER: geometry::Color = geometry::Color {
    red: 0xe0,
    green: 0xe0,
    blue: 0xe0,
};

/// white color
static WHITE: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0xff,
    blue: 0xff,
};

/// gray pen
static GRAY_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 1.0,
};

/// gray thick pen
static GRAY_THICK_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 2.0,
};

/// The function generates a separator label
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_view_sect(out: &mut dyn PathRenderer) -> () {
    simple_font::draw_string_upwards(
        "VIEW",
        Point {
            x: SECT_BASELINE_X,
            y: SECT_BASELINE_Y,
        },
        GRAY_PEN,
        SECT_FONT_SIZE,
        out,
    );
}

/// The function defines the draw directives for the view mode background
///
/// # Arguments
///
/// * `pos` - Position of the view button: 0..3
///
fn get_view_ground(pos: i32) -> [geometry::DrawDirective; 5] {
    let left_y = 18.0 + (((pos * 4) - 7) as f32).abs();
    let right_y = 18.0 + (((pos * 4) - 5) as f32).abs();
    let middle_y = left_y.min(right_y);
    [
        Move(Point { x: 0.0, y: left_y }),
        Curve(
            Point {
                x: 10.0,
                y: middle_y,
            },
            Point {
                x: 22.0,
                y: middle_y,
            },
            Point {
                x: 32.0,
                y: right_y,
            },
        ),
        Line(Point { x: 32.0, y: 32.0 }),
        Line(Point { x: 0.0, y: 32.0 }),
        Close,
    ]
}

/// The function generates a new window icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_view_new_window(out: &mut dyn PathRenderer) -> () {
    let center_x: f32 = 16.0;
    let center_y: f32 = 16.0;

    /* window */
    let titlebar_segs: [geometry::DrawDirective; 2] = [
        MoveRel(Offset {
            dx: 1.0,
            dy: 7.0,
        }),
        LineRel(Offset { dx: 30.0, dy: 0.0 }),
    ];
    out.render_path(&titlebar_segs, &Some(GRAY_THICK_PEN), &None);
    let border_segs: [geometry::DrawDirective; 4] = [
        MoveRel(Offset {
            dx: 31.0 - HALFLINE,
            dy: 7.0,
        }),
        LineRel(Offset { dx: 0.0, dy: 18.0 - HALFLINE }),
        LineRel(Offset { dx: -29.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: -18.0 + HALFLINE }),
    ];
    out.render_path(&border_segs, &Some(GRAY_PEN), &None);

    /* plus symbol */
    let plus_sym: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: center_x - 4.0,
            y: center_y,
        }),
        Line(Point {
            x: center_x + 4.0,
            y: center_y,
        }),
        Move(Point {
            x: center_x,
            y: center_y - 4.0,
        }),
        Line(Point {
            x: center_x,
            y: center_y + 4.0,
        }),
    ];
    out.render_path(&plus_sym, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_view_search(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 5] = get_view_ground(0);
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 19.0, y: 12.0 }, 7.0, 7.0);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);

    let icon_segs: [geometry::DrawDirective; 2] = [
        MoveRel(Offset { dx: 16.5, dy: 18.75 }),
        LineRel(Offset { dx: -4.5, dy: 11.25 }),
    ];
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_view_search_now(out: &mut dyn PathRenderer) -> () {
    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 19.0, y: 12.0 }, 7.0, 7.0);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &Some(WHITE));

    let icon_segs: [geometry::DrawDirective; 2] = [
        MoveRel(Offset { dx: 16.5, dy: 18.75 }),
        LineRel(Offset { dx: -4.5, dy: 11.25 }),
    ];
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &Some(SILVER));
}

/// The function generates a steering wheel of ship navigation to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_view_navigate(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 5] = get_view_ground(1);
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* spoke of wheel */
    let r2: f32 = 15.0;
    let r1: f32 = 3.0;
    let cx: f32 = 16.0;
    let cy: f32 = 16.0;
    let mut icon_segs: [geometry::DrawDirective; 20] = [Close; 20];
    for index in 0..10 {
        let alpha: f32 = std::f32::consts::PI / 5.0 * (index as f32);
        let dx = alpha.cos();
        let dy = alpha.sin();
        icon_segs[index * 2 + 0] = Move(Point { x: cx + r1 * dx, y: cy + r1 * dy });
        icon_segs[index * 2 + 1] = Line(Point {
            x: cx + r2 * dx,
            y: cy + r2 * dy,
        });
    }
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);

    /* axle of wheel */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: cx, y: cy }, 2.0 + HALFLINE, 2.0 + HALFLINE);
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);

    /* rim of wheel */
    let icon_segs: [geometry::DrawDirective; 5] =
    get_circle_abs(Point { x: cx, y: cy }, 11.0, 11.0);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a hand icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_view_edit(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 5] = get_view_ground(2);
    out.render_path(&icon_segs, &None, &Some(WHITE));

    let icon_segs: [geometry::DrawDirective; 16] = [
        MoveRel(Offset {
            dx: 12.945591,
            dy: 29.3417,
        }),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: 0.513702,
                dy: -3.916,
            },
            Offset {
                dx: -0.09381,
                dy: -5.347,
            },
        ),
        CurveRel(
            Offset {
                dx: -0.607512,
                dy: -1.4311,
            },
            Offset {
                dx: -1.838417,
                dy: -0.5705,
            },
            Offset {
                dx: -2.814258,
                dy: -2.8143,
            },
        ),
        CurveRel(
            Offset {
                dx: -0.9758418,
                dy: -2.2438,
            },
            Offset {
                dx: -1.627765,
                dy: -10.592,
            },
            Offset {
                dx: -0.3752347,
                dy: -10.6942,
            },
        ),
        CurveRel(
            Offset {
                dx: 1.2525297,
                dy: -0.1022,
            },
            Offset {
                dx: 1.9699807,
                dy: 7.5985,
            },
            Offset {
                dx: 1.9699807,
                dy: 7.5985,
            },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: -0.374371,
                dy: -13.7912,
            },
            Offset {
                dx: 1.031895,
                dy: -13.696,
            },
        ),
        CurveRel(
            Offset {
                dx: 1.406266,
                dy: 0.095,
            },
            Offset {
                dx: 1.219512,
                dy: 11.0694,
            },
            Offset {
                dx: 1.219512,
                dy: 11.0694,
            },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: 0.516942,
                dy: -12.5406,
            },
            Offset {
                dx: 1.594746,
                dy: -12.5704,
            },
        ),
        CurveRel(
            Offset {
                dx: 1.077804,
                dy: -0.03,
            },
            Offset {
                dx: 1.031896,
                dy: 12.2889,
            },
            Offset {
                dx: 1.031896,
                dy: 12.2889,
            },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: 0.613873,
                dy: -10.7697,
            },
            Offset {
                dx: 1.594746,
                dy: -10.4127,
            },
        ),
        CurveRel(
            Offset {
                dx: 0.980873,
                dy: 0.357,
            },
            Offset {
                dx: 0.75047,
                dy: 11.4446,
            },
            Offset {
                dx: 0.75047,
                dy: 11.4446,
            },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: 0.16798,
                dy: -7.6202,
            },
            Offset {
                dx: 1.219511,
                dy: -7.6923,
            },
        ),
        CurveRel(
            Offset {
                dx: 1.051531,
                dy: -0.072,
            },
            Offset {
                dx: 1.266578,
                dy: 8.5419,
            },
            Offset {
                dx: 0.938087,
                dy: 11.0695,
            },
        ),
        CurveRel(
            Offset {
                dx: -0.328491,
                dy: 2.5275,
            },
            Offset {
                dx: -1.151965,
                dy: 2.3082,
            },
            Offset {
                dx: -1.40713,
                dy: 3.9399,
            },
        ),
        CurveRel(
            Offset {
                dx: -0.255165,
                dy: 1.6317,
            },
            Offset {
                dx: 0.187616,
                dy: 5.7223,
            },
            Offset {
                dx: 0.187616,
                dy: 5.7223,
            },
        ),
        Close,
    ];
    out.render_path(&icon_segs, &Some(GRAY_PEN), &Some(SILVER));
}

/// The function generates a growing plant icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_view_create(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 5] = get_view_ground(3);
    out.render_path(&icon_segs, &None, &Some(WHITE));

    let icon_segs: [geometry::DrawDirective; 15] = [
        MoveRel(Offset {
            dx: 15.97781,
            dy: 30.0081,
        }),
        LineRel(Offset {
            dx: 0.07836,
            dy: -5.2683,
        }),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: 0.178263,
                dy: -3.5108,
            },
            Offset {
                dx: 4.217509,
                dy: -5.3197,
            },
        ),
        CurveRel(
            Offset {
                dx: 4.039247,
                dy: -1.8089,
            },
            Offset {
                dx: 4.002077,
                dy: 0.5815,
            },
            Offset {
                dx: 1.061699,
                dy: 2.775,
            },
        ),
        CurveRel(
            Offset {
                dx: -2.940378,
                dy: 2.1934,
            },
            Offset {
                dx: -5.506003,
                dy: 2.665,
            },
            Offset {
                dx: -5.506003,
                dy: 2.665,
            },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: -0.730453,
                dy: -4.9278,
            },
            Offset {
                dx: -4.028792,
                dy: -6.6615,
            },
        ),
        CurveRel(
            Offset {
                dx: -3.2983386,
                dy: -1.7337,
            },
            Offset {
                dx: -2.9281036,
                dy: 0.5468,
            },
            Offset {
                dx: -0.8382,
                dy: 3.2441,
            },
        ),
        CurveRel(
            Offset {
                dx: 2.089904,
                dy: 2.6973,
            },
            Offset {
                dx: 4.945349,
                dy: 3.2033,
            },
            Offset {
                dx: 4.945349,
                dy: 3.2033,
            },
        ),
        LineRel(Offset {
            dx: -0.172165,
            dy: -9.6788,
        }),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: -0.172379,
                dy: -1.7078,
            },
            Offset {
                dx: -1.995904,
                dy: -2.934,
            },
        ),
        CurveRel(
            Offset {
                dx: -1.823526,
                dy: -1.2263,
            },
            Offset {
                dx: -3.108662,
                dy: -0.2453,
            },
            Offset {
                dx: -1.428661,
                dy: 1.6621,
            },
        ),
        CurveRel(
            Offset {
                dx: 1.680001,
                dy: 1.9075,
            },
            Offset {
                dx: 3.494646,
                dy: 1.0572,
            },
            Offset {
                dx: 3.494646,
                dy: 1.0572,
            },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 0.0 },
            Offset {
                dx: -0.758347,
                dy: -1.9473,
            },
            Offset {
                dx: 2.025709,
                dy: -3.7533,
            },
        ),
        CurveRel(
            Offset {
                dx: 2.784055,
                dy: -1.806,
            },
            Offset {
                dx: 2.369392,
                dy: 0.7466,
            },
            Offset {
                dx: 0.713489,
                dy: 2.1585,
            },
        ),
        CurveRel(
            Offset {
                dx: -1.655904,
                dy: 1.4118,
            },
            Offset {
                dx: -2.981447,
                dy: 1.7423,
            },
            Offset {
                dx: -2.981447,
                dy: 1.7423,
            },
        ),
    ];
    out.render_path(&icon_segs, &Some(GRAY_PEN), &Some(SILVER));
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "view_sect",
            viewport: SECT_ICON_VIEW_RECT,
            generate: generate_view_sect,
        },
        IconSource {
            name: "view_new_window",
            viewport: ICON_VIEW_RECT,
            generate: generate_view_new_window,
        },
        IconSource {
            name: "view_search",
            viewport: ICON_VIEW_RECT,
            generate: generate_view_search,
        },
        IconSource {
            name: "view_search_now",
            viewport: ICON_VIEW_RECT,
            generate: generate_view_search_now,
        },
        IconSource {
            name: "view_navigate",
            viewport: ICON_VIEW_RECT,
            generate: generate_view_navigate,
        },
        IconSource {
            name: "view_edit",
            viewport: ICON_VIEW_RECT,
            generate: generate_view_edit,
        },
        IconSource {
            name: "view_create",
            viewport: ICON_VIEW_RECT,
            generate: generate_view_create,
        },
    ]
}

/*
 * Copyright 2023-2026 Andreas Warnke
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
