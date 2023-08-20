/* File: pencil_feature_painter.c; Copyright and License: see below */

#include "pencil_feature_painter.h"
#include "pencil_layout_data.h"
#include "u8/u8_trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;
const static double SINE_OF_45_DEGREE = 0.707106781187;

void pencil_feature_painter_init( pencil_feature_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    draw_feature_label_init( &((*this_).draw_feature_label) );

    U8_TRACE_END();
}

void pencil_feature_painter_destroy( pencil_feature_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_feature_label_destroy( &((*this_).draw_feature_label) );
    pencil_marker_destroy( &((*this_).marker) );

    U8_TRACE_END();
}

void pencil_feature_painter_draw( pencil_feature_painter_t *this_,
                                  const layout_feature_t *layouted_feature,
                                  bool mark_focused,
                                  bool mark_highlighted,
                                  bool mark_selected,
                                  bool gray_out,
                                  const data_profile_part_t *profile,
                                  const pencil_size_t *pencil_size,
                                  PangoLayout *layout,
                                  cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != profile );
    assert( NULL != pencil_size );
    assert( NULL != layouted_feature );
    assert( NULL != layout );
    assert( NULL != cr );

    const data_feature_t *the_feature = layout_feature_get_data_const( layouted_feature );
    const geometry_rectangle_t *const feature_symbol_box = layout_feature_get_symbol_box_const( layouted_feature );

    if ( data_feature_is_valid( the_feature ) )
    {
        U8_TRACE_INFO_INT("drawing feature id", data_feature_get_row_id( the_feature ) );

        /* select color */
        GdkRGBA foreground_color;
        {
            if ( mark_highlighted )
            {
                foreground_color = pencil_size_get_highlight_color( pencil_size );
            }
            else if ( gray_out )
            {
                foreground_color = pencil_size_get_gray_out_color( pencil_size );
            }
            else
            {
                foreground_color = pencil_size_get_standard_color( pencil_size );
            }
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        switch ( data_feature_get_main_type (the_feature) )
        {
            case DATA_FEATURE_TYPE_PORT:  /* or */
            case DATA_FEATURE_TYPE_IN_PORT_PIN:  /* or */
            case DATA_FEATURE_TYPE_OUT_PORT_PIN:
            {
                pencil_feature_painter_private_draw_port_pin_icon( this_, layouted_feature, pencil_size, foreground_color, cr );
            }
            break;

            case DATA_FEATURE_TYPE_ENTRY:  /* or */
            case DATA_FEATURE_TYPE_EXIT:
            {
                pencil_feature_painter_private_draw_entry_exit_icon( this_, layouted_feature, pencil_size, foreground_color, cr );
            }
            break;

            case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:  /* or */
            case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
            {
                pencil_feature_painter_private_draw_interface_icon( this_, layouted_feature, pencil_size, cr );
            }
            break;

            case DATA_FEATURE_TYPE_LIFELINE:
            {
                pencil_feature_painter_private_draw_lifeline_icon( this_, layouted_feature, mark_highlighted, pencil_size, cr );
            }
            break;

            case DATA_FEATURE_TYPE_PROPERTY: /* or */
            case DATA_FEATURE_TYPE_OPERATION: /* or */
            case DATA_FEATURE_TYPE_TAGGED_VALUE:
            {
                /* no icon */
            }
            break;

            default:
            {
                U8_LOG_ANOMALY("unknown feature type in pencil_feature_painter_draw");
                /* this may happen if a new database file has been read by an old program version */
                /* no icon */
            }
            break;
        }

        /* draw the label */
        draw_feature_label_draw_key_and_value( &((*this_).draw_feature_label),
                                               layout_feature_get_data_const( layouted_feature ),
                                               profile,
                                               &foreground_color,
                                               layout_feature_get_label_box_const( layouted_feature ),
                                               pencil_size,
                                               layout,
                                               cr
                                             );

#ifdef PENCIL_LAYOUT_DATA_DRAW_FOR_DEBUG
        /* draw the rectangles */
        {
            const geometry_rectangle_t *const feature_label_box
                = layout_feature_get_label_box_const ( layouted_feature );

            cairo_set_source_rgba( cr, 0.5, 0.7, 1.0, 0.5 );
            cairo_rectangle( cr,
                             geometry_rectangle_get_left ( feature_symbol_box ),
                             geometry_rectangle_get_top ( feature_symbol_box ),
                             geometry_rectangle_get_width ( feature_symbol_box ),
                             geometry_rectangle_get_height ( feature_symbol_box )
                           );
            cairo_rectangle( cr,
                             geometry_rectangle_get_left ( feature_label_box ),
                             geometry_rectangle_get_top ( feature_label_box ),
                             geometry_rectangle_get_width ( feature_label_box ),
                             geometry_rectangle_get_height ( feature_label_box )
                           );
            cairo_stroke( cr );
        }
#endif

        if ( mark_selected )
        {
            pencil_marker_mark_selected_rectangle( &((*this_).marker), *feature_symbol_box, cr );
        }

        if ( mark_focused )
        {
            pencil_marker_mark_focused_rectangle( &((*this_).marker), *feature_symbol_box, cr );
        }
    }
    else
    {
        U8_LOG_ERROR("invalid visible feature in array!");
    }

    U8_TRACE_END();
}

