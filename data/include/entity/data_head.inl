/* File: data_head.inl; Copyright and License: see below */

#include "entity/data_id.h"
#include <assert.h>

static inline u8_error_t data_head_init_new ( data_head_t *this_,
                                              const char* head_key,
                                              const char* head_value )
{
    assert( NULL != head_key );
    assert( NULL != head_value );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = DATA_ROW_VOID;

    (*this_).key = utf8stringbuf_new( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    strerr = utf8stringbuf_copy_str( &((*this_).key), head_key );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).value = utf8stringbuf_new( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    strerr = utf8stringbuf_copy_str( &((*this_).value), head_value );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    return result;
}

static inline u8_error_t data_head_init ( data_head_t *this_,
                                          data_row_t head_id,
                                          const char* head_key,
                                          const char* head_value )
{
    assert( NULL != head_key );
    assert( NULL != head_value );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = head_id;

    (*this_).key = utf8stringbuf_new( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    strerr = utf8stringbuf_copy_str( &((*this_).key), head_key );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).value = utf8stringbuf_new( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    strerr = utf8stringbuf_copy_str( &((*this_).value), head_value );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    return result;
}

static inline void data_head_copy ( data_head_t *this_, const data_head_t *original )
{
    assert( NULL != original );

    (*this_) = (*original);
    /* repair the overwritten pointers */
    (*this_).key = utf8stringbuf_new( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    (*this_).value = utf8stringbuf_new( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
}

static inline void data_head_replace ( data_head_t *this_, const data_head_t *that )
{
    assert( NULL != that );

    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).key = utf8stringbuf_new( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    (*this_).value = utf8stringbuf_new( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
}

static inline void data_head_destroy ( data_head_t *this_ )
{
    (*this_).id = DATA_ROW_VOID;
}

static inline data_row_t data_head_get_row_id ( const data_head_t *this_ )
{
    return (*this_).id;
}

static inline void data_head_set_row_id ( data_head_t *this_, data_row_t id )
{
    (*this_).id = id;
}

static inline const char *data_head_get_key_const ( const data_head_t *this_ )
{
    return utf8stringbuf_get_string( &((*this_).key) );
}

static inline u8_error_t data_head_set_key ( data_head_t *this_, const char *key )
{
    assert( NULL != key );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( &((*this_).key), key );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_head_get_value_const ( const data_head_t *this_ )
{
    return utf8stringbuf_get_string( &((*this_).value) );
}

static inline bool data_head_has_value ( const data_head_t *this_ )
{
    return ( ! utf8stringbuf_equals_str( &((*this_).value), "" ) );
}

static inline u8_error_t data_head_set_value ( data_head_t *this_, const char *value )
{
    assert( NULL != value );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( &((*this_).value), value );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline bool data_head_is_valid ( const data_head_t *this_ )
{
    return ( DATA_ROW_VOID != (*this_).id );
}

static inline void data_head_trace ( const data_head_t *this_ )
{
    U8_TRACE_INFO( "data_head_t" );
    U8_TRACE_INFO_INT( "- id:", (*this_).id );
    U8_TRACE_INFO_STR( "- key:", utf8stringbuf_get_string( &((*this_).key)) );
    U8_TRACE_INFO_STR( "- value:", utf8stringbuf_get_string( &((*this_).value)) );
}


/*
Copyright 2023-2024 Andreas Warnke

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
