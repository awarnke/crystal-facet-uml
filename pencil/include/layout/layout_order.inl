/* File: layout_order.inl; Copyright and License: see below */

#include "trace.h"

static inline void layout_order_init ( layout_order_t *this_, int32_t first, int32_t second )
{
    (*this_).first = first;
    (*this_).second = second;
}


static inline void layout_order_reinit ( layout_order_t *this_, int32_t first, int32_t second )
{
    (*this_).first = first;
    (*this_).second = second;
}

static inline void layout_order_copy ( layout_order_t *this_, const layout_order_t *original )
{
    (*this_) = (*original);
}

static inline void layout_order_replace ( layout_order_t *this_, const layout_order_t *original )
{
    (*this_) = (*original);
}

static inline void layout_order_init_empty ( layout_order_t *this_ )
{
    (*this_).first = 0;
    (*this_).second = 0;
}

static inline void layout_order_reinit_empty ( layout_order_t *this_ )
{
    (*this_).first = 0;
    (*this_).second = 0;
}

static inline void layout_order_destroy ( layout_order_t *this_ )
{
}

static inline int32_t layout_order_get_first ( const layout_order_t *this_ )
{
    return (*this_).first;
}

static inline int32_t layout_order_get_second ( const layout_order_t *this_ )
{
    return (*this_).second;
}

static inline void layout_order_trace ( const layout_order_t *this_ )
{
    TRACE_INFO( "layout_order_t" );
    TRACE_INFO_INT( "- first:", (*this_).first );
    TRACE_INFO_INT( "- second:", (*this_).second );
}


/*
Copyright 2018-2018 Andreas Warnke

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
