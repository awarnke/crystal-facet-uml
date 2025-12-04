//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use super::shape::get_rect_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::CloseRel;
use crate::stream_if::geometry::DrawDirective::Curve;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::DrawDirective::MoveRel;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;

/// The view rectangle of each icon except navigation icons
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 16.0,
};

/// icon center x
const CX: f32 = 16.0;
/// icon center y
const CY: f32 = 8.0;

/// The view rectangle of navigation icons
const NAVICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 12.0,
    height: 16.0,
};

/// navicon center x
const NAV_CX: f32 = 6.0;
/// navicon center y (plus 2 to move down to baseline of font)
const NAV_CY: f32 = 10.0;

/// white color
static WHITE: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0xff,
    blue: 0xff,
};

/// green color
static GREEN: geometry::Color = geometry::Color {
    red: 0xbf,
    green: 0xff,
    blue: 0xbf,
};

/// black color
static BLACK: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0x0,
    blue: 0x0,
};

/// black pen
static BLACK_PEN: geometry::Pen = geometry::Pen {
    color: BLACK,
    width: 1.0,
};

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

/// half line width
const HALFLINE: f32 = 0.5;

/// The function generates an arrow to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_relate(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = [
        Move(Point { x: 17.0, y: 16.0 }),
        Line(Point { x: 17.0, y: 10.5 }),
        Curve(
            Point { x: 17.0, y: 7.5 },
            Point { x: 19.5, y: 5.0 },
            Point { x: 22.5, y: 5.0 },
        ),
        Line(Point { x: 25.0, y: 5.0 }),
        Line(Point { x: 25.0, y: 1.0 }),
        Line(Point { x: 32.0, y: 6.5 }),
        Line(Point { x: 25.0, y: 12.0 }),
        Line(Point { x: 25.0, y: 8.0 }),
        Line(Point { x: 23.5, y: 8.0 }),
        Curve(
            Point { x: 21.5, y: 8.0 },
            Point { x: 20.0, y: 9.5 },
            Point { x: 20.0, y: 12.5 },
        ),
        Line(Point { x: 20.0, y: 16.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(GREEN));

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 12] = [
        Move(Point { x: 18.0, y: 15.0 }),
        Line(Point { x: 18.0, y: 11.0 }),
        Curve(
            Point { x: 18.0, y: 8.0 },
            Point { x: 20.0, y: 6.0 },
            Point { x: 23.0, y: 6.0 },
        ),
        Line(Point { x: 26.0, y: 6.0 }),
        Line(Point { x: 26.0, y: 3.0 }),
        Line(Point { x: 30.5, y: 6.5 }),
        Line(Point { x: 26.0, y: 10.0 }),
        Line(Point { x: 26.0, y: 7.0 }),
        Line(Point { x: 23.5, y: 7.0 }),
        Curve(
            Point { x: 21.5, y: 7.0 },
            Point { x: 19.0, y: 8.5 },
            Point { x: 19.0, y: 11.5 },
        ),
        Line(Point { x: 19.0, y: 15.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK));
}

/// The function generates a flower image to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_refine(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 0.0,
        top: 0.0,
        width: 6.0,
        height: 16.0,
    });
    out.render_path(&icon_segs, &None, &Some(GREEN));

    let icon_segs: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 6.0,
        top: 7.0,
        width: 9.0,
        height: 9.0,
    });
    out.render_path(&icon_segs, &None, &Some(GREEN));

    /* item list and plus sign */
    let icon_segs: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 1.5,
        top: 1.5,
        width: 3.0,
        height: 3.0,
    });
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);

    let icon_segs: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 1.5,
        top: 7.5,
        width: 3.0,
        height: 3.0,
    });
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);

    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 7.0, y: 11.5 }),
        LineRel(Offset { dx: 7.0, dy: 0.0 }),
        MoveRel(Offset { dx: -3.5, dy: -3.5 }),
        LineRel(Offset { dx: 0.0, dy: 7.0 }),
    ];
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);

    generate_sketch_relate(out);
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_create(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 8.0,
        top: 0.0,
        width: 15.0,
        height: 15.0,
    });
    out.render_path(&icon_segs, &None, &Some(GREEN));

    /* item list and plus sign */
    let icon_segs: [geometry::DrawDirective; 4] = [
        Move(Point { x: 9.0, y: 7.5 }),
        LineRel(Offset { dx: 13.0, dy: 0.0 }),
        MoveRel(Offset { dx: -6.5, dy: -6.5 }),
        LineRel(Offset { dx: 0.0, dy: 13.0 }),
    ];
    out.render_path(&icon_segs, &Some(BLACK_PEN), &None);
}

