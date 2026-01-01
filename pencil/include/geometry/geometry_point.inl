/* File: geometry_point.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <math.h>

static inline void geometry_point_init ( geometry_point_t *this_, double x, double y )
{
    (*this_).x = x;
    (*this_).y = y;
}

static inline geometry_point_t geometry_point_new ( double x, double y )
{
    geometry_point_t result;
    geometry_point_init( &result, x, y );
    return result;
}

static inline void geometry_point_reinit ( geometry_point_t *this_, double x, double y )
{
    (*this_).x = x;
    (*this_).y = y;
}

static inline void geometry_point_copy ( geometry_point_t *this_, const geometry_point_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_point_replace ( geometry_point_t *this_, const geometry_point_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_point_destroy ( geometry_point_t *this_ )
{
}

static inline double geometry_point_get_x ( const geometry_point_t *this_ )
{
    return (*this_).x;
}

static inline double geometry_point_get_y ( const geometry_point_t *this_ )
{
    return (*this_).y;
}

static inline double geometry_point_calc_chess_distance ( const geometry_point_t *this_, const geometry_point_t *that )
{
    assert( NULL != that );
    return ( fabs( (*this_).x - (*that).x ) + fabs( (*this_).y - (*that).y ) );
}

static inline bool geometry_point_equals ( const geometry_point_t *this_, const geometry_point_t *that )
{
    assert( NULL != that );
    return ( ( fabs( (*this_).x - (*that).x ) + fabs( (*this_).y - (*that).y ) ) < 0.000000001 );
}

static inline void geometry_point_shift ( geometry_point_t *this_, double delta_x, double delta_y )
{
    (*this_).x += delta_x;
    (*this_).y += delta_y;
}

static inline void geometry_point_trace ( const geometry_point_t *this_ )
{
    U8_TRACE_INFO( "geometry_point_t" );
    U8_TRACE_INFO_INT( "- x:", (*this_).x );
    U8_TRACE_INFO_INT( "- y:", (*this_).y );
}


/*
Copyright 2019-2026 Andreas Warnke

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
