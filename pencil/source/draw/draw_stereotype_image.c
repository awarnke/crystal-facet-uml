/* File: draw_stereotype_image.c; Copyright and License: see below */

#include "draw/draw_stereotype_image.h"
#include "pencil_layout_data.h"
#include "u8/u8_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const double DRAW_STEREOTYPE_IMAGE_WIDTH_TO_HEIGHT = 1.0;

void draw_stereotype_image_draw ( const draw_stereotype_image_t *this_,
                                  const char *image_description,
                                  const geometry_rectangle_t *bounds,
                                  cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != image_description );
    assert( NULL != bounds );

    /* determine linewith to avoid that drawings overlap to the outside of bounds */
    const double ln_w = cairo_get_line_width( cr );

    /* calculate artifact bounds */
    const double art_left = geometry_rectangle_get_left( bounds );
    const double art_right = geometry_rectangle_get_right( bounds );
    const double art_top = geometry_rectangle_get_top( bounds );
    const double art_bottom = geometry_rectangle_get_bottom( bounds );
    const double art_height = geometry_rectangle_get_height( bounds );
    //const double art_width = geometry_rectangle_get_width( bounds );
    const double art_corner_edge = art_height * 0.3;

    /* draw the icon */
    cairo_set_source_rgba( cr, 0.0, 0.5, 0.0, 1.0 );
    cairo_move_to ( cr, art_right, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top );
    cairo_line_to ( cr, art_left, art_top );
    cairo_line_to ( cr, art_left, art_bottom );
    cairo_line_to ( cr, art_right, art_bottom );
    cairo_line_to ( cr, art_right, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top );
    cairo_stroke (cr);

#ifdef PENCIL_LAYOUT_DATA_DRAW_FOR_DEBUG
        /* draw the rectangle */
        {
            cairo_set_source_rgba( cr, 1.0, 0.5, 0.6, 0.5 );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( bounds ),
                              geometry_rectangle_get_top ( bounds ),
                              geometry_rectangle_get_width ( bounds ),
                              geometry_rectangle_get_height ( bounds )
                            );
            cairo_stroke (cr);
            cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
        }
#endif

    U8_TRACE_END();
}


/*
Copyright 2023-2023 Andreas Warnke
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
