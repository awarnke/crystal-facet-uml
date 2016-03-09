/* File: gui_diagram_painter.c; Copyright and License: see below */

#include "gui_diagram_painter.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void gui_diagram_painter_init( gui_diagram_painter_t *this_, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    (*this_).db_reader = db_reader;

    TRACE_END();
}

void gui_diagram_painter_destroy( gui_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void gui_diagram_painter_draw ( gui_diagram_painter_t *this_, data_database_t *db, int64_t diagram_id, cairo_t *cr, geometry_rectangle_t destination )
{
    TRACE_BEGIN();
    TRACE_INFO_INT("drawing diagram id",diagram_id);

    double left, top, right, bottom;
    double width, height;

#if 0
    cairo_clip_extents ( cr, &left, &top, &right, &bottom );
    width = right-left;
    height = bottom-top;
#endif
    left = geometry_rectangle_get_left ( &destination );
    top = geometry_rectangle_get_top ( &destination );
    right = geometry_rectangle_get_right ( &destination );
    bottom = geometry_rectangle_get_bottom ( &destination );
    width = geometry_rectangle_get_width ( &destination );
    height = geometry_rectangle_get_height ( &destination );

    TRACE_INFO_INT( "w", (int)(width) );
    TRACE_INFO_INT( "h", (int)(height) );

    {
        data_diagram_t my_diag;
        data_error_t db_err;
        db_err= data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_id, &((*this_).private_current_diagram) );

        if ( DATA_ERROR_NONE == db_err ) {

            /* draw border line */
            cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
            cairo_rectangle ( cr, left+2.0, top+2.0, width-4.0, height-4.0 );
            cairo_stroke (cr);

            /* draw title corner */
            cairo_move_to ( cr, left+2.0, top+2.0+14.0 );
            cairo_line_to ( cr, left+(width/3.0), top+2.0+14.0 );
            cairo_line_to ( cr, left+(width/3.0)+4.0, top+2.0+14.0-4.0 );
            cairo_line_to ( cr, left+(width/3.0)+4.0, top+2.0 );
            cairo_stroke (cr);

            cairo_move_to ( cr, left+4.0, top+2.0+10.0 );
            cairo_show_text ( cr, utf8stringbuf_get_string( (*this_).private_current_diagram.name ) );
        }
        else
        {
            /* draw cross line */
            cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
            cairo_move_to ( cr, left, top );
            cairo_line_to ( cr, right, bottom );
            cairo_move_to ( cr, left, bottom );
            cairo_line_to ( cr, right, top );
            cairo_stroke (cr);
        }
    }

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
