/* File: data_diagram.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void data_diagram_init_empty ( data_diagram_t *this_ )
{
    (*this_).id = DATA_ROW_VOID;
    (*this_).parent_id = DATA_ROW_VOID;
    (*this_).diagram_type = DATA_DIAGRAM_TYPE_LIST;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    utf8stringbuf_clear( (*this_).stereotype );
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    utf8stringbuf_clear( (*this_).name );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).list_order = 0;
    (*this_).display_flags = DATA_DIAGRAM_FLAG_NONE;
    data_uuid_init_new( &((*this_).uuid) );
}

static inline void data_diagram_reinit_empty ( data_diagram_t *this_ )
{
    /* data_diagram_destroy( this_ );  -- not necessary */
    data_diagram_init_empty( this_ );
}

static inline u8_error_t data_diagram_init_new ( data_diagram_t *this_,
                                                 data_row_t parent_diagram_id,
                                                 data_diagram_type_t diagram_type,
                                                 const char* stereotype,
                                                 const char* name,
                                                 const char* description,
                                                 int32_t list_order,
                                                 data_diagram_flag_t display_flags )
{
    assert( NULL != stereotype );
    assert( NULL != name );
    assert( NULL != description );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = DATA_ROW_VOID;
    (*this_).parent_id = parent_diagram_id;
    (*this_).diagram_type = diagram_type;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).stereotype, stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).list_order = list_order;
    (*this_).display_flags = display_flags;
    data_uuid_init_new( &((*this_).uuid) );

    return result;
}

static inline u8_error_t data_diagram_init ( data_diagram_t *this_,
                                             data_row_t diagram_id,
                                             data_row_t parent_diagram_id,
                                             data_diagram_type_t diagram_type,
                                             const char* stereotype,
                                             const char* name,
                                             const char* description,
                                             int32_t list_order,
                                             data_diagram_flag_t display_flags,
                                             const char* uuid )
{
    assert( NULL != stereotype );
    assert( NULL != name );
    assert( NULL != description );
    assert( NULL != uuid );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = diagram_id;
    (*this_).parent_id = parent_diagram_id;
    (*this_).diagram_type = diagram_type;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).stereotype, stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    (*this_).list_order = list_order;
    (*this_).display_flags = display_flags;
    result |= data_uuid_init( &((*this_).uuid), uuid );

    return result;
}

static inline void data_diagram_copy ( data_diagram_t *this_, const data_diagram_t *original )
{
    assert( NULL != original );

    (*this_) = (*original);
    /* repair the overwritten pointers */
    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    data_uuid_copy( &((*this_).uuid), &((*original).uuid) );
}

static inline void data_diagram_replace ( data_diagram_t *this_, const data_diagram_t *that )
{
    assert( NULL != that );

    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    data_uuid_replace( &((*this_).uuid), &((*that).uuid) );
}

static inline void data_diagram_destroy ( data_diagram_t *this_ )
{
    (*this_).id = DATA_ROW_VOID;
    (*this_).parent_id = DATA_ROW_VOID;
    data_uuid_destroy( &((*this_).uuid) );
}

static inline void data_diagram_trace ( const data_diagram_t *this_ )
{
    U8_TRACE_INFO( "data_diagram_t" );
    U8_TRACE_INFO_INT( "- id:", (*this_).id );
    U8_TRACE_INFO_INT( "- parent_id:", (*this_).parent_id );
    U8_TRACE_INFO_INT( "- diagram_type:", (*this_).diagram_type );
    U8_TRACE_INFO_STR( "- stereotype:", utf8stringbuf_get_string((*this_).stereotype) );
    U8_TRACE_INFO_STR( "- name:", utf8stringbuf_get_string((*this_).name) );
    U8_TRACE_INFO_STR( "- description:", utf8stringbuf_get_string((*this_).description) );
    U8_TRACE_INFO_INT( "- list_order:", (*this_).list_order );
    U8_TRACE_INFO_HEX( "- display_flags:", (*this_).display_flags );
    U8_TRACE_INFO_STR( "- uuid:", data_uuid_get_string( &((*this_).uuid) ) );
}

static inline data_row_t data_diagram_get_row_id ( const data_diagram_t *this_ )
{
    return (*this_).id;
}

static inline void data_diagram_set_row_id ( data_diagram_t *this_, data_row_t id )
{
    (*this_).id = id;
}

static inline data_id_t data_diagram_get_data_id ( const data_diagram_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_DIAGRAM, (*this_).id );
    return result;
}

static inline data_row_t data_diagram_get_parent_row_id ( const data_diagram_t *this_ )
{
    return (*this_).parent_id;
}

static inline void data_diagram_set_parent_row_id ( data_diagram_t *this_, data_row_t parent_id )
{
    (*this_).parent_id = parent_id;
}

static inline data_id_t data_diagram_get_parent_data_id ( const data_diagram_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_DIAGRAM, (*this_).parent_id );
    return result;
}

static inline data_diagram_type_t data_diagram_get_diagram_type ( const data_diagram_t *this_ )
{
    return (*this_).diagram_type;
}

static inline void data_diagram_set_diagram_type ( data_diagram_t *this_, data_diagram_type_t diagram_type )
{
    (*this_).diagram_type = diagram_type;
}

static inline const char *data_diagram_get_stereotype_const ( const data_diagram_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).stereotype );
}

static inline bool data_diagram_has_stereotype ( const data_diagram_t *this_ )
{
    return ( ! utf8stringbuf_equals_str( (*this_).stereotype, "" ) );
}

static inline u8_error_t data_diagram_set_stereotype ( data_diagram_t *this_, const char *stereotype )
{
    assert( NULL != stereotype );

    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).stereotype, stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_diagram_get_name_const ( const data_diagram_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).name );
}

static inline u8_error_t data_diagram_set_name ( data_diagram_t *this_, const char *name )
{
    assert( NULL != name );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_diagram_get_description_const ( const data_diagram_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).description );
}

static inline u8_error_t data_diagram_set_description ( data_diagram_t *this_, const char *description )
{
    assert( NULL != description );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline u8_error_t data_diagram_append_description ( data_diagram_t *this_, const char *description )
{
    assert( NULL != description );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_append_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_append_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline int32_t data_diagram_get_list_order ( const data_diagram_t *this_ )
{
    return (*this_).list_order;
}

static inline void data_diagram_set_list_order ( data_diagram_t *this_, int32_t list_order )
{
    (*this_).list_order = list_order;
}

static inline data_diagram_flag_t data_diagram_get_display_flags ( const data_diagram_t *this_ )
{
    return (*this_).display_flags;
}

static inline void data_diagram_set_display_flags ( data_diagram_t *this_, data_diagram_flag_t display_flags )
{
    (*this_).display_flags = display_flags;
}

static inline const char *data_diagram_get_uuid_const ( const data_diagram_t *this_ )
{
    return data_uuid_get_string( &((*this_).uuid) );
}

static inline u8_error_t data_diagram_set_uuid ( data_diagram_t *this_, const char *uuid )
{
    assert( NULL != uuid );

    const u8_error_t result = data_uuid_reinit( &((*this_).uuid), uuid );

    return result;
}

static inline bool data_diagram_is_valid ( const data_diagram_t *this_ )
{
    return ( DATA_ROW_VOID != (*this_).id );
}


/*
Copyright 2016-2024 Andreas Warnke

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
