/* File: geometry_anchor.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <math.h>

static inline void geometry_anchor_init( geometry_anchor_t *this_,
                                         double x,
                                         double y,
                                         geometry_h_align_t x_align,
                                         geometry_v_align_t y_align )
{
    geometry_point_init( &((*this_).reference_point), x, y );
    (*this_).x_align = x_align;
    (*this_).y_align = y_align;
}

static inline void geometry_anchor_copy( geometry_anchor_t *this_, const geometry_anchor_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_anchor_replace( geometry_anchor_t *this_, const geometry_anchor_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_anchor_destroy( geometry_anchor_t *this_ )
{
    geometry_point_destroy( &((*this_).reference_point) );
}

static inline double geometry_anchor_get_x( const geometry_anchor_t *this_ )
{
    return geometry_point_get_x( &((*this_).reference_point) );
}

static inline double geometry_anchor_get_y( const geometry_anchor_t *this_ )
{
    return geometry_point_get_y( &((*this_).reference_point) );
}

static inline geometry_h_align_t geometry_anchor_get_x_align( const geometry_anchor_t *this_ )
{
    return (*this_).x_align;
}

static inline geometry_v_align_t geometry_anchor_get_y_align( const geometry_anchor_t *this_ )
{
    return (*this_).y_align;
}

static inline const geometry_point_t * geometry_anchor_get_point_const( const geometry_anchor_t *this_ )
{
    return &((*this_).reference_point);
}

static inline geometry_rectangle_t geometry_anchor_align_rect( const geometry_anchor_t *this_,
                                                               const geometry_rectangle_t *unaligned )
{
    assert( unaligned != NULL );

    geometry_rectangle_t result;
    double left = geometry_h_align_get_left( &((*this_).x_align),
                                             geometry_rectangle_get_width( unaligned ),
                                             geometry_point_get_x( &((*this_).reference_point) ),
                                             0.0 /* reference_width is zero, the reference is a point */
                                           );
    double top = geometry_v_align_get_top( &((*this_).y_align),
                                           geometry_rectangle_get_height( unaligned ),
                                           geometry_point_get_y( &((*this_).reference_point) ),
                                           0.0 /* reference_height is zero, the reference is a point */
                                         );
    geometry_rectangle_init( &result,
                             left,
                             top,
                             geometry_rectangle_get_width( unaligned ),
                             geometry_rectangle_get_height( unaligned )
                           );
    return result;
}

static inline geometry_rectangle_t geometry_anchor_align_dim ( const geometry_anchor_t *this_,
                                                               const geometry_dimensions_t *unaligned )
{
    assert( unaligned != NULL );

    geometry_rectangle_t result;
    double left = geometry_h_align_get_left( &((*this_).x_align),
                                             geometry_dimensions_get_width( unaligned ),
                                             geometry_point_get_x( &((*this_).reference_point) ),
                                             0.0 /* reference_width is zero, the reference is a point */
                                           );
    double top = geometry_v_align_get_top( &((*this_).y_align),
                                           geometry_dimensions_get_height( unaligned ),
                                           geometry_point_get_y( &((*this_).reference_point) ),
                                           0.0 /* reference_height is zero, the reference is a point */
                                         );
    geometry_rectangle_init( &result,
                             left,
                             top,
                             geometry_dimensions_get_width( unaligned ),
                             geometry_dimensions_get_height( unaligned )
                           );
    return result;
}

static inline geometry_rectangle_t geometry_anchor_align_dim_bounded ( const geometry_anchor_t *this_,
                                                                       const geometry_dimensions_t *unaligned,
                                                                       const geometry_rectangle_t *fence )
{
    assert( unaligned != NULL );

    geometry_rectangle_t result;

    const double width = geometry_dimensions_get_width( unaligned );
    const double height = geometry_dimensions_get_height( unaligned );

    double left = geometry_h_align_get_left( &((*this_).x_align),
                                             width,
                                             geometry_point_get_x( &((*this_).reference_point) ),
                                             0.0 /* reference_width is zero, the reference is a point */
    );
    double top = geometry_v_align_get_top( &((*this_).y_align),
                                           height,
                                           geometry_point_get_y( &((*this_).reference_point) ),
                                           0.0 /* reference_height is zero, the reference is a point */
    );

    /* move the top-left point in case the result rectangle is outside fence */
    if ( left + width > geometry_rectangle_get_right( fence ) )
    {
        left = geometry_rectangle_get_right( fence ) - width;
    }
    if ( top + height > geometry_rectangle_get_bottom( fence ) )
    {
        top = geometry_rectangle_get_bottom( fence ) - height;
    }
    if ( left < geometry_rectangle_get_left( fence ) )
    {
        left = geometry_rectangle_get_left( fence );
    }
    if ( top < geometry_rectangle_get_top( fence ) )
    {
        top = geometry_rectangle_get_top( fence );
    }

    geometry_rectangle_init( &result, left, top, width, height );
    return result;
}

static inline void geometry_anchor_trace( const geometry_anchor_t *this_ )
{
    U8_TRACE_INFO( "geometry_anchor_t" );
    U8_TRACE_INFO_INT( "- x:", geometry_point_get_x( &((*this_).reference_point) ) );
    U8_TRACE_INFO_INT( "- y:", geometry_point_get_y( &((*this_).reference_point) ) );
    U8_TRACE_INFO_INT( "- x_align:", (*this_).x_align );
    U8_TRACE_INFO_INT( "- y_align:", (*this_).y_align );
}


/*
Copyright 2021-2025 Andreas Warnke

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