/// The function generates a pair of arrows pointing in horizontal direction to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_move_h(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 11] = [
        Move(Point { x: 8.0, y: 7.5 }),
        Line(Point { x: 14.0, y: 3.0 }),
        Line(Point { x: 14.0, y: 6.0 }),
        Line(Point { x: 17.0, y: 6.0 }),
        Line(Point { x: 17.0, y: 3.0 }),
        Line(Point { x: 23.0, y: 7.5 }),
        Line(Point { x: 17.0, y: 12.0 }),
        Line(Point { x: 17.0, y: 9.0 }),
        Line(Point { x: 14.0, y: 9.0 }),
        Line(Point { x: 14.0, y: 12.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 11] = [
        Move(Point { x: 9.0, y: 7.5 }),
        Line(Point { x: 13.0, y: 5.0 }),
        Line(Point { x: 13.0, y: 7.0 }),
        Line(Point { x: 18.0, y: 7.0 }),
        Line(Point { x: 18.0, y: 5.0 }),
        Line(Point { x: 22.0, y: 7.5 }),
        Line(Point { x: 18.0, y: 10.0 }),
        Line(Point { x: 18.0, y: 8.0 }),
        Line(Point { x: 13.0, y: 8.0 }),
        Line(Point { x: 13.0, y: 10.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK));
}

/// The function generates a pair of arrows pointing in vertical direction to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_move_v(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 11] = [
        Move(Point { x: 15.5, y: 0.0 }),
        Line(Point { x: 11.0, y: 6.0 }),
        Line(Point { x: 14.0, y: 6.0 }),
        Line(Point { x: 14.0, y: 9.0 }),
        Line(Point { x: 11.0, y: 9.0 }),
        Line(Point { x: 15.5, y: 15.0 }),
        Line(Point { x: 20.0, y: 9.0 }),
        Line(Point { x: 17.0, y: 9.0 }),
        Line(Point { x: 17.0, y: 6.0 }),
        Line(Point { x: 20.0, y: 6.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 11] = [
        Move(Point { x: 15.5, y: 1.0 }),
        Line(Point { x: 13.0, y: 5.0 }),
        Line(Point { x: 15.0, y: 5.0 }),
        Line(Point { x: 15.0, y: 10.0 }),
        Line(Point { x: 13.0, y: 10.0 }),
        Line(Point { x: 15.5, y: 14.0 }),
        Line(Point { x: 18.0, y: 10.0 }),
        Line(Point { x: 16.0, y: 10.0 }),
        Line(Point { x: 16.0, y: 5.0 }),
        Line(Point { x: 18.0, y: 5.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK));
}

/// The function generates two paira of arrows pointing in horizontal and vertical direction
/// to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_move_2d(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 9] = [
        Move(Point { x: 15.5, y: 0.0 }),
        Line(Point { x: 12.5, y: 4.5 }),
        Line(Point { x: 8.0, y: 7.5 }),
        Line(Point { x: 12.5, y: 10.5 }),
        Line(Point { x: 15.5, y: 15.0 }),
        Line(Point { x: 18.5, y: 10.5 }),
        Line(Point { x: 23.0, y: 7.5 }),
        Line(Point { x: 18.5, y: 4.5 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(WHITE));

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 11] = [
        Move(Point { x: 9.0, y: 7.5 }),
        Line(Point { x: 13.0, y: 5.0 }),
        Line(Point { x: 13.0, y: 7.0 }),
        Line(Point { x: 18.0, y: 7.0 }),
        Line(Point { x: 18.0, y: 5.0 }),
        Line(Point { x: 22.0, y: 7.5 }),
        Line(Point { x: 18.0, y: 10.0 }),
        Line(Point { x: 18.0, y: 8.0 }),
        Line(Point { x: 13.0, y: 8.0 }),
        Line(Point { x: 13.0, y: 10.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK));

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 11] = [
        Move(Point { x: 15.5, y: 1.0 }),
        Line(Point { x: 13.0, y: 5.0 }),
        Line(Point { x: 15.0, y: 5.0 }),
        Line(Point { x: 15.0, y: 10.0 }),
        Line(Point { x: 13.0, y: 10.0 }),
        Line(Point { x: 15.5, y: 14.0 }),
        Line(Point { x: 18.0, y: 10.0 }),
        Line(Point { x: 16.0, y: 10.0 }),
        Line(Point { x: 16.0, y: 5.0 }),
        Line(Point { x: 18.0, y: 5.0 }),
        Close,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK));
}

