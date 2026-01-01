/* File: geometry_non_linear_scale.c; Copyright and License: see below */

#define __STDC_LIMIT_MACROS
#include "geometry/geometry_non_linear_scale.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

void geometry_non_linear_scale_init ( geometry_non_linear_scale_t *this_, double lower_bound, double upper_bound )
{
    U8_TRACE_BEGIN();
    assert( lower_bound <= upper_bound );

    (*this_).num_points = 2;
    (*this_).location[0] = lower_bound;
    (*this_).order[0] = INT32_MIN;
    (*this_).location[1] = upper_bound;
    (*this_).order[1] = INT32_MAX;

    U8_TRACE_END();
}

void geometry_non_linear_scale_trace ( const geometry_non_linear_scale_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );

    U8_TRACE_INFO( "geometry_non_linear_scale_t" );
    U8_TRACE_INFO_INT( "- num_points:", (*this_).num_points );
    for ( uint32_t pos = 0; pos < (*this_).num_points; pos ++ )
    {
        U8_TRACE_INFO_INT_INT( "- location-%, order:", (int)((*this_).location[pos]*100.0), (*this_).order[pos] );
    }

    U8_TRACE_END();
}

u8_error_t geometry_non_linear_scale_add_order ( geometry_non_linear_scale_t *this_, int32_t order )
{
    U8_TRACE_BEGIN();
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    assert( (*this_).num_points >= 2 );  /* prevent division by zero */
    u8_error_t result = U8_ERROR_NONE;

    /* check for duplicates and for possible insert position */
    bool duplicate = false;
    uint32_t insert_pos = 0;
    for ( uint32_t pos = 0; pos < (*this_).num_points; pos ++ )
    {
        if ( order > (*this_).order[pos] )
        {
            insert_pos = pos+1;
        }
        else if ( order == (*this_).order[pos] )
        {
            duplicate = true;
            U8_TRACE_INFO_INT( "duplicate:", order );
        }
    }

    /* insert if possible */
    if ( ! duplicate )
    {
        if ( (*this_).num_points < GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS )
        {
            double lower_bound;
            double upper_bound;
            lower_bound = (*this_).location[ 0 ];
            upper_bound = (*this_).location[ (*this_).num_points - 1 ];

            /* insert order value */
            for ( uint32_t pos2 = (*this_).num_points; pos2 > insert_pos; pos2 -- )
            {
                (*this_).order[ pos2 ] = (*this_).order[ pos2-1 ];
            }
            (*this_).order[ insert_pos ] = order;
            (*this_).num_points ++;

            /* re-location in interval */
            double interval_width;
            interval_width = ( upper_bound - lower_bound );
            for ( uint32_t pos3 = 0; pos3 < (*this_).num_points; pos3 ++ )
            {
                (*this_).location[ pos3 ] = lower_bound + ( (double) pos3 * interval_width ) / (double) ( (*this_).num_points - 1 );
            }
        }
        else
        {
            U8_LOG_WARNING( "geometry_non_linear_scale_t has not enough points." );
            result = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
        }
    }

    /* check for monotonic increase of order values and location values */
#ifndef NDEBUG
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    assert( (*this_).num_points >= 2 );
    for ( uint32_t pos2 = 1; pos2 < (*this_).num_points; pos2 ++ )
    {
        assert( (*this_).order[pos2-1] < (*this_).order[pos2] );
        assert( (*this_).location[pos2-1]-0.000000001 < (*this_).location[pos2] );  /* equal locations are possible if lower_bound == upper_bound */
    }
#endif

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2026 Andreas Warnke

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
