/* File: document_element_writer.c; Copyright and License: see below */

#include "document/document_element_writer.h"
#include "utf8stringbuf/utf8string.h"
#include "data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
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

/* IO_FILE_FORMAT_HTML */

enum HTML_DIAGRAM_MAX { HTML_DIAGRAM_MAX_DEPTH = 6, };
static const char HTML_DTD[]
    = "<!DOCTYPE html>";
static const char HTML_DOC_START[]
    = "\n<html xmlns=\"http://www.w3.org/1999/xhtml\">";
static const char HTML_HEAD_START[]
    = "\n<head>"
      "\n    <meta charset=\"UTF-8\" />";
      /* "\n    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />"; < old */
static const char HTML_HEAD_TITLE_START[]
    = "\n<title>";
static const char HTML_HEAD_TITLE_END[]
    = "</title>";
static const char HTML_HEAD_CSS_START[]
    = "\n<link rel=\"stylesheet\" type=\"text/css\" href=\"";
static const char HTML_HEAD_CSS_END[]
    = ".css\" />";
static const char HTML_HEAD_END[]
    = "\n</head>";
static const char HTML_BODY_START[]
    = "\n<body>";
static const char HTML_NAV_START[]
    = "\n<nav>";
static const char HTML_NAV_END[]
    = "\n</nav>";
static const char HTML_ARTICLE_START[]
    = "\n<main>"
      "\n<article>";
static const char HTML_ARTICLE_END[]
    = "\n</article>"
      "\n</main>";
static const char HTML_BODY_END[]
    = "\n</body>";
static const char HTML_DOC_END[]
    = "\n</html>\n";
static const char HTML_TOC_SUBLIST_START[]
    = "\n<ul class=\"toc\">";
static const char *HTML_TOC_SUBLIST_ENTRY_START[HTML_DIAGRAM_MAX_DEPTH]
    = {
      "\n<li class=\"toc1\">",
      "\n<li class=\"toc2\">",
      "\n<li class=\"toc3\">",
      "\n<li class=\"toc4\">",
      "\n<li class=\"toc5\">",
      "\n<li class=\"toc6\">"
      };
static const char HTML_TOC_SUBLIST_ENTRY_TITLE_START[]
    = "\n<a href=\"#";
static const char HTML_TOC_SUBLIST_ENTRY_TITLE_MIDDLE[]
    = "\">";
static const char HTML_TOC_SUBLIST_ENTRY_TITLE_END[]
    = "</a>";
static const char HTML_TOC_SUBLIST_ENTRY_END[]
    = "\n</li>";
static const char HTML_TOC_SUBLIST_END[]
    = "\n</ul>";

static const char *HTML_DIAGRAM_TITLE_START[HTML_DIAGRAM_MAX_DEPTH]
    = {
      "\n\n<h1 id=\"",
      "\n\n<h2 id=\"",
      "\n\n<h3 id=\"",
      "\n\n<h4 id=\"",
      "\n\n<h5 id=\"",
      "\n\n<h6 id=\""
      };
static const char HTML_DIAGRAM_TITLE_MIDDLE[]
    = "\" class=\"diag-title\">";
static const char *HTML_DIAGRAM_TITLE_END[HTML_DIAGRAM_MAX_DEPTH]
    = {
      "</h1>",
      "</h2>",
      "</h3>",
      "</h4>",
      "</h5>",
      "</h6>"
      };
static const char HTML_DIAGRAM_IMG_START[]
    = "\n<div class=\"mediaobject\"><img src=\"";
static const char HTML_DIAGRAM_IMG_END[]
    = ".png\" width=\"840\" alt=\"\" /></div>";

static const char HTML_ANY_DESCR_NEWLINE[]
    = "\n<br />\n";
static const char HTML_ANY_DESCR_XREF_START[]
    = "<a href=\"#";
static const char HTML_ANY_DESCR_XREF_MIDDLE[]
    = "\">";
static const char HTML_ANY_DESCR_XREF_END[]
    = "</a>";

