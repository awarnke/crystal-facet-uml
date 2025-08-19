//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use super::shape::get_rect_abs;
use super::shape::get_rounded_rect_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::CloseRel;
use crate::stream_if::geometry::DrawDirective::Curve;
use crate::stream_if::geometry::DrawDirective::CurveRel;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::Offset;
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

/// The function generates a type_diag_activity
///
pub fn generate_type_diag_activity(out: &mut dyn PathRenderer) -> () {
    let icon_segs_start: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: 0.0,
            y: CY - HALFLINE,
        }),
        LineRel(Offset { dx: 3.0, dy: 0.0 }),
    ];
    out.render_path(&icon_segs_start, &Some(GRAY_PEN), &None);

    let icon_segs_start: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: 29.0,
            y: CY - HALFLINE,
        }),
        LineRel(Offset { dx: 3.0, dy: 0.0 }),
    ];
    out.render_path(&icon_segs_start, &Some(GRAY_PEN), &None);

    let icon_segs_fork: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: 5.0,
            y: CY - 7.0,
        }),
        LineRel(Offset { dx: 0.0, dy: 13.0 }),
    ];
    out.render_path(&icon_segs_fork, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_join: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: 27.0,
            y: CY - 7.0,
        }),
        LineRel(Offset { dx: 0.0, dy: 13.0 }),
    ];
    out.render_path(&icon_segs_join, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_act1: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 6.0 - HALFLINE,
            top: 3.0 - HALFLINE,
            width: 13.0,
            height: 7.0,
        },
        3.0,
    );
    out.render_path(&icon_segs_act1, &Some(GRAY_PEN), &None);

    let icon_segs_act2: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 6.0 - HALFLINE,
            top: 14.0 - HALFLINE,
            width: 13.0,
            height: 7.0,
        },
        3.0,
    );
    out.render_path(&icon_segs_act2, &Some(GRAY_PEN), &None);
}

/// The function generates a type_diag_block
///
pub fn generate_type_diag_block(out: &mut dyn PathRenderer) -> () {
    let icon_segs_left: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 2.0,
        top: 2.0,
        width: 10.0,
        height: 20.0,
    });
    out.render_path(&icon_segs_left, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_right: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 20.0,
        top: 2.0,
        width: 10.0,
        height: 20.0,
    });
    out.render_path(&icon_segs_right, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_link: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: 14.0,
            y: CY + 3.0 + HALFLINE,
        }),
        Line(Point {
            x: 18.0,
            y: CY + 3.0 + HALFLINE,
        }),
        Move(Point {
            x: 14.0,
            y: CY - 3.0 - HALFLINE,
        }),
        Line(Point {
            x: 18.0,
            y: CY - 3.0 - HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_link, &Some(GRAY_PEN), &None);
}

