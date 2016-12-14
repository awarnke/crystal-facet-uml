/* File: pencil_classifier_painter.c; Copyright and License: see below */

#include "pencil_classifier_painter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
                                      data_visible_classifier_t *visible_classifier,
                                      bool mark_focused,
                                      bool mark_highlighted,
                                      bool mark_selected,
                                      pencil_size_t *pencil_size,
                                      cairo_t *cr,
                                      geometry_rectangle_t classifier_bounds )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != cr );

    double left, top;
    double width, height;

    left = geometry_rectangle_get_left ( &classifier_bounds );
    top = geometry_rectangle_get_top ( &classifier_bounds );
    width = geometry_rectangle_get_width ( &classifier_bounds );
    height = geometry_rectangle_get_height ( &classifier_bounds );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
    {
        data_classifier_t *classifier;
        data_diagramelement_t *diagramelement;
        classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
        diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );

        TRACE_INFO_INT("drawing classifier id", data_classifier_get_id( classifier ) );

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );

        if ( mark_highlighted )
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
        cairo_rectangle ( cr, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
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
                cairo_move_to ( cr, left+2.0*gap, top+gap+f_line_gap+f_big_ascent );
                cairo_show_text ( cr, data_classifier_get_name_ptr( classifier ));

                {
                    PangoLayout *layout;
                    PangoFontDescription *desc;

                    layout = pango_cairo_create_layout (cr);

                    desc = pango_font_description_from_string ("Sans 18px");
                    pango_layout_set_font_description (layout, desc);
                    pango_font_description_free (desc);

                    pango_layout_set_text (layout, data_classifier_get_name_ptr( classifier ), -1);

                    int text_width, text_height;
                    pango_layout_get_pixel_size (layout, &text_width, &text_height);

                    cairo_move_to ( cr, left+2.0*gap + 0.5 *(width-text_width), top+gap+f_line_gap+f_big_ascent );
                    pango_cairo_show_layout (cr, layout);

                    g_object_unref (layout);
                }
            }
            else
            {
                cairo_set_font_size ( cr, std_font_size );
                cairo_move_to ( cr, left+2.0*gap, top+gap+f_line_gap+f_std_ascent );
                cairo_show_text ( cr, utf8stringbuf_get_string( stereotype_buf ));
                cairo_set_font_size ( cr, big_font_size );
                cairo_move_to ( cr, left+2.0*gap, top+gap+2.0*f_line_gap+f_std_size+f_big_ascent );
                cairo_show_text ( cr, data_classifier_get_name_ptr( classifier ));
            }
        }

        if ( mark_selected )
        {
            pencil_private_marker_mark_selected_rectangle( &((*this_).marker), classifier_bounds, cr );
        }
    }
    else
    {
        TSLOG_ERROR("invalid visible classifier in array!");
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
