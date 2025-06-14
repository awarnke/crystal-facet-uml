//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use super::shape::get_rect_abs;
use super::shape::get_rounded_rect_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::CloseRel;
use crate::stream_if::geometry::DrawDirective::Curve;
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

/// The function defines the path points for a print of an animal paw
///
/// # Arguments
///
/// * `center` - The center point of the paw
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
        Move(Point { x: 20.0, y: CY }),
        Line(Point { x: 10.0, y: CY }),
        Move(Point {
            x: 1.0,
            y: CY - 5.0,
        }),
        Curve(
            Point {
                x: 4.0,
                y: CY - 5.0,
            },
            Point {
                x: 6.0,
                y: CY - 3.0,
            },
            Point { x: 6.0, y: CY },
        ),
        Curve(
            Point {
                x: 6.0,
                y: CY + 3.0,
            },
            Point {
                x: 4.0,
                y: CY + 5.0,
            },
            Point {
                x: 1.0,
                y: CY + 5.0,
            },
        ),
    ];
    out.render_path(&icon_segs_4_ends, &Some(GRAY_THICK_PEN), &None);

    let arrow_tip: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 31.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
    ];
    out.render_path(&arrow_tip, &Some(GRAY_THICK_PEN), &None);
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

    let arrow_tip: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 31.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
    ];
    out.render_path(&arrow_tip, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_extend
///
pub fn generate_type_rel_extend(out: &mut dyn PathRenderer) -> () {
    let icon_segs_ellipsis: [geometry::DrawDirective; 5] = [
        MoveRel(Offset {
            dx: 22.0 + HALFLINE,
            dy: 1.0 + HALFLINE,
        }),
        CurveRel(
            Offset { dx: 3.9, dy: 0.0 },
            Offset { dx: 7.0, dy: 2.2 },
            Offset { dx: 7.0, dy: 5.0 },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 2.8 },
            Offset { dx: -3.1, dy: 5.0 },
            Offset { dx: -7.0, dy: 5.0 },
        ),
        CurveRel(
            Offset { dx: -3.9, dy: 0.0 },
            Offset { dx: -7.0, dy: -2.2 },
            Offset { dx: -7.0, dy: -5.0 },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: -2.8 },
            Offset { dx: 3.1, dy: -5.0 },
            Offset { dx: 7.0, dy: -5.0 },
        ),
    ];
    out.render_path(&icon_segs_ellipsis, &Some(GRAY_PEN), &None);

    let icon_segs_part_ellipsis: [geometry::DrawDirective; 5] = [
        MoveRel(Offset { dx: 2.0, dy: 16.0 }),
        CurveRel(
            Offset { dx: 0.0, dy: -3.2 },
            Offset { dx: 3.8, dy: -6.0 },
            Offset { dx: 7.0, dy: -6.0 },
        ),
        CurveRel(
            Offset { dx: -1.0, dy: 4.2 },
            Offset { dx: 3.8, dy: 8.0 },
            Offset { dx: 9.0, dy: 7.0 },
        ),
        CurveRel(
            Offset { dx: 0.0, dy: 2.2 },
            Offset { dx: -3.8, dy: 5.0 },
            Offset { dx: -8.0, dy: 5.0 },
        ),
        CurveRel(
            Offset { dx: -4.2, dy: 0.0 },
            Offset { dx: -8.0, dy: -2.8 },
            Offset { dx: -8.0, dy: -6.0 },
        ),
    ];
    out.render_path(&icon_segs_part_ellipsis, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_include
///
pub fn generate_type_rel_include(out: &mut dyn PathRenderer) -> () {
    let icon_segs_ellipsis: [geometry::DrawDirective; 4] = [
        MoveRel(Offset {
            dx: 19.0 + HALFLINE,
            dy: 1.0 + HALFLINE,
        }),
        CurveRel(
            Offset { dx: 6.1, dy: 0.0 },
            Offset { dx: 11.0, dy: 3.6 },
            Offset { dx: 11.0, dy: 8.0 },
        ),
        LineRel(Offset { dx: -11.0, dy: 0.0 }),
        CloseRel,
    ];
    out.render_path(&icon_segs_ellipsis, &Some(GRAY_PEN), &None);

    let icon_segs_part_ellipsis: [geometry::DrawDirective; 6] = [
        MoveRel(Offset { dx: 2.0, dy: 15.0 }),
        CurveRel(
            Offset { dx: 0.0, dy: -3.9 },
            Offset { dx: 4.5, dy: -7.0 },
            Offset { dx: 10.0, dy: -7.0 },
        ),
        LineRel(Offset { dx: 0.0, dy: 7.0 }),
        LineRel(Offset { dx: 10.0, dy: 0.0 }),
        CurveRel(
            Offset { dx: 0.0, dy: 3.9 },
            Offset { dx: -4.5, dy: 7.0 },
            Offset { dx: -10.0, dy: 7.0 },
        ),
        CurveRel(
            Offset { dx: -5.5, dy: 0.0 },
            Offset {
                dx: -10.0,
                dy: -3.1,
            },
            Offset {
                dx: -10.0,
                dy: -7.0,
            },
        ),
    ];
    out.render_path(&icon_segs_part_ellipsis, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_deploy
///
pub fn generate_type_rel_deploy(out: &mut dyn PathRenderer) -> () {
    /* circles */
    let icon_rear2_wheel: [geometry::DrawDirective; 5] = get_circle_abs(
        Point {
            x: 6.0 + HALFLINE,
            y: 19.0 + HALFLINE,
        },
        2.0,
        2.0,
    );
    out.render_path(&icon_rear2_wheel, &Some(GRAY_PEN), &None);
    let icon_rear1_wheel: [geometry::DrawDirective; 5] = get_circle_abs(
        Point {
            x: 12.0 + HALFLINE,
            y: 19.0 + HALFLINE,
        },
        2.0,
        2.0,
    );
    out.render_path(&icon_rear1_wheel, &Some(GRAY_PEN), &None);
    let icon_front_wheel: [geometry::DrawDirective; 5] = get_circle_abs(
        Point {
            x: 26.0 + HALFLINE,
            y: 19.0 + HALFLINE,
        },
        2.0,
        2.0,
    );
    out.render_path(&icon_front_wheel, &Some(GRAY_PEN), &None);

    let icon_chassis: [geometry::DrawDirective; 8] = [
        Move(Point { x: 1.0, y: 16.0 }),
        Line(Point { x: 3.0, y: 16.0 }),
        Move(Point { x: 15.0, y: 16.0 }),
        Line(Point { x: 23.0, y: 16.0 }),
        Line(Point { x: 23.0, y: 6.0 }),
        Line(Point { x: 28.0, y: 6.0 }),
        Line(Point { x: 30.0, y: 11.0 }),
        Line(Point { x: 30.0, y: 17.0 }),
    ];
    out.render_path(&icon_chassis, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_manifest
///
pub fn generate_type_rel_manifest(out: &mut dyn PathRenderer) -> () {
    /* artifact */
    let icon_manifest: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 6.0 - HALFLINE,
            top: 3.0 - HALFLINE,
            width: 12.0,
            height: 12.0,
        },
        3.0,
    );
    out.render_path(&icon_manifest, &Some(GRAY_PEN), &None);

    let icon_chassis: [geometry::DrawDirective; 8] = [
        Move(Point { x: 5.0, y: 18.0 }),
        Line(Point { x: 26.0, y: 18.0 }),
        Move(Point { x: 5.0, y: 21.0 }),
        Line(Point { x: 8.0, y: 21.0 }),
        Move(Point { x: 14.0, y: 21.0 }),
        Line(Point { x: 17.0, y: 21.0 }),
        Move(Point { x: 23.0, y: 21.0 }),
        Line(Point { x: 26.0, y: 21.0 }),
    ];
    out.render_path(&icon_chassis, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_contain
///
pub fn generate_type_rel_contain(out: &mut dyn PathRenderer) -> () {
    /* container */
    let icon_container: [geometry::DrawDirective; 9] = get_rounded_rect_abs(
        Rect {
            left: CX - 12.0 - HALFLINE,
            top: 2.0 - HALFLINE,
            width: 25.0,
            height: 21.0,
        },
        5.0,
    );
    out.render_path(&icon_container, &Some(GRAY_PEN), &None);
    let icon_contained: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: CX, y: CY + 3.0 }, 6.0, 6.0);
    out.render_path(&icon_contained, &Some(GRAY_THICK_PEN), &None);

    let icon_chassis: [geometry::DrawDirective; 4] = [
        Move(Point { x: CX, y: CY - 1.0 }),
        Line(Point { x: CX, y: CY + 7.0 }),
        Move(Point {
            x: CX - 4.0,
            y: CY + 3.0,
        }),
        Line(Point {
            x: CX + 4.0,
            y: CY + 3.0,
        }),
    ];
    out.render_path(&icon_chassis, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_depend
///
pub fn generate_type_rel_depend(out: &mut dyn PathRenderer) -> () {
    let icon_dashed_line: [geometry::DrawDirective; 9] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 4.0, y: CY }),
        Move(Point { x: 9.0, y: CY }),
        Line(Point { x: 12.0, y: CY }),
        Move(Point { x: 17.0, y: CY }),
        Line(Point { x: 20.0, y: CY }),
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 30.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
    ];
    out.render_path(&icon_dashed_line, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_realize
///
pub fn generate_type_rel_realize(out: &mut dyn PathRenderer) -> () {
    let icon_dashed_line: [geometry::DrawDirective; 6] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 4.0, y: CY }),
        Move(Point { x: 9.0, y: CY }),
        Line(Point { x: 12.0, y: CY }),
        Move(Point { x: 17.0, y: CY }),
        Line(Point { x: 20.0, y: CY }),
    ];
    out.render_path(&icon_dashed_line, &Some(GRAY_THICK_PEN), &None);
    let icon_arrow_tip: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 30.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
        Close,
    ];
    out.render_path(&icon_arrow_tip, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_return
///
pub fn generate_type_rel_return(out: &mut dyn PathRenderer) -> () {
    let icon_dashed_line: [geometry::DrawDirective; 6] = [
        Move(Point { x: 12.0, y: CY }),
        Line(Point { x: 15.0, y: CY }),
        Move(Point { x: 20.0, y: CY }),
        Line(Point { x: 23.0, y: CY }),
        Move(Point { x: 28.0, y: CY }),
        Line(Point { x: 31.0, y: CY }),
    ];
    out.render_path(&icon_dashed_line, &Some(GRAY_THICK_PEN), &None);
    let icon_arrow_tip: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: 11.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 2.0, y: CY }),
        Line(Point {
            x: 11.0,
            y: CY + 7.0,
        }),
        Close,
    ];
    out.render_path(&icon_arrow_tip, &Some(GRAY_THICK_PEN), &Some(GRAY));
}

