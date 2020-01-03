/* File: universal_int32_pair.inl; Copyright and License: see below */

#include "trace.h"

static inline void universal_int32_pair_init ( universal_int32_pair_t *this_, int32_t first, int32_t second )
{
    (*this_).first = first;
    (*this_).second = second;
}


static inline void universal_int32_pair_reinit ( universal_int32_pair_t *this_, int32_t first, int32_t second )
{
    (*this_).first = first;
    (*this_).second = second;
}

static inline void universal_int32_pair_copy ( universal_int32_pair_t *this_, const universal_int32_pair_t *original )
{
    (*this_) = (*original);
}

static inline void universal_int32_pair_replace ( universal_int32_pair_t *this_, const universal_int32_pair_t *original )
{
    (*this_) = (*original);
}

static inline void universal_int32_pair_init_empty ( universal_int32_pair_t *this_ )
{
    (*this_).first = 0;
    (*this_).second = 0;
}

static inline void universal_int32_pair_reinit_empty ( universal_int32_pair_t *this_ )
{
    (*this_).first = 0;
    (*this_).second = 0;
}

static inline void universal_int32_pair_destroy ( universal_int32_pair_t *this_ )
{
}

static inline int32_t universal_int32_pair_get_first ( const universal_int32_pair_t *this_ )
{
    return (*this_).first;
}

static inline int32_t universal_int32_pair_get_second ( const universal_int32_pair_t *this_ )
{
    return (*this_).second;
}

static inline void universal_int32_pair_trace ( const universal_int32_pair_t *this_ )
{
    TRACE_INFO( "universal_int32_pair_t" );
    TRACE_INFO_INT( "- first:", (*this_).first );
    TRACE_INFO_INT( "- second:", (*this_).second );
}


/*
Copyright 2016-2020 Andreas Warnke

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
