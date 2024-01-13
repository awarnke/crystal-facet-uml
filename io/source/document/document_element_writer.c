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

#define DOCUMENT_ELEMENT_WRITER_LEFT_POINTING_GUILLEMENTS "\xc2\xab"
#define DOCUMENT_ELEMENT_WRITER_RIGHT_POINTING_GUILLEMENTS "\xc2\xbb"

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
    = "</term>"
      "\n<listitem>";
static const char DOCBOOK_ELEMENT_ATTRIBUTES_START[]
    = "\n<para>";
static const char DOCBOOK_ELEMENT_STEREO_START[]
    = "\n" DOCUMENT_ELEMENT_WRITER_LEFT_POINTING_GUILLEMENTS;
static const char DOCBOOK_ELEMENT_STEREO_END[]
    = DOCUMENT_ELEMENT_WRITER_RIGHT_POINTING_GUILLEMENTS;
static const char DOCBOOK_ELEMENT_ID_START[]
    = "\n<token>";
static const char DOCBOOK_ELEMENT_ID_END[]
    = "</token>";
static const char DOCBOOK_ELEMENT_SEE_START[] = "\n<emphasis>(appears in ";
static const char DOCBOOK_ELEMENT_SEE_NEXT[] = ",\n";
static const char DOCBOOK_ELEMENT_SEE_MORE[] = ", ...";
static const char DOCBOOK_ELEMENT_SEE_END[] = ")\n</emphasis>";
static const char DOCBOOK_ELEMENT_ATTRIBUTES_END[]
    = "\n</para>";
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
    = "\n<html>";
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
static const char HTML_HEADER[]
    = "\n<header></header>";
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
static const char HTML_FOOTER[]
    = "\n<footer></footer>";
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

static const char HTML_DIAG_START[] = "\n<div class=\"diag\">";
static const char HTML_DIAG_HEAD_START[] = "\n<p>";
static const char HTML_DIAG_NAME_START[] = "\n<strong class=\"diag-name\">";
static const char HTML_DIAG_NAME_END[] = "</strong>";
static const char HTML_DIAG_STEREO_START[] = "\n<em class=\"diag-stereo\">";
static const char HTML_DIAG_STEREO_END[] = "</em>";
static const char HTML_DIAG_TYPE_START[] = "\n<em class=\"diag-type\">";
static const char HTML_DIAG_TYPE_END[] = "</em>";
static const char HTML_DIAG_ID_START[] = "\n<em class=\"diag-id\">";
static const char HTML_DIAG_ID_END[] = "</em>";
static const char HTML_DIAG_HEAD_END[] = "\n</p>";
static const char HTML_DIAG_DESCR_START[] = "\n<p class=\"diag-descr\">\n";
static const char HTML_DIAG_DESCR_END[] = "\n</p>";
static const char HTML_DIAG_END[] = "\n</div>";

static const char HTML_CLAS_START[] = "\n<div class=\"clas\">";
static const char HTML_CLAS_HEAD_START[] = "\n<p>";
static const char HTML_CLAS_NAME_START[] = "\n<strong class=\"clas-name\">";
static const char HTML_CLAS_NAME_END[] = "</strong>";
static const char HTML_CLAS_STEREO_START[] = "\n<em class=\"clas-stereo\">";
static const char HTML_CLAS_STEREO_END[] = "</em>";
static const char HTML_CLAS_TYPE_START[] = "\n<em class=\"clas-type\">";
static const char HTML_CLAS_TYPE_END[] = "</em>";
static const char HTML_CLAS_ID_START[] = "\n<em class=\"clas-id\">";
static const char HTML_CLAS_ID_END[] = "</em>";
static const char HTML_CLAS_HEAD_END[] = "\n</p>";
static const char HTML_CLAS_SEE_START[] = "\n<p class=\"clas-see\">\n";
static const char HTML_CLAS_SEE_NEXT[] = ",\n";
static const char HTML_CLAS_SEE_MORE[] = ", ...";
static const char HTML_CLAS_SEE_END[] = "\n</p>";
static const char HTML_CLAS_DESCR_START[] = "\n<p class=\"clas-descr\">\n";
static const char HTML_CLAS_DESCR_END[] = "\n</p>";
static const char HTML_CLAS_END[] = "\n</div>";

