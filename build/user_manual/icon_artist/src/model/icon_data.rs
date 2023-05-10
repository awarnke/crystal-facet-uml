//! The module provides functions to render a model to vector graphics.

use super::geometry;
use super::geometry::Color;
use super::geometry::DrawDirective::Close;
use super::geometry::DrawDirective::Line;
use super::geometry::DrawDirective::Move;
use super::geometry::Point;
use crate::render::render_svg::VecRenderer;

/// Defines the data needed to render an icon
pub struct IconSource<'my_lifespan> {
    /// The name
    pub name: &'my_lifespan str,
    /// The generator function
    pub generate: fn(out: &mut VecRenderer) -> (),
}

/// The function converts a recursive_plant to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if VecRenderer cannot write to sink.
///
pub fn generate_gsn_strategy(out: &mut VecRenderer) -> () {
    let icon_segs: [geometry::DrawDirective; 5] = [
        Move(Point { x: 7.0, y: 7.0 }),
        Line(Point { x: 31.0, y: 7.0 }),
        Line(Point { x: 25.0, y: 25.0 }),
        Line(Point { x: 1.0, y: 25.0 }),
        Close,
    ];
    let icon_color: Color = Color {
        red: 0,
        green: 0,
        blue: 0,
    };
    out.path(&icon_segs, &icon_color)
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[IconSource {
        name: "gsn_strategy",
        generate: generate_gsn_strategy,
    }]
}
