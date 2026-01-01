/* File: io_txt_writer.c; Copyright and License: see below */

#include "format/io_txt_writer.h"
#include "utf8stringbuf/utf8stringlines.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

enum io_txt_writer_indent_enum {
    TXT_WRITER_INDENT_COLUMN = 80,  /*!< the text column in which the id shal end */
    TXT_WRITER_MAX_COLUMN = 80,  /*!< the number of max columns the text shall be layouted - if possible */
};

static const char TXT_ID_INDENT_SPACES[TXT_WRITER_INDENT_COLUMN+1]
    = "                                                                                ";
static const char TXT_NEWLINE[] = "\n";

void io_txt_writer_init ( io_txt_writer_t *this_,
                          universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );

    (*this_).output = output;

    U8_TRACE_END();
}

void io_txt_writer_destroy( io_txt_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).output = NULL;

    U8_TRACE_END();
}

u8_error_t io_txt_writer_write_indent_multiline_string ( io_txt_writer_t *this_,
                                                         const char *indent,
                                                         const char *multiline_string )
{
    U8_TRACE_BEGIN();
    assert( NULL != indent );
    assert( NULL != (*this_).output );
    u8_error_t result = U8_ERROR_NONE;
    utf8stringview_t indent_view;
    utf8stringview_init_str( &indent_view, indent );

    if ( NULL != multiline_string )
    {
        utf8stringview_t line_list;
        utf8stringview_init_str( &line_list, multiline_string );
        utf8stringlines_t line_iter;
        utf8stringlines_init( &line_iter,
                              &line_list,
                              TXT_WRITER_MAX_COLUMN - utf8stringview_get_length( &indent_view ) /*line_length*/
                            );

        while ( utf8stringlines_has_next( &line_iter ) )
        {
            utf8stringview_t line = utf8stringlines_next( &line_iter );
            const bool is_empty_last_line
                = ( ! utf8stringlines_has_next( &line_iter ) )&&( 0 == utf8stringview_get_length( &line ) );
            if ( ! is_empty_last_line )
            {
                /* print indent pattern */
                result |= universal_output_stream_write( (*this_).output,
                                                        utf8stringview_get_start( &indent_view ),
                                                        utf8stringview_get_length( &indent_view )
                                                    );

                /* print next line */
                result |= universal_output_stream_write( (*this_).output,
                                                        utf8stringview_get_start( &line ),
                                                        utf8stringview_get_length( &line )
                                                    );

                if ( ! utf8stringview_ends_with_str( &line, TXT_NEWLINE ) )
                {
                    /* print newline */
                    result |= universal_output_stream_write( (*this_).output, TXT_NEWLINE, strlen(TXT_NEWLINE) );
                }
            }
        }
        utf8stringlines_destroy( &line_iter );
        utf8stringview_destroy( &line_list );
    }

    utf8stringview_destroy( &indent_view );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_txt_writer_write_aligned_id ( io_txt_writer_t *this_, int right_align_to, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( DATA_TABLE_VOID != data_id_get_table(&id) );
    assert( DATA_ROW_VOID != data_id_get_row_id(&id) );
    assert( NULL != (*this_).output );
    assert( sizeof(TXT_ID_INDENT_SPACES) == 1+TXT_WRITER_INDENT_COLUMN );
    u8_error_t result = U8_ERROR_NONE;

    /* indent */
    if ( right_align_to > TXT_WRITER_INDENT_COLUMN )
    {
        U8_LOG_ERROR_INT( "more spaces requested than available. missing:", right_align_to - TXT_WRITER_INDENT_COLUMN );
        right_align_to = TXT_WRITER_INDENT_COLUMN;
    }

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE+2];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( &id_str );
        utf8stringbuf_append_str( &id_str, "[" );
        data_id_to_utf8stringbuf( &id, id_str );
        utf8stringbuf_append_str( &id_str, "]" );
        const unsigned int len = utf8stringbuf_get_length( &id_str );

        /* do indentation: */
        int remaining_indent = right_align_to - len;
        if ( remaining_indent > 0 )
        {
            result |= universal_output_stream_write( (*this_).output, &TXT_ID_INDENT_SPACES, remaining_indent );
        }

        result |= universal_output_stream_write( (*this_).output, utf8stringbuf_get_string( &id_str ), len );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_txt_writer_write_plain_id ( io_txt_writer_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( DATA_TABLE_VOID != data_id_get_table(&id) );
    assert( DATA_ROW_VOID != data_id_get_row_id(&id) );
    assert( NULL != (*this_).output );
    u8_error_t result = U8_ERROR_NONE;

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( &id_str );
        data_id_to_utf8stringbuf( &id, id_str );

        const unsigned int len = utf8stringbuf_get_length( &id_str );
        result |= universal_output_stream_write ( (*this_).output, utf8stringbuf_get_string( &id_str ), len );
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2017-2026 Andreas Warnke

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
