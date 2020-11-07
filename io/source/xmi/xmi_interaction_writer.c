/* File: xmi_interaction_writer.c; Copyright and License: see below */

#include "xmi/xmi_interaction_writer.h"
#include "xmi/xmi_element_info_map.h"
#include "xmi/xmi_element_info.h"
#include "xmi/xmi_xml.h"
#include "util/string/utf8string.h"
#include "data_id.h"
#include "data_classifier_type.h"
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

/* Note: when writing, each method typically starts with writing a newline, it does not end with writing a newline. */

/* IO_FILE_FORMAT_XMI2 */

    /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.8.6.4 */
static const char XMI2_UML_OWNED_COMMENT_START[]
    = "\n<ownedComment ";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.8.6.4 */
static const char XMI2_UML_OWNED_COMMENT_MIDDLE[]
    = ">";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.8.6.4 */
static const char XMI2_UML_OWNED_COMMENT_END[]
    = "\n</ownedComment>";

    /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.8.2 */
static const char XMI2_UML_COMMENT_BODY_START[]
    = "\n<body>";
static const char XMI2_UML_COMMENT_BODY_END[]
    = "\n</body>";
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.9.2 */
static const char XMI2_UML_ANNOTATED_ELEMENT_START[]
    = "\n<annotatedElement ";
static const char XMI2_UML_ANNOTATED_ELEMENT_END[]
    = "/>";

static const char XMI2_FALLBACK_NESTING_ELEMENT[]
    = "packagedElement";

static const char XMI2_COMMENT_TYPE[]
    = "Comment";

void xmi_interaction_writer_init ( xmi_interaction_writer_t *this_,
                                   data_database_reader_t *db_reader,
                                   data_stat_t *io_export_stat,
                                   xml_writer_t *out_writer  )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != io_export_stat );
    assert( NULL != out_writer );

    (*this_).mode = IO_WRITER_PASS_BASE;
    (*this_).export_stat = io_export_stat;
    (*this_).xml_writer = out_writer;
    xmi_type_converter_init( &((*this_).xmi_types) );

    TRACE_END();
}

void xmi_interaction_writer_destroy( xmi_interaction_writer_t *this_ )
{
    TRACE_BEGIN();

    xmi_type_converter_destroy( &((*this_).xmi_types) );
    xml_writer_destroy( (*this_).xml_writer );

    TRACE_END();
}

int xmi_interaction_writer_start_diagram( xmi_interaction_writer_t *this_,
                                          data_classifier_type_t parent_type, 
                                          const data_diagram_t *diagram_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != diagram_ptr );
    int export_err = 0;

    const char *const diagram_name = data_diagram_get_name_ptr(diagram_ptr);
    const char *const diagram_descr = data_diagram_get_description_ptr(diagram_ptr);
    const size_t diagram_descr_len = utf8string_get_length(diagram_descr);
    const data_id_t diagram_id = data_diagram_get_data_id(diagram_ptr);

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* nesting_property;
        const int nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_classifier( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake child type */
                                                                         &nesting_property
                                                                       );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write an interaction to an illegal place. */
            /* This should not have happened, because latest the model is a valid nesting container */
            TRACE_INFO("xmi_element_writer: request to write an interaction to an illegal place!")
            assert(false);
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- COMMENT ON UML-CONFORMANCE: Unsuitable nested type of " );
            export_err |= xml_writer_write_plain_id( (*this_).xml_writer, diagram_id );
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );
            /* use a fallback */
            nesting_property = XMI2_FALLBACK_NESTING_ELEMENT;
        }

        /* write nesting tag */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_START_TAG_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, nesting_property );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_SEPARATOR );
        xml_writer_increase_indent ( (*this_).xml_writer );

        /* write type attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_NS_UML );
        const char* c_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                             parent_type,
                                                                             DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake child type */
                                                                             XMI_SPEC_UML
                                                                           );
        export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, c_type );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_END );

        /* write id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_START );
        export_err |= xmi_interaction_writer_private_encode_xmi_id( this_, diagram_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_END );

        /* write name attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_NAME_START );
        export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, diagram_name );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_NAME_END );

        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_START_TAG_END );

        /* write real id comment */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_COMMENT_START );
        export_err |= xml_writer_write_plain_id( (*this_).xml_writer, diagram_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_COMMENT_END );

        /* write description */
        if ( 0 != diagram_descr_len )
        {
            export_err |= xmi_interaction_writer_private_write_xmi_comment( this_,
                                                                            diagram_id,
                                                                            "specification",
                                                                            diagram_descr
                                                                          );
        }

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_interaction_writer_end_diagram( xmi_interaction_writer_t *this_,
                                        data_classifier_type_t parent_type )
{
    TRACE_BEGIN();
    int export_err = 0;

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* nesting_property;
        const int nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_classifier( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake child type */
                                                                         &nesting_property
                                                                       );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write a classifier to an illegal place */
            /* use a fallback */
            nesting_property = XMI2_FALLBACK_NESTING_ELEMENT;
        }

        /* adjust indentation, write end tag */
        xml_writer_decrease_indent ( (*this_).xml_writer );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_END_TAG_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, nesting_property );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_END_TAG_END );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_interaction_writer_private_write_xmi_comment( xmi_interaction_writer_t *this_,
                                                      data_id_t element_id,
                                                      const char *comment_type,
                                                      const char *comment )

