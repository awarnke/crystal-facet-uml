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

/// Defines the possible kinds of drawing irectived within paths
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum DrawDirective {
    /// A move moves the actual cursor without drawing a line
    Move(Point),
    /// A line moves the cursor and draws a straight line
    Line(Point),
    /// A curve moves the cursor and draws a curved line
    Curve(Point, Point, Point),
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
