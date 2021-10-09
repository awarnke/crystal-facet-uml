/* File: xhtml_element_writer.c; Copyright and License: see below */

#include "xhtml/xhtml_element_writer.h"
#include "util/string/utf8string.h"
#include "data_id.h"
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
    = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
static const char DOCBOOK_DOC_START[]
    = "\n<book xmlns=\"http://docbook.org/ns/docbook\" version=\"5.0\" xml:lang=\"en\">";
static const char DOCBOOK_DOC_TITLE_START[]
    = "\n<title>";
static const char DOCBOOK_DOC_TITLE_END[]
    = "</title>";
static const char DOCBOOK_DOC_END[]
    = "\n</book>\n";

static const char DOCBOOK_TOP_DIAGRAM_START[]
    = "\n<chapter xml:id=\"";
static const char DOCBOOK_TOP_DIAGRAM_MIDDLE[]
    = "\">";
static const char DOCBOOK_TOP_DIAGRAM_END[]
    = "\n</chapter>";
static const char DOCBOOK_DIAGRAM_START[]
    = "\n<section xml:id=\"";
static const char DOCBOOK_DIAGRAM_MIDDLE[]
    = "\">";
static const char DOCBOOK_DIAGRAM_TITLE_START[]
    = "\n<title>";
static const char DOCBOOK_DIAGRAM_TITLE_END[]
    = "</title>";
static const char DOCBOOK_DIAGRAM_IMG_START[]
    = "\n<para>"
      "\n    <mediaobject>"
      "\n        <imageobject><imagedata fileref=\"";
static const char DOCBOOK_DIAGRAM_IMG_MIDDLE[]
    = ".pdf\" width=\"12cm\"/></imageobject>"
      "\n        <imageobject><imagedata fileref=\"";
static const char DOCBOOK_DIAGRAM_IMG_END[]
    = ".png\"/></imageobject>"
      "\n    </mediaobject>"
      "\n</para>";
static const char DOCBOOK_DIAGRAM_END[]
    = "\n</section>";

static const char DOCBOOK_DESCRIPTION_START[]
    = "\n<para>";
static const char DOCBOOK_DESCRIPTION_MIDDLE[]  /* optional */
    = "\n</para>"
      "\n<para>";
static const char DOCBOOK_DESCRIPTION_XREF_START[]
    = "<xref linkend=\"";
static const char DOCBOOK_DESCRIPTION_XREF_MIDDLE[]
    = "\"/>: ";
static const char DOCBOOK_DESCRIPTION_XREF_END[]
    = "";
static const char DOCBOOK_DESCRIPTION_END[]
    = "\n</para>";

static const char DOCBOOK_ELEMENT_LIST_START[]
    = "\n<variablelist>";
static const char DOCBOOK_ELEMENT_START[]
    = "\n<varlistentry>";
static const char DOCBOOK_ELEMENT_NAME_START[]
    = "\n<term>";
static const char DOCBOOK_ELEMENT_NAME_END[]
    = "</term>";
static const char DOCBOOK_ELEMENT_ID_START[]
    = "\n<listitem>"
      "\n    <para><token>";
static const char DOCBOOK_ELEMENT_ID_END[]
    = "</token></para>";
static const char DOCBOOK_ELEMENT_DESCR_START[]  /* optional */
    = "\n<para>";
static const char DOCBOOK_ELEMENT_DESCR_END[]  /* optional */
    = "\n</para>";
static const char DOCBOOK_ELEMENT_END[]
    = "\n    </listitem>"
      "\n</varlistentry>";
static const char DOCBOOK_ELEMENT_LIST_END[]
    = "\n</variablelist>";

/* IO_FILE_FORMAT_XHTML */

enum XHTML_DIAGRAM_MAX { XHTML_DIAGRAM_MAX_DEPTH = 6, };
static const char XHTML_ENC[]
    = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
static const char XHTML_DTD[]
    = "\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";
static const char XHTML_DOC_START[]
    = "\n<html xmlns=\"http://www.w3.org/1999/xhtml\">";
static const char XHTML_HEAD_START[]
    = "\n<head>"
      "\n    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />";
static const char XHTML_HEAD_TITLE_START[]
    = "\n<title>";
static const char XHTML_HEAD_TITLE_END[]
    = "</title>";
static const char XHTML_HEAD_CSS_START[]
    = "\n<link rel=\"stylesheet\" type=\"text/css\" href=\"";
