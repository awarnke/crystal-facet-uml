/* File: geometry_h_align.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"

static inline double geometry_h_align_get_left ( const geometry_h_align_t *this_, double width, double reference_left, double reference_width )
{
    double left;

    switch ( *this_ )
    {
        case GEOMETRY_H_ALIGN_LEFT:
        {
            left = reference_left;
        }
        break;

        case GEOMETRY_H_ALIGN_CENTER:
        {
            left = reference_left + 0.5 * ( reference_width - width );
        }
        break;

        case GEOMETRY_H_ALIGN_RIGHT:
        {
            left = reference_left + reference_width - width;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_h_align_t in geometry_h_align_get_left()");
            assert(0);
            left = 0.0;
        }
        break;
    }

    return left;
}


/*
Copyright 2017-2018 Andreas Warnke

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