/// The function generates a type_rel_generalize
///
pub fn generate_type_rel_generalize(out: &mut dyn PathRenderer) -> () {
    let icon_line: [geometry::DrawDirective; 2] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 21.0, y: CY }),
    ];
    out.render_path(&icon_line, &Some(GRAY_THICK_PEN), &None);
    let icon_arrow_tip: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 30.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
        Close,
    ];
    out.render_path(&icon_arrow_tip, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_sync
///
pub fn generate_type_rel_sync(out: &mut dyn PathRenderer) -> () {
    let icon_line: [geometry::DrawDirective; 2] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 20.0, y: CY }),
    ];
    out.render_path(&icon_line, &Some(GRAY_THICK_PEN), &None);
    let icon_arrow_tip: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 30.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
        Close,
    ];
    out.render_path(&icon_arrow_tip, &Some(GRAY_THICK_PEN), &Some(GRAY));
}

/// The function generates a type_rel_async
///
pub fn generate_type_rel_async(out: &mut dyn PathRenderer) -> () {
    let icon_arrow: [geometry::DrawDirective; 5] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 30.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Move(Point {
            x: 24.0,
            y: CY + 5.0,
        }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
    ];
    out.render_path(&icon_arrow, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_control_flow
///
pub fn generate_type_rel_control_flow(out: &mut dyn PathRenderer) -> () {
    let icon_arrow: [geometry::DrawDirective; 5] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 30.0, y: CY }),
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 30.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
    ];
    out.render_path(&icon_arrow, &Some(GRAY_THICK_PEN), &None);

    let icon_control_cycles: [geometry::DrawDirective; 6] = [
        Move(Point {
            x: 4.0 + HALFLINE,
            y: CY - 3.0,
        }),
        Line(Point {
            x: 4.0 + HALFLINE,
            y: CY + 3.0,
        }),
        Move(Point {
            x: 11.0 + HALFLINE,
            y: CY - 3.0,
        }),
        Line(Point {
            x: 11.0 + HALFLINE,
            y: CY + 3.0,
        }),
        Move(Point {
            x: 18.0 + HALFLINE,
            y: CY - 3.0,
        }),
        Line(Point {
            x: 18.0 + HALFLINE,
            y: CY + 3.0,
        }),
    ];
    out.render_path(&icon_control_cycles, &Some(GRAY_PEN), &None);
}

