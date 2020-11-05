/* File: xmi_element_writer.c; Copyright and License: see below */

#include "xmi/xmi_element_writer.h"
#include "xmi/xmi_element_info_map.h"
#include "xmi/xmi_element_info.h"
#include "util/string/utf8string.h"
#include "data_id.h"
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

/* Note: when writing, each method typically starts with writing a newline, it does not end with writing a newline. */

/* IO_FILE_FORMAT_XMI2 */

static const char XMI2_ENC[]
    = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.1 : 1,1a,1e,1f */
/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.3.1.3 */
/* spec-ref: https://www.omg.org/spec/SysML/1.6/PDF chapter G.3 */
//static const char XMI2_DOC_START[]
//    = "\n<xmi:XMI xmlns:xmi=\"http://www.omg.org/spec/XMI/20131001\""
//      "\n         xmlns:uml=\"http://www.omg.org/spec/UML/20161101\""
//      "\n         xmlns:SysML=\"http://www.omg.org/spec/SysML/20181001\">"
//      "\n         <!-- XMI 2.5.1, UML 2.5.1, SysML 1.6 -->";
static const char XMI2_DOC_START[]
    = "\n<xmi:XMI xmlns:uml=\"http://www.omg.org/spec/UML/20110701\""
      "\n         xmlns:StandardProfile=\"http://www.omg.org/spec/UML/20131001/StandardProfile\""
      "\n         xmlns:xmi=\"http://www.omg.org/spec/XMI/20110701\""
      "\n         xmlns:SysML=\"http://www.omg.org/spec/SysML/20131001/SysML.xmi\">"
      /*"\n         xmlns:SysML=\"http://www.omg.org/spec/SysML/20120322/SysML\">"*/
      "\n         <!-- XMI 2.4.1, UML 2.4.1, SysML 1.4 -->";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.1 : 1,1a */
static const char XMI2_DOC_END[]
    = "\n</xmi:XMI>";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 7.5.3 + 7.5.5 */
static const char XMI2_DOC_METAINFO_START[]
    = "\n<xmi:Documentation exporter=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 7.5.3 + 7.5.5 */
static const char XMI2_DOC_METAINFO_MIDDLE[]
    = "\" exporterVersion=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 7.5.3 + 7.5.5 */
static const char XMI2_DOC_METAINFO_END[]
    = "\"/>";

static const char XMI2_XML_NS_UML[]
    = "uml:";
static const char XMI2_XML_NS_SYSML[]
    = "SysML:";
static const char XMI2_XML_NS_STDPROF[]
    = "StandardProfile:";

    /* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2e */
static const char XMI2_GENERIC_ID_START[]
    = "xmi:id=\"";
/* spec-ref: https://www.omg.org/spec/XMI/2.5.1/PDF chapter 9.5.2 : 2e */
static const char XMI2_GENERIC_ID_END[]
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

static const char XMI2_NL[]
    = "\n";

/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.2.2, 12.3.5 */
/* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Packages */
static const char XMI2_UML_MODEL_START[]
    = "\n<uml:Model xmi:type=\"uml:Model\" xmi:id=\"00000\" ";
static const char XMI2_UML_MODEL_MIDDLE[]
    = ">";
static const char XMI2_UML_MODEL_END[]
    = "\n</uml:Model>";

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

/* spec-ref: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Packages */
/*
static const char XMI2_UML_STEREOTYPE_START[]
    = "\n<ownedStereotype ";
static const char XMI2_UML_STEREOTYPE_MIDDLE[]
    = ">";
static const char XMI2_UML_STEREOTYPE_END[]
    = "\n</ownedStereotype>";
*/

/* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: all */
static const char XMI2_EXT_BASE_ELEMENT_START[]
    = "base_";
static const char XMI2_EXT_BASE_ELEMENT_MIDDLE[]
    = "=\"";
static const char XMI2_EXT_BASE_ELEMENT_END[]
    = "\" ";

static const char XMI2_FALLBACK_NESTING_ELEMENT[]
    = "packagedElement";

static const char XMI2_STATE_REGION_NESTING_STATE[]
    = "region";
static const char XMI2_STATE_REGION_TYPE[]
    = "Region";
static const char XMI2_COMMENT_TYPE[]
    = "Comment";

void xmi_element_writer_init ( xmi_element_writer_t *this_,
                               data_database_reader_t *db_reader,
                               data_stat_t *io_export_stat,
                               universal_output_stream_t *output )
{
    TRACE_BEGIN();
    assert( NULL != output );
    assert( NULL != io_export_stat );
    assert( NULL != db_reader );

    (*this_).mode = IO_WRITER_PASS_BASE;
    (*this_).export_stat = io_export_stat;

    xml_writer_init( &((*this_).xml_writer), output );
    xmi_type_converter_init( &((*this_).xmi_types) );

    TRACE_END();
}

