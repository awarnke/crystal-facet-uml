/* File: universial_int32_pair.inl; Copyright and License: see below */

#include "trace.h"

static inline void universial_int32_pair_init ( universial_int32_pair_t *this_, int32_t first, int32_t second )
{
    (*this_).first = first;
    (*this_).second = second;
}


static inline void universial_int32_pair_reinit ( universial_int32_pair_t *this_, int32_t first, int32_t second )
{
    (*this_).first = first;
    (*this_).second = second;
}

static inline void universial_int32_pair_copy ( universial_int32_pair_t *this_, const universial_int32_pair_t *original )
{
    (*this_) = (*original);
}

static inline void universial_int32_pair_replace ( universial_int32_pair_t *this_, const universial_int32_pair_t *original )
{
    (*this_) = (*original);
}

static inline void universial_int32_pair_init_empty ( universial_int32_pair_t *this_ )
{
    (*this_).first = 0;
    (*this_).second = 0;
}

static inline void universial_int32_pair_reinit_empty ( universial_int32_pair_t *this_ )
{
    (*this_).first = 0;
    (*this_).second = 0;
}

static inline void universial_int32_pair_destroy ( universial_int32_pair_t *this_ )
{
}

static inline int32_t universial_int32_pair_get_first ( universial_int32_pair_t *this_ )
{
    return (*this_).first;
}

static inline int32_t universial_int32_pair_get_second ( universial_int32_pair_t *this_ )
{
    return (*this_).second;
}

static inline void universial_int32_pair_trace ( universial_int32_pair_t *this_ )
{
    TRACE_INFO( "universial_int32_pair_t" );
    TRACE_INFO_INT( "- first:", (*this_).first );
    TRACE_INFO_INT( "- second:", (*this_).second );
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