/// The function generates a type_rel_associate
///
pub fn generate_type_rel_associate(out: &mut dyn PathRenderer) -> () {
    let icon_arrow: [geometry::DrawDirective; 5] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 30.0, y: CY }),
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 30.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
    ];
    out.render_path(&icon_arrow, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_communication_path
///
pub fn generate_type_rel_communication_path(out: &mut dyn PathRenderer) -> () {
    let icon_arrow: [geometry::DrawDirective; 2] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 31.0, y: CY }),
    ];
    out.render_path(&icon_arrow, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a type_rel_aggregate
///
pub fn generate_type_rel_aggregate(out: &mut dyn PathRenderer) -> () {
    let icon_arrow: [geometry::DrawDirective; 4] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 3.3, y: CY }),
        Move(Point { x: 15.1, y: CY }),
        Line(Point { x: 31.0, y: CY }),
    ];
    out.render_path(&icon_arrow, &Some(GRAY_THICK_PEN), &None);

    let icon_feathers: [geometry::DrawDirective; 5] = [
        Move(Point { x: 3.0, y: CY }),
        Line(Point {
            x: 7.0,
            y: CY - 4.0 - HALFLINE,
        }),
        Line(Point { x: 16.0, y: CY }),
        Line(Point {
            x: 7.0,
            y: CY + 4.0 + HALFLINE,
        }),
        Close,
    ];
    out.render_path(&icon_feathers, &Some(GRAY_PEN), &None);
}