static const char HTML_DIAG_LIST_START[] = "\n<div class=\"diag\">";
static const char HTML_DIAG_START[] = "<p>";  /* hint: no whitespace before p */
static const char HTML_DIAG_NAME_START[] = "<strong class=\"diag-name\">";
static const char HTML_DIAG_NAME_END[] = "</strong>";
static const char HTML_DIAG_STEREO_START[] = "<em class=\"diag-stereo\">";
static const char HTML_DIAG_STEREO_END[] = "</em>";
static const char HTML_DIAG_TYPE_START[] = "<em class=\"diag-type\">";
static const char HTML_DIAG_TYPE_END[] = "</em>";
static const char HTML_DIAG_ID_START[] = "<em class=\"diag-id\">";
static const char HTML_DIAG_ID_END[] = "</em>";
static const char HTML_DIAG_DESCR_START[] = "\n</p><p class=\"diag-descr\">\n";  /* hint: no whitespace before p */
static const char HTML_DIAG_DESCR_END[] = "\n";
static const char HTML_DIAG_END[] = "\n</p>";
static const char HTML_DIAG_LIST_END[] = "</div>";  /* hint: no whitespace after p */

static const char HTML_CLAS_LIST_START[] = "\n<div class=\"clas\">";
static const char HTML_CLAS_START[] = "<p>";  /* hint: no whitespace before p */
static const char HTML_CLAS_NAME_START[] = "<strong class=\"clas-name\">";
static const char HTML_CLAS_NAME_END[] = "</strong>";
static const char HTML_CLAS_STEREO_START[] = "<em class=\"clas-stereo\">";
static const char HTML_CLAS_STEREO_END[] = "</em>";
static const char HTML_CLAS_TYPE_START[] = "<em class=\"clas-type\">";
static const char HTML_CLAS_TYPE_END[] = "</em>";
static const char HTML_CLAS_ID_START[] = "<em class=\"clas-id\">";
static const char HTML_CLAS_ID_END[] = "</em>";
static const char HTML_CLAS_DESCR_START[] = "\n</p><p class=\"clas-descr\">\n";  /* hint: no whitespace before p */
static const char HTML_CLAS_DESCR_END[] = "\n";
static const char HTML_CLAS_END[] = "\n</p>";
static const char HTML_CLAS_LIST_END[] = "</div>";  /* hint: no whitespace after p */

static const char HTML_FEAT_LIST_START[] = "\n<div class=\"feat\">";
static const char HTML_FEAT_START[] = "<p>";  /* hint: no whitespace before p */
static const char HTML_FEAT_NAME_START[] = "<strong class=\"feat-name\">";
static const char HTML_FEAT_NAME_END[] = "</strong>";
static const char HTML_FEAT_STEREO_START[] = "<em class=\"feat-stereo\">";
static const char HTML_FEAT_STEREO_END[] = "</em>";
static const char HTML_FEAT_TYPE_START[] = "<em class=\"feat-type\">";
static const char HTML_FEAT_TYPE_END[] = "</em>";
static const char HTML_FEAT_ID_START[] = "<em class=\"feat-id\">";
static const char HTML_FEAT_ID_END[] = "</em>";
static const char HTML_FEAT_DESCR_START[] = "\n</p><p class=\"feat-descr\">\n";  /* hint: no whitespace before p */
static const char HTML_FEAT_DESCR_END[] = "\n";
static const char HTML_FEAT_END[] = "\n</p>";
static const char HTML_FEAT_LIST_END[] = "</div>";  /* hint: no whitespace after p */

static const char HTML_REL_LIST_START[] = "\n<div class=\"rel\">";
static const char HTML_REL_START[] = "<p>";  /* hint: no whitespace before p */
static const char HTML_REL_NAME_START[] = "<strong class=\"rel-name\">";
static const char HTML_REL_NAME_END[] = "</strong>";
static const char HTML_REL_STEREO_START[] = "<em class=\"rel-stereo\">";
static const char HTML_REL_STEREO_END[] = "</em>";
static const char HTML_REL_TYPE_START[] = "<em class=\"rel-type\">";
static const char HTML_REL_TYPE_END[] = "</em>";
static const char HTML_REL_ID_START[] = "<em class=\"rel-id\">";
static const char HTML_REL_ID_END[] = "</em>";
static const char HTML_REL_DESCR_START[] = "\n</p><p class=\"rel-descr\">\n";  /* hint: no whitespace before p */
static const char HTML_REL_DESCR_END[] = "\n";
static const char HTML_REL_END[] = "\n</p>";
static const char HTML_REL_LIST_END[] = "</div>";  /* hint: no whitespace after p */

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
#define io_element_writer_impl_t document_element_writer_t
struct document_element_writer_io_element_writer_if_struct {
#include "io_element_writer_if.inl"
};
#undef io_element_writer_impl_t

