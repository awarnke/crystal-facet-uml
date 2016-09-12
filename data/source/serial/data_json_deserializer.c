/* File: data_json_deserializer.c; Copyright and License: see below */

#include "serial/data_json_deserializer.h"
#include "trace.h"
#include "log.h"
#include <assert.h>

void data_json_deserializer_init ( data_json_deserializer_t *this_, const char *in_data )
{
    TRACE_BEGIN();

    data_json_tokenizer_init( &((*this_).tokenizer) );

    (*this_).in_data = in_data;
    (*this_).read_pos = 0;

    (*this_).container_stack_size = 0;
    (*this_).root_object_count = 0;
    (*this_).last_token = DATA_JSON_DESERIALIZER_STATE_START;

    TRACE_END();
}

void data_json_deserializer_reinit ( data_json_deserializer_t *this_, const char *in_data )
{
    TRACE_BEGIN();

    data_json_deserializer_destroy( this_ );
    data_json_deserializer_init( this_, in_data );

    TRACE_END();
}

void data_json_deserializer_destroy ( data_json_deserializer_t *this_ )
{
    TRACE_BEGIN();

    data_json_tokenizer_destroy( &((*this_).tokenizer) );

    TRACE_END();
}

data_error_t data_json_deserializer_expect_begin_set ( data_json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID;
    
    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_deserializer_expect_end_set ( data_json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID;

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_deserializer_get_type_of_next_element ( data_json_deserializer_t *this_, data_table_t *out_type )
{
    TRACE_BEGIN();
    assert ( NULL != out_type );
    data_error_t result = DATA_ERROR_NONE;

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID;

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_deserializer_get_next_classifier ( data_json_deserializer_t *this_, data_classifier_t *out_object )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );
    data_error_t result = DATA_ERROR_NONE;

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID;

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_deserializer_get_next_diagram ( data_json_deserializer_t *this_, data_diagram_t *out_object )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );
    data_error_t result = DATA_ERROR_NONE;

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID;

    TRACE_END_ERR( result );
    return result;
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
