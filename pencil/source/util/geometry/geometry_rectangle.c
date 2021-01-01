/* File: geometry_rectangle.c; Copyright and License: see below */

#include "util/geometry/geometry_rectangle.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>
#include <math.h>

int geometry_rectangle_init_by_difference ( geometry_rectangle_t *this_,
                                            const geometry_rectangle_t *rect_a,
                                            const geometry_rectangle_t *rect_b )
{
    TRACE_BEGIN();
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
                        TRACE_INFO( "to.bo.le.ri." );
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
                        TRACE_INFO( "to.bo.le." );
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
                        TRACE_INFO( "to.bo.ri." );
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
                        TRACE_INFO( "to.bo." );
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
                        TRACE_INFO( "to.le.ri." );
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
                        TRACE_INFO( "to.le." );
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
                        TRACE_INFO( "to.ri." );
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
                        TRACE_INFO( "to." );
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
                        TRACE_INFO( "bo.le.ri." );
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
                        TRACE_INFO( "bo.le." );
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
                        TRACE_INFO( "bo.ri." );
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
                        TRACE_INFO( "bo." );
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
                        TRACE_INFO( "le.ri." );
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
                        TRACE_INFO( "le." );
                        /* 1 solution */
                        keep_left_part = true;
                    }

                }
                else  /* ! check_left */
                {
                    if ( check_right )
                    {
                        TRACE_INFO( "ri." );
                        /* 1 solution */
                        keep_right_part = true;
                    }
                    else  /* ! check_right */
                    {
                        TRACE_INFO( "0." );
                        /* rect_b contains rect_a, result is empty */
                    }
                }
            }
        }

        if ( keep_top_part )
        {
            assert ( check_top );
            TRACE_INFO( "-top-" );
            geometry_rectangle_init ( this_, a_left, a_top, geometry_rectangle_get_width( rect_a ), b_top-a_top );
        }
        else if ( keep_bottom_part )
        {
            assert ( check_bottom );
            TRACE_INFO( "-bottom-" );
            geometry_rectangle_init ( this_, a_left, b_bottom, geometry_rectangle_get_width( rect_a ), a_bottom-b_bottom );
        }
        else if ( keep_left_part )
        {
            assert ( check_left );
            TRACE_INFO( "-left-" );
            geometry_rectangle_init ( this_, a_left, a_top, b_left-a_left, geometry_rectangle_get_height( rect_a ) );
        }
        else if ( keep_right_part )
        {
            assert ( check_right );
            TRACE_INFO( "-right-" );
            geometry_rectangle_init ( this_, b_right, a_top, a_right-b_right, geometry_rectangle_get_height( rect_a ) );
        }
        else
        {
            TRACE_INFO( "-none-" );
            geometry_rectangle_init ( this_, a_left, a_top, 0.0, 0.0 );
        }
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2020-2021 Andreas Warnke

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