static const char HTML_FEAT_START[] = "\n<div class=\"feat\">";
static const char HTML_FEAT_HEAD_START[] = "\n<p>";
static const char HTML_FEAT_NAME_START[] = "\n<strong class=\"feat-name\">";
static const char HTML_FEAT_NAME_END[] = "</strong>";
static const char HTML_FEAT_STEREO_START[] = "\n<em class=\"feat-stereo\">";
static const char HTML_FEAT_STEREO_END[] = "</em>";
static const char HTML_FEAT_VALUE_START[] = "\n<em class=\"feat-value\">";
static const char HTML_FEAT_VALUE_END[] = "</em>";
static const char HTML_FEAT_TYPE_START[] = "\n<em class=\"feat-type\">";
static const char HTML_FEAT_TYPE_END[] = "</em>";
static const char HTML_FEAT_ID_START[] = "\n<em class=\"feat-id\">";
static const char HTML_FEAT_ID_END[] = "</em>";
static const char HTML_FEAT_HEAD_END[] = "\n</p>";
static const char HTML_FEAT_DESCR_START[] = "\n<p class=\"feat-descr\">\n";
static const char HTML_FEAT_DESCR_END[] = "\n</p>";
static const char HTML_FEAT_END[] = "\n</div>";

static const char HTML_REL_START[] = "\n<div class=\"rel\">";
static const char HTML_REL_HEAD_START[] = "\n<p>";
static const char HTML_REL_NAME_START[] = "\n<strong class=\"rel-name\">";
static const char HTML_REL_NAME_END[] = "</strong>";
static const char HTML_REL_DEST_START[] = "\n<strong class=\"rel-dest\">";
static const char HTML_REL_DEST_END[] = "</strong>";
static const char HTML_REL_STEREO_START[] = "\n<em class=\"rel-stereo\">";
static const char HTML_REL_STEREO_END[] = "</em>";
static const char HTML_REL_TYPE_START[] = "\n<em class=\"rel-type\">";
static const char HTML_REL_TYPE_END[] = "</em>";
static const char HTML_REL_ID_START[] = "\n<em class=\"rel-id\">";
static const char HTML_REL_ID_END[] = "</em>";
static const char HTML_REL_HEAD_END[] = "\n</p>";
static const char HTML_REL_DESCR_START[] = "\n<p class=\"rel-descr\">\n";
static const char HTML_REL_DESCR_END[] = "\n</p>";
static const char HTML_REL_END[] = "\n</div>";

/* IO_FILE_FORMAT_TXT */

static const int TXT_ID_INDENT_COLUMN = 48;
static const char TXT_NEWLINE[] = "\n";
static const char TXT_SINGLE_INDENT[] = "| ";
static const char TXT_DOUBLE_INDENT[] = "  | ";
static const char TXT_SPACE_INDENT[] = "  ";
static const char TXT_COLON_SPACE[] = ": ";
static const char TXT_SPACE[] = " ";

#define io_element_writer_impl_t document_element_writer_t
/*!
 *  \brief A struct of function pointers which is the interface of an io_element_writer
 *
 *  To avoid typecasts, this struct provides function pointers with the exact right signatures
 *  provided via io_element_writer_impl_t
 */
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
    data_rules_init( &((*this_).data_rules) );
    document_link_provider_init( &((*this_).link_provider), db_reader );
    io_txt_icon_init( &((*this_).txt_icon) );

    io_txt_writer_init( &((*this_).txt_writer), output );
    io_xml_writer_init( &((*this_).xml_writer), output );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            io_md_writer_init( &((*this_).md_writer),
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
            io_md_writer_init( &((*this_).md_writer),
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
            io_md_writer_init( &((*this_).md_writer), db_reader, "\n", "", ": ", "", &((*this_).xml_writer) );
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            assert(false);  /* use another io_element_writer instead */
            io_md_writer_init( &((*this_).md_writer), db_reader, "\n", "", ": ", "", &((*this_).xml_writer) );
        }
        break;
    }

    U8_TRACE_END();
}

