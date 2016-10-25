/* File: geometry_non_linear_scale.c; Copyright and License: see below */

#define __STDC_LIMIT_MACROS
#include "trace.h"
#include <stdint.h>

void geometry_non_linear_scale_init ( geometry_non_linear_scale_t *this_, double lower_bound, double upper_bound )
{
    assert( lower_bound < upper_bound );
    (*this_).num_points = 2;
    (*this_).location[0] = lower_bound;
    (*this_).order[0] = INT32_MIN;
    (*this_).location[1] = upper_bound;
    (*this_).order[1] = INT32_MAX;
}

void geometry_non_linear_scale_trace ( geometry_non_linear_scale_t *this_ )
{
    assert( (*this_).num_points <= GEOMETRY_NON_LINEAR_SCALE_MAX_POINTS );
    TRACE_INFO( "geometry_non_linear_scale_t" );
    TRACE_INFO_INT( "- num_points:", (*this_).num_points );
    for ( uint32_t pos = 0; pos < (*this_).num_points; pos ++ )
    {
        TRACE_INFO_INT_INT( "- location,order:", (int)((*this_).location[pos]), (*this_).order[pos] );
    }
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