void xmi_element_writer_destroy( xmi_element_writer_t *this_ )
{
    TRACE_BEGIN();

    xmi_type_converter_destroy( &((*this_).xmi_types) );
    xml_writer_destroy( &((*this_).xml_writer) );

    TRACE_END();
}

int xmi_element_writer_write_header( xmi_element_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    assert ( NULL != document_title );
    int export_err = 0;

    xml_writer_reset_indent ( &((*this_).xml_writer) );

    /* xml header */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_ENC );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_START );
    /* meta info */
    xml_writer_increase_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_METAINFO_START );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), META_INFO_PROGRAM_ID_STR );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_METAINFO_MIDDLE );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), META_VERSION_STR );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_METAINFO_END );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_start_main( xmi_element_writer_t *this_, const char *document_title )
{
    TRACE_BEGIN();
    int export_err = 0;

    /* uml model */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_MODEL_START );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_START );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_END );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_MODEL_MIDDLE );
    xml_writer_increase_indent ( &((*this_).xml_writer) );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_start_nested_classifier( xmi_element_writer_t *this_,
                                                data_classifier_type_t parent_type,
                                                const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    int export_err = 0;

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* nesting_property;
        const int nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_classifier( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         data_classifier_get_main_type(classifier_ptr),
                                                                         &nesting_property
                                                                       );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write a classifier to an illegal place. */
            /* This can happen in the fallback case. */
            /* During the regular tree traversal, xmi_element_writer_can_classifier_nest_classifier is checked and adhered. */
            TRACE_INFO("xmi_element_writer: request to write a classifier to an illegal place!")
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            const data_id_t classifier_id = data_classifier_get_data_id(classifier_ptr);
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n<!-- COMMENT ON UML-CONFORMANCE: Unsuitable nested type of " );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n<!-- PROPOSAL: Pack the " );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " into a more suitable container or change its type -->" );
            /* use a fallback */
            nesting_property = XMI2_FALLBACK_NESTING_ELEMENT;
        }

        /* write nesting tag */
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), nesting_property );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
        xml_writer_increase_indent ( &((*this_).xml_writer) );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_write_classifier( xmi_element_writer_t *this_,
                                         data_classifier_type_t parent_type,
                                         const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    int export_err = 0;

    const char *const classifier_name = data_classifier_get_name_ptr(classifier_ptr);
    const char *const classifier_stereo = data_classifier_get_stereotype_ptr(classifier_ptr);
    const size_t classifier_stereo_len = utf8string_get_length(classifier_stereo);
    const char *const classifier_descr = data_classifier_get_description_ptr(classifier_ptr);
    const size_t classifier_descr_len = utf8string_get_length(classifier_descr);
    const data_id_t classifier_id = data_classifier_get_data_id(classifier_ptr);
    const data_classifier_type_t classifier_type = data_classifier_get_main_type(classifier_ptr);
    const xmi_element_info_t *classifier_info
        = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                               classifier_type,
                                               (parent_type==DATA_CLASSIFIER_TYPE_UML_STATE)
                                             );

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_XML_NS_UML );
        const char* c_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                             parent_type,
                                                                             classifier_type,
                                                                             XMI_SPEC_UML
                                                                           );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), c_type );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
        export_err |= xmi_element_writer_private_encode_xmi_id( this_, classifier_id );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );

        if ( xmi_element_info_is_a_named_element( classifier_info ) )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_END );
        }

        if ( NULL != xmi_element_info_get_additional_properties( classifier_info ) )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                                   xmi_element_info_get_additional_properties( classifier_info )
                                                 );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
        }

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_START );
        export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_END );

        if ( 0 != classifier_stereo_len )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                                   "\n<!-- note: export of stereotypes is subject to change -->"
                                                 );
