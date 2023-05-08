//! The module gets icon data from the model and passes it to render_svg.

use super::render_svg::VecRenderer;
use crate::model::geometry;
use crate::model::geometry::Color;
use crate::model::geometry::DrawDirective::Line;
use crate::model::geometry::DrawDirective::Move;
use crate::model::geometry::Point;
use crate::model::icon_data;

/// The function converts a recursive_plant to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if the the vector graphics cannot be written to a file.
///
pub fn generate_files(out_dir: &str) -> () {
    let icons: &'static [icon_data::IconData] = icon_data::get_icons();
    for icon in icons {}
}
