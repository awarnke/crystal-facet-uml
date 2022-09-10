/* File: gui_sketch_marker.inl; Copyright and License: see below */

#include "trace/trace.h"
#include <assert.h>

static inline void gui_sketch_marker_init( gui_sketch_marker_t *this_, bool with_pencil_markers )
{
    (*this_).with_pencil_markers = with_pencil_markers;
    pencil_marker_init( &((*this_).pencil_marker) );
}

static inline void gui_sketch_marker_destroy( gui_sketch_marker_t *this_ )
{
    pencil_marker_destroy( &((*this_).pencil_marker) );
}


/*
Copyright 2018-2022 Andreas Warnke

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
