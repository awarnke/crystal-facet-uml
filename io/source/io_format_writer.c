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
    (*this_).current_tree_depth = 0;

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

enum XHTML_SECT_MAX { XHTML_SECT_MAX_DEPTH = 6, };
static const char XHTML_SECT_START[]
= "        <div>\n";
static const char *XHTML_SECT_TITLE_START[XHTML_SECT_MAX_DEPTH]
= {"            <h1 class=\"title\">","            <h2 class=\"title\">","            <h3 class=\"title\">",
   "            <h4 class=\"title\">","            <h5 class=\"title\">","            <h6 class=\"title\">"};
static const char *XHTML_SECT_TITLE_END[XHTML_SECT_MAX_DEPTH]
= {"</h1>\n","</h2>\n","</h3>\n","</h4>\n","</h5>\n","</h6>\n"};
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


static const int TXT_ID_INDENT_COLUMN = 48;
static const char TXT_ID_INDENT_SPACES[] = "                                                ";
static const char TXT_NEWLINE[] = "\n";
static const char TXT_SINGLE_INDENT[] = "| ";
static const char TXT_DOUBLE_INDENT[] = "  | ";
static const char TXT_SPACE_INDENT[] = "  ";
static const char TXT_COLON_SPACE[] = ": ";
static const char TXT_ARROW_SPACE[] = "--> ";
static const char TXT_SPACE_ARROW_SPACE[] = " --> ";


static const char TWO_NEWLINES[] = "\n\n";
static const size_t TWO_NEWLINES_LEN = 2;
static const char LINE_END[] = "\n";
static const size_t LINE_END_LEN = 1;
static const char SPACE[] = "  ";
static const size_t SPACE_LEN = 2;
static const size_t COLON_SPACE_LEN = 2;
static const char SPACE_ARROW_SPACE[] = " --> ";
static const size_t SPACE_ARROW_SPACE_LEN = 5;
static const char ARROW_SPACE[] = "--> ";
static const size_t ARROW_SPACE_LEN = 4;



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

