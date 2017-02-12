/* File: pencil_feature_painter.c; Copyright and License: see below */

#include "pencil_feature_painter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_feature_painter_init( pencil_feature_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );

    TRACE_END();
}

void pencil_feature_painter_destroy( pencil_feature_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_feature_painter_draw ( pencil_feature_painter_t *this_,
                                   data_feature_t *the_feature,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   pencil_size_t *pencil_size,
                                   geometry_rectangle_t *feature_bounds,
                                   PangoLayout *layout,
                                   cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != the_feature );
    assert( NULL != layout );
    assert( NULL != cr );
    assert( NULL != feature_bounds );

    double left, top;
    double width, height;

    left = geometry_rectangle_get_left ( feature_bounds );
    top = geometry_rectangle_get_top ( feature_bounds );
    width = geometry_rectangle_get_width ( feature_bounds );
    height = geometry_rectangle_get_height ( feature_bounds );

    double gap = pencil_size_get_standard_object_border( pencil_size );

    if ( data_feature_is_valid( the_feature ) )
    {
        TRACE_INFO_INT("drawing feature id", data_feature_get_id( the_feature ) );

        /* draw text */
        {
            /* select color */
            GdkRGBA foreground_color;
            {
                if ( mark_highlighted )
                {
                    foreground_color = pencil_size_get_highlight_color( pencil_size );
                }
                else
                {
                    foreground_color = pencil_size_get_standard_color( pencil_size );
                }
            }

            /* prepare text */
            char label_text[DATA_FEATURE_MAX_KEY_SIZE + DATA_FEATURE_MAX_VALUE_SIZE + 2 ];
            utf8stringbuf_t label_buf = UTF8STRINGBUF(label_text);
            utf8stringbuf_copy_str( label_buf, data_feature_get_key_ptr( the_feature ) );
            utf8stringbuf_append_str( label_buf, ": " );
            utf8stringbuf_append_str( label_buf, data_feature_get_value_ptr( the_feature ) );

            pango_layout_set_font_description (layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (layout, utf8stringbuf_get_string( label_buf ), -1);

            /* draw text */
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
            cairo_move_to ( cr, left, top );
            pango_cairo_show_layout (cr, layout);
        }

        if ( mark_selected )
        {
            pencil_marker_mark_selected_rectangle( &((*this_).marker), *feature_bounds, cr );
        }

        if ( mark_focused )
        {
            pencil_marker_mark_focused_rectangle( &((*this_).marker), *feature_bounds, cr );
        }
    }
    else
    {
        TSLOG_ERROR("invalid visible feature in array!");
    }

    TRACE_END();
}


/*
Copyright 2017-2017 Andreas Warnke

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
