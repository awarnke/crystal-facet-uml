/* File: data_json_serializer.c; Copyright and License: see below */

#include "serial/data_json_serializer.h"
#include "data_json_constants.h"
#include "trace.h"
#include "log.h"
#include <assert.h>

void data_json_serializer_init ( data_json_serializer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).in_array = false;
    (*this_).is_first = false;

    TRACE_END();
}

void data_json_serializer_destroy ( data_json_serializer_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

data_error_t data_json_serializer_begin_set ( data_json_serializer_t *this_, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( ! (*this_).in_array )
    {
        (*this_).in_array = true;

        utf8error_t strerr = UTF8ERROR_SUCCESS;

        strerr |= utf8stringbuf_append_str( out, DATA_JSON_CONSTANTS_BEGIN_OBJECT_NL );
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_SET
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_BEGIN_ARRAY_NL );

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
        (*this_).is_first = true;
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_serializer_end_set ( data_json_serializer_t *this_, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        (*this_).in_array = false;

        utf8error_t strerr = UTF8ERROR_SUCCESS;

        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_ARRAY_NL );
        strerr |= utf8stringbuf_append_str( out, DATA_JSON_CONSTANTS_END_OBJECT_NL );

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
        (*this_).is_first = false;
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

/*
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_begin_array() was not yet called.
 */
data_error_t data_json_serializer_append_classifier ( data_json_serializer_t *this_, data_classifier_t *object, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        utf8error_t strerr = UTF8ERROR_SUCCESS;

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
        (*this_).is_first = false;
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

/*
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_begin_array() was not yet called.
 */
data_error_t data_json_serializer_append_diagram ( data_json_serializer_t *this_, data_diagram_t *object, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        utf8error_t strerr = UTF8ERROR_SUCCESS;

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
        (*this_).is_first = false;
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

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