static const char XHTML_HEAD_CSS_END[]
    = ".css\" />";
static const char XHTML_HEAD_END[]
    = "\n</head>";
static const char XHTML_BODY_START[]
    = "\n<body>";
static const char XHTML_BODY_END[]
    = "\n</body>";
static const char XHTML_DOC_END[]
    = "\n</html>\n";
static const char XHTML_TOC_SUBLIST_START[]
    = "\n<ul class=\"toc\">";
static const char *XHTML_TOC_SUBLIST_ENTRY_START[XHTML_DIAGRAM_MAX_DEPTH]
    = {
      "\n<li class=\"toc1\">",
      "\n<li class=\"toc2\">",
      "\n<li class=\"toc3\">",
      "\n<li class=\"toc4\">",
      "\n<li class=\"toc5\">",
      "\n<li class=\"toc6\">"
      };
static const char XHTML_TOC_SUBLIST_ENTRY_TITLE_START[]
    = "\n<a href=\"#";
static const char XHTML_TOC_SUBLIST_ENTRY_TITLE_MIDDLE[]
    = "\">";
static const char XHTML_TOC_SUBLIST_ENTRY_TITLE_END[]
    = "</a>";
static const char XHTML_TOC_SUBLIST_ENTRY_END[]
    = "\n</li>";
static const char XHTML_TOC_SUBLIST_END[]
    = "\n</ul>";

static const char XHTML_DIAGRAM_START[]
    = "\n<div class=\"diagram\" id=\"";
static const char XHTML_DIAGRAM_MIDDLE[]
    = "\">";
static const char *XHTML_DIAGRAM_TITLE_START[XHTML_DIAGRAM_MAX_DEPTH]
    = {
      "\n<h1 class=\"title\">",
      "\n<h2 class=\"title\">",
      "\n<h3 class=\"title\">",
      "\n<h4 class=\"title\">",
      "\n<h5 class=\"title\">",
      "\n<h6 class=\"title\">"
      };
static const char *XHTML_DIAGRAM_TITLE_END[XHTML_DIAGRAM_MAX_DEPTH]
    = {
      "</h1>",
      "</h2>",
      "</h3>",
      "</h4>",
      "</h5>",
      "</h6>"
      };
static const char XHTML_DIAGRAM_IMG_START[]
    = "\n<div class=\"mediaobject\"><img src=\"";
static const char XHTML_DIAGRAM_IMG_END[]
    = ".png\" alt=\"\" /></div>";
static const char XHTML_DIAGRAM_END[]
    = "\n</div>";

static const char XHTML_DESCRIPTION_START[]
    = "\n<div class=\"description\"><p>";
static const char XHTML_DESCRIPTION_MIDDLE[]  /* optional */
    = "\n<br />\n";
static const char XHTML_DESCRIPTION_XREF_START[]
    = "<a href=\"#";
static const char XHTML_DESCRIPTION_XREF_MIDDLE[]
    = "\">";
static const char XHTML_DESCRIPTION_XREF_END[]
    = "</a>";
static const char XHTML_DESCRIPTION_END[]
    = "\n</p></div>";

static const char XHTML_ELEMENT_LIST_START[]
    = "\n<div class=\"element\">";
static const char XHTML_ELEMENT_START[]
    = "\n<p>";
static const char XHTML_ELEMENT_NAME_START[]
    = "\n<strong class=\"elementname\">";
static const char XHTML_ELEMENT_NAME_END[]
    = "</strong>";
static const char XHTML_ELEMENT_ID_START[]
    = "\n<em class=\"elementid\">";
static const char XHTML_ELEMENT_ID_END[]
    = "</em>";
static const char XHTML_ELEMENT_DESCR_START[]  /* optional */
    = "\n</p>"
      "\n<p class=\"elementdescr\">\n";
static const char XHTML_ELEMENT_DESCR_END[]  /* optional */
    = "\n";
static const char XHTML_ELEMENT_END[]
    = "\n</p>";
static const char XHTML_ELEMENT_LIST_END[]
    = "\n</div>";

/* IO_FILE_FORMAT_TXT */

static const int TXT_ID_INDENT_COLUMN = 48;
static const char TXT_NEWLINE[] = "\n";
static const char TXT_SINGLE_INDENT[] = "| ";
static const char TXT_DOUBLE_INDENT[] = "  | ";
static const char TXT_SPACE_INDENT[] = "  ";
static const char TXT_COLON_SPACE[] = ": ";
static const char TXT_ARROW_SPACE[] = "--> ";
static const char TXT_SPACE_ARROW_SPACE[] = " --> ";

