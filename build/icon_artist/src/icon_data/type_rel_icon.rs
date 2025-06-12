//! The module provides functions to render an icon to vector graphics.

use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective;
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

/// icon center y
const CY: f32 = 12.0;

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

/// The function defines the path  points for a rectangle in relative offsets
///
/// # Arguments
///
/// * `top_left` - The relative offset of the top left corner
/// * `width` - The width of the rectangle
/// * `height` - The height of the rectangle
///
fn get_small_paw(center: Point) -> [DrawDirective; 11] {
    [
        DrawDirective::Move(Point {
            x: center.x,
            y: center.y - 1.0,
        }),
        DrawDirective::Line(Point {
            x: center.x - 1.5,
            y: center.y,
        }),
        DrawDirective::Line(Point {
            x: center.x,
            y: center.y + 1.0,
        }),
        DrawDirective::Move(Point {
            x: center.x - 1.0,
            y: center.y,
        }),
        DrawDirective::Line(Point {
            x: center.x + 0.2,
            y: center.y,
        }),
        DrawDirective::Move(Point {
            x: center.x + 1.0,
            y: center.y - 1.5,
        }),
        DrawDirective::Line(Point {
            x: center.x + 2.0,
            y: center.y - 2.0,
        }),
        DrawDirective::Move(Point {
            x: center.x + 1.5,
            y: center.y - 0.0,
        }),
        DrawDirective::Line(Point {
            x: center.x + 2.5,
            y: center.y - 0.0,
        }),
        DrawDirective::Move(Point {
            x: center.x + 1.0,
            y: center.y + 1.5,
        }),
        DrawDirective::Line(Point {
            x: center.x + 2.0,
            y: center.y + 2.0,
        }),
    ]
}

/// The function generates a type_rel_refine
///
pub fn generate_type_rel_refine(out: &mut dyn PathRenderer) -> () {
    let icon_segs_4_ends: [geometry::DrawDirective; 5] = [
        Move(Point {
            x: 20.0,
            y: CY ,
        }),
        Line(Point {
            x: 10.0,
             y: CY ,
        }),
        Move(Point {
            x: 1.0,
             y: CY - 5.0 ,
        }),
        Curve(
            Point { x: 4.0, y: CY - 5.0  },
            Point {
                x: 6.0,
                y: CY - 3.0 ,
            },
            Point { x: 6.0, y: CY  },
        ),
        Curve(
            Point { x: 6.0, y: CY + 3.0  },
            Point {
                x: 4.0,
              y: CY + 5.0 ,
            },
            Point { x: 1.0, y: CY + 5.0  },
        ),
    ];
    out.render_path(&icon_segs_4_ends, &Some(GRAY_THICK_PEN), &None);

    let arrot_tip: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: 22.0,
             y: CY - 9.0 ,
        }),
        Line(
            Point { x: 31.0, y: CY  },
        ),
        Line(
            Point {
                x: 22.0,
             y: CY + 9.0  ,
            },
        ),
    ];
    out.render_path(&arrot_tip, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_trace
///
pub fn generate_type_rel_trace(out: &mut dyn PathRenderer) -> () {
    let paw_1 = get_small_paw(Point {
        x: 2.0,
        y: CY - 2.0 - HALFLINE,
    });
    out.render_path(&paw_1, &Some(GRAY_PEN), &None);
    let paw_2 = get_small_paw(Point {
        x: 7.0,
        y: CY + 2.0 - HALFLINE,
    });
    out.render_path(&paw_2, &Some(GRAY_PEN), &None);
    let paw_3 = get_small_paw(Point {
        x: 12.0,
        y: CY - 2.0 - HALFLINE,
    });
    out.render_path(&paw_3, &Some(GRAY_PEN), &None);
    let paw_4 = get_small_paw(Point {
        x: 17.0,
        y: CY + 2.0 - HALFLINE,
    });
    out.render_path(&paw_4, &Some(GRAY_PEN), &None);

    let arrot_tip: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: 22.0,
             y: CY - 9.0 - HALFLINE,
        }),
        Line(
            Point { x: 31.0, y: CY - HALFLINE },
        ),
        Line(
            Point {
                x: 22.0,
              y: CY + 9.0  - HALFLINE,
            },
        ),
    ];
    out.render_path(&arrot_tip, &Some(GRAY_THICK_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "type_rel_refine",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_refine,
        },
        IconSource {
            name: "type_rel_trace",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_trace,
        },
    ]
}
