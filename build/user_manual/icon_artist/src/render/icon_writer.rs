//! The module creates two VecRenderers per icon
//! and passes it to icon_data for rendering.

use super::render_svg::VecRenderer;
use crate::model::geometry::Rect;
use crate::model::icon_data;
use std::fs;
use std::fs::File;
use std::path::PathBuf;

/// creates a parent directory and a file in it
///
/// # Panics
///
/// This function panics if the the file cannot be created.
///
fn open_file_to_write(out_dir: &str, file_name: &str) -> File {
    /* create the output directory, panic in case an error occured */
    fs::create_dir_all(out_dir).unwrap();

    /* create the file, panic in case an error occured */
    let mut file_to_use = PathBuf::from(out_dir);
    file_to_use.push(file_name);
    File::create(&file_to_use).unwrap()
}

/// The function creates a file for each icon and triggers the rendering
/// of the icon as vector graphics drawing directives to the file.
///
/// # Panics
///
/// This function panics if the vector graphics cannot be written to a file.
///
pub fn generate_files(out_dir: &str) -> () {
    let icons: &'static [icon_data::IconSource<'static>] = icon_data::get_icons();
    for icon in icons {
        let file_name: String = icon.name.to_owned() + ".svg";
        let mut svg_file = open_file_to_write(out_dir, &file_name);
        let mut v_render = VecRenderer {
            output_file: &mut svg_file,
            font_size: 8.0,
        };
        let view = Rect {
            left: 0.0,
            top: 0.0,
            width: 32.0,
            height: 32.0,
        };
        v_render.header(&view);
        (icon.generate)(&mut v_render);
        v_render.footer();
    }
}