/* the vmt implementing the interface */
static const struct document_element_writer_io_element_writer_if_struct document_element_writer_private_io_element_writer_if
    = {
        .write_header = &document_element_writer_write_header,
        .start_main = &document_element_writer_start_main,
        .can_classifier_nest_classifier = &document_element_writer_can_classifier_nest_classifier,
        .can_classifier_nest_relationship = &document_element_writer_can_classifier_nest_relationship,
        .start_classifier = &document_element_writer_start_classifier,
        .assemble_classifier = &document_element_writer_assemble_classifier,
        .end_classifier = &document_element_writer_end_classifier,
        .start_feature = &document_element_writer_start_feature,
        .assemble_feature = &document_element_writer_assemble_feature,
        .end_feature = &document_element_writer_end_feature,
        .start_relationship = &document_element_writer_start_relationship,
        .assemble_relationship = &document_element_writer_assemble_relationship,
        .end_relationship = &document_element_writer_end_relationship,
        .start_diagram = &document_element_writer_start_diagram,
        .assemble_diagram = &document_element_writer_assemble_diagram,
        .end_diagram = &document_element_writer_end_diagram,
        .start_diagramelement = &document_element_writer_start_diagramelement,
        .assemble_diagramelement = &document_element_writer_assemble_diagramelement,
        .end_diagramelement = &document_element_writer_end_diagramelement,
        .end_main = &document_element_writer_end_main,
        .write_footer = &document_element_writer_write_footer
    };

void document_element_writer_init ( document_element_writer_t *this_,
                                    data_database_reader_t *db_reader,
                                    io_file_format_t export_type,
                                    data_stat_t *io_export_stat,
                                    universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );
    assert( NULL != io_export_stat );
    assert( NULL != db_reader );

    io_element_writer_private_init( &((*this_).element_writer),
                                    (io_element_writer_if_t*) &document_element_writer_private_io_element_writer_if,
                                    this_
                                  );
    (*this_).export_stat = io_export_stat;

    (*this_).export_type = export_type;
    (*this_).current_tree_depth = 0;

    json_type_name_map_init( &((*this_).type_map) );

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

        case IO_FILE_FORMAT_HTML:
        {
            md_filter_init( &((*this_).md_filter),
                            db_reader,
                            HTML_ANY_DESCR_NEWLINE,
                            HTML_ANY_DESCR_XREF_START,
                            HTML_ANY_DESCR_XREF_MIDDLE,
                            HTML_ANY_DESCR_XREF_END,
                            &((*this_).xml_writer)
                          );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            md_filter_init( &((*this_).md_filter), db_reader, "\n", "", ": ", "", &((*this_).xml_writer) );
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            assert(false);  /* use another io_element_writer instead */
            md_filter_init( &((*this_).md_filter), db_reader, "\n", "", ": ", "", &((*this_).xml_writer) );
        }
        break;
    }

    U8_TRACE_END();
}

void document_element_writer_destroy( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    md_filter_destroy( &((*this_).md_filter) );
    xml_writer_destroy( &((*this_).xml_writer) );
    txt_writer_destroy( &((*this_).txt_writer) );

    json_type_name_map_destroy( &((*this_).type_map) );

    (*this_).export_stat = NULL;

    io_element_writer_private_destroy( &((*this_).element_writer) );

    U8_TRACE_END();
}

io_element_writer_t * document_element_writer_get_element_writer( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    io_element_writer_t * base = &((*this_).element_writer);

    U8_TRACE_END();
    return base;
}

