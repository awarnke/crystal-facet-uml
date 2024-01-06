/* File: io_xml_writer.c; Copyright and License: see below */

#include "format/io_xml_writer.h"
#include "data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

const char IO_XML_WRITER_START_TAG_START[2] = "<";
const char IO_XML_WRITER_START_TAG_END[2] = ">";
const char IO_XML_WRITER_END_TAG_START[3] = "</";
const char IO_XML_WRITER_END_TAG_END[2] = ">";
const char IO_XML_WRITER_EMPTY_TAG_START[2] = "<";
const char IO_XML_WRITER_EMPTY_TAG_END[3] = "/>";
const char IO_XML_WRITER_ATTR_SEPARATOR[2] = " ";
const char IO_XML_WRITER_ATTR_VALUE_START[3] = "=\"";
const char IO_XML_WRITER_ATTR_VALUE_END[2] = "\"";
const char IO_XML_WRITER_COMMENT_START[5] = "<!--";
const char IO_XML_WRITER_COMMENT_END[4] = "-->";
const char IO_XML_WRITER_NL[2] = "\n";
#define IO_XML_WRITER_PRIVATE_MAX_INDENT_LEVELS (12)

static const char *const IO_XML_WRITER_PRIVATE_ENCODE_XML_STRINGS[IO_XML_WRITER_PRIVATE_MAX_INDENT_LEVELS][6][2] = {
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n" },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n                " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n                    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n                        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n                            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n                                " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n                                    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n                                        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "\n", "\n                                            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    }
};

static const char *const IO_XML_WRITER_PRIVATE_ENCODE_XML_COMMENTS[IO_XML_WRITER_PRIVATE_MAX_INDENT_LEVELS][8][2] = {
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n" },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n                " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n                    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n                        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n                            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n                                " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n                                    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n                                        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n" },
        { "<", "&lt;" },
        { ">", "&gt;" },
        { "\"", "&quot;" },
        { "&", "&amp;" },
        { "-", " - " },
        { "\n", "\n                                            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    }
};

static const char *const IO_XML_WRITER_PRIVATE_INDENT_PLAIN[IO_XML_WRITER_PRIVATE_MAX_INDENT_LEVELS][2][2] = {
    {
        { "\n", "\n" },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n                " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n                    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n                        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n                            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n                                " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n                                    " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n                                        " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    },
    {
        { "\n", "\n                                            " },  /* indentation level */
        { NULL, NULL }  /* end translation table */
    }
};

void io_xml_writer_init ( io_xml_writer_t *this_, universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );

    (*this_).output = output;
    universal_escaping_output_stream_init( &((*this_).esc_output), &(IO_XML_WRITER_PRIVATE_ENCODE_XML_STRINGS[0]), output );
    (*this_).indent_level = 0;

    (*this_).xml_encode_table = &(IO_XML_WRITER_PRIVATE_ENCODE_XML_STRINGS[0]);
    (*this_).xml_comments_encode_table = &(IO_XML_WRITER_PRIVATE_ENCODE_XML_COMMENTS[0]);
    (*this_).xml_plain_table = &(IO_XML_WRITER_PRIVATE_INDENT_PLAIN[0]);

    U8_TRACE_END();
}

void io_xml_writer_destroy( io_xml_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    universal_escaping_output_stream_destroy( &((*this_).esc_output) );
    (*this_).output = NULL;

    U8_TRACE_END();
}

u8_error_t io_xml_writer_write_plain_id ( io_xml_writer_t *this_, data_id_t id )
{
    U8_TRACE_BEGIN();
    assert( DATA_TABLE_VOID != data_id_get_table(&id) );
    assert( DATA_ROW_ID_VOID != data_id_get_row_id(&id) );
    u8_error_t result = U8_ERROR_NONE;

    /* print id */
    {
        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_clear( id_str );
        data_id_to_utf8stringbuf( &id, id_str );

        const unsigned int len = utf8stringbuf_get_length(id_str);
        universal_escaping_output_stream_change_rules( &((*this_).esc_output), (*this_).xml_plain_table );
        result = universal_escaping_output_stream_write( &((*this_).esc_output), utf8stringbuf_get_string(id_str), len );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_xml_writer_write_int ( io_xml_writer_t *this_, int64_t number )
{
    U8_TRACE_BEGIN();
    char numberStr[21]; /* this is sufficient for signed 64 bit integers: -9223372036854775806 */
    u8_error_t result = U8_ERROR_NONE;

    /* Note: snprintf is not available on every OS */
    sprintf( numberStr, "%" PRIi64, number );
    result = io_xml_writer_write_plain( this_, &(numberStr[0]) );

    U8_TRACE_END_ERR( result );
    return result;
}

void io_xml_writer_private_update_encoding_tables ( io_xml_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    const unsigned int level
        = ( (*this_).indent_level >= IO_XML_WRITER_PRIVATE_MAX_INDENT_LEVELS )
        ? ( IO_XML_WRITER_PRIVATE_MAX_INDENT_LEVELS - 1 )
        : ( (*this_).indent_level );
    (*this_).xml_encode_table = &(IO_XML_WRITER_PRIVATE_ENCODE_XML_STRINGS[level]);
    (*this_).xml_comments_encode_table = &(IO_XML_WRITER_PRIVATE_ENCODE_XML_COMMENTS[level]);
    (*this_).xml_plain_table = &(IO_XML_WRITER_PRIVATE_INDENT_PLAIN[level]);

    U8_TRACE_END();
}


/*
Copyright 2017-2024 Andreas Warnke

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
