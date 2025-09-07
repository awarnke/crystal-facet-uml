//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use super::shape::get_rounded_rect_abs;
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

/// red line color
static RED: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0x3f,
    blue: 0x3f,
};

/// red pen
static RED_PEN: geometry::Pen = geometry::Pen {
    color: RED,
    width: 1.0,
};

/// red bold pen
static RED_THICK_PEN: geometry::Pen = geometry::Pen {
    color: RED,
    width: 2.0,
};

/// yellow line color
static YELLOW: geometry::Color = geometry::Color {
    red: 0xee,
    green: 0xee,
    blue: 0x00,
};

/// yellow pen
static YELLOW_PEN: geometry::Pen = geometry::Pen {
    color: YELLOW,
    width: 1.0,
};

/// yellow bold pen
static YELLOW_THICK_PEN: geometry::Pen = geometry::Pen {
    color: YELLOW,
    width: 2.0,
};

/// blue line color
static BLUE: geometry::Color = geometry::Color {
    red: 0x3f,
    green: 0x3f,
    blue: 0xff,
};

/// blue pen
static BLUE_PEN: geometry::Pen = geometry::Pen {
    color: BLUE,
    width: 1.0,
};

/// blue bold pen
static BLUE_THICK_PEN: geometry::Pen = geometry::Pen {
    color: BLUE,
    width: 2.0,
};

/// The function generates a message_error icon
///
pub fn generate_message_error(out: &mut dyn PathRenderer) -> () {
    let icon_shape: [geometry::DrawDirective; 9] = [
        Move(Point {
            x: CX - 4.0,
            y: CY - 12.0,
        }),
        Line(Point {
            x: CX + 4.0,
            y: CY - 12.0,
        }),
        Line(Point {
            x: CX + 12.0,
            y: CY - 4.0,
        }),
        Line(Point {
            x: CX + 12.0,
            y: CY + 4.0,
        }),
        Line(Point {
            x: CX + 4.0,
            y: CY + 12.0,
        }),
        Line(Point {
            x: CX - 4.0,
            y: CY + 12.0,
        }),
        Line(Point {
            x: CX - 12.0,
            y: CY + 4.0,
        }),
        Line(Point {
            x: CX - 12.0,
            y: CY - 4.0,
        }),
        Close,
    ];
    out.render_path(&icon_shape, &Some(RED_THICK_PEN), &None);

    let icon_mark: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 2.0 - HALFLINE,
            top: CY - 8.0 - HALFLINE,
            width: 5.0,
            height: 17.0,
        },
        2.5,
    );
    out.render_path(&icon_mark, &Some(RED_PEN), &None);
}

/// The function generates a message_info icon
///
pub fn generate_message_info(out: &mut dyn PathRenderer) -> () {
    let icon_shape: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY }, 13.0, 13.0);
    out.render_path(&icon_shape, &Some(BLUE_THICK_PEN), &None);

    let icon_mark_dot: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY - 6.0 }, 2.0 + HALFLINE, 2.0 + HALFLINE);
    out.render_path(&icon_mark_dot, &Some(BLUE_PEN), &None);

    let icon_mark: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 2.0 - HALFLINE,
            top: CY - HALFLINE,
            width: 5.0,
            height: 9.0,
        },
        2.0 + HALFLINE,
    );
    out.render_path(&icon_mark, &Some(BLUE_PEN), &None);
}

/// The function generates a message_user_doc icon
///
pub fn generate_message_user_doc(out: &mut dyn PathRenderer) -> () {
    let icon_object: [geometry::DrawDirective; 7] = [
        Move(Point {
            x: CX - 14.0,
            y: CY - 4.0,
        }),
        Curve(
            Point {
                x: CX - 9.0,
                y: CY - 6.0,
            },
            Point {
                x: CX - 3.0,
                y: CY - 4.0,
            },
            Point { x: CX, y: CY - 1.0 },
        ),
        Curve(
            Point {
                x: CX + 3.0,
                y: CY - 4.0,
            },
            Point {
                x: CX + 9.0,
                y: CY - 6.0,
            },
            Point {
                x: CX + 14.0,
                y: CY - 4.0,
            },
        ),
        Line(Point {
            x: CX + 14.0,
            y: CY + 6.0,
        }),
        Curve(
            Point {
                x: CX + 9.0,
                y: CY + 4.0,
            },
            Point {
                x: CX + 3.0,
                y: CY + 6.0,
            },
            Point { x: CX, y: CY + 9.0 },
        ),
        Curve(
            Point {
                x: CX - 3.0,
                y: CY + 6.0,
            },
            Point {
                x: CX - 9.0,
                y: CY + 4.0,
            },
            Point {
                x: CX - 14.0,
                y: CY + 6.0,
            },
        ),
        Close,
    ];
    out.render_path(&icon_object, &Some(BLACK_THICK_PEN), &Some(WHITE));
}

/// The function generates a message_warn icon
///
pub fn generate_message_warn(out: &mut dyn PathRenderer) -> () {
    let icon_shape: [geometry::DrawDirective; 7] = [
        Move(Point {
            x: CX - 4.0,
            y: CY - 10.0,
        }),
        Curve(
            Point {
                x: CX - 2.0,
                y: CY - 13.0,
            },
            Point {
                x: CX + 2.0,
                y: CY - 13.0,
            },
            Point {
                x: CX + 4.0,
                y: CY - 10.0,
            },
        ),
        Line(Point {
            x: CX + 13.0,
            y: CY + 5.0,
        }),
        Curve(
            Point {
                x: CX + 15.0,
                y: CY + 9.0,
            },
            Point {
                x: CX + 12.0,
                y: CY + 12.0,
            },
            Point {
                x: CX + 9.0,
                y: CY + 12.0,
            },
        ),
        Line(Point {
            x: CX - 9.0,
            y: CY + 12.0,
        }),
        Curve(
            Point {
                x: CX - 12.0,
                y: CY + 12.0,
            },
            Point {
                x: CX - 15.0,
                y: CY + 9.0,
            },
            Point {
                x: CX - 13.0,
                y: CY + 5.0,
            },
        ),
        Close,
    ];
    out.render_path(&icon_shape, &Some(YELLOW_THICK_PEN), &None);

    let icon_feature: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 2.0 - HALFLINE,
            top: CY - 7.0 - HALFLINE,
            width: 5.0,
            height: 16.0,
        },
        2.5,
    );
    out.render_path(&icon_feature, &Some(YELLOW_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "message_error",
            viewport: ICON_VIEW_RECT,
            generate: generate_message_error,
        },
        IconSource {
            name: "message_info",
            viewport: ICON_VIEW_RECT,
            generate: generate_message_info,
        },
        IconSource {
            name: "message_user_doc",
            viewport: ICON_VIEW_RECT,
            generate: generate_message_user_doc,
        },
        IconSource {
            name: "message_warn",
            viewport: ICON_VIEW_RECT,
            generate: generate_message_warn,
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
