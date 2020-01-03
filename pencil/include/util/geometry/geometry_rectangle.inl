/* File: geometry_rectangle.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <assert.h>
#include <math.h>

static inline void geometry_rectangle_init ( geometry_rectangle_t *this_, double left, double top, double width, double height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline void geometry_rectangle_reinit ( geometry_rectangle_t *this_, double left, double top, double width, double height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline void geometry_rectangle_copy ( geometry_rectangle_t *this_, const geometry_rectangle_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_rectangle_replace ( geometry_rectangle_t *this_, const geometry_rectangle_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_rectangle_init_empty ( geometry_rectangle_t *this_ )
{
    (*this_).left = 0.0;
    (*this_).top = 0.0;
    (*this_).width = 0.0;
    (*this_).height = 0.0;
}

static inline void geometry_rectangle_reinit_empty ( geometry_rectangle_t *this_ )
{
    (*this_).left = 0.0;
    (*this_).top = 0.0;
    (*this_).width = 0.0;
    (*this_).height = 0.0;
}

static inline int geometry_rectangle_init_by_intersect ( geometry_rectangle_t *this_,
                                                          const geometry_rectangle_t *rect_a,
                                                          const geometry_rectangle_t *rect_b )
{
    assert( NULL != rect_a );
    assert( NULL != rect_b );

    int result = 0;
    double rect_a_right;
    double rect_a_bottom;
    double rect_b_right;
    double rect_b_bottom;

    rect_a_right = (*rect_a).left + (*rect_a).width;
    rect_a_bottom = (*rect_a).top + (*rect_a).height;
    rect_b_right = (*rect_b).left + (*rect_b).width;
    rect_b_bottom = (*rect_b).top + (*rect_b).height;

    (*this_).left = fmax( (*rect_a).left, (*rect_b).left );
    (*this_).top = fmax( (*rect_a).top, (*rect_b).top );
    (*this_).width = fmin( rect_a_right, rect_b_right ) - (*this_).left;
    (*this_).height = fmin( rect_a_bottom, rect_b_bottom ) - (*this_).top;
    if (( (*this_).width < -0.000000001 ) || ( (*this_).height < -0.000000001 ))
    {
        /* if intersection is empty, result is -1 */
        (*this_).left = 0.0;
        (*this_).top = 0.0;
        (*this_).width = 0.0;
        (*this_).height = 0.0;
        result = -1;
    }
    else if (( (*this_).width < 0.0 ) || ( (*this_).height < 0.0 ))
    {
        /* update rounding error */
        (*this_).width = 0.0;
        (*this_).height = 0.0;
    }

    return result;
}

static inline int geometry_rectangle_init_by_bounds ( geometry_rectangle_t *this_,
                                                      const geometry_rectangle_t *rect_a,
                                                      const geometry_rectangle_t *rect_b )
{
    assert( NULL != rect_a );
    assert( NULL != rect_b );

    int result = 0;
    double rect_a_right;
    double rect_a_bottom;
    double rect_b_right;
    double rect_b_bottom;

    rect_a_right = (*rect_a).left + (*rect_a).width;
    rect_a_bottom = (*rect_a).top + (*rect_a).height;
    rect_b_right = (*rect_b).left + (*rect_b).width;
    rect_b_bottom = (*rect_b).top + (*rect_b).height;

    (*this_).left = fmin( (*rect_a).left, (*rect_b).left );
    (*this_).top = fmin( (*rect_a).top, (*rect_b).top );
    (*this_).width = fmax( rect_a_right, rect_b_right ) - (*this_).left;
    (*this_).height = fmax( rect_a_bottom, rect_b_bottom ) - (*this_).top;

    return result;
}

static inline bool geometry_rectangle_is_intersecting ( const geometry_rectangle_t *this_, const geometry_rectangle_t *that )
{
    assert( NULL != that );

    bool result;
    double rect_this_right;
    double rect_this_bottom;
    double rect_that_right;
    double rect_that_bottom;

    rect_this_right = (*this_).left + (*this_).width + 0.000000001;  /* touching is intersecting */
    rect_this_bottom = (*this_).top + (*this_).height + 0.000000001;
    rect_that_right = (*that).left + (*that).width + 0.000000001;
    rect_that_bottom = (*that).top + (*that).height + 0.000000001;

    if ( ( rect_this_right < (*that).left )
        || ( rect_this_bottom < (*that).top )
        || ( (*this_).left > rect_that_right )
        || ( (*this_).top > rect_that_bottom ) )
    {
        result = false;
    }
    else
    {
        result = true;
    }

    return result;
}

