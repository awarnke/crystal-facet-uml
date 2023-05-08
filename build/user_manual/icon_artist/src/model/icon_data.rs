//! The module provides functions to render a model to vector graphics.

use super::geometry;
use super::geometry::Color;
use super::geometry::DrawDirective::Line;
use super::geometry::DrawDirective::Move;
use super::geometry::Point;

/// Defines a plant
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct IconData {
    /// The x-coordinate
    pub root_x: f32,
    /// The y-coordinate
    pub root_y: f32,
    /// The angle at trunk
    pub trunk_angle: f32,
    /// The length of the trunk
    pub trunk_len: f32,
    /// The color of the plant
    pub color: Color,
}

const MAX_REC: usize = 7;

/// The Rect struct provides some methods
impl IconData {
    /// The function converts a recursive_plant to vector graphics drawing directives
    ///
    fn render_plant_rec(
        self: &Self,
        rec_budget: usize,
        segments: &mut [geometry::DrawDirective; MAX_REC + 1],
    ) -> () {
        let next_x: f32 = self.root_x + (f32::cos(self.trunk_angle) * self.trunk_len);
        let next_y: f32 = self.root_y - (f32::sin(self.trunk_angle) * self.trunk_len);
        segments[MAX_REC - rec_budget] = Line(Point {
            x: next_x,
            y: next_y,
        });
        if (rec_budget > 0) && (self.trunk_len > 4.0) {
            let branch_part: IconData = IconData {
                root_x: next_x,
                root_y: next_y,
                trunk_angle: self.trunk_angle + 0.3,
                trunk_len: self.trunk_len * 0.8,
                color: self.color.brighter(),
            };
            branch_part.render_plant_rec(rec_budget - 1, segments);

            //  do the arm
            let mut arm_1_segs: [geometry::DrawDirective; MAX_REC + 1] =
                [Move(Point { x: 0.0, y: 0.0 }); MAX_REC + 1];
            arm_1_segs[0] = Move(Point {
                x: next_x,
                y: next_y,
            });
            let branch_1_part: IconData = IconData {
                root_x: next_x,
                root_y: next_y,
                trunk_angle: self.trunk_angle + 1.3,
                trunk_len: self.trunk_len * 0.5,
                color: self.color.brighter(),
            };
            branch_1_part.render_plant_rec(rec_budget - 1, &mut arm_1_segs);

            let mut arm_2_segs: [geometry::DrawDirective; MAX_REC + 1] =
                [Move(Point { x: 0.0, y: 0.0 }); MAX_REC + 1];
            arm_2_segs[0] = Move(Point {
                x: next_x,
                y: next_y,
            });
            let branch_2_part: IconData = IconData {
                root_x: next_x,
                root_y: next_y,
                trunk_angle: self.trunk_angle - 1.1,
                trunk_len: self.trunk_len * 0.3,
                color: self.color.brighter(),
            };
            branch_2_part.render_plant_rec(rec_budget - 1, &mut arm_2_segs);
        }
    }

    /// The function converts a recursive_plant to vector graphics drawing directives
    ///
    pub fn render_plant(self: &Self) -> () {
        let mut plant_segs: [geometry::DrawDirective; MAX_REC + 1] =
            [Move(Point { x: 0.0, y: 0.0 }); MAX_REC + 1];
        plant_segs[0] = Move(Point {
            x: self.root_x,
            y: self.root_y,
        });
    }
}

/// The function returns an array of IconData
///
pub fn get_icons() -> &'static [IconData] {
    &[]
}
