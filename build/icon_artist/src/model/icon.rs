//! The module provides functions to render an icon to vector graphics.

use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;

/// Defines the data needed to render an icon
pub struct IconSource<'my_lifespan> {
    /// The name
    pub name: &'my_lifespan str,
    /// The rectangle that defines the visible region
    pub viewport: Rect,
    /// The generator function
    pub generate: fn(out: &mut dyn PathRenderer) -> (),
}