u8_error_t document_element_writer_write_header( document_element_writer_t *this_, const char *document_title )
{
    U8_TRACE_BEGIN();
    assert ( NULL != document_title );
    u8_error_t export_err = U8_ERROR_NONE;

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

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DTD );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DOC_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_TITLE_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_TITLE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_CSS_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_CSS_END );
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_BODY_START );
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
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_start_main( document_element_writer_t *this_, const char *document_title )
{
    U8_TRACE_BEGIN();
    assert( document_title != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            /* no main start */
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_ARTICLE_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no main start */
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    U8_TRACE_END_ERR(export_err);
    return export_err;
}

bool document_element_writer_can_classifier_nest_classifier( document_element_writer_t *this_,
                                                             data_classifier_type_t host_type,
                                                             data_classifier_type_t child_type )
{
    U8_TRACE_BEGIN();
    const bool can_nest = false;
    U8_TRACE_END();
    return can_nest;
}

bool document_element_writer_can_classifier_nest_relationship( document_element_writer_t *this_,
                                                               data_classifier_type_t host_type,
                                                               data_relationship_type_t child_type )
{
    U8_TRACE_BEGIN();
    const bool can_nest = true;
    U8_TRACE_END();
    return can_nest;
}

u8_error_t document_element_writer_start_toc ( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_NAV_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_start_toc_sublist ( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    /* increase tree depth */
    (*this_).current_tree_depth ++;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_start_toc_entry ( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            const unsigned int index_of_depth = ((*this_).current_tree_depth > HTML_DIAGRAM_MAX_DEPTH)
                ? (HTML_DIAGRAM_MAX_DEPTH-1)
                : ((*this_).current_tree_depth-1);
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_START[index_of_depth] );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_write_toc_entry ( document_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            const char *const diag_name = data_diagram_get_name_const(diag_ptr);
            const data_id_t diag_id = data_diagram_get_data_id(diag_ptr);

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_TITLE_START );
            export_err |= xml_writer_write_plain_id ( &((*this_).xml_writer), diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_TITLE_MIDDLE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_TITLE_END );
        }
        break;


        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_end_toc_entry ( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_END );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_end_toc_sublist ( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_END );
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

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_end_toc ( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_NAV_END );
        }
        break;

        default:
        {
            /* nothing to do, only xhtml provides a table of contents */
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_start_classifier( document_element_writer_t *this_,
                                                     data_classifier_type_t host_type,
                                                     const data_classifier_t *classifier_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_LIST_START );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_assemble_classifier( document_element_writer_t *this_,
                                                        data_classifier_type_t host_type,
                                                        const data_classifier_t *classifier_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    const char *const classifier_name = data_classifier_get_name_const(classifier_ptr);
    const char *const classifier_descr = data_classifier_get_description_const(classifier_ptr);
    const size_t classifier_descr_len = utf8string_get_length(classifier_descr);
    const data_id_t classifier_id = data_classifier_get_data_id(classifier_ptr);
    const char *const classifier_type_name
        = json_type_name_map_get_classifier_type( &((*this_).type_map),
                                                  host_type,
                                                  data_classifier_get_main_type( classifier_ptr )
                                                );
    const char *const classifier_stereotype = data_classifier_get_stereotype_const( classifier_ptr );

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

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_START );
            if ( 0 != utf8string_get_length( classifier_stereotype ) )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_STEREO_START );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_stereotype );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_STEREO_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_TYPE_START );
            export_err |= xml_writer_write_xml_enc( &((*this_).xml_writer), classifier_type_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_TYPE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_ID_END );
            if ( 0 != classifier_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), classifier_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_LIST_START );
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
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    /* update export statistics */
    data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_EXPORTED );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_end_classifier( document_element_writer_t *this_,
                                                   data_classifier_type_t host_type,
                                                   const data_classifier_t *classifier_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_LIST_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_LIST_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no end classifier tags */
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_start_feature( document_element_writer_t *this_,
                                                  data_classifier_type_t parent_type,
                                                  const data_feature_t *feature_ptr)
{
    U8_TRACE_BEGIN();
    assert( feature_ptr != NULL );
    u8_error_t write_error = U8_ERROR_NONE;
    U8_TRACE_END_ERR(write_error);
    return write_error;
}

u8_error_t document_element_writer_assemble_feature( document_element_writer_t *this_,
                                                     const data_classifier_t *parent,
                                                     const data_feature_t *feature_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    assert( parent != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    const char *const feature_key = data_feature_get_key_const( feature_ptr );
    const char *const feature_value = data_feature_get_value_const( feature_ptr );
    const size_t feature_value_len = utf8string_get_length(feature_value);
    const char *const feature_descr = data_feature_get_description_const( feature_ptr );
    const size_t feature_descr_len = utf8string_get_length(feature_descr);
    const data_id_t feature_id = data_feature_get_data_id( feature_ptr );
    const data_classifier_type_t parent_type = data_classifier_get_main_type( parent );
    const char *const feature_type_name
        = json_type_name_map_get_feature_type( &((*this_).type_map),
                                               parent_type,
                                               data_feature_get_main_type( feature_ptr )
                                             );

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

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_NAME_END );
            if ( 0 != feature_value_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_STEREO_START );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_STEREO_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_TYPE_START );
            export_err |= xml_writer_write_xml_enc( &((*this_).xml_writer), feature_type_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_TYPE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), feature_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_ID_END );
            if ( 0 != feature_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), feature_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_DESCR_END );
            }
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_END );
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
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    /* update export statistics */
    const data_feature_type_t feat_type = data_feature_get_main_type( feature_ptr );
    const data_stat_table_t feat_or_lifeline
        = ( feat_type == DATA_FEATURE_TYPE_LIFELINE ) ? DATA_STAT_TABLE_LIFELINE : DATA_STAT_TABLE_FEATURE;
    data_stat_inc_count ( (*this_).export_stat, feat_or_lifeline, DATA_STAT_SERIES_EXPORTED );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_end_feature( document_element_writer_t *this_,
                                                data_classifier_type_t parent_type,
                                                const data_feature_t *feature_ptr)
{
    U8_TRACE_BEGIN();
    assert( feature_ptr != NULL );
    u8_error_t write_error = U8_ERROR_NONE;
    U8_TRACE_END_ERR(write_error);
    return write_error;
}

