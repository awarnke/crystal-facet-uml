/* File: txt_writer.c; Copyright and License: see below */

#include "txt/txt_writer.h"
#include "trace.h"
#include "tslog.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

enum txt_writer_indent_enum {
    TXT_WRITER_INDENT_COLUMN = 48,  /*!< the text column in which the id starts */
};

static const char TXT_ID_INDENT_SPACES[TXT_WRITER_INDENT_COLUMN+1] = "                                                ";
static const char TXT_NEWLINE[] = "\n";

void txt_writer_init ( txt_writer_t *this_,
                       universal_output_stream_t *output )
{
    TRACE_BEGIN();
    assert( NULL != output );

    (*this_).output = output;

    TRACE_END();
}

void txt_writer_destroy( txt_writer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).output = NULL;

    TRACE_END();
}

int txt_writer_write_indent_multiline_string ( txt_writer_t *this_,
                                               const char *indent,
                                               const char *multiline_string )
{
    TRACE_BEGIN();
    assert( NULL != indent );
    assert( NULL != (*this_).output );
    int result = 0;
    const size_t indent_length = strlen( indent );

    if ( NULL != multiline_string )
    {
        const char *line_start = multiline_string;
        size_t line_length = 0;
        bool ignore_newline = false;  /* newlines after returns are ignored */

        size_t length = strlen( multiline_string );
        for ( size_t index = 0; index < length; index ++ )
        {
            bool end_of_line = false;

            char current = multiline_string[index];
            if ( '\r' == current )
            {
                ignore_newline = true;
                end_of_line = true;
            }
            else if ( '\n' == current )
            {
                if ( ignore_newline )
                {
                    line_start = &(multiline_string[index+1]);
                }
                else
                {
                    end_of_line = true;
                }
                ignore_newline = false;
            }
            else
            {
                ignore_newline = false;
                line_length ++;
                if ( index+1 == length )
                {
                    end_of_line = true;
                }
            }

            if ( end_of_line )
            {
                /* print indent pattern */
                result |= universal_output_stream_write ( (*this_).output, indent, indent_length );

                /* print next line */
                result |= universal_output_stream_write ( (*this_).output, line_start, line_length );

                /* print newline */
                result |= universal_output_stream_write ( (*this_).output, TXT_NEWLINE, strlen(TXT_NEWLINE) );

                /* reset line indices */
                line_start = &(multiline_string[index+1]);
                line_length = 0;
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

int txt_writer_write_indent_id ( txt_writer_t *this_,
                                 int indent_width,
                                 data_id_t id )
{
    TRACE_BEGIN();
    assert( DATA_TABLE_VOID != data_id_get_table(&id) );
    assert( DATA_ROW_ID_VOID != data_id_get_row_id(&id) );
    assert( NULL != (*this_).output );
    assert( sizeof(TXT_ID_INDENT_SPACES) == 1+TXT_WRITER_INDENT_COLUMN );
    int result = 0;

    /* indent */
    if ( indent_width > TXT_WRITER_INDENT_COLUMN )
    {
        TSLOG_ERROR_INT( "more spaces requested than available. missing:", indent_width - TXT_WRITER_INDENT_COLUMN );
        indent_width = TXT_WRITER_INDENT_COLUMN;
    }
    if ( indent_width > 0 )
    {
        result |= universal_output_stream_write( (*this_).output, &TXT_ID_INDENT_SPACES, indent_width );
    }

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE+2];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( id_str );
        utf8stringbuf_append_str( id_str, " [" );
        data_id_to_utf8stringbuf( &id, id_str );
        utf8stringbuf_append_str( id_str, "]" );

        const unsigned int len = utf8stringbuf_get_length(id_str);
        result |= universal_output_stream_write( (*this_).output, utf8stringbuf_get_string(id_str), len );
    }

    TRACE_END_ERR( result );
    return result;
}

int txt_writer_write_plain_id ( txt_writer_t *this_, data_id_t id )
{
    TRACE_BEGIN();
    assert( DATA_TABLE_VOID != data_id_get_table(&id) );
    assert( DATA_ROW_ID_VOID != data_id_get_row_id(&id) );
    assert( NULL != (*this_).output );
    int result = 0;

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( id_str );
        data_id_to_utf8stringbuf( &id, id_str );

        const unsigned int len = utf8stringbuf_get_length(id_str);
        result |= universal_output_stream_write ( (*this_).output, utf8stringbuf_get_string(id_str), len );
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2017-2022 Andreas Warnke

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
