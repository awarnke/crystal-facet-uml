#![warn(missing_docs)]

//! This crate renders svg files for the icons in icon_data.

pub mod icon_data;
pub mod model;
pub mod render;
pub mod stream_if;
use icon_data::gui_file_icon;
use icon_data::gui_sketch_icon;
use icon_data::gui_view_icon;
use icon_data::stereotype_icon;
use icon_data::type_class_icon;
use icon_data::type_diag_icon;
use icon_data::type_rel_icon;
use model::icon::IconSource;
use render::icon_writer;
use render::icon_writer::FileType;
use std::env;

static OUT_DIR_STEREO: &'static str = "../../documentation/user_manual/4_stereotypes";
static OUT_DIR_GUI: &'static str = "../../gui/source/resources";

/// The main function defines parameters and starts the icon_writer.
fn main() {
    let args: Vec<String> = env::args().collect();

    for argument in args {
        let arg = argument.as_str();
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
        if arg == "-r" {
            let type_rel_icons: &'static [IconSource<'static>] = type_rel_icon::get_icons();
            icon_writer::generate_files(type_rel_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(type_rel_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-s" {
            let stereo_icons: &'static [IconSource<'static>] = stereotype_icon::get_icons();
            icon_writer::generate_files(stereo_icons, FileType::Svg, OUT_DIR_STEREO);
            icon_writer::generate_files(stereo_icons, FileType::IndexOfSvg, OUT_DIR_STEREO);
            println!("Generated files have been written to '{}'.", OUT_DIR_STEREO);
        }
        if arg == "-g" {
            let stereo_icons: &'static [IconSource<'static>] = gui_sketch_icon::get_icons();
            icon_writer::generate_files(stereo_icons, FileType::Svg, OUT_DIR_GUI);
            icon_writer::generate_files(stereo_icons, FileType::PixBuf, OUT_DIR_GUI);
            println!("Generated files have been written to '{}'.", OUT_DIR_GUI);
        }
        if arg == "-h" {
            println!("options are");
            println!("-f file icons");
            println!("-e edit icons");
            println!("-v view icons");
            println!("-g gui sketch icons");
            println!("-d diagram icons");
            println!("-c classifier icons");
            println!("-s stereotype icons");
            println!("-x feature icons");
            println!("-r relationship icons");
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