{
    TRACE_BEGIN();
    assert( NULL != comment_type );
    assert( NULL != comment );
    int export_err = 0;

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_OWNED_COMMENT_START );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_START );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_NS_UML );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_COMMENT_TYPE );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_END );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_START );
    export_err |= xmi_interaction_writer_private_encode_xmi_id( this_, element_id );
    export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, "#" );
    export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, comment_type );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_END );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_OWNED_COMMENT_MIDDLE );
    xml_writer_increase_indent ( (*this_).xml_writer );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_COMMENT_BODY_START );
    xml_writer_increase_indent ( (*this_).xml_writer );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_NL );
    export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, comment );

    xml_writer_decrease_indent ( (*this_).xml_writer );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_COMMENT_BODY_END );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_ANNOTATED_ELEMENT_START );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_IDREF_START );
    export_err |= xmi_interaction_writer_private_encode_xmi_id( this_, element_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_IDREF_END );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_ANNOTATED_ELEMENT_END );

    xml_writer_decrease_indent ( (*this_).xml_writer );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_OWNED_COMMENT_END );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_interaction_writer_private_encode_xmi_id( xmi_interaction_writer_t *this_,
                                                  data_id_t element_id )

{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( data_id_get_table(&element_id) )
    {
        case DATA_TABLE_VOID:
        {
            TSLOG_WARNING( "invalid data_id_t at xmi_interaction_writer_private_encode_xmi_id." ); /* "1" is reserved for profiles */
            export_err |= -1;
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, "6" /* instead of C */ );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, "4" /* instead of F */ );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, "8" /* instead of R */ );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, "3" /* instead of E */ );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, "9" /* instead of D */ );
        }
        break;

        default:
        {
            TSLOG_ERROR( "unknown data_id_t at xmi_interaction_writer_private_encode_xmi_id." );
        }
        break;
    }

    if ( data_id_get_table(&element_id) != DATA_TABLE_VOID )
    {
        if ( 100 > data_id_get_row_id(&element_id) )
        {
            if ( 10 > data_id_get_row_id(&element_id) )
            {
                if ( 0 <= data_id_get_row_id(&element_id) )
                {
                    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "000" );
                }
                else
                {
                    /* row_id is negative */
                }
            }
            else
            {
                export_err |= xml_writer_write_plain ( (*this_).xml_writer, "00" );
            }
        }
        else
        {
            if ( 1000 > data_id_get_row_id(&element_id) )
            {
                export_err |= xml_writer_write_plain ( (*this_).xml_writer, "0" );
            }
            else
            {
                /* row_id is greater than 1000 */
            }
        }
        export_err |= xml_writer_write_int ( (*this_).xml_writer, data_id_get_row_id(&element_id) );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2020-2020 Andreas Warnke

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
