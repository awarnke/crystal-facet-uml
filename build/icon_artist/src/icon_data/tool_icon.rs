//! The module provides functions to render an icon to vector graphics.

use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;
use crate::stream_if::simple_font;

/// The view rectangle of each icon except navigation icons
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 12.0,
    height: 32.0,
};

/// icon center x
const BASELINE_X: f32 = 10.0;
const BASELINE_Y: f32 = 30.0;
const FONT_SIZE: f32 = 12.0;

/// gray line color
static GRAY: geometry::Color = geometry::Color {
    red: 0x7f,
    green: 0x7f,
    blue: 0x7f,
};

/// gray pen
static GRAY_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 1.0,
};

/// The function generates a separator label
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_sect(out: &mut dyn PathRenderer) -> () {
    simple_font::draw_string_upwards(
        "FILE",
        Point {
            x: BASELINE_X,
            y: BASELINE_Y,
        },
        GRAY_PEN,
        FONT_SIZE,
        out,
    );
}

/// The function generates a separator label
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_view_sect(out: &mut dyn PathRenderer) -> () {
    simple_font::draw_string_upwards(
        "VIEW",
        Point {
            x: BASELINE_X,
            y: BASELINE_Y,
        },
        GRAY_PEN,
        FONT_SIZE,
        out,
    );
}

/// The function generates a separator label
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_edit_sect(out: &mut dyn PathRenderer) -> () {
    simple_font::draw_string_upwards(
        "EDIT",
        Point {
            x: BASELINE_X,
            y: BASELINE_Y,
        },
        GRAY_PEN,
        FONT_SIZE,
        out,
    );
}

/// The function generates a separator label
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_help_sect(out: &mut dyn PathRenderer) -> () {
    simple_font::draw_string_upwards(
        "HELP",
        Point {
            x: BASELINE_X,
            y: BASELINE_Y,
        },
        GRAY_PEN,
        FONT_SIZE,
        out,
    );
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "file_sect",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_sect,
        },
        IconSource {
            name: "view_sect",
            viewport: ICON_VIEW_RECT,
            generate: generate_view_sect,
        },
        IconSource {
            name: "edit_sect",
            viewport: ICON_VIEW_RECT,
            generate: generate_edit_sect,
        },
        IconSource {
            name: "help_sect",
            viewport: ICON_VIEW_RECT,
            generate: generate_help_sect,
        },
    ]
}

/*
Copyright 2025-2026 Andreas Warnke

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
