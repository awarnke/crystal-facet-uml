/* File: gui_sketch_marker.c; Copyright and License: see below */

#include "sketch/gui_sketch_marker.h"
#include "trace/trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const double BLACK_R = 0.0;
static const double BLACK_G = 0.0;
static const double BLACK_B = 0.0;
static const double BLACK_A = 1.0;
static const double WHITE_R = 1.0;
static const double WHITE_G = 1.0;
static const double WHITE_B = 1.0;
static const double WHITE_A = 1.0;

static const double LINE_W = 1.0;

void gui_sketch_marker_prepare_draw ( const gui_sketch_marker_t *this_,
                                      data_id_t element_id,
                                      const gui_marked_set_t *marked_set,
                                      shape_int_rectangle_t rect,
                                      cairo_t *cr )
{
    const data_id_t focused_id = gui_marked_set_get_focused_obj ( marked_set );
    const data_id_t highlighted_id = gui_marked_set_get_highlighted ( marked_set );
    const data_id_t highlighted_diag_id = gui_marked_set_get_highlighted_diagram ( marked_set );
    const data_small_set_t *const selected_set = gui_marked_set_get_selected_set_const ( marked_set );

    const int32_t left = shape_int_rectangle_get_left( &rect );
    const int32_t top = shape_int_rectangle_get_top( &rect );
    const uint32_t width = shape_int_rectangle_get_width( &rect );
    const uint32_t height = shape_int_rectangle_get_height( &rect );

    /* mark highlighted */
    if (( data_id_equals( &element_id, &highlighted_id ) )
        ||( data_id_equals( &element_id, &highlighted_diag_id ) ))
    {
        /* draw a white border around the target rectangle */
        {
            cairo_set_source_rgba( cr, WHITE_R, WHITE_G, WHITE_B, WHITE_A );
            cairo_rectangle ( cr, left-3, top-1, 2.0*LINE_W, height+2 );
            cairo_rectangle ( cr, left-3, top-3, width+6, 2.0*LINE_W );
            cairo_rectangle ( cr, left+width+1, top-1, 2.0*LINE_W, height+2 );
            cairo_rectangle ( cr, left-3, top+height+1, width+6, 2.0*LINE_W );
            cairo_fill (cr);
        }
    }

    /* mark focused diagram */
    if ( data_id_equals( &element_id, &focused_id ) )
    {
        cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
        cairo_rectangle ( cr, left-2, top-1, LINE_W, height+2 );
        cairo_rectangle ( cr, left-2, top-2, width+4, LINE_W );
        cairo_rectangle ( cr, left+width+1, top-1, LINE_W, height+2 );
        cairo_rectangle ( cr, left-2, top+height+1, width+4, LINE_W );
        cairo_fill (cr);
    }

    /* mark focused */
    if ( data_id_equals( &element_id, &focused_id ) && (*this_).with_pencil_markers )
    {
        geometry_rectangle_t pencil_rect;
        geometry_rectangle_init( &pencil_rect, left, top, width, height );
        pencil_marker_mark_focused_rectangle( &((*this_).pencil_marker), pencil_rect, cr );
    }

    /* mark selected */
    if ( data_small_set_contains( selected_set, element_id ) && (*this_).with_pencil_markers )
    {
        geometry_rectangle_t pencil_rect;
        geometry_rectangle_init( &pencil_rect, left, top, width, height );
        pencil_marker_mark_selected_rectangle( &((*this_).pencil_marker), pencil_rect, cr );
    }

    /* set color to mark highlighted */
    if ( data_id_equals( &element_id, &highlighted_id ) )
    {
        /* use same color as in pencil_size.inl */
        cairo_set_source_rgba( cr, 0.0, 0.6, 0.4, 1.0 );
    }
    else
    {
        cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
    }
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
