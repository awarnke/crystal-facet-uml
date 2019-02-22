/* File: draw_symbol.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline void draw_symbol_init( draw_symbol_t *this_ )
{

}

static inline void draw_symbol_destroy( draw_symbol_t *this_ )
{

}

static inline geometry_rectangle_t draw_symbol_get_component_bounds ( const draw_symbol_t *this_,
                                                                      double x,
                                                                      double y,
                                                                      geometry_h_align_t h_align,
                                                                      geometry_v_align_t v_align,
                                                                      double height )
{
    geometry_rectangle_t result;

    const double width = 1.4 * height;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_symbol_get_artifact_bounds ( const draw_symbol_t *this_,
                                                                     double x,
                                                                     double y,
                                                                     geometry_h_align_t h_align,
                                                                     geometry_v_align_t v_align,
                                                                     double height )
{
    geometry_rectangle_t result;

    const double width = 0.7 * height;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_symbol_get_actor_bounds ( const draw_symbol_t *this_,
                                                                  double x,
                                                                  double y,
                                                                  geometry_h_align_t h_align,
                                                                  geometry_v_align_t v_align,
                                                                  double height )
{
    geometry_rectangle_t result;

    const double width = height/3.0;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_symbol_get_circle_bounds ( const draw_symbol_t *this_,
                                                                   double x,
                                                                   double y,
                                                                   geometry_h_align_t h_align,
                                                                   geometry_v_align_t v_align,
                                                                   double height )
{
    geometry_rectangle_t result;

    const double width = height;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_symbol_get_time_bounds ( const draw_symbol_t *this_,
                                                                 double x,
                                                                 double y,
                                                                 geometry_h_align_t h_align,
                                                                 geometry_v_align_t v_align,
                                                                 double height )
{
    geometry_rectangle_t result;

    const double width = height/1.732050808;
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}

static inline geometry_rectangle_t draw_symbol_get_sync_bounds ( const draw_symbol_t *this_,
                                                                 double x,
                                                                 double y,
                                                                 geometry_h_align_t h_align,
                                                                 geometry_v_align_t v_align,
                                                                 double height,
                                                                 const pencil_size_t *pencil_size )
{
    geometry_rectangle_t result;

    const double width = 3.0 * pencil_size_get_bold_line_width( pencil_size );
    geometry_rectangle_init ( &result,
                              geometry_h_align_get_left( &h_align, width, x, 0.0 ),
                              geometry_v_align_get_top( &v_align, height, y, 0.0 ),
                              width,
                              height
                            );

    return result;
}


/*
Copyright 2019-2019 Andreas Warnke

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