u8_error_t document_element_writer_start_relationship( document_element_writer_t *this_,
                                                       data_classifier_type_t host_type,
                                                       const data_relationship_t *relation_ptr)
{
    U8_TRACE_BEGIN();
    assert( relation_ptr != NULL );
    u8_error_t write_error = U8_ERROR_NONE;
    U8_TRACE_END_ERR(write_error);
    return write_error;
}

u8_error_t document_element_writer_assemble_relationship( document_element_writer_t *this_,
                                                          const data_classifier_t *host,
                                                          const data_relationship_t *relation_ptr,
                                                          const data_classifier_t *from_c,
                                                          const data_feature_t *from_f,
                                                          const data_classifier_t *to_c,
                                                          const data_feature_t *to_f )
{
    U8_TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    /* NULL is allowed here: dest_classifier_ptr */
    u8_error_t export_err = U8_ERROR_NONE;

    const char *const relation_name = data_relationship_get_name_const( relation_ptr );
    const data_id_t relation_id = data_relationship_get_data_id( relation_ptr );
    const char *const relation_descr = data_relationship_get_description_const( relation_ptr );
    const size_t relation_descr_len = utf8string_get_length(relation_descr);
    const char *const dest_classifier_name
        = (NULL != to_c)
        ? data_classifier_get_name_const( to_c )
        : "";
    const bool from_c_valid = ( from_c == NULL ) ? false : data_classifier_is_valid( from_c );
    const bool to_c_valid = ( to_c == NULL ) ? false : data_classifier_is_valid( to_c );
    const bool statemachine_context
        = (from_c_valid
        && ((data_classifier_get_main_type( from_c ) == DATA_CLASSIFIER_TYPE_STATE)
        || (data_classifier_get_main_type( from_c ) == DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY)
        || (data_classifier_get_main_type( from_c ) == DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY)))
        || (to_c_valid
        && (( data_classifier_get_main_type( to_c ) == DATA_CLASSIFIER_TYPE_STATE)
        || (data_classifier_get_main_type( to_c ) == DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY)
        || (data_classifier_get_main_type( to_c ) == DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY)));
    const char *const relation_type_name
        = json_type_name_map_get_relationship_type( &((*this_).type_map),
                                                    statemachine_context,
                                                    data_relationship_get_main_type( relation_ptr )
                                                  );
    const char *const relation_stereotype = data_relationship_get_stereotype_const( relation_ptr );

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

        case IO_FILE_FORMAT_HTML:
        {
            /* list start */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_LIST_START );
            /* element */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_START );
            if ( 0 != utf8string_get_length( relation_stereotype ) )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_STEREO_START );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_stereotype );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_STEREO_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_NAME_END );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), TXT_SPACE_ARROW_SPACE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), dest_classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_TYPE_START );
            export_err |= xml_writer_write_xml_enc( &((*this_).xml_writer), relation_type_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_TYPE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_ID_END );
            if ( 0 != relation_descr_len )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), relation_descr );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_END );
            /* list end */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_LIST_END );
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
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    /* update export statistics */
    data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_EXPORTED );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_end_relationship( document_element_writer_t *this_,
                                                     data_classifier_type_t host_type,
                                                     const data_relationship_t *relation_ptr)
{
    U8_TRACE_BEGIN();
    assert( relation_ptr != NULL );
    u8_error_t write_error = U8_ERROR_NONE;
    U8_TRACE_END_ERR(write_error);
    return write_error;
}

