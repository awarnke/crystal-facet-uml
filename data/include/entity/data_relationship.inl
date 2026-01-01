/* File: data_relationship.inl; Copyright and License: see below */

#include "entity/data_id.h"
#include <assert.h>

static inline void data_relationship_init_empty ( data_relationship_t *this_ )
{
    (*this_).id = DATA_ROW_VOID;
    (*this_).from_classifier_id = DATA_ROW_VOID;
    (*this_).from_feature_id = DATA_ROW_VOID;
    (*this_).to_classifier_id = DATA_ROW_VOID;
    (*this_).to_feature_id = DATA_ROW_VOID;
    (*this_).main_type = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;

    (*this_).stereotype = utf8stringbuf_new( (*this_).private_stereotype_buffer, sizeof((*this_).private_stereotype_buffer) );
    utf8stringbuf_clear( &((*this_).stereotype) );
    (*this_).name = utf8stringbuf_new( (*this_).private_name_buffer, sizeof((*this_).private_name_buffer) );
    utf8stringbuf_clear( &((*this_).name) );
    (*this_).description = utf8stringbuf_new( (*this_).private_description_buffer, sizeof((*this_).private_description_buffer) );
    utf8stringbuf_clear( &((*this_).description) );

    (*this_).list_order = 0;
    data_uuid_init_new( &((*this_).uuid) );
}

static inline void data_relationship_reinit_empty ( data_relationship_t *this_ )
{
    /* data_relationship_destroy( this_ );  -- not necessary */
    data_relationship_init_empty( this_ );
}

static inline u8_error_t data_relationship_init_new ( data_relationship_t *this_,
                                                      data_row_t from_classifier_id,
                                                      data_row_t from_feature_id,
                                                      data_row_t to_classifier_id,
                                                      data_row_t to_feature_id,
                                                      data_relationship_type_t relationship_main_type,
                                                      const char* stereotype,
                                                      const char* name,
                                                      const char* description,
                                                      int32_t list_order)
{
    assert( NULL != stereotype );
    assert( NULL != name );
    assert( NULL != description );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = DATA_ROW_VOID;
    (*this_).from_classifier_id = from_classifier_id;
    (*this_).from_feature_id = from_feature_id;
    (*this_).to_classifier_id = to_classifier_id;
    (*this_).to_feature_id = to_feature_id;
    (*this_).main_type = relationship_main_type;

    (*this_).stereotype = utf8stringbuf_new( (*this_).private_stereotype_buffer, sizeof((*this_).private_stereotype_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).stereotype), stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).name = utf8stringbuf_new( (*this_).private_name_buffer, sizeof((*this_).private_name_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).name), name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_new( (*this_).private_description_buffer, sizeof((*this_).private_description_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).description), description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).list_order = list_order;
    data_uuid_init_new( &((*this_).uuid) );

    return result;
}

static inline u8_error_t data_relationship_init ( data_relationship_t *this_,
                                                  data_row_t relationship_id,
                                                  data_row_t from_classifier_id,
                                                  data_row_t from_feature_id,
                                                  data_row_t to_classifier_id,
                                                  data_row_t to_feature_id,
                                                  data_relationship_type_t relationship_main_type,
                                                  const char* stereotype,
                                                  const char* name,
                                                  const char* description,
                                                  int32_t list_order,
                                                  const char* uuid )
{
    assert( NULL != stereotype );
    assert( NULL != name );
    assert( NULL != description );
    assert( NULL != uuid );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).id = relationship_id;
    (*this_).from_classifier_id = from_classifier_id;
    (*this_).from_feature_id = from_feature_id;
    (*this_).to_classifier_id = to_classifier_id;
    (*this_).to_feature_id = to_feature_id;
    (*this_).main_type = relationship_main_type;

    (*this_).stereotype = utf8stringbuf_new( (*this_).private_stereotype_buffer, sizeof((*this_).private_stereotype_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).stereotype), stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).name = utf8stringbuf_new( (*this_).private_name_buffer, sizeof((*this_).private_name_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).name), name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_new( (*this_).private_description_buffer, sizeof((*this_).private_description_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).description), description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).list_order = list_order;
    result |= data_uuid_init( &((*this_).uuid), uuid );

    return result;
}