/// The function generates an arrow up icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_page_up_gray(out: &mut dyn PathRenderer) -> () {
    /* item list and plus sign */
    let icon_segs: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX - 15.0,
            y: 14.0,
        }),
        Line(Point { x: CX, y: 2.0 }),
        Line(Point {
            x: CX + 15.0,
            y: 14.0,
        }),
    ];
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates an arrow up icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_page_up_bold(out: &mut dyn PathRenderer) -> () {
    /* item list and plus sign */
    let icon_segs: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX - 15.0,
            y: 14.0,
        }),
        Line(Point { x: CX, y: 2.0 }),
        Line(Point {
            x: CX + 15.0,
            y: 14.0,
        }),
    ];
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &Some(GREEN));
}

/// The function generates an arrow down icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_page_down_gray(out: &mut dyn PathRenderer) -> () {
    /* item list and plus sign */
    let icon_segs: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX - 15.0,
            y: 2.0,
        }),
        Line(Point { x: CX, y: 14.0 }),
        Line(Point {
            x: CX + 15.0,
            y: 2.0,
        }),
    ];
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates an arrow down icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_page_down_bold(out: &mut dyn PathRenderer) -> () {
    /* item list and plus sign */
    let icon_segs: [geometry::DrawDirective; 3] = [
        Move(Point {
            x: CX - 15.0,
            y: 2.0,
        }),
        Line(Point { x: CX, y: 14.0 }),
        Line(Point {
            x: CX + 15.0,
            y: 2.0,
        }),
    ];
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &Some(GREEN));
}

/// The function generates a dot for a breadcrumb
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_navigate_breadcrumb_folder(out: &mut dyn PathRenderer) -> () {
    let small_circle: [geometry::DrawDirective; 5] = get_circle_abs(
        Point {
            x: NAV_CX,
            y: NAV_CY,
        },
        1.0 + HALFLINE,
        1.0 + HALFLINE,
    );
    out.render_path(&small_circle, &Some(BLACK_PEN), &Some(WHITE));
}

/// The function generates an icon for closed folders
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_navigate_closed_folder(out: &mut dyn PathRenderer) -> () {
    let triangle: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: NAV_CX - 3.0 - HALFLINE,
            y: NAV_CY - 4.0 + HALFLINE,
        }),
        Line(Point {
            x: NAV_CX + 3.0 + HALFLINE,
            y: NAV_CY + HALFLINE,
        }),
        Line(Point {
            x: NAV_CX - 3.0 - HALFLINE,
            y: NAV_CY + 4.0 + HALFLINE,
        }),
        Close,
    ];
    out.render_path(&triangle, &Some(BLACK_PEN), &Some(WHITE));
}

