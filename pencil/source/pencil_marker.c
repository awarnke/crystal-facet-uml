/* File: pencil_marker.c; Copyright and License: see below */

#include "pencil_marker.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_marker_init( pencil_marker_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_marker_destroy( pencil_marker_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_marker_mark_focused_rectangle ( pencil_marker_t *this_, geometry_rectangle_t rect, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    double left;
    double top;
    double right;
    double bottom;

    left = geometry_rectangle_get_left ( &rect );
    top = geometry_rectangle_get_top ( &rect );
    right = geometry_rectangle_get_right ( &rect );
    bottom = geometry_rectangle_get_bottom ( &rect );

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

void pencil_marker_mark_selected_rectangle ( pencil_marker_t *this_, geometry_rectangle_t rect, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    double left;
    double top;
    double right;
    double bottom;
    static const double EXPAND_OVER_BORDER = 2.0;
    static const double INNER_EDGE = 8.0;

    left = geometry_rectangle_get_left ( &rect );
    top = geometry_rectangle_get_top ( &rect );
    right = geometry_rectangle_get_right ( &rect );
    bottom = geometry_rectangle_get_bottom ( &rect );

    cairo_set_source_rgba( cr, 1.0, 0.3, 0.8, 1.0 );

    cairo_move_to( cr, left+INNER_EDGE, top-EXPAND_OVER_BORDER );
    cairo_line_to( cr, left-EXPAND_OVER_BORDER, top-EXPAND_OVER_BORDER );
    cairo_line_to( cr, left-EXPAND_OVER_BORDER, top+INNER_EDGE );
    cairo_close_path( cr );
    cairo_fill (cr);

    cairo_move_to( cr, right-INNER_EDGE, top-EXPAND_OVER_BORDER );
    cairo_line_to( cr, right+EXPAND_OVER_BORDER, top-EXPAND_OVER_BORDER );
    cairo_line_to( cr, right+EXPAND_OVER_BORDER, top+INNER_EDGE );
    cairo_close_path( cr );
    cairo_fill (cr);

    cairo_move_to( cr, right-INNER_EDGE, bottom+EXPAND_OVER_BORDER );
    cairo_line_to( cr, right+EXPAND_OVER_BORDER, bottom+EXPAND_OVER_BORDER );
    cairo_line_to( cr, right+EXPAND_OVER_BORDER, bottom-INNER_EDGE );
    cairo_close_path( cr );
    cairo_fill (cr);

    cairo_move_to( cr, left-EXPAND_OVER_BORDER, bottom-INNER_EDGE );
    cairo_line_to( cr, left-EXPAND_OVER_BORDER, bottom+EXPAND_OVER_BORDER );
    cairo_line_to( cr, left+INNER_EDGE, bottom+EXPAND_OVER_BORDER );
    cairo_close_path( cr );
    cairo_fill (cr);

    TRACE_END();
}


/*
Copyright 2016-2017 Andreas Warnke

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
