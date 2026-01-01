//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use super::shape::get_circle_rel;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::CloseRel;
use crate::stream_if::geometry::DrawDirective::Continue;
use crate::stream_if::geometry::DrawDirective::ContinueRel;
use crate::stream_if::geometry::DrawDirective::Curve;
use crate::stream_if::geometry::DrawDirective::CurveRel;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::DrawDirective::MoveRel;
use crate::stream_if::geometry::DrawDirective::SymmetricRel;
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

/// blue color
static BLUE: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0x0,
    blue: 0x99,
};

/// red color
static RED: geometry::Color = geometry::Color {
    red: 0xcc,
    green: 0x0,
    blue: 0x0,
};

/// green color
static GREEN: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0xaa,
    blue: 0x0,
};

/// light gray color
static LIGHT_GRAY: geometry::Color = geometry::Color {
    red: 0xcc,
    green: 0xcc,
    blue: 0xcc,
};

/// light blue color
static LIGHT_BLUE: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0x0,
    blue: 0xdd,
};

/// blue pen
static BLUE_PEN: geometry::Pen = geometry::Pen {
    color: BLUE,
    width: 1.0,
};

/// green pen
static GREEN_PEN: geometry::Pen = geometry::Pen {
    color: GREEN,
    width: 1.0,
};

/// red pen
static RED_PEN: geometry::Pen = geometry::Pen {
    color: RED,
    width: 1.0,
};

/// The function generates a database icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_deploy_database(out: &mut dyn PathRenderer) -> () {
    let rx: f32 = 12.0; /* radius x-direction */
    let ry: f32 = 4.0; /* radius y-direction */
    let ctrlpnt_dx: f32 = rx * 0.5625; /* control point distance x */
    let ctrlpnt_dy: f32 = ry * 0.5625; /* control point distance y */

    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 5] = [
        MoveRel(Offset {
            dx: 4.0,
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
    out.render_path(&icon_segs, &None, &None);

    /* top circle */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle_rel(
        Offset {
            dx: 16.0,
            dy: 1.0 + ry,
        },
        rx,
        ry,
    );
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates a local-pc icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_deploy_local(out: &mut dyn PathRenderer) -> () {
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
    out.render_path(&icon_segs, &None, &None);

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
    out.render_path(&icon_segs, &None, &Some(LIGHT_GRAY));

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
    out.render_path(&icon_segs, &None, &Some(LIGHT_BLUE));
}

/// The function generates a cloud icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_deploy_cloud(out: &mut dyn PathRenderer) -> () {
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
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates ECB entity icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_ecb_entity(out: &mut dyn PathRenderer) -> () {
    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 18.0, y: 18.0 }, 12.0, 12.0);
    out.render_path(&icon_segs, &None, &None);

    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 2] = [
        Move(Point { x: 7.0, y: 30.5 }),
        Line(Point { x: 29.0, y: 30.5 }),
    ];
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates ECB control icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_ecb_control(out: &mut dyn PathRenderer) -> () {
    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 18.0, y: 18.0 }, 12.0, 12.0);
    out.render_path(&icon_segs, &None, &None);

    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 3] = [
        Move(Point { x: 22.0, y: 1.0 }),
        Line(Point { x: 17.0, y: 6.0 }),
        Line(Point { x: 22.0, y: 11.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates ECB boundary icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_ecb_boundary(out: &mut dyn PathRenderer) -> () {
    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 18.0, y: 18.0 }, 12.0, 12.0);
    out.render_path(&icon_segs, &None, &None);

    /* bottom cylinder */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 1.0, y: 9.0 }),
        Line(Point { x: 1.0, y: 27.0 }),
        Move(Point { x: 1.0, y: 18.0 }),
        Line(Point { x: 6.0, y: 18.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates a GSN goal icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_gsn_goal(out: &mut dyn PathRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 1.0, y: 7.0 }),
        Line(Point { x: 31.0, y: 7.0 }),
        Line(Point { x: 31.0, y: 25.0 }),
        Line(Point { x: 1.0, y: 25.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &Some(BLUE_PEN), &None);
}

/// The function generates a GSN context icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_gsn_context(out: &mut dyn PathRenderer) -> () {
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
    out.render_path(&icon_segs, &Some(BLUE_PEN), &None);
}