/// The function generates an icon for open folders
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_navigate_open_folder(out: &mut dyn PathRenderer) -> () {
    let triangle: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: NAV_CX - 4.0 - HALFLINE,
            y: NAV_CY - 2.0 - HALFLINE,
        }),
        Line(Point {
            x: NAV_CX - HALFLINE,
            y: NAV_CY + 4.0 + HALFLINE,
        }),
        Line(Point {
            x: NAV_CX + 4.0 - HALFLINE,
            y: NAV_CY - 2.0 - HALFLINE,
        }),
        Close,
    ];
    out.render_path(&triangle, &Some(BLACK_PEN), &Some(GREEN));
}

/// The function defines the draw directives for the name label
///
fn get_name_label(left: f32, top: f32) -> [geometry::DrawDirective; 6] {
    [
        MoveRel(Offset { dx: left, dy: top }),
        LineRel(Offset { dx: 10.0, dy: 0.0 }),
        LineRel(Offset { dx: 0.0, dy: 2.0 }),
        LineRel(Offset { dx: -2.0, dy: 2.0 }),
        LineRel(Offset { dx: -8.0, dy: 0.0 }),
        CloseRel,
    ]
}

/// The function generates a selected icon for creating new diagram as child
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_navigate_create_child(out: &mut dyn PathRenderer) -> () {
    /* paper */
    let paper: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 2.0 + HALFLINE,
        top: 2.0 + HALFLINE,
        width: 24.0,
        height: 11.0,
    });
    out.render_path(&paper, &Some(BLACK_PEN), &Some(WHITE));

    /* label */
    let label: [geometry::DrawDirective; 6] = get_name_label(2.0 + HALFLINE, 2.0 + HALFLINE);
    out.render_path(&label, &Some(BLACK_PEN), &Some(GREEN));

    /* plus */
    let plus: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 3.0 + HALFLINE,
            y: CY + HALFLINE,
        }),
        Line(Point {
            x: CX + 3.0 + HALFLINE,
            y: CY + HALFLINE,
        }),
        Move(Point {
            x: CX + HALFLINE,
            y: CY - 3.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + HALFLINE,
            y: CY + 3.0 + HALFLINE,
        }),
    ];
    out.render_path(&plus, &Some(BLACK_PEN), &None);
}

/// The function generates a deselected icon for creating new diagram as child
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_navigate_create_child_0(out: &mut dyn PathRenderer) -> () {
    /* paper */
    let paper: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 2.0 + HALFLINE,
        top: 2.0 + HALFLINE,
        width: 24.0,
        height: 11.0,
    });
    out.render_path(&paper, &Some(GRAY_PEN), &None);

    /* label */
    let label: [geometry::DrawDirective; 6] = get_name_label(2.0 + HALFLINE, 2.0 + HALFLINE);
    out.render_path(&label, &Some(GRAY_PEN), &None);

    /* plus */
    let plus: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 3.0 + HALFLINE,
            y: CY + HALFLINE,
        }),
        Line(Point {
            x: CX + 3.0 + HALFLINE,
            y: CY + HALFLINE,
        }),
        Move(Point {
            x: CX + HALFLINE,
            y: CY - 3.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + HALFLINE,
            y: CY + 3.0 + HALFLINE,
        }),
    ];
    out.render_path(&plus, &Some(GRAY_PEN), &None);
}

