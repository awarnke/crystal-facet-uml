/* File: md_filter.c; Copyright and License: see below */

#include "md/md_filter.h"
#include "trace.h"
#include "tslog.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
static const char * const MD_FILTER_PRIVATE_ENCODE_FMT_DB_STRINGS[] = {
    "<", "&lt;",
    ">", "&gt;",
    "\"", "&quot;",
    "&", "&amp;",
    "\n\n", "</para>\n<para>",
    "\n+", "</para>\n<para>+",  /* markdown list */
    "\n-", "</para>\n<para>-",  /* markdown list or heading */
    "\n*", "</para>\n<para>*",  /* markdown list */
    "\n0", "</para>\n<para>0",  /* markdown list */
    "\n1", "</para>\n<para>1",  /* markdown list */
    "\n2", "</para>\n<para>2",  /* markdown list */
    "\n3", "</para>\n<para>3",  /* markdown list */
    "\n4", "</para>\n<para>4",  /* markdown list */
    "\n5", "</para>\n<para>5",  /* markdown list */
    "\n6", "</para>\n<para>6",  /* markdown list */
    "\n7", "</para>\n<para>7",  /* markdown list */
    "\n8", "</para>\n<para>8",  /* markdown list */
    "\n9", "</para>\n<para>9",  /* markdown list */
    "\n>", "</para>\n<para>&gt;",  /* markdown citation */
    "\n=", "</para>\n<para>=",  /* markdown heading */
    "\n#", "</para>\n<para>#",  /* markdown heading */
    "\n ", "</para>\n<para>&#xA0;",  /* markdown list-entry continuation */
    "\n|", "</para>\n<para>|",  /* markdown table */
    NULL,  /* end translation table */
};

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
static const char * const MD_FILTER_PRIVATE_ENCODE_FMT_XHTML_STRINGS[] = {
    "<", "&lt;",
    ">", "&gt;",
    "\"", "&quot;",
    "&", "&amp;",
    "\n\n", "<br />\n",
    "\n+", "<br />+",  /* markdown list */
    "\n-", "<br />-",  /* markdown list or heading */
    "\n*", "<br />*",  /* markdown list */
    "\n0", "<br />0",  /* markdown list */
    "\n1", "<br />1",  /* markdown list */
    "\n2", "<br />2",  /* markdown list */
    "\n3", "<br />3",  /* markdown list */
    "\n4", "<br />4",  /* markdown list */
    "\n5", "<br />5",  /* markdown list */
    "\n6", "<br />6",  /* markdown list */
    "\n7", "<br />7",  /* markdown list */
    "\n8", "<br />8",  /* markdown list */
    "\n9", "<br />9",  /* markdown list */
    "\n>", "<br />&gt;",  /* markdown citation */
    "\n=", "<br />=",  /* markdown heading */
    "\n#", "<br />#",  /* markdown heading */
    "\n ", "<br />&nbsp;",  /* markdown list-entry continuation */
    "\n|", "<br />|",  /* markdown table */
    NULL,  /* end translation table */
};

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
static const char MD_FILTER_LINEBREAK[] = "\n";
static const char MD_FILTER_ID_START[] = "./";
static const char MD_FILTER_ID_AS_ID_END[] = "#id";
static const char MD_FILTER_ID_AS_NAME_END[] = "#name";

void md_filter_init ( md_filter_t *this_, data_database_reader_t *db_reader, xml_writer_t *sink )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != sink );

    (*this_).db_reader = db_reader;
    (*this_).sink = sink;

    (*this_).fmt_xhtml_encode_table = MD_FILTER_PRIVATE_ENCODE_FMT_XHTML_STRINGS;
    (*this_).fmt_db_encode_table = MD_FILTER_PRIVATE_ENCODE_FMT_DB_STRINGS;

    TRACE_END();
}

void md_filter_destroy( md_filter_t *this_ )
{
    TRACE_BEGIN();

    (*this_).sink = NULL;
    (*this_).db_reader = NULL;

    TRACE_END();
}

int md_filter_write_fmt_xhtml_enc ( md_filter_t *this_, const char *text )
{
    TRACE_BEGIN();
    assert ( NULL != text );
    assert ( NULL != (*this_).db_reader );
    assert ( NULL != (*this_).sink );

    /*
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).temp_output, text );
    strerr |= utf8stringbuf_replace_all( (*this_).temp_output, (*this_).fmt_xhtml_encode_table );
    size_t text_len = utf8stringbuf_get_length( (*this_).temp_output );
    size_t out_count;
    out_count = fwrite( utf8stringbuf_get_string( (*this_).temp_output ), 1 / * size of char * /, text_len, (*this_).output );
    const int result = ((out_count != text_len)||( UTF8ERROR_SUCCESS != strerr)) ? -1 : 0;
*/
    const int result = xml_writer_write_xml_enc ( (*this_).sink, text );

    TRACE_END_ERR( result );
    return result;
}

int md_filter_write_fmt_db_enc ( md_filter_t *this_, const char *text )
{
    TRACE_BEGIN();
    assert ( NULL != text );
    assert ( NULL != (*this_).db_reader );
    assert ( NULL != (*this_).sink );
/*
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).temp_output, text );
    strerr |= utf8stringbuf_replace_all( (*this_).temp_output, (*this_).fmt_db_encode_table );
    size_t text_len = utf8stringbuf_get_length( (*this_).temp_output );
    size_t out_count;
    out_count = fwrite( utf8stringbuf_get_string( (*this_).temp_output ), 1 / * size of char * /, text_len, (*this_).output );
    const int result = ((out_count != text_len)||( UTF8ERROR_SUCCESS != strerr)) ? -1 : 0;
*/
    const int result = xml_writer_write_xml_enc ( (*this_).sink, text );

    TRACE_END_ERR( result );
    return result;
}

int md_filter_transform ( md_filter_t *this_, const char *text )
{
    TRACE_BEGIN();
    assert ( NULL != text );
    assert ( NULL != (*this_).db_reader );
    assert ( NULL != (*this_).sink );

    unsigned int text_start_byte = 0;
    const unsigned int text_byte_length = utf8string_get_length( text );
    int write_err = 0;

    for ( unsigned int text_current_byte = text_start_byte; text_current_byte < text_byte_length; text_current_byte ++ )
    {
        if ( utf8string_equals_region_str( text, text_current_byte, MD_FILTER_LINEBREAK ) )
        {
            write_err |= xml_writer_write_xml_enc_buf( (*this_).sink, &(text[text_start_byte]), text_current_byte-text_start_byte );
            write_err |= xml_writer_write_plain ( (*this_).sink, "<br />" );
            text_start_byte = text_current_byte+1;

        }
        else if ( utf8string_equals_region_str( text, text_current_byte, MD_FILTER_ID_START ) )
        {
            /*
static const char * const MD_FILTER_ID_AS_ID_END[] = "#id";
static const char * const MD_FILTER_ID_AS_NAME_END[] = "#name";
            */
        }

        if ( text_current_byte+1 == text_byte_length )
        {
            write_err |= xml_writer_write_xml_enc( (*this_).sink, &(text[text_start_byte]) );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}


/*
Copyright 2019-2020 Andreas Warnke

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
