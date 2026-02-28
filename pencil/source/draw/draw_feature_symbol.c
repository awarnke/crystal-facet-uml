/* File: draw_feature_symbol.c; Copyright and License: see below */

#include "draw/draw_feature_symbol.h"
#include "u8/u8_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

void draw_feature_symbol_init( draw_feature_symbol_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).last_depth = 0;

    U8_TRACE_END();
}

void draw_feature_symbol_reinit( draw_feature_symbol_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).last_depth = 0;

    U8_TRACE_END();
}

void draw_feature_symbol_destroy( draw_feature_symbol_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_END();
}


void draw_feature_symbol_draw_execution_spec( draw_feature_symbol_t *this_,
                                              const geometry_rectangle_t *bounds,
                                              int32_t from_order,
                                              int32_t to_order,
                                              uint32_t depth,
                                              const pencil_size_t *pencil_size,
                                              cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );

    const double x = geometry_rectangle_get_center_x ( bounds );
    const double top = geometry_rectangle_get_top ( bounds );
    const double bottom = geometry_rectangle_get_bottom ( bounds );

    double dashes[2];
    dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
    dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
    cairo_set_dash ( cr, dashes, 2, 0.0 );

    cairo_move_to ( cr, x + 2.0 * depth, top );
    cairo_line_to ( cr, x + 2.0 * depth, bottom );
    cairo_stroke (cr);

    (*this_).last_depth = depth;

    cairo_set_dash ( cr, NULL, 0, 0.0 );

    U8_TRACE_END();
}

void draw_feature_symbol_draw_timeline( const draw_feature_symbol_t *this_,
                                        const geometry_rectangle_t *bounds,
                                        int32_t from_order,
                                        int32_t to_order,
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

    double dashes[2];
    dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
    dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
    cairo_set_dash ( cr, dashes, 2, 0.0 );

    cairo_move_to ( cr, left, y );
    cairo_line_to ( cr, right, y );
    cairo_stroke (cr);

    cairo_set_dash ( cr, NULL, 0, 0.0 );

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
