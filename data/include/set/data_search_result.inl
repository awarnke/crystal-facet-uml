/* File: data_search_result.inl; Copyright and License: see below */

#include <assert.h>
#include "u8/u8_log.h"

static inline void data_search_result_init_void ( data_search_result_t *this_ )
{
    data_id_init_void( &((*this_).match_object_id) );
    data_type_init_void( &((*this_).match_object_type) );
    (*this_).match_object_name = utf8stringbuf_new( (*this_).private_match_name_buffer, sizeof((*this_).private_match_name_buffer) );
    utf8stringbuf_clear( &((*this_).match_object_name) );

    data_id_init_void( &((*this_).src_classifier_id) );
    data_id_init_void( &((*this_).dst_classifier_id) );
    data_id_init_void( &((*this_).diagram_id) );
}

static inline void data_search_result_init_diagram ( data_search_result_t *this_,
                                                     data_row_t match_id,
                                                     data_diagram_type_t match_type,
                                                     const char* match_name )
{
    assert( NULL != match_name );
    utf8error_t strerr;

    data_id_init( &((*this_).match_object_id), DATA_TABLE_DIAGRAM, match_id );
    data_type_init_diagram( &((*this_).match_object_type), match_type );
    (*this_).match_object_name = utf8stringbuf_new( (*this_).private_match_name_buffer, sizeof((*this_).private_match_name_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).match_object_name), match_name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        /* just the read-only name of a search result - truncation should not matter */
        U8_LOG_ANOMALY_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }

    data_id_init_void( &((*this_).src_classifier_id) );
    data_id_init_void( &((*this_).dst_classifier_id) );
    data_id_init( &((*this_).diagram_id), DATA_TABLE_DIAGRAM, match_id );
}

static inline void data_search_result_init_classifier ( data_search_result_t *this_,
                                                        data_row_t match_id,
                                                        data_classifier_type_t match_type,
                                                        const char* match_name,
                                                        data_row_t diagram_id  )
{
    assert( NULL != match_name );
    utf8error_t strerr;

    data_id_init( &((*this_).match_object_id), DATA_TABLE_CLASSIFIER, match_id );
    data_type_init_classifier( &((*this_).match_object_type), match_type );
    (*this_).match_object_name = utf8stringbuf_new( (*this_).private_match_name_buffer, sizeof((*this_).private_match_name_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).match_object_name), match_name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        /* just the read-only name of a search result - truncation should not matter */
        U8_LOG_ANOMALY_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }

    data_id_init_void( &((*this_).src_classifier_id) );
    data_id_init_void( &((*this_).dst_classifier_id) );
    data_id_init( &((*this_).diagram_id), DATA_TABLE_DIAGRAM, diagram_id );
}

static inline void data_search_result_init_feature ( data_search_result_t *this_,
                                                     data_row_t match_id,
                                                     data_feature_type_t match_type,
                                                     const char* match_name,
                                                     data_row_t classifier_id,
                                                     data_row_t diagram_id )
{
    assert( NULL != match_name );
    utf8error_t strerr;

    data_id_init( &((*this_).match_object_id), DATA_TABLE_FEATURE, match_id );
    data_type_init_feature( &((*this_).match_object_type), match_type );
    (*this_).match_object_name = utf8stringbuf_new( (*this_).private_match_name_buffer, sizeof((*this_).private_match_name_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).match_object_name), match_name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        /* just the read-only name of a search result - truncation should not matter */
        U8_LOG_ANOMALY_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }

    data_id_init( &((*this_).src_classifier_id), DATA_TABLE_CLASSIFIER, classifier_id );
    data_id_init_void( &((*this_).dst_classifier_id) );
    data_id_init( &((*this_).diagram_id), DATA_TABLE_DIAGRAM, diagram_id );
}

