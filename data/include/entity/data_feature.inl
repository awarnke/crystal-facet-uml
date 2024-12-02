/* File: data_feature.inl; Copyright and License: see below */

#include "entity/data_id.h"
#include <assert.h>

static inline void data_feature_init_empty ( data_feature_t *this_ )
{
    (*this_).id = DATA_ROW_VOID;
    (*this_).classifier_id = DATA_ROW_VOID;
    (*this_).main_type = DATA_FEATURE_TYPE_PROPERTY;

    (*this_).key = utf8stringbuf_init( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    utf8stringbuf_clear( (*this_).key );
    (*this_).value = utf8stringbuf_init( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    utf8stringbuf_clear( (*this_).value );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).list_order = 0;
    data_uuid_init_new( &((*this_).uuid) );
}

static inline void data_feature_reinit_empty ( data_feature_t *this_ )
{
    /* data_feature_destroy( this_ );  -- not necessary */
    data_feature_init_empty( this_ );
}

static inline u8_error_t data_feature_init_new ( data_feature_t *this_,
                                                 data_feature_type_t feature_main_type,
                                                 data_row_t classifier_id,
                                                 const char* feature_key,
                                                 const char* feature_value,
                                                 const char* feature_description,
                                                 int32_t list_order )
{
    assert( NULL != feature_key );
    assert( NULL != feature_value );
    assert( NULL != feature_description );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = DATA_ROW_VOID;
    (*this_).classifier_id = classifier_id;
    (*this_).main_type = feature_main_type;

    (*this_).key = utf8stringbuf_init( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).key, feature_key );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).value = utf8stringbuf_init( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).value, feature_value );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, feature_description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).list_order = list_order;
    data_uuid_init_new( &((*this_).uuid) );

    return result;
}

static inline u8_error_t data_feature_init ( data_feature_t *this_,
                                             data_row_t feature_id,
                                             data_feature_type_t feature_main_type,
                                             data_row_t classifier_id,
                                             const char* feature_key,
                                             const char* feature_value,
                                             const char* feature_description,
                                             int32_t list_order,
                                             const char* uuid )
{
    assert( NULL != feature_key );
    assert( NULL != feature_value );
    assert( NULL != feature_description );
    assert( NULL != uuid );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = feature_id;
    (*this_).classifier_id = classifier_id;
    (*this_).main_type = feature_main_type;

    (*this_).key = utf8stringbuf_init( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).key, feature_key );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).value = utf8stringbuf_init( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).value, feature_value );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, feature_description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).list_order = list_order;
    result |= data_uuid_init( &((*this_).uuid), uuid );

    return result;
}

static inline void data_feature_copy ( data_feature_t *this_, const data_feature_t *original )
{
    assert( NULL != original );

    (*this_) = (*original);
    /* repair the overwritten pointers */
    (*this_).key = utf8stringbuf_init( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    (*this_).value = utf8stringbuf_init( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    data_uuid_copy( &((*this_).uuid), &((*original).uuid) );
}

static inline void data_feature_replace ( data_feature_t *this_, const data_feature_t *that )
{
    assert( NULL != that );

    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).key = utf8stringbuf_init( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    (*this_).value = utf8stringbuf_init( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    data_uuid_replace( &((*this_).uuid), &((*that).uuid) );
}

static inline void data_feature_destroy ( data_feature_t *this_ )
{
    (*this_).id = DATA_ROW_VOID;
    data_uuid_destroy( &((*this_).uuid) );
}

static inline data_row_t data_feature_get_row_id ( const data_feature_t *this_ )
{
    return (*this_).id;
}

static inline void data_feature_set_row_id ( data_feature_t *this_, data_row_t id )
{
    (*this_).id = id;
}

static inline data_id_t data_feature_get_data_id ( const data_feature_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_FEATURE, (*this_).id );
    return result;
}

static inline data_row_t data_feature_get_classifier_row_id ( const data_feature_t *this_ )
{
    return (*this_).classifier_id;
}

static inline void data_feature_set_classifier_row_id ( data_feature_t *this_, data_row_t classifier_id )
{
    (*this_).classifier_id = classifier_id;
}

static inline data_id_t data_feature_get_classifier_data_id ( const data_feature_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_CLASSIFIER, (*this_).classifier_id );
    return result;
}

static inline data_feature_type_t data_feature_get_main_type ( const data_feature_t *this_ )
{
    return (*this_).main_type;
}

static inline void data_feature_set_main_type ( data_feature_t *this_, data_feature_type_t main_type )
{
    (*this_).main_type = main_type;
}

static inline const char *data_feature_get_key_const ( const data_feature_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).key );
}

static inline u8_error_t data_feature_set_key ( data_feature_t *this_, const char *key )
{
    assert( NULL != key );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).key, key );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_feature_get_value_const ( const data_feature_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).value );
}

static inline bool data_feature_has_value ( const data_feature_t *this_ )
{
    return ( ! utf8stringbuf_equals_str( (*this_).value, "" ) );
}

static inline u8_error_t data_feature_set_value ( data_feature_t *this_, const char *value )
{
    assert( NULL != value );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).value, value );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_feature_get_description_const ( const data_feature_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).description );
}

static inline u8_error_t data_feature_set_description ( data_feature_t *this_, const char *description )
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

static inline u8_error_t data_feature_append_description ( data_feature_t *this_, const char *description )
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

static inline int32_t data_feature_get_list_order ( const data_feature_t *this_ )
{
    return (*this_).list_order;
}

static inline void data_feature_set_list_order ( data_feature_t *this_, int32_t list_order )
{
    (*this_).list_order = list_order;
}

static inline const char *data_feature_get_uuid_const ( const data_feature_t *this_ )
{
    return data_uuid_get_string( &((*this_).uuid) );
}

static inline u8_error_t data_feature_set_uuid ( data_feature_t *this_, const char *uuid )
{
    assert( NULL != uuid );

    const u8_error_t result = data_uuid_reinit( &((*this_).uuid), uuid );

    return result;
}

static inline bool data_feature_is_valid ( const data_feature_t *this_ )
{
    return ( DATA_ROW_VOID != (*this_).id );
}

static inline void data_feature_trace ( const data_feature_t *this_ )
{
    U8_TRACE_INFO( "data_feature_t" );
    U8_TRACE_INFO_INT( "- id:", (*this_).id );
    U8_TRACE_INFO_INT( "- main_type:", (*this_).main_type );
    U8_TRACE_INFO_INT( "- classifier_id:", (*this_).classifier_id );
    U8_TRACE_INFO_STR( "- key:", utf8stringbuf_get_string((*this_).key) );
    U8_TRACE_INFO_STR( "- value:", utf8stringbuf_get_string((*this_).value) );
    U8_TRACE_INFO_STR( "- description:", utf8stringbuf_get_string((*this_).description) );
    U8_TRACE_INFO_INT( "- list_order:", (*this_).list_order );
    U8_TRACE_INFO_STR( "- uuid:", data_uuid_get_string( &((*this_).uuid) ) );
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
