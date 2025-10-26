//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use super::shape::get_rect_abs;
use super::shape::get_rounded_rect_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
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

/// gray thick pen
static GRAY_THICK_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 2.0,
};

/// The function generates a gear wheel to vector graphics drawing directives
///
pub fn generate_type_clas_stereotype(out: &mut dyn PathRenderer) -> () {
    /* spoke of wheel */
    let r3: f32 = 11.0;
    let r2: f32 = 10.0;
    let r1: f32 = 8.5;
    let mut icon_segs: [geometry::DrawDirective; 61] = [Close; 61];
    icon_segs[0] = Move(Point { x: CX + r2, y: CY });
    for index in 0..10 {
        let alpha: f32 = std::f32::consts::PI / 5.0 * (index as f32);
        let a_dx = alpha.cos();
        let a_dy = alpha.sin();
        let beta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.05);
        let b_dx = beta.cos();
        let b_dy = beta.sin();
        let gamma: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.5);
        let g_dx = gamma.cos();
        let g_dy = gamma.sin();
        let delta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.55);
        let d_dx = delta.cos();
        let d_dy = delta.sin();
        let epsilon: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.65);
        let e_dx = epsilon.cos();
        let e_dy = epsilon.sin();
        let zeta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.9);
        let z_dx = zeta.cos();
        let z_dy = zeta.sin();
        let omega: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 1.0);
        let o_dx = omega.cos();
        let o_dy = omega.sin();
        icon_segs[index * 6 + 1] = LineRel(Offset {
            dx: (-r2) * a_dx + r1 * b_dx,
            dy: (-r2) * a_dy + r1 * b_dy,
        });
        icon_segs[index * 6 + 2] = LineRel(Offset {
            dx: (-r1) * b_dx + r1 * g_dx,
            dy: (-r1) * b_dy + r1 * g_dy,
        });
        icon_segs[index * 6 + 3] = LineRel(Offset {
            dx: (-r1) * g_dx + r2 * d_dx,
            dy: (-r1) * g_dy + r2 * d_dy,
        });
        icon_segs[index * 6 + 4] = LineRel(Offset {
            dx: (-r2) * d_dx + r3 * e_dx,
            dy: (-r2) * d_dy + r3 * e_dy,
        });
        icon_segs[index * 6 + 5] = LineRel(Offset {
            dx: (-r3) * e_dx + r3 * z_dx,
            dy: (-r3) * e_dy + r3 * z_dy,
        });
        icon_segs[index * 6 + 6] = LineRel(Offset {
            dx: (-r3) * z_dx + r2 * o_dx,
            dy: (-r3) * z_dy + r2 * o_dy,
        });
    }
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);

    /* rim of wheel */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle_abs(Point { x: CX, y: CY }, 2.0, 2.0);
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);
}