static inline bool geometry_rectangle_is_containing ( const geometry_rectangle_t *this_, const geometry_rectangle_t *that )
{
    assert( NULL != that );

    bool result;
    double rect_this_right;
    double rect_this_bottom;
    double rect_that_right;
    double rect_that_bottom;

    rect_this_right = (*this_).left + (*this_).width;
    rect_this_bottom = (*this_).top + (*this_).height;
    rect_that_right = (*that).left + (*that).width;
    rect_that_bottom = (*that).top + (*that).height;

    if ( ( (*this_).left < (*that).left + 0.000000001 ) /* touching is containing */
        && ( (*this_).top < (*that).top + 0.000000001 )
        && ( rect_this_right + 0.000000001 > rect_that_right )
        && ( rect_this_bottom + 0.000000001 > rect_that_bottom ) )
    {
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

static inline void geometry_rectangle_init_by_corners ( geometry_rectangle_t *this_, double x1, double y1, double x2, double y2 )
{
    if ( x1 < x2 )
    {
        (*this_).left = x1;
        (*this_).width = x2-x1;
    }
    else
    {
        (*this_).left = x2;
        (*this_).width = x1-x2;
    }
    if ( y1 < y2 )
    {
        (*this_).top = y1;
        (*this_).height = y2-y1;
    }
    else
    {
        (*this_).top = y2;
        (*this_).height = y1-y2;
    }
}

static inline void geometry_rectangle_destroy ( geometry_rectangle_t *this_ )
{
}

static inline double geometry_rectangle_get_left ( const geometry_rectangle_t *this_ )
{
    return (*this_).left;
}

static inline double geometry_rectangle_get_top ( const geometry_rectangle_t *this_ )
{
    return (*this_).top;
}

static inline double geometry_rectangle_get_right ( const geometry_rectangle_t *this_ )
{
    return (*this_).left + (*this_).width;
}

static inline double geometry_rectangle_get_bottom ( const geometry_rectangle_t *this_ )
{
    return (*this_).top + (*this_).height;
}

static inline double geometry_rectangle_get_width ( const geometry_rectangle_t *this_ )
{
    return (*this_).width;
}

static inline double geometry_rectangle_get_height ( const geometry_rectangle_t *this_ )
{
    return (*this_).height;
}

static inline double geometry_rectangle_get_center_x ( const geometry_rectangle_t *this_ )
{
    return (*this_).left + 0.5*(*this_).width;
}

static inline double geometry_rectangle_get_center_y ( const geometry_rectangle_t *this_ )
{
    return (*this_).top + 0.5*(*this_).height;
}

static inline geometry_point_t geometry_rectangle_get_center ( const geometry_rectangle_t *this_ )
{
    geometry_point_t result;
    geometry_point_init ( &result, (*this_).left + 0.5*(*this_).width, (*this_).top + 0.5*(*this_).height );
    return result;
}

static inline double geometry_rectangle_get_area ( const geometry_rectangle_t *this_ )
{
    return (*this_).width * (*this_).height;
}

static inline bool geometry_rectangle_contains ( const geometry_rectangle_t *this_, double x, double y )
{
    return (( (*this_).left <= x )&&( x < (*this_).left + (*this_).width )&&( (*this_).top <= y )&&( y < (*this_).top + (*this_).height ));
}

static inline double geometry_rectangle_calc_chess_distance ( const geometry_rectangle_t *this_, double x, double y )
{
    double result = 0.0;
    if ( x < (*this_).left )
    {
        result += ( (*this_).left - x );
    }
    else if ( x > (*this_).left + (*this_).width )
    {
        result += ( x - ((*this_).left + (*this_).width) );
    }
    if ( y < (*this_).top )
    {
        result += ( (*this_).top - y );
    }
    else if ( y > (*this_).top + (*this_).height )
    {
        result += ( y - ((*this_).top + (*this_).height) );
    }
    return result;
}

static inline bool geometry_rectangle_is_empty ( const geometry_rectangle_t *this_ )
{
    return ( ( (*this_).width < 0.000000001 )||( (*this_).height < 0.000000001 ) );
}

static inline double geometry_rectangle_get_intersect_area ( const geometry_rectangle_t *this_, const geometry_rectangle_t *that )
{
    geometry_rectangle_t intersect;
    geometry_rectangle_init_by_intersect( &intersect, this_, that );
    return geometry_rectangle_get_area(&intersect);
}

static inline void geometry_rectangle_shift ( geometry_rectangle_t *this_, double delta_x, double delta_y )
{
    (*this_).left += delta_x;
    (*this_).top += delta_y;
}

static inline void geometry_rectangle_expand ( geometry_rectangle_t *this_, double delta_width, double delta_height )
{
    (*this_).width += delta_width;
    if ( (*this_).width < 0.0 )
    {
        (*this_).width = 0.0;
    }

    (*this_).height += delta_height;
    if ( (*this_).height < 0.0 )
    {
        (*this_).height = 0.0;
    }
}

static inline void geometry_rectangle_trace ( const geometry_rectangle_t *this_ )
{
    TRACE_INFO( "geometry_rectangle_t" );
    TRACE_INFO_INT( "- left:", (*this_).left );
    TRACE_INFO_INT( "- top:", (*this_).top );
    TRACE_INFO_INT( "- width:", (*this_).width );
    TRACE_INFO_INT( "- height:", (*this_).height );
}


/*
Copyright 2016-2020 Andreas Warnke

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
