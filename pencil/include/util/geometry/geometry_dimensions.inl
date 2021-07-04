/* File: geometry_dimensions.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <assert.h>
#include <math.h>

static inline void geometry_dimensions_init ( geometry_dimensions_t *this_, double width, double height )
{
    (*this_).width = width;
    (*this_).height = height;
}

static inline void geometry_dimensions_reinit ( geometry_dimensions_t *this_, double width, double height )
{
    (*this_).width = width;
    (*this_).height = height;
}

static inline void geometry_dimensions_copy ( geometry_dimensions_t *this_, const geometry_dimensions_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_dimensions_replace ( geometry_dimensions_t *this_, const geometry_dimensions_t *original )
{
    assert( NULL != original );
    (*this_) = (*original);
}

static inline void geometry_dimensions_init_empty ( geometry_dimensions_t *this_ )
{
    (*this_).width = 0.0;
    (*this_).height = 0.0;
}

static inline void geometry_dimensions_reinit_empty ( geometry_dimensions_t *this_ )
{
    (*this_).width = 0.0;
    (*this_).height = 0.0;
}

static inline void geometry_dimensions_destroy ( geometry_dimensions_t *this_ )
{
}

static inline double geometry_dimensions_get_width ( const geometry_dimensions_t *this_ )
{
    return (*this_).width;
}

static inline double geometry_dimensions_get_height ( const geometry_dimensions_t *this_ )
{
    return (*this_).height;
}

static inline double geometry_dimensions_get_area ( const geometry_dimensions_t *this_ )
{
    return (*this_).width * (*this_).height;
}

static inline bool geometry_dimensions_is_empty ( const geometry_dimensions_t *this_ )
{
    return ( ( (*this_).width < 0.000000001 )||( (*this_).height < 0.000000001 ) );
}

static inline bool geometry_dimensions_can_contain ( const geometry_dimensions_t *this_, const geometry_dimensions_t *that )
{
    assert( NULL != that );

    const bool result
        = ( (*this_).width + 0.000000001 > (*that).width )&&( (*this_).height + 0.000000001 > (*that).height );

    return result;
}

static inline void geometry_dimensions_expand ( geometry_dimensions_t *this_, double delta_width, double delta_height )
{
    (*this_).width += delta_width;
    if ( (*this_).width < 0.0 )
    {
        (*this_).width = 0.0;
    }

    (*this_).height += delta_height;
    if ( (*this_).height < 0.0 )
    {
        (*this_).height = 0.0;
    }
}

static inline void geometry_dimensions_trace ( const geometry_dimensions_t *this_ )
{
    TRACE_INFO( "geometry_dimensions_t" );
    TRACE_INFO_INT( "- width:", (*this_).width );
    TRACE_INFO_INT( "- height:", (*this_).height );
}


/*
Copyright 2019-2021 Andreas Warnke

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
