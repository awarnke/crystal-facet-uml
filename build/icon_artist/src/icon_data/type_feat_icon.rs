//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use super::shape::get_rect_abs;
use super::shape::get_rounded_rect_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
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
    height: 24.0,
};

/// icon center x
const CX: f32 = 16.0;
/// icon center y
const CY: f32 = 12.0;

/// half line width
const HALFLINE: f32 = 0.5;

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

/// The function generates a type_feat_property
///
pub fn generate_type_feat_property(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX + HALFLINE,
            y: 8.0,
        }),
        Line(Point {
            x: CX + HALFLINE,
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
            x: CX + HALFLINE,
            y: 22.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + HALFLINE,
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
    let icon_object: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX + HALFLINE,
            y: 14.0,
        }),
        Line(Point {
            x: CX + HALFLINE,
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

/// The function defines the draw directives for the name label
///
fn get_rect_with_opening(gap: f32) -> [geometry::DrawDirective; 6] {
    [
        Move(Point {
            x: CX + HALFLINE,
            y: CY - gap,
        }),
        Line(Point {
            x: CX + HALFLINE,
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
            x: CX + HALFLINE,
            y: 22.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + HALFLINE,
            y: CY + gap,
        }),
    ]
}

/// The function defines the draw directives for the name label
///
fn get_rounded_rect_with_opening(gap: f32) -> [geometry::DrawDirective; 10] {
    [
        Move(Point {
            x: CX + HALFLINE,
            y: CY - gap,
        }),
        Line(Point {
            x: CX + HALFLINE,
            y: 4.5 + HALFLINE,
        }),
        Curve(
            Point {
                x: CX + HALFLINE,
                y: 2.5 + HALFLINE,
            },
            Point {
                x: CX + 1.5 + HALFLINE,
                y: 1.0 + HALFLINE,
            },
            Point {
                x: CX + 3.5 + HALFLINE,
                y: 1.0 + HALFLINE,
            },
        ),
        Line(Point {
            x: 26.5 + HALFLINE,
            y: 1.0 + HALFLINE,
        }),
        Curve(
            Point {
                x: 28.5 + HALFLINE,
                y: 1.0 + HALFLINE,
            },
            Point {
                x: 30.0 + HALFLINE,
                y: 2.5 + HALFLINE,
            },
            Point {
                x: 30.0 + HALFLINE,
                y: 4.5 + HALFLINE,
            },
        ),
        Line(Point {
            x: 30.0 + HALFLINE,
            y: 18.5 + HALFLINE,
        }),
        Curve(
            Point {
                x: 30.0 + HALFLINE,
                y: 20.5 + HALFLINE,
            },
            Point {
                x: 28.5 + HALFLINE,
                y: 22.0 + HALFLINE,
            },
            Point {
                x: 26.5 + HALFLINE,
                y: 22.0 + HALFLINE,
            },
        ),
        Line(Point {
            x: CX + 3.5 + HALFLINE,
            y: 22.0 + HALFLINE,
        }),
        Curve(
            Point {
                x: CX + 1.5 + HALFLINE,
                y: 22.0 + HALFLINE,
            },
            Point {
                x: CX + HALFLINE,
                y: 20.5 + HALFLINE,
            },
            Point {
                x: CX + HALFLINE,
                y: 18.5 + HALFLINE,
            },
        ),
        Line(Point {
            x: CX + HALFLINE,
            y: CY + gap,
        }),
    ]
}

/// The function generates a type_feat_entry
///
pub fn generate_type_feat_entry(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 10] = get_rounded_rect_with_opening(5.0);
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    /* port */
    let icon_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX + 1.0, y: CY }, 4.0, 4.0);
    out.render_path(&icon_circle, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_feat_exit
///
pub fn generate_type_feat_exit(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 10] = get_rounded_rect_with_opening(5.0);
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    /* port */
    let icon_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX + 1.0, y: CY }, 4.0, 4.0);
    out.render_path(&icon_circle, &Some(GRAY_THICK_PEN), &None);

    let icon_feature: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 1.0,
            y: CY - 2.0,
        }),
        Line(Point {
            x: CX + 3.0,
            y: CY + 2.0,
        }),
        Move(Point {
            x: CX + 3.0,
            y: CY - 2.0,
        }),
        Line(Point {
            x: CX - 1.0,
            y: CY + 2.0,
        }),
    ];
    out.render_path(&icon_feature, &Some(GRAY_PEN), &None);
}