u8_error_t document_element_writer_start_diagram( document_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

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

        case IO_FILE_FORMAT_HTML:
        {
            /* no start diagram tags */
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no start diagram tags */
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }
    if ( export_err != 0 )
    {
        U8_LOG_ERROR( "not all bytes could be written" );
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_assemble_diagram( document_element_writer_t *this_,
                                                     const data_diagram_t *parent,
                                                     const data_diagram_t *diag_ptr,
                                                     const char *diagram_file_base_name )
{
    U8_TRACE_BEGIN();
    /* parent may be NULL */
    assert ( NULL != diag_ptr );
    assert ( NULL != diagram_file_base_name );
    u8_error_t export_err = U8_ERROR_NONE;

    const char *const diag_name = data_diagram_get_name_const( diag_ptr );
    const char *const diag_description = data_diagram_get_description_const( diag_ptr );
    const data_id_t diag_id = data_diagram_get_data_id(diag_ptr);
    const char *const diag_type_name
        = json_type_name_map_get_diagram_type( &((*this_).type_map),
                                               data_diagram_get_diagram_type( diag_ptr )
                                              );
    const char *const diag_stereotype = data_diagram_get_stereotype_const( diag_ptr );

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

        case IO_FILE_FORMAT_HTML:
        {
            const unsigned int index_of_depth = ((*this_).current_tree_depth > HTML_DIAGRAM_MAX_DEPTH)
                ? (HTML_DIAGRAM_MAX_DEPTH-1)
                : ((*this_).current_tree_depth-1);
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_TITLE_START[index_of_depth] );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_TITLE_MIDDLE );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_TITLE_END[index_of_depth] );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_LIST_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_START );
            if ( 0 != utf8string_get_length( diag_stereotype ) )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_STEREO_START );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_stereotype );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_STEREO_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_TYPE_START );
            export_err |= xml_writer_write_xml_enc( &((*this_).xml_writer), diag_type_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_TYPE_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_ID_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), diag_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_ID_END );
            if ( 0 != utf8string_get_length( diag_description ) )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_DESCR_START );
                export_err |= md_filter_transform ( &((*this_).md_filter), diag_description );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_DESCR_END );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_LIST_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_IMG_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), diagram_file_base_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_IMG_END );
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
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    /* update export statistics */
    data_stat_inc_count ( (*this_).export_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_end_diagram( document_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_END : DOCBOOK_DIAGRAM_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            /* no end diagram tags */
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no end diagram tags */
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    /* decrease tree depth */
    (*this_).current_tree_depth --;

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t document_element_writer_start_diagramelement( document_element_writer_t *this_,
                                                         const data_diagram_t *parent,
                                                         const data_diagramelement_t *diagramelement_ptr )
{
    U8_TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    u8_error_t write_error = U8_ERROR_NONE;

    U8_TRACE_END_ERR(write_error);
    return write_error;
}

u8_error_t document_element_writer_assemble_diagramelement( document_element_writer_t *this_,
                                                            const data_diagram_t *parent,
                                                            const data_diagramelement_t *diagramelement_ptr,
                                                            const data_classifier_t *occurrence,
                                                            const data_feature_t *feat_occur )
{
    U8_TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    assert( occurrence != NULL );
    /* NULL is allowed here: feat_occur */
    u8_error_t write_error = U8_ERROR_NONE;

    U8_TRACE_END_ERR(write_error);
    return write_error;
}

u8_error_t document_element_writer_end_diagramelement( document_element_writer_t *this_,
                                                       const data_diagram_t *parent,
                                                       const data_diagramelement_t *diagramelement_ptr )
{
    U8_TRACE_BEGIN();
    assert( diagramelement_ptr != NULL );
    assert( parent != NULL );
    u8_error_t write_error = U8_ERROR_NONE;

    U8_TRACE_END_ERR(write_error);
    return write_error;
}

u8_error_t document_element_writer_end_main( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            /* no main end */
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_ARTICLE_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no main end */
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    U8_TRACE_END_ERR(export_err);
    return export_err;
}

u8_error_t document_element_writer_write_footer( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_BODY_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), HTML_DOC_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            /* no footer */
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_LOGIC_STATE;
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2017-2023 Andreas Warnke

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
