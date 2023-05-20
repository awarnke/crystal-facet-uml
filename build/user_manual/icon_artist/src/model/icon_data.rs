//! The module provides functions to render an icon to vector graphics.

use super::geometry;
use super::geometry::Color;
use super::geometry::DrawDirective::Close;
use super::geometry::DrawDirective::CloseRel;
use super::geometry::DrawDirective::Curve;
use super::geometry::DrawDirective::CurveRel;
use super::geometry::DrawDirective::Line;
use super::geometry::DrawDirective::LineRel;
use super::geometry::DrawDirective::Move;
use super::geometry::DrawDirective::MoveRel;
use super::geometry::DrawDirective::Symmetric;
use super::geometry::DrawDirective::SymmetricRel;
use super::geometry::Offset;
use super::geometry::Point;
use super::geometry::Rect;
use crate::render::render_svg::VecRenderer;

/// Defines the data needed to render an icon
pub struct IconSource<'my_lifespan> {
    /// The name
    pub name: &'my_lifespan str,
    /// The generator function
    pub generate: fn(out: &mut VecRenderer) -> (),
}

/// The view rectangle of each icon
pub static ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 32.0,
};

/// blue color
static BLUE: Color = Color {
    red: 0x0,
    green: 0x0,
    blue: 0x99,
};

/// The function defines the control points for a circle in absolute coordinates
///
/// # Arguments
///
/// * `cx` - The absolute x-coordinate of the center
/// * `cy` - The absolute y-coordinate of the center
/// * `rx` - The radius in x-direction
/// * `ry` - The radius in y-direction
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
fn get_circle(cx: f32, cy: f32, rx: f32, ry: f32) -> [geometry::DrawDirective; 5] {
    let ctrlpnt_dx: f32 = rx * 0.55; /* control point distance x */
    let ctrlpnt_dy: f32 = ry * 0.55; /* control point distance y */
    [
        Move(Point { x: cx - rx, y: cy }),
        Curve(
            Point {
                x: cx - rx,
                y: cy - ctrlpnt_dy,
            },
            Point {
                x: cx - ctrlpnt_dx,
                y: cy - ry,
            },
            Point { x: cx, y: cy - ry },
        ),
        Symmetric(
            Point {
                x: cx + rx,
                y: cy - ctrlpnt_dy,
            },
            Point { x: cx + rx, y: cy },
        ),
        Symmetric(
            Point {
                x: cx + ctrlpnt_dx,
                y: cy + ry,
            },
            Point { x: cx, y: cy + ry },
        ),
        Symmetric(
            Point {
                x: cx - rx,
                y: cy + ctrlpnt_dy,
            },
            Point { x: cx - rx, y: cy },
        ),
    ]
}

/// The function defines the control points for a circle in relative offsets
///
/// # Arguments
///
/// * `c_dx` - The relative x-offset of the center
/// * `c_dy` - The relative y-offset of the center
/// * `rx` - The radius in x-direction
/// * `ry` - The radius in y-direction
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
fn get_circle_rel(c_dx: f32, c_dy: f32, rx: f32, ry: f32) -> [geometry::DrawDirective; 5] {
    let ctrlpnt_dx: f32 = rx * 0.55; /* control point distance x */
    let ctrlpnt_dy: f32 = ry * 0.55; /* control point distance y */
    [
        MoveRel(Offset {
            dx: c_dx - rx,
            dy: c_dy,
        }),
        CurveRel(
            Offset {
                dx: 0.0,
                dy: (-ctrlpnt_dy),
            },
            Offset {
                dx: rx - ctrlpnt_dx,
                dy: (-ry),
            },
            Offset { dx: rx, dy: (-ry) },
        ),
        SymmetricRel(
            Offset {
                dx: rx,
                dy: ry - ctrlpnt_dy,
            },
            Offset { dx: rx, dy: ry },
        ),
        SymmetricRel(
            Offset {
                dx: (-rx) + ctrlpnt_dx,
                dy: ry,
            },
            Offset { dx: (-rx), dy: ry },
        ),
        SymmetricRel(
            Offset {
                dx: (-rx),
                dy: (-ry) + ctrlpnt_dy,
            },
            Offset {
                dx: (-rx),
                dy: (-ry),
            },
        ),
    ]
}

/// The function generates a database icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_deploy_database(out: &mut VecRenderer) -> () {
    let rx: f32 = 10.0; /* radius x-direction */
    let ry: f32 = 4.0; /* radius y-direction */
    let ctrlpnt_dx: f32 = rx * 0.55; /* control point distance x */
    let ctrlpnt_dy: f32 = ry * 0.55; /* control point distance y */

    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 5] = [
        MoveRel(Offset {
            dx: 6.0,
            dy: 1.0 + ry,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: 30.0 - (2.0 * ry),
        }),
        CurveRel(
            Offset {
                dx: 0.0,
                dy: ctrlpnt_dy,
            },
            Offset {
                dx: rx - ctrlpnt_dx,
                dy: ry,
            },
            Offset { dx: rx, dy: ry },
        ),
        SymmetricRel(
            Offset {
                dx: rx,
                dy: (-ry) + ctrlpnt_dy,
            },
            Offset { dx: rx, dy: (-ry) },
        ),
        LineRel(Offset {
            dx: 0.0,
            dy: (-30.0) + (2.0 * ry),
        }),
    ];
    out.path(&icon_segs, &None, &None);

    /* top circle */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle_rel(16.0, 1.0 + ry, rx, ry);
    out.path(&icon_segs, &None, &None);
}

