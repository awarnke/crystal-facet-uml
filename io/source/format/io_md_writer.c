/* File: io_md_writer.c; Copyright and License: see below */

#include "format/io_md_writer.h"
#include "utf8stringbuf/utf8stringview.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
static const char IO_MD_WRITER_LINEBREAK = '\n';
static const char IO_MD_WRITER_LINK_AS_ID[] = "#id";
static const char IO_MD_WRITER_LINK_AS_NAME[] = "#name";

void io_md_writer_init ( io_md_writer_t *this_,
                         data_database_reader_t *db_reader,
                         const char * tag_linebreak,
                         const char * tag_xref_start,
                         const char * tag_xref_middle,
                         const char * tag_xref_end,
                         io_xml_writer_t *sink )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != tag_linebreak );
    assert( NULL != tag_xref_start );
    assert( NULL != tag_xref_middle );
    assert( NULL != tag_xref_end );
    assert( NULL != sink );

    (*this_).db_reader = db_reader;
    (*this_).tag_linebreak = tag_linebreak;
    (*this_).tag_xref_start = tag_xref_start;
    (*this_).tag_xref_middle = tag_xref_middle;
    (*this_).tag_xref_end = tag_xref_end;
    (*this_).sink = sink;

    U8_TRACE_END();
}

void io_md_writer_destroy( io_md_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).sink = NULL;
    (*this_).db_reader = NULL;

    U8_TRACE_END();
}

u8_error_t io_md_writer_transform ( io_md_writer_t *this_, const char *text )
{
    U8_TRACE_BEGIN();
    assert ( NULL != text );
    assert ( NULL != (*this_).db_reader );
    assert ( NULL != (*this_).sink );

    unsigned int text_start_byte = 0;
    const unsigned int text_byte_length = utf8string_get_length( text );
    u8_error_t write_err = U8_ERROR_NONE;

    for ( unsigned int text_current_byte = 0; text_current_byte < text_byte_length; text_current_byte ++ )
    {
        const char current = text[text_current_byte];  /* note: only in case current<=0x7f this is a valid code point */
        const char peeknext = text[text_current_byte+1];  /* note: only in case current<=0x7f this is a valid code point, 0 at string end */
        if ( current == IO_MD_WRITER_LINEBREAK )
        {
            if (( peeknext == IO_MD_WRITER_LINEBREAK )
                || ( peeknext == '+' )  /* list */
                || ( peeknext == '*' )  /* list */
                || ( peeknext == '-' )  /* list or heading */
                || ( peeknext == '=' )  /* heading */
                || ( peeknext == '#' )  /* heading */
                || ( peeknext == '0' )  /* ordered list */
                || ( peeknext == '1' )  /* ordered list */
                || ( peeknext == '2' )  /* ordered list */
                || ( peeknext == '3' )  /* ordered list */
                || ( peeknext == '4' )  /* ordered list */
                || ( peeknext == '5' )  /* ordered list */
                || ( peeknext == '6' )  /* ordered list */
                || ( peeknext == '7' )  /* ordered list */
                || ( peeknext == '8' )  /* ordered list */
                || ( peeknext == '9' )  /* ordered list */
                || ( peeknext == '>' )  /* citation */
                /*|| ( peeknext == ' ' )*/  /* list continuation */
                || ( peeknext == '|' ))  /* table */

            {
                const utf8stringview_t str_view = UTF8STRINGVIEW( &(text[text_start_byte]), text_current_byte-text_start_byte );
                write_err |= io_xml_writer_write_xml_enc_view( (*this_).sink, &str_view );
                write_err |= io_xml_writer_write_plain ( (*this_).sink, (*this_).tag_linebreak );
                text_start_byte = text_current_byte+1;
            }
        }
        else if ( current == DATA_TABLE_ALPHANUM_DIAGRAM /* = 'D' */)
        {
            /* try to parse an id */
            data_id_t probe_id = DATA_ID_VOID;
            utf8stringview_t string_to_parse;
            utf8stringview_t remainder = UTF8STRINGVIEW_EMPTY;
            const utf8error_t region_err
                = utf8stringview_init_region( &string_to_parse, text, text_current_byte, text_byte_length - text_current_byte );
            if ( region_err == UTF8ERROR_SUCCESS )
            {
                data_id_init_by_stringview( &probe_id, &string_to_parse, &remainder );
            }
            const unsigned int id_length = utf8stringview_get_length( &string_to_parse ) - utf8stringview_get_length( &remainder );

            if ( data_id_is_valid( &probe_id ) )
            {
                const bool show_id = utf8stringview_starts_with_str( &remainder, IO_MD_WRITER_LINK_AS_ID );
                const bool show_name
                    = show_id
                    ? false
                    : utf8stringview_starts_with_str( &remainder, IO_MD_WRITER_LINK_AS_NAME );

                if ( show_id || show_name )
                {
                    u8_error_t d_err;
                    d_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader, data_id_get_row_id( &probe_id ), &((*this_).temp_diagram) );
                    if ( d_err == U8_ERROR_NONE )
                    {
                        /* write previously parsed characters */
                        const utf8stringview_t str_view = UTF8STRINGVIEW( &(text[text_start_byte]), text_current_byte-text_start_byte );
                        write_err |= io_xml_writer_write_xml_enc_view( (*this_).sink, &str_view );
                        text_start_byte = text_current_byte;

                        /* write id */
                        char probe_id_str_buf[DATA_ID_MAX_UTF8STRING_SIZE] = "";
                        utf8stringbuf_t probe_id_str = UTF8STRINGBUF( probe_id_str_buf );
                        write_err |= data_id_to_utf8stringbuf ( &probe_id, probe_id_str );
                        write_err |= io_xml_writer_write_plain ( (*this_).sink, (*this_).tag_xref_start );
                        write_err |= io_xml_writer_write_xml_enc( (*this_).sink, utf8stringbuf_get_string( &probe_id_str ) );
                        write_err |= io_xml_writer_write_plain ( (*this_).sink, (*this_).tag_xref_middle );
                        if ( show_id )
                        {
                            write_err |= io_xml_writer_write_xml_enc( (*this_).sink, utf8stringbuf_get_string( &probe_id_str ) );
                            text_current_byte += id_length + sizeof(IO_MD_WRITER_LINK_AS_ID)-1 - 1;
                        }
                        else /* show_name */
                        {
                            write_err |= io_xml_writer_write_xml_enc( (*this_).sink, data_diagram_get_name_const( &((*this_).temp_diagram) ) );
                            text_current_byte += id_length + sizeof(IO_MD_WRITER_LINK_AS_NAME)-1 - 1;
                        }
                        write_err |= io_xml_writer_write_plain ( (*this_).sink, (*this_).tag_xref_end );
                        text_start_byte = text_current_byte+1;

                        /* destroy the diagram */
                        data_diagram_destroy( &((*this_).temp_diagram) );
                    }
                    else
                    {
                        U8_TRACE_INFO_INT("id found but diagram does not exist: D", data_id_get_row_id( &probe_id ) );
                    }
                }
                else
                {
                    U8_TRACE_INFO_INT("id found but #id or #name fragment missing: D", data_id_get_row_id( &probe_id ) );
                }
            }
        }

        if ( text_current_byte+1 == text_byte_length )
        {
            write_err |= io_xml_writer_write_xml_enc( (*this_).sink, &(text[text_start_byte]) );
        }
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}


/*
Copyright 2019-2024 Andreas Warnke

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