void document_element_writer_destroy( document_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    io_md_writer_destroy( &((*this_).md_writer) );
    io_xml_writer_destroy( &((*this_).xml_writer) );
    io_txt_writer_destroy( &((*this_).txt_writer) );

    io_txt_icon_destroy( &((*this_).txt_icon) );
    document_link_provider_destroy( &((*this_).link_provider) );
    data_rules_destroy( &((*this_).data_rules) );
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

    io_xml_writer_reset_indent ( &((*this_).xml_writer) );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ENC );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_TITLE_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_TITLE_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DTD );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DOC_START );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_TITLE_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_TITLE_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_CSS_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_CSS_END );
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEAD_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_BODY_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_HEADER );
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
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_ARTICLE_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
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
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_NAV_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        default:
        {
            /* nothing to do, only html provides a table of contents */
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
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        default:
        {
            /* nothing to do, only html provides a table of contents */
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
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_START[index_of_depth] );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        default:
        {
            /* nothing to do, only html provides a table of contents */
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

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_TITLE_START );
            export_err |= io_xml_writer_write_plain_id ( &((*this_).xml_writer), diag_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_TITLE_MIDDLE );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_TITLE_END );
        }
        break;


        default:
        {
            /* nothing to do, only html provides a table of contents */
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
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_ENTRY_END );
        }
        break;

        default:
        {
            /* nothing to do, only html provides a table of contents */
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
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_TOC_SUBLIST_END );
        }
        break;

        default:
        {
            /* nothing to do, only html provides a table of contents */
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
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_NAV_END );
        }
        break;

        default:
        {
            /* nothing to do, only html provides a table of contents */
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
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
    data_diagram_t (*appears_in_diagrams)[];
    uint32_t appears_in_diagrams_count;
    const u8_error_t appears_in_diagrams_err
        = document_link_provider_get_occurrences( &((*this_).link_provider),
                                                  classifier_id,
                                                  &appears_in_diagrams,
                                                  &appears_in_diagrams_count
                                                );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ATTRIBUTES_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            if ( 0 != utf8string_get_length( classifier_stereotype ) )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_STEREO_START );
                export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_stereotype );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_STEREO_END );
            }
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );

            if (( U8_ERROR_NONE == appears_in_diagrams_err )
               ||( U8_ERROR_ARRAY_BUFFER_EXCEEDED == appears_in_diagrams_err ))
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_SEE_START );
                bool is_first = true;
                for ( uint_fast32_t idx = 0; idx < appears_in_diagrams_count; idx ++ )
                {
                    const data_diagram_t *const current = &((*appears_in_diagrams)[idx]);
                    const data_id_t diag_ref_id = data_diagram_get_data_id( current );
                    if ( is_first )
                    {
                        is_first = false;
                    }
                    else
                    {
                        export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_SEE_NEXT );
                    }
                    const char *const diag_ref_name = data_diagram_get_name_const( current );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_XREF_START );
                    export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), diag_ref_id );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_XREF_MIDDLE );
                    export_err |= io_xml_writer_write_xml_enc( &((*this_).xml_writer), diag_ref_name );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_XREF_END );
                }
                if ( U8_ERROR_ARRAY_BUFFER_EXCEEDED == appears_in_diagrams_err )
                {
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_SEE_MORE );
                }
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_SEE_END );
            }

            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ATTRIBUTES_END );

            if ( 0 != classifier_descr_len )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= io_md_writer_transform ( &((*this_).md_writer), classifier_descr );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
            }

            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            if (( U8_ERROR_NONE == appears_in_diagrams_err )
               ||( U8_ERROR_ARRAY_BUFFER_EXCEEDED == appears_in_diagrams_err ))
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_SEE_START );
                bool is_first = true;
                for ( uint_fast32_t idx = 0; idx < appears_in_diagrams_count; idx ++ )
                {
                    const data_diagram_t *const current = &((*appears_in_diagrams)[idx]);
                    const data_id_t diag_ref_id = data_diagram_get_data_id( current );
                    if ( is_first )
                    {
                        is_first = false;
                    }
                    else
                    {
                        export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_SEE_NEXT );
                    }
                    const char *const diag_ref_name = data_diagram_get_name_const( current );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_ANY_DESCR_XREF_START );
                    export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), diag_ref_id );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_ANY_DESCR_XREF_MIDDLE );
                    export_err |= io_xml_writer_write_xml_enc( &((*this_).xml_writer), diag_ref_name );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_ANY_DESCR_XREF_END );
                }
                if ( U8_ERROR_ARRAY_BUFFER_EXCEEDED == appears_in_diagrams_err )
                {
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_SEE_MORE );
                }
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_SEE_END );
            }

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_HEAD_START );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_NAME_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_NAME_END );
            if ( 0 != utf8string_get_length( classifier_stereotype ) )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_STEREO_START );
                export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_stereotype );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_STEREO_END );
            }
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_TYPE_START );
            export_err |= io_xml_writer_write_xml_enc( &((*this_).xml_writer), classifier_type_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_TYPE_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_ID_START );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_ID_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_HEAD_END );

            if ( 0 != classifier_descr_len )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_DESCR_START );
                export_err |= io_md_writer_transform ( &((*this_).md_writer), classifier_descr );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_DESCR_END );
            }
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), classifier_name );
            const utf8stringview_t classifier_name_view = UTF8STRINGVIEW_STR( classifier_name );
            const size_t clas_codepoint = utf8stringview_count_codepoints( &classifier_name_view );
            export_err |= io_txt_writer_write_indent_id( &((*this_).txt_writer),
                                                         TXT_ID_INDENT_COLUMN - clas_codepoint,
                                                         classifier_id
                                                       );
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= io_txt_writer_write_indent_multiline_string( &((*this_).txt_writer),
                                                                       TXT_SINGLE_INDENT,
                                                                       classifier_descr
                                                                     );
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_CLAS_END );
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
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        default:
        {
        }
        break;
    }

    U8_TRACE_END_ERR(export_err);
    return export_err;
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
    const data_feature_type_t feature_type = data_feature_get_main_type( feature_ptr );
    const char *const feature_type_name
        = json_type_name_map_get_feature_type( &((*this_).type_map),
                                               parent_type,
                                               feature_type
                                             );
    const bool has_stereotype = data_rules_feature_value_is_stereotype( &((*this_).data_rules), feature_type );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ATTRIBUTES_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            if ( 0 != feature_value_len )
            {
                if ( has_stereotype )
                {
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_STEREO_START );
                    export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_STEREO_END );
                }
                else
                {
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
                    export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
                }
            }
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), feature_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ATTRIBUTES_END );

            if ( 0 != feature_descr_len )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= io_md_writer_transform ( &((*this_).md_writer), feature_descr );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
            }

            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_HEAD_START );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_NAME_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_NAME_END );
            if ( 0 != feature_value_len )
            {
                if ( has_stereotype )
                {
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_STEREO_START );
                    export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_STEREO_END );
                }
                else
                {
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), TXT_COLON_SPACE );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_VALUE_START );
                    export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
                    export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_VALUE_END );
                }
            }
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_TYPE_START );
            export_err |= io_xml_writer_write_xml_enc( &((*this_).xml_writer), feature_type_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_TYPE_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_ID_START );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), feature_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_ID_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_HEAD_END );
            if ( 0 != feature_descr_len )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_DESCR_START );
                export_err |= io_md_writer_transform ( &((*this_).md_writer), feature_descr );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_DESCR_END );
            }
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_SPACE_INDENT );
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), feature_key );
            if ( 0 != feature_value_len )
            {
                export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_COLON_SPACE );
                export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), feature_value );
            }

            const utf8stringview_t feature_key_view = UTF8STRINGVIEW_STR( feature_key );
            const utf8stringview_t feature_value_view = UTF8STRINGVIEW_STR( feature_value );
            const size_t feat_key_codepoint = utf8stringview_count_codepoints( &feature_key_view );
            const size_t feat_value_codepoint = utf8stringview_count_codepoints( &feature_value_view );
            int id_indent_width = TXT_ID_INDENT_COLUMN - utf8string_get_length(TXT_SPACE_INDENT) - feat_key_codepoint
                - ((feature_value_len==0)?0:(feat_value_codepoint+utf8string_get_length(TXT_COLON_SPACE)));
            export_err |= io_txt_writer_write_indent_id( &((*this_).txt_writer),
                                                         id_indent_width,
                                                         feature_id
                                                       );
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= io_txt_writer_write_indent_multiline_string( &((*this_).txt_writer),
                                                                       TXT_DOUBLE_INDENT,
                                                                       feature_descr
                                                                     );
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FEAT_END );
        }
        break;

        default:
        {
        }
        break;
    }

    U8_TRACE_END_ERR(export_err);
    return export_err;
}