/// The function generates a GSN strategy icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_gsn_strategy(out: &mut dyn PathRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 7.0, y: 7.0 }),
        Line(Point { x: 31.0, y: 7.0 }),
        Line(Point { x: 25.0, y: 25.0 }),
        Line(Point { x: 1.0, y: 25.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &Some(BLUE_PEN), &None);
}

/// The function generates a GSN assumption icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_gsn_assumption(out: &mut dyn PathRenderer) -> () {
    /* ellipsis */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 16.0, y: 10.0 }, 15.0, 8.0);
    out.render_path(&icon_segs, &Some(BLUE_PEN), &None);

    /* A-character */
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 24.0, y: 25.0 }),
        Line(Point { x: 26.5, y: 19.0 }),
        Line(Point { x: 29.0, y: 25.0 }),
        Move(Point { x: 24.75, y: 23.5 }),
        Line(Point { x: 28.25, y: 23.5 }),
    ];
    out.render_path(&icon_segs, &Some(BLUE_PEN), &None);
}

/// The function generates a GSN justification icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_gsn_justification(out: &mut dyn PathRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 16.0, y: 10.0 }, 15.0, 8.0);
    out.render_path(&icon_segs, &Some(BLUE_PEN), &None);

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
    out.render_path(&icon_segs, &Some(BLUE_PEN), &None);
}

/// The function generates a GSN solution icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_gsn_solution(out: &mut dyn PathRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 16.0, y: 16.0 }, 15.0, 15.0);
    out.render_path(&icon_segs, &Some(BLUE_PEN), &None);
}

