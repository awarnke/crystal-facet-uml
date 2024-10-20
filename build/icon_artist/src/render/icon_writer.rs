//! The module creates a VecRenderer per icon
//! and passes it to icon_data for rendering.

use super::render_c::CRenderer;
use super::render_svg::VecRenderer;
use crate::model::icon::IconSource;
use std::fs;
use std::fs::File;
use std::io::Write;
use std::path::PathBuf;

/// creates a parent directory and a file in it
///
/// # Arguments
///
/// * `out_dir` - The directory where files shall be written to
/// * `file_name` - The filename of the file to be created
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

/// The function writes a starting para tag to the docbook out file.
///
/// # Panics
///
/// This function panics if the vector graphics cannot be written to a file.
///
fn write_db_header(out: &mut File) -> () {
    write!(out, "<para>\n").expect("Error at writing file");
    write!(out, "  <variablelist>\n").expect("Error at writing file");
    write!(out, "    <title>Stereotype images</title>\n").expect("Error at writing file");
}

/// The function writes a ending para tag to the docbook out file.
///
/// # Panics
///
/// This function panics if the vector graphics cannot be written to a file.
///
fn write_db_footer(out: &mut File) -> () {
    write!(out, "  </variablelist>\n").expect("Error at writing file");
    write!(out, "</para>\n").expect("Error at writing file");
}

/// The function writes a icon entry to the docbook out file.
///
/// # Panics
///
/// This function panics if the vector graphics cannot be written to a file.
///
fn write_db_icon_entry(icon: &IconSource, out: &mut File) -> () {
    write!(out, "    <varlistentry>\n").expect("Error at writing file");
    write!(out, "      <term>{}</term>\n", icon.name).expect("Error at writing file");
    write!(out, "      <listitem><para>\n        ").expect("Error at writing file");
    write!(
        out,
        "\
        <mediaobject><imageobject>\
        <imagedata fileref=\"{}.svg\" width=\"1cm\"/>\
        </imageobject></mediaobject>\
        ",
        icon.name
    )
    .expect("Error at writing file");
    write!(out, "\n      </para><para>\n").expect("Error at writing file");
    write!(out, "        <command><![CDATA[").expect("Error at writing file");
    let mut db_render = VecRenderer {
        output_file: out,
        force_colors: false,
    };
    (icon.generate)(&mut db_render);
    write!(out, "]]></command>\n").expect("Error at writing file");
    write!(out, "      </para></listitem>\n").expect("Error at writing file");
    write!(out, "    </varlistentry>\n").expect("Error at writing file");
}

/// Defines the possible filetypes to generate
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum FileType {
    /// A set of standalone svg images.
    Svg,
    /// An index file in docbook format.
    IndexOfSvg,
    /// A c struct coontainnig pixbuf data.
    PixBuf,
}

/// The function creates a file for each icon and triggers the rendering
/// of the icon as vector graphics drawing directives to the file.
///
/// # Arguments
///
/// * `icons` - The list of images to be rendered
/// * `out_type` - The output format
/// * `out_dir` - The output directory where to export the generated images to
///
/// # Panics
///
/// This function panics if the vector graphics cannot be written to a file.
///
pub fn generate_files(icons: &[IconSource], out_type: FileType, out_dir: &str) -> () {
    if out_type == FileType::IndexOfSvg {
        let mut db_file = open_file_to_write(out_dir, &"stereotype_images.xml");
        write_db_header(&mut db_file);

        for icon in icons {
            /* write an image reference to a docbook file */
            write_db_icon_entry(icon, &mut db_file);
        }

        write_db_footer(&mut db_file);
    }

    if out_type == FileType::Svg {
        for icon in icons {
            /* render an svg file */
            let file_name: String = icon.name.to_owned() + ".svg";
            let mut svg_file = open_file_to_write(out_dir, &file_name);
            let mut v_render = VecRenderer {
                output_file: &mut svg_file,
                force_colors: true,
            };
            v_render.header(&icon.viewport);
            (icon.generate)(&mut v_render);
            v_render.footer();
        }
    }

    if out_type == FileType::PixBuf {
        /* write some c files */
        for icon in icons {
            /* render a c file */
            let file_name: String = icon.name.to_owned() + ".c";
            let mut c_file = open_file_to_write(out_dir, &file_name);
            let mut c_render = CRenderer::new(&mut c_file, &icon.name, &icon.viewport);
            (icon.generate)(&mut c_render);
            c_render.write_cimpl();
        }
    }
}
