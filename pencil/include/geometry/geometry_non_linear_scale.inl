/* File: geometry_non_linear_scale.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>
#include <stdbool.h>

static inline void geometry_non_linear_scale_reinit ( geometry_non_linear_scale_t *this_, double lower_bound, double upper_bound )
{
    geometry_non_linear_scale_init( this_, lower_bound, upper_bound );
}

static inline void geometry_non_linear_scale_destroy ( geometry_non_linear_scale_t *this_ )
{
}

static inline double geometry_non_linear_scale_get_location ( const geometry_non_linear_scale_t *this_, int32_t order )
{
    U8_TRACE_BEGIN();
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    assert( (*this_).num_points >= 2 );
    double result = (*this_).location[(*this_).num_points-1];
    bool found;

    found = false;
    if ( order <= (*this_).order[0] )
    {
        found = true;
        result = (*this_).location[0];
        U8_TRACE_INFO_INT( "result-%", result*100 );
    }
    for ( uint32_t pos = 1; ( pos < (*this_).num_points ) && ( ! found ) ; pos ++ )
    {
        if ( order < (*this_).order[pos] )
        {
            found = true;
            const double loc_interval_width = (*this_).location[pos] - (*this_).location[pos-1];
            const uint32_t ord_interval_width = (*this_).order[pos] - (*this_).order[pos-1];
            /* prevent division by zero */
            const uint32_t ord_interval_width_nonzero = ( ord_interval_width < 1 ) ? 1 : ord_interval_width;
            const uint32_t order_interval_offset = order - (*this_).order[pos-1];
            result = (*this_).location[pos-1] + ( loc_interval_width * ((double)order_interval_offset) / ((double)ord_interval_width_nonzero) );

            U8_TRACE_INFO_INT( "interval id:", pos );
            U8_TRACE_INFO_INT_INT( "interval [i-1,i]:", (int32_t)(*this_).location[pos-1], (int32_t)(*this_).location[pos] );
            U8_TRACE_INFO_INT( "result", (int32_t)result );
        }
        else if ( order == (*this_).order[pos] )
        {
            found = true;
            result = (*this_).location[pos];
            U8_TRACE_INFO_INT( "result", (int32_t)result );
        }
    }

    U8_TRACE_END();
    return result;
}

static inline int32_t geometry_non_linear_scale_get_order ( const geometry_non_linear_scale_t *this_, double location, double snap_interval )
{
    U8_TRACE_BEGIN();
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    assert( (*this_).num_points >= 2 );
    assert( snap_interval >= 0.0 );
    int32_t result = (*this_).order[(*this_).num_points-1];
    bool found;

    found = false;
    if ( location <= (*this_).location[0] ) /* on left border, no snap_interval */
    {
        found = true;
        result = (*this_).order[0];
        U8_TRACE_INFO_INT( "result", result );
    }
    for ( uint32_t pos = 1; ( pos < (*this_).num_points ) && ( ! found ) ; pos ++ )
    {
        const double snap_dist = ( pos == ( (*this_).num_points - 1 ) ) ? 0.0 : snap_interval; /* no snap at right border */
        if ( location <= ( (*this_).location[pos] + snap_dist ) )
        {
            if ( location > ( (*this_).location[pos] - snap_dist ) )
            {
                found = true;
                result = (*this_).order[pos];
                U8_TRACE_INFO_INT( "result", result );
            }
            else
            {
                found = true;
                const double loc_interval_width = (*this_).location[pos] - (*this_).location[pos-1];
                /* prevent division by zero */
                const double loc_interval_width_nonzero = ( loc_interval_width < 0.000000001 ) ? 0.000000001 : loc_interval_width;
                const uint32_t ord_interval_width = (*this_).order[pos] - (*this_).order[pos-1];
                const uint32_t order_interval_offset = ((double)ord_interval_width) * ( location - (*this_).location[pos-1] ) / loc_interval_width_nonzero;
                result = (*this_).order[pos-1] + order_interval_offset;

                U8_TRACE_INFO_INT( "interval id:", pos );
                U8_TRACE_INFO_INT_INT( "interval [i-1,i]:", (*this_).order[pos-1], (*this_).order[pos] );
                U8_TRACE_INFO_INT( "result", result );
            }
        }
    }

    U8_TRACE_END();
    return result;
}

static inline bool geometry_non_linear_scale_is_order_on_grid ( const geometry_non_linear_scale_t *this_, int32_t order )
{
    U8_TRACE_BEGIN();
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    assert( (*this_).num_points >= 2 );
    bool found = false;
    bool result = false;

    found = false;
    for ( uint32_t pos = 1; ( pos < ( (*this_).num_points - 1 ) ) && ( ! found ) ; pos ++ )
    {
        if ( order < (*this_).order[pos] )
        {
            found = true;
            result = false;
        }
        else if ( order == (*this_).order[pos] )
        {
            found = true;
            result = true;
        }
    }

    U8_TRACE_END();
    return result;
}

static inline uint32_t geometry_non_linear_scale_get_grid_intervals ( const geometry_non_linear_scale_t *this_ )
{
    assert( (*this_).num_points >= 2 );
    return( (*this_).num_points - 1 );
}

static inline double geometry_non_linear_scale_get_grid_distances ( const geometry_non_linear_scale_t *this_ )
{
    assert( (*this_).num_points >= 2 );
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    return(( (*this_).location[(*this_).num_points-1] - (*this_).location[0] )/( (*this_).num_points - 1 ));
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
