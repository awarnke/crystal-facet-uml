/* File: io_format_writer.c; Copyright and License: see below */

#include "io_format_writer.h"
#include "util/string/utf8string.h"
#include "meta/meta_version.h"
#include "meta/meta_info.h"
#include "trace.h"
#include "tslog.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* GENERAL STRUCTURE */

/*
 *    <THING>_START
 *    <THING>_MIDDLE            #optional, if dynamic content needs to be added to the start
 *    <THING>_TITLE_START       #alternative to TITLE: NAME
 *    <THING>_TITLE_END         #alternative to TITLE: NAME
 *    ...                       #optional text
 *    <THING>_<OTHERSUB>_START  #optional if there ore other sub-things
 *    <THING>_<OTHERSUB>_END    #optional if there ore other sub-things
 *    <THING>_END
 */

/* IO_FILE_FORMAT_DOCBOOK */

static const char DOCBOOK_ENC[]
    = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
static const char DOCBOOK_DOC_START[]
    = "<book xmlns=\"http://docbook.org/ns/docbook\" version=\"5.0\" xml:lang=\"en\">\n";
static const char DOCBOOK_DOC_TITLE_START[]
    = "    <title>";
static const char DOCBOOK_DOC_TITLE_END[]
    = "</title>\n";
static const char DOCBOOK_DOC_END[]
    = "</book>\n";

static const char DOCBOOK_TOP_DIAGRAM_START[]
    = "    <chapter xml:id=\"";
static const char DOCBOOK_TOP_DIAGRAM_MIDDLE[]
    = "\">\n";
static const char DOCBOOK_TOP_DIAGRAM_END[]
    = "    </chapter>\n";
static const char DOCBOOK_DIAGRAM_START[]
    = "    <section xml:id=\"";
static const char DOCBOOK_DIAGRAM_MIDDLE[]
    = "\">\n";
static const char DOCBOOK_DIAGRAM_TITLE_START[]
    = "        <title>";
static const char DOCBOOK_DIAGRAM_TITLE_END[]
    = "</title>\n";
static const char DOCBOOK_DIAGRAM_IMG_START[]
    = "\n"
      "        <para>\n"
      "            <mediaobject>\n"
      "                <imageobject><imagedata fileref=\"";
static const char DOCBOOK_DIAGRAM_IMG_MIDDLE[]
    = ".pdf\" width=\"12cm\"/></imageobject>\n"
      "                <imageobject><imagedata fileref=\"";
static const char DOCBOOK_DIAGRAM_IMG_END[]
    = ".png\"/></imageobject>\n"
      "            </mediaobject>\n"
      "        </para>\n";
static const char DOCBOOK_DIAGRAM_END[]
    = "    </section>\n";

static const char DOCBOOK_DESCRIPTION_START[]
    = "        <para>\n";
static const char DOCBOOK_DESCRIPTION_MIDDLE[]  /* optional */
    = "\n"
      "        </para>\n"
      "        <para>\n";
static const char DOCBOOK_DESCRIPTION_XREF_START[]
    = "<xref linkend=\"";
static const char DOCBOOK_DESCRIPTION_XREF_MIDDLE[]
    = "\"/>: ";
static const char DOCBOOK_DESCRIPTION_XREF_END[]
    = "";
static const char DOCBOOK_DESCRIPTION_END[]
    = "\n"
      "        </para>\n";

static const char DOCBOOK_ELEMENT_LIST_START[]
    = "        <variablelist>\n";
static const char DOCBOOK_ELEMENT_START[]
    = "            <varlistentry>\n";
static const char DOCBOOK_ELEMENT_NAME_START[]
    = "                <term>";
static const char DOCBOOK_ELEMENT_NAME_END[]
    = "</term>\n";
static const char DOCBOOK_ELEMENT_ID_START[]
    = "                <listitem>\n"
      "                    <para><token>";
static const char DOCBOOK_ELEMENT_ID_END[]
    = "</token></para>\n";
static const char DOCBOOK_ELEMENT_DESCR_START[]  /* optional */
    = "                    <para>\n";
static const char DOCBOOK_ELEMENT_DESCR_END[]  /* optional */
    = "\n"
      "                    </para>\n";
static const char DOCBOOK_ELEMENT_END[]
    = "                </listitem>\n"
      "            </varlistentry>\n";
