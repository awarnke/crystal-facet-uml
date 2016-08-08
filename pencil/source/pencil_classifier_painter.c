/* File: pencil_classifier_painter.c; Copyright and License: see below */

#include "pencil_classifier_painter.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void pencil_classifier_painter_init( pencil_classifier_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_private_marker_init( &((*this_).marker) );

    TRACE_END();
}

void pencil_classifier_painter_destroy( pencil_classifier_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_private_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_classifier_painter_draw ( pencil_classifier_painter_t *this_,
                                      pencil_input_data_t *input_data,
                                      data_id_t mark_focused,
                                      data_id_t mark_highlighted,
                                      data_small_set_t *mark_selected,
                                      pencil_size_t *pencil_size,
                                      cairo_t *cr,
                                      geometry_rectangle_t destination )
{
    TRACE_BEGIN();
    geometry_rectangle_t focused_rect;
    geometry_rectangle_init_empty( &focused_rect );

    double left, top, right, bottom;
    double width, height;

    left = geometry_rectangle_get_left ( &destination );
    top = geometry_rectangle_get_top ( &destination );
    right = geometry_rectangle_get_right ( &destination );
    bottom = geometry_rectangle_get_bottom ( &destination );
    width = geometry_rectangle_get_width ( &destination );
    height = geometry_rectangle_get_height ( &destination );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    /* iterate over all classifiers */
    {
        uint32_t count;
        count = pencil_input_data_get_visible_classifier_count ( input_data );
        for ( uint32_t index = 0; index < count; index ++ )
        {
            data_visible_classifier_t *visible_classifier;
            visible_classifier = pencil_input_data_get_visible_classifier_ptr ( input_data, index );

            if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
            {
                data_classifier_t *classifier;
                data_diagramelement_t *diagramelement;
                classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
                diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );

                TRACE_INFO_INT("drawing classifier id", data_classifier_get_id( classifier ) );

                double std_line_width = pencil_size_get_standard_line_width( pencil_size );
                cairo_set_line_width( cr, std_line_width );

                double box_top;
                double box_height;
                box_height = height/(double)count;
                box_top = (double)index*box_height+top;

                if ( data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) ))
                {
                    GdkRGBA highlight_color;
                    highlight_color = pencil_size_get_highlight_color( pencil_size );
                    cairo_set_source_rgba( cr, highlight_color.red, highlight_color.green, highlight_color.blue, highlight_color.alpha );
                }
                else
                {
                    GdkRGBA standard_color;
                    standard_color = pencil_size_get_standard_color( pencil_size );
                    cairo_set_source_rgba( cr, standard_color.red, standard_color.green, standard_color.blue, standard_color.alpha );
                }
                cairo_rectangle ( cr, left+gap, box_top+gap, width-2.0*gap, box_height-2.0*gap );
                cairo_stroke (cr);

                {
                    double std_font_size = pencil_size_get_standard_font_size(pencil_size);
                    double big_font_size = pencil_size_get_larger_font_size(pencil_size);
                    double f_std_size = pencil_size_get_standard_font_size( pencil_size );
                    double f_std_ascent = pencil_size_get_standard_font_ascent( pencil_size );
                    double f_big_ascent = pencil_size_get_larger_font_ascent( pencil_size );

                    char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
                    utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
                    utf8stringbuf_copy_str( stereotype_buf, "<<" );
                    utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_ptr( classifier ) );
                    utf8stringbuf_append_str( stereotype_buf, ">>" );
                    if ( utf8stringbuf_get_length( stereotype_buf ) == 4 )
                    {
                        cairo_set_font_size ( cr, big_font_size );
                        cairo_move_to ( cr, left+2.0*gap, box_top+gap+f_line_gap+f_big_ascent );
                        cairo_show_text ( cr, data_classifier_get_name_ptr( classifier ));
                    }
                    else
                    {
                        cairo_set_font_size ( cr, std_font_size );
                        cairo_move_to ( cr, left+2.0*gap, box_top+gap+f_line_gap+f_std_ascent );
                        cairo_show_text ( cr, utf8stringbuf_get_string( stereotype_buf ));
                        cairo_set_font_size ( cr, big_font_size );
                        cairo_move_to ( cr, left+2.0*gap, box_top+gap+2.0*f_line_gap+f_std_size+f_big_ascent );
                        cairo_show_text ( cr, data_classifier_get_name_ptr( classifier ));
                    }
                }

                if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ))
                {
                    geometry_rectangle_reinit( &focused_rect, left+gap, box_top+gap, width-2.0*gap, box_height-2.0*gap );
                }
                if ( data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ))
                {
                    geometry_rectangle_t selected_rect;
                    geometry_rectangle_init( &selected_rect, left+gap, box_top+gap, width-2.0*gap, box_height-2.0*gap );
                    pencil_private_marker_mark_selected_rectangle( &((*this_).marker), selected_rect, cr );
                    geometry_rectangle_destroy( &selected_rect );
                }
            }
            else
            {
                LOG_ERROR("invalid visible classifier in array!");
            }
        }
    }

    /* mark focused */
    {
        if ( ! geometry_rectangle_is_empty( &focused_rect ) )
        {
            pencil_private_marker_mark_focused_rectangle( &((*this_).marker), focused_rect, cr );
        }
    }

    geometry_rectangle_destroy( &focused_rect );
    TRACE_END();
}

data_id_t pencil_classifier_painter_get_object_id_at_pos ( pencil_classifier_painter_t *this_,
                                                           pencil_input_data_t *input_data,
                                                           double x,
                                                           double y,
                                                           geometry_rectangle_t destination,
                                                           bool dereference )
{
    TRACE_BEGIN();
    data_id_t result;
    data_id_init_void( &result );

    if ( geometry_rectangle_contains( &destination, x, y ) )
    {
        double top;
        double height;
        top = geometry_rectangle_get_top ( &destination );
        height = geometry_rectangle_get_height ( &destination );

        uint32_t count;
        count = pencil_input_data_get_visible_classifier_count ( input_data );
        uint32_t index = (uint32_t) (((y-top) * count) / height);
        if ( index < count )
        {
            data_visible_classifier_t *visible_classifier;
            visible_classifier = pencil_input_data_get_visible_classifier_ptr ( input_data, index );
            if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
            {
                if ( dereference )
                {
                    data_classifier_t *classifier;
                    classifier = data_visible_classifier_get_classifier_ptr ( visible_classifier );
                    data_id_reinit( &result, DATA_TABLE_CLASSIFIER, data_classifier_get_id( classifier ) );
                }
                else
                {
                    data_diagramelement_t *diagramelement;
                    diagramelement = data_visible_classifier_get_diagramelement_ptr ( visible_classifier );
                    data_id_reinit( &result, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) );
                }
            }
        }
    }

    TRACE_END();
    return result;
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