u8_error_t document_element_writer_start_relationship( document_element_writer_t *this_,
                                                       data_classifier_type_t host_type,
                                                       const data_relationship_t *relation_ptr)
{
    U8_TRACE_BEGIN();
    assert( relation_ptr != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        default:
        {
        }
        break;
    }

    U8_TRACE_END_ERR(export_err);
    return export_err;
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
    const data_relationship_type_t relation_type = data_relationship_get_main_type( relation_ptr );
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
                                                    relation_type
                                                  );
    const char*const relation_txticon = io_txt_icon_get_relationship ( &((*this_).txt_icon), relation_type );

    const char *const relation_stereotype = data_relationship_get_stereotype_const( relation_ptr );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            /* list start */
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            /* element */
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ATTRIBUTES_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            if ( 0 != utf8string_get_length( relation_stereotype ) )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_STEREO_START );
                export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_stereotype );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_STEREO_END );
            }
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), TXT_SPACE );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_txticon );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), TXT_SPACE );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), dest_classifier_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ATTRIBUTES_END );

            if ( 0 != relation_descr_len )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_START );
                export_err |= io_md_writer_transform ( &((*this_).md_writer), relation_descr );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_DESCR_END );
            }

            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
            /* list end */
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_HEAD_START );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_NAME_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_NAME_END );
            if ( 0 != utf8string_get_length( relation_stereotype ) )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_STEREO_START );
                export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_stereotype );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_STEREO_END );
            }
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), TXT_SPACE );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_txticon );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), TXT_SPACE );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_DEST_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), dest_classifier_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_DEST_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_TYPE_START );
            export_err |= io_xml_writer_write_xml_enc( &((*this_).xml_writer), relation_type_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_TYPE_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_ID_START );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_ID_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_HEAD_END );
            if ( 0 != relation_descr_len )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_DESCR_START );
                export_err |= io_md_writer_transform ( &((*this_).md_writer), relation_descr );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_DESCR_END );
            }
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_SPACE_INDENT );

            size_t relation_name_len = utf8string_get_length(relation_name);
            /* print arrow */
            if ( relation_name_len != 0 )
            {
                export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), relation_name );
                export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_SPACE );
            }
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), relation_txticon );
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_SPACE );

            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), dest_classifier_name );

            /* print id */
            const utf8stringview_t relation_name_view = UTF8STRINGVIEW_STR( relation_name );
            const utf8stringview_t relation_txticon_view = UTF8STRINGVIEW_STR( relation_txticon );
            const utf8stringview_t dest_classifier_name_view = UTF8STRINGVIEW_STR( dest_classifier_name );
            const size_t rel_name_codepoint = utf8stringview_count_codepoints( &relation_name_view);
            const size_t rel_icon_codepoint = utf8stringview_count_codepoints( &relation_txticon_view );
            const size_t rel_dest_codepoint = utf8stringview_count_codepoints( &dest_classifier_name_view );
            int id_indent_width
                = TXT_ID_INDENT_COLUMN
                - utf8string_get_length(TXT_SPACE_INDENT)
                - ((relation_name_len==0)?0:(rel_name_codepoint+utf8string_get_length(TXT_SPACE)))
                - rel_icon_codepoint
                - utf8string_get_length(TXT_SPACE)
                - rel_dest_codepoint;
            export_err |= io_txt_writer_write_indent_id( &((*this_).txt_writer),
                                                         id_indent_width,
                                                         relation_id
                                                       );

            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= io_txt_writer_write_indent_multiline_string( &((*this_).txt_writer),
                                                                       TXT_DOUBLE_INDENT,
                                                                       relation_descr
                                                                     );
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
    u8_error_t export_err = U8_ERROR_NONE;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_HTML:
        {
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_REL_END );
        }
        break;

        default:
        {
        }
        break;
    }

    U8_TRACE_END_ERR(export_err);
    return export_err;
}