/* TODO */                export_err |= xmi_element_writer_private_write_xmi_comment( this_,
/* TODO */                                                                            classifier_id,
/* TODO */                                                                            "stereotype",
/* TODO */                                                                            classifier_stereo
/* TODO */                                                                          );
        }
        if ( ! xmi_element_info_is_a_named_element( classifier_info ) )
        {
            export_err |= xmi_element_writer_private_write_xmi_comment( this_,
                                                                        classifier_id,
                                                                        "name",
                                                                        classifier_name
                                                                      );
        }
        if ( classifier_type == DATA_CLASSIFIER_TYPE_UML_COMMENT )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_COMMENT_BODY_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_descr );

            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_COMMENT_BODY_END );
        }
        else if ( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
        {
            /* nothing to do here, classifier_descr will be written later at IO_WRITER_PASS_PROFILE */
        }
        else if ( 0 != classifier_descr_len )
        {
            export_err |= xmi_element_writer_private_write_xmi_comment( this_,
                                                                        classifier_id,
                                                                        "specification",
                                                                        classifier_descr
                                                                      );
        }

        /* generate start of pseudo subelement region to statemachines and states */
        if ( classifier_type == DATA_CLASSIFIER_TYPE_UML_STATE )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_STATE_REGION_NESTING_STATE );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_XML_NS_UML );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_STATE_REGION_TYPE );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "#region" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );
        }

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_EXPORTED );
    }
    else if ( (*this_).mode == IO_WRITER_PASS_PROFILE )
    {
        /* write profile tag if sysml/standardprofile-only extention */
        if ( xmi_type_converter_get_xmi_spec_of_classifier( &((*this_).xmi_types), classifier_type ) == XMI_SPEC_SYSML )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_XML_NS_SYSML );
            const char* profile_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                                       parent_type,
                                                                                       classifier_type,
                                                                                       XMI_SPEC_SYSML
                                                                                     );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), profile_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "1" );
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, classifier_id );
            /*export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), "#profile" );*/
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_START );
            const char* base_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                                    parent_type,
                                                                                    classifier_type,
                                                                                    XMI_SPEC_UML
                                                                                  );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), base_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_MIDDLE );
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );
            xml_writer_increase_indent ( &((*this_).xml_writer) );

            if ( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "<id>" );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "1" );
                export_err |= xmi_element_writer_private_encode_xmi_id( this_, classifier_id );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "</id>" );

                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "<text>" );
                xml_writer_increase_indent ( &((*this_).xml_writer) );

                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_descr );

                xml_writer_decrease_indent ( &((*this_).xml_writer) );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n</text>" );

                /*
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "<master xmi:idref=\"" );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "1" );
                export_err |= xmi_element_writer_private_encode_xmi_id( this_, classifier_id );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\" />" );
                */
            }

            /*
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_END );
            */

            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_XML_NS_SYSML );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), profile_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
        }
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_write_feature( xmi_element_writer_t *this_,
                                      data_classifier_type_t parent_type,
                                      const data_feature_t *feature_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    int export_err = 0;

    const char *const feature_key = data_feature_get_key_ptr( feature_ptr );
    const char *const feature_value = data_feature_get_value_ptr( feature_ptr );
    const size_t feature_value_len = utf8string_get_length(feature_value);
    const char *const feature_descr = data_feature_get_description_ptr( feature_ptr );
    const size_t feature_descr_len = utf8string_get_length(feature_descr);
    const data_id_t feature_id = data_feature_get_data_id( feature_ptr );
    const data_feature_type_t feature_type = data_feature_get_main_type( feature_ptr );
    const xmi_element_info_t *feature_info
        = xmi_element_info_map_get_feature( &xmi_element_info_map_standard, feature_type );

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* owning_type;
        const int owning_err
            = xmi_type_converter_get_xmi_owning_property_of_feature( &((*this_).xmi_types),
                                                                     parent_type,
                                                                     feature_type,
                                                                     &owning_type
                                                                   );
        if ( owning_err != 0 )
        {
            /* The caller requested to write a feature to an illegal place */
            TRACE_INFO("xmi_element_writer: request to write a feature to an illegal place!")
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n<!-- COMMENT ON UML-CONFORMANCE: Unsuitable parent type of " );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), feature_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->" );
            if ( feature_type == DATA_FEATURE_TYPE_LIFELINE )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n<!-- PROPOSAL: None, maybe there is a fix in a future version of crystal_facet_uml -->" );
                /* TODO */
            }
            else
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n<!-- PROPOSAL: Pack the " );
                export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), feature_id );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " into a more suitable container or change its type -->" );
            }
        }

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), owning_type );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_XML_NS_UML );
        const char* f_type = xmi_type_converter_get_xmi_type_of_feature ( &((*this_).xmi_types),
                                                                          feature_type,
                                                                          XMI_SPEC_UML
                                                                        );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), f_type );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
        export_err |= xmi_element_writer_private_encode_xmi_id( this_, feature_id );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_START );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_END );

        if ( NULL != xmi_element_info_get_additional_properties( feature_info ) )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                                   xmi_element_info_get_additional_properties( feature_info )
                                                 );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
        }

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );
        /*
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_START );
        export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), feature_id );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_END );
        */
        xml_writer_increase_indent ( &((*this_).xml_writer) );

        if ( 0 != feature_value_len )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                                   "\n<!-- note: export of valuetypes is subject to change -->"
                                                 );
            export_err |= xmi_element_writer_private_write_xmi_comment( this_,
                                                                        feature_id,
                                                                        "valuetype",
                                                                        feature_value
                                                                      );
        }

        if ( 0 != feature_descr_len )
        {
            export_err |= xmi_element_writer_private_write_xmi_comment( this_,
                                                                        feature_id,
                                                                        "specification",
                                                                        feature_descr
                                                                      );
        }
        /* TODO: A Lifeline must have an interaction property */

        xml_writer_decrease_indent ( &((*this_).xml_writer) );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), owning_type );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_write_relationship( xmi_element_writer_t *this_,
                                           data_classifier_type_t parent_type,
                                           const data_relationship_t *relation_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    /* NULL is allowed here: dest_classifier_ptr */
    int export_err = 0;

    const char *const relation_name = data_relationship_get_name_ptr( relation_ptr );
    const data_id_t relation_id = data_relationship_get_data_id( relation_ptr );
    const char *const relation_descr = data_relationship_get_description_ptr( relation_ptr );
    const size_t relation_descr_len = utf8string_get_length(relation_descr);
    const data_id_t from_classifier_id = data_relationship_get_from_classifier_data_id( relation_ptr );
    const data_id_t from_feature_id = data_relationship_get_from_feature_data_id( relation_ptr );
    const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( relation_ptr );
    const data_id_t to_feature_id = data_relationship_get_to_feature_data_id( relation_ptr );
    const data_relationship_type_t relation_type = data_relationship_get_main_type( relation_ptr );
    const xmi_element_info_t *relation_info
        = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                 relation_type,
                                                 (parent_type==DATA_CLASSIFIER_TYPE_UML_STATE)
                                               );

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* nesting_property;
        const int nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_relationship( &((*this_).xmi_types),
                                                                           parent_type,
                                                                           relation_type,
                                                                           &nesting_property
                                                                         );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write a relationship to an illegal place */
            TRACE_INFO("xmi_element_writer: request to write a relationship to an illegal place!")
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n<!-- COMMENT ON UML-CONFORMANCE: Unsuitable parent type of " );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " -->" );
            /*
            if ( feature_type == DATA_FEATURE_TYPE_LIFELINE )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n<!-- PROPOSAL: None, maybe there is a fix in a future version of crystal_facet_uml -->" );
            }
            else
            */
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n<!-- PROPOSAL: Change either the " );
                export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), " or the classifier at its source end to a more suitable type -->" );
                /* classifier types at target end are not yet checked */
            }
            /* use a fallback */
            nesting_property = XMI2_FALLBACK_NESTING_ELEMENT;
        }

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), nesting_property );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_XML_NS_UML );
        const char* r_type = xmi_type_converter_get_xmi_type_of_relationship ( &((*this_).xmi_types),
                                                                               parent_type,
                                                                               relation_type,
                                                                               XMI_SPEC_UML
                                                                             );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), r_type );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
        export_err |= xmi_element_writer_private_encode_xmi_id( this_, relation_id );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_START );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_NAME_END );

        if ( NULL != xmi_element_info_get_additional_properties( relation_info ) )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                                   xmi_element_info_get_additional_properties( relation_info )
                                                 );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
        }

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );
        /*
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_START );
        export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_END );
        */
        xml_writer_increase_indent ( &((*this_).xml_writer) );

        if ( 0 != relation_descr_len )
        {
            export_err |= xmi_element_writer_private_write_xmi_comment( this_,
                                                                        relation_id,
                                                                        "specification",
                                                                        relation_descr
                                                                      );
        }

        /* source */
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
        const char* from_type = xmi_type_converter_get_xmi_from_property_of_relationship ( &((*this_).xmi_types),
                                                                                           parent_type,
                                                                                           relation_type
                                                                                         );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), from_type );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_IDREF_START );
        if ( data_id_is_valid( &from_feature_id ) )
        {
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, from_feature_id );
        }
        else
        {
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, from_classifier_id );
        }
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_IDREF_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );

        /* destination */
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
        const char* to_type = xmi_type_converter_get_xmi_to_property_of_relationship ( &((*this_).xmi_types),
                                                                                       parent_type,
                                                                                       relation_type
                                                                                     );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), to_type );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_IDREF_START );
        if ( data_id_is_valid( &to_feature_id ) )
        {
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, to_feature_id );
        }
        else
        {
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, to_classifier_id );
        }
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_IDREF_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );

        xml_writer_decrease_indent ( &((*this_).xml_writer) );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), nesting_property );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_EXPORTED );
    }
    else if ( (*this_).mode == IO_WRITER_PASS_PROFILE )
    {
        /* write profile tag if sysml/standardprofile-only extention */
        if ( xmi_type_converter_get_xmi_spec_of_relationship( &((*this_).xmi_types), relation_type ) == XMI_SPEC_STANDARD )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_XML_NS_STDPROF );
            const char* profile_type = xmi_type_converter_get_xmi_type_of_relationship ( &((*this_).xmi_types),
                                                                                         parent_type,
                                                                                         relation_type,
                                                                                         XMI_SPEC_STANDARD
                                                                                       );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), profile_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "1" );
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, relation_id );
            /*export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), "#profile" );*/
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_START );
            const char* base_type = xmi_type_converter_get_xmi_type_of_relationship ( &((*this_).xmi_types),
                                                                                      parent_type,
                                                                                      relation_type,
                                                                                      XMI_SPEC_UML
                                                                                    );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), base_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_MIDDLE );
            export_err |= xmi_element_writer_private_encode_xmi_id( this_, relation_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );
            /*
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), relation_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_END );
            */
        }
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_end_nested_classifier( xmi_element_writer_t *this_,
                                              data_classifier_type_t parent_type,
                                              const data_classifier_t *classifier_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    const data_classifier_type_t classifier_type = data_classifier_get_main_type(classifier_ptr);
    int export_err = 0;

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        /* generate end to pseudo subelement region to statemachines and states */
        if ( classifier_type == DATA_CLASSIFIER_TYPE_UML_STATE )
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_STATE_REGION_NESTING_STATE );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
        }

        /* determine nesting tag */
        const char* nesting_property;
        const int nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_classifier( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         classifier_type,
                                                                         &nesting_property
                                                                       );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write a classifier to an illegal place */
            /* use a fallback */
            nesting_property = XMI2_FALLBACK_NESTING_ELEMENT;
        }

        /* adjust indentation, write end tag */
        xml_writer_decrease_indent ( &((*this_).xml_writer) );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), nesting_property );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_end_main( xmi_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_MODEL_END );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_write_footer( xmi_element_writer_t *this_ )
{
    TRACE_BEGIN();
    int export_err = 0;

    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_END );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_private_write_xmi_comment( xmi_element_writer_t *this_,
                                                data_id_t element_id,
                                                const char *comment_type,
                                                const char *comment )