/// The function generates a type_feat_port
///
pub fn generate_type_feat_port(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 6] = get_rect_with_opening(3.0);
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    /* port */
    let icon_feature: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 2.0,
        top: CY - 3.0,
        width: 6.0,
        height: 6.0,
    });
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_feat_provide
///
pub fn generate_type_feat_provide(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 16.0 + HALFLINE,
        top: 1.0 + HALFLINE,
        width: 14.0,
        height: 21.0,
    });
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    /* port */
    let icon_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 6.0, y: CY }, 4.0, 4.0);
    out.render_path(&icon_circle, &Some(GRAY_THICK_PEN), &None);
    let icon_feature: [geometry::DrawDirective; 2] =
        [Move(Point { x: 11.0, y: CY }), Line(Point { x: CX, y: CY })];
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_feat_require
///
pub fn generate_type_feat_require(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 16.0 + HALFLINE,
        top: 1.0 + HALFLINE,
        width: 14.0,
        height: 21.0,
    });
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    /* port */
    let icon_feature: [geometry::DrawDirective; 5] = [
        Move(Point {
            x: 6.0,
            y: CY - 4.0,
        }),
        Curve(
            Point {
                x: 8.2,
                y: CY - 4.0,
            },
            Point {
                x: 10.0,
                y: CY - 2.2,
            },
            Point { x: 10.0, y: CY },
        ),
        Curve(
            Point {
                x: 10.0,
                y: CY + 2.2,
            },
            Point {
                x: 8.2,
                y: CY + 4.0,
            },
            Point {
                x: 6.0,
                y: CY + 4.0,
            },
        ),
        Move(Point { x: 11.0, y: CY }),
        Line(Point { x: CX, y: CY }),
    ];
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_feat_life
///
pub fn generate_type_feat_life(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 5.0 - HALFLINE,
        top: 1.0 + HALFLINE,
        width: 11.0,
        height: 4.0,
    });
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    /* line */
    let icon_feature: [geometry::DrawDirective; 6] = [
        Move(Point { x: CX, y: 8.0 }),
        Line(Point { x: CX, y: 11.0 }),
        Move(Point { x: CX, y: 14.0 }),
        Line(Point { x: CX, y: 17.0 }),
        Move(Point { x: CX, y: 20.0 }),
        Line(Point { x: CX, y: 23.0 }),
    ];
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_feat_rx
///
pub fn generate_type_feat_rx(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 6] = get_rect_with_opening(3.0);
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    /* port */
    let icon_feature: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 2.0,
        top: CY - 3.0,
        width: 6.0,
        height: 6.0,
    });
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);

    /* arrow */
    let icon_feature: [geometry::DrawDirective; 5] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: CX - 5.0, y: CY }),
        Line(Point {
            x: CX - 11.0,
            y: CY - 4.5,
        }),
        Move(Point { x: CX - 5.0, y: CY }),
        Line(Point {
            x: CX - 11.0,
            y: CY + 4.5,
        }),
    ];
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_feat_tx
///
pub fn generate_type_feat_tx(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 6] = get_rect_with_opening(3.0);
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    /* port */
    let icon_feature: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 2.0,
        top: CY - 3.0,
        width: 6.0,
        height: 6.0,
    });
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);

    /* arrow */
    let icon_feature: [geometry::DrawDirective; 5] = [
        Move(Point { x: CX - 4.0, y: CY }),
        Line(Point { x: 2.0, y: CY }),
        Line(Point {
            x: 8.0,
            y: CY - 4.5,
        }),
        Move(Point { x: 2.0, y: CY }),
        Line(Point {
            x: 8.0,
            y: CY + 4.5,
        }),
    ];
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_feat_tag
///
pub fn generate_type_feat_tag(out: &mut dyn PathRenderer) -> () {
    /* tag */
    let icon_feature: [geometry::DrawDirective; 7] = [
        Move(Point { x: 11.25, y: 12.0 }),
        Line(Point { x: 7.0, y: 8.75 }),
        Line(Point { x: 7.0, y: 5.25 }),
        Line(Point { x: 11.25, y: 2.0 }),
        Line(Point { x: 30.0, y: 2.0 }),
        Line(Point { x: 30.0, y: 12.0 }),
        Line(Point { x: 13.0, y: 12.0 }),
    ];
    out.render_path(&icon_feature, &Some(GRAY_THICK_PEN), &None);

    /* thread */
    let icon_feature: [geometry::DrawDirective; 5] = [
        Move(Point {
            x: 2.0 + HALFLINE,
            y: 21.0,
        }),
        Line(Point {
            x: 7.0 + HALFLINE,
            y: 11.0,
        }),
        Move(Point {
            x: 11.0,
            y: 7.0 - HALFLINE,
        }),
        Line(Point {
            x: 10.0 + HALFLINE,
            y: 7.0,
        }),
        Curve(
            Point { x: 14.0, y: 14.0 },
            Point { x: 13.0, y: 16.0 },
            Point {
                x: 8.0 + HALFLINE,
                y: 20.0 + HALFLINE,
            },
        ),
    ];
    out.render_path(&icon_feature, &Some(GRAY_PEN), &None);
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
        IconSource {
            name: "type_feat_entry",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_entry,
        },
        IconSource {
            name: "type_feat_exit",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_exit,
        },
        IconSource {
            name: "type_feat_port",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_port,
        },
        IconSource {
            name: "type_feat_provide",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_provide,
        },
        IconSource {
            name: "type_feat_require",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_require,
        },
        IconSource {
            name: "type_feat_life",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_life,
        },
        IconSource {
            name: "type_feat_rx",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_rx,
        },
        IconSource {
            name: "type_feat_tx",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_tx,
        },
        IconSource {
            name: "type_feat_tag",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_feat_tag,
        },
    ]
}

/*
 * Copyright 2025-2026 Andreas Warnke
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
