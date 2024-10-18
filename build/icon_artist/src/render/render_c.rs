//! The module provides functions to generate gdk pixbuf c-source files.

use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;
use raqote;
use std::fs::File;
use std::io::Write;

/// Defines a pixmap renderer that produces c code
pub struct CRenderer<'my_lifespan> {
    /// The file that is open for writing
    pub output_file: &'my_lifespan mut File,
    pub icon_name: &'my_lifespan str,
    pub viewport: &'my_lifespan Rect,
    dt: raqote::DrawTarget,
}

/// The CRenderer struct provides some methods to write c header and footer
impl<'my_lifespan> CRenderer<'my_lifespan> {
    pub(super) fn new(
        output_file: &'my_lifespan mut File,
        icon_name: &'my_lifespan str,
        viewport: &'my_lifespan Rect,
    ) -> CRenderer<'my_lifespan> {
        CRenderer {
            output_file: output_file,
            icon_name: icon_name,
            viewport: viewport,
            dt: raqote::DrawTarget::new(viewport.width as i32, viewport.height as i32),
        }
    }

    /// The function write_cimpl converts the graphics drawing to c format
    ///
    /// # Arguments
    ///
    /// * `view` - The bounding box of the visible area
    ///
    /// # Panics
    ///
    /// This function panics if the vector graphics cannot be written to a file.
    ///
    pub(super) fn write_cimpl(self: &mut Self) -> () {
        write!(
            self.output_file,
            "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<svg
    width=\"100%\"
    height=\"100%\"
    viewBox=\"\"
    preserveAspectRatio=\"xMidYMid meet\"
    xmlns=\"http://www.w3.org/2000/svg\"
    version=\"1.1\"
    >
    \
            ",
        )
        .expect("Error at writing file");
    }
}

/// The CRenderer struct provides some methods that implement a PathRenderer
impl<'my_lifespan> PathRenderer for CRenderer<'my_lifespan> {
    /// The function render_path converts the vector graphics drawing directive path
    /// to a pixmap in c format
    /// # Arguments
    ///
    /// * `segs` - The segments of the path
    /// * `fg_col` - The foreground color by which the path is stroked
    /// * `bg_col` - The background color by which the path is filled
    ///
    /// # Panics
    ///
    /// This function panics if the vector graphics cannot be written to a file.
    ///
    fn render_path(
        self: &mut Self,
        segs: &[DrawDirective],
        fg_col: &Option<geometry::Color>,
        bg_col: &Option<geometry::Color>,
    ) -> () {
    }
}