/// The function generates a local-pc icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_deploy_local(out: &mut VecRenderer) -> () {
    /* border line */
    let icon_segs: [geometry::DrawDirective; 7] = [
        MoveRel(Offset { dx: 1.0, dy: 23.0 }),
        LineRel(Offset { dx: 16.0, dy: 8.0 }),
        LineRel(Offset { dx: 8.0, dy: -6.0 }),
        LineRel(Offset { dx: 3.0, dy: -11.0 }),
        LineRel(Offset {
            dx: -16.0,
            dy: -7.0,
        }),
        LineRel(Offset { dx: -2.0, dy: 11.0 }),
        CloseRel,
    ];
    out.path(&icon_segs, &None, &None);

    /* keyboard */
    let icon_segs: [geometry::DrawDirective; 5] = [
        MoveRel(Offset { dx: 5.0, dy: 23.0 }),
        LineRel(Offset { dx: 11.5, dy: 5.5 }),
        LineRel(Offset { dx: 5.0, dy: -3.5 }),
        LineRel(Offset {
            dx: -11.0,
            dy: -5.2,
        }),
        CloseRel,
    ];
    let gray: Color = Color {
        red: 0xcc,
        green: 0xcc,
        blue: 0xcc,
    };
    out.path(&icon_segs, &None, &Some(gray));

    /* screen */
    let icon_segs: [geometry::DrawDirective; 5] = [
        MoveRel(Offset { dx: 11.5, dy: 17.0 }),
        LineRel(Offset { dx: 12.5, dy: 6.0 }),
        LineRel(Offset { dx: 2.0, dy: -8.0 }),
        LineRel(Offset {
            dx: -12.8,
            dy: -6.0,
        }),
        CloseRel,
    ];
    let blue: Color = Color {
        red: 0x0,
        green: 0x0,
        blue: 0xdd,
    };
    out.path(&icon_segs, &None, &Some(blue));
}

/// The function generates a cloud icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_deploy_cloud(out: &mut VecRenderer) -> () {
    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 5] = [
        MoveRel(Offset { dx: 3.0, dy: 22.0 }),
        CurveRel(
            Offset { dx: -3.0, dy: -3.0 },
            Offset { dx: 3.0, dy: -9.0 },
            Offset { dx: 6.0, dy: -6.0 },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: -5.0 },
            Offset { dx: 8.0, dy: -6.0 },
            Offset { dx: 9.0, dy: -1.0 },
        ),
        CurveRel(
            Offset { dx: 7.0, dy: -2.0 },
            Offset { dx: 14.0, dy: 4.0 },
            Offset { dx: 11.0, dy: 7.0 },
        ),
        CloseRel,
    ];
    out.path(&icon_segs, &None, &None);
}

/// The function generates ECB entity icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_ecb_entity(out: &mut VecRenderer) -> () {
    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(18.0, 18.0, 12.0, 12.0);
    out.path(&icon_segs, &None, &None);

    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 2] = [
        Move(Point { x: 7.0, y: 30.5 }),
        Line(Point { x: 29.0, y: 30.5 }),
    ];
    out.path(&icon_segs, &None, &None);
}

/// The function generates ECB control icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_ecb_control(out: &mut VecRenderer) -> () {
    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(18.0, 18.0, 12.0, 12.0);
    out.path(&icon_segs, &None, &None);

    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 3] = [
        Move(Point { x: 22.0, y: 1.0 }),
        Line(Point { x: 17.0, y: 6.0 }),
        Line(Point { x: 22.0, y: 11.0 }),
    ];
    out.path(&icon_segs, &None, &None);
}

/// The function generates ECB boundary icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_ecb_boundary(out: &mut VecRenderer) -> () {
    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle(18.0, 18.0, 12.0, 12.0);
    out.path(&icon_segs, &None, &None);

    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 1.0, y: 9.0 }),
        Line(Point { x: 1.0, y: 27.0 }),
        Move(Point { x: 1.0, y: 18.0 }),
        Line(Point { x: 6.0, y: 18.0 }),
    ];
    out.path(&icon_segs, &None, &None);
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
            name: "deploy_database",
            generate: generate_deploy_database,
        },
        IconSource {
            name: "deploy_local",
            generate: generate_deploy_local,
        },
        IconSource {
            name: "deploy_cloud",
            generate: generate_deploy_cloud,
        },
        IconSource {
            name: "ecb_entity",
            generate: generate_ecb_entity,
        },
        IconSource {
            name: "ecb_control",
            generate: generate_ecb_control,
        },
        IconSource {
            name: "ecb_boundary",
            generate: generate_ecb_boundary,
        },
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
