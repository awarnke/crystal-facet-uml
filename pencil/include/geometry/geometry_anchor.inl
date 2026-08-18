/* File: geometry_anchor.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "u8/u8_f64.h"
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

    const double width = geometry_rectangle_get_width( unaligned );
    const double height = geometry_rectangle_get_height( unaligned );

    const double left = geometry_h_align_get_left( &((*this_).x_align),
                                                   width,
                                                   geometry_point_get_x( &((*this_).reference_point) ),
                                                   0.0 /* reference_width is zero, the reference is a point */
                                                 );
    const double top = geometry_v_align_get_top( &((*this_).y_align),
                                                 height,
                                                 geometry_point_get_y( &((*this_).reference_point) ),
                                                 0.0 /* reference_height is zero, the reference is a point */
                                               );

    geometry_rectangle_init( &result, left, top, width, height );
    return result;
}

static inline geometry_rectangle_t geometry_anchor_align_dim ( const geometry_anchor_t *this_,
                                                               const geometry_dimensions_t *unaligned )
{
    assert( unaligned != NULL );

    geometry_rectangle_t result;

    const double width = geometry_dimensions_get_width( unaligned );
    const double height = geometry_dimensions_get_height( unaligned );

    const double left = geometry_h_align_get_left( &((*this_).x_align),
                                                   width,
                                                   geometry_point_get_x( &((*this_).reference_point) ),
                                                   0.0 /* reference_width is zero, the reference is a point */
                                                 );
    const double top = geometry_v_align_get_top( &((*this_).y_align),
                                                 height,
                                                 geometry_point_get_y( &((*this_).reference_point) ),
                                                 0.0 /* reference_height is zero, the reference is a point */
                                               );

    geometry_rectangle_init( &result, left, top, width, height );
    return result;
}

static inline geometry_rectangle_t geometry_anchor_align_dim_closest( const geometry_anchor_t *this_,
                                                                      const geometry_dimensions_t *unaligned,
                                                                      const geometry_rectangle_t *permitted_area )
{
    assert( unaligned != NULL );
    assert( permitted_area != NULL );

    geometry_rectangle_t result;

    const double width = geometry_dimensions_get_width( unaligned );
    const double height = geometry_dimensions_get_height( unaligned );

    const double left = geometry_h_align_get_left( &((*this_).x_align),
                                                   width,
                                                   geometry_point_get_x( &((*this_).reference_point) ),
                                                   0.0 /* reference_width is zero, the reference is a point */
                                                 );
    const double top = geometry_v_align_get_top( &((*this_).y_align),
                                                 height,
                                                 geometry_point_get_y( &((*this_).reference_point) ),
                                                 0.0 /* reference_height is zero, the reference is a point */
                                               );

    /* force the result into permitted_area */
    const double right = left + width;
    const double bottom = top + height;
    const double permitted_left = geometry_rectangle_get_left( permitted_area );
    const double permitted_top = geometry_rectangle_get_top( permitted_area );
    const double permitted_right = geometry_rectangle_get_right( permitted_area );
    const double permitted_bottom = geometry_rectangle_get_bottom( permitted_area );
    double new_left = left;
    double new_top = top;
    if ( left < permitted_left )
    {
        if ( right > permitted_right )
        {
            U8_TRACE_INFO( "geometry_anchor_align_dim_closest cannot fit dim-x into permitted_area" );
        }
        else
        {
            /* move the unaligned area either till right side is at limit or till left side is fitting */
            new_left = u8_f64_min2( permitted_right - width, permitted_left );
        }
    }
    else
    {
        if ( right > permitted_right )
        {
            /* move the unaligned area either till left side is at limit or till right side is fitting */
            new_left = u8_f64_max2( permitted_right - width, permitted_left );
        }
        else
        {
            /* rectangle is fitting, no move */
        }
    }
    if ( top < permitted_top )
    {
        if ( bottom > permitted_bottom )
        {
            U8_TRACE_INFO( "geometry_anchor_align_dim_closest cannot fit dim-y into permitted_area" );
        }
        else
        {
            /* move the unaligned area either till bottom side is at limit or till top side is fitting */
            new_top = u8_f64_min2( permitted_bottom - height, permitted_top );
        }
    }
    else
    {
        if ( bottom > permitted_bottom )
        {
            /* move the unaligned area either till top side is at limit or till bottom side is fitting */
            new_top = u8_f64_max2( permitted_bottom - height, permitted_top );
        }
        else
        {
            /* rectangle is fitting, no move */
        }
    }

#if 0
    /* in case of center, allow small moves */
    if ( (*this_).x_align == GEOMETRY_H_ALIGN_CENTER )
    {
        /* if left/top position exceeds preferred_location, then move */
        const double min_x = geometry_rectangle_get_left( preferred_location );
        const double max_x = geometry_rectangle_get_right( preferred_location );
        const double max_width = geometry_rectangle_get_width( preferred_location );
        if ( left < min_x )
        {
            if ( width <= max_width )
            {
                /* left/top position is too far left, but can fit if moved */
                left = min_x;
            }
            else
            {
                /* unaligned does not fit to preferred_location, simply center the result */
                left = geometry_rectangle_get_center_x( preferred_location ) - 0.5 * width;
            }
        }
        else if ( left + width > max_x )
        {
            if ( width <= max_width )
            {
                /* left/top position is too far left, but can fit if moved */
                left = max_x - width;
            }
            else
            {
                /* unaligned does not fit to preferred_location, simply center the result */
                left = geometry_rectangle_get_center_x( preferred_location ) - 0.5 * width;
            }
        }
        /* if result moved too far, pull it back to anchor */
        const double reference_x = geometry_point_get_x( &((*this_).reference_point) );
        if ( left + width < reference_x )
        {
            left = reference_x - width;
        }
        if ( left > reference_x )
        {
            left = reference_x;
        }
    }
    if ( (*this_).y_align == GEOMETRY_V_ALIGN_CENTER )
    {
        /* if left/top position exceeds preferred_location, then move */
        const double min_y = geometry_rectangle_get_top( preferred_location );
        const double max_y = geometry_rectangle_get_bottom( preferred_location );
        const double max_height = geometry_rectangle_get_height( preferred_location );
        if ( top < min_y )
        {
            if ( height <= max_height )
            {
                /* left/top position is too far left, but can fit if moved */
                top = min_y;
            }
            else
            {
                /* unaligned does not fit to preferred_location, simply center the result */
                top = geometry_rectangle_get_center_y( preferred_location ) - 0.5 * height;
            }
        }
        else if ( top + height > max_y )
        {
            if ( height <= max_height )
            {
                /* left/top position is too far left, but can fit if moved */
                top = max_y - height;
            }
            else
            {
                /* unaligned does not fit to preferred_location, simply center the result */
                top = geometry_rectangle_get_center_y( preferred_location ) - 0.5 * height;
            }
        }
        /* if result moved too far, pull it back to anchor */
        const double reference_y = geometry_point_get_y( &((*this_).reference_point) );
        if ( top + height < reference_y )
        {
            top = reference_y - height;
        }
        if ( top > reference_y )
        {
            top = reference_y;
        }
    }
#endif

    geometry_rectangle_init( &result, new_left, new_top, width, height );
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
Copyright 2021-2026 Andreas Warnke

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
