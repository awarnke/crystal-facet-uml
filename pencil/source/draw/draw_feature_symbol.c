/* File: draw_feature_symbol.c; Copyright and License: see below */

#include "draw/draw_feature_symbol.h"
#include "u8/u8_trace.h"
#include "u8/u8_f64.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

void draw_feature_symbol_init( draw_feature_symbol_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).last_depth = 0;
    (*this_).last_location = INFINITY;

    U8_TRACE_END();
}

void draw_feature_symbol_reinit( draw_feature_symbol_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).last_depth = 0;
    (*this_).last_location = INFINITY;

    U8_TRACE_END();
}

void draw_feature_symbol_destroy( draw_feature_symbol_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_END();
}


void draw_feature_symbol_draw_execution_spec( draw_feature_symbol_t *this_,
                                              const geometry_rectangle_t *bounds,
                                              double to_y,
                                              uint32_t depth,
                                              const pencil_size_t *pencil_size,
                                              cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );

    const double x = geometry_rectangle_get_center_x ( bounds );
    double top = geometry_rectangle_get_top ( bounds );
    double bottom = geometry_rectangle_get_bottom ( bounds );
    const double gap = pencil_size_get_preferred_object_distance( pencil_size );
    const double half_gap = 0.5 * gap;

    /* DRAW THE NON EXECUTION SPEC LIFELINES (DASHED LINE) */
    {
        double dashes[2];
        dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
        dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
        cairo_set_dash ( cr, dashes, 2, 0.0 );
    }

    /* do not fill the top gap with an execution specification bar */
    if (( to_y == ( -INFINITY ) )&&( depth != 0 ))
    {
        const double start_exec_bar = ( (*this_).last_location > ( top + gap ) ) ? ( top + gap ) : u8_f64_max2( (*this_).last_location, top );

        cairo_move_to ( cr, x, start_exec_bar );
        cairo_line_to ( cr, x, top );
        cairo_stroke (cr);

        /* overwrite top value by start_exec_bar */
        top = start_exec_bar;
    }

    /* do not fill the bottom gap with an execution specification bar */
    if (( (*this_).last_location == ( INFINITY ) )&&( depth != 0 ))
    {
        const double end_exec_bar = ( to_y < ( bottom - gap ) ) ? ( bottom - gap ) : u8_f64_min2( to_y, bottom );

        cairo_move_to ( cr, x, bottom );
        cairo_line_to ( cr, x, end_exec_bar );
        cairo_stroke (cr);

        /* overwrite bottom value by end_exec_bar */
        bottom = end_exec_bar;
    }

    /* draw a normal, dashed lifeline */
    if ( depth == 0 )
    {
        cairo_move_to ( cr, x, u8_f64_min2( (*this_).last_location, bottom ) );
        cairo_line_to ( cr, x, u8_f64_max2( to_y, top ) );
        cairo_stroke (cr);
    }

    /* restore cr drawing context */
    {
        cairo_set_dash ( cr, NULL, 0, 0.0 );
    }

    /* DRAW THE EXECUTION SPECIFICATION */

    for ( int depth_run = 0; depth_run < depth; depth_run ++ )
    {
        cairo_move_to ( cr, x - half_gap + depth_run * half_gap, u8_f64_min2( (*this_).last_location, bottom ) );
        cairo_line_to ( cr, x - half_gap + depth_run * half_gap, u8_f64_max2( to_y, top ) );
        cairo_stroke (cr);
    }
    if ( depth > 0 )
    {
        cairo_move_to ( cr, x + half_gap * depth, u8_f64_min2( (*this_).last_location, bottom ) );
        cairo_line_to ( cr, x + half_gap * depth, u8_f64_max2( to_y, top ) );
        cairo_stroke (cr);
    }

    (*this_).last_depth = depth;
    (*this_).last_location = u8_f64_max2( to_y, top );

    U8_TRACE_END();
}

void draw_feature_symbol_draw_timeline( draw_feature_symbol_t *this_,
                                        const geometry_rectangle_t *bounds,
                                        double to_x,
                                        bool active,
                                        const pencil_size_t *pencil_size,
                                        cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );

    const double left = geometry_rectangle_get_left ( bounds );
    const double right = geometry_rectangle_get_right ( bounds );
    const double y = geometry_rectangle_get_center_y ( bounds );

    if ( ! active )
    {
        double dashes[2];
        dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
        dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
        cairo_set_dash ( cr, dashes, 2, 0.0 );
    }
    else
    {
        cairo_set_line_width( cr, pencil_size_get_bold_line_width( pencil_size ) );
    }

    cairo_move_to ( cr, u8_f64_min2( (*this_).last_location, right ), y );
    cairo_line_to ( cr, u8_f64_max2( to_x, left ), y );
    cairo_stroke (cr);

    cairo_set_dash ( cr, NULL, 0, 0.0 );
    cairo_set_line_width( cr, pencil_size_get_standard_line_width( pencil_size ) );

    (*this_).last_location = u8_f64_max2( to_x, left );

    U8_TRACE_END();
}

void draw_feature_symbol_draw_life_rectangle( const draw_feature_symbol_t *this_,
                                              const geometry_rectangle_t *bounds,
                                              bool highlighted,
                                              const pencil_size_t *pencil_size,
                                              cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );

    const double top = geometry_rectangle_get_top ( bounds );
    const double left = geometry_rectangle_get_left ( bounds );
    const double right = geometry_rectangle_get_right ( bounds );
    const double bottom = geometry_rectangle_get_bottom ( bounds );

    if ( highlighted )
    {
        double dashes[2];
        dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
        dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
        cairo_set_dash ( cr, dashes, 2, 0.0 );

        cairo_move_to ( cr, left, top );
        cairo_line_to ( cr, left, bottom );
        cairo_line_to ( cr, right, bottom );
        cairo_line_to ( cr, right, top );
        cairo_line_to ( cr, left, top );
        cairo_stroke (cr);

        cairo_set_dash ( cr, NULL, 0, 0.0 );
    }

    U8_TRACE_END();
}


/*
Copyright 2026-2026 Andreas Warnke
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