int io_format_writer_start_diagram( io_format_writer_t *this_ )
{
    TRACE_BEGIN();
    assert ( NULL != (*this_).output );
    int export_err = 0;

    /* increase tree depth */
    (*this_).current_tree_depth ++;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_START );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_START );
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
    int64_t diag_id = data_diagram_get_id(diag_ptr);

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
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            unsigned int index_of_depth = (*this_).current_tree_depth - 1;
            index_of_depth = ( index_of_depth >= XHTML_SECT_MAX_DEPTH ) ? ( XHTML_SECT_MAX_DEPTH-1 ) : index_of_depth;
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_TITLE_START[index_of_depth] );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diag_name );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_TITLE_END[index_of_depth] );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_PARA_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diag_description );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_IMG_START );
            export_err |= io_format_writer_private_write_xml_enc ( this_, diagram_file_base_name );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_IMG_END );
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_PARA_END );
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_format_writer_private_write_plain ( this_, diag_name );
            export_err |= io_format_writer_private_write_indent_id( this_,
                                                                    TXT_ID_INDENT_COLUMN - strlen(diag_name),
                                                                    DATA_TABLE_DIAGRAM,
                                                                    diag_id
                                                                  );
            export_err |= io_format_writer_private_write_plain ( this_, TXT_NEWLINE );
            export_err |= io_format_writer_private_write_indent_multiline_string( this_,
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
    assert ( NULL != (*this_).output );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_format_writer_private_write_plain ( this_, TXT_NEWLINE );
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

int io_format_writer_write_classifier( io_format_writer_t *this_, data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    assert ( NULL != (*this_).output );
    int export_err = 0;

    const char *classifier_name = data_classifier_get_name_ptr(classifier_ptr);
    const char *classifier_descr = data_classifier_get_description_ptr(classifier_ptr);
    int64_t classifier_id = data_classifier_get_id(classifier_ptr);

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_format_writer_private_write_plain ( this_, TXT_NEWLINE );
            export_err |= io_format_writer_private_write_plain ( this_, classifier_name );
            export_err |= io_format_writer_private_write_indent_id( this_,
                                                                    TXT_ID_INDENT_COLUMN - strlen(classifier_name),
                                                                    DATA_TABLE_CLASSIFIER,
                                                                    classifier_id
                                                                  );
            export_err |= io_format_writer_private_write_plain ( this_, TXT_NEWLINE );
            export_err |= io_format_writer_private_write_indent_multiline_string( this_,
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

int io_format_writer_write_feature( io_format_writer_t *this_, data_feature_t *feature_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    assert ( NULL != (*this_).output );
    int export_err = 0;

    const char *feature_key = data_feature_get_key_ptr( feature_ptr );
    const char *feature_value = data_feature_get_value_ptr( feature_ptr );
    const char *feature_descr = data_feature_get_description_ptr( feature_ptr );
    int64_t feature_id = data_feature_get_id( feature_ptr );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_format_writer_private_write_plain ( this_, TXT_SPACE_INDENT );
            export_err |= io_format_writer_private_write_plain ( this_, feature_key );

            size_t feature_key_len = strlen(feature_key);
            size_t feature_value_len = strlen(feature_value);
            if ( 0 != feature_value_len )
            {
                export_err |= io_format_writer_private_write_plain ( this_, TXT_COLON_SPACE );
                export_err |= io_format_writer_private_write_plain ( this_, feature_value );
            }

            int id_indent_width = TXT_ID_INDENT_COLUMN - strlen(TXT_SPACE_INDENT) - feature_key_len
                - ((feature_value_len==0)?0:feature_value_len+strlen(TXT_COLON_SPACE));
            export_err |= io_format_writer_private_write_indent_id( this_,
                                                                    id_indent_width,
                                                                    DATA_TABLE_FEATURE,
                                                                    feature_id
                                                                  );
            export_err |= io_format_writer_private_write_plain ( this_, TXT_NEWLINE );
            export_err |= io_format_writer_private_write_indent_multiline_string( this_,
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
                                         data_relationship_t *relation_ptr,
                                         data_classifier_t *dest_classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    assert ( NULL != dest_classifier_ptr );
    assert ( NULL != (*this_).output );
    int export_err = 0;

    const char *relation_name = data_relationship_get_name_ptr( relation_ptr );
    int64_t relation_id = data_relationship_get_id( relation_ptr );
    const char *relation_descr = data_relationship_get_description_ptr( relation_ptr );
    const char *dest_classifier_name = data_classifier_get_name_ptr( dest_classifier_ptr );

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
        }
        break;

        case IO_FILE_FORMAT_TXT:
        {
            export_err |= io_format_writer_private_write_plain ( this_, TXT_SPACE_INDENT );
            export_err |= io_format_writer_private_write_plain ( this_, relation_name );

            size_t relation_name_len = strlen(relation_name);
            /* print arrow */
            if ( relation_name_len == 0 )
            {
                export_err |= io_format_writer_private_write_plain ( this_, TXT_ARROW_SPACE );
            }
            else
            {
                export_err |= io_format_writer_private_write_plain ( this_, TXT_SPACE_ARROW_SPACE );
            }

            export_err |= io_format_writer_private_write_plain ( this_, dest_classifier_name );

            /* print id */
            size_t dest_classifier_name_len = strlen( dest_classifier_name );
            int id_indent_width = TXT_ID_INDENT_COLUMN - strlen(TXT_SPACE_INDENT) - relation_name_len
                - ((relation_name_len==0)?strlen(TXT_ARROW_SPACE):strlen(TXT_SPACE_ARROW_SPACE))
                - dest_classifier_name_len;
            export_err |= io_format_writer_private_write_indent_id( this_,
                                                                    id_indent_width,
                                                                    DATA_TABLE_RELATIONSHIP,
                                                                    relation_id
                                                                  );

            export_err |= io_format_writer_private_write_plain ( this_, TXT_NEWLINE );
            export_err |= io_format_writer_private_write_indent_multiline_string( this_,
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
    assert ( NULL != (*this_).output );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
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
    assert ( NULL != (*this_).output );
    int export_err = 0;

    switch ( (*this_).export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            export_err |= io_format_writer_private_write_plain ( this_, DOCBOOK_SECT_END );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_SECT_END );
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
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            export_err |= io_format_writer_private_write_plain ( this_, XHTML_DOC_END );
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


int io_format_writer_private_write_indent_multiline_string ( io_format_writer_t *this_,
                                                             const char *indent,
                                                             const char *multiline_string )
{
    TRACE_BEGIN();
    assert( NULL != indent );
    assert( NULL != (*this_).output );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */
    size_t indent_length = strlen( indent );

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
                out_count = fwrite( LINE_END, 1 /* size of char */, LINE_END_LEN, (*this_).output );
                if ( out_count != LINE_END_LEN )
                {
                    TSLOG_ERROR_INT( "not all bytes could be written. missing:", LINE_END_LEN - out_count );
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

int io_format_writer_private_write_indent_id ( io_format_writer_t *this_,
                                               int indent_width,
                                               data_table_t table,
                                               int64_t row_id )
{
    TRACE_BEGIN();
    assert( NULL != (*this_).output );
    assert( DATA_TABLE_VOID != table );
    assert( DATA_ID_VOID_ID != row_id );
    assert( sizeof(TXT_ID_INDENT_SPACES) == 1+TXT_ID_INDENT_COLUMN );
    assert( indent_width <= TXT_ID_INDENT_COLUMN );
    int result = 0;
    size_t out_count;  /* checks if the number of written characters matches the expectation */

    /* indent */
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


/*
Copyright 2017-2019 Andreas Warnke

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