static const char DOCBOOK_ELEMENT_LIST_END[]
    = "        </variablelist>\n";

/* IO_FILE_FORMAT_XMI2 */

static const char XMI2_ENC[]
    = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.1 : 1,1a,1e,1f */
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.3.1.3 */
static const char XMI2_DOC_START[]
    = "<xmi:XMI xmlns:xmi=\"http://www.omg.org/spec/XMI/20131001\" "
      "xmlns:uml=\"http://www.omg.org/spec/UML/20161101\">\n";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.1 : 1,1a */
static const char XMI2_DOC_END[]
    = "</xmi:XMI>\n";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 7.5.3 + 7.5.5 */
static const char XMI2_DOC_METAINFO_START[]
    = "<xmi:Documentation exporter=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 7.5.3 + 7.5.5 */
static const char XMI2_DOC_METAINFO_MIDDLE[]
    = "\" exporterVersion=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 7.5.3 + 7.5.5 */
static const char XMI2_DOC_METAINFO_END[]
    = "\"/ >\n";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2e */
static const char XMI2_GENERIC_ID_START[]
    = "xmi:id=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2e */
static const char XMI2_GENERIC_ID_END[]
    = "\" ";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2e */
static const char XMI2_GENERIC_LABEL_START[]
    = "xmi:label=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2e */
static const char XMI2_GENERIC_LABEL_END[]
    = "\" ";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2g */
static const char XMI2_GENERIC_TYPE_START[]
    = "xmi:type=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2g */
static const char XMI2_GENERIC_TYPE_END[]
    = "\" ";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2l */
static const char XMI2_GENERIC_IDREF_START[]
    = "xmi:idref=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2l */
static const char XMI2_GENERIC_IDREF_END[]
    = "\" ";
/* spec-ref: not-found */
static const char XMI2_GENERIC_NAME_START[]
    = "name=\"";
/* spec-ref: not-found */
static const char XMI2_GENERIC_NAME_END[]
    = "\" ";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.5 */
static const char XMI2_UML_PACKAGE_START[]
    = "<uml:Package ";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.5 */
static const char XMI2_UML_PACKAGE_MIDDLE[]
    = ">\n";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.5 */
static const char XMI2_UML_PACKAGE_END[]
    = "</uml:Package>\n";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.5 */
static const char XMI2_UML_PACKAGED_ELEMENT_START[]
    = "    <packagedElement ";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.5 */
static const char XMI2_UML_PACKAGED_ELEMENT_MIDDLE[]
    = ">\n";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.5 */
static const char XMI2_UML_PACKAGED_ELEMENT_END[]
    = "    </packagedElement>\n";

/* IO_FILE_FORMAT_XHTML */

enum XHTML_DIAGRAM_MAX { XHTML_DIAGRAM_MAX_DEPTH = 6, };
static const char XHTML_ENC[]
    = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
static const char XHTML_DTD[]
    = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n";
static const char XHTML_DOC_START[]
    = "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
static const char XHTML_HEAD_START[]
    = "    <head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />";
static const char XHTML_HEAD_TITLE_START[]
    = "        <title>";
static const char XHTML_HEAD_TITLE_END[]
    = "</title>\n";
static const char XHTML_HEAD_CSS_START[]
    = "        <link rel=\"stylesheet\" type=\"text/css\" href=\"";
static const char XHTML_HEAD_CSS_END[]
    = ".css\" />\n";
static const char XHTML_HEAD_END[]
    = "    </head>\n";
static const char XHTML_BODY_START[]
    = "    <body>\n";
static const char XHTML_BODY_END[]
    = "    </body>\n";
static const char XHTML_DOC_END[]
    = "</html>\n";
static const char XHTML_TOC_SUBLIST_START[]
    = "        <ul class=\"toc\">\n";
static const char *XHTML_TOC_SUBLIST_ENTRY_START[XHTML_DIAGRAM_MAX_DEPTH]
    = {
      "            <li class=\"toc1\">\n",
      "            <li class=\"toc2\">\n",
      "            <li class=\"toc3\">\n",
      "            <li class=\"toc4\">\n",
      "            <li class=\"toc5\">\n",
      "            <li class=\"toc6\">\n"
      };
static const char XHTML_TOC_SUBLIST_ENTRY_TITLE_START[]
    = "                <a href=\"#";
