/* File: xmi_atom_writer.c; Copyright and License: see below */

#include "xmi/xmi_atom_writer.h"
#include "xmi/xmi_element_info.h"
#include "xmi/xmi_element_info_map.h"
#include "xmi/xmi_xml.h"
#include "util/string/utf8string.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <stdbool.h>
#include <assert.h>

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

static const char XMI2_COMMENT_TYPE[]
    = "Comment";

void xmi_atom_writer_init ( xmi_atom_writer_t *this_,
                            xml_writer_t *out_writer )
{
    TRACE_BEGIN();
    assert( NULL != out_writer );

    (*this_).xml_writer = out_writer;

    TRACE_END();
}

void xmi_atom_writer_destroy( xmi_atom_writer_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

int xmi_atom_writer_write_xmi_comment( xmi_atom_writer_t *this_,
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
    export_err |= xmi_atom_writer_encode_xmi_id( this_, element_id );
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

    /* TODO: annotatedElement is not mandatory - refers to explicitly drawn relationships */
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_ANNOTATED_ELEMENT_START );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_IDREF_START );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, element_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_IDREF_END );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_ANNOTATED_ELEMENT_END );

    xml_writer_decrease_indent ( (*this_).xml_writer );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI2_UML_OWNED_COMMENT_END );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_encode_xmi_id( xmi_atom_writer_t *this_,
                                   data_id_t element_id )

