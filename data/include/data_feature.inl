/* File: data_feature.inl; Copyright and License: see below */

#include "data_id.h"
#include <assert.h>

static inline void data_feature_init_empty ( data_feature_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).classifier_id = DATA_ID_VOID_ID;
    (*this_).main_type = DATA_FEATURE_TYPE_PROPERTY;

    (*this_).key = utf8stringbuf_init( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    utf8stringbuf_clear( (*this_).key );
    (*this_).value = utf8stringbuf_init( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    utf8stringbuf_clear( (*this_).value );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).list_order = 0;
}

static inline void data_feature_reinit_empty ( data_feature_t *this_ )
{
    /* data_feature_destroy( this_ );  -- not necessary */
    data_feature_init_empty( this_ );
}

static inline data_error_t data_feature_init ( data_feature_t *this_,
                                               int64_t feature_id,
                                               data_feature_type_t feature_main_type,
                                               int64_t classifier_id,
                                               const char* feature_key,
                                               const char* feature_value,
                                               const char* feature_description,
                                               int32_t list_order )
{
    assert( NULL != feature_key );
    assert( NULL != feature_value );
    assert( NULL != feature_description );
    utf8error_t strerr;
    data_error_t result = DATA_ERROR_NONE;

    (*this_).id = feature_id;
    (*this_).classifier_id = classifier_id;
    (*this_).main_type = feature_main_type;

    (*this_).key = utf8stringbuf_init( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).key, feature_key );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).value = utf8stringbuf_init( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).value, feature_value );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, feature_description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).list_order = list_order;

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
}

static inline void data_feature_replace ( data_feature_t *this_, const data_feature_t *that )
{
    assert( NULL != that );

    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).key = utf8stringbuf_init( sizeof((*this_).private_key_buffer), (*this_).private_key_buffer );
    (*this_).value = utf8stringbuf_init( sizeof((*this_).private_value_buffer), (*this_).private_value_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
}

static inline void data_feature_destroy ( data_feature_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
}

static inline int64_t data_feature_get_id ( const data_feature_t *this_ )
{
    return (*this_).id;
}

static inline void data_feature_set_id ( data_feature_t *this_, int64_t id )
{
    (*this_).id = id;
}

static inline data_id_t data_feature_get_data_id ( const data_feature_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_FEATURE, (*this_).id );
    return result;
}

static inline int64_t data_feature_get_classifier_id ( const data_feature_t *this_ )
{
    return (*this_).classifier_id;
}

static inline void data_feature_set_classifier_id ( data_feature_t *this_, int64_t classifier_id )
{
    (*this_).classifier_id = classifier_id;
}

static inline data_feature_type_t data_feature_get_main_type ( const data_feature_t *this_ )
{
    return (*this_).main_type;
}

static inline void data_feature_set_main_type ( data_feature_t *this_, data_feature_type_t main_type )
{
    (*this_).main_type = main_type;
}

static inline const char *data_feature_get_key_ptr ( const data_feature_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).key );
}

static inline utf8stringbuf_t data_feature_get_key_buf_ptr ( data_feature_t *this_ )
{
    return (*this_).key;
}

static inline data_error_t data_feature_set_key ( data_feature_t *this_, const char *key )
{
    assert( NULL != key );
    data_error_t result = DATA_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).key, key );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_feature_get_value_ptr ( const data_feature_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).value );
}

static inline utf8stringbuf_t data_feature_get_value_buf_ptr ( data_feature_t *this_ )
{
    return (*this_).value;
}

static inline bool data_feature_has_value ( const data_feature_t *this_ )
{
    return ( ! utf8stringbuf_equals_str( (*this_).value, "" ) );
}

static inline data_error_t data_feature_set_value ( data_feature_t *this_, const char *value )
{
    assert( NULL != value );
    data_error_t result = DATA_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).value, value );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_feature_get_description_ptr ( const data_feature_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).description );
}

static inline utf8stringbuf_t data_feature_get_description_buf_ptr ( data_feature_t *this_ )
{
    return (*this_).description;
}

static inline data_error_t data_feature_set_description ( data_feature_t *this_, const char *description )
{
    assert( NULL != description );
    data_error_t result = DATA_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
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

static inline bool data_feature_is_valid ( const data_feature_t *this_ )
{
    return ( DATA_ID_VOID_ID != (*this_).id );
}

static inline void data_feature_trace ( const data_feature_t *this_ )
{
    TRACE_INFO( "data_feature_t" );
    TRACE_INFO_INT( "- id:", (*this_).id );
    TRACE_INFO_INT( "- main_type:", (*this_).main_type );
    TRACE_INFO_INT( "- classifier_id:", (*this_).classifier_id );
    TRACE_INFO_STR( "- key:", utf8stringbuf_get_string((*this_).key) );
    TRACE_INFO_STR( "- value:", utf8stringbuf_get_string((*this_).value) );
    TRACE_INFO_STR( "- description:", utf8stringbuf_get_string((*this_).description) );
    TRACE_INFO_INT( "- list_order:", (*this_).list_order );
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
