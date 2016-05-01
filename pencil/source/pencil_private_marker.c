/* File: pencil_private_marker.c; Copyright and License: see below */

#include "pencil_private_marker.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void pencil_private_marker_init( pencil_private_marker_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_private_marker_destroy( pencil_private_marker_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_private_marker_focus_rectangle ( pencil_private_marker_t *this_, geometry_rectangle_t rect, cairo_t *cr )
{
    TRACE_BEGIN();

    double left, top, right, bottom;
    double width, height;

    left = geometry_rectangle_get_left ( &rect );
    top = geometry_rectangle_get_top ( &rect );
    right = geometry_rectangle_get_right ( &rect );
    bottom = geometry_rectangle_get_bottom ( &rect );
    width = geometry_rectangle_get_width ( &rect );
    height = geometry_rectangle_get_height ( &rect );

    cairo_set_source_rgba( cr, 0.9, 0.85, 0.0, 1.0 );

    cairo_rectangle ( cr, left-8.0, top-8.0, 3.0, 7.0 );
    cairo_fill (cr);

    cairo_rectangle ( cr, left-5.0, top-8.0, 4.0, 3.0 );
    cairo_fill (cr);

    cairo_rectangle ( cr, right+5.0, top-8.0, 3.0, 7.0 );
    cairo_fill (cr);

    cairo_rectangle ( cr, right+1.0, top-8.0, 4.0, 3.0 );
    cairo_fill (cr);

    cairo_rectangle ( cr, left-8.0, bottom+1.0, 3.0, 7.0 );
    cairo_fill (cr);

    cairo_rectangle ( cr, left-5.0, bottom+5.0, 4.0, 3.0 );
    cairo_fill (cr);

    cairo_rectangle ( cr, right+5.0, bottom+1.0, 3.0, 7.0 );
    cairo_fill (cr);

    cairo_rectangle ( cr, right+1.0, bottom+5.0, 4.0, 3.0 );
    cairo_fill (cr);

    TRACE_END();
}


/*
Copyright 2016-2016 Andreas Warnke

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