/// The function generates a deselected icon for creating new diagram as sibling
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_navigate_create_sibling_0(out: &mut dyn PathRenderer) -> () {
    /* paper */
    let paper: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 1.0 + HALFLINE,
        top: 1.0 + HALFLINE,
        width: 29.0,
        height: 13.0,
    });
    out.render_path(&paper, &Some(GRAY_PEN), &None);

    /* label */
    let label: [geometry::DrawDirective; 6] = get_name_label(1.0 + HALFLINE, 1.0 + HALFLINE);
    out.render_path(&label, &Some(GRAY_PEN), &None);

    /* plus */
    let plus: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 3.0 + HALFLINE,
            y: CY + HALFLINE,
        }),
        Line(Point {
            x: CX + 3.0 + HALFLINE,
            y: CY + HALFLINE,
        }),
        Move(Point {
            x: CX + HALFLINE,
            y: CY - 3.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + HALFLINE,
            y: CY + 3.0 + HALFLINE,
        }),
    ];
    out.render_path(&plus, &Some(GRAY_PEN), &None);
}

/// The function generates a selected icon for creating new diagram as sibling
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_navigate_create_sibling(out: &mut dyn PathRenderer) -> () {
    /* paper */
    let paper: [geometry::DrawDirective; 5] = get_rect_abs(Rect {
        left: 1.0 + HALFLINE,
        top: 1.0 + HALFLINE,
        width: 29.0,
        height: 13.0,
    });
    out.render_path(&paper, &Some(BLACK_PEN), &Some(WHITE));

    /* label */
    let label: [geometry::DrawDirective; 6] = get_name_label(1.0 + HALFLINE, 1.0 + HALFLINE);
    out.render_path(&label, &Some(BLACK_PEN), &Some(GREEN));

    /* plus */
    let plus: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: CX - 3.0 + HALFLINE,
            y: CY + HALFLINE,
        }),
        Line(Point {
            x: CX + 3.0 + HALFLINE,
            y: CY + HALFLINE,
        }),
        Move(Point {
            x: CX + HALFLINE,
            y: CY - 3.0 + HALFLINE,
        }),
        Line(Point {
            x: CX + HALFLINE,
            y: CY + 3.0 + HALFLINE,
        }),
    ];
    out.render_path(&plus, &Some(BLACK_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "sketch_relate",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_relate,
        },
        IconSource {
            name: "sketch_refine",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_refine,
        },
        IconSource {
            name: "sketch_create",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_create,
        },
        IconSource {
            name: "sketch_move_h",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_move_h,
        },
        IconSource {
            name: "sketch_move_v",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_move_v,
        },
        IconSource {
            name: "sketch_move_2d",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_move_2d,
        },
        IconSource {
            name: "sketch_page_up_gray",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_page_up_gray,
        },
        IconSource {
            name: "sketch_page_up_bold",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_page_up_bold,
        },
        IconSource {
            name: "sketch_page_down_gray",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_page_down_gray,
        },
        IconSource {
            name: "sketch_page_down_bold",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_page_down_bold,
        },
        IconSource {
            name: "navigate_breadcrumb_folder",
            viewport: NAVICON_VIEW_RECT,
            generate: generate_navigate_breadcrumb_folder,
        },
        IconSource {
            name: "navigate_closed_folder",
            viewport: NAVICON_VIEW_RECT,
            generate: generate_navigate_closed_folder,
        },
        IconSource {
            name: "navigate_open_folder",
            viewport: NAVICON_VIEW_RECT,
            generate: generate_navigate_open_folder,
        },
        IconSource {
            name: "navigate_create_child",
            viewport: ICON_VIEW_RECT,
            generate: generate_navigate_create_child,
        },
        IconSource {
            name: "navigate_create_child_0",
            viewport: ICON_VIEW_RECT,
            generate: generate_navigate_create_child_0,
        },
        IconSource {
            name: "navigate_create_sibling",
            viewport: ICON_VIEW_RECT,
            generate: generate_navigate_create_sibling,
        },
        IconSource {
            name: "navigate_create_sibling_0",
            viewport: ICON_VIEW_RECT,
            generate: generate_navigate_create_sibling_0,
        },
    ]
}

/*
Copyright 2025-2025 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
