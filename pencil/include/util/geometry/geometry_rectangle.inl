/* File: geometry_rectangle.inl; Copyright and License: see below */

#include "trace.h"

static inline void geometry_rectangle_init ( geometry_rectangle_t *this_, double left, double top, double width, double height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline double geometry_rectangle_get_left ( geometry_rectangle_t *this_ )
{
    return (*this_).left;
}

static inline double geometry_rectangle_get_top ( geometry_rectangle_t *this_ )
{
    return (*this_).top;
}

static inline double geometry_rectangle_get_right ( geometry_rectangle_t *this_ )
{
    return (*this_).left + (*this_).width;
}

static inline double geometry_rectangle_get_bottom ( geometry_rectangle_t *this_ )
{
    return (*this_).top + (*this_).height;
}

static inline double geometry_rectangle_get_width ( geometry_rectangle_t *this_ )
{
    return (*this_).width;
}

static inline double geometry_rectangle_get_height ( geometry_rectangle_t *this_ )
{
    return (*this_).height;
}

static inline bool geometry_rectangle_contains ( geometry_rectangle_t *this_, double x, double y )
{
    return (( (*this_).left <= x )&&( x < (*this_).left + (*this_).width )&&( (*this_).top <= y )&&( y < (*this_).top + (*this_).height ));
}

static inline void geometry_rectangle_trace ( geometry_rectangle_t *this_ )
{
    TRACE_INFO( "geometry_rectangle_t" );
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
