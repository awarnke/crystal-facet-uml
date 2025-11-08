#![warn(missing_docs)]

//! This crate renders svg files for the icons in icon_data.

pub mod icon_data;
pub mod model;
pub mod render;
pub mod stream_if;
use icon_data::gui_edit_icon;
use icon_data::gui_file_icon;
use icon_data::gui_sketch_icon;
use icon_data::gui_view_icon;
use icon_data::logo_icon;
use icon_data::message_icon;
use icon_data::stereotype_icon;
use icon_data::type_class_icon;
use icon_data::type_diag_icon;
use icon_data::type_feat_icon;
use icon_data::type_rel_icon;
use icon_data::user_manual_icon;
use model::icon::IconSource;
use render::icon_writer;
use render::icon_writer::FileType;
use std::env;

static OUT_DIR_USER_MANUAL: &'static str = "../../documentation/user_manual";
static OUT_DIR_STEREOTYPES: &'static str = "../../documentation/user_manual/4_stereotypes";
static OUT_DIR_GUI: &'static str = "../../gui/source/resources";

/// The main function defines parameters and starts the icon_writer.
fn main() {
    let args: Vec<String> = env::args().collect();

    for argument in args {
        let arg = argument.as_str();
        if arg == "-e" {
            let gui_edit_icons: &'static [IconSource<'static>] = gui_edit_icon::get_icons();
            icon_writer::generate_files(gui_edit_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(gui_edit_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-f" {
            let gui_file_icons: &'static [IconSource<'static>] = gui_file_icon::get_icons();
            icon_writer::generate_files(gui_file_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(gui_file_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-v" {
            let gui_view_icons: &'static [IconSource<'static>] = gui_view_icon::get_icons();
            icon_writer::generate_files(gui_view_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(gui_view_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-d" {
            let type_diag_icons: &'static [IconSource<'static>] = type_diag_icon::get_icons();
            icon_writer::generate_files(type_diag_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(type_diag_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-c" {
            let type_class_icons: &'static [IconSource<'static>] = type_class_icon::get_icons();
            icon_writer::generate_files(type_class_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(type_class_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-x" {
            let type_feat_icons: &'static [IconSource<'static>] = type_feat_icon::get_icons();
            icon_writer::generate_files(type_feat_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(type_feat_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-r" {
            let type_rel_icons: &'static [IconSource<'static>] = type_rel_icon::get_icons();
            icon_writer::generate_files(type_rel_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(type_rel_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-s" {
            let stereo_icons: &'static [IconSource<'static>] = stereotype_icon::get_icons();
            icon_writer::generate_files(stereo_icons, FileType::Svg, OUT_DIR_STEREOTYPES);
            icon_writer::generate_files(stereo_icons, FileType::IndexOfSvg, OUT_DIR_STEREOTYPES);
            println!(
                "Generated files have been written to '{}'.",
                OUT_DIR_STEREOTYPES
            );
        }
        if arg == "-g" {
            let sketch_icons: &'static [IconSource<'static>] = gui_sketch_icon::get_icons();
            icon_writer::generate_files(sketch_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(sketch_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-m" {
            let message_icons: &'static [IconSource<'static>] = message_icon::get_icons();
            icon_writer::generate_files(message_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(message_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-u" {
            let manual_icons: &'static [IconSource<'static>] = user_manual_icon::get_icons();
            icon_writer::generate_files(manual_icons, FileType::Svg, OUT_DIR_USER_MANUAL);
            println!(
                "Generated files have been written to '{}'.",
                OUT_DIR_USER_MANUAL
            );
        }
        if arg == "-l" {
            let logo_icons: &'static [IconSource<'static>] = logo_icon::get_icons();
            icon_writer::generate_files(logo_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(logo_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!(
                "Generated files have been written to '{}'.",
                OUT_DIR_USER_MANUAL
            );
        }

        if arg == "-h" {
            println!("options are");
            println!("-l logo icon variants");
            println!("   ");
            println!("-f file icons");
            println!("-v view icons");
            println!("-e edit icons");
            println!("   ");
            println!("-g gui sketch icons");
            println!("   ");
            println!("-d diagram icons");
            println!("-c classifier icons");
            println!("-x feature icons");
            println!("-r relationship icons");
            println!("-s stereotype icons");
            println!("   ");
            println!("-m message icons");
            println!("   ");
            println!("-u user_manual icons");
            println!("   ");
            println!("note: to convert svg to pdf, run 'rsvg-convert -f pdf -o some_icon.pdf some_icon.svg'");
        }
    }
}

/*
Copyright 2023-2025 Andreas Warnke

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