/* define a struct where the function pointers have the exact right signatures to avoid typecasts */
#define io_element_writer_impl_t xhtml_element_writer_t
struct xhtml_element_writer_io_element_writer_if_struct {
#include "io_element_writer_if.inl"
};
#undef io_element_writer_impl_t

/* the vmt implementing the interface */
static const struct xhtml_element_writer_io_element_writer_if_struct xhtml_element_writer_private_io_element_writer_if
    = {
        .write_header = &xhtml_element_writer_write_header,
        .start_main = &xhtml_element_writer_start_main,
        .can_classifier_nest_classifier = &xhtml_element_writer_can_classifier_nest_classifier,
        .can_classifier_nest_relationship = &xhtml_element_writer_can_classifier_nest_relationship,
        .start_classifier = &xhtml_element_writer_start_classifier,
        .assemble_classifier = &xhtml_element_writer_assemble_classifier,
        .end_classifier = &xhtml_element_writer_end_classifier,
        .start_feature = &xhtml_element_writer_start_feature,
        .assemble_feature = &xhtml_element_writer_assemble_feature,
        .end_feature = &xhtml_element_writer_end_feature,
        .start_relationship = &xhtml_element_writer_start_relationship,
        .assemble_relationship = &xhtml_element_writer_assemble_relationship,
        .end_relationship = &xhtml_element_writer_end_relationship,
        .start_diagram = &xhtml_element_writer_start_diagram,
        .assemble_diagram = &xhtml_element_writer_assemble_diagram,
        .end_diagram = &xhtml_element_writer_end_diagram,
        .start_diagramelement = &xhtml_element_writer_start_diagramelement,
        .assemble_diagramelement = &xhtml_element_writer_assemble_diagramelement,
        .end_diagramelement = &xhtml_element_writer_end_diagramelement,
        .end_main = &xhtml_element_writer_end_main,
        .write_footer = &xhtml_element_writer_write_footer
    };

void xhtml_element_writer_init ( xhtml_element_writer_t *this_,
                                 data_database_reader_t *db_reader,
                                 io_file_format_t export_type,
                                 data_stat_t *io_export_stat,
                                 universal_output_stream_t *output )
{
    TRACE_BEGIN();
    assert( NULL != output );
    assert( NULL != io_export_stat );
    assert( NULL != db_reader );

    io_element_writer_private_init( &((*this_).element_writer),
                                    (io_element_writer_if_t*) &xhtml_element_writer_private_io_element_writer_if,
                                    this_
                                  );
    (*this_).export_stat = io_export_stat;

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

void xhtml_element_writer_destroy( xhtml_element_writer_t *this_ )
{
    TRACE_BEGIN();

    md_filter_destroy( &((*this_).md_filter) );
    xml_writer_destroy( &((*this_).xml_writer) );
    txt_writer_destroy( &((*this_).txt_writer) );

    (*this_).export_stat = NULL;

    io_element_writer_private_destroy( &((*this_).element_writer) );

    TRACE_END();
}

io_element_writer_t * xhtml_element_writer_get_element_writer( xhtml_element_writer_t *this_ )
{
    TRACE_BEGIN();

    io_element_writer_t * base = &((*this_).element_writer);

    TRACE_END();
    return base;
}

int xhtml_element_writer_write_header( xhtml_element_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    assert ( NULL != document_title );
    int export_err = 0;

    xml_writer_reset_indent ( &((*this_).xml_writer) );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ENC );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_TITLE_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_TITLE_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ENC );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DTD );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DOC_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_TITLE_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_TITLE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_CSS_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_CSS_END );
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_HEAD_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_BODY_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
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

int xhtml_element_writer_start_main( xhtml_element_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    assert( document_title != NULL );
    int write_error = 0;
    TRACE_END_ERR(write_error);
    return write_error;
}

bool xhtml_element_writer_can_classifier_nest_classifier( xhtml_element_writer_t *this_,
                                                          data_classifier_type_t host_type,
                                                          data_classifier_type_t child_type )
{
    TRACE_BEGIN();
    const bool can_nest = false;
    TRACE_END();
    return can_nest;
}

