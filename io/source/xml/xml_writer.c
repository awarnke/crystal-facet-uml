/* File: xml_writer.c; Copyright and License: see below */

#include "xml/xml_writer.h"
#include "trace.h"
#include "tslog.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static const char * const XML_WRITER_PRIVATE_ENCODE_XML_STRINGS[] = {
    "<", "&lt;",
    ">", "&gt;",
    "\"", "&quot;",
    "&", "&amp;",
    NULL,  /* end translation table */
};

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
static const char * const XML_WRITER_PRIVATE_ENCODE_FMT_DB_STRINGS[] = {
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
    NULL,  /* end translation table */
};

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
static const char * const XML_WRITER_PRIVATE_ENCODE_FMT_XHTML_STRINGS[] = {
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
    NULL,  /* end translation table */
};

void xml_writer_init ( xml_writer_t *this_, FILE *output )
{
    TRACE_BEGIN();
    assert( NULL != output );

    (*this_).output = output;

    (*this_).temp_output = utf8stringbuf_init( sizeof( (*this_).temp_output_buffer), (*this_).temp_output_buffer );
    (*this_).xml_encode_table = XML_WRITER_PRIVATE_ENCODE_XML_STRINGS;
    (*this_).fmt_xhtml_encode_table = XML_WRITER_PRIVATE_ENCODE_FMT_XHTML_STRINGS;
    (*this_).fmt_db_encode_table = XML_WRITER_PRIVATE_ENCODE_FMT_DB_STRINGS;

    TRACE_END();
}

void xml_writer_destroy( xml_writer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).output = NULL;

    TRACE_END();
}

int xml_writer_private_write_plain_id ( xml_writer_t *this_, data_table_t table, int64_t row_id )
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
