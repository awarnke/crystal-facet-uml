//! The module geometry provides a couple of geometric data structures.

/// Defines a point
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct Point {
    /// The x-coordinate
    pub x: f32,
    /// The y-coordinate
    pub y: f32,
}

/// Defines an delta offset
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct Offset {
    /// The x-coordinate offset
    pub dx: f32,
    /// The y-coordinate offset
    pub dy: f32,
}

/// The Point struct provides some methods
impl Point {
    /// rounds the x coordinate to a multiple of unit
    pub fn round_x(self: &Self, unit: f32) -> f32 {
        (self.x / unit).round() * unit
    }

    /// rounds the y coordinate to a multiple of unit
    pub fn round_y(self: &Self, unit: f32) -> f32 {
        (self.y / unit).round() * unit
    }

    /// adds an offset to a point, returns a new point
    pub fn add(position: Point, delta: Offset) -> Point {
        Point {
            x: position.x + delta.dx,
            y: position.y + delta.dy,
        }
    }
}

/// The Offset struct provides some methods
impl Offset {
    /// rounds the x coordinate to a multiple of unit
    pub fn round_dx(self: &Self, unit: f32) -> f32 {
        (self.dx / unit).round() * unit
    }

    /// rounds the y coordinate to a multiple of unit
    pub fn round_dy(self: &Self, unit: f32) -> f32 {
        (self.dy / unit).round() * unit
    }

    /// determines the offset between two points, returns an offset
    pub fn delta(from: Point, to: Point) -> Offset {
        Offset {
            dx: to.x - from.x,
            dy: to.y - from.y,
        }
    }
}

/// Defines the possible kinds of drawing irectived within paths
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum DrawDirective {
    /// A move moves the actual cursor without drawing a line.
    Move(Point),
    /// A move moves the actual cursor without drawing a line.
    MoveRel(Offset),
    /// A line draws a straight line and moves the cursor.
    Line(Point),
    /// A line draws a straight line and moves the cursor.
    LineRel(Offset),
    /// A continuation of the previous line with a single point as parameter.
    Continue(Point),
    /// A continuation of the previous line with a single offset as parameter.
    ContinueRel(Offset),
    /// A curve draws a curved line and moves the cursor.
    Curve(Point, Point, Point),
    /// A curve draws a curved line and moves the cursor.
    CurveRel(Offset, Offset, Offset),
    /// A symmetric curve continues a previous Curve,
    /// assuming a symmetric first control point,
    /// draws a curved line and moves the cursor.
    Symmetric(Point, Point),
    /// A symmetric curve continues a previous Curve,
    /// assuming a symmetric first control point,
    /// draws a curved line and moves the cursor.
    SymmetricRel(Offset, Offset),
    /// Close the subpath by drawing a straight line and moving the cursor.
    Close,
    /// Close the subpath by drawing a straight line and moving the cursor.
    /// CloseRel is identical to Close, it may be a matter of style to close a path
    /// in a similar way as drawing it.
    CloseRel,
}

/// Defines a rectangle
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct Rect {
    /// The left x-coordinate
    pub left: f32,
    /// The top y-coordinate, note that lower values refer to higher coordinates (more to the top)
    pub top: f32,
    /// The width
    pub width: f32,
    /// The height
    pub height: f32,
}

/// The Rect struct provides some methods
impl Rect {
    /// calculates the width
    pub fn right(self: &Self) -> f32 {
        self.left + self.width
    }

    /// calculates the height
    pub fn bottom(self: &Self) -> f32 {
        self.top + self.height
    }

    /// calculates the x_center
    pub fn x_center(self: &Self) -> f32 {
        self.left + (self.width / 2.0)
    }

    /// calculates the y_center
    pub fn y_center(self: &Self) -> f32 {
        self.top + (self.height / 2.0)
    }
}

/// Defines a color
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct Color {
    /// The red part
    pub red: u8,
    /// The green part
    pub green: u8,
    /// The blue part
    pub blue: u8,
}

/// The Color struct provides some methods
impl Color {
    /// calculates the width
    pub fn to_svg(self: &Self) -> String {
        format!("{:02x}{:02x}{:02x}", self.red, self.green, self.blue)
    }
    /// calculates a brigher color, starting from the current color
    pub fn brighter(self: &Self) -> Color {
        Color {
            red: (0xff - ((0xff - self.red) as u16 * 7) / 8) as u8,
            green: (0xff - ((0xff - self.green) as u16 * 7) / 8) as u8,
            blue: (0xff - ((0xff - self.blue) as u16 * 7) / 8) as u8,
        }
    }
}

/// The function defines the control points for a circle in absolute coordinates
///
/// # Arguments
///
/// * `cx` - The absolute x-coordinate of the center
/// * `cy` - The absolute y-coordinate of the center
/// * `rx` - The radius in x-direction
/// * `ry` - The radius in y-direction
///
pub fn get_circle_abs(cx: f32, cy: f32, rx: f32, ry: f32) -> [DrawDirective; 5] {
    let ctrlpnt_dx: f32 = rx * 0.552284749831; /* control point distance x */
    let ctrlpnt_dy: f32 = ry * 0.552284749831; /* control point distance y */
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
/// * `c_dx` - The relative x-offset of the center
/// * `c_dy` - The relative y-offset of the center
/// * `rx` - The radius in x-direction
/// * `ry` - The radius in y-direction
///
pub fn get_circle_rel(c_dx: f32, c_dy: f32, rx: f32, ry: f32) -> [DrawDirective; 5] {
    let ctrlpnt_dx: f32 = rx * 0.552284749831; /* control point distance x */
    let ctrlpnt_dy: f32 = ry * 0.552284749831; /* control point distance y */
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
