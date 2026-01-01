/* File: geometry_offset.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <math.h>

static inline void geometry_offset_init ( geometry_offset_t *this_, double dx, double dy )
{
    (*this_).dx = dx;
    (*this_).dy = dy;
}

static inline geometry_offset_t geometry_offset_new ( double dx, double dy )
{
    geometry_offset_t result;
    geometry_offset_init( &result, dx, dy );
    return result;
}

static inline void geometry_offset_reinit ( geometry_offset_t *this_, double dx, double dy )
{
    (*this_).dx = dx;
    (*this_).dy = dy;
}

static inline void geometry_offset_copy ( geometry_offset_t *this_, const geometry_offset_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_offset_replace ( geometry_offset_t *this_, const geometry_offset_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_offset_destroy ( geometry_offset_t *this_ )
{
}

static inline double geometry_offset_get_dx ( const geometry_offset_t *this_ )
{
    return (*this_).dx;
}

static inline double geometry_offset_get_dy ( const geometry_offset_t *this_ )
{
    return (*this_).dy;
}

static inline bool geometry_offset_equals ( const geometry_offset_t *this_, const geometry_offset_t *that )
{
    assert( NULL != that );
    return ( ( fabs( (*this_).dx - (*that).dx ) + fabs( (*this_).dy - (*that).dy ) ) < 0.000000001 );
}

static inline geometry_direction_t geometry_offset_get_direction ( const geometry_offset_t *this_ )
{
    geometry_direction_t result;
    if ( (*this_).dx > 0.000000001 )
    {
        if ( (*this_).dy > 0.000000001 )
        {
            result = GEOMETRY_DIRECTION_DOWN_RIGHT;
        }
        else if ( (*this_).dy < -0.000000001 )
        {
            result = GEOMETRY_DIRECTION_UP_RIGHT;
        }
        else
        {
            result = GEOMETRY_DIRECTION_RIGHT;
        }
    }
    else if ( (*this_).dx < -0.000000001 )
    {
        if ( (*this_).dy > 0.000000001 )
        {
            result = GEOMETRY_DIRECTION_DOWN_LEFT;
        }
        else if ( (*this_).dy < -0.000000001 )
        {
            result = GEOMETRY_DIRECTION_UP_LEFT;
        }
        else
        {
            result = GEOMETRY_DIRECTION_LEFT;
        }
    }
    else
    {
        if ( (*this_).dy > 0.000000001 )
        {
            result = GEOMETRY_DIRECTION_DOWN;
        }
        else if ( (*this_).dy < -0.000000001 )
        {
            result = GEOMETRY_DIRECTION_UP;
        }
        else
        {
            result = GEOMETRY_DIRECTION_CENTER;
        }
    }
    return result;
}

static inline void geometry_offset_trace ( const geometry_offset_t *this_ )
{
    U8_TRACE_INFO( "geometry_offset_t" );
    U8_TRACE_INFO_INT( "- dx:", (*this_).dx );
    U8_TRACE_INFO_INT( "- dy:", (*this_).dy );
}


/*
Copyright 2025-2026 Andreas Warnke

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