static const char XHTML_TOC_SUBLIST_ENTRY_TITLE_MIDDLE[]
    = "\">";
static const char XHTML_TOC_SUBLIST_ENTRY_TITLE_END[]
    = "</a>\n";
static const char XHTML_TOC_SUBLIST_ENTRY_END[]
    = "            </li>\n";
static const char XHTML_TOC_SUBLIST_END[]
    = "        </ul>\n";

static const char XHTML_DIAGRAM_START[]
    = "        <div class=\"diagram\" id=\"";
static const char XHTML_DIAGRAM_MIDDLE[]
    = "\">\n";
static const char *XHTML_DIAGRAM_TITLE_START[XHTML_DIAGRAM_MAX_DEPTH]
    = {
      "            <h1 class=\"title\">",
      "            <h2 class=\"title\">",
      "            <h3 class=\"title\">",
      "            <h4 class=\"title\">",
      "            <h5 class=\"title\">",
      "            <h6 class=\"title\">"
      };
static const char *XHTML_DIAGRAM_TITLE_END[XHTML_DIAGRAM_MAX_DEPTH]
    = {
      "</h1>\n",
      "</h2>\n",
      "</h3>\n",
      "</h4>\n",
      "</h5>\n",
      "</h6>\n"
      };
static const char XHTML_DIAGRAM_IMG_START[]
    = "                <div class=\"mediaobject\"><img src=\"";
static const char XHTML_DIAGRAM_IMG_END[]
    = ".png\" alt=\"\" /></div>\n";
static const char XHTML_DIAGRAM_END[]
    = "        </div>\n";

static const char XHTML_DESCRIPTION_START[]
    = "            <div class=\"description\"><p>\n";
static const char XHTML_DESCRIPTION_MIDDLE[]  /* optional */
    = "\n"
      "            <br />\n";
static const char XHTML_DESCRIPTION_XREF_START[]
    = "<a href=\"#";
static const char XHTML_DESCRIPTION_XREF_MIDDLE[]
    = "\">";
static const char XHTML_DESCRIPTION_XREF_END[]
    = "</a>";
static const char XHTML_DESCRIPTION_END[]
    = "\n"
      "            </p></div>\n";

static const char XHTML_ELEMENT_LIST_START[]
    = "            <div class=\"element\">\n";
static const char XHTML_ELEMENT_START[]
    = "            <p>\n";
static const char XHTML_ELEMENT_NAME_START[]
    = "                <strong class=\"elementname\">";
static const char XHTML_ELEMENT_NAME_END[]
    = "</strong>\n";
static const char XHTML_ELEMENT_ID_START[]
    = "                <em class=\"elementid\">";
static const char XHTML_ELEMENT_ID_END[]
    = "</em>\n";
static const char XHTML_ELEMENT_DESCR_START[]  /* optional */
    = "\n"
      "            </p>\n"
      "            <p class=\"elementdescr\">\n";
static const char XHTML_ELEMENT_DESCR_END[]  /* optional */
    = "\n";
static const char XHTML_ELEMENT_END[]
    = "            </p>\n";
static const char XHTML_ELEMENT_LIST_END[]
    = "            </div>\n";

/* IO_FILE_FORMAT_TXT */

static const int TXT_ID_INDENT_COLUMN = 48;
static const char TXT_NEWLINE[] = "\n";
static const char TXT_SINGLE_INDENT[] = "| ";
static const char TXT_DOUBLE_INDENT[] = "  | ";
static const char TXT_SPACE_INDENT[] = "  ";
static const char TXT_COLON_SPACE[] = ": ";
static const char TXT_ARROW_SPACE[] = "--> ";
static const char TXT_SPACE_ARROW_SPACE[] = " --> ";

/* IO_FILE_FORMAT_CSS */

