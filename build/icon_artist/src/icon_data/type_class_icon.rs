//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use super::shape::get_rect_abs;
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

/*
 * STATIC ELEMENTS:
 gui/source/resources/type_clas_actor.c  -- only for behavioral views
 gui/source/resources/type_clas_artifact.c
 gui/source/resources/type_clas_comment.c  -- also for behavioral views
 * DYNAMIC ARTIFACTS;
 gui/source/resources/type_clas_accept.c
 gui/source/resources/type_clas_activity.c
 gui/source/resources/type_clas_decision.c
 gui/source/resources/type_clas_deephistory.c
 gui/source/resources/type_clas_final.c
 gui/source/resources/type_clas_fork.c
 gui/source/resources/type_clas_history.c
 gui/source/resources/type_clas_initial.c
 gui/source/resources/type_clas_interaction_use.c
 gui/source/resources/type_clas_join.c
 gui/source/resources/type_clas_region.c
 gui/source/resources/type_clas_send.c
 gui/source/resources/type_clas_state.c
 gui/source/resources/type_clas_time.c
 gui/source/resources/type_clas_usecase.c
*/

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