/// The function generates a type_diag_box
///
pub fn generate_type_diag_box(out: &mut dyn PathRenderer) -> () {
    let icon_segs_left: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 2.0,
        top: 8.0,
        width: 7.0,
        height: 7.0,
    });
    out.render_path(&icon_segs_left, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_mid: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 13.0,
        top: 15.0,
        width: 7.0,
        height: 7.0,
    });
    out.render_path(&icon_segs_mid, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_right: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 24.0,
        top: 2.0,
        width: 7.0,
        height: 7.0,
    });
    out.render_path(&icon_segs_right, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_class
///
pub fn generate_type_diag_class(out: &mut dyn PathRenderer) -> () {
    let icon_segs_left: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 2.0 - HALFLINE,
        top: 16.0 - HALFLINE,
        width: 6.0,
        height: 6.0,
    });
    out.render_path(&icon_segs_left, &Some(GRAY_PEN), &None);

    let icon_segs_mid: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 13.0 - HALFLINE,
        top: 2.0 - HALFLINE,
        width: 6.0,
        height: 6.0,
    });
    out.render_path(&icon_segs_mid, &Some(GRAY_PEN), &None);

    let icon_segs_right: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 24.0 - HALFLINE,
        top: 16.0 - HALFLINE,
        width: 6.0,
        height: 6.0,
    });
    out.render_path(&icon_segs_right, &Some(GRAY_PEN), &None);

    let icon_segs_link: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - HALFLINE,
            y: 11.0,
        }),
        LineRel(Offset { dx: 4.5, dy: 6.0 }),
        LineRel(Offset { dx: -9.0, dy: 0.0 }),
        CloseRel,
    ];
    out.render_path(&icon_segs_link, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_communication
///
pub fn generate_type_diag_communication(out: &mut dyn PathRenderer) -> () {
    let icon_segs_top: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 4.0 - HALFLINE,
        top: 2.0 - HALFLINE,
        width: 9.0,
        height: 6.0,
    });
    out.render_path(&icon_segs_top, &Some(GRAY_PEN), &None);

    let icon_segs_bottom: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 4.0 - HALFLINE,
        top: 16.0 + HALFLINE,
        width: 9.0,
        height: 6.0,
    });
    out.render_path(&icon_segs_bottom, &Some(GRAY_PEN), &None);

    let icon_segs_right: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX + 9.0,
            y: 5.0,
        }),
        CurveRel(
            Offset { dx: 6.0, dy: 1.0 },
            Offset { dx: 6.0, dy: 11.0 },
            Offset { dx: 1.0, dy: 14.0 },
        ),
        LineRel(Offset { dx: 4.0, dy: 0.0 }),
    ];
    out.render_path(&icon_segs_right, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_left: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX - 9.0,
            y: 19.0,
        }),
        CurveRel(
            Offset { dx: -6.0, dy: -1.0 },
            Offset {
                dx: -6.0,
                dy: -11.0,
            },
            Offset {
                dx: -1.0,
                dy: -14.0,
            },
        ),
        LineRel(Offset { dx: -4.0, dy: 0.0 }),
    ];
    out.render_path(&icon_segs_left, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_component
///
pub fn generate_type_diag_component(out: &mut dyn PathRenderer) -> () {
    let icon_segs_left_port: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 8.0,
        top: CY - 3.0,
        width: 5.0,
        height: 5.0,
    });
    out.render_path(&icon_segs_left_port, &Some(GRAY_THICK_PEN), &None);
    let icon_segs_right_port: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX + 3.0,
        top: CY - 3.0,
        width: 5.0,
        height: 5.0,
    });
    out.render_path(&icon_segs_right_port, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_mid: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 1.0,
            y: CY - HALFLINE,
        }),
        Line(Point {
            x: CX + 1.0,
            y: CY - HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_mid, &Some(GRAY_PEN), &None);

    let icon_segs_left: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 6.0 + HALFLINE,
            y: CY + 4.0,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: 2.0 + HALFLINE,
        }),
        LineRel(Offset { dx: -9.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: -14.0 }),
        LineRel(Offset { dx: 9.0, dy: 0.0 }),
        LineRel(Offset {
            dx: 0.0,
            dy: 2.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_left, &Some(GRAY_PEN), &None);
    let icon_segs_right: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX + 6.0 - HALFLINE,
            y: CY + 4.0,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: 2.0 + HALFLINE,
        }),
        LineRel(Offset { dx: 9.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: -14.0 }),
        LineRel(Offset { dx: -9.0, dy: 0.0 }),
        LineRel(Offset {
            dx: 0.0,
            dy: 2.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_right, &Some(GRAY_PEN), &None);
}

