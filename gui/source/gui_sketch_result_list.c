/* File: gui_sketch_result_list.c; Copyright and License: see below */

#include "gui_sketch_result_list.h"
#include "util/geometry/geometry_rectangle.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>

void gui_sketch_result_list_init( gui_sketch_result_list_t *this_ )
{
    TRACE_BEGIN();

    (*this_).visible = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    TRACE_END();
}

void gui_sketch_result_list_destroy( gui_sketch_result_list_t *this_ )
{
    TRACE_BEGIN();

    shape_int_rectangle_destroy( &((*this_).bounds) );

    TRACE_END();
}

void gui_sketch_result_list_draw ( gui_sketch_result_list_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();

    if ( (*this_).visible )
    {
        int32_t left;
        int32_t top;
        uint32_t width;
        uint32_t height;

        left = shape_int_rectangle_get_left( &((*this_).bounds) );
        top = shape_int_rectangle_get_top( &((*this_).bounds) );
        width = shape_int_rectangle_get_width( &((*this_).bounds) );
        height = shape_int_rectangle_get_height( &((*this_).bounds) );

    }

    TRACE_END();
}


/*
Copyright 2018-2018 Andreas Warnke

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

