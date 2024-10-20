//! The module geometry provides a couple of geometric data structures.

use crate::stream_if::geometry::DrawDirective;
use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;
use crate::stream_if::geometry::Rect;

/// control point distance to approximate an arc by cubic bezier curves
const ARC_APPROX: f32 = 0.552284749831;

/// The function defines the control points for a circle in absolute coordinates
///
/// # Arguments
///
/// * `center` - The absolute coordinate of the center
/// * `rx` - The radius in x-direction
/// * `ry` - The radius in y-direction
///
pub fn get_circle_abs(center: Point, rx: f32, ry: f32) -> [DrawDirective; 5] {
    let ctrlpnt_dx: f32 = rx * ARC_APPROX; /* control point distance x */
    let ctrlpnt_dy: f32 = ry * ARC_APPROX; /* control point distance y */
    let cx = center.x;
    let cy = center.y;
    [
        DrawDirective::Move(Point { x: cx - rx, y: cy }),
        DrawDirective::Curve(
            Point {
                x: cx - rx,
                y: cy - ctrlpnt_dy,
            },
            Point {
                x: cx - ctrlpnt_dx,
                y: cy - ry,
            },
            Point { x: cx, y: cy - ry },
        ),
        DrawDirective::Symmetric(
            Point {
                x: cx + rx,
                y: cy - ctrlpnt_dy,
            },
            Point { x: cx + rx, y: cy },
        ),
        DrawDirective::Symmetric(
            Point {
                x: cx + ctrlpnt_dx,
                y: cy + ry,
            },
            Point { x: cx, y: cy + ry },
        ),
        DrawDirective::Symmetric(
            Point {
                x: cx - rx,
                y: cy + ctrlpnt_dy,
            },
            Point { x: cx - rx, y: cy },
        ),
    ]
}

/// The function defines the control points for a circle in relative offsets
///
/// # Arguments
///
/// * `center` - The relative offset of the center
/// * `rx` - The radius in x-direction
/// * `ry` - The radius in y-direction
///
pub fn get_circle_rel(center: Offset, rx: f32, ry: f32) -> [DrawDirective; 5] {
    let ctrlpnt_dx: f32 = rx * ARC_APPROX; /* control point distance x */
    let ctrlpnt_dy: f32 = ry * ARC_APPROX; /* control point distance y */
    let c_dx = center.dx;
    let c_dy = center.dy;
    [
        DrawDirective::MoveRel(Offset {
            dx: c_dx - rx,
            dy: c_dy,
        }),
        DrawDirective::CurveRel(
            Offset {
                dx: 0.0,
                dy: (-ctrlpnt_dy),
            },
            Offset {
                dx: rx - ctrlpnt_dx,
                dy: (-ry),
            },
            Offset { dx: rx, dy: (-ry) },
        ),
        DrawDirective::SymmetricRel(
            Offset {
                dx: rx,
                dy: ry - ctrlpnt_dy,
            },
            Offset { dx: rx, dy: ry },
        ),
        DrawDirective::SymmetricRel(
            Offset {
                dx: (-rx) + ctrlpnt_dx,
                dy: ry,
            },
            Offset { dx: (-rx), dy: ry },
        ),
        DrawDirective::SymmetricRel(
            Offset {
                dx: (-rx),
                dy: (-ry) + ctrlpnt_dy,
            },
            Offset {
                dx: (-rx),
                dy: (-ry),
            },
        ),
    ]
}

/// The function defines the control points for a rounded rectangle in relative offsets
///
/// # Arguments
///
/// * `top_left` - The relative offset of the top left corner
/// * `width` - The width of the rectangle
/// * `height` - The height of the rectangle
/// * `r` - The radius of the corners in x- and y-directions
///
pub fn get_rounded_rect_rel(
    top_left: Offset,
    width: f32,
    height: f32,
    r: f32,
) -> [DrawDirective; 9] {
    let ctrlpnt_dx: f32 = r * ARC_APPROX; /* control point distance x */
    let ctrlpnt_dy: f32 = r * ARC_APPROX; /* control point distance y */
    [
        DrawDirective::MoveRel(Offset {
            dx: top_left.dx,
            dy: top_left.dy + r,
        }),
        DrawDirective::CurveRel(
            Offset {
                dx: 0.0,
                dy: -ctrlpnt_dy,
            },
            Offset {
                dx: r - ctrlpnt_dx,
                dy: -r,
            },
            Offset { dx: r, dy: -r },
        ),
        DrawDirective::LineRel(Offset {
            dx: width - 2.0 * r,
            dy: 0.0,
        }),
        DrawDirective::CurveRel(
            Offset {
                dx: ctrlpnt_dx,
                dy: 0.0,
            },
            Offset {
                dx: r,
                dy: r - ctrlpnt_dy,
            },
            Offset { dx: r, dy: r },
        ),
        DrawDirective::LineRel(Offset {
            dx: 0.0,
            dy: height - 2.0 * r,
        }),
        DrawDirective::CurveRel(
            Offset {
                dx: 0.0,
                dy: ctrlpnt_dy,
            },
            Offset {
                dx: ctrlpnt_dx - r,
                dy: r,
            },
            Offset { dx: -r, dy: r },
        ),
        DrawDirective::LineRel(Offset {
            dx: 2.0 * r - width,
            dy: 0.0,
        }),
        DrawDirective::CurveRel(
            Offset {
                dx: -ctrlpnt_dx,
                dy: 0.0,
            },
            Offset {
                dx: -r,
                dy: ctrlpnt_dy - r,
            },
            Offset { dx: -r, dy: -r },
        ),
        DrawDirective::CloseRel,
    ]
}