/// The function generates a type_diag_composite
///
pub fn generate_type_diag_composite(out: &mut dyn PathRenderer) -> () {
    let icon_segs_outer_port: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 2.0,
        top: CY - 3.0,
        width: 5.0,
        height: 5.0,
    });
    out.render_path(&icon_segs_outer_port, &Some(GRAY_THICK_PEN), &None);
    let icon_segs_inner_port: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 3.0,
        top: CY - 3.0,
        width: 5.0,
        height: 5.0,
    });
    out.render_path(&icon_segs_inner_port, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_conn: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: 9.0,
            y: CY - HALFLINE,
        }),
        Line(Point {
            x: 11.0,
            y: CY - HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_conn, &Some(GRAY_PEN), &None);

    let icon_segs_outer: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: 4.0 + HALFLINE,
            y: CY + 4.0,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: 5.0 + HALFLINE,
        }),
        LineRel(Offset { dx: 26.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: -20.0 }),
        LineRel(Offset { dx: -26.0, dy: 0.0 }),
        LineRel(Offset {
            dx: 0.0,
            dy: 5.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_outer, &Some(GRAY_PEN), &None);
    let icon_segs_inner: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX + 0.0 - HALFLINE,
            y: CY + 4.0,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: 2.0 + HALFLINE,
        }),
        LineRel(Offset { dx: 12.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: -14.0 }),
        LineRel(Offset { dx: -12.0, dy: 0.0 }),
        LineRel(Offset {
            dx: 0.0,
            dy: 2.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_inner, &Some(GRAY_PEN), &None);
}

/// The function generates a type_diag_deployment
///
pub fn generate_type_diag_deployment(out: &mut dyn PathRenderer) -> () {
    let icon_segs_component: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 5.0 - HALFLINE,
        top: 1.0 + HALFLINE,
        width: 11.0,
        height: 4.0,
    });
    out.render_path(&icon_segs_component, &Some(GRAY_PEN), &None);

    let icon_segs_device: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 13.0,
        top: 12.0,
        width: 23.0,
        height: 10.0,
    });
    out.render_path(&icon_segs_device, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_inner: [geometry::DrawDirective; 5] = [
        Move(Point {
            x: CX - 13.0,
            y: 12.0,
        }),
        LineRel(Offset {
            dx: 3.0 + HALFLINE,
            dy: -3.0 - HALFLINE,
        }),
        LineRel(Offset { dx: 23.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: 10.0 }),
        LineRel(Offset {
            dx: -3.0 - HALFLINE,
            dy: 3.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_inner, &Some(GRAY_PEN), &None);
}

/// The function generates a type_diag_internal
///
pub fn generate_type_diag_internal(out: &mut dyn PathRenderer) -> () {
    let icon_segs_outer: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 11.0,
        top: 2.0,
        width: 22.0,
        height: 20.0,
    });
    out.render_path(&icon_segs_outer, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_inner: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 5.0,
        top: 5.0,
        width: 10.0,
        height: 14.0,
    });
    out.render_path(&icon_segs_inner, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_list
///
pub fn generate_type_diag_list(out: &mut dyn PathRenderer) -> () {
    let icon_segs_inner: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 11.0,
            y: CY - 7.0,
        }),
        Line(Point {
            x: CX + 11.0,
            y: CY - 7.0,
        }),
        Move(Point {
            x: CX - 11.0,
            y: CY,
        }),
        Line(Point {
            x: CX + 11.0,
            y: CY,
        }),
        Move(Point {
            x: CX - 11.0,
            y: CY + 7.0,
        }),
        Line(Point {
            x: CX + 11.0,
            y: CY + 7.0,
        }),
    ];
    out.render_path(&icon_segs_inner, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_overview
///
pub fn generate_type_diag_overview(out: &mut dyn PathRenderer) -> () {
    let icon_segs_left_circle: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 5.5,
            y: CY - 1.5,
        }),
        CurveRel(
            Offset { dx: -1.0, dy: -3.0 },
            Offset { dx: -5.5, dy: -3.0 },
            Offset { dx: -5.5, dy: 1.5 },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 4.5 },
            Offset { dx: 6.0, dy: 4.5 },
            Offset { dx: 6.0, dy: 0.0 },
        ),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
    ];
    out.render_path(&icon_segs_left_circle, &Some(GRAY_PEN), &None);

    let icon_segs_right_circle: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX + 5.5,
            y: CY + 1.5,
        }),
        CurveRel(
            Offset { dx: 1.0, dy: 3.0 },
            Offset { dx: 5.5, dy: 3.0 },
            Offset { dx: 5.5, dy: -1.5 },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: -4.5 },
            Offset { dx: -6.0, dy: -4.5 },
            Offset { dx: -6.0, dy: 0.0 },
        ),
        LineRel(Offset { dx: -2.0, dy: -2.0 }),
    ];
    out.render_path(&icon_segs_right_circle, &Some(GRAY_PEN), &None);

    let icon_segs_top: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX + 8.0,
            y: CY - 7.0,
        }),
        Curve(
            Point {
                x: CX + 3.5,
                y: CY - 7.0 - 4.5,
            },
            Point {
                x: CX - 2.5,
                y: CY - 7.0 - 4.5,
            },
            Point {
                x: CX - 7.0,
                y: CY - 7.0,
            },
        ),
        Line(Point {
            x: CX - 6.0,
            y: CY - 11.0,
        }),
    ];
    out.render_path(&icon_segs_top, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_bottom: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX - 8.0,
            y: CY + 7.0,
        }),
        Curve(
            Point {
                x: CX - 3.5,
                y: CY + 7.0 + 4.5,
            },
            Point {
                x: CX + 2.5,
                y: CY + 7.0 + 4.5,
            },
            Point {
                x: CX + 7.0,
                y: CY + 7.0,
            },
        ),
        Line(Point {
            x: CX + 6.0,
            y: CY + 11.0,
        }),
    ];
    out.render_path(&icon_segs_bottom, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_package
///
pub fn generate_type_diag_package(out: &mut dyn PathRenderer) -> () {
    let icon_segs_box: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 10.0 + 8.0,
            y: 3.0,
        }),
        Line(Point {
            x: CX - 10.0,
            y: 9.0,
        }),
        Line(Point {
            x: CX - 10.0,
            y: 22.0,
        }),
        Line(Point {
            x: CX + 10.0,
            y: 22.0,
        }),
        Line(Point {
            x: CX + 10.0,
            y: 9.0,
        }),
        Line(Point {
            x: CX + 10.0 - 6.0,
            y: 1.0,
        }),
    ];
    out.render_path(&icon_segs_box, &Some(GRAY_THICK_PEN), &None);
    let icon_segs_border: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 10.0 + 2.0,
            y: 9.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + 10.0 - 2.0,
            y: 9.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segs_border, &Some(GRAY_PEN), &None);
}