/// The function generates a flower image to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_type_clas_image(out: &mut dyn PathRenderer) -> () {
    /* flower leaves */
    let r3: f32 = 11.0;
    let r2: f32 = 9.0;
    let r1: f32 = 4.25;
    let mut icon_segs: [geometry::DrawDirective; 21] = [Close; 21];
    icon_segs[0] = Move(Point { x: CX + r1, y: CY });
    for index in 0..10 {
        let alpha: f32 = std::f32::consts::PI / 5.0 * (index as f32);
        let a_dx = alpha.cos();
        let a_dy = alpha.sin();
        let beta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.3);
        let b_dx = beta.cos();
        let b_dy = beta.sin();
        let gamma: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.5);
        let g_dx = gamma.cos();
        let g_dy = gamma.sin();
        let delta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.7);
        let d_dx = delta.cos();
        let d_dy = delta.sin();
        let omega: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 1.0);
        let o_dx = omega.cos();
        let o_dy = omega.sin();
        icon_segs[index * 2 + 1] = CurveRel(
            Offset {
                dx: (-r1) * a_dx + r2 * a_dx,
                dy: (-r1) * a_dy + r2 * a_dy,
            },
            Offset {
                dx: (-r1) * a_dx + r3 * b_dx,
                dy: (-r1) * a_dy + r3 * b_dy,
            },
            Offset {
                dx: (-r1) * a_dx + r3 * g_dx,
                dy: (-r1) * a_dy + r3 * g_dy,
            },
        );
        icon_segs[index * 2 + 2] = CurveRel(
            Offset {
                dx: (-r3) * g_dx + r3 * d_dx,
                dy: (-r3) * g_dy + r3 * d_dy,
            },
            Offset {
                dx: (-r3) * g_dx + r2 * o_dx,
                dy: (-r3) * g_dy + r2 * o_dy,
            },
            Offset {
                dx: (-r3) * g_dx + r1 * o_dx,
                dy: (-r3) * g_dy + r1 * o_dy,
            },
        );
    }
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);

    /* flower center */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY }, 3.25, 3.25);
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_block
///
pub fn generate_type_clas_block(out: &mut dyn PathRenderer) -> () {
    let icon_base_rect: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 8.0,
        top: CY - 8.0,
        width: 16.0,
        height: 16.0,
    });
    out.render_path(&icon_base_rect, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_subsystem
///
pub fn generate_type_clas_subsystem(out: &mut dyn PathRenderer) -> () {
    let icon_base_rect: [geometry::DrawDirective; 21] = [
        Move(Point {
            x: CX - 9.0,
            y: CY - 9.0,
        }),
        Line(Point {
            x: CX - 4.0,
            y: CY - 9.0,
        }),
        Line(Point {
            x: CX - 4.0,
            y: CY - 7.0,
        }),
        Line(Point {
            x: CX + 4.0,
            y: CY - 7.0,
        }),
        Line(Point {
            x: CX + 4.0,
            y: CY - 9.0,
        }),
        Line(Point {
            x: CX + 9.0,
            y: CY - 9.0,
        }),
        Line(Point {
            x: CX + 9.0,
            y: CY - 4.0,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY - 4.0,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY + 4.0,
        }),
        Line(Point {
            x: CX + 9.0,
            y: CY + 4.0,
        }),
        Line(Point {
            x: CX + 9.0,
            y: CY + 9.0,
        }),
        Line(Point {
            x: CX + 4.0,
            y: CY + 9.0,
        }),
        Line(Point {
            x: CX + 4.0,
            y: CY + 7.0,
        }),
        Line(Point {
            x: CX - 4.0,
            y: CY + 7.0,
        }),
        Line(Point {
            x: CX - 4.0,
            y: CY + 9.0,
        }),
        Line(Point {
            x: CX - 9.0,
            y: CY + 9.0,
        }),
        Line(Point {
            x: CX - 9.0,
            y: CY + 4.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY + 4.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY - 4.0,
        }),
        Line(Point {
            x: CX - 9.0,
            y: CY - 4.0,
        }),
        Close,
    ];
    out.render_path(&icon_base_rect, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_component
///
pub fn generate_type_clas_component(out: &mut dyn PathRenderer) -> () {
    let icon_base_rect: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 4.0,
        top: CY - 8.0,
        width: 16.0,
        height: 16.0,
    });
    out.render_path(&icon_base_rect, &Some(GRAY_THICK_PEN), &None);

    /* provides */
    let icon_circle: [geometry::DrawDirective; 5] = get_circle_abs(
        Point {
            x: CX - 12.5,
            y: CY - 4.0 - HALFLINE,
        },
        2.5,
        2.5,
    );
    out.render_path(&icon_circle, &Some(GRAY_THICK_PEN), &None);
    let icon_conn1: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 9.0,
            y: CY - 4.0 - HALFLINE,
        }),
        Line(Point {
            x: CX - 5.0,
            y: CY - 4.0 - HALFLINE,
        }),
    ];
    out.render_path(&icon_conn1, &Some(GRAY_PEN), &None);

    /* requires */
    let icon_circle: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX - 12.5,
            y: CY + 1.5 + HALFLINE,
        }),
        Curve(
            Point {
                x: CX - 11.0,
                y: CY + 1.5 + HALFLINE,
            },
            Point {
                x: CX - 10.0,
                y: CY + 2.5 + HALFLINE,
            },
            Point {
                x: CX - 10.0,
                y: CY + 4.0 + HALFLINE,
            },
        ),
        Curve(
            Point {
                x: CX - 10.0,
                y: CY + 5.5 + HALFLINE,
            },
            Point {
                x: CX - 11.0,
                y: CY + 6.5 + HALFLINE,
            },
            Point {
                x: CX - 12.5,
                y: CY + 6.5 + HALFLINE,
            },
        ),
    ];
    out.render_path(&icon_circle, &Some(GRAY_THICK_PEN), &None);
    let icon_conn1: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 9.0,
            y: CY + 4.0 + HALFLINE,
        }),
        Line(Point {
            x: CX - 5.0,
            y: CY + 4.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_conn1, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_part
///
pub fn generate_type_clas_part(out: &mut dyn PathRenderer) -> () {
    let icon_base_rect: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 4.0,
        top: CY - 8.0,
        width: 16.0,
        height: 16.0,
    });
    out.render_path(&icon_base_rect, &Some(GRAY_THICK_PEN), &Some(GRAY));

    /* provides */
    let icon_circle: [geometry::DrawDirective; 5] = get_circle_abs(
        Point {
            x: CX - 12.0,
            y: CY - 5.0,
        },
        3.0,
        3.0,
    );
    out.render_path(&icon_circle, &Some(GRAY_THICK_PEN), &None);
    let icon_conn1: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 9.0,
            y: CY - 4.0 - HALFLINE,
        }),
        Line(Point {
            x: CX - 5.0,
            y: CY - 4.0 - HALFLINE,
        }),
    ];
    out.render_path(&icon_conn1, &Some(GRAY_PEN), &None);

    /* requires */
    let icon_circle: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX - 12.5,
            y: CY + 1.5 + HALFLINE,
        }),
        Curve(
            Point {
                x: CX - 11.0,
                y: CY + 1.5 + HALFLINE,
            },
            Point {
                x: CX - 10.0,
                y: CY + 2.5 + HALFLINE,
            },
            Point {
                x: CX - 10.0,
                y: CY + 4.0 + HALFLINE,
            },
        ),
        Curve(
            Point {
                x: CX - 10.0,
                y: CY + 5.5 + HALFLINE,
            },
            Point {
                x: CX - 11.0,
                y: CY + 6.5 + HALFLINE,
            },
            Point {
                x: CX - 12.5,
                y: CY + 6.5 + HALFLINE,
            },
        ),
    ];
    out.render_path(&icon_circle, &Some(GRAY_THICK_PEN), &None);
    let icon_conn1: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 9.0,
            y: CY + 4.0 + HALFLINE,
        }),
        Line(Point {
            x: CX - 5.0,
            y: CY + 4.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_conn1, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_class
///
pub fn generate_type_clas_class(out: &mut dyn PathRenderer) -> () {
    let icon_base_rect: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 7.0,
        top: CY - 9.0,
        width: 14.0,
        height: 18.0,
    });
    out.render_path(&icon_base_rect, &Some(GRAY_THICK_PEN), &None);

    let icon_segment: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 6.0,
            y: CY - 1.0,
        }),
        Line(Point {
            x: CX + 6.0,
            y: CY - 1.0,
        }),
    ];
    out.render_path(&icon_segment, &Some(GRAY_THICK_PEN), &None);

    let icon_segment2: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 5.0,
            y: CY + 2.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + 5.0,
            y: CY + 2.0 + HALFLINE,
        }),
        Move(Point {
            x: CX - 5.0,
            y: CY + 5.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + 5.0,
            y: CY + 5.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segment2, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_object
///
pub fn generate_type_clas_object(out: &mut dyn PathRenderer) -> () {
    let icon_base_rect: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 7.0,
        top: CY - 9.0,
        width: 14.0,
        height: 18.0,
    });
    out.render_path(&icon_base_rect, &Some(GRAY_THICK_PEN), &None);

    let icon_base_rect: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 6.0,
        top: CY - 8.0,
        width: 12.0,
        height: 8.0,
    });
    out.render_path(&icon_base_rect, &None, &Some(GRAY));

    let icon_segment2: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 5.0,
            y: CY + 2.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + 5.0,
            y: CY + 2.0 + HALFLINE,
        }),
        Move(Point {
            x: CX - 5.0,
            y: CY + 5.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + 5.0,
            y: CY + 5.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segment2, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_interface
///
pub fn generate_type_clas_interface(out: &mut dyn PathRenderer) -> () {
    let icon_segment: [geometry::DrawDirective; 12] = [
        Move(Point {
            x: CX - 7.0,
            y: CY - 5.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY - 9.0,
        }),
        Line(Point {
            x: CX - 3.0,
            y: CY - 9.0,
        }),
        Move(Point {
            x: CX + 3.0,
            y: CY - 9.0,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY - 9.0,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY - 5.0,
        }),
        Move(Point {
            x: CX + 7.0,
            y: CY + 5.0,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY + 9.0,
        }),
        Line(Point {
            x: CX + 3.0,
            y: CY + 9.0,
        }),
        Move(Point {
            x: CX - 3.0,
            y: CY + 9.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY + 9.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY + 5.0,
        }),
    ];
    out.render_path(&icon_segment, &Some(GRAY_THICK_PEN), &None);

    let icon_segment2: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 7.0,
            y: CY - 1.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY - 1.0 + HALFLINE,
        }),
    ];
    out.render_path(&icon_segment2, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_node
///
pub fn generate_type_clas_node(out: &mut dyn PathRenderer) -> () {
    let icon_segs_device: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 13.0,
        top: CY - 2.0,
        width: 23.0,
        height: 10.0,
    });
    out.render_path(&icon_segs_device, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_inner: [geometry::DrawDirective; 5] = [
        Move(Point {
            x: CX - 13.0,
            y: CY - 2.0,
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

/// The function generates a type_clas_package
///
pub fn generate_type_clas_package(out: &mut dyn PathRenderer) -> () {
    let icon_package: [geometry::DrawDirective; 8] = [
        Move(Point {
            x: CX - 13.0,
            y: CY - 4.0,
        }),
        Curve(
            Point {
                x: CX - 13.0,
                y: CY - 6.0,
            },
            Point {
                x: CX - 12.0,
                y: CY - 7.0,
            },
            Point {
                x: CX - 10.0,
                y: CY - 7.0,
            },
        ),
        Line(Point {
            x: CX - 5.0,
            y: CY - 7.0,
        }),
        Curve(
            Point {
                x: CX - 3.0,
                y: CY - 7.0,
            },
            Point {
                x: CX - 2.0,
                y: CY - 6.0,
            },
            Point {
                x: CX - 2.0,
                y: CY - 4.0,
            },
        ),
        Line(Point {
            x: CX + 13.0,
            y: CY - 4.0,
        }),
        Line(Point {
            x: CX + 13.0,
            y: CY + 10.0,
        }),
        Line(Point {
            x: CX - 13.0,
            y: CY + 10.0,
        }),
        Close,
    ];
    out.render_path(&icon_package, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_constraint
///
pub fn generate_type_clas_constraint(out: &mut dyn PathRenderer) -> () {
    let icon_segs_equation: [geometry::DrawDirective; 8] = [
        Move(Point {
            x: CX - 6.5,
            y: CY - 6.0,
        }),
        Curve(
            Point {
                x: CX - 6.0,
                y: CY - 7.0,
            },
            Point {
                x: CX - 5.0,
                y: CY - 8.0,
            },
            Point {
                x: CX - 3.0,
                y: CY - 8.0,
            },
        ),
        Line(Point {
            x: CX + 3.0,
            y: CY - 8.0,
        }),
        Curve(
            Point {
                x: CX + 5.0,
                y: CY - 8.0,
            },
            Point {
                x: CX + 7.0,
                y: CY - 6.0,
            },
            Point {
                x: CX + 7.0,
                y: CY - 4.0,
            },
        ),
        Move(Point {
            x: CX + 7.0,
            y: CY + 3.0,
        }),
        Curve(
            Point {
                x: CX + 7.0,
                y: CY + 5.0,
            },
            Point {
                x: CX + 5.0,
                y: CY + 7.0,
            },
            Point {
                x: CX + 3.0,
                y: CY + 7.0,
            },
        ),
        Line(Point {
            x: CX - 3.0,
            y: CY + 7.0,
        }),
        Curve(
            Point {
                x: CX - 5.0,
                y: CY + 7.0,
            },
            Point {
                x: CX - 6.0,
                y: CY + 6.0,
            },
            Point {
                x: CX - 6.5,
                y: CY + 5.0,
            },
        ),
    ];
    out.render_path(&icon_segs_equation, &Some(GRAY_THICK_PEN), &None);

    let icon_segs_in1: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 7.0 - HALFLINE,
        top: CY - 5.0 - HALFLINE,
        width: 4.0,
        height: 4.0,
    });
    out.render_path(&icon_segs_in1, &Some(GRAY_PEN), &None);

    let icon_segs_in2: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 7.0 - HALFLINE,
        top: CY + 0.0 + HALFLINE,
        width: 4.0,
        height: 4.0,
    });
    out.render_path(&icon_segs_in2, &Some(GRAY_PEN), &None);

    let icon_segs_out: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX + 4.0 - HALFLINE,
        top: CY - 2.0 - HALFLINE,
        width: 4.0,
        height: 4.0,
    });
    out.render_path(&icon_segs_out, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_requirement
///
pub fn generate_type_clas_requirement(out: &mut dyn PathRenderer) -> () {
    let icon_segs_goal: [geometry::DrawDirective; 6] = [
        Move(Point { x: 10.0, y: 23.0 }),
        Line(Point { x: 10.0, y: 1.0 }),
        Move(Point { x: 13.0, y: 3.0 }),
        Line(Point {
            x: 23.0,
            y: 6.0 + HALFLINE,
        }),
        Line(Point { x: 13.0, y: 10.0 }),
        Close,
    ];
    out.render_path(&icon_segs_goal, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_artifact
///
pub fn generate_type_clas_artifact(out: &mut dyn PathRenderer) -> () {
    let icon_paper: [geometry::DrawDirective; 7] = [
        Move(Point {
            x: CX + 7.0,
            y: CY - 2.0,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY + 10.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY + 10.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY - 10.0,
        }),
        Line(Point {
            x: CX - 1.0,
            y: CY - 10.0,
        }),
        Line(Point {
            x: CX + 1.0,
            y: CY - 4.0,
        }),
        Close,
    ];
    out.render_path(&icon_paper, &Some(GRAY_THICK_PEN), &Some(GRAY));

    let icon_edge: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 1.0 + HALFLINE,
            y: CY - 10.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 7.0 + HALFLINE,
            y: CY - 2.0 - HALFLINE,
        }),
    ];
    out.render_path(&icon_edge, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_comment
///
pub fn generate_type_clas_comment(out: &mut dyn PathRenderer) -> () {
    let icon_paper: [geometry::DrawDirective; 7] = [
        Move(Point {
            x: CX + 7.0,
            y: CY - 2.0,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY + 10.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY + 10.0,
        }),
        Line(Point {
            x: CX - 7.0,
            y: CY - 10.0,
        }),
        Line(Point {
            x: CX - 1.0,
            y: CY - 10.0,
        }),
        Line(Point {
            x: CX + 1.0,
            y: CY - 4.0,
        }),
        Close,
    ];
    out.render_path(&icon_paper, &Some(GRAY_THICK_PEN), &None);

    let icon_edge: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: CX - 1.0 + HALFLINE,
            y: CY - 10.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 7.0 + HALFLINE,
            y: CY - 2.0 - HALFLINE,
        }),
    ];
    out.render_path(&icon_edge, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_actor
///
pub fn generate_type_clas_actor(out: &mut dyn PathRenderer) -> () {
    /* head */
    let icon_head: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY - 7.0 }, 3.0, 3.0);
    out.render_path(&icon_head, &Some(GRAY_THICK_PEN), &None);

    let icon_body: [geometry::DrawDirective; 8] = [
        Move(Point {
            x: CX - 5.0,
            y: CY + 10.0,
        }),
        Line(Point { x: CX, y: CY + 1.0 }),
        Line(Point { x: CX, y: CY - 3.0 }),
        Move(Point {
            x: CX + 5.0,
            y: CY + 10.0,
        }),
        Line(Point { x: CX, y: CY + 1.0 }),
        Move(Point {
            x: CX - 6.0,
            y: CY + 1.0,
        }),
        Line(Point { x: CX, y: CY - 2.0 }),
        Line(Point {
            x: CX + 6.0,
            y: CY + 1.0,
        }),
    ];
    out.render_path(&icon_body, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_interaction_use
///
pub fn generate_type_clas_interaction_use(out: &mut dyn PathRenderer) -> () {
    /* arrows */
    let icon_arrows: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX + 10.0,
            y: 9.0,
        }),
        Line(Point {
            x: CX - 9.0,
            y: 12.0,
        }),
        Line(Point {
            x: CX - 6.0,
            y: 9.0,
        }),
        Move(Point {
            x: CX - 10.0,
            y: 16.0,
        }),
        Line(Point {
            x: CX + 9.0,
            y: 19.0,
        }),
        Line(Point {
            x: CX + 6.0,
            y: 16.0,
        }),
    ];
    out.render_path(&icon_arrows, &Some(GRAY_THICK_PEN), &None);

    let icon_diagref: [geometry::DrawDirective; 9] = [
        Move(Point {
            x: CX - 14.0 - HALFLINE,
            y: CY - 4.0,
        }),
        Line(Point {
            x: CX - 14.0 - HALFLINE,
            y: CY - 8.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 14.0 + HALFLINE,
            y: CY - 8.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 14.0 + HALFLINE,
            y: CY + 10.0 + HALFLINE,
        }),
        Line(Point {
            x: CX - 14.0 - HALFLINE,
            y: CY + 10.0 + HALFLINE,
        }),
        Line(Point {
            x: CX - 14.0 - HALFLINE,
            y: CY - 5.0 - HALFLINE,
        }),
        Line(Point {
            x: CX - 8.0,
            y: CY - 5.0 - HALFLINE,
        }),
        Line(Point {
            x: CX - 6.0 - HALFLINE,
            y: CY - 7.0,
        }),
        Line(Point {
            x: CX - 6.0 - HALFLINE,
            y: CY - 8.0,
        }),
    ];
    out.render_path(&icon_diagref, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_accept
///
pub fn generate_type_clas_accept(out: &mut dyn PathRenderer) -> () {
    let icon_body: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 12.0,
            y: CY - 5.0,
        }),
        Line(Point {
            x: CX + 14.0,
            y: CY - 5.0,
        }),
        Line(Point {
            x: CX + 14.0,
            y: CY + 5.0,
        }),
        Line(Point {
            x: CX - 12.0,
            y: CY + 5.0,
        }),
        Line(Point { x: CX - 5.0, y: CY }),
        Close,
    ];
    out.render_path(&icon_body, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_activity
///
pub fn generate_type_clas_activity(out: &mut dyn PathRenderer) -> () {
    let icon_rect: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 12.0,
            top: CY - 8.0,
            width: 24.0,
            height: 16.0,
        },
        5.0,
    );
    out.render_path(&icon_rect, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_decision
///
pub fn generate_type_clas_decision(out: &mut dyn PathRenderer) -> () {
    let icon_body: [geometry::DrawDirective; 5] = [
        Move(Point {
            x: CX - HALFLINE,
            y: CY - 9.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 9.0 - HALFLINE,
            y: CY - HALFLINE,
        }),
        Line(Point {
            x: CX - HALFLINE,
            y: CY + 9.0 - HALFLINE,
        }),
        Line(Point {
            x: CX - 9.0 - HALFLINE,
            y: CY - HALFLINE,
        }),
        Close,
    ];
    out.render_path(&icon_body, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_deephistory
///
pub fn generate_type_clas_deephistory(out: &mut dyn PathRenderer) -> () {
    let icon_segs_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY }, 11.0 + HALFLINE, 11.0 + HALFLINE);
    out.render_path(&icon_segs_circle, &Some(GRAY_PEN), &None);

    let icon_body: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 6.0,
            y: CY - 6.0,
        }),
        Line(Point {
            x: CX - 6.0,
            y: CY + 6.0,
        }),
        Move(Point { x: CX - 5.0, y: CY }),
        Line(Point { x: CX - 1.0, y: CY }),
        Move(Point {
            x: CX + 0.0,
            y: CY - 6.0,
        }),
        Line(Point {
            x: CX + 0.0,
            y: CY + 6.0,
        }),
    ];
    out.render_path(&icon_body, &Some(GRAY_THICK_PEN), &None);

    let icon_asterisk: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX + 2.0,
            y: CY - 2.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY - 6.0 - HALFLINE,
        }),
        Move(Point {
            x: CX + 2.0,
            y: CY - 6.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 7.0,
            y: CY - 2.0 - HALFLINE,
        }),
        Move(Point {
            x: CX + 4.0 + HALFLINE,
            y: CY - 8.0,
        }),
        Line(Point {
            x: CX + 4.0 + HALFLINE,
            y: CY - 1.0,
        }),
    ];
    out.render_path(&icon_asterisk, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_final
///
pub fn generate_type_clas_final(out: &mut dyn PathRenderer) -> () {
    let icon_segs_end_dot: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY }, 6.0, 6.0);
    out.render_path(&icon_segs_end_dot, &None, &Some(GRAY));
    let icon_segs_end_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY }, 9.0, 9.0);
    out.render_path(&icon_segs_end_circle, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_fork
///
pub fn generate_type_clas_fork(out: &mut dyn PathRenderer) -> () {
    let icon_base_rect: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 2.0,
        top: CY - 11.0,
        width: 4.0,
        height: 21.0,
    });
    out.render_path(&icon_base_rect, &None, &Some(GRAY));

    let icon_body: [geometry::DrawDirective; 9] = [
        Move(Point {
            x: CX + 5.0,
            y: CY - 6.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 14.0,
            y: CY - 6.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 11.0 + HALFLINE,
            y: CY - 8.0 - HALFLINE,
        }),
        Move(Point {
            x: CX + 5.0,
            y: CY + 5.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + 14.0,
            y: CY + 5.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + 11.0 + HALFLINE,
            y: CY + 7.0 + HALFLINE,
        }),
        Move(Point {
            x: CX - 14.0,
            y: CY - HALFLINE,
        }),
        Line(Point {
            x: CX - 5.0,
            y: CY - HALFLINE,
        }),
        Line(Point {
            x: CX - 8.0 + HALFLINE,
            y: CY - 2.0 - HALFLINE,
        }),
    ];
    out.render_path(&icon_body, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_history
///
pub fn generate_type_clas_history(out: &mut dyn PathRenderer) -> () {
    let icon_segs_circle: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY }, 11.0 + HALFLINE, 11.0 + HALFLINE);
    out.render_path(&icon_segs_circle, &Some(GRAY_PEN), &None);

    let icon_body: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 3.0,
            y: CY - 6.0,
        }),
        Line(Point {
            x: CX - 3.0,
            y: CY + 6.0,
        }),
        Move(Point { x: CX - 2.0, y: CY }),
        Line(Point { x: CX + 2.0, y: CY }),
        Move(Point {
            x: CX + 3.0,
            y: CY - 6.0,
        }),
        Line(Point {
            x: CX + 3.0,
            y: CY + 6.0,
        }),
    ];
    out.render_path(&icon_body, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_initial
///
pub fn generate_type_clas_initial(out: &mut dyn PathRenderer) -> () {
    let icon_segs_start: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY }, 7.0, 7.0);
    out.render_path(&icon_segs_start, &None, &Some(GRAY));
}