static inline void data_search_result_init_relationship ( data_search_result_t *this_,
                                                          data_row_t match_id,
                                                          data_relationship_type_t match_type,
                                                          const char* match_name,
                                                          data_row_t from_classifier_id,
                                                          data_row_t to_classifier_id,
                                                          data_row_t diagram_id )
{
    assert( NULL != match_name );
    utf8error_t strerr;

    data_id_init( &((*this_).match_object_id), DATA_TABLE_RELATIONSHIP, match_id );
    data_type_init_relationship( &((*this_).match_object_type), match_type );
    (*this_).match_object_name = utf8stringbuf_new( (*this_).private_match_name_buffer, sizeof((*this_).private_match_name_buffer) );
    strerr = utf8stringbuf_copy_str( &((*this_).match_object_name), match_name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        /* just the read-only name of a search result - truncation should not matter */
        U8_LOG_ANOMALY_INT( "utf8stringbuf_copy_str() failed:", strerr );
    }

    data_id_init( &((*this_).src_classifier_id), DATA_TABLE_CLASSIFIER, from_classifier_id );
    data_id_init( &((*this_).dst_classifier_id), DATA_TABLE_CLASSIFIER, to_classifier_id );
    data_id_init( &((*this_).diagram_id), DATA_TABLE_DIAGRAM, diagram_id );
}

static inline void data_search_result_copy ( data_search_result_t *this_, const data_search_result_t *original )
{
    assert( NULL != original );

    *this_ = *original;
    /* repair the overwritten pointers */
    (*this_).match_object_name = utf8stringbuf_new( (*this_).private_match_name_buffer, sizeof((*this_).private_match_name_buffer) );
}

static inline void data_search_result_destroy ( data_search_result_t *this_ )
{
    data_id_destroy( &((*this_).match_object_id) );
    data_id_destroy( &((*this_).src_classifier_id) );
    data_id_destroy( &((*this_).dst_classifier_id) );
    data_id_destroy( &((*this_).diagram_id) );
}

static inline data_id_t data_search_result_get_match_id ( const data_search_result_t *this_ )
{
    return (*this_).match_object_id;
}

static inline data_id_t *data_search_result_get_match_id_ptr ( data_search_result_t *this_ )
{
    return &((*this_).match_object_id);
}

static inline const data_id_t *data_search_result_get_match_id_const ( const data_search_result_t *this_ )
{
    return &((*this_).match_object_id);
}

static inline data_type_t data_search_result_get_match_type ( const data_search_result_t *this_ )
{
    return (*this_).match_object_type;
}

static inline const char* data_search_result_get_match_name_const ( const data_search_result_t *this_ )
{
    return utf8stringbuf_get_string( &((*this_).match_object_name) );
}

static inline data_id_t data_search_result_get_src_classifier_id ( const data_search_result_t *this_ )
{
    return (*this_).src_classifier_id;
}

static inline data_id_t *data_search_result_get_src_classifier_id_ptr ( data_search_result_t *this_ )
{
    return &((*this_).src_classifier_id);
}

static inline const data_id_t *data_search_result_get_src_classifier_id_const ( const data_search_result_t *this_ )
{
    return &((*this_).src_classifier_id);
}

static inline data_id_t data_search_result_get_dst_classifier_id ( const data_search_result_t *this_ )
{
    return (*this_).dst_classifier_id;
}

static inline data_id_t *data_search_result_get_dst_classifier_id_ptr ( data_search_result_t *this_ )
{
    return &((*this_).dst_classifier_id);
}

static inline const data_id_t *data_search_result_get_dst_classifier_id_const ( const data_search_result_t *this_ )
{
    return &((*this_).dst_classifier_id);
}

static inline data_id_t data_search_result_get_diagram_id ( const data_search_result_t *this_ )
{
    return (*this_).diagram_id;
}

static inline data_id_t *data_search_result_get_diagram_id_ptr ( data_search_result_t *this_ )
{
    return &((*this_).diagram_id);
}

static inline const data_id_t *data_search_result_get_diagram_id_const ( const data_search_result_t *this_ )
{
    return &((*this_).diagram_id);
}

static inline void data_search_result_trace ( const data_search_result_t *this_ )
{
    U8_TRACE_INFO_STR( "- name:", utf8stringbuf_get_string( &((*this_).match_object_name) ) );
    U8_TRACE_INFO_INT( "- type:", data_type_get_type_as_int( &((*this_).match_object_type) ) );
    data_id_trace( &((*this_).match_object_id) );
    data_id_trace( &((*this_).src_classifier_id) );
    data_id_trace( &((*this_).dst_classifier_id) );
    data_id_trace( &((*this_).diagram_id) );
}

static inline bool data_search_result_is_valid ( const data_search_result_t *this_ )
{
    return data_id_is_valid( &((*this_).match_object_id) );
}


/*
Copyright 2020-2025 Andreas Warnke

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