bool xhtml_element_writer_can_classifier_nest_relationship( xhtml_element_writer_t *this_,
                                                            data_classifier_type_t host_type,
                                                            data_relationship_type_t child_type )
{
    TRACE_BEGIN();
    const bool can_nest = true;
    TRACE_END();
    return can_nest;
}

int xhtml_element_writer_start_toc_sublist ( xhtml_element_writer_t *this_ )
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
            xml_writer_increase_indent ( &((*this_).xml_writer) );
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

int xhtml_element_writer_start_toc_entry ( xhtml_element_writer_t *this_ )
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
            xml_writer_increase_indent ( &((*this_).xml_writer) );
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

int xhtml_element_writer_write_toc_entry ( xhtml_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_XHTML:
        {
            const char *const diag_name = data_diagram_get_name_const(diag_ptr);
            const data_id_t diag_id = data_diagram_get_data_id(diag_ptr);

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_TOC_SUBLIST_ENTRY_TITLE_START );
            export_err |= xml_writer_write_plain_id ( &((*this_).xml_writer), diag_id );
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

int xhtml_element_writer_end_toc_entry ( xhtml_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_XHTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
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

int xhtml_element_writer_end_toc_sublist ( xhtml_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_XHTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
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

int xhtml_element_writer_start_classifier( xhtml_element_writer_t *this_,
                                           data_classifier_type_t host_type,
                                           const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
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

int xhtml_element_writer_assemble_classifier( xhtml_element_writer_t *this_,
                                              data_classifier_type_t host_type,
                                              const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    int export_err = 0;

    const char *const classifier_name = data_classifier_get_name_const(classifier_ptr);
    const char *const classifier_descr = data_classifier_get_description_const(classifier_ptr);
    const size_t classifier_descr_len = utf8string_get_length(classifier_descr);
    const data_id_t classifier_id = data_classifier_get_data_id(classifier_ptr);

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            if ( 0 != classifier_descr_len )
            {
                xml_writer_increase_indent ( &((*this_).xml_writer) );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), classifier_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
                xml_writer_decrease_indent ( &((*this_).xml_writer) );
            }
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
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

int xhtml_element_writer_end_classifier( xhtml_element_writer_t *this_,
                                         data_classifier_type_t host_type,
                                         const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
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

int xhtml_element_writer_start_feature( xhtml_element_writer_t *this_,
                                        data_classifier_type_t parent_type,
                                        const data_feature_t *feature_ptr)
{
    TRACE_BEGIN();
    assert( feature_ptr != NULL );
    int write_error = 0;
    TRACE_END_ERR(write_error);
    return write_error;
}

int xhtml_element_writer_assemble_feature( xhtml_element_writer_t *this_,
                                           data_classifier_type_t parent_type,
                                           const data_feature_t *feature_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    int export_err = 0;

    const char *const feature_key = data_feature_get_key_const( feature_ptr );
    const char *const feature_value = data_feature_get_value_const( feature_ptr );
    const size_t feature_value_len = utf8string_get_length(feature_value);
    const char *const feature_descr = data_feature_get_description_const( feature_ptr );
    const size_t feature_descr_len = utf8string_get_length(feature_descr);
    const data_id_t feature_id = data_feature_get_data_id( feature_ptr );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            if ( 0 != feature_value_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), feature_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            if ( 0 != feature_descr_len )
            {
                xml_writer_increase_indent ( &((*this_).xml_writer) );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), feature_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
                xml_writer_decrease_indent ( &((*this_).xml_writer) );
            }
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            if ( 0 != feature_value_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), feature_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_ID_END );
            if ( 0 != feature_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), feature_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_ELEMENT_DESCR_END );
            }
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
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

int xhtml_element_writer_end_feature( xhtml_element_writer_t *this_,
                                      data_classifier_type_t parent_type,
                                      const data_feature_t *feature_ptr)
{
    TRACE_BEGIN();
    assert( feature_ptr != NULL );
    int write_error = 0;
    TRACE_END_ERR(write_error);
    return write_error;
}

int xhtml_element_writer_start_relationship( xhtml_element_writer_t *this_,
                                             data_classifier_type_t host_type,
                                             const data_relationship_t *relation_ptr)
{
    TRACE_BEGIN();
    assert( relation_ptr != NULL );
    int write_error = 0;
    TRACE_END_ERR(write_error);
    return write_error;
}

int xhtml_element_writer_assemble_relationship( xhtml_element_writer_t *this_,
                                                const data_classifier_t *host,
                                                const data_relationship_t *relation_ptr,
                                                const data_classifier_t *from_c,
                                                const data_feature_t *from_f,
                                                const data_classifier_t *to_c,
                                                const data_feature_t *to_f )
{
    TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    /* NULL is allowed here: dest_classifier_ptr */
    int export_err = 0;

    const char *const relation_name = data_relationship_get_name_const( relation_ptr );
    const data_id_t relation_id = data_relationship_get_data_id( relation_ptr );
    const char *const relation_descr = data_relationship_get_description_const( relation_ptr );
    const size_t relation_descr_len = utf8string_get_length(relation_descr);
    const char *const dest_classifier_name
        = (NULL != to_c)
        ? data_classifier_get_name_const( to_c )
        : "";

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            /* list start */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            /* element */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), TXT_SPACE_ARROW_SPACE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), dest_classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            if ( 0 != relation_descr_len )
            {
                xml_writer_increase_indent ( &((*this_).xml_writer) );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), relation_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
                xml_writer_decrease_indent ( &((*this_).xml_writer) );
            }
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
            /* list end */
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
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
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
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

