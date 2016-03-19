/* File: shape_int_rectangle.inl; Copyright and License: see below */

#include "trace.h"

static inline void shape_int_rectangle_init ( shape_int_rectangle_t *this_, int32_t left, int32_t top, int32_t width, int32_t height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline void shape_int_rectangle_destroy ( shape_int_rectangle_t *this_ )
{
}

static inline int32_t shape_int_rectangle_get_left ( shape_int_rectangle_t *this_ )
{
    return (*this_).left;
}

static inline int32_t shape_int_rectangle_get_top ( shape_int_rectangle_t *this_ )
{
    return (*this_).top;
}

static inline int32_t shape_int_rectangle_get_right ( shape_int_rectangle_t *this_ )
{
    return (*this_).left + (*this_).width;
}

static inline int32_t shape_int_rectangle_get_bottom ( shape_int_rectangle_t *this_ )
{
    return (*this_).top + (*this_).height;
}

static inline int32_t shape_int_rectangle_get_width ( shape_int_rectangle_t *this_ )
{
    return (*this_).width;
}

static inline int32_t shape_int_rectangle_get_height ( shape_int_rectangle_t *this_ )
{
    return (*this_).height;
}

static inline void shape_int_rectangle_trace ( shape_int_rectangle_t *this_ )
{
    TRACE_INFO( "shape_int_rectangle_t" );
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