u8_error_t document_element_writer_start_diagram( document_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != diag_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    const data_id_t diag_id = data_diagram_get_data_id(diag_ptr);
    const char *const diag_name = data_diagram_get_name_const( diag_ptr );

    /* increase tree depth */
    (*this_).current_tree_depth ++;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_START : DOCBOOK_DIAGRAM_START );
            export_err |= io_xml_writer_write_plain_id ( &((*this_).xml_writer), diag_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_MIDDLE : DOCBOOK_DIAGRAM_MIDDLE );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            const unsigned int index_of_depth = ((*this_).current_tree_depth > HTML_DIAGRAM_MAX_DEPTH)
                ? (HTML_DIAGRAM_MAX_DEPTH-1)
                : ((*this_).current_tree_depth-1);
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_TITLE_START[index_of_depth] );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), diag_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_TITLE_MIDDLE );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_TITLE_END[index_of_depth] );
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
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_TITLE_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_TITLE_END );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_NAME_END );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ATTRIBUTES_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );
            if ( 0 != utf8string_get_length( diag_stereotype ) )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_STEREO_START );
                export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_stereotype );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_STEREO_END );
            }
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_START );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), diag_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ID_END );
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_ATTRIBUTES_END );

            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_END );
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_ELEMENT_LIST_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_START );
            export_err |= io_md_writer_transform ( &((*this_).md_writer), diag_description );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DESCRIPTION_END );

            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_IMG_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diagram_file_base_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_IMG_MIDDLE );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diagram_file_base_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DIAGRAM_IMG_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            /* diagram start */
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_START );
            io_xml_writer_increase_indent ( &((*this_).xml_writer) );

            /* diagram contents */
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_HEAD_START );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_NAME_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_NAME_END );
            if ( 0 != utf8string_get_length( diag_stereotype ) )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_STEREO_START );
                export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diag_stereotype );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_STEREO_END );
            }
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_TYPE_START );
            export_err |= io_xml_writer_write_xml_enc( &((*this_).xml_writer), diag_type_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_TYPE_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_ID_START );
            export_err |= io_xml_writer_write_plain_id( &((*this_).xml_writer), diag_id );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_ID_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_HEAD_END );
            if ( 0 != utf8string_get_length( diag_description ) )
            {
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_DESCR_START );
                export_err |= io_md_writer_transform ( &((*this_).md_writer), diag_description );
                export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_DESCR_END );
            }

            /* diagram end happens here to avoid recursion to child diagrams before the end */
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAG_END );

            /* show the diagram-picture */
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_IMG_START );
            export_err |= io_xml_writer_write_xml_enc ( &((*this_).xml_writer), diagram_file_base_name );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DIAGRAM_IMG_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), diag_name );
            const utf8stringview_t diag_name_view = UTF8STRINGVIEW_STR( diag_name );
            const size_t diag_name_codepoint = utf8stringview_count_codepoints( &diag_name_view );
            export_err |= io_txt_writer_write_indent_id( &((*this_).txt_writer),
                                                         TXT_ID_INDENT_COLUMN - diag_name_codepoint,
                                                         diag_id
                                                       );
            export_err |= io_txt_writer_write_plain ( &((*this_).txt_writer), TXT_NEWLINE );
            export_err |= io_txt_writer_write_indent_multiline_string( &((*this_).txt_writer),
                                                                       TXT_SINGLE_INDENT,
                                                                       diag_description
                                                                     );
        }
        break;

        default:
        {
            U8_LOG_ERROR("error: unknown_format.");
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), ((*this_).current_tree_depth==1) ? DOCBOOK_TOP_DIAGRAM_END : DOCBOOK_DIAGRAM_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
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
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_ARTICLE_END );
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
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
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
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), DOCBOOK_DOC_END );
        }
        break;

        case IO_FILE_FORMAT_HTML:
        {
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_FOOTER );
            io_xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_BODY_END );
            export_err |= io_xml_writer_write_plain ( &((*this_).xml_writer), HTML_DOC_END );
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
            export_err = U8_ERROR_CONFIG_OUT_OF_RANGE;
        }
        break;
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
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
