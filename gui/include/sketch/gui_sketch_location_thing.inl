/* File: gui_sketch_location_thing.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <assert.h>

static inline void gui_sketch_location_thing_init( gui_sketch_location_thing_t *this_ )
{
    (*this_).kind = GUI_SKETCH_LOCATION_THING_KIND_BORDER;
}

static inline void gui_sketch_location_thing_destroy( gui_sketch_location_thing_t *this_ )
{
}


/*
Copyright 2024-2024 Andreas Warnke

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
