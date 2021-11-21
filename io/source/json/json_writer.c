/* File: json_writer.c; Copyright and License: see below */

#include "json/json_writer.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static const char * const JSON_WRITER_PRIVATE_ENCODE_JSON_STRINGS[][2] = {
    { "\x09", "\\t" },  /* tab */
    { "\x0a", "\\n" },  /* newline */
    { "\x0d", "\\r" },  /* return */
    { "\x08", "\\b" },  /* backspace */
    { "\x0c", "\\f" },  /* form feed */
    { "\"", "\\\"" },  /* double quote */
    { "\\", "\\\\" },  /* backslash*/
    { "/", "\\/" },  /* slash */
    { NULL, NULL }  /* for JSON, see rfc7159 */
};

const char JSON_CONSTANTS_INDENT_QUOTE[(2*JSON_WRITER_MAX_INDENT)+sizeof(JSON_CONSTANTS_QUOTE)]
    = JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB
      JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_QUOTE;

void json_writer_init ( json_writer_t *this_,
                       universal_output_stream_t *output )
{
    TRACE_BEGIN();
    assert( NULL != output );

    (*this_).output = output;
    universal_escaping_output_stream_init( &((*this_).esc_output), &JSON_WRITER_PRIVATE_ENCODE_JSON_STRINGS, output );

    (*this_).json_string_encode_table = &JSON_WRITER_PRIVATE_ENCODE_JSON_STRINGS;
    (*this_).json_stringlist_encode_table = &JSON_WRITER_PRIVATE_ENCODE_JSON_STRINGS;

    TRACE_END();
}

void json_writer_destroy( json_writer_t *this_ )
{
    TRACE_BEGIN();

    universal_escaping_output_stream_destroy( &((*this_).esc_output) );
    (*this_).output = NULL;

    TRACE_END();
}

int json_writer_write_plain_id ( json_writer_t *this_, data_id_t id )
{
    TRACE_BEGIN();
    assert( DATA_TABLE_VOID != data_id_get_table(&id) );
    assert( DATA_ROW_ID_VOID != data_id_get_row_id(&id) );
    int result = 0;

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( id_str );
        data_id_to_utf8stringbuf( &id, id_str );

        const unsigned int len = utf8stringbuf_get_length(id_str);
        universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_string_encode_table );
        result = universal_escaping_output_stream_write( &((*this_).esc_output), utf8stringbuf_get_string(id_str), len );
    }

    TRACE_END_ERR( result );
    return result;
}

int json_writer_write_int ( json_writer_t *this_, int64_t number )
{
    TRACE_BEGIN();
    char numberStr[21]; /* this is sufficient for signed 64 bit integers: -9223372036854775806 */
    int result = 0;

    /* Note: snprintf is not available on every OS */
    sprintf( numberStr, "%" PRIi64, number );
    result = json_writer_write_plain( this_, &(numberStr[0]) );

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2021-2021 Andreas Warnke

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
