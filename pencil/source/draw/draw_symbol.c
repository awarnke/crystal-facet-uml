/* File: draw_symbol.c; Copyright and License: see below */

#include "draw/draw_symbol.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;

void draw_symbol_draw_component ( const draw_symbol_t *this_,
                                  geometry_rectangle_t bounds,
                                  cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    /* calculate component bounds */
    const double comp_right = geometry_rectangle_get_right( &bounds );
    const double comp_top = geometry_rectangle_get_top( &bounds );
    const double comp_bottom = geometry_rectangle_get_bottom( &bounds );
    const double comp_height = geometry_rectangle_get_height( &bounds );
    const double port_half_width = comp_height * 0.2;
    const double comp_left = geometry_rectangle_get_left( &bounds ) + port_half_width;
    //const double comp_width = geometry_rectangle_get_width( &bounds ) - port_half_width;

    /* calculate bounds of the two ports */
    const double port_left = comp_left - port_half_width;
    const double port_width = 2.0*port_half_width;
    const double port_height = 0.2*comp_height;
    const double port1_top = comp_top + 0.2*comp_height;
    const double port2_top = comp_top + 0.6*comp_height;

    /* draw the icon */
    cairo_move_to ( cr, comp_left, port1_top );
    cairo_line_to ( cr, comp_left, comp_top );
    cairo_line_to ( cr, comp_right, comp_top );
    cairo_line_to ( cr, comp_right, comp_bottom );
    cairo_line_to ( cr, comp_left, comp_bottom );
    cairo_line_to ( cr, comp_left, port2_top + port_height );
    cairo_stroke (cr);
    cairo_move_to ( cr, comp_left, port2_top );
    cairo_line_to ( cr, comp_left, port1_top + port_height );
    cairo_stroke (cr);
    cairo_rectangle ( cr, port_left, port1_top, port_width, port_height );
    cairo_stroke (cr);
    cairo_rectangle ( cr, port_left, port2_top, port_width, port_height );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_symbol_draw_artifact ( const draw_symbol_t *this_,
                                 geometry_rectangle_t bounds,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    /* calculate artifact bounds */
    const double art_left = geometry_rectangle_get_left( &bounds );
    const double art_right = geometry_rectangle_get_right( &bounds );
    const double art_top = geometry_rectangle_get_top( &bounds );
    const double art_bottom = geometry_rectangle_get_bottom( &bounds );
    const double art_height = geometry_rectangle_get_height( &bounds );
    //const double art_width = geometry_rectangle_get_width( &bounds );
    const double art_corner_edge = art_height * 0.3;

    /* draw the icon */
    cairo_move_to ( cr, art_right, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top );
    cairo_line_to ( cr, art_left, art_top );
    cairo_line_to ( cr, art_left, art_bottom );
    cairo_line_to ( cr, art_right, art_bottom );
    cairo_line_to ( cr, art_right, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_symbol_draw_actor ( const draw_symbol_t *this_,
                              geometry_rectangle_t bounds,
                              cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    /* calculate actor bounds */
    const double act_left = geometry_rectangle_get_left( &bounds );
    const double act_right = geometry_rectangle_get_right( &bounds );
    const double act_top = geometry_rectangle_get_top( &bounds );
    const double act_bottom = geometry_rectangle_get_bottom( &bounds );
    const double act_height = geometry_rectangle_get_height( &bounds );
    const double act_width = geometry_rectangle_get_width( &bounds );

    /* draw the icon */
    const double radius = act_width/2.0;
    const double ctrl_offset = radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
    const double head_bottom = act_top + act_height/3.0;
    const double leg_top = act_top + act_height*0.6;
    const double arm_top = act_top + act_height*0.45;

    cairo_move_to ( cr, act_left + radius, head_bottom );
    cairo_curve_to ( cr, act_left + ctrl_offset, head_bottom, act_left, head_bottom - ctrl_offset, act_left /* end point x */, act_top + radius /* end point y */ );
    cairo_curve_to ( cr, act_left, act_top + ctrl_offset, act_left + ctrl_offset, act_top, act_left + radius /* end point x */, act_top /* end point y */ );
    cairo_curve_to ( cr, act_right - ctrl_offset, act_top, act_right, act_top + ctrl_offset, act_right /* end point x */, act_top + radius /* end point y */ );
    cairo_curve_to ( cr, act_right, head_bottom - ctrl_offset, act_right - ctrl_offset, head_bottom, act_left + radius /* end point x */, head_bottom /* end point y */ );
    cairo_line_to ( cr, act_left + radius, leg_top );
    cairo_line_to ( cr, act_left, act_bottom );
    cairo_move_to ( cr, act_right, act_bottom );
    cairo_line_to ( cr, act_left + radius, leg_top );
    cairo_move_to ( cr, act_right, arm_top );
    cairo_line_to ( cr, act_left, arm_top );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_symbol_draw_circle ( const draw_symbol_t *this_,
                               geometry_rectangle_t bounds,
                               const pencil_size_t *pencil_size,
                               bool stroke,
                               bool fill,
                               bool shallow_history,
                               bool deep_history,
                               cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != pencil_size );
    assert ( NULL != cr );

    const double circle_top = geometry_rectangle_get_top( &bounds );
    const double circle_bottom = geometry_rectangle_get_bottom( &bounds );
    const double center_x = geometry_rectangle_get_x_center( &bounds );
    const double center_y = geometry_rectangle_get_y_center( &bounds );
    const double circle_left = geometry_rectangle_get_left( &bounds );
    const double circle_right = geometry_rectangle_get_right( &bounds );
    const double circle_width = geometry_rectangle_get_width( &bounds );
    const double circle_height = geometry_rectangle_get_height( &bounds );
    const double circle_x_radius = center_x - circle_left;
    const double circle_y_radius = center_y - circle_top;

    const double gap = pencil_size_get_standard_object_border( pencil_size );

    if ( stroke )
    {
        const double ctrl_x_offset = circle_x_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
        const double ctrl_y_offset = circle_y_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

        cairo_move_to ( cr, center_x, circle_bottom );
        cairo_curve_to ( cr, circle_left + ctrl_x_offset, circle_bottom, circle_left, circle_bottom - ctrl_y_offset, circle_left /* end point x */, center_y /* end point y */ );
        cairo_curve_to ( cr, circle_left, circle_top + ctrl_y_offset, circle_left + ctrl_x_offset, circle_top, center_x /* end point x */, circle_top /* end point y */ );
        cairo_curve_to ( cr, circle_right - ctrl_x_offset, circle_top, circle_right, circle_top + ctrl_y_offset, circle_right /* end point x */, center_y /* end point y */ );
        cairo_curve_to ( cr, circle_right, circle_bottom - ctrl_y_offset, circle_right - ctrl_x_offset, circle_bottom, center_x /* end point x */, circle_bottom /* end point y */ );
        cairo_stroke (cr);
    }

    if ( fill )
    {
        const double circle2_x_radius = circle_x_radius - gap;
        const double circle2_y_radius = circle_y_radius - gap;
        const double circle2_top = circle_top + gap;
        const double circle2_bottom = circle_bottom - gap;
        const double circle2_left = circle_left + gap;
        const double circle2_right = circle_right - gap;
        const double ctrl2_x_offset = circle2_x_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
        const double ctrl2_y_offset = circle2_y_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

        /* draw a smaller filled circle */
        cairo_move_to ( cr, center_x, circle2_bottom );
        cairo_curve_to ( cr, circle2_left + ctrl2_x_offset, circle2_bottom, circle2_left, circle2_bottom - ctrl2_y_offset, circle2_left /* end point x */, center_y /* end point y */ );
        cairo_curve_to ( cr, circle2_left, circle2_top + ctrl2_y_offset, circle2_left + ctrl2_x_offset, circle2_top, center_x /* end point x */, circle2_top /* end point y */ );
        cairo_curve_to ( cr, circle2_right - ctrl2_x_offset, circle2_top, circle2_right, circle2_top + ctrl2_y_offset, circle2_right /* end point x */, center_y /* end point y */ );
        cairo_curve_to ( cr, circle2_right, circle2_bottom - ctrl2_y_offset, circle2_right - ctrl2_x_offset, circle2_bottom, center_x /* end point x */, circle2_bottom /* end point y */ );
        cairo_fill (cr);
    }

    if ( shallow_history )
    {
        const double quarter_x_font = 0.15 * circle_width;
        const double quarter_y_font = 0.15 * circle_height;
        cairo_move_to ( cr, center_x - quarter_x_font, center_y - 2.0 * quarter_y_font );
        cairo_line_to ( cr, center_x - quarter_x_font, center_y + 2.0 * quarter_y_font );
        cairo_move_to ( cr, center_x - quarter_x_font, center_y );
        cairo_line_to ( cr, center_x + quarter_x_font, center_y );
        cairo_move_to ( cr, center_x + quarter_x_font, center_y - 2.0 * quarter_y_font );
        cairo_line_to ( cr, center_x + quarter_x_font, center_y + 2.0 * quarter_y_font );
        cairo_stroke (cr);
    }
    else if ( deep_history )
    {
        const double quarter_x_font = 0.15 * circle_width;
        const double quarter_y_font = 0.15 * circle_height;
        cairo_move_to ( cr, center_x - 1.5 * quarter_x_font, center_y - 2.0 * quarter_y_font );
        cairo_line_to ( cr, center_x - 1.5 * quarter_x_font, center_y + 2.0 * quarter_y_font );
        cairo_move_to ( cr, center_x - 1.5 * quarter_x_font, center_y );
        cairo_line_to ( cr, center_x + 0.3 * quarter_x_font, center_y );
        cairo_move_to ( cr, center_x + 0.3 * quarter_x_font, center_y - 2.0 * quarter_y_font );
        cairo_line_to ( cr, center_x + 0.3 * quarter_x_font, center_y + 2.0 * quarter_y_font );
        cairo_stroke (cr);
        cairo_move_to ( cr, center_x + 1.5 * quarter_x_font, center_y - 2.0 * quarter_y_font );
        cairo_line_to ( cr, center_x + 1.5 * quarter_x_font, center_y );
        cairo_move_to ( cr, center_x + 0.8 * quarter_x_font, center_y - 1.6 * quarter_y_font );
        cairo_line_to ( cr, center_x + 2.2 * quarter_x_font, center_y - 0.4 * quarter_y_font );
        cairo_move_to ( cr, center_x + 2.2 * quarter_x_font, center_y - 1.6 * quarter_y_font );
        cairo_line_to ( cr, center_x + 0.8 * quarter_x_font, center_y - 0.4 * quarter_y_font );
        cairo_stroke (cr);
    }

    TRACE_END();
}

void draw_symbol_draw_time ( const draw_symbol_t *this_,
                             geometry_rectangle_t bounds,
                             cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    const double top = geometry_rectangle_get_top( &bounds );
    const double bottom = geometry_rectangle_get_bottom( &bounds );
    const double left = geometry_rectangle_get_left( &bounds );
    const double right = geometry_rectangle_get_right( &bounds );

    cairo_move_to ( cr, right, bottom );
    cairo_line_to ( cr, left, top );
    cairo_line_to ( cr, right, top );
    cairo_line_to ( cr, left, bottom );
    cairo_line_to ( cr, right, bottom );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_symbol_draw_sync ( const draw_symbol_t *this_,
                             geometry_rectangle_t bounds,
                             cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    const double left = geometry_rectangle_get_left( &bounds );
    const double top = geometry_rectangle_get_top( &bounds );
    const double width = geometry_rectangle_get_width( &bounds );
    const double height = geometry_rectangle_get_height( &bounds );

    cairo_rectangle ( cr, left, top, width, height );
    cairo_fill (cr);

    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke
    http://www.apache.org/licenses/LICENSE-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