/// The function generates a type_rel_compose
///
pub fn generate_type_rel_compose(out: &mut dyn PathRenderer) -> () {
    let icon_arrow: [geometry::DrawDirective; 4] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 3.3, y: CY }),
        Move(Point { x: 15.1, y: CY }),
        Line(Point { x: 31.0, y: CY }),
    ];
    out.render_path(&icon_arrow, &Some(GRAY_THICK_PEN), &None);

    let icon_feathers: [geometry::DrawDirective; 5] = [
        Move(Point { x: 3.0, y: CY }),
        Line(Point {
            x: 7.0,
            y: CY - 4.0 - HALFLINE,
        }),
        Line(Point { x: 16.0, y: CY }),
        Line(Point {
            x: 7.0,
            y: CY + 4.0 + HALFLINE,
        }),
        Close,
    ];
    out.render_path(&icon_feathers, &Some(GRAY_PEN), &Some(GRAY));
}

/// The function generates a type_rel_object_flow
///
pub fn generate_type_rel_object_flow(out: &mut dyn PathRenderer) -> () {
    /* object */
    let icon_object: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 7.0 - HALFLINE,
        top: 2.0 - HALFLINE,
        width: 7.0,
        height: 6.0,
    });
    out.render_path(&icon_object, &Some(GRAY_PEN), &None);

    let icon_arrow: [geometry::DrawDirective; 5] = [
        Move(Point { x: 1.0, y: CY }),
        Line(Point { x: 30.0, y: CY }),
        Move(Point {
            x: 21.0,
            y: CY - 7.0,
        }),
        Line(Point { x: 30.0, y: CY }),
        Line(Point {
            x: 21.0,
            y: CY + 7.0,
        }),
    ];
    out.render_path(&icon_arrow, &Some(GRAY_THICK_PEN), &None);
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
        IconSource {
            name: "type_rel_include",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_include,
        },
        IconSource {
            name: "type_rel_extend",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_extend,
        },
        IconSource {
            name: "type_rel_deploy",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_deploy,
        },
        IconSource {
            name: "type_rel_manifest",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_manifest,
        },
        IconSource {
            name: "type_rel_contain",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_contain,
        },
        IconSource {
            name: "type_rel_depend",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_depend,
        },
        IconSource {
            name: "type_rel_realize",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_realize,
        },
        IconSource {
            name: "type_rel_return",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_return,
        },
        IconSource {
            name: "type_rel_generalize",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_generalize,
        },
        IconSource {
            name: "type_rel_sync",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_sync,
        },
        IconSource {
            name: "type_rel_async",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_async,
        },
        IconSource {
            name: "type_rel_control_flow",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_control_flow,
        },
        IconSource {
            name: "type_rel_associate",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_associate,
        },
        IconSource {
            name: "type_rel_communication_path",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_communication_path,
        },
        IconSource {
            name: "type_rel_aggregate",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_aggregate,
        },
        IconSource {
            name: "type_rel_compose",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_compose,
        },
        IconSource {
            name: "type_rel_object_flow",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_rel_object_flow,
        },
    ]
}
