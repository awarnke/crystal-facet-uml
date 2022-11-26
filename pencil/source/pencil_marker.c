/* File: pencil_marker.c; Copyright and License: see below */

#include "pencil_marker.h"
#include "u8/u8_trace.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_marker_init( pencil_marker_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_END();
}

void pencil_marker_destroy( pencil_marker_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_END();
}

void pencil_marker_mark_focused_rectangle ( const pencil_marker_t *this_, geometry_rectangle_t rect, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    static const double YELLOW_BOX_SIZE = 8.0;
    static const double TOTAL_SIZE = 9.0;
    static const double LINE_WIDTH = 1.0;
    static const double GAP = 1.0;

    const double left = floor(geometry_rectangle_get_left ( &rect )); /* floor needed to position on pixel boundary*/
    const double top = floor(geometry_rectangle_get_top ( &rect ));
    const double right = floor(geometry_rectangle_get_right ( &rect ));
    const double bottom = floor(geometry_rectangle_get_bottom ( &rect ));

    /* gray lines */
    cairo_set_source_rgba( cr, 0.55, 0.55, 0.55, 1.0 );
    
    cairo_rectangle ( cr, left-GAP-LINE_WIDTH, top-GAP-TOTAL_SIZE, LINE_WIDTH, YELLOW_BOX_SIZE );
    cairo_rectangle ( cr, left-GAP-TOTAL_SIZE, top-GAP-LINE_WIDTH, TOTAL_SIZE, LINE_WIDTH );
    cairo_fill (cr);

    cairo_rectangle ( cr, right+GAP, top-GAP-TOTAL_SIZE, LINE_WIDTH, YELLOW_BOX_SIZE );
    cairo_rectangle ( cr, right+GAP, top-GAP-LINE_WIDTH, TOTAL_SIZE, LINE_WIDTH );
    cairo_fill (cr);

    cairo_rectangle ( cr, right+GAP, bottom+GAP+LINE_WIDTH, LINE_WIDTH, YELLOW_BOX_SIZE );
    cairo_rectangle ( cr, right+GAP, bottom+GAP, TOTAL_SIZE, LINE_WIDTH );
    cairo_fill (cr);

    cairo_rectangle ( cr, left-GAP-LINE_WIDTH, bottom+GAP+LINE_WIDTH, LINE_WIDTH, YELLOW_BOX_SIZE );
    cairo_rectangle ( cr, left-GAP-TOTAL_SIZE, bottom+GAP, TOTAL_SIZE, LINE_WIDTH );
    cairo_fill (cr);

    /* yellow box */
    cairo_set_source_rgba( cr, 0.95, 1.0, 0.0, 1.0 );

    cairo_rectangle ( cr, left-GAP-TOTAL_SIZE, top-GAP-TOTAL_SIZE, YELLOW_BOX_SIZE, YELLOW_BOX_SIZE );
    cairo_fill (cr);

    cairo_rectangle ( cr, right+GAP+LINE_WIDTH, top-GAP-TOTAL_SIZE, YELLOW_BOX_SIZE, YELLOW_BOX_SIZE );
    cairo_fill (cr);

    cairo_rectangle ( cr, left-GAP-TOTAL_SIZE, bottom+GAP+LINE_WIDTH, YELLOW_BOX_SIZE, YELLOW_BOX_SIZE );
    cairo_fill (cr);

    cairo_rectangle ( cr, right+GAP+LINE_WIDTH, bottom+GAP+LINE_WIDTH, YELLOW_BOX_SIZE, YELLOW_BOX_SIZE );
    cairo_fill (cr);

    U8_TRACE_END();
}

void pencil_marker_mark_selected_rectangle ( const pencil_marker_t *this_, geometry_rectangle_t rect, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    static const double EXPAND_OVER_BORDER = 2.0;
    static const double INNER_EDGE = 8.0;

    const double left = floor(geometry_rectangle_get_left ( &rect )); /* floor needed to position on pixel boundary*/
    const double top = floor(geometry_rectangle_get_top ( &rect ));
    const double right = floor(geometry_rectangle_get_right ( &rect ));
    const double bottom = floor(geometry_rectangle_get_bottom ( &rect ));

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

    const double width = right-left;
    static const double LINE_LENGTH = 16.0;
    static const double LINE_WIDTH = 1.0;
    if ( width > 2.0*EXPAND_OVER_BORDER + LINE_LENGTH )
    {
        cairo_rectangle ( cr, left, bottom + EXPAND_OVER_BORDER - LINE_WIDTH, LINE_LENGTH, LINE_WIDTH );
        cairo_rectangle ( cr, right - LINE_LENGTH, bottom + EXPAND_OVER_BORDER - LINE_WIDTH, LINE_LENGTH, LINE_WIDTH );
        cairo_fill (cr);
        cairo_rectangle ( cr, left, top - EXPAND_OVER_BORDER, LINE_LENGTH, LINE_WIDTH );
        cairo_rectangle ( cr, right - LINE_LENGTH, top - EXPAND_OVER_BORDER, LINE_LENGTH, LINE_WIDTH );
        cairo_fill (cr);
    }
    const double height = bottom-top;
    if ( height > 2.0*EXPAND_OVER_BORDER + LINE_LENGTH )
    {
        cairo_rectangle ( cr, left - EXPAND_OVER_BORDER, top, LINE_WIDTH, LINE_LENGTH );
        cairo_rectangle ( cr, left - EXPAND_OVER_BORDER, bottom - LINE_LENGTH, LINE_WIDTH, LINE_LENGTH );
        cairo_fill (cr);
        cairo_rectangle ( cr, right + EXPAND_OVER_BORDER - LINE_WIDTH, top, LINE_WIDTH, LINE_LENGTH );
        cairo_rectangle ( cr, right + EXPAND_OVER_BORDER - LINE_WIDTH, bottom - LINE_LENGTH, LINE_WIDTH, LINE_LENGTH );
        cairo_fill (cr);
    }

    U8_TRACE_END();
}


/*
Copyright 2016-2022 Andreas Warnke

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