void pencil_feature_painter_private_draw_lifeline_icon ( pencil_feature_painter_t *this_,
                                                         const layout_feature_t *layouted_feature,
                                                         bool marked,
                                                         const pencil_size_t *pencil_size,
                                                         cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_feature );
    assert( NULL != cr );

    const geometry_rectangle_t *const feature_symbol_box = layout_feature_get_symbol_box_const( layouted_feature );

    const double left = geometry_rectangle_get_left ( feature_symbol_box );
    const double top = geometry_rectangle_get_top ( feature_symbol_box );
    const double width = geometry_rectangle_get_width ( feature_symbol_box );
    const double height = geometry_rectangle_get_height ( feature_symbol_box );

    double dashes[2];
    dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
    dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
    cairo_set_dash ( cr, dashes, 2, 0.0 );

    if ( GEOMETRY_DIRECTION_RIGHT == layout_feature_get_icon_direction( layouted_feature ) )
    {
        /* lineline in timing diagrams */
        const double center_y = geometry_rectangle_get_center_y ( feature_symbol_box );

        cairo_move_to ( cr, left, center_y );
        cairo_line_to ( cr, left + width, center_y );
        cairo_stroke (cr);
    }
    else if ( GEOMETRY_DIRECTION_DOWN == layout_feature_get_icon_direction( layouted_feature ) )
    {
        /* lifeline in sequence diagrams */
        const double center_x = geometry_rectangle_get_center_x ( feature_symbol_box );

        cairo_move_to ( cr, center_x, top );
        cairo_line_to ( cr, center_x, top + height );
        cairo_stroke (cr);
    }
    else
    {
        /* lifeline in communication diagrams, only drawn if highlighted: */
        if ( marked )
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

    U8_TRACE_END();
}

void pencil_feature_painter_private_draw_port_pin_icon ( pencil_feature_painter_t *this_,
                                                         const layout_feature_t *layouted_feature,
                                                         const pencil_size_t *pencil_size,
                                                         GdkRGBA foreground_color,
                                                         cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_feature );
    assert( NULL != cr );

    const geometry_rectangle_t *const symbol_box_bounds = layout_feature_get_symbol_box_const( layouted_feature );

    const double left = geometry_rectangle_get_left ( symbol_box_bounds );
    const double top = geometry_rectangle_get_top ( symbol_box_bounds );
    const double width = geometry_rectangle_get_width ( symbol_box_bounds );
    const double height = geometry_rectangle_get_height ( symbol_box_bounds );

    cairo_rectangle ( cr, left, top, width, height );

    /* Note: It is possible to read out the current color and set it again */
    /* but the interface for that looks like this might result in 1 additional memory allocation */
    /* which shall be avoided */
    /* cairo_pattern_t *const defined_color = cairo_get_source( cr ); */
    /* cairo_pattern_reference( defined_color );                      */
    /* ...                                                            */
    /* cairo_set_source( cr, defined_color );                         */
    /* cairo_pattern_destroy( defined_color );                        */

    cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );  /* white background */
    cairo_fill_preserve (cr);
    cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
    cairo_stroke (cr);

    /* draw the arrow */
    const double center_x = geometry_rectangle_get_center_x ( symbol_box_bounds );
    const double center_y = geometry_rectangle_get_center_y ( symbol_box_bounds );
    const double h_arrow_left = left + 0.25*width;
    const double h_arrow_right = left + 0.75*width;
    const double h_arrow_top = top + 0.25*height;
    const double h_arrow_bottom = top + 0.75*height;
    const double v_arrow_left = left + 0.25*width;
    const double v_arrow_right = left + 0.75*width;
    const double v_arrow_top = top + 0.25*height;
    const double v_arrow_bottom = top + 0.75*height;
    switch ( layout_feature_get_icon_direction( layouted_feature ) )
    {
        case GEOMETRY_DIRECTION_LEFT:
        {
            cairo_move_to ( cr, h_arrow_left, center_y );
            cairo_line_to ( cr, h_arrow_right, center_y );
            cairo_move_to ( cr, h_arrow_right, h_arrow_top );
            cairo_line_to ( cr, h_arrow_left, center_y );
            cairo_line_to ( cr, h_arrow_right, h_arrow_bottom );

            cairo_stroke (cr);
        }
        break;

        case GEOMETRY_DIRECTION_UP:
        {
            cairo_move_to ( cr, center_x, v_arrow_top );
            cairo_line_to ( cr, center_x, v_arrow_bottom );
            cairo_move_to ( cr, v_arrow_left, v_arrow_bottom );
            cairo_line_to ( cr, center_x, v_arrow_top );
            cairo_line_to ( cr, v_arrow_right, v_arrow_bottom );

            cairo_stroke (cr);
        }
        break;

        case GEOMETRY_DIRECTION_RIGHT:
        {
            cairo_move_to ( cr, h_arrow_right, center_y );
            cairo_line_to ( cr, h_arrow_left, center_y );
            cairo_move_to ( cr, h_arrow_left, h_arrow_top );
            cairo_line_to ( cr, h_arrow_right, center_y );
            cairo_line_to ( cr, h_arrow_left, h_arrow_bottom );

            cairo_stroke (cr);
        }
        break;

        case GEOMETRY_DIRECTION_DOWN:
        {
            cairo_move_to ( cr, center_x, v_arrow_bottom );
            cairo_line_to ( cr, center_x, v_arrow_top );
            cairo_move_to ( cr, v_arrow_left, v_arrow_top );
            cairo_line_to ( cr, center_x, v_arrow_bottom );
            cairo_line_to ( cr, v_arrow_right, v_arrow_top );

            cairo_stroke (cr);
        }
        break;

        case GEOMETRY_DIRECTION_CENTER:
        {
            /* no arrow */
        }
        break;

        default:
        {
            U8_LOG_ERROR( "unexpected value in geometry_direction_t." );
        }
        break;
    }

    U8_TRACE_END();
}

