#![warn(missing_docs)]

//! This crate renders two svg files for each icon in icon_data.

pub mod model;
pub mod render;
use model::icon::IconSource;
use model::stereotype_icon;
use render::icon_writer;

static OUT_DIR: &'static str = "../../../documentation/user_manual/4_stereotypes";

/// The main function defines parameters and starts the icon_writer.
fn main() {
    let icons: &'static [IconSource<'static>] = stereotype_icon::get_icons();
    icon_writer::generate_files(icons, OUT_DIR);
    println!("Generated files have been written to '{}'.", OUT_DIR);
}

/*
Copyright 2023-2023 Andreas Warnke

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
