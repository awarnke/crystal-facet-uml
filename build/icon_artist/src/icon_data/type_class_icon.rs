//! The module provides functions to render an icon to vector graphics.

use super::shape::get_circle_abs;
use crate::model::icon::IconSource;
use crate::stream_if::geometry;
use crate::stream_if::geometry::DrawDirective::Close;
use crate::stream_if::geometry::DrawDirective::CurveRel;
use crate::stream_if::geometry::DrawDirective::LineRel;
use crate::stream_if::geometry::DrawDirective::Move;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;
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

/// gray pen
static GRAY_PEN: geometry::Pen = geometry::Pen {
    color: GRAY,
    width: 1.0,
};

/// The function generates a gear wheel to vector graphics drawing directives
///
pub fn generate_type_clas_stereotype(out: &mut dyn PathRenderer) -> () {
    /* spoke of wheel */
    let r3: f32 = 11.0;
    let r2: f32 = 10.0;
    let r1: f32 = 8.5;
    let cx: f32 = 16.0;
    let cy: f32 = 12.0;
    let mut icon_segs: [geometry::DrawDirective; 61] = [Close; 61];
    icon_segs[0] = Move(Point { x: cx + r2, y: cy });
    for index in 0..10 {
        let alpha: f32 = std::f32::consts::PI / 5.0 * (index as f32);
        let a_dx = alpha.cos();
        let a_dy = alpha.sin();
        let beta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.05);
        let b_dx = beta.cos();
        let b_dy = beta.sin();
        let gamma: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.5);
        let g_dx = gamma.cos();
        let g_dy = gamma.sin();
        let delta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.55);
        let d_dx = delta.cos();
        let d_dy = delta.sin();
        let epsilon: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.65);
        let e_dx = epsilon.cos();
        let e_dy = epsilon.sin();
        let zeta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.9);
        let z_dx = zeta.cos();
        let z_dy = zeta.sin();
        let omega: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 1.0);
        let o_dx = omega.cos();
        let o_dy = omega.sin();
        icon_segs[index * 6 + 1] = LineRel(Offset {
            dx: (-r2) * a_dx + r1 * b_dx,
            dy: (-r2) * a_dy + r1 * b_dy,
        });
        icon_segs[index * 6 + 2] = LineRel(Offset {
            dx: (-r1) * b_dx + r1 * g_dx,
            dy: (-r1) * b_dy + r1 * g_dy,
        });
        icon_segs[index * 6 + 3] = LineRel(Offset {
            dx: (-r1) * g_dx + r2 * d_dx,
            dy: (-r1) * g_dy + r2 * d_dy,
        });
        icon_segs[index * 6 + 4] = LineRel(Offset {
            dx: (-r2) * d_dx + r3 * e_dx,
            dy: (-r2) * d_dy + r3 * e_dy,
        });
        icon_segs[index * 6 + 5] = LineRel(Offset {
            dx: (-r3) * e_dx + r3 * z_dx,
            dy: (-r3) * e_dy + r3 * z_dy,
        });
        icon_segs[index * 6 + 6] = LineRel(Offset {
            dx: (-r3) * z_dx + r2 * o_dx,
            dy: (-r3) * z_dy + r2 * o_dy,
        });
    }
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);

    /* rim of wheel */
    let icon_segs: [geometry::DrawDirective; 5] = get_circle_abs(Point { x: cx, y: cy }, 2.0, 2.0);
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);
}

/// The function generates a flower image to vector graphics drawing directives
///
/// # Panics
///
/// This function panics if PathRenderer cannot write to the output sink.
///
pub fn generate_type_clas_image(out: &mut dyn PathRenderer) -> () {
    /* flower leaves */
    let r3: f32 = 11.0;
    let r2: f32 = 9.0;
    let r1: f32 = 4.25;
    let cx: f32 = 16.0;
    let cy: f32 = 12.0;
    let mut icon_segs: [geometry::DrawDirective; 21] = [Close; 21];
    icon_segs[0] = Move(Point { x: cx + r1, y: cy });
    for index in 0..10 {
        let alpha: f32 = std::f32::consts::PI / 5.0 * (index as f32);
        let a_dx = alpha.cos();
        let a_dy = alpha.sin();
        let beta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.3);
        let b_dx = beta.cos();
        let b_dy = beta.sin();
        let gamma: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.5);
        let g_dx = gamma.cos();
        let g_dy = gamma.sin();
        let delta: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 0.7);
        let d_dx = delta.cos();
        let d_dy = delta.sin();
        let omega: f32 = std::f32::consts::PI / 5.0 * ((index as f32) + 1.0);
        let o_dx = omega.cos();
        let o_dy = omega.sin();
        icon_segs[index * 2 + 1] = CurveRel(
            Offset {
                dx: (-r1) * a_dx + r2 * a_dx,
                dy: (-r1) * a_dy + r2 * a_dy,
            },
            Offset {
                dx: (-r1) * a_dx + r3 * b_dx,
                dy: (-r1) * a_dy + r3 * b_dy,
            },
            Offset {
                dx: (-r1) * a_dx + r3 * g_dx,
                dy: (-r1) * a_dy + r3 * g_dy,
            },
        );
        icon_segs[index * 2 + 2] = CurveRel(
            Offset {
                dx: (-r3) * g_dx + r3 * d_dx,
                dy: (-r3) * g_dy + r3 * d_dy,
            },
            Offset {
                dx: (-r3) * g_dx + r2 * o_dx,
                dy: (-r3) * g_dy + r2 * o_dy,
            },
            Offset {
                dx: (-r3) * g_dx + r1 * o_dx,
                dy: (-r3) * g_dy + r1 * o_dy,
            },
        );
    }
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);

    /* flower center */
    let icon_segs: [geometry::DrawDirective; 5] =
        get_circle_abs(Point { x: cx, y: cy }, 3.25, 3.25);
    out.render_path(&icon_segs, &Some(GRAY_PEN), &None);
}

/// The function returns an array of IconSource
///
pub fn get_icons() -> &'static [IconSource<'static>] {
    &[
        IconSource {
            name: "type_clas_stereotype",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_stereotype,
        },
        IconSource {
            name: "type_clas_image",
            viewport: ICON_VIEW_RECT,
            generate: generate_type_clas_image,
        },
    ]
}

/*
 * Copyright 2023-2025 Andreas Warnke
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