static const char CSS_ALL[]
="body {\n"
"    background-color: rgb(255,255,255);\n"
"    font-family: Helvetica,Arial,sans-serif;\n"
"}\n"
".toc1 {\n"
"    color:rgb(192,128,0);\n"
"    font-size: small;\n"
"    counter-reset: cnt-head-two;\n"
"}\n"
".toc2::before {\n"
"    counter-increment: cnt-head-two;\n"
"    content: counter(cnt-head-two) \"\\0000a0 \";\n"
"}\n"
".toc2 {\n"
"    color:rgb(192,128,0);\n"
"    font-size: small;\n"
"    counter-reset: cnt-head-three;\n"
"}\n"
".toc3::before {\n"
"    counter-increment: cnt-head-three;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \"\\0000a0 \";\n"
"}\n"
".toc3 {\n"
"    color:rgb(192,128,0);\n"
"    font-size: x-small;\n"
"    counter-reset: cnt-head-four;\n"
"}\n"
".toc4::before {\n"
"    counter-increment: cnt-head-four;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \"\\0000a0 \";\n"
"}\n"
".toc4 {\n"
"    color:rgb(192,128,0);\n"
"    font-size: x-small;\n"
"    counter-reset: cnt-head-five;\n"
"}\n"
".toc5::before {\n"
"    counter-increment: cnt-head-five;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \"\\0000a0 \";\n"
"}\n"
".toc5 {\n"
"    color:rgb(192,128,0);\n"
"    font-size: xx-small;\n"
"    counter-reset: cnt-head-six;\n"
"}\n"
".toc6::before {\n"
"    counter-increment: cnt-head-six;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \".\" counter(cnt-head-six) \"\\0000a0 \";\n"
"}\n"
".toc6 {\n"
"    color:rgb(192,128,0);\n"
"    font-size: xx-small;\n"
"}\n"
".description {\n"
"    background-color:rgb(216,255,240);\n"
"    padding:6px;\n"
"    margin:2px;\n"
"    border:1px solid #44CC88;\n"
"}\n"
".title {\n"
"    color:rgb(192,128,0);\n"
"}\n"
".element {\n"
"    background-color:rgb(240,240,240);\n"
"}\n"
".elementname {\n"
"    color:rgb(0,128,80);\n"
"}\n"
".elementid::before {\n"
"    content:\" \\0000a0 \\0000a0 {id=\";\n"
"}\n"
".elementid {\n"
"    color:rgb(160,160,160);\n"
"}\n"
".elementid::after {\n"
"    content:\"}\";\n"
"}\n"
".elementdescr {\n"
"    background-color:rgb(255,255,255);\n"
"    padding:6px;\n"
"    margin:2px;\n"
"    border:1px solid #CCCCCC;\n"
"}\n"
"h1 {\n"
"    counter-reset: cnt-head-two;\n"
"}\n"
"h2::before {\n"
"    counter-increment: cnt-head-two;\n"
"    content: counter(cnt-head-two) \"\\0000a0 \";\n"
"}\n"
"h2 {\n"
"    counter-reset: cnt-head-three;\n"
"}\n"
"h3::before {\n"
"    counter-increment: cnt-head-three;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \"\\0000a0 \";\n"
"}\n"
"h3 {\n"
"    counter-reset: cnt-head-four;\n"
"}\n"
"h4::before {\n"
"    counter-increment: cnt-head-four;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \"\\0000a0 \";\n"
"}\n"
"h4 {\n"
"    counter-reset: cnt-head-five;\n"
"}\n"
"h5::before {\n"
"    counter-increment: cnt-head-five;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \"\\0000a0 \";\n"
"}\n"
"h5 {\n"
"    counter-reset: cnt-head-six;\n"
"}\n"
"h6::before {\n"
"    counter-increment: cnt-head-six;\n"
"    content: counter(cnt-head-two) \".\" counter(cnt-head-three) \".\" counter(cnt-head-four) \".\" counter(cnt-head-five) \".\" counter(cnt-head-six) \"\\0000a0 \";\n"
"}\n";

