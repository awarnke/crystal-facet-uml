/* File: data_classifier.inl; Copyright and License: see below */

#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

static inline void data_classifier_init_empty ( data_classifier_t *this_ )
{
    (*this_).id = DATA_ROW_ID_VOID;
    (*this_).main_type = DATA_CLASSIFIER_TYPE_BLOCK;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    utf8stringbuf_clear( (*this_).stereotype );
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    utf8stringbuf_clear( (*this_).name );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).x_order = 0;
    (*this_).y_order = 0;
    (*this_).list_order = 0;
    data_uuid_init_new( &((*this_).uuid) );
}

static inline void data_classifier_reinit_empty ( data_classifier_t *this_ )
{
    /* data_classifier_destroy( this_ );  -- not necessary */
    data_classifier_init_empty( this_ );
}

static inline u8_error_t data_classifier_init_new ( data_classifier_t *this_,
                                                    data_classifier_type_t main_type,
                                                    const char* stereotype,
                                                    const char* name,
                                                    const char* description,
                                                    int32_t x_order,
                                                    int32_t y_order,
                                                    int32_t list_order )
{
    assert( NULL != stereotype );
    assert( NULL != name );
    assert( NULL != description );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = DATA_ROW_ID_VOID;
    (*this_).main_type = main_type;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).stereotype, stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).x_order = x_order;
    (*this_).y_order = y_order;
    (*this_).list_order = list_order;
    data_uuid_init_new( &((*this_).uuid) );

    return result;
}

static inline u8_error_t data_classifier_init ( data_classifier_t *this_,
                                                data_row_id_t id,
                                                data_classifier_type_t main_type,
                                                const char* stereotype,
                                                const char* name,
                                                const char* description,
                                                int32_t x_order,
                                                int32_t y_order,
                                                int32_t list_order,
                                                const char* uuid )
{
    assert( NULL != stereotype );
    assert( NULL != name );
    assert( NULL != description );
    assert( NULL != uuid );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = id;
    if ( main_type == DATA_CLASSIFIER_TYPE_DEPRECATED_FEATURE )
    {
        main_type = DATA_CLASSIFIER_TYPE_REQUIREMENT;
        TSLOG_ANOMALY_INT( "Classifier type FEATURE is deprecated. Converted to REQUIREMENT. See id C", id );
    }
    (*this_).main_type = main_type;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).stereotype, stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).x_order = x_order;
    (*this_).y_order = y_order;
    (*this_).list_order = list_order;
    result |= data_uuid_init( &((*this_).uuid), uuid );

    return result;
}

static inline u8_error_t data_classifier_reinit ( data_classifier_t *this_,
                                                  data_row_id_t id,
                                                  data_classifier_type_t main_type,
                                                  const char* stereotype,
                                                  const char* name,
                                                  const char* description,
                                                  int32_t x_order,
                                                  int32_t y_order,
                                                  int32_t list_order,
                                                  const char* uuid )
{
    return data_classifier_init( this_, id, main_type, stereotype, name, description, x_order, y_order, list_order, uuid );
}

static inline void data_classifier_copy ( data_classifier_t *this_, const data_classifier_t *original )
{
    assert( NULL != original );

    (*this_) = (*original);
    /* repair the overwritten pointers */
    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    data_uuid_copy( &((*this_).uuid), &((*original).uuid) );
}

static inline void data_classifier_replace ( data_classifier_t *this_, const data_classifier_t *that )
{
    assert( NULL != that );

    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    data_uuid_replace( &((*this_).uuid), &((*that).uuid) );
}

static inline void data_classifier_destroy ( data_classifier_t *this_ )
{
    (*this_).id = DATA_ROW_ID_VOID;
    data_uuid_destroy( &((*this_).uuid) );
}

