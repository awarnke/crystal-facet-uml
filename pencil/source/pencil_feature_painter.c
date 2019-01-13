/* File: pencil_feature_painter.c; Copyright and License: see below */

#include "pencil_feature_painter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;

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
                                   layout_feature_t *layouted_feature,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   bool grey_out,
                                   pencil_size_t *pencil_size,
                                   PangoLayout *layout,
                                   cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_feature );
    assert( NULL != layout );
    assert( NULL != cr );

    const data_feature_t *the_feature = layout_feature_get_data_ptr( layouted_feature );
    const geometry_rectangle_t *feature_bounds = layout_feature_get_bounds_ptr( layouted_feature );

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

        /* select color */
        GdkRGBA foreground_color;
        {
            if ( mark_highlighted )
            {
                foreground_color = pencil_size_get_highlight_color( pencil_size );
            }
            else if ( grey_out )
            {
                foreground_color = pencil_size_get_gray_out_color( pencil_size );
            }
            else
            {
                foreground_color = pencil_size_get_standard_color( pencil_size );
            }
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        /* draw rectangle of ports */
        if ( DATA_FEATURE_TYPE_PORT == data_feature_get_main_type (the_feature) )
        {
            double port_icon_size;
            port_icon_size = pencil_size_get_standard_font_size( pencil_size );
            double port_icon_gap;
            port_icon_gap = 0.5 * gap;

            double box_left;
            double box_top;
            double box_height;
            double box_width;

            box_top = top + port_icon_gap + 0.5 * ( height - port_icon_size );
            box_height = port_icon_size - 2.0 * port_icon_gap;
            box_width = port_icon_size - 2.0 * port_icon_gap;

            if ( PENCIL_LAYOUT_DIRECTION_RIGHT == layout_feature_get_direction( layouted_feature ) )
            {
                /* box to left, text to right */
                box_left = left + port_icon_gap;

                left += port_icon_size + gap;
                width -= port_icon_size + gap;
            }
            else
            {
                /* box to right, text to left */
                box_left = left + width - port_icon_size + port_icon_gap;

                width -= port_icon_size + gap;
            }

            cairo_rectangle ( cr, box_left, box_top, box_width, box_height );

            cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );  /* white background */
            cairo_fill_preserve (cr);
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
            cairo_stroke (cr);
        }

        /* draw circle of interfaces */
        if (( DATA_FEATURE_TYPE_PROVIDED_INTERFACE == data_feature_get_main_type (the_feature) )
            || ( DATA_FEATURE_TYPE_REQUIRED_INTERFACE == data_feature_get_main_type (the_feature) ))
        {
            double bottom = top + height;
            double right = left + width;
            double half_width = 0.5 * width;
            double half_height = 0.5 * height;
            double center_x = left + half_width;
            double center_y = top + half_height;
            double ctrl_xoffset = half_width * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
            double ctrl_yoffset = half_height * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

            cairo_move_to ( cr, center_x, bottom );
            cairo_curve_to ( cr, left + ctrl_xoffset, bottom, left, bottom - ctrl_yoffset, left /* end point x */, center_y /* end point y */ );
            cairo_curve_to ( cr, left, top + ctrl_yoffset, left + ctrl_xoffset, top, center_x /* end point x */, top /* end point y */ );
            cairo_curve_to ( cr, right - ctrl_xoffset, top, right, top + ctrl_yoffset, right /* end point x */, center_y /* end point y */ );
            cairo_curve_to ( cr, right, bottom - ctrl_yoffset, right - ctrl_xoffset, bottom, center_x /* end point x */, bottom /* end point y */ );
            cairo_stroke (cr);
        }

        /* draw dotted line of lifelines */
        if ( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type (the_feature) )
        {
            double dashes[2];
            dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
            dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
            cairo_set_dash ( cr, dashes, 2, 0.0 );

            if ( PENCIL_LAYOUT_DIRECTION_RIGHT == layout_feature_get_direction( layouted_feature ) )
            {
                /* lineline in timing diagrams */
                double y_center = geometry_rectangle_get_y_center ( feature_bounds );

                cairo_move_to ( cr, left, y_center );
                cairo_line_to ( cr, left + width, y_center );
                cairo_stroke (cr);
            }
            else if ( PENCIL_LAYOUT_DIRECTION_DOWN == layout_feature_get_direction( layouted_feature ) )
            {
                /* lifeline in sequence diagrams */
                double x_center = geometry_rectangle_get_x_center ( feature_bounds );

                cairo_move_to ( cr, x_center, top );
                cairo_line_to ( cr, x_center, top + height );
                cairo_stroke (cr);
            }
            else
            {
                /* lifeline in communication diagrams, only drawn if highlighted: */
                if ( mark_highlighted )
                {
                    cairo_move_to ( cr, left, top );
                    cairo_line_to ( cr, left, top + height );
                    cairo_line_to ( cr, left + width, top + height );
                    cairo_line_to ( cr, left + width, top );
                    cairo_line_to ( cr, left, top );
                    cairo_stroke (cr);
                }
            }

            cairo_set_dash ( cr, NULL, 0, 0.0 );
        }

        /* draw text - except for lifelines */
        if ( DATA_FEATURE_TYPE_LIFELINE != data_feature_get_main_type (the_feature) )
        {

            /* prepare text */
            char label_text[DATA_FEATURE_MAX_KEY_SIZE + DATA_FEATURE_MAX_VALUE_SIZE + 2 ];
            utf8stringbuf_t label_buf = UTF8STRINGBUF(label_text);
            utf8stringbuf_copy_str( label_buf, data_feature_get_key_ptr( the_feature ) );
            if ( data_feature_has_value( the_feature ) )
            {
                utf8stringbuf_append_str( label_buf, ": " );
                utf8stringbuf_append_str( label_buf, data_feature_get_value_ptr( the_feature ) );
            }

            pango_layout_set_font_description (layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (layout, utf8stringbuf_get_string( label_buf ), -1);

            /* precalculate text dimensions to vertically center the text */
            int text2_width;
            int text2_height;
            pango_layout_get_pixel_size (layout, &text2_width, &text2_height);
            double y_adjust = ( height - text2_height ) / 2.0;

            /* draw text */
            cairo_move_to ( cr, left, top + y_adjust );
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

void pencil_feature_painter_get_minimum_bounds ( pencil_feature_painter_t *this_,
                                                 const data_feature_t *the_feature,
                                                 const pencil_size_t *pencil_size,
                                                 PangoLayout *font_layout,
                                                 geometry_rectangle_t *out_feature_bounds )
{
    TRACE_BEGIN();
    assert( NULL != the_feature );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_feature_bounds );

    double left = 0.0;
    double top = 0.0;
    double width = 0.0;
    double height = 0.0;

    if ( data_feature_is_valid( the_feature ) )
    {
        TRACE_INFO_INT("calculating minimum bounds of feature id", data_feature_get_id( the_feature ) );

        /* layout text */
        int text2_width;
        int text2_height;
        {
            /* prepare text */
            char label_text[DATA_FEATURE_MAX_KEY_SIZE + DATA_FEATURE_MAX_VALUE_SIZE + 2 ];
            utf8stringbuf_t label_buf = UTF8STRINGBUF(label_text);
            utf8stringbuf_copy_str( label_buf, data_feature_get_key_ptr( the_feature ) );
            if ( data_feature_has_value( the_feature ) )
            {
                utf8stringbuf_append_str( label_buf, ": " );
                utf8stringbuf_append_str( label_buf, data_feature_get_value_ptr( the_feature ) );
            }

            /* determine text width and height */
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (font_layout, utf8stringbuf_get_string( label_buf ), -1);
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);
        }

        /* for the height, ignore the actual height (text2_height) and return the standard font height + line gap */
        double lineheight;
        lineheight = pencil_size_get_standard_font_size( pencil_size )
                     + pencil_size_get_font_line_gap( pencil_size );

        height += lineheight;
        /*height += text2_height;*/
        width += text2_width;
    }
    else
    {
        TSLOG_ERROR("invalid feature in array!");
    }

    geometry_rectangle_reinit( out_feature_bounds, left, top, width, height );
    TRACE_END();
}


/*
Copyright 2017-2019 Andreas Warnke

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
