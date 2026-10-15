/* File: data_enum_def.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8string.h"

static inline void data_enum_def_init ( data_enum_def_t *this_, int value, const char* name )
{
    (*this_).value = value;
    (*this_).name = name;
}

static inline void data_enum_def_destroy ( data_enum_def_t *this_ )
{
    *this_ = DATA_ENUM_DEF_VOID;
}

static inline int data_enum_def_get_value ( const data_enum_def_t *this_ )
{
    return (*this_).value;
}

static inline const char* data_enum_def_get_name ( const data_enum_def_t *this_ )
{
    return (*this_).name;
}

static inline void data_enum_def_trace ( const data_enum_def_t *this_ )
{
    U8_TRACE_INFO_INT_STR( "data_enum_def_t", (*this_).value, (*this_).name );
}

static inline bool data_enum_def_is_valid ( const data_enum_def_t *this_ )
{
    return ( -1 != (*this_).value );
}


/*
Copyright 2026-2026 Andreas Warnke

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
