/* File: geometry_anchor.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <assert.h>
#include <math.h>

static inline void geometry_anchor_init ( geometry_anchor_t *this_,
                                          double x,
                                          double y,
                                          geometry_h_align_t x_align,
                                          geometry_v_align_t y_align )
{
    geometry_point_init( &((*this_).reference_point), x, y );
    (*this_).x_align = x_align;
    (*this_).y_align = y_align;
}

static inline void geometry_anchor_copy ( geometry_anchor_t *this_, const geometry_anchor_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_anchor_replace ( geometry_anchor_t *this_, const geometry_anchor_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_anchor_destroy ( geometry_anchor_t *this_ )
{
    geometry_point_destroy( &((*this_).reference_point) );
}

static inline double geometry_anchor_get_x ( const geometry_anchor_t *this_ )
{
    return geometry_point_get_x( &((*this_).reference_point) );
}

static inline double geometry_anchor_get_y ( const geometry_anchor_t *this_ )
{
    return geometry_point_get_y( &((*this_).reference_point) );
}

static inline geometry_h_align_t geometry_anchor_get_x_align ( const geometry_anchor_t *this_ )
{
    return (*this_).x_align;
}

static inline geometry_v_align_t geometry_anchor_get_y_align ( const geometry_anchor_t *this_ )
{
    return (*this_).y_align;
}

static inline void geometry_anchor_trace ( const geometry_anchor_t *this_ )
{
    TRACE_INFO( "geometry_anchor_t" );
    TRACE_INFO_INT( "- x:", geometry_point_get_x( &((*this_).reference_point) ) );
    TRACE_INFO_INT( "- y:", geometry_point_get_y( &((*this_).reference_point) ) );
    TRACE_INFO_INT( "- x_align:", (*this_).x_align );
    TRACE_INFO_INT( "- y_align:", (*this_).y_align );
}


/*
Copyright 2021-2021 Andreas Warnke

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
