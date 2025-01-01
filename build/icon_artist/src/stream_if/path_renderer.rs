//! The module geometry provides a couple of geometric data structures.

use crate::stream_if::geometry::Color;
use crate::stream_if::geometry::DrawDirective;
use crate::stream_if::geometry::Pen;

/// A path renderer is an object that provides a function to
/// draw a path.
///
/// # Lifetimes
///
/// * `'all_testing` refers to the lifetime of `TestSuite` and `TestCase`
///   objects: They exist during the whole test run.
///
pub trait PathRenderer {
    /// This function renders a path; it is called by a drawing source
    /// and implemented by a drawing sink (renderer).
    ///
    /// # Arguments
    ///
    /// * `segs` - A list of drawing directives which is a path
    /// * `stroke_color_width` - A stroke color and width
    /// * `fill_color` - A fill color
    fn render_path(
        self: &mut Self,
        segs: &[DrawDirective],
        stroke: &Option<Pen>,
        fill: &Option<Color>,
    ) -> ();
}

/*
Copyright 2024-2025 Andreas Warnke

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
