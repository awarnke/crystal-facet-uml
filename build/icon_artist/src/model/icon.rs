//! The module provides functions to render an icon to vector graphics.

use crate::stream_if::geometry::Rect;
use crate::stream_if::path_renderer::PathRenderer;

/// Defines the data needed to render an icon
pub struct IconSource<'my_lifespan> {
    /// The name
    pub name: &'my_lifespan str,
    /// The rectangle that defines the visible region
    pub viewport: Rect,
    /// The generator function
    pub generate: fn(out: &mut dyn PathRenderer) -> (),
}

/*
Copyright 2023-2026 Andreas Warnke

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
