//! The module provides functions to render an icon to vector graphics.

use super::geometry;
use super::geometry::get_circle_abs;
use super::geometry::Color;
use super::geometry::DrawDirective::CloseRel;
use super::geometry::DrawDirective::Curve;
use super::geometry::DrawDirective::CurveRel;
use super::geometry::DrawDirective::Line;
use super::geometry::DrawDirective::LineRel;
use super::geometry::DrawDirective::Move;
use super::geometry::DrawDirective::MoveRel;
use super::geometry::Offset;
use super::geometry::Point;
use super::geometry::Rect;
use super::icon::IconSource;
use crate::render::render_svg::VecRenderer;

/// The view rectangle of each icon
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 32.0,
};

/// green fill color
static GREEN: Color = Color {
    red: 0x0,
    green: 0xff,
    blue: 0x99,
};

/// black color
static BLACK: Color = Color {
    red: 0x0,
    green: 0x0,
    blue: 0x0,
};

/// white color
static WHITE: Color = Color {
    red: 0xff,
    green: 0xff,
    blue: 0xff,
};

const BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE: f32 = 0.552284749831;

/// The function defines the draw directives for the file symbols contour
///
fn get_db_storage_contour() -> [geometry::DrawDirective; 9] {
    let x_rad: f32 = 10.0;
    let y_rad: f32 = 4.0;
    let height: f32 = 22.0;
    let center_x: f32 = 16.0;
    [
        MoveRel(Offset {
            dx: center_x - x_rad,
            dy: center_x - 0.5 * height,
        }),
        CurveRel(
            Offset {
                dx: 0.0,
                dy: -y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: x_rad - x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: -y_rad,
            },
            Offset {
                dx: x_rad,
                dy: -y_rad,
            },
        ),
        CurveRel(
            Offset {
                dx: x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: 0.0,
            },
            Offset {
                dx: x_rad,
                dy: y_rad - y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: x_rad,
                dy: y_rad,
            },
        ),
        LineRel(Offset {
            dx: 0.0,
            dy: height,
        }),
        CurveRel(
            Offset {
                dx: 0.0,
                dy: y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: -x_rad + x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: y_rad,
            },
            Offset {
                dx: -x_rad,
                dy: y_rad,
            },
        ),
        CurveRel(
            Offset {
                dx: -x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: 0.0,
            },
            Offset {
                dx: -x_rad,
                dy: -y_rad + y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: -x_rad,
                dy: -y_rad,
            },
        ),
        LineRel(Offset {
            dx: 0.0,
            dy: -height,
        }),
        CurveRel(
            Offset {
                dx: 0.0,
                dy: y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: x_rad - x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: y_rad,
            },
            Offset {
                dx: x_rad,
                dy: y_rad,
            },
        ),
        CurveRel(
            Offset {
                dx: x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: 0.0,
            },
            Offset {
                dx: x_rad,
                dy: -y_rad + y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: x_rad,
                dy: -y_rad,
            },
        ),
    ]
}

/// The function defines the draw directives for the file symbols shadows
///
fn get_db_storage_shadows() -> [geometry::DrawDirective; 12] {
    let x_rad: f32 = 10.0;
    let y_rad: f32 = 4.0;
    let height: f32 = 22.0;
    let center_x: f32 = 16.0;
    let step1_angle: f32 = std::f32::consts::PI * 1.2;
    let step1_dx = step1_angle.cos() * x_rad;
    let step1_dy = step1_angle.sin() * -y_rad;
    let step2_angle: f32 = std::f32::consts::PI * 1.4;
    let step2_dx = step2_angle.cos() * x_rad;
    let step2_dy = step2_angle.sin() * -y_rad;
    let step3_angle: f32 = std::f32::consts::PI * 1.8;
    let step3_dx = step3_angle.cos() * x_rad;
    let step3_dy = step3_angle.sin() * -y_rad;
    [
        MoveRel(Offset {
            dx: center_x - x_rad,
            dy: 16.0 - 0.5 * height,
        }),
        LineRel(Offset {
            dx: x_rad + step1_dx,
            dy: step1_dy,
        }),
        LineRel(Offset {
            dx: step2_dx - step1_dx,
            dy: step2_dy - step1_dy,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: height,
        }),
        LineRel(Offset {
            dx: -step2_dx + step1_dx,
            dy: -step2_dy + step1_dy,
        }),
        LineRel(Offset {
            dx: -step1_dx - x_rad,
            dy: -step1_dy,
        }),
        CloseRel,
        MoveRel(Offset {
            dx: 2.0 * x_rad,
            dy: 0.0,
        }),
        LineRel(Offset {
            dx: -x_rad + step3_dx,
            dy: step3_dy,
        }),
        LineRel(Offset {
            dx: 0.0,
            dy: height,
        }),
        LineRel(Offset {
            dx: -step3_dx + x_rad,
            dy: -step3_dy,
        }),
        CloseRel,
    ]
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_file_new_db(out: &mut VecRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.path(&icon_segs, &Some(BLACK), &None);
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_file_use_db(out: &mut VecRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.path(&icon_segs, &Some(BLACK), &None);
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_file_save(out: &mut VecRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.path(&icon_segs, &Some(BLACK), &None);
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_file_export(out: &mut VecRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = get_db_storage_shadows();
    out.path(&icon_segs, &None, &Some(GREEN));

    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.path(&icon_segs, &Some(BLACK), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "file_new_db",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_new_db,
        },
        IconSource {
            name: "file_use_db",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_use_db,
        },
        IconSource {
            name: "file_save",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_save,
        },
        IconSource {
            name: "file_export",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_export,
        },
    ]
}
