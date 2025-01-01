/* File: geometry_grid.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <stdbool.h>

static inline void geometry_grid_init ( geometry_grid_t *this_, geometry_grid_kind_t kind )
{
    (*this_).kind = kind;
    geometry_non_linear_scale_init( &((*this_).x_scale), 0.0, 640.0 );
    geometry_non_linear_scale_init( &((*this_).y_scale), 0.0, 480.0 );
}

static inline void geometry_grid_reinit ( geometry_grid_t *this_, geometry_grid_kind_t kind )
{
    (*this_).kind = kind;
    geometry_non_linear_scale_init( &((*this_).x_scale), 0.0, 640.0 );
    geometry_non_linear_scale_init( &((*this_).y_scale), 0.0, 480.0 );
}

static inline void geometry_grid_destroy ( geometry_grid_t *this_ )
{
}

static inline void geometry_grid_trace ( const geometry_grid_t *this_ )
{
    switch ( (*this_).kind )
    {
        case GEOMETRY_GRID_KIND_0:
        default:
        {
            U8_TRACE_INFO( "geometry_grid_t: GEOMETRY_GRID_KIND_0" );
        }
        break;

        case GEOMETRY_GRID_KIND_X:
        {
            U8_TRACE_INFO( "geometry_grid_t: GEOMETRY_GRID_KIND_X" );
            geometry_non_linear_scale_trace( &((*this_).x_scale) );
        }
        break;

        case GEOMETRY_GRID_KIND_Y:
        {
            U8_TRACE_INFO( "geometry_grid_t: GEOMETRY_GRID_KIND_Y" );
            geometry_non_linear_scale_trace( &((*this_).y_scale) );
        }
        break;

        case GEOMETRY_GRID_KIND_XY:
        {
            U8_TRACE_INFO( "geometry_grid_t: GEOMETRY_GRID_KIND_XY" );
            geometry_non_linear_scale_trace( &((*this_).x_scale) );
            geometry_non_linear_scale_trace( &((*this_).y_scale) );
        }
        break;
    }
}

static inline geometry_grid_kind_t geometry_grid_get_kind ( const geometry_grid_t *this_ )
{
    return (*this_).kind;
}

static inline geometry_non_linear_scale_t * geometry_grid_get_x_scale_ptr ( geometry_grid_t *this_ )
{
    return &((*this_).x_scale);
}

static inline const geometry_non_linear_scale_t * geometry_grid_get_x_scale_const ( const geometry_grid_t *this_ )
{
    return &((*this_).x_scale);
}

static inline geometry_non_linear_scale_t * geometry_grid_get_y_scale_ptr ( geometry_grid_t *this_ )
{
    return &((*this_).y_scale);
}

static inline const geometry_non_linear_scale_t * geometry_grid_get_y_scale_const ( const geometry_grid_t *this_ )
{
    return &((*this_).y_scale);
}


/*
Copyright 2024-2025 Andreas Warnke

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