/// The function generates a type_diag_parametric
///
pub fn generate_type_diag_parametric(out: &mut dyn PathRenderer) -> () {
    let icon_segs_equation: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 6.0 - HALFLINE,
            top: CY - 6.0 - HALFLINE,
            width: 13.0,
            height: 13.0,
        },
        3.0,
    );
    out.render_path(&icon_segs_equation, &Some(GRAY_PEN), &None);

    let icon_segs_in1: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 6.0 - HALFLINE,
        top: CY - 4.0 - HALFLINE,
        width: 3.0,
        height: 3.0,
    });
    out.render_path(&icon_segs_in1, &Some(GRAY_PEN), &None);

    let icon_segs_in2: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 6.0 - HALFLINE,
        top: CY + 1.0 + HALFLINE,
        width: 3.0,
        height: 3.0,
    });
    out.render_path(&icon_segs_in2, &Some(GRAY_PEN), &None);

    let icon_segs_out: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX + 4.0 - HALFLINE,
        top: CY - 1.0 - HALFLINE,
        width: 3.0,
        height: 3.0,
    });
    out.render_path(&icon_segs_out, &Some(GRAY_PEN), &None);

    let icon_segs_lines: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 15.0,
            y: CY - 3.0,
        }),
        Line(Point {
            x: CX - 8.0,
            y: CY - 3.0,
        }),
        Move(Point {
            x: CX - 15.0,
            y: CY + 3.0,
        }),
        Line(Point {
            x: CX - 8.0,
            y: CY + 3.0,
        }),
        Move(Point { x: CX + 8.0, y: CY }),
        Line(Point {
            x: CX + 15.0,
            y: CY,
        }),
    ];
    out.render_path(&icon_segs_lines, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_profile
///
pub fn generate_type_diag_profile(out: &mut dyn PathRenderer) -> () {
    let icon_segs_left_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 9.0, y: 15.0 }, 6.0, 6.0);
    out.render_path(&icon_segs_left_circle, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_right_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 25.0, y: 6.0 }, 4.0, 4.0);
    out.render_path(&icon_segs_right_circle, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_lines: [geometry::DrawDirective; 4] = [
        Move(Point { x: 11.0, y: 7.5 }),
        Line(Point { x: 20.0, y: 3.0 }),
        Move(Point { x: 24.0, y: 12.0 }),
        Line(Point { x: 16.0, y: 18.0 }),
    ];
    out.render_path(&icon_segs_lines, &Some(GRAY_PEN), &None);
}

