//! The module provides functions to render an icon to vector graphics.

use super::geometry;
use super::geometry::Color;
use super::geometry::DrawDirective::Close;
use super::geometry::DrawDirective::Curve;
use super::geometry::DrawDirective::Line;
use super::geometry::DrawDirective::Move;
use super::geometry::Point;
use crate::render::render_svg::VecRenderer;

/// Defines the data needed to render an icon
pub struct IconSource<'my_lifespan> {
    /// The name
    pub name: &'my_lifespan str,
    /// The generator function
    pub generate: fn(out: &mut VecRenderer) -> (),
}

/// The function defines the control points for a circle
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
fn get_circle(cx: f32, cy: f32, rx: f32, ry: f32) -> [geometry::DrawDirective; 5] {
    let ctrlpntd_x: f32 = rx * 0.55; /* control point distance x */
    let ctrlpntd_y: f32 = ry * 0.55; /* control point distance y */
    [
        Move(Point { x: cx - rx, y: cy }),
        Curve(
            Point {
                x: cx - rx,
                y: cy - ctrlpntd_y,
            },
            Point {
                x: cx - ctrlpntd_x,
                y: cy - ry,
            },
            Point { x: cx, y: cy - ry },
        ),
        Curve(
            Point {
                x: cx + ctrlpntd_x,
                y: cy - ry,
            },
            Point {
                x: cx + rx,
                y: cy - ctrlpntd_y,
            },
            Point { x: cx + rx, y: cy },
        ),
        Curve(
            Point {
                x: cx + rx,
                y: cy + ctrlpntd_y,
            },
            Point {
                x: cx + ctrlpntd_x,
                y: cy + ry,
            },
            Point { x: cx, y: cy + ry },
        ),
        Curve(
            Point {
                x: cx - ctrlpntd_x,
                y: cy + ry,
            },
            Point {
                x: cx - rx,
                y: cy + ctrlpntd_y,
            },
            Point { x: cx - rx, y: cy },
        ),
    ]
}

/// The function generated a GSN goal icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_goal(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 1.0, y: 7.0 }),
        Line(Point { x: 30.0, y: 7.0 }),
        Line(Point { x: 30.0, y: 25.0 }),
        Line(Point { x: 1.0, y: 25.0 }),
        Close,
    ];
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &Some(icon_color));
}

/// The function generated a GSN goal icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_context(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 8.0, y: 25.0 }),
        Curve(
            Point { x: 0.0, y: 25.0 },
            Point { x: 0.0, y: 7.0 },
            Point { x: 8.0, y: 7.0 },
        ),
        Line(Point { x: 24.0, y: 7.0 }),
        Curve(
            Point { x: 32.0, y: 7.0 },
            Point { x: 32.0, y: 25.0 },
            Point { x: 24.0, y: 25.0 },
        ),
        Close,
    ];
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &Some(icon_color));
}

/// The function generated a GSN strategy icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_strategy(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 7.0, y: 7.0 }),
        Line(Point { x: 31.0, y: 7.0 }),
        Line(Point { x: 25.0, y: 25.0 }),
        Line(Point { x: 1.0, y: 25.0 }),
        Close,
    ];
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &Some(icon_color));
}

/// The function generated a GSN goal icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_assumption(out: &mut VecRenderer) -> () {
    /* ellipsis */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(16.0, 10.0, 15.0, 8.0);
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &Some(icon_color));

    /* A-character */
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 25.0, y: 25.0 }),
        Line(Point { x: 27.0, y: 19.0 }),
        Line(Point { x: 29.0, y: 25.0 }),
        Move(Point { x: 25.4, y: 23.5 }),
        Line(Point { x: 28.6, y: 23.5 }),
    ];
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &Some(icon_color));
}

/// The function generated a GSN goal icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_justification(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(16.0, 10.0, 15.0, 8.0);
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &Some(icon_color));

    /* J-character */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 25.0, y: 24.0 }),
        Curve(
            Point { x: 25.0, y: 26.0 },
            Point { x: 29.0, y: 26.0 },
            Point { x: 29.0, y: 24.0 },
        ),
        Line(Point { x: 29.0, y: 19.0 }),
        Line(Point { x: 25.0, y: 19.0 }),
    ];
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &Some(icon_color));
}

/// The function generated a GSN goal icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_solution(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(16.0, 16.0, 15.0, 15.0);
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &Some(icon_color));
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "gsn_goal",
            generate: generate_gsn_goal,
        },
        IconSource {
            name: "gsn_context",
            generate: generate_gsn_context,
        },
        IconSource {
            name: "gsn_strategy",
            generate: generate_gsn_strategy,
        },
        IconSource {
            name: "gsn_assumption",
            generate: generate_gsn_assumption,
        },
        IconSource {
            name: "gsn_justification",
            generate: generate_gsn_justification,
        },
        IconSource {
            name: "gsn_solution",
            generate: generate_gsn_solution,
        },
    ]
}
