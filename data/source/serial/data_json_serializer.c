/* File: data_json_serializer.c; Copyright and License: see below */

#include "serial/data_json_serializer.h"
#include "trace.h"
#include "log.h"
#include <assert.h>

void data_json_serializer_init ( data_json_serializer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).in_array = false;

    TRACE_END();
}

void data_json_serializer_destroy ( data_json_serializer_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

/*
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_begin_array() was already called.
 */
data_error_t data_json_serializer_begin_array ( data_json_serializer_t *this_, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    TRACE_END_ERR( result );
    return result;
}

/*
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_end_array() was already called.
 */
data_error_t data_json_serializer_end_array ( data_json_serializer_t *this_, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    TRACE_END_ERR( result );
    return result;
}

/*
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_begin_array() was not yet called.
 */
data_error_t data_json_serializer_append_classifier ( data_json_serializer_t *this_, data_classifier_t object, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    TRACE_END_ERR( result );
    return result;
}

/*
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_begin_array() was not yet called.
 */
data_error_t data_json_serializer_append_diagram ( data_json_serializer_t *this_, data_diagram_t object, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

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
