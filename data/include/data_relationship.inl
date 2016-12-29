/* File: data_relationship.inl; Copyright and License: see below */

#include "data_id.h"

static inline void data_relationship_init_empty ( data_relationship_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).from_classifier_id = DATA_ID_VOID_ID;
    (*this_).to_classifier_id = DATA_ID_VOID_ID;
    (*this_).main_type = DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY;

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    utf8stringbuf_clear( (*this_).name );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).list_order = 0;
}

static inline void data_relationship_reinit_empty ( data_relationship_t *this_ )
{
    /* data_relationship_destroy( this_ );  -- not necessary */
    data_relationship_init_empty( this_ );
}

static inline void data_relationship_copy ( data_relationship_t *this_, const data_relationship_t *original )
{
    (*this_) = (*original);
    /* repair the overwritten pointers */
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
}

static inline void data_relationship_replace ( data_relationship_t *this_, const data_relationship_t *that )
{
    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
}

static inline void data_relationship_destroy ( data_relationship_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
}

static inline int64_t data_relationship_get_id ( data_relationship_t *this_ )
{
    return (*this_).id;
}

static inline void data_relationship_set_id ( data_relationship_t *this_, int64_t id )
{
    (*this_).id = id;
}

static inline data_relationship_type_t data_relationship_get_main_type ( data_relationship_t *this_ )
{
    return (*this_).main_type;
}

static inline void data_relationship_set_main_type ( data_relationship_t *this_, data_relationship_type_t main_type )
{
    (*this_).main_type = main_type;
}

static inline int64_t data_relationship_get_from_classifier_id ( data_relationship_t *this_ )
{
    return (*this_).from_classifier_id;
}

static inline void data_relationship_set_from_classifier_id ( data_relationship_t *this_, int64_t from_classifier_id )
{
    (*this_).from_classifier_id = from_classifier_id;
}

static inline int64_t data_relationship_get_to_classifier_id ( data_relationship_t *this_ )
{
    return (*this_).to_classifier_id;
}

static inline void data_relationship_set_to_classifier_id ( data_relationship_t *this_, int64_t to_classifier_id )
{
    (*this_).to_classifier_id = to_classifier_id;
}

static inline const char *data_relationship_get_name_ptr ( data_relationship_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).name );
}

static inline utf8stringbuf_t data_relationship_get_name_buf_ptr ( data_relationship_t *this_ )
{
    return (*this_).name;
}

static inline data_error_t data_relationship_set_name ( data_relationship_t *this_, const char *name )
{
    data_error_t result = DATA_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_relationship_get_description_ptr ( data_relationship_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).description );
}

static inline utf8stringbuf_t data_relationship_get_description_buf_ptr ( data_relationship_t *this_ )
{
    return (*this_).description;
}

static inline data_error_t data_relationship_set_description ( data_relationship_t *this_, const char *description )
{
    data_error_t result = DATA_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline int32_t data_relationship_get_list_order ( data_relationship_t *this_ )
{
    return (*this_).list_order;
}

static inline void data_relationship_set_list_order ( data_relationship_t *this_, int32_t list_order )
{
    (*this_).list_order = list_order;
}

static inline bool data_relationship_is_valid ( data_relationship_t *this_ )
{
    return ( DATA_ID_VOID_ID != (*this_).id );
}

static inline void data_relationship_trace ( data_relationship_t *this_ )
{
    TRACE_INFO( "data_relationship_t" );
    TRACE_INFO_INT( "- id:", (*this_).id );
    TRACE_INFO_INT( "- main_type:", (*this_).main_type );
    TRACE_INFO_INT( "- from_classifier_id:", (*this_).from_classifier_id );
    TRACE_INFO_INT( "- to_classifier_id:", (*this_).to_classifier_id );
    TRACE_INFO_STR( "- name:", utf8stringbuf_get_string((*this_).name) );
    TRACE_INFO_STR( "- description:", utf8stringbuf_get_string((*this_).description) );
    TRACE_INFO_INT( "- list_order:", (*this_).list_order );
}


/*
Copyright 2016-2016 Andreas Warnke

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
