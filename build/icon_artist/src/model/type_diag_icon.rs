//! The module provides functions to render an icon to vector graphics.

use super::icon::IconSource;
use super::shape::get_rect_abs;
use super::shape::get_rounded_rect_abs;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::CloseRel;
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

    let icon_segs_link: [geometry::DrawDirective; 2] = [
        Move(Point {
            x: 14.0,
            y: CY + HALFLINE,
        }),
        Line(Point {
            x: 18.0,
            y: CY + HALFLINE,
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

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    /*
    ../../gui/source/resources/type_diag_communication.c
    ../../gui/source/resources/type_diag_component.c
    ../../gui/source/resources/type_diag_composite.c
    ../../gui/source/resources/type_diag_deployment.c
    ../../gui/source/resources/type_diag_internal.c
    ../../gui/source/resources/type_diag_list.c
    ../../gui/source/resources/type_diag_overview.c
    ../../gui/source/resources/type_diag_package.c
    ../../gui/source/resources/type_diag_parametric.c
    ../../gui/source/resources/type_diag_profile.c
    ../../gui/source/resources/type_diag_requirement.c
    ../../gui/source/resources/type_diag_sequence.c
    ../../gui/source/resources/type_diag_state.c
    ../../gui/source/resources/type_diag_timing.c
    ../../gui/source/resources/type_diag_usecase.c
    */

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
    ]
}