static inline void data_relationship_copy ( data_relationship_t *this_, const data_relationship_t *original )
{
    assert( NULL != original );

    (*this_) = (*original);
    /* repair the overwritten pointers */
    (*this_).stereotype = utf8stringbuf_new( (*this_).private_stereotype_buffer, sizeof((*this_).private_stereotype_buffer) );
    (*this_).name = utf8stringbuf_new( (*this_).private_name_buffer, sizeof((*this_).private_name_buffer) );
    (*this_).description = utf8stringbuf_new( (*this_).private_description_buffer, sizeof((*this_).private_description_buffer) );
    data_uuid_copy( &((*this_).uuid), &((*original).uuid) );
}

static inline void data_relationship_replace ( data_relationship_t *this_, const data_relationship_t *that )
{
    assert( NULL != that );

    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).stereotype = utf8stringbuf_new( (*this_).private_stereotype_buffer, sizeof((*this_).private_stereotype_buffer) );
    (*this_).name = utf8stringbuf_new( (*this_).private_name_buffer, sizeof((*this_).private_name_buffer) );
    (*this_).description = utf8stringbuf_new( (*this_).private_description_buffer, sizeof((*this_).private_description_buffer) );
    data_uuid_replace( &((*this_).uuid), &((*that).uuid) );
}

static inline void data_relationship_destroy ( data_relationship_t *this_ )
{
    (*this_).id = DATA_ROW_VOID;
    data_uuid_destroy( &((*this_).uuid) );
}

static inline data_row_t data_relationship_get_row_id ( const data_relationship_t *this_ )
{
    return (*this_).id;
}

static inline void data_relationship_set_row_id ( data_relationship_t *this_, data_row_t id )
{
    (*this_).id = id;
}

static inline data_id_t data_relationship_get_data_id ( const data_relationship_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_RELATIONSHIP, (*this_).id );
    return result;
}

static inline data_row_t data_relationship_get_from_classifier_row_id ( const data_relationship_t *this_ )
{
    return (*this_).from_classifier_id;
}

static inline void data_relationship_set_from_classifier_row_id ( data_relationship_t *this_, data_row_t from_classifier_id )
{
    (*this_).from_classifier_id = from_classifier_id;
}

static inline data_id_t data_relationship_get_from_classifier_data_id ( const data_relationship_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_CLASSIFIER, (*this_).from_classifier_id );
    return result;
}

static inline data_row_t data_relationship_get_from_feature_row_id ( const data_relationship_t *this_ )
{
    return (*this_).from_feature_id;
}

static inline void data_relationship_set_from_feature_row_id ( data_relationship_t *this_, data_row_t from_feature_id )
{
    (*this_).from_feature_id = from_feature_id;
}

static inline data_id_t data_relationship_get_from_feature_data_id ( const data_relationship_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_FEATURE, (*this_).from_feature_id );
    return result;
}

static inline data_row_t data_relationship_get_to_classifier_row_id ( const data_relationship_t *this_ )
{
    return (*this_).to_classifier_id;
}

static inline void data_relationship_set_to_classifier_row_id ( data_relationship_t *this_, data_row_t to_classifier_id )
{
    (*this_).to_classifier_id = to_classifier_id;
}

static inline data_id_t data_relationship_get_to_classifier_data_id ( const data_relationship_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_CLASSIFIER, (*this_).to_classifier_id );
    return result;
}

static inline data_row_t data_relationship_get_to_feature_row_id ( const data_relationship_t *this_ )
{
    return (*this_).to_feature_id;
}

static inline void data_relationship_set_to_feature_row_id ( data_relationship_t *this_, data_row_t to_feature_id )
{
    (*this_).to_feature_id = to_feature_id;
}

