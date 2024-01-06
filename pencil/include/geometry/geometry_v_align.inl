/* File: geometry_v_align.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"

static inline double geometry_v_align_get_top ( const geometry_v_align_t *this_, double height, double reference_top, double reference_height )
{
    double top;

    switch ( *this_ )
    {
        case GEOMETRY_V_ALIGN_TOP:
        {
            top = reference_top;
        }
        break;

        case GEOMETRY_V_ALIGN_CENTER:
        {
            top = reference_top + 0.5 * ( reference_height - height );
        }
        break;

        case GEOMETRY_V_ALIGN_BOTTOM:
        {
            top = reference_top + reference_height - height;
        }
        break;

        default:
        {
            U8_LOG_ERROR("unknown geometry_v_align_t in geometry_v_align_get_top()");
            assert(0);
            top = 0.0;
        }
        break;
    }

    return top;
}


/*
Copyright 2017-2024 Andreas Warnke

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
