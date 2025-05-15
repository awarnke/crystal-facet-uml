//! The module provides functions to render an icon to vector graphics.

use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::CloseRel;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::MoveRel;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;

/// The view rectangle of each icon
const ICON_VIEW_RECT: Rect = Rect {
    left: 0.0,
    top: 0.0,
    width: 32.0,
    height: 24.0,
};

/// gray line color
static GRAY: geometry::Color = geometry::Color {
    red: 0x7f,
    green: 0x7f,
    blue: 0x7f,
};

/// black color
static BLACK: geometry::Color = geometry::Color {
    red: 0x0,
    green: 0x0,
    blue: 0x0,
};

/// The function generates a gear wheel to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_relate(out: &mut dyn PathRenderer) -> () {

    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = [
        MoveRel(Offset { dx: 1.0, dy: 19.0 }),
        LineRel(Offset { dx: 3.0, dy: 3.0 }),
        LineRel(Offset { dx: 6.0, dy: -6.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 8.0, dy: -8.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 2.0, dy: -9.0 }),
        LineRel(Offset { dx: -9.0, dy: 2.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: -5.0, dy: 5.0 }),
        LineRel(Offset { dx: -2.0, dy: -2.0 }),
        CloseRel,
    ];
    out.render_path(&icon_segs, &None, &Some(GRAY) );

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 12] = [
        MoveRel(Offset { dx: 3.0, dy: 19.0 }),
        LineRel(Offset { dx: 1.0, dy: 1.0 }),
        LineRel(Offset { dx: 6.0, dy: -6.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 8.0, dy: -8.0 }),
        LineRel(Offset { dx: 1.5, dy: 1.5 }),
        LineRel(Offset { dx: 1.0, dy: -5.0 }),
        LineRel(Offset { dx: -5.0, dy: 1.0 }),
        LineRel(Offset { dx: 1.5, dy: 1.5 }),
        LineRel(Offset { dx: -7.0, dy: 7.0 }),
        LineRel(Offset { dx: -2.0, dy: -2.0 }),
        CloseRel,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK) );
}

/// The function generates a flower image to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_type_clas_image(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = [
        MoveRel(Offset { dx: 1.0, dy: 19.0 }),
        LineRel(Offset { dx: 3.0, dy: 3.0 }),
        LineRel(Offset { dx: 6.0, dy: -6.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 8.0, dy: -8.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 2.0, dy: -9.0 }),
        LineRel(Offset { dx: -9.0, dy: 2.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: -5.0, dy: 5.0 }),
        LineRel(Offset { dx: -2.0, dy: -2.0 }),
        CloseRel,
    ];
    out.render_path(&icon_segs, &None, &Some(GRAY) );

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 12] = [
        MoveRel(Offset { dx: 3.0, dy: 19.0 }),
        LineRel(Offset { dx: 1.0, dy: 1.0 }),
        LineRel(Offset { dx: 6.0, dy: -6.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 8.0, dy: -8.0 }),
        LineRel(Offset { dx: 1.5, dy: 1.5 }),
        LineRel(Offset { dx: 1.0, dy: -5.0 }),
        LineRel(Offset { dx: -5.0, dy: 1.0 }),
        LineRel(Offset { dx: 1.5, dy: 1.5 }),
        LineRel(Offset { dx: -7.0, dy: 7.0 }),
        LineRel(Offset { dx: -2.0, dy: -2.0 }),
        CloseRel,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK) );
}

/// The function generates a magnifying glass icon to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_sketch_create(out: &mut dyn PathRenderer) -> () {
    /* background */
    let icon_segs: [geometry::DrawDirective; 12] = [
        MoveRel(Offset { dx: 1.0, dy: 19.0 }),
        LineRel(Offset { dx: 3.0, dy: 3.0 }),
        LineRel(Offset { dx: 6.0, dy: -6.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 8.0, dy: -8.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 2.0, dy: -9.0 }),
        LineRel(Offset { dx: -9.0, dy: 2.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: -5.0, dy: 5.0 }),
        LineRel(Offset { dx: -2.0, dy: -2.0 }),
        CloseRel,
    ];
    out.render_path(&icon_segs, &None, &Some(GRAY) );

    /* arrow */
    let icon_segs: [geometry::DrawDirective; 12] = [
        MoveRel(Offset { dx: 3.0, dy: 19.0 }),
        LineRel(Offset { dx: 1.0, dy: 1.0 }),
        LineRel(Offset { dx: 6.0, dy: -6.0 }),
        LineRel(Offset { dx: 2.0, dy: 2.0 }),
        LineRel(Offset { dx: 8.0, dy: -8.0 }),
        LineRel(Offset { dx: 1.5, dy: 1.5 }),
        LineRel(Offset { dx: 1.0, dy: -5.0 }),
        LineRel(Offset { dx: -5.0, dy: 1.0 }),
        LineRel(Offset { dx: 1.5, dy: 1.5 }),
        LineRel(Offset { dx: -7.0, dy: 7.0 }),
        LineRel(Offset { dx: -2.0, dy: -2.0 }),
        CloseRel,
    ];
    out.render_path(&icon_segs, &None, &Some(BLACK) );
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "sketch_relate",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_relate,
        },
        IconSource {
            name: "sketch_refine",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_image,
        },
        IconSource {
            name: "sketch_create",
            viewport: ICON_VIEW_RECT,
            generate: generate_sketch_create,
        },
    ]
}

/*
Copyright 2025-2025 Andreas Warnke

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
