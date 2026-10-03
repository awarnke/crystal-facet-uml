//! The module provides functions to render an icon to vector graphics.

use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::CurveRel;
use crate::stream_if::geometry::DrawDirective::Line;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::DrawDirective::MoveRel;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;
use crate::stream_if::simple_font;

/// The view rectangle of each icon
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 32.0,
};

/// The view rectangle of section icons
const SECT_ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 12.0,
    height: 32.0,
};

/// section icon font baseline and size
const SECT_BASELINE_X: f32 = 10.0;
const SECT_BASELINE_Y: f32 = 30.0;
const SECT_FONT_SIZE: f32 = 12.0;

/// gray color
static GRAY: geometry::Color = geometry::Color {
    red: 0x7f,
    green: 0x7f,
    blue: 0x7f,
};

/// bright yellow color
static BRIGHT_YELLOW: geometry::Color = geometry::Color {
    red: 0xff,
    green: 0xff,
    blue: 0x44,
};

/// gray pen
static GRAY_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 1.0,
};

/// gray thick pen
static GRAY_THICK_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 2.0,
};

const BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE: f32 = 0.552284749831;

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
            x: SECT_BASELINE_X,
            y: SECT_BASELINE_Y,
        },
        GRAY_PEN,
        SECT_FONT_SIZE,
        out,
    );
}

/// The function defines the draw directives for the file symbols contour
///
/// The last two draw directives are the inner arc of the top ellipsis,
/// omitting these gives the outer bounds of the file symbol.
///
/// # Arguments
///
/// * `closed_shape` - True if the countour shall leave the right side open
///
fn get_db_storage_contour(closed_shape: bool) -> [geometry::DrawDirective; 9] {
    let x_rad: f32 = 10.0;
    let y_rad: f32 = 4.0;
    let height: f32 = 22.0;
    let center_x: f32 = 16.0;
    let center_y: f32 = 16.0;
    [
        MoveRel(Offset {
            dx: center_x - x_rad,
            dy: center_y - 0.5 * height,
        }),
        CurveRel(
            Offset {
                dx: 0.0,
                dy: y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: x_rad - x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: y_rad,
            },
            Offset {
                dx: x_rad,
                dy: y_rad,
            },
        ),
        CurveRel(
            Offset {
                dx: x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: 0.0,
            },
            Offset {
                dx: x_rad,
                dy: -y_rad + y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: x_rad,
                dy: -y_rad,
            },
        ),
        CurveRel(
            Offset {
                dx: 0.0,
                dy: -y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: -x_rad + x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: -y_rad,
            },
            Offset {
                dx: -x_rad,
                dy: -y_rad,
            },
        ),
        CurveRel(
            Offset {
                dx: -x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: 0.0,
            },
            Offset {
                dx: -x_rad,
                dy: y_rad - y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: -x_rad,
                dy: y_rad,
            },
        ),
        LineRel(Offset {
            dx: 0.0,
            dy: height,
        }),
        CurveRel(
            Offset {
                dx: 0.0,
                dy: y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: x_rad - x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: y_rad,
            },
            Offset {
                dx: x_rad,
                dy: y_rad,
            },
        ),
        CurveRel(
            Offset {
                dx: x_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
                dy: 0.0,
            },
            Offset {
                dx: x_rad,
                dy: -y_rad + y_rad * BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE,
            },
            Offset {
                dx: x_rad,
                dy: -y_rad,
            },
        ),
        LineRel(Offset {
            dx: 0.0,
            dy: if closed_shape { -height } else { -y_rad },
        }),
    ]
}

/// The function defines the draw directives for the asterisk symbol
///
fn get_asterisk() -> [geometry::DrawDirective; 8] {
    [
        MoveRel(Offset { dx: 18.0, dy: 11.0 }),
        LineRel(Offset { dx: 0.0, dy: 7.0 }),
        LineRel(Offset { dx: 8.0, dy: -2.0 }),
        MoveRel(Offset { dx: -16.0, dy: 0.0 }),
        LineRel(Offset { dx: 8.0, dy: 2.0 }),
        LineRel(Offset { dx: -4.0, dy: 7.0 }),
        MoveRel(Offset { dx: 4.0, dy: -7.0 }),
        LineRel(Offset { dx: 4.9, dy: 7.0 }),
    ]
}

