/* File: draw_symbol.c; Copyright and License: see below */

#include "draw/draw_symbol.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;

void draw_symbol_init( draw_symbol_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void draw_symbol_destroy( draw_symbol_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

geometry_rectangle_t draw_symbol_get_component_bounds ( const draw_symbol_t *this_,
                                                        double x,
                                                        double y,
                                                        geometry_h_align_t h_align,
                                                        geometry_v_align_t v_align,
                                                        double height
                                                      )
{
    TRACE_BEGIN();
    geometry_rectangle_t result;

    double width = 1.4 * height;
    geometry_rectangle_init ( &result, 
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ), 
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ), 
                              width,
                              height
                            );

    TRACE_END();
    return result;
}

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

geometry_rectangle_t draw_symbol_get_artifact_bounds ( const draw_symbol_t *this_,
                                                       double x,
                                                       double y,
                                                       geometry_h_align_t h_align,
                                                       geometry_v_align_t v_align,
                                                       double height
                                                     )
{
    TRACE_BEGIN();
    geometry_rectangle_t result;

    double width = 0.7 * height;
    geometry_rectangle_init ( &result, 
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ), 
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ), 
                              width,
                              height
                            );

    TRACE_END();
    return result;
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

geometry_rectangle_t draw_symbol_get_actor_bounds ( const draw_symbol_t *this_,
                                                    double x,
                                                    double y,
                                                    geometry_h_align_t h_align,
                                                    geometry_v_align_t v_align,
                                                    double height
                                                  )
{
    TRACE_BEGIN();
    geometry_rectangle_t result;

    double width = height/3.0;
    geometry_rectangle_init ( &result, 
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ), 
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ), 
                              width,
                              height
                            );

    TRACE_END();
    return result;
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
