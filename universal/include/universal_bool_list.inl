/* File: universal_bool_list.inl; Copyright and License: see below */

#include "trace.h"

static inline void universal_bool_list_init_pair ( universal_bool_list_t *this_, bool first, bool second )
{
    (*this_).list = (first ? 0x1 : 0x0) | (second ? 0x2 : 0x0);
}

static inline void universal_bool_list_reinit_pair ( universal_bool_list_t *this_, bool first, bool second )
{
    (*this_).list = (first ? 0x1 : 0x0) | (second ? 0x2 : 0x0);
}

static inline void universal_bool_list_copy ( universal_bool_list_t *this_, const universal_bool_list_t *original )
{
    (*this_) = (*original);
}

static inline void universal_bool_list_replace ( universal_bool_list_t *this_, const universal_bool_list_t *original )
{
    (*this_) = (*original);
}

static inline void universal_bool_list_init_empty ( universal_bool_list_t *this_ )
{
    (*this_).list = 0;
}

static inline void universal_bool_list_reinit_empty ( universal_bool_list_t *this_ )
{
    (*this_).list = 0;
}

static inline void universal_bool_list_destroy ( universal_bool_list_t *this_ )
{
}

static inline bool universal_bool_list_get_first ( const universal_bool_list_t *this_ )
{
    return ( 0x1 == ( (*this_).list & 0x1 ));
}

static inline bool universal_bool_list_get_second ( const universal_bool_list_t *this_ )
{
    return ( 0x2 == ( (*this_).list & 0x2 ));
}

static inline void universal_bool_list_trace ( const universal_bool_list_t *this_ )
{
    TRACE_INFO( "universal_bool_list_t" );
    TRACE_INFO_HEX( "- list:", (*this_).list );
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