/// The function generates a new-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_new(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour(true);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);

    /* plus symbol */
    let center_x: f32 = 16.0;
    let center_y: f32 = 19.0;
    let plus_sym: [geometry::DrawDirective; 4] = [
        Move(Point {
            x: center_x - 4.0,
            y: center_y,
        }),
        Line(Point {
            x: center_x + 4.0,
            y: center_y,
        }),
        Move(Point {
            x: center_x,
            y: center_y - 4.0,
        }),
        Line(Point {
            x: center_x,
            y: center_y + 4.0,
        }),
    ];
    out.render_path(&plus_sym, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates an open-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_open(out: &mut dyn PathRenderer) -> () {
    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour(true);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a save-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_save(out: &mut dyn PathRenderer) -> () {
    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour(false);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);

    /* asterisk symbol */
    let unsaved_sym: [geometry::DrawDirective; 8] = get_asterisk();
    out.render_path(&unsaved_sym, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates a saved-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_saved(out: &mut dyn PathRenderer) -> () {
    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour(false);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);

    /* ok symbol */
    let ok_sym: [geometry::DrawDirective; 3] = [
        Move(Point { x: 13.0, y: 20.0 }),
        Line(Point { x: 18.5, y: 23.5 }),
        Line(Point { x: 24.0, y: 12.0 }),
    ];
    out.render_path(&ok_sym, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates an unsaved-file icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_unsaved(out: &mut dyn PathRenderer) -> () {
    /* yellow background */
    let ground_segs: [geometry::DrawDirective; 9] = get_db_storage_contour(true);
    out.render_path(&ground_segs, &None, &Some(BRIGHT_YELLOW));

    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour(false);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);

    /* asterisk symbol */
    let unsaved_sym: [geometry::DrawDirective; 8] = get_asterisk();
    out.render_path(&unsaved_sym, &Some(GRAY_THICK_PEN), &None);
}

/// The function generates an export-files icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_file_export(out: &mut dyn PathRenderer) -> () {
    /* contour */
    let icon_segs: [geometry::DrawDirective; 9] = get_db_storage_contour(false);
    out.render_path(&icon_segs, &Some(GRAY_THICK_PEN), &None);

    /* out symbol */
    let out_sym: [geometry::DrawDirective; 7] = [
        MoveRel(Offset { dx: 13.0, dy: 18.0 }),
        LineRel(Offset { dx: 3.0, dy: -2.0 }),
        CurveRel(
            Offset { dx: 3.0, dy: -2.0 },
            Offset { dx: 6.0, dy: -2.0 },
            Offset { dx: 9.0, dy: 0.0 },
        ),
        LineRel(Offset { dx: 3.0, dy: 2.0 }),
        MoveRel(Offset { dx: -1.0, dy: -4.0 }),
        LineRel(Offset { dx: 2.0, dy: 5.0 }),
        LineRel(Offset { dx: -5.5, dy: 0.0 }),
    ];
    out.render_path(&out_sym, &Some(GRAY_THICK_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "file_sect",
            viewport: SECT_ICON_VIEW_RECT,
            generate: generate_file_sect,
        },
        IconSource {
            name: "file_new",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_new,
        },
        IconSource {
            name: "file_open",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_open,
        },
        IconSource {
            name: "file_save",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_save,
        },
        IconSource {
            name: "file_saved",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_saved,
        },
        IconSource {
            name: "file_unsaved",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_unsaved,
        },
        IconSource {
            name: "file_export",
            viewport: ICON_VIEW_RECT,
            generate: generate_file_export,
        },
    ]
}

/*
 * Copyright 2023-2026 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