{
    TRACE_BEGIN();
    assert( NULL != comment_type );
    assert( NULL != comment );
    int export_err = 0;

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_OWNED_COMMENT_START );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_START );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_XML_NS_UML );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_COMMENT_TYPE );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_TYPE_END );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_START );
    export_err |= xmi_element_writer_private_encode_xmi_id( this_, element_id );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), "#" );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), comment_type );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_ID_END );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_OWNED_COMMENT_MIDDLE );
    xml_writer_increase_indent ( &((*this_).xml_writer) );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_COMMENT_BODY_START );
    xml_writer_increase_indent ( &((*this_).xml_writer) );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_NL );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), comment );

    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_COMMENT_BODY_END );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_ANNOTATED_ELEMENT_START );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_IDREF_START );
    export_err |= xmi_element_writer_private_encode_xmi_id( this_, element_id );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_GENERIC_IDREF_END );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_ANNOTATED_ELEMENT_END );

    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_OWNED_COMMENT_END );

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_element_writer_private_encode_xmi_id( xmi_element_writer_t *this_,
                                              data_id_t element_id )

{
    TRACE_BEGIN();
    int export_err = 0;

    switch ( data_id_get_table(&element_id) )
    {
        case DATA_TABLE_VOID:
        {
            TSLOG_WARNING( "invalid data_id_t at xmi_element_writer_private_encode_xmi_id." ); /* "1" is reserved for profiles */
            export_err |= -1;
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "6" /* instead of C */ );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "4" /* instead of F */ );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "8" /* instead of R */ );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "3" /* instead of E */ );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "9" /* instead of D */ );
        }
        break;

        default:
        {
            TSLOG_ERROR( "unknown data_id_t at xmi_element_writer_private_encode_xmi_id." );
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
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "000" );
                }
                else
                {
                    /* row_id is negative */
                }
            }
            else
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "00" );
            }
        }
        else
        {
            if ( 1000 > data_id_get_row_id(&element_id) )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "0" );
            }
            else
            {
                /* row_id is greater than 1000 */
            }
        }
        export_err |= xml_writer_write_int ( &((*this_).xml_writer), data_id_get_row_id(&element_id) );
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
