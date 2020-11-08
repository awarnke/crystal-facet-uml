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

static const char XMI2_FALLBACK_NESTING_ELEMENT[]
    = "packagedElement";

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
    xmi_atom_writer_init( &((*this_).atom_writer), db_reader, (*this_).xml_writer );

    TRACE_END();
}

void xmi_interaction_writer_destroy( xmi_interaction_writer_t *this_ )
{
    TRACE_BEGIN();

    xmi_atom_writer_destroy( &((*this_).atom_writer) );
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
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), diagram_id );
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
            export_err |= xmi_atom_writer_write_xmi_comment( &((*this_).atom_writer),
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
