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

void draw_symbol_draw_component_icon ( const draw_symbol_t *this_,
                                       double x,
                                       double y,
                                       geometry_h_align_t h_align,
                                       geometry_v_align_t v_align,
                                       double height,
                                       cairo_t *cr,
                                       double *out_width )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != out_width );

    /* calculate component bounds */
    double comp_left;
    double comp_right;
    double comp_top;
    double comp_bottom;
    double comp_height;
    double comp_width;
    double port_half_width;

    comp_height = height;
    comp_width = comp_height * 1.2;
    port_half_width = height * 0.2;

    switch ( h_align )
    {
        case GEOMETRY_H_ALIGN_LEFT:
        {
            comp_left = x + port_half_width;
        }
        break;

        case GEOMETRY_H_ALIGN_CENTER:
        {
            comp_left = x + 0.5 * ( port_half_width - comp_width );
        }
        break;

        case GEOMETRY_H_ALIGN_RIGHT:
        {
            comp_left = x - comp_width;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_h_align_t in draw_symbol_private_draw_component_icon()");
            assert(0);
            comp_left = x;
        }
        break;
    }
    comp_right = comp_left + comp_width;

    switch ( v_align )
    {
        case GEOMETRY_V_ALIGN_TOP:
        {
            comp_top = y;
        }
        break;

        case GEOMETRY_V_ALIGN_CENTER:
        {
            comp_top = y - 0.5 * comp_height;
        }
        break;

        case GEOMETRY_V_ALIGN_BOTTOM:
        {
            comp_top = y - comp_height;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_v_align_t in draw_symbol_private_draw_component_icon()");
            assert(0);
            comp_top = y;
        }
        break;
    }
    comp_bottom = comp_top + comp_height;

    /* calculate bounds of the two ports */
    double port1_top;
    double port2_top;
    double port_left;
    double port_width;
    double port_height;

    port_left = comp_left - port_half_width;
    port_width = 2.0*port_half_width;
    port_height = 0.2*height;
    port1_top = comp_top + 0.2*height;
    port2_top = comp_top + 0.6*height;

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

    /* return the result */
    *out_width = comp_width + port_half_width;

    TRACE_END();
}

void draw_symbol_draw_artifact_icon ( const draw_symbol_t *this_,
                                      double x,
                                      double y,
                                      geometry_h_align_t h_align,
                                      geometry_v_align_t v_align,
                                      double height,
                                      cairo_t *cr,
                                      double *out_width )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != out_width );

    /* calculate artifact bounds */
    double art_left;
    double art_right;
    double art_top;
    double art_bottom;
    double art_height;
    double art_width;
    double art_corner_edge;

    art_height = height;
    art_width = art_height * 0.7;
    art_corner_edge = height * 0.3;

    switch ( h_align )
    {
        case GEOMETRY_H_ALIGN_LEFT:
        {
            art_left = x;
        }
        break;

        case GEOMETRY_H_ALIGN_CENTER:
        {
            art_left = x - 0.5 * art_width;
        }
        break;

        case GEOMETRY_H_ALIGN_RIGHT:
        {
            art_left = x - art_width;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_h_align_t in draw_symbol_private_draw_component_icon()");
            assert(0);
            art_left = x;
        }
        break;
    }
    art_right = art_left + art_width;

    switch ( v_align )
    {
        case GEOMETRY_V_ALIGN_TOP:
        {
            art_top = y;
        }
        break;

        case GEOMETRY_V_ALIGN_CENTER:
        {
            art_top = y - 0.5 * art_height;
        }
        break;

        case GEOMETRY_V_ALIGN_BOTTOM:
        {
            art_top = y - art_height;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_v_align_t in draw_symbol_private_draw_component_icon()");
            assert(0);
            art_top = y;
        }
        break;
    }
    art_bottom = art_top + art_height;

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

    /* return the result */
    *out_width = art_width;

    TRACE_END();
}

void draw_symbol_draw_actor_icon ( const draw_symbol_t *this_,
                                   double x,
                                   double y,
                                   geometry_h_align_t h_align,
                                   geometry_v_align_t v_align,
                                   double height,
                                   cairo_t *cr,
                                   double *out_width )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != out_width );

    /* calculate actor bounds */
    double act_left;
    double act_right;
    double act_top;
    double act_bottom;
    double act_height;
    double act_width;

    act_height = height;
    act_width = height/3.0;
    act_left = geometry_h_align_get_left( &h_align, act_width, x, 0.0 );
    act_top = geometry_v_align_get_top( &v_align, act_height, y, 0.0 );
    act_right = act_left + act_width;
    act_bottom = act_top + act_height;

    /* draw the icon */
    double radius = act_width/2.0;
    double ctrl_offset = radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
    double head_bottom = act_top + act_height/3.0;
    double leg_top = act_top + act_height*0.6;
    double arm_top = act_top + act_height*0.45;

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

    /* return the result */
    *out_width = act_width;

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