void io_format_writer_init ( io_format_writer_t *this_,
                             data_database_reader_t *db_reader,
                             io_file_format_t export_type,
                             universal_output_stream_t *output )
{
    TRACE_BEGIN();
    assert( NULL != output );
    assert( NULL != db_reader );

    (*this_).export_type = export_type;
    (*this_).current_tree_depth = 0;

    txt_writer_init( &((*this_).txt_writer), output );
    xml_writer_init( &((*this_).xml_writer), output );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            md_filter_init( &((*this_).md_filter),
                            db_reader,
                            DOCBOOK_DESCRIPTION_MIDDLE,
                            DOCBOOK_DESCRIPTION_XREF_START,
                            DOCBOOK_DESCRIPTION_XREF_MIDDLE,
                            DOCBOOK_DESCRIPTION_XREF_END,
                            &((*this_).xml_writer)
                          );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            md_filter_init( &((*this_).md_filter),
                            db_reader,
                            XHTML_DESCRIPTION_MIDDLE,
                            XHTML_DESCRIPTION_XREF_START,
                            XHTML_DESCRIPTION_XREF_MIDDLE,
                            XHTML_DESCRIPTION_XREF_END,
                            &((*this_).xml_writer)
                          );
        }
        break;

        default:
        {
            md_filter_init( &((*this_).md_filter), db_reader, "\n", "", ": ", "", &((*this_).xml_writer) );
        }
        break;
    }

    TRACE_END();
}

void io_format_writer_destroy( io_format_writer_t *this_ )
{
    TRACE_BEGIN();

    md_filter_destroy( &((*this_).md_filter) );
    xml_writer_destroy( &((*this_).xml_writer) );
    txt_writer_destroy( &((*this_).txt_writer) );

    TRACE_END();
}

int io_format_writer_write_header( io_format_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    assert ( NULL != document_title );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ENC );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_TITLE_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_TITLE_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            /* xml header */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_ENC );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_START );
            /* meta info */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_METAINFO_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), META_INFO_PROGRAM_ID_STR );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_METAINFO_MIDDLE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), META_VERSION_STR );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_METAINFO_END );
            /* uml model */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGE_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_START );
/* TODO */            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), "uml:Model" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGE_MIDDLE );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ENC );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DTD );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DOC_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_TITLE_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_TITLE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_CSS_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_CSS_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_BODY_START );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no header */
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

int io_format_writer_start_toc_sublist ( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    /* increase tree depth */
    (*this_).current_tree_depth ++;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_TOC_SUBLIST_START );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_start_toc_entry ( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_XHTML:
        {
            const unsigned int index_of_depth = ((*this_).current_tree_depth > XHTML_DIAGRAM_MAX_DEPTH)
                ? (XHTML_DIAGRAM_MAX_DEPTH-1)
                : ((*this_).current_tree_depth-1);
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_TOC_SUBLIST_ENTRY_START[index_of_depth] );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_write_toc_entry ( io_format_writer_t *this_, const data_diagram_t *diag_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_XHTML:
        {
            const char *const diag_name = data_diagram_get_name_ptr(diag_ptr);
            const int64_t diag_id = data_diagram_get_id(diag_ptr);

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_TOC_SUBLIST_ENTRY_TITLE_START );
            export_err |= xml_writer_write_plain_id ( &((*this_).xml_writer), DATA_TABLE_DIAGRAM, diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_TOC_SUBLIST_ENTRY_TITLE_MIDDLE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_TOC_SUBLIST_ENTRY_TITLE_END );
        }
        break;


        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_end_toc_entry ( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_TOC_SUBLIST_ENTRY_END );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_end_toc_sublist ( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_TOC_SUBLIST_END );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    /* decrease tree depth */
    (*this_).current_tree_depth --;

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_start_diagram( io_format_writer_t *this_, int64_t diag_id )
{
    TRACE_BEGIN();
    int export_err = 0;

    /* increase tree depth */
    (*this_).current_tree_depth ++;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_START : DOCBOOK_DIAGRAM_START );
            export_err |= xml_writer_write_plain_id ( &((*this_).xml_writer), DATA_TABLE_DIAGRAM, diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_MIDDLE : DOCBOOK_DIAGRAM_MIDDLE );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n\n<!-- " );
            export_err |= xml_writer_write_plain_id ( &((*this_).xml_writer), DATA_TABLE_DIAGRAM, diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->\n" );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_START );
            export_err |= xml_writer_write_plain_id ( &((*this_).xml_writer), DATA_TABLE_DIAGRAM, diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_MIDDLE );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no start diagram tags */
        }
        break;

        default:
        {
            TSLOG_ERROR("error: unknown_format.");
            export_err = -1;
        }
        break;
    }
    if ( export_err != 0 )
    {
        TSLOG_ERROR( "not all bytes could be written" );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_write_diagram( io_format_writer_t *this_,
                                    const data_diagram_t *diag_ptr,
                                    const char *diagram_file_base_name )
{
    TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    assert ( NULL != diagram_file_base_name );
    int export_err = 0;

    const char *const diag_name = data_diagram_get_name_ptr( diag_ptr );
    const char *const diag_description = data_diagram_get_description_ptr( diag_ptr );
    const int64_t diag_id = data_diagram_get_id(diag_ptr);

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_TITLE_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_TITLE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= md_filter_transform ( &((*this_).md_filter), diag_description );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_IMG_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diagram_file_base_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_IMG_MIDDLE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diagram_file_base_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_IMG_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "<!-- " );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->\n" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "<!--\n" );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_description );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n-->\n" );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            const unsigned int index_of_depth = ((*this_).current_tree_depth > XHTML_DIAGRAM_MAX_DEPTH)
                ? (XHTML_DIAGRAM_MAX_DEPTH-1)
                : ((*this_).current_tree_depth-1);
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_TITLE_START[index_of_depth] );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_TITLE_END[index_of_depth] );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DESCRIPTION_START );
            export_err |= md_filter_transform ( &((*this_).md_filter), diag_description );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DESCRIPTION_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_IMG_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diagram_file_base_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_IMG_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), diag_name );
            export_err |= txt_writer_write_indent_id( &((*this_).txt_writer),
                                                      TXT_ID_INDENT_COLUMN - utf8string_get_length(diag_name),
                                                      DATA_TABLE_DIAGRAM,
                                                      diag_id
                                                    );
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= txt_writer_write_indent_multiline_string( &((*this_).txt_writer),
                                                                    TXT_SINGLE_INDENT,
                                                                    diag_description
                                                                  );
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