/// The function generates a type_diag_requirement
///
pub fn generate_type_diag_requirement(out: &mut dyn PathRenderer) -> () {
    let icon_segs_goal1: [geometry::DrawDirective; 6] = [
        Move(Point { x: 2.0, y: 23.0 }),
        Line(Point { x: 2.0, y: 1.0 }),
        Move(Point { x: 5.0, y: 3.0 }),
        Line(Point {
            x: 15.0,
            y: 6.0 + HALFLINE,
        }),
        Line(Point { x: 5.0, y: 10.0 }),
        Close,
    ];
    out.render_path(&icon_segs_goal1, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_goal2: [geometry::DrawDirective; 6] = [
        Move(Point { x: 20.0, y: 20.0 }),
        Line(Point { x: 20.0, y: 1.0 }),
        Move(Point { x: 23.0, y: 3.0 }),
        Line(Point {
            x: 30.0,
            y: 5.0 + HALFLINE,
        }),
        Line(Point { x: 23.0, y: 8.0 }),
        Close,
    ];
    out.render_path(&icon_segs_goal2, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_sequence
///
pub fn generate_type_diag_sequence(out: &mut dyn PathRenderer) -> () {
    let icon_segs_arrows: [geometry::DrawDirective; 9] = [
        Move(Point {
            x: CX - 10.0,
            y: 3.0,
        }),
        Line(Point {
            x: CX + 9.0,
            y: 6.0,
        }),
        Line(Point {
            x: CX + 6.0,
            y: 3.0,
        }),
        Move(Point {
            x: CX + 10.0,
            y: 10.0,
        }),
        Line(Point {
            x: CX - 9.0,
            y: 13.0,
        }),
        Line(Point {
            x: CX - 6.0,
            y: 10.0,
        }),
        Move(Point {
            x: CX - 10.0,
            y: 17.0,
        }),
        Line(Point {
            x: CX + 9.0,
            y: 20.0,
        }),
        Line(Point {
            x: CX + 6.0,
            y: 17.0,
        }),
    ];
    out.render_path(&icon_segs_arrows, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_lifelines: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 13.0 - HALFLINE,
            y: 1.0,
        }),
        Line(Point {
            x: CX - 13.0 - HALFLINE,
            y: 23.0,
        }),
        Move(Point {
            x: CX + 13.0 + HALFLINE,
            y: 1.0,
        }),
        Line(Point {
            x: CX + 13.0 + HALFLINE,
            y: 23.0,
        }),
    ];
    out.render_path(&icon_segs_lifelines, &Some(GRAY_PEN), &None);
}

/// The function generates a type_diag_state
///
pub fn generate_type_diag_state(out: &mut dyn PathRenderer) -> () {
    let icon_segs_left_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 5.0, y: CY }, 3.0, 3.0);
    out.render_path(&icon_segs_left_circle, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_right_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 21.0, y: CY }, 9.0, 9.0);
    out.render_path(&icon_segs_right_circle, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_inner_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: 19.0, y: CY }, 3.0, 3.0);
    out.render_path(&icon_segs_inner_circle, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_timing
///
pub fn generate_type_diag_timing(out: &mut dyn PathRenderer) -> () {
    let icon_segs_arrows: [geometry::DrawDirective; 10] = [
        Move(Point { x: 1.0, y: 20.0 }),
        Line(Point { x: 6.0, y: 20.0 }),
        Line(Point { x: 6.0, y: 15.0 }),
        Line(Point { x: 11.0, y: 15.0 }),
        Line(Point { x: 11.0, y: 10.0 }),
        Line(Point { x: 16.0, y: 10.0 }),
        Line(Point { x: 16.0, y: 5.0 }),
        Line(Point { x: 26.0, y: 5.0 }),
        Line(Point { x: 26.0, y: 20.0 }),
        Line(Point { x: 31.0, y: 20.0 }),
    ];
    out.render_path(&icon_segs_arrows, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_diag_usecase
///
pub fn generate_type_diag_usecase(out: &mut dyn PathRenderer) -> () {
    let icon_segs_body: [geometry::DrawDirective; 21] = [
        Move(Point {
            x: 8.0,
            y: 22.0 + HALFLINE,
        }),
        Line(Point {
            x: 3.0,
            y: 22.0 + HALFLINE,
        }),
        Line(Point { x: 4.0, y: 18.0 }),
        Line(Point { x: 3.0, y: 13.0 }),
        Line(Point { x: 4.0, y: 10.5 }),
        Line(Point { x: 2.0, y: 10.0 }),
        Line(Point { x: 2.0, y: 6.0 }),
        Line(Point { x: 4.0, y: 5.0 }),
        Line(Point {
            x: 3.0,
            y: 3.0 - HALFLINE,
        }),
        Line(Point {
            x: 5.0,
            y: 2.0 - HALFLINE,
        }),
        Line(Point { x: 6.0, y: 3.0 }),
        Line(Point { x: 5.5, y: 5.0 }),
        Line(Point { x: 6.0, y: 6.0 }),
        Line(Point {
            x: 6.0,
            y: 8.0 + HALFLINE,
        }),
        Line(Point {
            x: 7.0 + HALFLINE,
            y: 8.0 + HALFLINE,
        }),
        Line(Point {
            x: 7.0 + HALFLINE,
            y: 11.0,
        }),
        Line(Point { x: 6.5, y: 10.8 }),
        Line(Point { x: 6.0, y: 13.0 }),
        Line(Point { x: 6.5, y: 17.0 }),
        Line(Point { x: 6.0, y: 21.0 }),
        Close,
    ];
    out.render_path(&icon_segs_body, &Some(GRAY_PEN), &None);
    let icon_segs_use_case: [geometry::DrawDirective; 5] = get_circle_abs(
        Point {
            x: 21.0,
            y: CY - 6.0,
        },
        9.0,
        4.0,
    );
    out.render_path(&icon_segs_use_case, &Some(GRAY_THICK_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "type_diag_activity",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_activity,
        },
        IconSource {
            name: "type_diag_block",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_block,
        },
        IconSource {
            name: "type_diag_box",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_box,
        },
        IconSource {
            name: "type_diag_class",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_class,
        },
        IconSource {
            name: "type_diag_communication",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_communication,
        },
        IconSource {
            name: "type_diag_component",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_component,
        },
        IconSource {
            name: "type_diag_composite",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_composite,
        },
        IconSource {
            name: "type_diag_deployment",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_deployment,
        },
        IconSource {
            name: "type_diag_internal",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_internal,
        },
        IconSource {
            name: "type_diag_list",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_list,
        },
        IconSource {
            name: "type_diag_overview",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_overview,
        },
        IconSource {
            name: "type_diag_package",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_package,
        },
        IconSource {
            name: "type_diag_parametric",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_parametric,
        },
        IconSource {
            name: "type_diag_profile",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_profile,
        },
        IconSource {
            name: "type_diag_requirement",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_requirement,
        },
        IconSource {
            name: "type_diag_sequence",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_sequence,
        },
        IconSource {
            name: "type_diag_state",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_state,
        },
        IconSource {
            name: "type_diag_timing",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_timing,
        },
        IconSource {
            name: "type_diag_usecase",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_diag_usecase,
        },
    ]
}

/*
 * Copyright 2024-2025 Andreas Warnke
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
