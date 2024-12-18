//! The module provides functions to render an icon to vector graphics.

use super::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::CloseRel;
use crate::stream_if::geometry::DrawDirective::CurveRel;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::DrawDirective::MoveRel;
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

/// green fill color
static GREEN: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0xff,
    blue: 0x99,
};

/// black color
static BLACK: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0x0,
    blue: 0x0,
};

/// white color
static WHITE: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0xff,
    blue: 0xff,
};

/// gray color
static GRAY: geometry::Color = geometry::Color {
    red: 0x7f,
    green: 0x7f,
    blue: 0x7f,
};

/// black pen
static BLACK_PEN: geometry::Pen = geometry::Pen {
    color: BLACK,
    width: 1.0,
};

/// white pen
static WHITE_PEN: geometry::Pen = geometry::Pen {
    color: WHITE,
    width: 1.0,
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
    let step1_angle: f32 = std::f32::consts::PI * 1.15;
    let step1_dx = step1_angle.cos() * x_rad;
    let step1_dy = step1_angle.sin() * -y_rad;
    let step2_angle: f32 = std::f32::consts::PI * 1.3;
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

/// The function defines the draw directives for the file symbols reflection
///
fn get_db_storage_reflection() -> [geometry::DrawDirective; 5] {
    let x_rad: f32 = 10.0;
    let y_rad: f32 = 4.0;
    let height: f32 = 22.0;
    let center_x: f32 = 16.0;
    let step1_angle: f32 = std::f32::consts::PI * 1.5;
    let step1_dx = step1_angle.cos() * x_rad;
    let step1_dy = step1_angle.sin() * -y_rad;
    let step2_angle: f32 = std::f32::consts::PI * 1.6;
    let step2_dx = step2_angle.cos() * x_rad;
    let step2_dy = step2_angle.sin() * -y_rad;
    [
        MoveRel(Offset {
            dx: center_x + step1_dx,
            dy: 16.0 - 0.5 * height + step1_dy,
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
        CloseRel,
    ]
}

/// The function generates a new-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_new(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);

    /* plus symbol */
    let plus_sym: [geometry::DrawDirective; 4] = [
        Move(Point { x: 12.5, y: 19.0 }),
        Line(Point { x: 19.5, y: 19.0 }),
        Move(Point { x: 16.0, y: 15.5 }),
        Line(Point { x: 16.0, y: 22.5 }),
    ];
    out.render_path(&plus_sym, &Some(BLACK_PEN), &None);
}

/// The function generates an open-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_open(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = get_db_storage_shadows();
    out.render_path(&icon_segs, &None, &Some(GRAY));

    let icon_segs: [geometry::DrawDirective; 5] = get_db_storage_reflection();
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);
}

/// The function generates a save-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_save(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = get_db_storage_shadows();
    out.render_path(&icon_segs, &None, &Some(GRAY));

    let icon_segs: [geometry::DrawDirective; 5] = get_db_storage_reflection();
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);

    /* ok symbol */
    let unsaved_sym: [geometry::DrawDirective; 16] = [
        MoveRel(Offset { dx: 19.5, dy: 11.5 }),
        LineRel(Offset { dx: 3.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: 7.0 }),
        LineRel(Offset { dx: 8.0, dy: -2.0 }),
        LineRel(Offset { dx: 0.0, dy: 3.0 }),
        LineRel(Offset { dx: -7.0, dy: 2.0 }),
        LineRel(Offset { dx: 4.0, dy: 7.0 }),
        LineRel(Offset { dx: -3.0, dy: 1.0 }),
        LineRel(Offset { dx: -3.5, dy: -6.0 }),
        LineRel(Offset { dx: -3.5, dy: 6.0 }),
        LineRel(Offset { dx: -3.0, dy: -1.0 }),
        LineRel(Offset { dx: 4.0, dy: -7.0 }),
        LineRel(Offset { dx: -7.0, dy: -2.0 }),
        LineRel(Offset { dx: 0.0, dy: -3.0 }),
        LineRel(Offset { dx: 8.0, dy: 2.0 }),
        CloseRel,
    ];
    out.render_path(&unsaved_sym, &Some(WHITE_PEN), &Some(GRAY));
}

/// The function generates a saved-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_saved(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = get_db_storage_shadows();
    out.render_path(&icon_segs, &None, &Some(GRAY));

    let icon_segs: [geometry::DrawDirective; 5] = get_db_storage_reflection();
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);

    /* ok symbol */
    let ok_sym: [geometry::DrawDirective; 8] = [
        Move(Point { x: 13.0, y: 20.0 }),
        Line(Point { x: 17.0, y: 20.0 }),
        Line(Point { x: 18.5, y: 23.5 }),
        Line(Point { x: 24.0, y: 12.0 }),
        Line(Point { x: 25.0, y: 12.5 }),
        Line(Point { x: 21.0, y: 26.5 }),
        Line(Point { x: 16.0, y: 27.0 }),
        Close,
    ];
    out.render_path(&ok_sym, &Some(BLACK_PEN), &Some(GREEN));
}

/// The function generates an export-files icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_export(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = get_db_storage_shadows();
    out.render_path(&icon_segs, &None, &Some(GRAY));

    let icon_segs: [geometry::DrawDirective; 5] = get_db_storage_reflection();
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour();
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);

    /* out symbol */
    let out_sym: [geometry::DrawDirective; 8] = [
        MoveRel(Offset { dx: 13.0, dy: 16.0 }),
        LineRel(Offset { dx: 10.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: -5.0 }),
        LineRel(Offset { dx: 6.0, dy: 8.0 }),
        LineRel(Offset { dx: -6.0, dy: 8.0 }),
        LineRel(Offset { dx: 0.0, dy: -5.0 }),
        LineRel(Offset { dx: -10.0, dy: 0.0 }),
        CloseRel,
    ];
    out.render_path(&out_sym, &Some(BLACK_PEN), &Some(GREEN));
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "file_new",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_new,
        },
        IconSource {
            name: "file_open",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_open,
        },
        IconSource {
            name: "file_save",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_save,
        },
        IconSource {
            name: "file_saved",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_saved,
        },
        IconSource {
            name: "file_export",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_export,
        },
    ]
}