/// The function generates a buffer icon of queueing theory to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_queue_buffer(out: &mut dyn PathRenderer) -> () {
    /* box */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 1.0, y: 8.0 }),
        Line(Point { x: 31.0, y: 8.0 }),
        Line(Point { x: 31.0, y: 24.0 }),
        Line(Point { x: 1.0, y: 24.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);

    /* 3 elements */
    let icon_segs: [geometry::DrawDirective; 6] = [
        Move(Point { x: 13.0, y: 8.0 }),
        Line(Point { x: 13.0, y: 24.0 }),
        Move(Point { x: 19.0, y: 8.0 }),
        Line(Point { x: 19.0, y: 24.0 }),
        Move(Point { x: 25.0, y: 8.0 }),
        Line(Point { x: 25.0, y: 24.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates a server icon of queueing theory to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_queue_server(out: &mut dyn PathRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 16.0, y: 16.0 }, 15.0, 15.0);
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates a queue icon of queueing theory to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_queue_queue(out: &mut dyn PathRenderer) -> () {
    /* box */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 1.0, y: 11.0 }),
        Line(Point { x: 18.5, y: 11.0 }),
        Line(Point { x: 18.5, y: 21.0 }),
        Line(Point { x: 1.0, y: 21.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);

    /* 3 elements */
    let icon_segs: [geometry::DrawDirective; 6] = [
        Move(Point { x: 8.0, y: 11.0 }),
        Line(Point { x: 8.0, y: 21.0 }),
        Move(Point { x: 11.5, y: 11.0 }),
        Line(Point { x: 11.5, y: 21.0 }),
        Move(Point { x: 15.0, y: 11.0 }),
        Line(Point { x: 15.0, y: 21.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);

    /* circle */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 26.0, y: 16.0 }, 5.0, 5.0);
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates a rationale/decision icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_reason_decision(out: &mut dyn PathRenderer) -> () {
    /* two trays */
    let icon_segs: [geometry::DrawDirective; 13] = [
        MoveRel(Offset { dx: 8.0, dy: 12.0 }),
        LineRel(Offset { dx: -4.0, dy: 7.0 }),
        ContinueRel(Offset { dx: 1.0, dy: 1.0 }),
        ContinueRel(Offset { dx: 6.0, dy: 0.0 }),
        ContinueRel(Offset { dx: 1.0, dy: -1.0 }),
        ContinueRel(Offset { dx: -4.0, dy: -7.0 }),
        LineRel(Offset { dx: 8.0, dy: -4.0 }),
        ContinueRel(Offset { dx: 9.0, dy: 0.0 }),
        LineRel(Offset { dx: -4.0, dy: 7.0 }),
        ContinueRel(Offset { dx: 1.0, dy: 1.0 }),
        ContinueRel(Offset { dx: 6.0, dy: 0.0 }),
        ContinueRel(Offset { dx: 1.0, dy: -1.0 }),
        ContinueRel(Offset { dx: -4.0, dy: -7.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);

    /* stand and scale */
    let icon_segs: [geometry::DrawDirective; 4] = [
        MoveRel(Offset { dx: 15.0, dy: 5.0 }),
        LineRel(Offset { dx: 1.0, dy: 3.0 }),
        MoveRel(Offset { dx: 0.0, dy: 2.0 }),
        LineRel(Offset { dx: 0.0, dy: 17.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates a rationale/chosen icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_reason_chosen(out: &mut dyn PathRenderer) -> () {
    /* rejected tray */
    let icon_segs: [geometry::DrawDirective; 4] = [
        MoveRel(Offset { dx: 1.0, dy: 24.0 }),
        ContinueRel(Offset { dx: 4.0, dy: 4.0 }),
        ContinueRel(Offset { dx: 22.0, dy: 0.0 }),
        ContinueRel(Offset { dx: 4.0, dy: -4.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);

    /* minus */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_rel(Offset { dx: 16.0, dy: 17.0 }, 8.0, 8.0);
    out.render_path(&icon_segs, &Some(GREEN_PEN), &None);
    let icon_segs: [geometry::DrawDirective; 4] = [
        MoveRel(Offset { dx: 11.0, dy: 17.0 }),
        LineRel(Offset { dx: 10.0, dy: 0.0 }),
        MoveRel(Offset { dx: -5.0, dy: -5.0 }),
        LineRel(Offset { dx: 0.0, dy: 10.0 }),
    ];
    out.render_path(&icon_segs, &Some(GREEN_PEN), &None);
}

/// The function generates a rationale/rejected icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_reason_rejected(out: &mut dyn PathRenderer) -> () {
    /* rejected tray */
    let icon_segs: [geometry::DrawDirective; 4] = [
        MoveRel(Offset { dx: 1.0, dy: 24.0 }),
        ContinueRel(Offset { dx: 4.0, dy: 4.0 }),
        ContinueRel(Offset { dx: 22.0, dy: 0.0 }),
        ContinueRel(Offset { dx: 4.0, dy: -4.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);

    /* minus */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_rel(Offset { dx: 16.0, dy: 17.0 }, 8.0, 8.0);
    out.render_path(&icon_segs, &Some(RED_PEN), &None);
    let icon_segs: [geometry::DrawDirective; 2] = [
        MoveRel(Offset { dx: 11.0, dy: 17.0 }),
        LineRel(Offset { dx: 10.0, dy: 0.0 }),
    ];
    out.render_path(&icon_segs, &Some(RED_PEN), &None);
}

/// The function generates a object flow icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_flow_object(out: &mut dyn PathRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 7] = [
        Move(Point { x: 32.0, y: 32.0 }),
        Move(Point { x: 0.0, y: 0.0 }),
        Move(Point { x: 5.0, y: 5.0 }),
        Continue(Point { x: 27.0, y: 5.0 }),
        Continue(Point { x: 27.0, y: 27.0 }),
        Continue(Point { x: 5.0, y: 27.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates an control flow icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_flow_control(out: &mut dyn PathRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 15] = [
        Move(Point { x: 1.0, y: 16.0 }),
        LineRel(Offset { dx: 2.0, dy: 0.0 }),
        MoveRel(Offset { dx: 3.0, dy: 0.0 }),
        LineRel(Offset { dx: 2.0, dy: 0.0 }),
        MoveRel(Offset { dx: 3.0, dy: 0.0 }),
        LineRel(Offset { dx: 2.0, dy: 0.0 }),
        MoveRel(Offset { dx: 3.0, dy: 0.0 }),
        LineRel(Offset { dx: 2.0, dy: 0.0 }),
        MoveRel(Offset { dx: 3.0, dy: 0.0 }),
        LineRel(Offset { dx: 2.0, dy: 0.0 }),
        MoveRel(Offset { dx: 3.0, dy: 0.0 }),
        LineRel(Offset { dx: 3.5, dy: 0.0 }),
        Move(Point { x: 25.0, y: 13.0 }),
        LineRel(Offset { dx: 5.0, dy: 3.0 }),
        LineRel(Offset { dx: -5.0, dy: 3.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);
}

/// The function generates an error flow icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_flow_exception(out: &mut dyn PathRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 7] = [
        Move(Point { x: 1.0, y: 9.0 }),
        Continue(Point { x: 27.0, y: 9.0 }),
        Continue(Point { x: 1.0, y: 23.0 }),
        Continue(Point { x: 30.0, y: 23.0 }),
        Move(Point { x: 26.0, y: 20.0 }),
        Continue(Point { x: 31.0, y: 23.0 }),
        Continue(Point { x: 26.0, y: 26.0 }),
    ];
    out.render_path(&icon_segs, &None, &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "deploy_database",
            viewport: ICON_VIEW_RECT,
            generate: generate_deploy_database,
        },
        IconSource {
            name: "deploy_local",
            viewport: ICON_VIEW_RECT,
            generate: generate_deploy_local,
        },
        IconSource {
            name: "deploy_cloud",
            viewport: ICON_VIEW_RECT,
            generate: generate_deploy_cloud,
        },
        IconSource {
            name: "ecb_entity",
            viewport: ICON_VIEW_RECT,
            generate: generate_ecb_entity,
        },
        IconSource {
            name: "ecb_control",
            viewport: ICON_VIEW_RECT,
            generate: generate_ecb_control,
        },
        IconSource {
            name: "ecb_boundary",
            viewport: ICON_VIEW_RECT,
            generate: generate_ecb_boundary,
        },
        IconSource {
            name: "gsn_goal",
            viewport: ICON_VIEW_RECT,
            generate: generate_gsn_goal,
        },
        IconSource {
            name: "gsn_context",
            viewport: ICON_VIEW_RECT,
            generate: generate_gsn_context,
        },
        IconSource {
            name: "gsn_strategy",
            viewport: ICON_VIEW_RECT,
            generate: generate_gsn_strategy,
        },
        IconSource {
            name: "gsn_assumption",
            viewport: ICON_VIEW_RECT,
            generate: generate_gsn_assumption,
        },
        IconSource {
            name: "gsn_justification",
            viewport: ICON_VIEW_RECT,
            generate: generate_gsn_justification,
        },
        IconSource {
            name: "gsn_solution",
            viewport: ICON_VIEW_RECT,
            generate: generate_gsn_solution,
        },
        IconSource {
            name: "queue_buffer",
            viewport: ICON_VIEW_RECT,
            generate: generate_queue_buffer,
        },
        IconSource {
            name: "queue_server",
            viewport: ICON_VIEW_RECT,
            generate: generate_queue_server,
        },
        IconSource {
            name: "queue_queue",
            viewport: ICON_VIEW_RECT,
            generate: generate_queue_queue,
        },
        IconSource {
            name: "reason_decision",
            viewport: ICON_VIEW_RECT,
            generate: generate_reason_decision,
        },
        IconSource {
            name: "reason_chosen",
            viewport: ICON_VIEW_RECT,
            generate: generate_reason_chosen,
        },
        IconSource {
            name: "reason_rejected",
            viewport: ICON_VIEW_RECT,
            generate: generate_reason_rejected,
        },
        IconSource {
            name: "flow_object",
            viewport: ICON_VIEW_RECT,
            generate: generate_flow_object,
        },
        IconSource {
            name: "flow_control",
            viewport: ICON_VIEW_RECT,
            generate: generate_flow_control,
        },
        IconSource {
            name: "flow_exception",
            viewport: ICON_VIEW_RECT,
            generate: generate_flow_exception,
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
