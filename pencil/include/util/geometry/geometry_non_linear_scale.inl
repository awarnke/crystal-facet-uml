/* File: geometry_non_linear_scale.inl; Copyright and License: see below */

#include "trace.h"
#include <assert.h>
#include <stdbool.h>

static inline void geometry_non_linear_scale_reinit ( geometry_non_linear_scale_t *this_, double lower_bound, double upper_bound )
{
    geometry_non_linear_scale_init( this_, lower_bound, upper_bound );
}

static inline void geometry_non_linear_scale_destroy ( geometry_non_linear_scale_t *this_ )
{
}

static inline double geometry_non_linear_scale_get_location ( geometry_non_linear_scale_t *this_, int32_t order )
{
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    double result = (*this_).location[(*this_).num_points];
    bool found;

    found = false;
    if ( order <= (*this_).order[0] )
    {
        found = true;
        result = (*this_).location[0];
        TRACE_INFO_INT( "result-%", result*100 );
    }
    for ( uint32_t pos = 1; ( pos < (*this_).num_points ) && ( ! found ) ; pos ++ )
    {
        if ( order < (*this_).order[pos] )
        {
            found = true;
            double loc_interval_width = (*this_).location[pos] - (*this_).location[pos-1];
            int32_t ord_interval_width = (*this_).order[pos] - (*this_).order[pos-1];
            if ( ord_interval_width == 0 )
            {
                result = (*this_).location[pos-1];  /* prevent division by zero */
            }
            else
            {
                result = (*this_).location[pos-1] + ( loc_interval_width * (double)( order - (*this_).order[pos-1] ) / (double) ord_interval_width );
            }
            TRACE_INFO_INT( "interval id [i-1,i]:", pos );
            TRACE_INFO_INT_INT( "interval [i-1,i]:", (*this_).location[pos-1], (*this_).location[pos] );
            TRACE_INFO_INT( "result-%", result*100 );
        }
        else if ( order == (*this_).order[pos] )
        {
            found = true;
            result = (*this_).location[pos];
            TRACE_INFO_INT( "result-%", result*100 );
        }
    }

    return result;
}

static inline int32_t geometry_non_linear_scale_get_order ( geometry_non_linear_scale_t *this_, double location )
{
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    int32_t result = (*this_).order[(*this_).num_points];
    bool found;

    found = false;
    if ( location <= (*this_).location[0] )
    {
        found = true;
        result = (*this_).order[0];
        TRACE_INFO_INT( "result", result );
    }
    for ( uint32_t pos = 1; ( pos < (*this_).num_points ) && ( ! found ) ; pos ++ )
    {
        if ( location <= (*this_).location[pos] )
        {
            found = true;
            double loc_interval_width = (*this_).location[pos] - (*this_).location[pos-1];
            uint32_t ord_interval_width = (*this_).order[pos] - (*this_).order[pos-1];
            if ( ( loc_interval_width > -0.000000001 ) && ( loc_interval_width < 0.000000001 ) )
            {
                result = (*this_).order[pos-1];  /* prevent division by zero */
            }
            else
            {
                result = (*this_).order[pos-1] + (int32_t)(( ord_interval_width * ( location - (*this_).location[pos-1] )) / loc_interval_width);
            }
            TRACE_INFO_INT_INT( "interval id, width [i-1,i]:", pos, ord_interval_width );
            TRACE_INFO_INT_INT( "interval [i-1,i]:", (*this_).order[pos-1], (*this_).order[pos] );
            TRACE_INFO_INT( "result", result );
        }
    }

    return result;
}


/*
Copyright 2016-2016 Andreas Warnke

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
