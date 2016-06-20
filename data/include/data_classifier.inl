/* File: data_classifier.inl; Copyright and License: see below */

static inline data_error_t data_classifier_init_new ( data_classifier_t *this_,
                                                      data_classifier_type_t main_type,
                                                      const char* stereotype,
                                                      const char* name,
                                                      const char* description,
                                                      int32_t x_order,
                                                      int32_t y_order )
{
    utf8error_t strerr;
    data_error_t result = DATA_ERROR_NONE;

    (*this_).id = DATA_ID_VOID_ID;
    (*this_).main_type = main_type;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).stereotype, stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).x_order = x_order;
    (*this_).y_order = y_order;

    return result;
}

static inline void data_classifier_init_empty ( data_classifier_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).main_type = DATA_CLASSIFIER_TYPE_BLOCK;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    utf8stringbuf_clear( (*this_).stereotype );
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    utf8stringbuf_clear( (*this_).name );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).x_order = 0;
    (*this_).y_order = 0;
}

static inline void data_classifier_reinit_empty ( data_classifier_t *this_ )
{
    /* data_classifier_destroy( this_ );  -- not necessary */
    data_classifier_init_empty( this_ );
}

static inline data_error_t data_classifier_init ( data_classifier_t *this_,
                                                  int64_t id,
                                                  data_classifier_type_t main_type,
                                                  const char* stereotype,
                                                  const char* name,
                                                  const char* description,
                                                  int32_t x_order,
                                                  int32_t y_order )
{
    utf8error_t strerr;
    data_error_t result = DATA_ERROR_NONE;

    (*this_).id = id;
    (*this_).main_type = main_type;

    (*this_).stereotype = utf8stringbuf_init( sizeof((*this_).private_stereotype_buffer), (*this_).private_stereotype_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).stereotype, stereotype );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).x_order = x_order;
    (*this_).y_order = y_order;

    return result;
}

static inline data_error_t data_classifier_reinit ( data_classifier_t *this_,
                                                    int64_t id,
                                                    data_classifier_type_t main_type,
                                                    const char* stereotype,
                                                    const char* name,
                                                    const char* description,
                                                    int32_t x_order,
                                                    int32_t y_order )
{
    return data_classifier_init( this_, id, main_type, stereotype, name, description, x_order, y_order );
}

static inline void data_classifier_destroy ( data_classifier_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
}

static inline void data_classifier_trace ( data_classifier_t *this_ )
{
    TRACE_INFO( "data_classifier_t" );
    TRACE_INFO_INT( "- id:", (*this_).id );
    TRACE_INFO_INT( "- main_type:", (*this_).main_type );
    TRACE_INFO_STR( "- stereotype:", utf8stringbuf_get_string((*this_).stereotype) );
    TRACE_INFO_STR( "- name:", utf8stringbuf_get_string((*this_).name) );
    TRACE_INFO_STR( "- description:", utf8stringbuf_get_string((*this_).description) );
    TRACE_INFO_INT( "- x_order:", (*this_).x_order );
    TRACE_INFO_INT( "- y_order:", (*this_).y_order );
}

static inline int64_t data_classifier_get_id ( data_classifier_t *this_ )
{
    return (*this_).id;
}

static inline data_classifier_type_t data_classifier_get_main_type ( data_classifier_t *this_ )
{
    return (*this_).main_type;
}

static inline const char *data_classifier_get_stereotype_ptr ( data_classifier_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).stereotype );
}

static inline const char *data_classifier_get_name_ptr ( data_classifier_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).name );
}

static inline const char *data_classifier_get_description_ptr ( data_classifier_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).description );
}

static inline int32_t data_classifier_get_x_order ( data_classifier_t *this_ )
{
    return (*this_).x_order;
}

static inline int32_t data_classifier_get_y_order ( data_classifier_t *this_ )
{
    return (*this_).y_order;
}

static inline bool data_classifier_is_valid ( data_classifier_t *this_ )
{
    return ( DATA_ID_VOID_ID != (*this_).id );
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
