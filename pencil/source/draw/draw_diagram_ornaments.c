/* File: draw_diagram_ornaments.c; Copyright and License: see below */

#include "draw/draw_diagram_ornaments.h"
#include "u8/u8_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

void draw_diagram_ornaments_init( draw_diagram_ornaments_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_END();
}

void draw_diagram_ornaments_destroy( draw_diagram_ornaments_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_END();
}

void draw_diagram_ornaments_draw_scale( const draw_diagram_ornaments_t *this_,
                                        const geometry_rectangle_t *bounds,
                                        const pencil_size_t *pencil_size,
                                        cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double left = geometry_rectangle_get_left ( bounds );
    const double top = geometry_rectangle_get_top ( bounds );
    const double width = geometry_rectangle_get_width ( bounds );
    const double height = geometry_rectangle_get_height ( bounds );
    const double bottom = top + height;

    const unsigned int MAX_STEP = 60;
    for ( unsigned int step = 0; step <= MAX_STEP; step ++ )
    {
        const double x = left + ( step * width ) / MAX_STEP;
        const double phi = 1.6180339;
        const double line_top = ( ( step % 5 ) == 0 ) ? top : top + ( height / phi );
        cairo_move_to ( cr, x, line_top );
        cairo_line_to ( cr, x, bottom );
        cairo_stroke (cr);
    }

    U8_TRACE_END();
}


/*
Copyright 2026-2026 Andreas Warnke
    http://www.apache.org/licenses/LICENSE-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
