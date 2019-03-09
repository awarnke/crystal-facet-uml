/* File: io_format_writer.c; Copyright and License: see below */

#include "io_format_writer.h"
#include "trace.h"
#include "tslog.h"
#include <gtk/gtk.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static const char * const IO_FORMAT_WRITER_PRIVATE_ENCODE_XML_STRINGS[] = {
    "<", "&lt;",
    ">", "&gt;",
    "\"", "&quot;",
    "&", "&amp;",
    NULL,  /* end translation table */
};

void io_format_writer_init ( io_format_writer_t *this_,
                             io_file_format_t export_type,
                             FILE *output )
{
    TRACE_BEGIN();
    assert( NULL != output );

    (*this_).export_type = export_type;
    (*this_).output = output;

    (*this_).temp_output = utf8stringbuf_init( sizeof( (*this_).temp_output_buffer), (*this_).temp_output_buffer );
    (*this_).xml_encode_table = IO_FORMAT_WRITER_PRIVATE_ENCODE_XML_STRINGS;

    TRACE_END();
}

void io_format_writer_destroy( io_format_writer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).output = NULL;

    TRACE_END();
}

static const char DOCBOOK_ENC[]
= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
static const char DOCBOOK_DTD[]
= "<!DOCTYPE article PUBLIC \"-//OASIS//DTD DocBook XML V4.5//EN\"\n"
"\"http://www.oasis-open.org/docbook/xml/4.5/docbookx.dtd\">\n";
static const char DOCBOOK_DOC_START[]
= "<article lang=\"en\">\n";
static const char DOCBOOK_DOC_TITLE_START[]
= "    <title>";
static const char DOCBOOK_DOC_TITLE_END[]
= "</title>\n";
static const char DOCBOOK_DOC_END[]
= "</article>\n";

static const char DOCBOOK_SECT_START[]
= "    <section>\n";
static const char DOCBOOK_SECT_TITLE_START[]
= "        <title>";
static const char DOCBOOK_SECT_TITLE_END[]
= "</title>\n";
static const char DOCBOOK_SECT_PARA_START[]
= "        <para>\n";
static const char DOCBOOK_SECT_IMG_START[]
="\n            <mediaobject>\n"
"                <imageobject><imagedata fileref=\"";
static const char DOCBOOK_SECT_IMG_MIDDLE[]
= ".pdf\" width=\"12cm\"/></imageobject>\n"
"                <imageobject><imagedata fileref=\"";
static const char DOCBOOK_SECT_IMG_END[]
= ".png\"/></imageobject>\n"
"            </mediaobject>\n";
static const char DOCBOOK_SECT_PARA_END[]
= "        </para>\n";
static const char DOCBOOK_SECT_END[]
= "    </section>\n";


static const char XHTML_ENC[]
= "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
static const char XHTML_DTD[]
= "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n";
static const char XHTML_DOC_START[]
= "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
static const char XHTML_DOC_TITLE_START[]
= "    <head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><title>";
static const char XHTML_DOC_TITLE_END[]
= "</title></head>\n"
"    <body>\n";
static const char XHTML_DOC_END[]
= "    </body>\n"
"</html>\n";

static const char XHTML_SECT_START[]
= "        <div>\n";
static const char XHTML_SECT_TITLE_START[]
= "            <h1 class=\"title\">";
static const char XHTML_SECT_TITLE_END[]
= "</h1>\n";
static const char XHTML_SECT_PARA_START[]
= "            <p class=\"description\">\n";
static const char XHTML_SECT_IMG_START[]
= "                <div class=\"mediaobject\"><img src=\"";
static const char XHTML_SECT_IMG_END[]
= ".png\"/></div>\n";
static const char XHTML_SECT_PARA_END[]
= "            </p>\n";
static const char XHTML_SECT_END[]
= "        </div>\n";


int io_format_writer_write_header( io_format_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).output );
    assert ( NULL != document_title );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_ENC );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_DTD );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_DOC_START );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_DOC_TITLE_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, document_title );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_DOC_TITLE_END );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_ENC );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_DTD );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_DOC_START );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_DOC_TITLE_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, document_title );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_DOC_TITLE_END );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        default:
        {
            TSLOG_ERROR("error: unknown_format.");
            export_err = -1;
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_start_diagram( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).output );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_START );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_START );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        default:
        {
            TSLOG_ERROR("error: unknown_format.");
            export_err = -1;
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_write_diagram( io_format_writer_t *this_,
                                    data_diagram_t *diag_ptr,
                                    const char *diagram_file_base_name )
{
    TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    assert ( NULL != diagram_file_base_name );
    assert ( NULL != (*this_).output );
    int export_err = 0;

    const char *diag_name;
    diag_name = data_diagram_get_name_ptr( diag_ptr );
    const char *diag_description;
    diag_description = data_diagram_get_description_ptr( diag_ptr );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_TITLE_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diag_name );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_TITLE_END );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_PARA_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diag_description );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_IMG_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diagram_file_base_name );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_IMG_MIDDLE );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diagram_file_base_name );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_IMG_END );
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_PARA_END );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_TITLE_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diag_name );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_TITLE_END );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_PARA_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diag_description );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_IMG_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diagram_file_base_name );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_IMG_END );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_PARA_END );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        default:
        {
            TSLOG_ERROR("error: unknown_format.");
            export_err = -1;
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_end_diagram( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).output );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_END );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_END );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        default:
        {
            TSLOG_ERROR("error: unknown_format.");
            export_err = -1;
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_write_footer( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).output );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_DOC_END );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_DOC_END );

            if ( export_err != 0 )
            {
                TSLOG_ERROR( "not all bytes could be written" );
            }
        }
        break;

        default:
        {
            TSLOG_ERROR("error: unknown_format.");
            export_err = -1;
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2019-2019 Andreas Warnke

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
