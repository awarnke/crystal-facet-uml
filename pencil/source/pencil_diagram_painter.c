/* File: pencil_diagram_painter.c; Copyright and License: see below */

#include "pencil_diagram_painter.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void pencil_diagram_painter_init( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_painter_init( &((*this_).classifier_painter) );

    TRACE_END();
}

void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_painter_destroy( &((*this_).classifier_painter) );

    TRACE_END();
}

void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   pencil_input_data_t *input_data,
                                   data_id_t *mark_focused,
                                   data_id_t *mark_highlighted,
                                   data_small_set_t *mark_selected,
                                   cairo_t *cr,
                                   geometry_rectangle_t destination )
{
    TRACE_BEGIN();

    double left, top, right, bottom;
    double width, height;

    left = geometry_rectangle_get_left ( &destination );
    top = geometry_rectangle_get_top ( &destination );
    right = geometry_rectangle_get_right ( &destination );
    bottom = geometry_rectangle_get_bottom ( &destination );
    width = geometry_rectangle_get_width ( &destination );
    height = geometry_rectangle_get_height ( &destination );

    TRACE_INFO_INT( "w", (int)(width) );
    TRACE_INFO_INT( "h", (int)(height) );

    /* draw diagram border and name */
    {
        data_diagram_t *diag = pencil_input_data_get_diagram_ptr( input_data );
        TRACE_INFO_INT("drawing diagram id",data_diagram_get_id(diag));
        if ( data_diagram_is_valid(diag) ) {

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
            cairo_show_text ( cr, data_diagram_get_name_ptr( diag ) );
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

    /* draw all contained classifiers */
    if (( width > 10.0 ) && ( height > 20.0 ))
    {
        geometry_rectangle_t destination;
        geometry_rectangle_init ( &destination, left + 5.0, top + 15.0, width - 10.0, height - 20.0 );
        pencil_classifier_painter_draw ( &((*this_).classifier_painter), input_data, cr, destination );
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
