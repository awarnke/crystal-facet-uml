/* File: gui_sketch_background.inl; Copyright and License: see below */

#include "u8/u8_log.h"

static inline shape_int_rectangle_t gui_sketch_background_get_bounds( gui_sketch_background_t *this_ )
{
    return (*this_).bounds;
}

static inline void gui_sketch_background_set_bounds( gui_sketch_background_t *this_, shape_int_rectangle_t bounds )
{
    (*this_).bounds = bounds;
}



/*
Copyright 2018-2025 Andreas Warnke

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
