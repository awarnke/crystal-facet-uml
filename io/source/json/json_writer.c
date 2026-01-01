/* File: json_writer.c; Copyright and License: see below */

#include "json/json_writer.h"
#include "entity/data_id.h"
#include "utf8stringbuf/utf8stringlines.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
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
    { NULL, NULL }  /* for JSON, see rfc7159 */
};

const char JSON_CONSTANTS_INDENT[(2*JSON_WRITER_MAX_INDENT)+sizeof("")]
    = JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB
      JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB;

const char JSON_CONSTANTS_INDENT_QUOTE[(2*JSON_WRITER_MAX_INDENT)+sizeof(JSON_CONSTANTS_QUOTE)]
    = JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB
      JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_TAB JSON_CONSTANTS_QUOTE;

void json_writer_init( json_writer_t *this_,
                       universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );

    (*this_).output = output;
    universal_escaping_output_stream_init( &((*this_).esc_output), &JSON_WRITER_PRIVATE_ENCODE_JSON_STRINGS, output );

    (*this_).json_string_encode_table = &JSON_WRITER_PRIVATE_ENCODE_JSON_STRINGS;

    U8_TRACE_END();
}

void json_writer_destroy( json_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    universal_escaping_output_stream_destroy( &((*this_).esc_output) );
    (*this_).output = NULL;

    U8_TRACE_END();
}

u8_error_t json_writer_write_plain_id ( json_writer_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( DATA_TABLE_VOID != data_id_get_table(&id) );
    assert( DATA_ROW_VOID != data_id_get_row_id(&id) );
    u8_error_t result = U8_ERROR_NONE;

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( &id_str );
        data_id_to_utf8stringbuf( &id, id_str );

        const unsigned int len = utf8stringbuf_get_length( &id_str );
        universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).json_string_encode_table );
        result = universal_escaping_output_stream_write( &((*this_).esc_output), utf8stringbuf_get_string( &id_str ), len );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t json_writer_write_int ( json_writer_t *this_, int64_t number )
{
    U8_TRACE_BEGIN();
    char numberStr[21]; /* this is sufficient for signed 64 bit integers: -9223372036854775806 */
    u8_error_t result = U8_ERROR_NONE;

    /* Note: snprintf is not available on every OS */
    sprintf( numberStr, "%" PRIi64, number );
    result = json_writer_write_plain( this_, &(numberStr[0]) );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t json_writer_write_member_string_array ( json_writer_t *this_,
                                                   unsigned int indent,
                                                   utf8string_t *enc_name,
                                                   utf8string_t *unenc_value,
                                                   bool next_follows )
{
    U8_TRACE_BEGIN();
    assert( 7 == JSON_WRITER_MAX_INDENT );
    assert( indent <= JSON_WRITER_MAX_INDENT );
    assert( NULL != enc_name );
    assert( NULL != unenc_value );
    u8_error_t write_err;

    utf8stringview_t line_list;
    utf8stringview_init_str( &line_list, unenc_value );
    utf8stringlines_t line_iter;
    utf8stringlines_init( &line_iter,
                          &line_list,
                          80 /*line_length*/
                        );

    write_err = json_writer_write_plain( this_, &(JSON_CONSTANTS_INDENT_QUOTE[2*(JSON_WRITER_MAX_INDENT-indent)]) );
    write_err |= json_writer_write_plain( this_, enc_name );
    write_err |= json_writer_write_plain( this_,
                                          JSON_CONSTANTS_QUOTE
                                          JSON_CONSTANTS_DEF
                                          JSON_CONSTANTS_BEGIN_ARRAY
                                          JSON_CONSTANTS_NL
    );
    while ( utf8stringlines_has_next( &line_iter ) )
    {
        utf8stringview_t line = utf8stringlines_next( &line_iter );
        write_err |= json_writer_write_plain( this_,
                                              JSON_CONSTANTS_TAB
                                              JSON_CONSTANTS_TAB
                                              JSON_CONSTANTS_TAB
                                              JSON_CONSTANTS_TAB
                                              JSON_CONSTANTS_TAB
                                              JSON_CONSTANTS_TAB
                                              JSON_CONSTANTS_TAB
                                              JSON_CONSTANTS_TAB
                                              JSON_CONSTANTS_QUOTE
                                            );
        write_err |= json_writer_write_string_view_enc( this_, &line );
        if ( utf8stringlines_has_next( &line_iter ) )
        {
            write_err |= json_writer_write_plain( this_,
                                                  JSON_CONSTANTS_QUOTE
                                                  JSON_CONSTANTS_NEXT_NL
                                                );
        }
        else
        {
            write_err |= json_writer_write_plain( this_,
                                                  JSON_CONSTANTS_QUOTE
                                                  JSON_CONSTANTS_NL
                                                );
        }
    }
    write_err = json_writer_write_plain( this_, &(JSON_CONSTANTS_INDENT[2*(JSON_WRITER_MAX_INDENT-indent)]) );
    write_err |= json_writer_write_plain( this_,
                                          next_follows
                                          ? JSON_CONSTANTS_END_ARRAY JSON_CONSTANTS_NEXT_NL
                                          : JSON_CONSTANTS_END_ARRAY JSON_CONSTANTS_NL
                                        );

    utf8stringlines_destroy( &line_iter );
    utf8stringview_destroy( &line_list );

    U8_TRACE_END_ERR( write_err );
    return write_err;
}


/*
Copyright 2021-2026 Andreas Warnke

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