static inline void data_classifier_trace ( const data_classifier_t *this_ )
{
    TRACE_INFO( "data_classifier_t" );
    TRACE_INFO_INT( "- id:", (*this_).id );
    TRACE_INFO_INT( "- main_type:", (*this_).main_type );
    TRACE_INFO_STR( "- stereotype:", utf8stringbuf_get_string((*this_).stereotype) );
    TRACE_INFO_STR( "- name:", utf8stringbuf_get_string((*this_).name) );
    TRACE_INFO_STR( "- description:", utf8stringbuf_get_string((*this_).description) );
    TRACE_INFO_INT( "- x_order:", (*this_).x_order );
    TRACE_INFO_INT( "- y_order:", (*this_).y_order );
    TRACE_INFO_INT( "- list_order:", (*this_).list_order );
    TRACE_INFO_STR( "- uuid:", data_uuid_get_string( &((*this_).uuid) ) );
}

static inline data_row_id_t data_classifier_get_row_id ( const data_classifier_t *this_ )
{
    return (*this_).id;
}

static inline void data_classifier_set_row_id ( data_classifier_t *this_, data_row_id_t id )
{
    (*this_).id = id;
}

static inline data_id_t data_classifier_get_data_id ( const data_classifier_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_CLASSIFIER, (*this_).id );
    return result;
}

static inline data_classifier_type_t data_classifier_get_main_type ( const data_classifier_t *this_ )
{
    return (*this_).main_type;
}

static inline void data_classifier_set_main_type ( data_classifier_t *this_, data_classifier_type_t main_type )
{
    (*this_).main_type = main_type;
}

static inline const char *data_classifier_get_stereotype_const ( const data_classifier_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).stereotype );
}

static inline bool data_classifier_has_stereotype ( data_classifier_t *this_ )
{
    return ( ! utf8stringbuf_equals_str( (*this_).stereotype, "" ) );
}

static inline u8_error_t data_classifier_set_stereotype ( data_classifier_t *this_, const char *stereotype )
{
    assert( NULL != stereotype );

    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).stereotype, stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_classifier_get_name_const ( const data_classifier_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).name );
}

static inline u8_error_t data_classifier_set_name ( data_classifier_t *this_, const char *name )
{
    assert( NULL != name );

    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_classifier_get_description_const ( const data_classifier_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).description );
}

static inline u8_error_t data_classifier_set_description ( data_classifier_t *this_, const char *description )
{
    assert( NULL != description );

    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline u8_error_t data_classifier_append_description ( data_classifier_t *this_, const char *description )
{
    assert( NULL != description );

    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_append_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_append_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline int32_t data_classifier_get_x_order ( const data_classifier_t *this_ )
{
    return (*this_).x_order;
}

static inline void data_classifier_set_x_order ( data_classifier_t *this_, int32_t x_order )
{
    (*this_).x_order = x_order;
}

static inline int32_t data_classifier_get_y_order ( const data_classifier_t *this_ )
{
    return (*this_).y_order;
}

static inline void data_classifier_set_y_order ( data_classifier_t *this_, int32_t y_order )
{
    (*this_).y_order = y_order;
}

static inline int32_t data_classifier_get_list_order ( const data_classifier_t *this_ )
{
    return (*this_).list_order;
}

static inline void data_classifier_set_list_order ( data_classifier_t *this_, int32_t list_order )
{
    (*this_).list_order = list_order;
}

static inline const char *data_classifier_get_uuid_const ( const data_classifier_t *this_ )
{
    return data_uuid_get_string( &((*this_).uuid) );
}

static inline u8_error_t data_classifier_set_uuid ( data_classifier_t *this_, const char *uuid )
{
    assert( NULL != uuid );

    const u8_error_t result = data_uuid_reinit( &((*this_).uuid), uuid );

    return result;
}

static inline bool data_classifier_is_valid ( const data_classifier_t *this_ )
{
    return ( DATA_ROW_ID_VOID != (*this_).id );
}


/*
Copyright 2016-2021 Andreas Warnke

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
