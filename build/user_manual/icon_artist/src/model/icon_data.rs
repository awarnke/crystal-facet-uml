//! The module provides functions to render an icon to vector graphics.

use super::geometry;
use super::geometry::Color;
use super::geometry::DrawDirective::Close;
use super::geometry::DrawDirective::Curve;
use super::geometry::DrawDirective::Line;
use super::geometry::DrawDirective::Move;
use super::geometry::DrawDirective::Symmetric;
use super::geometry::Point;
use crate::render::render_svg::VecRenderer;

/// Defines the data needed to render an icon
pub struct IconSource<'my_lifespan> {
    /// The name
    pub name: &'my_lifespan str,
    /// The generator function
    pub generate: fn(out: &mut VecRenderer) -> (),
}

/// black color
static BLUE: Color = Color {
    red: 0x0,
    green: 0x0,
    blue: 0xa0,
};

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
        Symmetric(
            Point {
                x: cx + rx,
                y: cy - ctrlpntd_y,
            },
            Point { x: cx + rx, y: cy },
        ),
        Symmetric(
            Point {
                x: cx + ctrlpntd_x,
                y: cy + ry,
            },
            Point { x: cx, y: cy + ry },
        ),
        Symmetric(
            Point {
                x: cx - rx,
                y: cy + ctrlpntd_y,
            },
            Point { x: cx - rx, y: cy },
        ),
    ]
}

/// The function generates a GSN goal icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_goal(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 1.0, y: 7.0 }),
        Line(Point { x: 31.0, y: 7.0 }),
        Line(Point { x: 31.0, y: 25.0 }),
        Line(Point { x: 1.0, y: 25.0 }),
        Close,
    ];
    out.path(&icon_segs, &Some(BLUE), &None);
}

/// The function generates a GSN context icon to vector graphics drawing directives
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
    out.path(&icon_segs, &Some(BLUE), &None);
}

/// The function generates a GSN strategy icon to vector graphics drawing directives
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
    out.path(&icon_segs, &Some(BLUE), &None);
}

/// The function generates a GSN assumption icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_assumption(out: &mut VecRenderer) -> () {
    /* ellipsis */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(16.0, 10.0, 15.0, 8.0);
    out.path(&icon_segs, &Some(BLUE), &None);

    /* A-character */
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 24.0, y: 25.0 }),
        Line(Point { x: 26.5, y: 19.0 }),
        Line(Point { x: 29.0, y: 25.0 }),
        Move(Point { x: 24.75, y: 23.5 }),
        Line(Point { x: 28.25, y: 23.5 }),
    ];
    out.path(&icon_segs, &Some(BLUE), &None);
}

/// The function generates a GSN justification icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_justification(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(16.0, 10.0, 15.0, 8.0);
    out.path(&icon_segs, &Some(BLUE), &None);

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
    out.path(&icon_segs, &Some(BLUE), &None);
}

/// The function generates a GSN solution icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_gsn_solution(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(16.0, 16.0, 15.0, 15.0);
    out.path(&icon_segs, &Some(BLUE), &None);
}

/// The function generates a server icon of queueing theory to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_queue_server(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(16.0, 16.0, 15.0, 15.0);
    out.path(&icon_segs, &None, &None);
}

/// The function generates a buffer icon of queueing theory to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_queue_buffer(out: &mut VecRenderer) -> () {
    /* box */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 1.0, y: 8.0 }),
        Line(Point { x: 31.0, y: 8.0 }),
        Line(Point { x: 31.0, y: 24.0 }),
        Line(Point { x: 1.0, y: 24.0 }),
    ];
    out.path(&icon_segs, &None, &None);

    /* 3 elements */
    let icon_segs: [geometry::DrawDirective; 6] = [
        Move(Point { x: 13.0, y: 8.0 }),
        Line(Point { x: 13.0, y: 24.0 }),
        Move(Point { x: 19.0, y: 8.0 }),
        Line(Point { x: 19.0, y: 24.0 }),
        Move(Point { x: 25.0, y: 8.0 }),
        Line(Point { x: 25.0, y: 24.0 }),
    ];
    out.path(&icon_segs, &None, &None);
}

/// The function generates a queue icon of queueing theory to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_queue_queue(out: &mut VecRenderer) -> () {
    /* box */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 1.0, y: 11.0 }),
        Line(Point { x: 18.5, y: 11.0 }),
        Line(Point { x: 18.5, y: 21.0 }),
        Line(Point { x: 1.0, y: 21.0 }),
    ];
    out.path(&icon_segs, &None, &None);

    /* 3 elements */
    let icon_segs: [geometry::DrawDirective; 6] = [
        Move(Point { x: 8.0, y: 11.0 }),
        Line(Point { x: 8.0, y: 21.0 }),
        Move(Point { x: 11.5, y: 11.0 }),
        Line(Point { x: 11.5, y: 21.0 }),
        Move(Point { x: 15.0, y: 11.0 }),
        Line(Point { x: 15.0, y: 21.0 }),
    ];
    out.path(&icon_segs, &None, &None);

    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(26.0, 16.0, 5.0, 5.0);
    out.path(&icon_segs, &None, &None);
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
        IconSource {
            name: "queue_buffer",
            generate: generate_queue_buffer,
        },
        IconSource {
            name: "queue_server",
            generate: generate_queue_server,
        },
        IconSource {
            name: "queue_queue",
            generate: generate_queue_queue,
        },
    ]
}