int io_format_writer_start_classifier( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_START );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n" );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_LIST_START );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
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

int io_format_writer_write_classifier( io_format_writer_t *this_, const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    int export_err = 0;

    const char *const classifier_name = data_classifier_get_name_ptr(classifier_ptr);
    const char *const classifier_descr = data_classifier_get_description_ptr(classifier_ptr);
    const size_t classifier_descr_len = utf8string_get_length(classifier_descr);
    const int64_t classifier_id = data_classifier_get_id(classifier_ptr);

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_CLASSIFIER,
                                                     classifier_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            if ( 0 != classifier_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), classifier_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGED_ELEMENT_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_START );
/* TODO */            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), "uml:Class" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_CLASSIFIER,
                                                     classifier_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_LABEL_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_LABEL_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGED_ELEMENT_MIDDLE );

            if ( 0 != classifier_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "    <!--\n    " );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n    -->\n" );
            }
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_CLASSIFIER,
                                                     classifier_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_END );
            if ( 0 != classifier_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), classifier_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), classifier_name );
            export_err |= txt_writer_write_indent_id( &((*this_).txt_writer),
                                                      TXT_ID_INDENT_COLUMN - utf8string_get_length(classifier_name),
                                                      DATA_TABLE_CLASSIFIER,
                                                      classifier_id
                                                    );
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= txt_writer_write_indent_multiline_string( &((*this_).txt_writer),
                                                                    TXT_SINGLE_INDENT,
                                                                    classifier_descr
                                                                  );
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

int io_format_writer_write_feature( io_format_writer_t *this_, const data_feature_t *feature_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    int export_err = 0;

    const char *const feature_key = data_feature_get_key_ptr( feature_ptr );
    const char *const feature_value = data_feature_get_value_ptr( feature_ptr );
    const size_t feature_value_len = utf8string_get_length(feature_value);
    const char *const feature_descr = data_feature_get_description_ptr( feature_ptr );
    const size_t feature_descr_len = utf8string_get_length(feature_descr);
    const int64_t feature_id = data_feature_get_id( feature_ptr );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            if ( 0 != feature_value_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_FEATURE,
                                                     feature_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            if ( 0 != feature_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), feature_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "        <!-- " );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_FEATURE,
                                                     feature_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->\n" );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "        <!-- " );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->\n" );

            if ( 0 != feature_value_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "        <!-- " );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->\n" );
            }

            if ( 0 != feature_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "        <!--\n" );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n-->\n" );
            }
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            if ( 0 != feature_value_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_FEATURE,
                                                     feature_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_END );
            if ( 0 != feature_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), feature_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_SPACE_INDENT );
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), feature_key );
            if ( 0 != feature_value_len )
            {
                export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_COLON_SPACE );
                export_err |= txt_writer_write_plain ( &((*this_).txt_writer), feature_value );
            }

            size_t feature_key_len = utf8string_get_length(feature_key);
            int id_indent_width = TXT_ID_INDENT_COLUMN - utf8string_get_length(TXT_SPACE_INDENT) - feature_key_len
                - ((feature_value_len==0)?0:feature_value_len+utf8string_get_length(TXT_COLON_SPACE));
            export_err |= txt_writer_write_indent_id( &((*this_).txt_writer),
                                                      id_indent_width,
                                                      DATA_TABLE_FEATURE,
                                                      feature_id
                                                    );
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= txt_writer_write_indent_multiline_string( &((*this_).txt_writer),
                                                                    TXT_DOUBLE_INDENT,
                                                                    feature_descr
                                                                  );
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

