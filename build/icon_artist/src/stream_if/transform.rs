//! The module transform provides a couple of simple 2D transformations.

use crate::stream_if::geometry::Offset;
use crate::stream_if::geometry::Point;

/// The function moves a slice of points by an offset
///
/// # Arguments
///
/// * `points` - The absolute coordinates of a slice of points
/// * `offset` - The offset by which to move the point
///
pub fn move_points(points: &mut [Point], offset: Offset) -> () {
    for point in points {
        *point = Point {
            x: point.x + offset.dx,
            y: point.y + offset.dy,
        }
    }
}

/// The function scales a slice of points by a factor, relative to 0,0
///
/// # Arguments
///
/// * `points` - The absolute coordinates of a slice of points
/// * `scale_x` - The x-scaling factor by which to scale the point
/// * `scale_y` - The y-scaling factor by which to scale the point
///
pub fn scale_points(points: &mut [Point], scale_x: f32, scale_y: f32) -> () {
    for point in points {
        *point = Point {
            x: point.x * scale_x,
            y: point.y * scale_y,
        }
    }
}

/// The function rotates a slice of points around the center 0,0
///
/// # Arguments
///
/// * `points` - The absolute coordinates of a slice of points
/// * `angle` - The rotating angle in red by which to rotate the point
///
pub fn rotate_points(points: &mut [Point], angle: f32) -> () {
    let sin_a = angle.sin();
    let cos_a = angle.cos();
    for point in points {
        *point = Point {
            x: point.x * cos_a - point.y * sin_a,
            y: point.x * sin_a + point.y * cos_a,
        }
    }
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
