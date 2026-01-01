/* File: u8_error_info.inl; Copyright and License: see below */

#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void u8_error_info_init_void ( u8_error_info_t *this_ )
{
    (*this_).error = U8_ERROR_NONE;
    (*this_).unit = U8_ERROR_INFO_UNIT_VOID;
    (*this_).position = -1;
    (*this_).name[0] = '\0';
}

static inline void u8_error_info_init ( u8_error_info_t *this_, u8_error_t error )
{
    (*this_).error = error;
    (*this_).unit = U8_ERROR_INFO_UNIT_VOID;
    (*this_).position = -1;
    (*this_).name[0] = '\0';
}

static inline void u8_error_info_init_line ( u8_error_info_t *this_, u8_error_t error, int32_t line )
{
    (*this_).error = error;
    (*this_).unit = U8_ERROR_INFO_UNIT_LINE;
    (*this_).position = line;
    (*this_).name[0] = '\0';
}

static inline void u8_error_info_init_name ( u8_error_info_t *this_, u8_error_t error, const char* name )
{
    (*this_).error = error;
    (*this_).unit = U8_ERROR_INFO_UNIT_NAME;
    (*this_).position = -1;
    utf8stringbuf_t my_strbuf = utf8stringbuf_new( &((*this_).name[0]), U8_ERROR_INFO_MAX_NAME_SIZE );
    utf8stringbuf_copy_str( &my_strbuf, name );
}

static inline void u8_error_info_init_line_name ( u8_error_info_t *this_, u8_error_t error, int32_t line, const char* name )
{
    (*this_).error = error;
    (*this_).unit = U8_ERROR_INFO_UNIT_LINE_NAME;
    (*this_).position = line;
    utf8stringbuf_t my_strbuf = utf8stringbuf_new( &((*this_).name[0]), U8_ERROR_INFO_MAX_NAME_SIZE );
    utf8stringbuf_copy_str( &my_strbuf, name );
}

static inline void u8_error_info_destroy ( u8_error_info_t *this_ )
{
}

static inline bool u8_error_info_is_error ( const u8_error_info_t *this_ )
{
    return ( (*this_).error != U8_ERROR_NONE );
}

static inline u8_error_t u8_error_info_get_error ( const u8_error_info_t *this_ )
{
    return (*this_).error;
}

static inline u8_error_info_unit_t u8_error_info_get_unit ( const u8_error_info_t *this_ )
{
    return (*this_).unit;
}

static inline int32_t u8_error_info_get_line ( const u8_error_info_t *this_ )
{
    return (*this_).position;
}

static inline const char* u8_error_info_get_name ( const u8_error_info_t *this_ )
{
    return &((*this_).name[0]);
}


/*
Copyright 2022-2026 Andreas Warnke

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