/// The function generates a type_clas_join
///
pub fn generate_type_clas_join(out: &mut dyn PathRenderer) -> () {
    let icon_base_rect: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: CX - 2.0,
        top: CY - 11.0,
        width: 4.0,
        height: 21.0,
    });
    out.render_path(&icon_base_rect, &None, &Some(GRAY));

    let icon_body: [geometry::DrawDirective; 9] = [
        Move(Point {
            x: CX - 14.0,
            y: CY - 6.0 - HALFLINE,
        }),
        Line(Point {
            x: CX - 5.0,
            y: CY - 6.0 - HALFLINE,
        }),
        Line(Point {
            x: CX - 8.0 - HALFLINE,
            y: CY - 8.0 - HALFLINE,
        }),
        Move(Point {
            x: CX - 14.0,
            y: CY + 5.0 + HALFLINE,
        }),
        Line(Point {
            x: CX - 5.0,
            y: CY + 5.0 + HALFLINE,
        }),
        Line(Point {
            x: CX - 8.0 - HALFLINE,
            y: CY + 7.0 + HALFLINE,
        }),
        Move(Point {
            x: CX + 5.0,
            y: CY - HALFLINE,
        }),
        Line(Point {
            x: CX + 14.0,
            y: CY - HALFLINE,
        }),
        Line(Point {
            x: CX + 11.0 - HALFLINE,
            y: CY - 2.0 - HALFLINE,
        }),
    ];
    out.render_path(&icon_body, &Some(GRAY_PEN), &None);
}