void pencil_feature_painter_private_draw_entry_exit_icon ( pencil_feature_painter_t *this_,
                                                           const layout_feature_t *layouted_feature,
                                                           const pencil_size_t *pencil_size,
                                                           GdkRGBA foreground_color,
                                                           cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_feature );
    assert( NULL != cr );

    const data_feature_t *the_feature = layout_feature_get_data_const( layouted_feature );
    const geometry_rectangle_t *const symbol_box_bounds = layout_feature_get_symbol_box_const( layouted_feature );

    const double left = geometry_rectangle_get_left ( symbol_box_bounds );
    const double top = geometry_rectangle_get_top ( symbol_box_bounds );
    const double center_x = geometry_rectangle_get_center_x( symbol_box_bounds );
    const double center_y = geometry_rectangle_get_center_y( symbol_box_bounds );
    const double circle_x_radius = center_x - left;
    const double circle_y_radius = center_y - top;
    const double bottom = geometry_rectangle_get_bottom( symbol_box_bounds );
    const double right = geometry_rectangle_get_right( symbol_box_bounds );
    const double ctrl_x_offset = circle_x_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
    const double ctrl_y_offset = circle_y_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

    cairo_move_to ( cr, center_x, bottom );
    cairo_curve_to ( cr, left + ctrl_x_offset, bottom, left, bottom - ctrl_y_offset, left /* end point x */, center_y /* end point y */ );
    cairo_curve_to ( cr, left, top + ctrl_y_offset, left + ctrl_x_offset, top, center_x /* end point x */, top /* end point y */ );
    cairo_curve_to ( cr, right - ctrl_x_offset, top, right, top + ctrl_y_offset, right /* end point x */, center_y /* end point y */ );
    cairo_curve_to ( cr, right, bottom - ctrl_y_offset, right - ctrl_x_offset, bottom, center_x /* end point x */, bottom /* end point y */ );

    cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );  /* white background */
    cairo_fill_preserve (cr);
    cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
    cairo_stroke (cr);

    /* draw X of exit icon */
    if ( data_feature_get_main_type( the_feature ) == DATA_FEATURE_TYPE_EXIT )
    {
        const double half_width = geometry_rectangle_get_width ( symbol_box_bounds )/2.0;
        const double half_height = geometry_rectangle_get_height ( symbol_box_bounds )/2.0;
        const double cross_end_dx = half_width * SINE_OF_45_DEGREE;
        const double cross_end_dy = half_height * SINE_OF_45_DEGREE;

        cairo_move_to ( cr, center_x + cross_end_dx, center_y - cross_end_dy );
        cairo_line_to ( cr, center_x - cross_end_dx, center_y + cross_end_dy );
        cairo_move_to ( cr, center_x - cross_end_dx, center_y - cross_end_dy );
        cairo_line_to ( cr, center_x + cross_end_dx, center_y + cross_end_dy );

        cairo_stroke (cr);
    }

    U8_TRACE_END();
}