int xhtml_element_writer_end_relationship( xhtml_element_writer_t *this_,
                                           data_classifier_type_t host_type,
                                           const data_relationship_t *relation_ptr)
{
    TRACE_BEGIN();
    assert( relation_ptr != NULL );
    int write_error = 0;
    TRACE_END_ERR(write_error);
    return write_error;
}

int xhtml_element_writer_start_diagram( xhtml_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    int export_err = 0;

    const data_id_t diag_id = data_diagram_get_data_id(diag_ptr);

    /* increase tree depth */
    (*this_).current_tree_depth ++;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_START : DOCBOOK_DIAGRAM_START );
            export_err |= xml_writer_write_plain_id ( &((*this_).xml_writer), diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_MIDDLE : DOCBOOK_DIAGRAM_MIDDLE );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_START );
            export_err |= xml_writer_write_plain_id ( &((*this_).xml_writer), diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XHTML_DIAGRAM_MIDDLE );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
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

int xhtml_element_writer_assemble_diagram( xhtml_element_writer_t *this_,
                                           const data_diagram_t *diag_ptr,
                                           const char *diagram_file_base_name )
{
    TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    assert ( NULL != diagram_file_base_name );
    int export_err = 0;

    const char *const diag_name = data_diagram_get_name_const( diag_ptr );
    const char *const diag_description = data_diagram_get_description_const( diag_ptr );
    const data_id_t diag_id = data_diagram_get_data_id(diag_ptr);

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
            assert(false);  /* use the xmi_element_writer instead */
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

int xhtml_element_writer_end_diagram( xhtml_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_END : DOCBOOK_DIAGRAM_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
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

int xhtml_element_writer_start_diagramelement( xhtml_element_writer_t *this_,
                                               const data_diagramelement_t *diagramelement_ptr,
                                               const data_diagram_t *parent,
                                               const data_classifier_t *occurrence)
{
    TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    assert( occurrence != NULL );
    int write_error = 0;

    TRACE_END_ERR(write_error);
    return write_error;
}

int xhtml_element_writer_assemble_diagramelement( xhtml_element_writer_t *this_,
                                                  const data_diagramelement_t *diagramelement_ptr,
                                                  const data_diagram_t *parent,
                                                  const data_classifier_t *occurrence)
{
    TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    assert( occurrence != NULL );
    int write_error = 0;

    TRACE_END_ERR(write_error);
    return write_error;
}

int xhtml_element_writer_end_diagramelement( xhtml_element_writer_t *this_,
                                             const data_diagramelement_t *diagramelement_ptr,
                                             const data_diagram_t *parent,
                                             const data_classifier_t *occurrence)
{
    TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    assert( occurrence != NULL );
    int write_error = 0;

    TRACE_END_ERR(write_error);
    return write_error;
}

int xhtml_element_writer_end_main( xhtml_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int write_error = 0;

    TRACE_END_ERR(write_error);
    return write_error;
}

int xhtml_element_writer_write_footer( xhtml_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_END );
        }
        break;

        case IO_FILE_FORMAT_XMI2:
        {
            assert(false);  /* use the xmi_element_writer instead */
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
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
Copyright 2017-2021 Andreas Warnke

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
