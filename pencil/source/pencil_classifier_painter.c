/* File: pencil_classifier_painter.c; Copyright and License: see below */

#include "pencil_classifier_painter.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void pencil_classifier_painter_init( pencil_classifier_painter_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_classifier_painter_destroy( pencil_classifier_painter_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_classifier_painter_draw ( pencil_classifier_painter_t *this_, pencil_input_data_t *input_data, cairo_t *cr, geometry_rectangle_t destination )
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

    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( input_data );

    /* iterate over all classifiers */
    {
        uint32_t count;
        count = pencil_input_data_get_classifier_count ( input_data );
        for ( uint32_t index = 0; index < count; index ++ )
        {
            data_classifier_t *classifier;
            classifier = pencil_input_data_get_classifier_ptr ( input_data, index );
            if (( classifier != NULL ) && ( data_classifier_is_valid( classifier ) ))
            {
                TRACE_INFO_INT("drawing classifier id", data_classifier_get_id( classifier ) );

                double box_top;
                double box_height;
                box_height = height;

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
                cairo_show_text ( cr, data_classifier_get_name_ptr( classifier ));
            }
            else
            {
                LOG_ERROR("invalid classifier in array!");
            }
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
