/* File: gui_sketch_marker.inl; Copyright and License: see below */

#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static inline void gui_sketch_marker_init( gui_sketch_marker_t *this_ )
{
    pencil_marker_init( &((*this_).pencil_marker) );
}

static inline void gui_sketch_marker_destroy( gui_sketch_marker_t *this_ )
{
    pencil_marker_destroy( &((*this_).pencil_marker) );
}

static const double GUI_SKETCH_MARKER_BLACK_R = 0.0;
static const double GUI_SKETCH_MARKER_BLACK_G = 0.0;
static const double GUI_SKETCH_MARKER_BLACK_B = 0.0;
static const double GUI_SKETCH_MARKER_BLACK_A = 1.0;

static inline void gui_sketch_marker_prepare_draw ( const gui_sketch_marker_t *this_,
                                                    data_table_t table,
                                                    data_row_id_t row_id,
                                                    gui_marked_set_t *marked_set,
                                                    shape_int_rectangle_t rect,
                                                    cairo_t *cr )
{
    const data_id_t focused_id = gui_marked_set_get_focused ( marked_set );
    const data_row_id_t focused_diagram_id = gui_marked_set_get_focused_diagram ( marked_set );
    const data_id_t highlighted_id = gui_marked_set_get_highlighted ( marked_set );
    const data_small_set_t *const selected_set = gui_marked_set_get_selected_set_ptr ( marked_set );

    if ( DATA_TABLE_DIAGRAM == table )
    {
        /* mark focused diagram */
        if ( row_id == focused_diagram_id )
        {
            cairo_set_source_rgba( cr,
                                   GUI_SKETCH_MARKER_BLACK_R,
                                   GUI_SKETCH_MARKER_BLACK_G,
                                   GUI_SKETCH_MARKER_BLACK_B,
                                   GUI_SKETCH_MARKER_BLACK_A
                                 );
            cairo_rectangle ( cr,
                              shape_int_rectangle_get_left( &rect ),
                              shape_int_rectangle_get_top( &rect ),
                              shape_int_rectangle_get_width( &rect ),
                              shape_int_rectangle_get_height( &rect )
                            );
            cairo_stroke (cr);
        }

        /* mark focused */
        if ( row_id == data_id_get_row_id( &focused_id ) )
        {
            geometry_rectangle_t pencil_rect;
            geometry_rectangle_init( &pencil_rect,
                                     shape_int_rectangle_get_left( &rect ),
                                     shape_int_rectangle_get_top( &rect ),
                                     shape_int_rectangle_get_width( &rect ),
                                     shape_int_rectangle_get_height( &rect )
                                   );
            pencil_marker_mark_focused_rectangle( &((*this_).pencil_marker), pencil_rect, cr );
        }

        /* mark selected */
        if ( data_small_set_contains_row_id( selected_set, table, row_id ) )
        {
            geometry_rectangle_t pencil_rect;
            geometry_rectangle_init( &pencil_rect,
                                     shape_int_rectangle_get_left( &rect ),
                                     shape_int_rectangle_get_top( &rect ),
                                     shape_int_rectangle_get_width( &rect ),
                                     shape_int_rectangle_get_height( &rect )
                                   );
            pencil_marker_mark_selected_rectangle( &((*this_).pencil_marker), pencil_rect, cr );
        }

        /* set color to mark highlighted */
        if ( row_id == data_id_get_row_id( &highlighted_id ) )
        {
            /* use same color as in pencil_size.inl */
            cairo_set_source_rgba( cr, 0.0, 0.8, 0.6, 1.0 );
        }
        else
        {
            cairo_set_source_rgba( cr,
                                   GUI_SKETCH_MARKER_BLACK_R,
                                   GUI_SKETCH_MARKER_BLACK_G,
                                   GUI_SKETCH_MARKER_BLACK_B,
                                   GUI_SKETCH_MARKER_BLACK_A
                                 );
        }
    }
    else
    {
        TSLOG_WARNING ( "marking objects other than DATA_TABLE_DIAGRAM is not yet implemented" );
    }
}


/*
Copyright 2018-2020 Andreas Warnke

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