static inline data_id_t data_relationship_get_to_feature_data_id ( const data_relationship_t *this_ )
{
    data_id_t result;
    data_id_init ( &result, DATA_TABLE_FEATURE, (*this_).to_feature_id );
    return result;
}

static inline data_relationship_type_t data_relationship_get_main_type ( const data_relationship_t *this_ )
{
    return (*this_).main_type;
}

static inline void data_relationship_set_main_type ( data_relationship_t *this_, data_relationship_type_t main_type )
{
    (*this_).main_type = main_type;
}

static inline const char *data_relationship_get_stereotype_const ( const data_relationship_t *this_ )
{
    return utf8stringbuf_get_string( &((*this_).stereotype) );
}

static inline bool data_relationship_has_stereotype ( const data_relationship_t *this_ )
{
    return ( ! utf8stringbuf_equals_str( &((*this_).stereotype), "" ) );
}

static inline u8_error_t data_relationship_set_stereotype ( data_relationship_t *this_, const char *stereotype )
{
    assert( NULL != stereotype );

    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( &((*this_).stereotype), stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_relationship_get_name_const ( const data_relationship_t *this_ )
{
    return utf8stringbuf_get_string( &((*this_).name) );
}

static inline u8_error_t data_relationship_set_name ( data_relationship_t *this_, const char *name )
{
    assert( NULL != name );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( &((*this_).name), name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_relationship_get_description_const ( const data_relationship_t *this_ )
{
    return utf8stringbuf_get_string( &((*this_).description) );
}

static inline u8_error_t data_relationship_set_description ( data_relationship_t *this_, const char *description )
{
    assert( NULL != description );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( &((*this_).description), description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline u8_error_t data_relationship_append_description ( data_relationship_t *this_, const char *description )
{
    assert( NULL != description );
    u8_error_t result = U8_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_append_str( &((*this_).description), description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_HEX( "utf8stringbuf_append_str() failed:", strerr );
        result = U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline int32_t data_relationship_get_list_order ( const data_relationship_t *this_ )
{
    return (*this_).list_order;
}

static inline void data_relationship_set_list_order ( data_relationship_t *this_, int32_t list_order )
{
    (*this_).list_order = list_order;
}

static inline const char *data_relationship_get_uuid_const ( const data_relationship_t *this_ )
{
    return data_uuid_get_string( &((*this_).uuid) );
}

static inline u8_error_t data_relationship_set_uuid ( data_relationship_t *this_, const char *uuid )
{
    assert( NULL != uuid );

    const u8_error_t result = data_uuid_reinit( &((*this_).uuid), uuid );

    return result;
}

static inline bool data_relationship_is_valid ( const data_relationship_t *this_ )
{
    return ( DATA_ROW_VOID != (*this_).id );
}

static inline void data_relationship_trace ( const data_relationship_t *this_ )
{
    U8_TRACE_INFO( "data_relationship_t" );
    U8_TRACE_INFO_INT( "- id:", (*this_).id );
    U8_TRACE_INFO_INT( "- from_classifier_id:", (*this_).from_classifier_id );
    U8_TRACE_INFO_INT( "- from_feature_id:", (*this_).from_feature_id );
    U8_TRACE_INFO_INT( "- to_classifier_id:", (*this_).to_classifier_id );
    U8_TRACE_INFO_INT( "- to_feature_id:", (*this_).to_feature_id );
    U8_TRACE_INFO_INT_STR( "- main_type:", (*this_).main_type, data_relationship_type_get_name( (*this_).main_type ) );
    U8_TRACE_INFO_STR( "- stereotype:", utf8stringbuf_get_string( &((*this_).stereotype)) );
    U8_TRACE_INFO_STR( "- name:", utf8stringbuf_get_string( &((*this_).name)) );
    U8_TRACE_INFO_STR( "- description:", utf8stringbuf_get_string( &((*this_).description)) );
    U8_TRACE_INFO_INT( "- list_order:", (*this_).list_order );
    U8_TRACE_INFO_STR( "- uuid:", data_uuid_get_string( &((*this_).uuid) ) );
}


/*
Copyright 2016-2026 Andreas Warnke

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
