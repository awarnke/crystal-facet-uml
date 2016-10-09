/* File: geometry_non_linear_scale.inl; Copyright and License: see below */

#include "trace.h"

static inline void geometry_non_linear_scale_init ( geometry_non_linear_scale_t *this_, double left, double top, double width, double height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline void geometry_non_linear_scale_reinit ( geometry_non_linear_scale_t *this_, double left, double top, double width, double height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline void geometry_non_linear_scale_init_empty ( geometry_non_linear_scale_t *this_ )
{
    (*this_).left = 0.0;
    (*this_).top = 0.0;
    (*this_).width = 0.0;
    (*this_).height = 0.0;
}

static inline void geometry_non_linear_scale_reinit_empty ( geometry_non_linear_scale_t *this_ )
{
    (*this_).left = 0.0;
    (*this_).top = 0.0;
    (*this_).width = 0.0;
    (*this_).height = 0.0;
}

static inline void geometry_non_linear_scale_destroy ( geometry_non_linear_scale_t *this_ )
{
}

static inline void geometry_non_linear_scale_trace ( geometry_non_linear_scale_t *this_ )
{
    TRACE_INFO( "geometry_non_linear_scale_t" );
    TRACE_INFO_INT( "- left:", (*this_).left );
    TRACE_INFO_INT( "- top:", (*this_).top );
    TRACE_INFO_INT( "- width:", (*this_).width );
    TRACE_INFO_INT( "- height:", (*this_).height );
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
