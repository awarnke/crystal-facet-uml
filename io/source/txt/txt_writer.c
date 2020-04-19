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

void txt_writer_init ( txt_writer_t *this_, FILE *output )
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
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */
    const size_t indent_length = strlen( indent );
    assert( (*this_).output != NULL );

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
                out_count = fwrite( indent, 1 /* size of char */, indent_length, (*this_).output );
                if ( out_count != indent_length )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", indent_length - out_count );
                    result = -1;
                }

                /* print next line */
                out_count = fwrite( line_start, 1 /* size of char */, line_length, (*this_).output );
                if ( out_count != line_length )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", line_length - out_count );
                    result = -1;
                }

                /* print newline */
                out_count = fwrite( TXT_NEWLINE, 1 /* size of char */, strlen(TXT_NEWLINE), (*this_).output );
                if ( out_count != strlen(TXT_NEWLINE) )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", strlen(TXT_NEWLINE) - out_count );
                    result = -1;
                }

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
                                 data_table_t table,
                                 int64_t row_id )
{
    TRACE_BEGIN();
    assert( DATA_TABLE_VOID != table );
    assert( DATA_ID_VOID_ID != row_id );
    assert( sizeof(TXT_ID_INDENT_SPACES) == 1+TXT_WRITER_INDENT_COLUMN );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */
    assert( (*this_).output != NULL );

    /* indent */
    if ( indent_width > TXT_WRITER_INDENT_COLUMN )
    {
        TSLOG_ERROR_INT( "more spaces requested than available. missing:", indent_width - TXT_WRITER_INDENT_COLUMN );
        indent_width = TXT_WRITER_INDENT_COLUMN;
    }
    if ( indent_width > 0 )
    {
        out_count = fwrite( &TXT_ID_INDENT_SPACES, 1, indent_width, (*this_).output );
        if ( out_count != indent_width )
        {
            TSLOG_ERROR_INT( "not all bytes could be written. missing:", indent_width - out_count );
            result = -1;
        }
    }

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE+2];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( id_str );
        utf8stringbuf_append_str( id_str, " [" );
        data_id_t the_id;
        data_id_init( &the_id, table, row_id );
        data_id_to_utf8stringbuf( &the_id, id_str );
        utf8stringbuf_append_str( id_str, "]" );

        unsigned int len = utf8stringbuf_get_length(id_str);
        out_count = fwrite( utf8stringbuf_get_string(id_str), 1, len, (*this_).output );
        if ( out_count != len )
        {
            TSLOG_ERROR_INT( "not all bytes could be written. missing:", len - out_count );
            result = -1;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

int txt_writer_write_plain_id ( txt_writer_t *this_, data_table_t table, int64_t row_id )
{
    TRACE_BEGIN();
    assert( DATA_TABLE_VOID != table );
    assert( DATA_ID_VOID_ID != row_id );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */
    assert( (*this_).output != NULL );

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( id_str );
        data_id_t the_id;
        data_id_init( &the_id, table, row_id );
        data_id_to_utf8stringbuf( &the_id, id_str );

        unsigned int len = utf8stringbuf_get_length(id_str);
        out_count = fwrite( utf8stringbuf_get_string(id_str), 1, len, (*this_).output );
        if ( out_count != len )
        {
            TSLOG_ERROR_INT( "not all bytes could be written. missing:", len - out_count );
            result = -1;
        }
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2017-2020 Andreas Warnke

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
