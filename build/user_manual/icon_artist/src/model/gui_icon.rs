//! The module provides functions to render an icon to vector graphics.

use super::geometry;
use super::geometry::get_circle_abs;
use super::geometry::DrawDirective::Close;
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

/// The function generates a steering wheel of ship navigation to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to the output sink.
///
pub fn generate_view_navigate(out: &mut VecRenderer) -> () {
    /* spoke of wheel */
    let r2: f32 = 15.0;
    let r1: f32 = 11.0;
    let cx: f32 = 16.0;
    let cy: f32 = 16.0;
    let mut icon_segs: [geometry::DrawDirective; 40] = [Close; 40];
    for index in 0..10 {
        let alpha: f32 = std::f32::consts::PI / 5.0 * (index as f32);
        let dx = alpha.cos();
        let dy = alpha.sin();
        icon_segs[index * 4 + 0] = Move(Point { x: cx, y: cy });
        icon_segs[index * 4 + 1] = LineRel(Offset {
            dx: r2 * dx,
            dy: r2 * dy,
        });
        icon_segs[index * 4 + 2] = MoveRel(Offset {
            dx: (r1 - r2) * dx,
            dy: (r1 - r2) * dy,
        });
        let beta: f32 = std::f32::consts::PI / 5.0 * ((index + 1) as f32);
        let n_dx = beta.cos();
        let n_dy = beta.sin();
        icon_segs[index * 4 + 3] = LineRel(Offset {
            dx: (-r1) * dx + r1 * n_dx,
            dy: (-r1) * dy + r1 * n_dy,
        });
    }
    out.path(&icon_segs, &None, &None);

    /* rim of wheel */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle_abs(cx, cy, 11.0, 11.0);
    out.path(&icon_segs, &None, &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[IconSource {
        name: "view_navigate",
        viewport: ICON_VIEW_RECT,
        generate: generate_view_navigate,
    }]
}