/// The function defines the control points for a rounded rectangle in absolute offsets
///
/// # Arguments
///
/// * `bounds` - The absolute coordinates of the rectangle
/// * `r` - The radius of the corners in x- and y-directions
///
pub fn get_rounded_rect_abs(bounds: Rect, r: f32) -> [DrawDirective; 9] {
    let ctrlpnt_dx: f32 = r * ARC_APPROX; /* control point distance x */
    let ctrlpnt_dy: f32 = r * ARC_APPROX; /* control point distance y */
    let left = bounds.left;
    let top = bounds.top;
    let right = bounds.right();
    let bottom = bounds.bottom();
    [
        DrawDirective::Move(Point {
            x: left,
            y: top + r,
        }),
        DrawDirective::Curve(
            Point {
                x: left,
                y: top + r - ctrlpnt_dy,
            },
            Point {
                x: left + r - ctrlpnt_dx,
                y: top,
            },
            Point {
                x: left + r,
                y: top,
            },
        ),
        DrawDirective::Line(Point {
            x: right - r,
            y: top,
        }),
        DrawDirective::Curve(
            Point {
                x: right - r + ctrlpnt_dx,
                y: top,
            },
            Point {
                x: right,
                y: top + r - ctrlpnt_dy,
            },
            Point {
                x: right,
                y: top + r,
            },
        ),
        DrawDirective::Line(Point {
            x: right,
            y: bottom - r,
        }),
        DrawDirective::Curve(
            Point {
                x: right,
                y: bottom - r + ctrlpnt_dy,
            },
            Point {
                x: right - r + ctrlpnt_dx,
                y: bottom,
            },
            Point {
                x: right - r,
                y: bottom,
            },
        ),
        DrawDirective::Line(Point {
            x: left + r,
            y: bottom,
        }),
        DrawDirective::Curve(
            Point {
                x: left + r - ctrlpnt_dx,
                y: bottom,
            },
            Point {
                x: left,
                y: bottom - r + ctrlpnt_dy,
            },
            Point {
                x: left,
                y: bottom - r,
            },
        ),
        DrawDirective::Close,
    ]
}

/// The function defines the path  points for a rectangle in relative offsets
///
/// # Arguments
///
/// * `top_left` - The relative offset of the top left corner
/// * `width` - The width of the rectangle
/// * `height` - The height of the rectangle
///
pub fn get_rect_rel(top_left: Offset, width: f32, height: f32) -> [DrawDirective; 5] {
    [
        DrawDirective::MoveRel(Offset {
            dx: top_left.dx,
            dy: top_left.dy,
        }),
        DrawDirective::LineRel(Offset { dx: width, dy: 0.0 }),
        DrawDirective::LineRel(Offset {
            dx: 0.0,
            dy: height,
        }),
        DrawDirective::LineRel(Offset {
            dx: -width,
            dy: 0.0,
        }),
        DrawDirective::CloseRel,
    ]
}

/// The function defines the path points for a rectangle in absolute offsets
///
/// # Arguments
///
/// * `bounds` - The absolute coordinates of the rectangle
///
pub fn get_rect_abs(bounds: Rect) -> [DrawDirective; 5] {
    let left = bounds.left;
    let top = bounds.top;
    let right = bounds.right();
    let bottom = bounds.bottom();
    [
        DrawDirective::Move(Point { x: left, y: top }),
        DrawDirective::Line(Point { x: right, y: top }),
        DrawDirective::Line(Point {
            x: right,
            y: bottom,
        }),
        DrawDirective::Line(Point { x: left, y: bottom }),
        DrawDirective::Close,
    ]
}

/*
Copyright 2023-2024 Andreas Warnke

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
