/* File: geometry_h_align.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline double geometry_h_align_get_left ( const geometry_h_align_t *this_, double width, double reference_left, double reference_width )
{
    assert( ( *this_ == GEOMETRY_H_ALIGN_LEFT )||( *this_ == GEOMETRY_H_ALIGN_CENTER )||( *this_ == GEOMETRY_H_ALIGN_RIGHT ) );
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

        default:
        case GEOMETRY_H_ALIGN_RIGHT:
        {
            left = reference_left + reference_width - width;
        }
        break;
    }

    return left;
}


/*
Copyright 2017-2026 Andreas Warnke

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