{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( data_id_get_table(&element_id) )
    {
        case DATA_TABLE_VOID:
        {
            TSLOG_WARNING( "invalid data_id_t at xmi_atom_writer_encode_xmi_id." ); /* "1" is reserved for profiles */
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
            TSLOG_ERROR( "unknown data_id_t at xmi_atom_writer_encode_xmi_id." );
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

int xmi_atom_writer_report_unknown_classifier( xmi_atom_writer_t *this_,
                                               data_id_t fact_classifier_id,
                                               data_classifier_type_t fact_classifier_type )
{
    TRACE_BEGIN();
    int export_err = 0;

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, fact_classifier_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, fact_classifier_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") has type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_classifier_type );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " which is unknown to this program version " );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_report_unknown_feature( xmi_atom_writer_t *this_,
                                            data_id_t fact_feature_id,
                                            data_feature_type_t fact_feature_type )
{
    TRACE_BEGIN();
    int export_err = 0;

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, fact_feature_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, fact_feature_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") has type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_feature_type );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " which is unknown to this program version " );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_report_unknown_relationship( xmi_atom_writer_t *this_,
                                                 data_id_t fact_relationship_id,
                                                 data_relationship_type_t fact_relationship_type )
{
    TRACE_BEGIN();
    int export_err = 0;

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, fact_relationship_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, fact_relationship_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") has type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_relationship_type );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " which is unknown to this program version " );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_report_illegal_container( xmi_atom_writer_t *this_,
                                              data_id_t fact_classifier_id,
                                              data_classifier_type_t fact_classifier_type,
                                              data_classifier_type_t fact_parent_type )
{
    TRACE_BEGIN();
    int export_err = 0;

    const xmi_element_info_t *parent_info;
    int map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                       DATA_CLASSIFIER_TYPE_PACKAGE, /*guess, only used for an error message */
                                                       fact_parent_type,
                                                       &parent_info
                                                     );
    if ( map_err != 0 )
    {
        TSLOG_WARNING("xmi_element_info_map_get_classifier could not map DATA_CLASSIFIER_TYPE_PACKAGE");
    }
    const xmi_element_info_t *classifier_info;
    map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                   fact_parent_type,
                                                   fact_classifier_type,
                                                   &classifier_info
                                                 );
    if ( map_err != 0 )
    {
        TSLOG_WARNING_INT("xmi_element_info_map_get_classifier could not map type", fact_classifier_type );
    }

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, fact_classifier_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, fact_classifier_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") of type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_classifier_type );
    if ( classifier_info != NULL )
    {
        const char * classifier_type_name = xmi_element_info_get_name ( classifier_info );
        if ( classifier_type_name != NULL )
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
            export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, classifier_type_name );
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
        }
    }
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " is nested in a container of type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_parent_type );
    if ( parent_info != NULL )
    {
        const char * parent_type_name = xmi_element_info_get_name ( parent_info );
        if ( parent_type_name != NULL )
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
            export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, parent_type_name );
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
        }
    }
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- CONFORMANCE: " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Unsuitable nested type to container type" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- PROPOSAL:    " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Pack the nested element into a suitable container or change its type" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_report_illegal_stereotype( xmi_atom_writer_t *this_,
                                               data_id_t element_id,
                                               utf8stringview_t stereotype )
{
    TRACE_BEGIN();
    int export_err = 0;

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, element_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, element_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") has stereotype " );
    export_err |= xml_writer_write_xml_comment_view( (*this_).xml_writer, stereotype );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- CONFORMANCE: " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Stereotype unsuitable as xml tag name" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- PROPOSAL:    " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Change the name of the stereotype" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_report_illegal_datatype( xmi_atom_writer_t *this_,
                                             data_id_t feature_id,
                                             const char * datatype )
{
    TRACE_BEGIN();
    int export_err = 0;

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, feature_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, feature_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") has valuetype/datatype " );
    export_err |= xml_writer_write_xml_comment( (*this_).xml_writer, datatype );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- CONFORMANCE: " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Feature is not a TypedElement" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- PROPOSAL:    " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Clear the valuetype/datatype or change the type of the feature" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_report_illegal_parent( xmi_atom_writer_t *this_,
                                           data_id_t fact_feature_id,
                                           data_feature_type_t fact_feature_type,
                                           data_classifier_type_t fact_parent_type )
{
    TRACE_BEGIN();
    int export_err = 0;

    const xmi_element_info_t *parent_info;
    int map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                       DATA_CLASSIFIER_TYPE_PACKAGE, /*guess, only used for an error message */
                                                       fact_parent_type,
                                                       &parent_info
                                                     );
    if ( map_err != 0 )
    {
        TSLOG_WARNING("xmi_element_info_map_get_classifier could not map DATA_CLASSIFIER_TYPE_PACKAGE");
    }
    const xmi_element_info_t *feature_info;
    map_err = xmi_element_info_map_get_feature( &xmi_element_info_map_standard,
                                                fact_parent_type,
                                                fact_feature_type,
                                                &feature_info
                                              );
    if ( map_err != 0 )
    {
        TSLOG_WARNING_INT("xmi_element_info_map_get_feature could not map type", fact_feature_type );
    }

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, fact_feature_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, fact_feature_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") of type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_feature_type );
    if ( feature_info != NULL )
    {
        const char * feature_type_name = xmi_element_info_get_name ( feature_info );
        if ( feature_type_name != NULL )
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
            export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, feature_type_name );
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
        }
    }
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " is child to classifier of type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_parent_type );
    if ( parent_info != NULL )
    {
        const char * parent_type_name = xmi_element_info_get_name ( parent_info );
        if ( parent_type_name != NULL )
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
            export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, parent_type_name );
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
        }
    }
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- CONFORMANCE: " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Unsuitable child type to parent type" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- PROPOSAL:    " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Change the type of the child feature or the parent element" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_report_illegal_location( xmi_atom_writer_t *this_,
                                             data_id_t fact_relationship_id,
                                             data_relationship_type_t fact_relationship_type,
                                             data_classifier_type_t fact_hosting_type
                                           )
{
    TRACE_BEGIN();
    int export_err = 0;

    const xmi_element_info_t *host_info;
    int map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                       DATA_CLASSIFIER_TYPE_PACKAGE, /*guess, only used for an error message */
                                                       fact_hosting_type,
                                                       &host_info
                                                     );
    if ( map_err != 0 )
    {
        TSLOG_WARNING("xmi_element_info_map_get_classifier could not map DATA_CLASSIFIER_TYPE_PACKAGE");
    }
    const xmi_element_info_t *relation_info;
    map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                     (fact_hosting_type==DATA_CLASSIFIER_TYPE_STATE),
                                                     fact_relationship_type,
                                                     &relation_info
                                                   );
    if ( map_err != 0 )
    {
        TSLOG_WARNING_INT("xmi_element_info_map_get_relationship could not map type", fact_relationship_type );
    }

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, fact_relationship_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, fact_relationship_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") of type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_relationship_type );
    if ( relation_info != NULL )
    {
        const char * relationship_type_name = xmi_element_info_get_name ( relation_info );
        if ( relationship_type_name != NULL )
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
            export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, relationship_type_name );
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
        }
    }
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " is located in classifier of type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_hosting_type );
    if ( host_info != NULL )
    {
        const char * parent_type_name = xmi_element_info_get_name ( host_info );
        if ( parent_type_name != NULL )
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
            export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, parent_type_name );
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
        }
    }
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- CONFORMANCE: " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Unsuitable relationship type to hosting location type" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- PROPOSAL:    " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Change the type of the relationship" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_atom_writer_report_illegal_relationship_end ( xmi_atom_writer_t *this_,
                                                      data_id_t fact_relationship_id,
                                                      data_relationship_type_t fact_relationship_type,
                                                      data_classifier_type_t fact_hosting_type,
                                                      bool fact_from_end,
                                                      data_classifier_type_t fact_end_classifier_type,
                                                      data_feature_type_t fact_end_feature_type )
{
    TRACE_BEGIN();
    int export_err = 0;

    const xmi_element_info_t *relation_info;
    int map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                         (fact_hosting_type==DATA_CLASSIFIER_TYPE_STATE),
                                                         fact_relationship_type,
                                                         &relation_info
                                                       );
    if ( map_err != 0 )
    {
        TSLOG_WARNING_INT("xmi_element_info_map_get_relationship could not map type", fact_relationship_type );
    }

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- STATUS:      " );
    export_err |= xml_writer_write_plain_id( (*this_).xml_writer, fact_relationship_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (aka " );
    export_err |= xmi_atom_writer_encode_xmi_id( this_, fact_relationship_id );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, ") of type " );
    export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_relationship_type );
    if ( relation_info != NULL )
    {
        const char * relationship_type_name = xmi_element_info_get_name ( relation_info );
        if ( relationship_type_name != NULL )
        {
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
            export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, relationship_type_name );
            export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
        }
    }
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, fact_from_end
                                                                ? " has source of type "
                                                                : " has target of type " );
    if ( fact_end_feature_type == DATA_FEATURE_TYPE_VOID )
    {
        export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_end_classifier_type );

        const xmi_element_info_t *classifier_info;
        int map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                           fact_hosting_type,
                                                           fact_end_classifier_type,
                                                           &classifier_info
                                                         );
        if ( map_err != 0 )
        {
            TSLOG_WARNING_INT("xmi_element_info_map_get_classifier could not map type", fact_end_classifier_type );
        }
        if ( classifier_info != NULL )
        {
            const char * end_type_name = xmi_element_info_get_name ( classifier_info );
            if ( end_type_name != NULL )
            {
                export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
                export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, end_type_name );
                export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
            }
        }
    }
    else
    {
        export_err |= xml_writer_write_int ( (*this_).xml_writer, (int64_t)fact_end_feature_type );

        const xmi_element_info_t *feature_info;
        int map_err = xmi_element_info_map_get_feature( &xmi_element_info_map_standard,
                                                        fact_end_classifier_type,
                                                        fact_end_feature_type,
                                                        &feature_info
                                                      );
        if ( map_err != 0 )
        {
            TSLOG_WARNING_INT("xmi_element_info_map_get_feature could not map type", fact_end_feature_type );
        }
        if ( feature_info != NULL )
        {
            const char * end_type_name = xmi_element_info_get_name ( feature_info );
            if ( end_type_name != NULL )
            {
                export_err |= xml_writer_write_plain ( (*this_).xml_writer, " (" );
                export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, end_type_name );
                export_err |= xml_writer_write_plain ( (*this_).xml_writer, ")" );
            }
        }
    }
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- CONFORMANCE: " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Unsuitable relationship type connecting to " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, fact_from_end
                                                                 ? "source end type"
                                                                 : "target end type" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\n<!-- PROPOSAL:    " );
    export_err |= xml_writer_write_xml_enc( (*this_).xml_writer, "Change the type of the relationship" );
    export_err |= xml_writer_write_plain ( (*this_).xml_writer, " -->" );

    TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2020-2021 Andreas Warnke

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
