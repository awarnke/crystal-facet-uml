/* File: geometry_rectangle.c; Copyright and License: see below */

#include "geometry/geometry_rectangle.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <math.h>

int geometry_rectangle_init_by_difference ( geometry_rectangle_t *this_,
                                            const geometry_rectangle_t *rect_a,
                                            const geometry_rectangle_t *rect_b )
{
    U8_TRACE_BEGIN();
    assert( NULL != rect_a );
    assert( NULL != rect_b );

    int result = 0;

    const double a_left = geometry_rectangle_get_left( rect_a );
    const double a_right = geometry_rectangle_get_right( rect_a );
    const double a_top = geometry_rectangle_get_top( rect_a );
    const double a_bottom = geometry_rectangle_get_bottom( rect_a );
    const double b_left = geometry_rectangle_get_left( rect_b );
    const double b_right = geometry_rectangle_get_right( rect_b );
    const double b_top = geometry_rectangle_get_top( rect_b );
    const double b_bottom = geometry_rectangle_get_bottom( rect_b );

    if ( ( b_right <= a_left )
        || ( b_bottom <= a_top )
        || ( b_left >= a_right )
        || ( b_top >= a_bottom ) )
    {
        /* no intersection */
        (*this_) = (*rect_a);
    }
    else
    {
        /* determine where to check for a suitable, maximum sub-rectangle */
        const bool check_top = (( b_top > a_top )&&( b_top < a_bottom ));
        const bool check_bottom = (( b_bottom > a_top )&&( b_bottom < a_bottom ));
        const bool check_left = (( b_left > a_left )&&( b_left < a_right ));
        const bool check_right = (( b_right > a_left )&&( b_right < a_right ));
        bool keep_top_part = false;
        bool keep_bottom_part = false;
        bool keep_left_part = false;
        bool keep_right_part = false;
        if ( check_top )
        {
            if ( check_bottom )
            {
                if ( check_left )
                {
                    if ( check_right )
                    {
                        U8_TRACE_INFO( "to.bo.le.ri." );
                        /* 4 possible solutions */
                        const double area_top = geometry_rectangle_get_width( rect_a ) * ( b_top-a_top );
                        const double area_bottom = geometry_rectangle_get_width( rect_a ) * ( a_bottom-b_bottom );
                        const double area_left = ( b_left-a_left ) * geometry_rectangle_get_height( rect_a );
                        const double area_right = ( a_right-b_right ) * geometry_rectangle_get_height( rect_a );
                        if (area_top>area_bottom)
                        {
                            if (area_top>area_left)
                            {
                                if (area_top>area_right)
                                {
                                    keep_top_part = true;
                                }
                                else  /* area_right is bigger */
                                {
                                    keep_right_part = true;
                                }
                            }
                            else  /* area_left is bigger */
                            {
                                if (area_left>area_right)
                                {
                                    keep_left_part = true;
                                }
                                else  /* area_right is bigger */
                                {
                                    keep_right_part = true;
                                }
                            }
                        }
                        else  /* area_bottom is bigger */
                        {
                            if (area_bottom>area_left)
                            {
                                if (area_bottom>area_right)
                                {
                                    keep_bottom_part = true;
                                }
                                else  /* area_right is bigger */
                                {
                                    keep_right_part = true;
                                }
                            }
                            else  /* area_left is bigger */
                            {
                                if (area_left>area_right)
                                {
                                    keep_left_part = true;
                                }
                                else  /* area_right is bigger */
                                {
                                    keep_right_part = true;
                                }
                            }
                        }
                    }
                    else  /* ! check_right */
                    {
                        U8_TRACE_INFO( "to.bo.le." );
                        /* 3 possible solutions */
                        const double area_top = geometry_rectangle_get_width( rect_a ) * ( b_top-a_top );
                        const double area_bottom = geometry_rectangle_get_width( rect_a ) * ( a_bottom-b_bottom );
                        const double area_left = ( b_left-a_left ) * geometry_rectangle_get_height( rect_a );
                        if (area_top>area_bottom)
                        {
                            if (area_top>area_left)
                            {
                                keep_top_part = true;
                            }
                            else  /* area_left is bigger */
                            {
                                keep_left_part = true;
                            }
                        }
                        else  /* area_bottom is bigger */
                        {
                            if (area_bottom>area_left)
                            {
                                keep_bottom_part = true;
                            }
                            else  /* area_left is bigger */
                            {
                                keep_left_part = true;
                            }
                        }
                    }
                }
                else  /* ! check_left */
                {
                    if ( check_right )
                    {
                        U8_TRACE_INFO( "to.bo.ri." );
                        /* 3 possible solutions */
                        const double area_top = geometry_rectangle_get_width( rect_a ) * ( b_top-a_top );
                        const double area_bottom = geometry_rectangle_get_width( rect_a ) * ( a_bottom-b_bottom );
                        const double area_right = ( a_right-b_right ) * geometry_rectangle_get_height( rect_a );
                        if (area_top>area_bottom)
                        {
                            if (area_top>area_right)
                            {
                                keep_top_part = true;
                            }
                            else  /* area_right is bigger */
                            {
                                keep_right_part = true;
                            }
                        }
                        else  /* area_bottom is bigger */
                        {
                            if (area_bottom>area_right)
                            {
                                keep_bottom_part = true;
                            }
                            else  /* area_right is bigger */
                            {
                                keep_right_part = true;
                            }
                        }
                    }
                    else  /* ! check_right */
                    {
                        U8_TRACE_INFO( "to.bo." );
                        /* 2 possible solutions */
                        if (( b_top-a_top )>( a_bottom-b_bottom ))
                        {
                            keep_top_part = true;
                        }
                        else
                        {
                            keep_bottom_part = true;
                        }
                    }
                }
            }
            else  /* ! check_bottom */
            {
                if ( check_left )
                {
                    if ( check_right )
                    {
                        U8_TRACE_INFO( "to.le.ri." );
                        /* 3 possible solutions */
                        const double area_top = geometry_rectangle_get_width( rect_a ) * ( b_top-a_top );
                        const double area_left = ( b_left-a_left ) * geometry_rectangle_get_height( rect_a );
                        const double area_right = ( a_right-b_right ) * geometry_rectangle_get_height( rect_a );
                        if (area_top>area_left)
                        {
                            if (area_top>area_right)
                            {
                                keep_top_part = true;
                            }
                            else  /* area_right is bigger */
                            {
                                keep_right_part = true;
                            }
                        }
                        else  /* area_left is bigger */
                        {
                            if (area_left>area_right)
                            {
                                keep_left_part = true;
                            }
                            else  /* area_right is bigger */
                            {
                                keep_right_part = true;
                            }
                        }
                    }
                    else  /* ! check_right */
                    {
                        U8_TRACE_INFO( "to.le." );
                        /* 2 possible solutions */
                        const double area_top = geometry_rectangle_get_width( rect_a ) * ( b_top-a_top );
                        const double area_left = ( b_left-a_left ) * geometry_rectangle_get_height( rect_a );
                        if (area_top>area_left)
                        {
                            keep_top_part = true;
                        }
                        else  /* area_left is bigger */
                        {
                            keep_left_part = true;
                        }
                    }

                }
                else  /* ! check_left */
                {
                    if ( check_right )
                    {
                        U8_TRACE_INFO( "to.ri." );
                        /* 2 possible solutions */
                        const double area_top = geometry_rectangle_get_width( rect_a ) * ( b_top-a_top );
                        const double area_right = ( a_right-b_right ) * geometry_rectangle_get_height( rect_a );
                        if (area_top>area_right)
                        {
                            keep_top_part = true;
                        }
                        else  /* area_right is bigger */
                        {
                            keep_right_part = true;
                        }
                    }
                    else  /* ! check_right */
                    {
                        U8_TRACE_INFO( "to." );
                        /* 1 solution */
                        keep_top_part = true;
                    }
                }
            }
        }
        else  /* ! check_top */
        {
            if ( check_bottom )
            {
                if ( check_left )
                {
                    if ( check_right )
                    {
                        U8_TRACE_INFO( "bo.le.ri." );
                        /* 3 possible solutions */
                        const double area_bottom = geometry_rectangle_get_width( rect_a ) * ( a_bottom-b_bottom );
                        const double area_left = ( b_left-a_left ) * geometry_rectangle_get_height( rect_a );
                        const double area_right = ( a_right-b_right ) * geometry_rectangle_get_height( rect_a );
                        if (area_bottom>area_left)
                        {
                            if (area_bottom>area_right)
                            {
                                keep_bottom_part = true;
                            }
                            else  /* area_right is bigger */
                            {
                                keep_right_part = true;
                            }
                        }
                        else  /* area_left is bigger */
                        {
                            if (area_left>area_right)
                            {
                                keep_left_part = true;
                            }
                            else  /* area_right is bigger */
                            {
                                keep_right_part = true;
                            }
                        }
                    }
                    else  /* ! check_right */
                    {
                        U8_TRACE_INFO( "bo.le." );
                        /* 2 possible solutions */
                        const double area_bottom = geometry_rectangle_get_width( rect_a ) * ( a_bottom-b_bottom );
                        const double area_left = ( b_left-a_left ) * geometry_rectangle_get_height( rect_a );
                        if (area_bottom>area_left)
                        {
                            keep_bottom_part = true;
                        }
                        else  /* area_left is bigger */
                        {
                            keep_left_part = true;
                        }
                    }

                }
                else  /* ! check_left */
                {
                    if ( check_right )
                    {
                        U8_TRACE_INFO( "bo.ri." );
                        /* 2 possible solutions */
                        const double area_bottom = geometry_rectangle_get_width( rect_a ) * ( a_bottom-b_bottom );
                        const double area_right = ( a_right-b_right ) * geometry_rectangle_get_height( rect_a );
                        if (area_bottom>area_right)
                        {
                            keep_bottom_part = true;
                        }
                        else  /* area_right is bigger */
                        {
                            keep_right_part = true;
                        }
                    }
                    else  /* ! check_right */
                    {
                        U8_TRACE_INFO( "bo." );
                        /* 1 solution */
                        keep_bottom_part = true;
                    }
                }
            }
            else  /* ! check_bottom */
            {
                if ( check_left )
                {
                    if ( check_right )
                    {
                        U8_TRACE_INFO( "le.ri." );
                        /* 2 possible solutions */
                        if (( b_left-a_left )>( a_right-b_right ))
                        {
                            keep_left_part = true;
                        }
                        else
                        {
                            keep_right_part = true;
                        }
                    }
                    else  /* ! check_right */
                    {
                        U8_TRACE_INFO( "le." );
                        /* 1 solution */
                        keep_left_part = true;
                    }

                }
                else  /* ! check_left */
                {
                    if ( check_right )
                    {
                        U8_TRACE_INFO( "ri." );
                        /* 1 solution */
                        keep_right_part = true;
                    }
                    else  /* ! check_right */
                    {
                        U8_TRACE_INFO( "0." );
                        /* rect_b contains rect_a, result is empty */
                    }
                }
            }
        }

        if ( keep_top_part )
        {
            assert ( check_top );
            U8_TRACE_INFO( "-top-" );
            geometry_rectangle_init ( this_, a_left, a_top, geometry_rectangle_get_width( rect_a ), b_top-a_top );
        }
        else if ( keep_bottom_part )
        {
            assert ( check_bottom );
            U8_TRACE_INFO( "-bottom-" );
            geometry_rectangle_init ( this_, a_left, b_bottom, geometry_rectangle_get_width( rect_a ), a_bottom-b_bottom );
        }
        else if ( keep_left_part )
        {
            assert ( check_left );
            U8_TRACE_INFO( "-left-" );
            geometry_rectangle_init ( this_, a_left, a_top, b_left-a_left, geometry_rectangle_get_height( rect_a ) );
        }
        else if ( keep_right_part )
        {
            assert ( check_right );
            U8_TRACE_INFO( "-right-" );
            geometry_rectangle_init ( this_, b_right, a_top, a_right-b_right, geometry_rectangle_get_height( rect_a ) );
        }
        else
        {
            U8_TRACE_INFO( "-none-" );
            geometry_rectangle_init ( this_, a_left, a_top, 0.0, 0.0 );
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

int geometry_rectangle_init_by_difference_at_pivot( geometry_rectangle_t *this_,
                                                    const geometry_rectangle_t *moon,
                                                    const geometry_rectangle_t *shadow,
                                                    const geometry_point_t *pivot_point )
{
    U8_TRACE_BEGIN();
    assert( NULL != moon );
    assert( NULL != shadow );
    assert( NULL != pivot_point );

    int result = 0;

    const double moon_left = geometry_rectangle_get_left( moon );
    const double moon_right = geometry_rectangle_get_right( moon );
    const double moon_top = geometry_rectangle_get_top( moon );
    const double moon_bottom = geometry_rectangle_get_bottom( moon );
    const double moon_width = geometry_rectangle_get_width( moon );
    const double moon_height = geometry_rectangle_get_height( moon );

    geometry_rectangle_t shadow_intersect;
    const int no_overlap = geometry_rectangle_init_by_intersect( &shadow_intersect, moon, shadow );
    if ( no_overlap == 0 )
    {
        const double shadow_left = geometry_rectangle_get_left( &shadow_intersect );
        const double shadow_top = geometry_rectangle_get_top( &shadow_intersect );
        const double shadow_right = geometry_rectangle_get_right( &shadow_intersect );
        const double shadow_bottom = geometry_rectangle_get_bottom( &shadow_intersect );
        const double left_width = shadow_left - moon_left;
        const double top_height = shadow_top - moon_top;
        const double right_width = moon_right - shadow_right;
        const double bottom_height = moon_bottom - shadow_bottom;

        geometry_direction_t keep;
        if ( geometry_point_get_x( pivot_point ) <= shadow_left )
        {
            if ( geometry_point_get_y( pivot_point ) <= shadow_top )
            {
                keep = ( left_width > top_height ) ? GEOMETRY_DIRECTION_LEFT : GEOMETRY_DIRECTION_UP;
            }
            else if ( geometry_point_get_y( pivot_point ) >= shadow_bottom )
            {
                keep = ( left_width > bottom_height ) ? GEOMETRY_DIRECTION_LEFT : GEOMETRY_DIRECTION_DOWN;
            }
            else
            {
                keep = GEOMETRY_DIRECTION_LEFT;
            }
        }
        else if ( geometry_point_get_x( pivot_point ) >= shadow_right )
        {
            if ( geometry_point_get_y( pivot_point ) <= shadow_top )
            {
                keep = ( right_width > top_height ) ? GEOMETRY_DIRECTION_RIGHT : GEOMETRY_DIRECTION_UP;
            }
            else if ( geometry_point_get_y( pivot_point ) >= shadow_bottom )
            {
                keep = ( right_width > bottom_height ) ? GEOMETRY_DIRECTION_RIGHT : GEOMETRY_DIRECTION_DOWN;
            }
            else
            {
                keep = GEOMETRY_DIRECTION_RIGHT;
            }
        }
        else
        {
            if ( geometry_point_get_y( pivot_point ) <= shadow_top )
            {
                keep = GEOMETRY_DIRECTION_UP;
            }
            else if ( geometry_point_get_y( pivot_point ) >= shadow_bottom )
            {
                keep = GEOMETRY_DIRECTION_DOWN;
            }
            else
            {
                if ( geometry_point_get_x( pivot_point ) < geometry_rectangle_get_center_x( &shadow_intersect ) )
                {
                    if ( geometry_point_get_y( pivot_point ) < geometry_rectangle_get_center_y( &shadow_intersect ) )
                    {
                        keep = ( left_width > top_height ) ? GEOMETRY_DIRECTION_LEFT : GEOMETRY_DIRECTION_UP;
                    }
                    else
                    {
                        keep = ( left_width > bottom_height ) ? GEOMETRY_DIRECTION_LEFT : GEOMETRY_DIRECTION_DOWN;
                    }
                }
                else
                {
                    if ( geometry_point_get_y( pivot_point ) < geometry_rectangle_get_center_y( &shadow_intersect ) )
                    {
                        keep = ( right_width > top_height ) ? GEOMETRY_DIRECTION_RIGHT : GEOMETRY_DIRECTION_UP;
                    }
                    else
                    {
                        keep = ( right_width > bottom_height ) ? GEOMETRY_DIRECTION_RIGHT : GEOMETRY_DIRECTION_DOWN;
                    }
                }
            }
        }

        if ( keep == GEOMETRY_DIRECTION_LEFT )
        {
            /* take left side of shadow_intersect */
            geometry_rectangle_init( this_, moon_left, moon_top, shadow_left - moon_left, moon_height );
            U8_TRACE_INFO_INT_INT( "left: w*h", left_width, moon_height );
        }
        else if ( keep == GEOMETRY_DIRECTION_RIGHT )
        {
            /* take right side of shadow_intersect */
            geometry_rectangle_init( this_, shadow_right, moon_top, moon_right - shadow_right, moon_height );
            U8_TRACE_INFO_INT_INT( "right: w*h", right_width, moon_height);
        }
        else if ( keep == GEOMETRY_DIRECTION_DOWN )
        {
            /* take bottom side of shadow_intersect */
            geometry_rectangle_init( this_, moon_left, shadow_bottom, moon_width, moon_bottom - shadow_bottom );
            U8_TRACE_INFO_INT_INT( "bottom: w*h", moon_width, bottom_height);
        }
        else
        {
            /* take top side of shadow_intersect */
            geometry_rectangle_init( this_, moon_left, moon_top, moon_width, shadow_top - moon_top );
            U8_TRACE_INFO_INT_INT( "top: w*h", moon_width, top_height);
        }
    }
    else
    {
        *this_ = *moon;
        U8_TRACE_INFO_INT_INT( "no intersect: w*h", moon_width, moon_height );
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2020-2025 Andreas Warnke

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