/// The function generates a type_clas_region
///
pub fn generate_type_clas_region(out: &mut dyn PathRenderer) -> () {
    let icon_body: [geometry::DrawDirective; 12] = [
        Move(Point {
            x: CX - 3.0,
            y: CY - 8.0,
        }),
        Line(Point {
            x: CX + 3.0,
            y: CY - 8.0,
        }),
        Move(Point {
            x: CX + 8.0,
            y: CY - 8.0,
        }),
        Curve(
            Point {
                x: CX + 11.0,
                y: CY - 8.0,
            },
            Point {
                x: CX + 13.0,
                y: CY - 6.0,
            },
            Point {
                x: CX + 13.0,
                y: CY - 3.0,
            },
        ),
        Move(Point {
            x: CX + 13.0,
            y: CY + 3.0,
        }),
        Curve(
            Point {
                x: CX + 13.0,
                y: CY + 6.0,
            },
            Point {
                x: CX + 11.0,
                y: CY + 8.0,
            },
            Point {
                x: CX + 8.0,
                y: CY + 8.0,
            },
        ),
        Move(Point {
            x: CX + 3.0,
            y: CY + 8.0,
        }),
        Line(Point {
            x: CX - 3.0,
            y: CY + 8.0,
        }),
        Move(Point {
            x: CX - 8.0,
            y: CY + 8.0,
        }),
        Curve(
            Point {
                x: CX - 11.0,
                y: CY + 8.0,
            },
            Point {
                x: CX - 13.0,
                y: CY + 6.0,
            },
            Point {
                x: CX - 13.0,
                y: CY + 3.0,
            },
        ),
        Move(Point {
            x: CX - 13.0,
            y: CY - 3.0,
        }),
        Curve(
            Point {
                x: CX - 13.0,
                y: CY - 6.0,
            },
            Point {
                x: CX - 11.0,
                y: CY - 8.0,
            },
            Point {
                x: CX - 8.0,
                y: CY - 8.0,
            },
        ),
    ];
    out.render_path(&icon_body, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_send
///
pub fn generate_type_clas_send(out: &mut dyn PathRenderer) -> () {
    let icon_body: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: CX - 14.0,
            y: CY - 5.0,
        }),
        Line(Point {
            x: CX + 6.0,
            y: CY - 5.0,
        }),
        Line(Point {
            x: CX + 13.0,
            y: CY,
        }),
        Line(Point {
            x: CX + 6.0,
            y: CY + 5.0,
        }),
        Line(Point {
            x: CX - 14.0,
            y: CY + 5.0,
        }),
        Close,
    ];
    out.render_path(&icon_body, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_state
///
pub fn generate_type_clas_state(out: &mut dyn PathRenderer) -> () {
    let icon_rect: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 9.0,
            top: CY - 7.0,
            width: 18.0,
            height: 14.0,
        },
        5.0,
    );
    out.render_path(&icon_rect, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_time
///
pub fn generate_type_clas_time(out: &mut dyn PathRenderer) -> () {
    let icon_body: [geometry::DrawDirective; 6] = [
        Move(Point { x: CX, y: CY }),
        Line(Point {
            x: CX - 6.0,
            y: CY - 8.0,
        }),
        Curve(
            Point {
                x: CX - 9.0,
                y: CY - 12.0,
            },
            Point {
                x: CX + 9.0,
                y: CY - 12.0,
            },
            Point {
                x: CX + 6.0,
                y: CY - 8.0,
            },
        ),
        Line(Point {
            x: CX - 6.0,
            y: CY + 8.0,
        }),
        Curve(
            Point {
                x: CX - 9.0,
                y: CY + 12.0,
            },
            Point {
                x: CX + 9.0,
                y: CY + 12.0,
            },
            Point {
                x: CX + 6.0,
                y: CY + 8.0,
            },
        ),
        Close,
    ];
    out.render_path(&icon_body, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_clas_usecase
///
pub fn generate_type_clas_usecase(out: &mut dyn PathRenderer) -> () {
    let icon_segs_use_case: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY }, 14.0, 8.0);
    out.render_path(&icon_segs_use_case, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_undef
///
pub fn generate_type_undef(out: &mut dyn PathRenderer) -> () {
    let icon_body: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 4.0 - HALFLINE,
            y: CY - 4.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 4.0 - HALFLINE,
            y: CY + 4.0 - HALFLINE,
        }),
        Move(Point {
            x: CX - 4.0 - HALFLINE,
            y: CY + 4.0 - HALFLINE,
        }),
        Line(Point {
            x: CX + 4.0 - HALFLINE,
            y: CY - 4.0 - HALFLINE,
        }),
    ];
    out.render_path(&icon_body, &Some(GRAY_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "type_clas_stereotype",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_stereotype,
        },
        IconSource {
            name: "type_clas_image",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_image,
        },
        IconSource {
            name: "type_clas_block",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_block,
        },
        IconSource {
            name: "type_clas_subsystem",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_subsystem,
        },
        IconSource {
            name: "type_clas_component",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_component,
        },
        IconSource {
            name: "type_clas_part",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_part,
        },
        IconSource {
            name: "type_clas_class",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_class,
        },
        IconSource {
            name: "type_clas_object",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_object,
        },
        IconSource {
            name: "type_clas_interface",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_interface,
        },
        IconSource {
            name: "type_clas_node",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_node,
        },
        IconSource {
            name: "type_clas_package",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_package,
        },
        IconSource {
            name: "type_clas_constraint",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_constraint,
        },
        IconSource {
            name: "type_clas_requirement",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_requirement,
        },
        IconSource {
            name: "type_clas_artifact",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_artifact,
        },
        IconSource {
            name: "type_clas_comment",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_comment,
        },
        IconSource {
            name: "type_clas_actor",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_actor,
        },
        IconSource {
            name: "type_clas_interaction_use",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_interaction_use,
        },
        IconSource {
            name: "type_clas_accept",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_accept,
        },
        IconSource {
            name: "type_clas_activity",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_activity,
        },
        IconSource {
            name: "type_clas_decision",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_decision,
        },
        IconSource {
            name: "type_clas_deephistory",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_deephistory,
        },
        IconSource {
            name: "type_clas_final",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_final,
        },
        IconSource {
            name: "type_clas_fork",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_fork,
        },
        IconSource {
            name: "type_clas_history",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_history,
        },
        IconSource {
            name: "type_clas_initial",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_initial,
        },
        IconSource {
            name: "type_clas_join",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_join,
        },
        IconSource {
            name: "type_clas_region",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_region,
        },
        IconSource {
            name: "type_clas_send",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_send,
        },
        IconSource {
            name: "type_clas_state",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_state,
        },
        IconSource {
            name: "type_clas_time",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_time,
        },
        IconSource {
            name: "type_clas_usecase",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_usecase,
        },
        IconSource {
            name: "type_undef",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_undef,
        },
    ]
}

/*
 * Copyright 2023-2025 Andreas Warnke
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