void pencil_feature_painter_private_draw_interface_icon ( pencil_feature_painter_t *this_,
                                                          const layout_feature_t *layouted_feature,
                                                          const pencil_size_t *pencil_size,
                                                          cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_feature );
    assert( NULL != cr );

    const geometry_rectangle_t *const symbol_box_bounds = layout_feature_get_symbol_box_const( layouted_feature );

    const double left = geometry_rectangle_get_left ( symbol_box_bounds );
    const double top = geometry_rectangle_get_top ( symbol_box_bounds );
    const double width = geometry_rectangle_get_width ( symbol_box_bounds );
    const double height = geometry_rectangle_get_height ( symbol_box_bounds );

    double bottom = top + height;
    double right = left + width;
    double half_width = 0.5 * width;
    double half_height = 0.5 * height;
    double center_x = left + half_width;
    double center_y = top + half_height;
    double ctrl_xoffset = half_width * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
    double ctrl_yoffset = half_height * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

    switch ( layout_feature_get_icon_direction( layouted_feature ) )
    {
        case GEOMETRY_DIRECTION_LEFT:
        {
            cairo_move_to ( cr, center_x, top );
            cairo_curve_to ( cr, right - ctrl_xoffset, top, right, top + ctrl_yoffset, right /* end point x */, center_y /* end point y */ );
            cairo_curve_to ( cr, right, bottom - ctrl_yoffset, right - ctrl_xoffset, bottom, center_x /* end point x */, bottom /* end point y */ );
            cairo_stroke (cr);
        }
        break;

        case GEOMETRY_DIRECTION_UP:
        {
            cairo_move_to ( cr, right, center_y );
            cairo_curve_to ( cr, right, bottom - ctrl_yoffset, right - ctrl_xoffset, bottom, center_x /* end point x */, bottom /* end point y */ );
            cairo_curve_to ( cr, left + ctrl_xoffset, bottom, left, bottom - ctrl_yoffset, left /* end point x */, center_y /* end point y */ );
            cairo_stroke (cr);
        }
        break;

        case GEOMETRY_DIRECTION_RIGHT:
        {
            cairo_move_to ( cr, center_x, bottom );
            cairo_curve_to ( cr, left + ctrl_xoffset, bottom, left, bottom - ctrl_yoffset, left /* end point x */, center_y /* end point y */ );
            cairo_curve_to ( cr, left, top + ctrl_yoffset, left + ctrl_xoffset, top, center_x /* end point x */, top /* end point y */ );
            cairo_stroke (cr);
        }
        break;

        case GEOMETRY_DIRECTION_DOWN:
        {
            cairo_move_to ( cr, left, center_y );
            cairo_curve_to ( cr, left, top + ctrl_yoffset, left + ctrl_xoffset, top, center_x /* end point x */, top /* end point y */ );
            cairo_curve_to ( cr, right - ctrl_xoffset, top, right, top + ctrl_yoffset, right /* end point x */, center_y /* end point y */ );
            cairo_stroke (cr);
        }
        break;

        case GEOMETRY_DIRECTION_CENTER:
        {
            cairo_move_to ( cr, center_x, bottom );
            cairo_curve_to ( cr, left + ctrl_xoffset, bottom, left, bottom - ctrl_yoffset, left /* end point x */, center_y /* end point y */ );
            cairo_curve_to ( cr, left, top + ctrl_yoffset, left + ctrl_xoffset, top, center_x /* end point x */, top /* end point y */ );
            cairo_curve_to ( cr, right - ctrl_xoffset, top, right, top + ctrl_yoffset, right /* end point x */, center_y /* end point y */ );
            cairo_curve_to ( cr, right, bottom - ctrl_yoffset, right - ctrl_xoffset, bottom, center_x /* end point x */, bottom /* end point y */ );
            cairo_stroke (cr);
        }
        break;

        default:
        {
            U8_LOG_ERROR( "unexpected value in geometry_direction_t." );
        }
        break;
    }

    U8_TRACE_END();
}

void pencil_feature_painter_get_minimum_bounds ( pencil_feature_painter_t *this_,
                                                 const data_feature_t *the_feature,
                                                 const data_profile_part_t *profile,
                                                 const pencil_size_t *pencil_size,
                                                 PangoLayout *font_layout,
                                                 geometry_dimensions_t *out_feature_bounds )
{
    U8_TRACE_BEGIN();
    assert( NULL != the_feature );
    assert( NULL != profile );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_feature_bounds );

    const geometry_dimensions_t label_dim_proposal = {
        .width = 25.0 * pencil_size_get_standard_font_size( pencil_size ),
        .height = pencil_size_get_standard_font_size( pencil_size )
    };
    draw_feature_label_get_key_and_value_dimensions( &((*this_).draw_feature_label),
                                                     the_feature,
                                                     profile,
                                                     &label_dim_proposal,
                                                     pencil_size,
                                                     font_layout,
                                                     out_feature_bounds
                                                   );

    U8_TRACE_END();
}


/*
Copyright 2017-2023 Andreas Warnke

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