int io_format_writer_write_relationship( io_format_writer_t *this_,
                                         const data_relationship_t *relation_ptr,
                                         const data_classifier_t *dest_classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    assert ( NULL != dest_classifier_ptr );
    int export_err = 0;

    const char *const relation_name = data_relationship_get_name_ptr( relation_ptr );
    const int64_t relation_id = data_relationship_get_id( relation_ptr );
    const char *const relation_descr = data_relationship_get_description_ptr( relation_ptr );
    const size_t relation_descr_len = utf8string_get_length(relation_descr);
    const char *const dest_classifier_name = data_classifier_get_name_ptr( dest_classifier_ptr );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            /* list start */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_START );
            /* element */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), TXT_SPACE_ARROW_SPACE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), dest_classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_RELATIONSHIP,
                                                     relation_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            if ( 0 != relation_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), relation_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
            /* list end */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGED_ELEMENT_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_START );
/* TODO */            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), "uml:Association" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_RELATIONSHIP,
                                                     relation_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_LABEL_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_LABEL_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGED_ELEMENT_MIDDLE );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "        <!-- DESTINATION: " );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), dest_classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->\n" );

            if ( 0 != relation_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "        <!--\n" );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n-->\n" );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGED_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            /* list start */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_LIST_START );
            /* element */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), TXT_SPACE_ARROW_SPACE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), dest_classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer),
                                                     DATA_TABLE_RELATIONSHIP,
                                                     relation_id
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_END );
            if ( 0 != relation_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), relation_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_END );
            /* list end */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_LIST_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_SPACE_INDENT );
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), relation_name );

            size_t relation_name_len = utf8string_get_length(relation_name);
            /* print arrow */
            if ( relation_name_len == 0 )
            {
                export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_ARROW_SPACE );
            }
            else
            {
                export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_SPACE_ARROW_SPACE );
            }

            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), dest_classifier_name );

            /* print id */
            size_t dest_classifier_name_len = utf8string_get_length( dest_classifier_name );
            int id_indent_width = TXT_ID_INDENT_COLUMN - utf8string_get_length(TXT_SPACE_INDENT) - relation_name_len
                - ((relation_name_len==0)?utf8string_get_length(TXT_ARROW_SPACE):utf8string_get_length(TXT_SPACE_ARROW_SPACE))
                - dest_classifier_name_len;
            export_err |= txt_writer_write_indent_id( &((*this_).txt_writer),
                                                      id_indent_width,
                                                      DATA_TABLE_RELATIONSHIP,
                                                      relation_id
                                                    );

            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= txt_writer_write_indent_multiline_string( &((*this_).txt_writer),
                                                                    TXT_DOUBLE_INDENT,
                                                                    relation_descr
                                                                  );
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

int io_format_writer_end_classifier( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGED_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_LIST_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no end classifier tags */
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
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_END : DOCBOOK_DIAGRAM_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no end diagram tags */
        }
        break;

        default:
        {
            TSLOG_ERROR("error: unknown_format.");
            export_err = -1;
        }
        break;
    }

    /* decrease tree depth */
    (*this_).current_tree_depth --;

    TRACE_END_ERR( export_err );
    return export_err;
}

int io_format_writer_write_stylesheet( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_CSS:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), CSS_ALL );
        }
        break;

        default:
        {
            /* no stylesheet file */
            TSLOG_ERROR("error: format is not stylesheet.");
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
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_PACKAGE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_END );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_BODY_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DOC_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no footer */
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
