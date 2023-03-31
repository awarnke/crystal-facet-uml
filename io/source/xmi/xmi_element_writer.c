/* File: xmi_element_writer.c; Copyright and License: see below */

#include "xmi/xmi_element_writer.h"
#include "xmi/xmi_element_info_map.h"
#include "xmi/xmi_element_info.h"
#include "xmi/xmi_element_part.h"
#include "xmi/xmi_xml.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include "utf8stringbuf/utf8stringviewiterator.h"
#include "data_id.h"
#include "meta/meta_version.h"
#include "meta/meta_info.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* define a struct where the function pointers have the exact right signatures to avoid typecasts */
#define io_element_writer_impl_t xmi_element_writer_t
struct xmi_element_writer_io_element_writer_if_struct {
#include "io_element_writer_if.inl"
};
#undef io_element_writer_impl_t

/* the vmt implementing the interface */
static const struct xmi_element_writer_io_element_writer_if_struct xmi_element_writer_private_io_element_writer_if
    = {
        .write_header = &xmi_element_writer_write_header,
        .start_main = &xmi_element_writer_start_main,
        .can_classifier_nest_classifier = &xmi_element_writer_can_classifier_nest_classifier,
        .can_classifier_nest_relationship = &xmi_element_writer_can_classifier_nest_relationship,
        .start_classifier = &xmi_element_writer_start_classifier,
        .assemble_classifier = &xmi_element_writer_assemble_classifier,
        .end_classifier = &xmi_element_writer_end_classifier,
        .start_feature = &xmi_element_writer_start_feature,
        .assemble_feature = &xmi_element_writer_assemble_feature,
        .end_feature = &xmi_element_writer_end_feature,
        .start_relationship = &xmi_element_writer_start_relationship,
        .assemble_relationship = &xmi_element_writer_assemble_relationship,
        .end_relationship = &xmi_element_writer_end_relationship,
        .start_diagram = &xmi_element_writer_start_diagram,
        .assemble_diagram = &xmi_element_writer_assemble_diagram,
        .end_diagram = &xmi_element_writer_end_diagram,
        .start_diagramelement = &xmi_element_writer_start_diagramelement,
        .assemble_diagramelement = &xmi_element_writer_assemble_diagramelement,
        .end_diagramelement = &xmi_element_writer_end_diagramelement,
        .end_main = &xmi_element_writer_end_main,
        .write_footer = &xmi_element_writer_write_footer
    };

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
/* spec-ref: https://www.omg.org/spec/MOF/2.5.1/PDF annex A */
//static const char XMI2_DOC_START[]
//    = "\n<xmi:XMI xmlns:xmi=\"http://www.omg.org/spec/XMI/20131001\""
//      "\n         xmlns:uml=\"http://www.omg.org/spec/UML/20161101\""
//      "\n         xmlns:SysML=\"http://www.omg.org/spec/SysML/20181001\">"
//      "\n         <!-- XMI 2.5.1, UML 2.5.1, SysML 1.6 -->";
static const char XMI2_DOC_START[]
    = "\n<xmi:XMI xmlns:uml=\"http://www.omg.org/spec/UML/20110701\""
      "\n         xmlns:StandardProfile=\"http://www.omg.org/spec/UML/20131001/StandardProfile\""
      "\n         xmlns:xmi=\"http://www.omg.org/spec/XMI/20110701\""
      "\n         xmlns:SysML=\"http://www.omg.org/spec/SysML/20131001/SysML.xmi\""
      "\n         xmlns:mofext=\"http://www.omg.org/spec/MOF/20131001\""
      "\n         xmlns:LocalProfile=\"http://localhost/crystal-facet-uml/298b72db-cc85-4ed0-bd7b-01dc4efd52b4\">"
      "\n         <!-- XMI 2.4.1, UML 2.4.1, SysML 1.4, MOF 2.5.1 -->";
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

/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.2.2, 12.3.5 */
/* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Packages */
static const char XMI2_UML_MODEL_START[]
    = "\n<uml:Model xmi:type=\"uml:Model\" xmi:id=\"00000\" ";
static const char XMI2_UML_MODEL_MIDDLE[]
    = ">";
static const char XMI2_UML_MODEL_END[]
    = "\n</uml:Model>";

/* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.8.2 */
static const char XMI2_UML_COMMENT_BODY_START[]
    = "\n<body>";
static const char XMI2_UML_COMMENT_BODY_END[]
    = "\n</body>";

/* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: all */
static const char XMI2_EXT_BASE_ELEMENT_START[]
    = "base_";
static const char XMI2_EXT_BASE_ELEMENT_MIDDLE[]
    = "=\"";
static const char XMI2_EXT_BASE_ELEMENT_END[]
    = "\" ";

static const char XMI2_STATE_REGION_NESTING_STATE[]
    = "region";
static const char XMI2_STATE_REGION_TYPE[]
    = "Region";

void xmi_element_writer_init ( xmi_element_writer_t *this_,
                               data_stat_t *io_export_stat,
                               universal_output_stream_t *output )
{
    U8_TRACE_BEGIN();
    assert( NULL != output );
    assert( NULL != io_export_stat );

    (*this_).mode = XMI_WRITER_PASS_BASE;
    (*this_).export_stat = io_export_stat;

    io_element_writer_private_init( &((*this_).element_writer),
                                    (io_element_writer_if_t*) &xmi_element_writer_private_io_element_writer_if,
                                    this_
                                  );
    xml_writer_init( &((*this_).xml_writer), output );
    xmi_type_converter_init( &((*this_).xmi_types) );
    xmi_atom_writer_init( &((*this_).atom_writer), &((*this_).xml_writer) );

    xmi_interaction_writer_init( &((*this_).interaction_writer),
                                 io_export_stat,
                                 &((*this_).xml_writer)
                               );

    U8_TRACE_END();
}

void xmi_element_writer_destroy( xmi_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    xmi_interaction_writer_destroy( &((*this_).interaction_writer) );

    xmi_atom_writer_destroy( &((*this_).atom_writer) );
    xmi_type_converter_destroy( &((*this_).xmi_types) );
    xml_writer_destroy( &((*this_).xml_writer) );
    io_element_writer_private_destroy( &((*this_).element_writer) );

    U8_TRACE_END();
}

io_element_writer_t * xmi_element_writer_get_element_writer( xmi_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();

    io_element_writer_t * base = &((*this_).element_writer);

    U8_TRACE_END();
    return base;
}

void xmi_element_writer_set_mode( xmi_element_writer_t *this_, xmi_writer_pass_t mode )
{
    U8_TRACE_BEGIN();

    U8_TRACE_INFO_STR("mode:", (mode==XMI_WRITER_PASS_BASE) ? "XMI_WRITER_PASS_BASE" : "XMI_WRITER_PASS_PROFILE");
    (*this_).mode = mode;

    U8_TRACE_END();
}

u8_error_t xmi_element_writer_write_header( xmi_element_writer_t *this_, const char *document_title )
{
    U8_TRACE_BEGIN();
    assert ( NULL != document_title );
    u8_error_t export_err = U8_ERROR_NONE;

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

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_start_main( xmi_element_writer_t *this_, const char *document_title )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    /* uml model */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_MODEL_START );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_START );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), document_title );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_END );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_MODEL_MIDDLE );
    xml_writer_increase_indent ( &((*this_).xml_writer) );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

bool xmi_element_writer_can_classifier_nest_classifier ( xmi_element_writer_t *this_,
                                                         data_classifier_type_t host_type,
                                                         data_classifier_type_t child_type )
{
    const bool base_pass = ( XMI_WRITER_PASS_BASE == (*this_).mode );
    const bool can_nest = xmi_type_converter_can_nest_classifier( &((*this_).xmi_types), host_type, child_type );
    return ( can_nest && base_pass );
}

bool xmi_element_writer_can_classifier_nest_relationship ( xmi_element_writer_t *this_,
                                                           data_classifier_type_t host_type,
                                                           data_relationship_type_t child_type )
{
    const bool base_pass = ( XMI_WRITER_PASS_BASE == (*this_).mode );
    const bool can_nest = xmi_type_converter_can_nest_relationship( &((*this_).xmi_types), host_type, child_type );
    return ( can_nest && base_pass );
}

u8_error_t xmi_element_writer_start_classifier( xmi_element_writer_t *this_,
                                                data_classifier_type_t host_type,
                                                const data_classifier_t *classifier_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    const data_id_t classifier_id = data_classifier_get_data_id(classifier_ptr);
    const data_classifier_type_t classifier_type = data_classifier_get_main_type(classifier_ptr);
    const xmi_element_info_t *classifier_info;
    const u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                                    host_type,
                                                                    classifier_type,
                                                                    &classifier_info
                                                                  );
    U8_TRACE_INFO_STR("xmi_element_info_t:", xmi_element_info_get_name( classifier_info ));

    if ( map_err != 0 )
    {
        /* The caller requested to write a classifier of unknown type */
        U8_TRACE_INFO("xmi_element_writer: request to write a classifier of unknown type!");

        if ( (*this_).mode == XMI_WRITER_PASS_BASE )  /* count errors only once */
        {
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_unknown_classifier( &((*this_).atom_writer),
                                                                     classifier_id,
                                                                     classifier_type
                                                                   );
        }
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_BASE )
    {

        /* determine nesting tag */
        const char* nesting_property;
        const u8_error_t nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_classifier( &((*this_).xmi_types),
                                                                         host_type,
                                                                         classifier_type,
                                                                         &nesting_property
                                                                       );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write a classifier to an illegal place. */
            /* This can happen in the fallback case. */
            /* During the regular tree traversal, xmi_element_writer_can_classifier_nest_classifier is checked and adhered. */
            U8_TRACE_INFO("xmi_element_writer: request to write a classifier to an illegal place!");
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_illegal_container( &((*this_).atom_writer),
                                                                    classifier_id,
                                                                    classifier_type,
                                                                    host_type
                                                                  );
            /* use a fallback */
            nesting_property = XMI_ELEMENT_PART_FALLBACK_NESTING_ELEMENT;
        }

        /* write nesting tag */
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), nesting_property );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
        xml_writer_increase_indent ( &((*this_).xml_writer) );
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_assemble_classifier( xmi_element_writer_t *this_,
                                                   data_classifier_type_t host_type,
                                                   const data_classifier_t *classifier_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    const char *const classifier_name = data_classifier_get_name_const(classifier_ptr);
    const char *const classifier_stereo = data_classifier_get_stereotype_const(classifier_ptr);
    const char *const classifier_descr = data_classifier_get_description_const(classifier_ptr);
    const size_t classifier_descr_len = utf8string_get_length(classifier_descr);
    const data_id_t classifier_id = data_classifier_get_data_id(classifier_ptr);
    const data_classifier_type_t classifier_type = data_classifier_get_main_type(classifier_ptr);
    const xmi_element_info_t *classifier_info;
    const u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                                    host_type,
                                                                    classifier_type,
                                                                    &classifier_info
                                                                  );

    if ( map_err != 0 )
    {
        /* The caller requested to write a classifier of unknown type, error was already logged at xmi_element_writer_start_classifier */
        U8_TRACE_INFO_INT("xmi_element_writer: request to write a classifier of unknown type", classifier_type );
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_BASE )
    {
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_UML );
        const char* c_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                             host_type,
                                                                             classifier_type,
                                                                             XMI_SPEC_UML
                                                                           );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), c_type );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), classifier_id );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );

        if ( xmi_element_info_is_a_named_element( classifier_info ) )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_END );
        }
        else
        {
            /* only comments and imaged do not have names */
            assert(( classifier_type == DATA_CLASSIFIER_TYPE_COMMENT )||( classifier_type == DATA_CLASSIFIER_TYPE_IMAGE ));
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

        if ( classifier_type == DATA_CLASSIFIER_TYPE_COMMENT )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_COMMENT_BODY_START );
            xml_writer_increase_indent ( &((*this_).xml_writer) );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_name );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n" );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_descr );

            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_COMMENT_BODY_END );
        }
        else if ( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
        {
            /* nothing to do here, classifier_descr will be written later at XMI_WRITER_PASS_PROFILE */
        }
        else if ( 0 != classifier_descr_len )
        {
            export_err |= xmi_atom_writer_write_xmi_comment( &((*this_).atom_writer),
                                                             classifier_id,
                                                             "specification",
                                                             classifier_descr
                                                           );
        }

        /* generate extension point for use cases */
        if ( classifier_type == DATA_CLASSIFIER_TYPE_USE_CASE )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "extensionPoint" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_UML );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "ExtensionPoint" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "#extensionpoint" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), "" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );
        }

        /* generate start of pseudo subelement region to statemachines and states */
        if ( classifier_type == DATA_CLASSIFIER_TYPE_STATE )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_STATE_REGION_NESTING_STATE );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
            xml_writer_increase_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_UML );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_STATE_REGION_TYPE );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "#region" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );
        }

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_EXPORTED );
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_PROFILE )
    {
        /* write profile tag if sysml/standardprofile-only extension */
        if ( xmi_type_converter_get_xmi_spec_of_classifier( &((*this_).xmi_types), classifier_type ) == XMI_SPEC_SYSML )
        {
            const char* profile_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                                       host_type,
                                                                                       classifier_type,
                                                                                       XMI_SPEC_SYSML
                                                                                     );
            assert( profile_type != NULL );
            const char* base_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                                    host_type,
                                                                                    classifier_type,
                                                                                    XMI_SPEC_UML
                                                                                  );
            if ( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
            {
                /* the base class is a Class, but the derived property name from AbstractRequirement is base_NamedElement */
                base_type = "NamedElement";
                /* but: one could understand differently the SysML 1.6 chapter 16.3.2.5 Requirement */
            }

            export_err |=  xmi_element_writer_private_start_stereotype( this_,
                                                                        XMI_XML_NS_SYSML,
                                                                        UTF8STRINGVIEW_STR(profile_type),
                                                                        base_type,
                                                                        classifier_id
                                                                      );

            if ( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "<id>" );
                /* for postfix: see xmi_element_writer_private_start_stereotype */
                export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), classifier_id );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "#" );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), profile_type );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "</id>" );

                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "<text>" );
                xml_writer_increase_indent ( &((*this_).xml_writer) );

                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), classifier_descr );

                xml_writer_decrease_indent ( &((*this_).xml_writer) );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\n</text>" );

                /*
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "<master xmi:idref=\"" );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "1" );
                export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer),classifier_id );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "\" />" );
                */
            }

            /*
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_START );
            export_err |= xml_writer_write_plain_id( &((*this_).xml_writer), classifier_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_COMMENT_END );
            */

            export_err |=  xmi_element_writer_private_end_stereotype( this_, XMI_XML_NS_SYSML, UTF8STRINGVIEW_STR(profile_type) );
        }

        /* write user-defined stereotypes */
        {
            utf8stringviewiterator_t stereo_iterator;
            utf8stringviewiterator_init( &stereo_iterator,
                                         UTF8STRINGVIEW_STR(classifier_stereo),
                                         ","
                                       );
            while( utf8stringviewiterator_has_next( &stereo_iterator ) )
            {
                const utf8stringview_t stereotype_view = utf8stringviewiterator_next( &stereo_iterator );
                size_t stereotype_len = utf8stringview_get_length( stereotype_view );
                if ( stereotype_len != 0 )
                {
                    bool is_name = xml_writer_contains_xml_tag_name_characters( &((*this_).xml_writer), stereotype_view );
                    if ( is_name )
                    {
                        const char* base_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                                                host_type,
                                                                                                classifier_type,
                                                                                                XMI_SPEC_UML
                                                                                              );
                        if ( classifier_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
                        {
                            /* the base class is a Class, but the derived property name from AbstractRequirement is base_NamedElement */
                            base_type = "NamedElement";
                            /* but: one could understand differently the SysML 1.6 chapter 16.3.2.5 Requirement */
                        }

                        export_err |=  xmi_element_writer_private_start_stereotype( this_,
                                                                                    XMI_XML_NS_LOCALPROF,
                                                                                    stereotype_view,
                                                                                    base_type,
                                                                                    classifier_id
                                                                                );
                        export_err |=  xmi_element_writer_private_end_stereotype( this_, XMI_XML_NS_LOCALPROF, stereotype_view );
                    }
                    else
                    {
                        export_err |= xmi_atom_writer_report_illegal_stereotype( &((*this_).atom_writer),
                                                                                 classifier_id,
                                                                                 stereotype_view
                                                                               );
                        /* update export statistics */
                        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING );
                    }
                }
            }
        }
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_end_classifier( xmi_element_writer_t *this_,
                                              data_classifier_type_t host_type,
                                              const data_classifier_t *classifier_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != classifier_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    const data_classifier_type_t classifier_type = data_classifier_get_main_type(classifier_ptr);
    const xmi_element_info_t *classifier_info;
    const u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                                    host_type,
                                                                    classifier_type,
                                                                    &classifier_info
                                                                  );

    if ( map_err != 0 )
    {
        /* The caller requested to write a classifier of unknown type, error was already logged at xmi_element_writer_start_classifier */
        U8_TRACE_INFO_INT("xmi_element_writer: request to write a classifier of unknown type", classifier_type );
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_BASE )
    {
        /* generate end to pseudo subelement region to statemachines and states */
        if ( classifier_type == DATA_CLASSIFIER_TYPE_STATE )
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_STATE_REGION_NESTING_STATE );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
        }

        /* determine nesting tag */
        const char* nesting_property;
        const u8_error_t nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_classifier( &((*this_).xmi_types),
                                                                         host_type,
                                                                         classifier_type,
                                                                         &nesting_property
                                                                       );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write a classifier to an illegal place */
            /* use a fallback */
            nesting_property = XMI_ELEMENT_PART_FALLBACK_NESTING_ELEMENT;
        }

        /* adjust indentation, write end tag */
        xml_writer_decrease_indent ( &((*this_).xml_writer) );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), nesting_property );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_start_feature( xmi_element_writer_t *this_,
                                             data_classifier_type_t parent_type,
                                             const data_feature_t *feature_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    const char *const feature_key = data_feature_get_key_const( feature_ptr );
    const data_id_t feature_id = data_feature_get_data_id( feature_ptr );
    const data_feature_type_t feature_type = data_feature_get_main_type( feature_ptr );
    const xmi_element_info_t *feature_info;
    const u8_error_t map_err = xmi_element_info_map_get_feature( &xmi_element_info_map_standard,
                                                                 parent_type,
                                                                 feature_type,
                                                                 &feature_info
                                                               );
    U8_TRACE_INFO_STR("xmi_element_info_t:", xmi_element_info_get_name( feature_info ));

    if ( map_err != 0 )
    {
        /* The caller requested to write a feature of unknown type */
        U8_TRACE_INFO("xmi_element_writer: request to write a feature of unknown type!");

        if ( (*this_).mode == XMI_WRITER_PASS_BASE )  /* count errors only once */
        {
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_ERROR );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_unknown_feature( &((*this_).atom_writer),
                                                                  feature_id,
                                                                  feature_type
                                                                );
        }
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_BASE )
    {
        if ( ! xmi_element_info_is_a_tagged_value( feature_info ) )
        {
            /* determine nesting tag */
            const char* owning_type;
            const u8_error_t owning_err
                = xmi_type_converter_get_xmi_owning_property_of_feature( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         feature_type,
                                                                         &owning_type
                                                                       );
            if ( owning_err != 0 )
            {
                /* The caller requested to write a feature to an illegal place */
                U8_TRACE_INFO("xmi_element_writer: request to write a feature to an illegal place!");
                /* update export statistics */
                data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_WARNING );
                /* inform the user via an XML comment: */
                export_err |= xmi_atom_writer_report_illegal_parent( &((*this_).atom_writer),
                                                                     feature_id,
                                                                     feature_type,
                                                                     parent_type
                                                                   );
                owning_type = XMI_ELEMENT_PART_FALLBACK_OWNED_FEATURE;
            }

            /* write nesting tag */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), owning_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

            /* write type attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_UML );
            const char* f_type = xmi_type_converter_get_xmi_type_of_feature ( &((*this_).xmi_types),
                                                                              parent_type,
                                                                              feature_type,
                                                                              XMI_SPEC_UML
                                                                            );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), f_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_END );

            /* write id attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), feature_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );

            /* write name attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_END );

            if ( NULL != xmi_element_info_get_additional_properties( feature_info ) )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                                       xmi_element_info_get_additional_properties( feature_info )
                                                     );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
            }

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );

            xml_writer_increase_indent ( &((*this_).xml_writer) );

            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_EXPORTED );
        }
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_PROFILE )
    {
        if ( xmi_element_info_is_a_tagged_value( feature_info ) )
        {
            /* write nesting tag */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_MOFEXT );
            const char* f_type = xmi_type_converter_get_xmi_type_of_feature ( &((*this_).xmi_types),
                                                                              parent_type,
                                                                              feature_type,
                                                                              XMI_SPEC_MOF
                                                                            );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), f_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

            /* write type attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_MOFEXT );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), f_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_END );

            /* write id attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), feature_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );

            /* write name attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_key );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_END );

            /* write value attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_VALUE_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer),
                                                     data_feature_get_value_const( feature_ptr )
                                                   );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_VALUE_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );

            xml_writer_increase_indent ( &((*this_).xml_writer) );

            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_EXPORTED );
        }
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_assemble_feature( xmi_element_writer_t *this_,
                                                const data_classifier_t *parent,
                                                const data_feature_t *feature_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    assert( parent != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    const data_classifier_type_t parent_type = data_classifier_get_main_type( parent );
    const char *const feature_value = data_feature_get_value_const( feature_ptr );
    const size_t feature_value_len = utf8string_get_length(feature_value);
    const char *const feature_descr = data_feature_get_description_const( feature_ptr );
    const size_t feature_descr_len = utf8string_get_length(feature_descr);
    const data_id_t feature_id = data_feature_get_data_id( feature_ptr );
    const data_feature_type_t feature_type = data_feature_get_main_type( feature_ptr );
    const xmi_element_info_t *feature_info;
    const u8_error_t map_err = xmi_element_info_map_get_feature( &xmi_element_info_map_standard,
                                                                 parent_type,
                                                                 feature_type,
                                                                 &feature_info
                                                               );

    if ( map_err != 0 )
    {
        /* The caller requested to write a feature of unknown type, error was already logged at xmi_element_writer_start_feature */
        U8_TRACE_INFO_INT("xmi_element_writer: request to write a feature of unknown type", feature_type );
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_BASE )
    {
        if ( ! xmi_element_info_is_a_tagged_value( feature_info ) )
        {
            if ( 0 != feature_value_len )
            {
                if ( xmi_element_info_is_a_typed_element( feature_info ) )
                {
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "type" );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_START );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_UML );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "DataType" );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_END );

                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
                    export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), feature_id );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "#type" );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );

                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_START );
                    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), feature_value );
                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_END );

                    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );
                }
                else
                {
                    export_err |= xmi_atom_writer_report_illegal_datatype( &((*this_).atom_writer),
                                                                           feature_id,
                                                                           feature_value
                                                                         );
                }
            }

            if ( 0 != feature_descr_len )
            {
                export_err |= xmi_atom_writer_write_xmi_comment( &((*this_).atom_writer),
                                                                 feature_id,
                                                                 "specification",
                                                                 feature_descr
                                                               );
            }

            if ( parent_type == DATA_CLASSIFIER_TYPE_INTERACTION )
            {
                const data_id_t classifier_id = data_feature_get_classifier_data_id ( feature_ptr );
                export_err |= xmi_interaction_writer_assemble_feature( &((*this_).interaction_writer),
                                                                       classifier_id,
                                                                       DATA_CLASSIFIER_TYPE_INTERACTION,
                                                                       feature_ptr
                                                                     );
            }
        }
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_PROFILE )
    {
        if ( xmi_element_info_is_a_tagged_value( feature_info ) )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "element" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_START );
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), data_classifier_get_data_id( parent ) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );
        }
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_end_feature( xmi_element_writer_t *this_,
                                           data_classifier_type_t parent_type,
                                           const data_feature_t *feature_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    u8_error_t export_err = U8_ERROR_NONE;

    const data_feature_type_t feature_type = data_feature_get_main_type( feature_ptr );
    const xmi_element_info_t *feature_info;
    const u8_error_t map_err = xmi_element_info_map_get_feature( &xmi_element_info_map_standard,
                                                                 parent_type,
                                                                 feature_type,
                                                                 &feature_info
                                                               );

    if ( map_err != 0 )
    {
        /* The caller requested to write a feature of unknown type, error was already logged at xmi_element_writer_start_feature */
        U8_TRACE_INFO_INT("xmi_element_writer: request to write a feature of unknown type", feature_type );
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_BASE )
    {
        if ( ! xmi_element_info_is_a_tagged_value( feature_info ) )
        {
            /* determine nesting tag */
            const char* owning_type;
            const u8_error_t owning_err
                = xmi_type_converter_get_xmi_owning_property_of_feature( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         feature_type,
                                                                         &owning_type
                                                                       );
            if ( owning_err != 0 )
            {
                /* The caller requested to write a feature to an illegal place */
                U8_TRACE_INFO("xmi_element_writer: request to write a feature to an illegal place!");
                owning_type = XMI_ELEMENT_PART_FALLBACK_OWNED_FEATURE;
            }

            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), owning_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
        }
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_PROFILE )
    {
        if ( xmi_element_info_is_a_tagged_value( feature_info ) )
        {
            xml_writer_decrease_indent ( &((*this_).xml_writer) );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_MOFEXT );
            const char* f_type = xmi_type_converter_get_xmi_type_of_feature ( &((*this_).xmi_types),
                                                                              parent_type,
                                                                              feature_type,
                                                                              XMI_SPEC_MOF
                                                                            );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), f_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
        }
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_start_relationship( xmi_element_writer_t *this_,
                                                  data_classifier_type_t host_type,
                                                  const data_relationship_t *relation_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    /* NULL is allowed here: dest_classifier_ptr */
    u8_error_t export_err = U8_ERROR_NONE;

    const char *const relation_name = data_relationship_get_name_const( relation_ptr );
    const size_t relation_name_len = utf8string_get_length(relation_name);
    const char *const relation_stereo = data_relationship_get_stereotype_const(relation_ptr);
    const data_id_t relation_id = data_relationship_get_data_id( relation_ptr );
    const data_relationship_type_t relation_type = data_relationship_get_main_type( relation_ptr );
    const xmi_element_info_t *relation_info;
    const u8_error_t map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                                      (host_type==DATA_CLASSIFIER_TYPE_STATE),
                                                                      relation_type,
                                                                      &relation_info
                                                                    );
    U8_TRACE_INFO_STR("xmi_element_info_t:", xmi_element_info_get_name( relation_info ));
    const bool is_annotated_element
        = (( host_type == DATA_CLASSIFIER_TYPE_COMMENT )&&( relation_type == DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY ));

    if ( map_err != 0 )
    {
        /* The caller requested to write a relationship of unknown type */
        U8_TRACE_INFO("xmi_element_writer: request to write a relationship of unknown type!");

        if ( (*this_).mode == XMI_WRITER_PASS_BASE )  /* count errors only once */
        {
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_unknown_relationship( &((*this_).atom_writer),
                                                                       relation_id,
                                                                       relation_type
                                                                     );
        }
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* nesting_property;
        const u8_error_t nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_relationship( &((*this_).xmi_types),
                                                                           host_type,
                                                                           relation_type,
                                                                           &nesting_property
                                                                         );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write a relationship to an illegal place */
            U8_TRACE_INFO("xmi_element_writer: request to write a relationship to an illegal place!");
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_illegal_location( &((*this_).atom_writer),
                                                                   relation_id,
                                                                   relation_type,
                                                                   host_type
                                                                 );
            /* use a fallback */
            nesting_property = XMI_ELEMENT_PART_FALLBACK_NESTING_ELEMENT;
        }

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), nesting_property );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

        if ( is_annotated_element )
        {
            const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( relation_ptr );
            const data_id_t to_feature_id = data_relationship_get_to_feature_data_id( relation_ptr );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_START );
            if ( data_id_is_valid( &to_feature_id ) )
            {
                export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), to_feature_id );
            }
            else
            {
                export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), to_classifier_id );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_END );
        }
        else
        {
            /* write type attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_UML );
            const char* r_type = xmi_type_converter_get_xmi_type_of_relationship ( &((*this_).xmi_types),
                                                                                   host_type,
                                                                                   relation_type,
                                                                                   XMI_SPEC_UML
                                                                                 );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), r_type );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_END );

            /* write id attribute */
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer),relation_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );

            /* write name attribute */
            if ( xmi_element_info_is_a_named_element( relation_info ) )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_START );
                export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), relation_name );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_NAME_END );
            }

            if ( NULL != xmi_element_info_get_additional_properties( relation_info ) )
            {
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                                       xmi_element_info_get_additional_properties( relation_info )
                                                     );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
            }
        }

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );

        xml_writer_increase_indent ( &((*this_).xml_writer) );

        /* write name comment */
        if (( ! xmi_element_info_is_a_named_element( relation_info ) )&&( relation_name_len > 0 ))
        {
            export_err |= xmi_atom_writer_write_xmi_comment( &((*this_).atom_writer),
                                                             relation_id,
                                                             "name",
                                                             relation_name
                                                           );
        }

        /* update export statistics */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_EXPORTED );
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_PROFILE )
    {
        /* write profile tag if sysml/standardprofile-only extention */
        if ( xmi_type_converter_get_xmi_spec_of_relationship( &((*this_).xmi_types), relation_type ) == XMI_SPEC_STANDARD )
        {
            const char* profile_type = xmi_type_converter_get_xmi_type_of_relationship ( &((*this_).xmi_types),
                                                                                         host_type,
                                                                                         relation_type,
                                                                                         XMI_SPEC_STANDARD
                                                                                       );
            assert( profile_type != NULL );
            const char* base_type = xmi_type_converter_get_xmi_type_of_relationship ( &((*this_).xmi_types),
                                                                                      host_type,
                                                                                      relation_type,
                                                                                      XMI_SPEC_UML
                                                                                    );
            export_err |=  xmi_element_writer_private_start_stereotype( this_,
                                                                        XMI_XML_NS_STDPROF,
                                                                        UTF8STRINGVIEW_STR(profile_type),
                                                                        base_type,
                                                                        relation_id
                                                                      );
            export_err |=  xmi_element_writer_private_end_stereotype( this_, XMI_XML_NS_STDPROF, UTF8STRINGVIEW_STR(profile_type) );
        }

        /* write user-defined stereotypes */
        {
            utf8stringviewiterator_t stereo_iterator;
            utf8stringviewiterator_init( &stereo_iterator,
                                         UTF8STRINGVIEW_STR(relation_stereo),
                                         ","
                                       );
            while( utf8stringviewiterator_has_next( &stereo_iterator ) )
            {
                const utf8stringview_t stereotype_view = utf8stringviewiterator_next( &stereo_iterator );
                size_t stereotype_len = utf8stringview_get_length( stereotype_view );
                if ( stereotype_len != 0 )
                {
                    bool is_name = xml_writer_contains_xml_tag_name_characters( &((*this_).xml_writer), stereotype_view );
                    if ( is_name )
                    {
                        const char* base_type = xmi_type_converter_get_xmi_type_of_relationship ( &((*this_).xmi_types),
                                                                                                  host_type,
                                                                                                  relation_type,
                                                                                                  XMI_SPEC_UML
                                                                                                );

                        export_err |=  xmi_element_writer_private_start_stereotype( this_,
                                                                                    XMI_XML_NS_LOCALPROF,
                                                                                    stereotype_view,
                                                                                    base_type,
                                                                                    relation_id
                                                                                  );
                        export_err |=  xmi_element_writer_private_end_stereotype( this_, XMI_XML_NS_LOCALPROF, stereotype_view );
                    }
                    else
                    {
                        export_err |= xmi_atom_writer_report_illegal_stereotype( &((*this_).atom_writer),
                                                                                 relation_id,
                                                                                 stereotype_view
                                                                               );
                        /* update export statistics */
                        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_WARNING );
                    }
                }
            }
        }
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_assemble_relationship( xmi_element_writer_t *this_,
                                                     const data_classifier_t *host,
                                                     const data_relationship_t *relation_ptr,
                                                     const data_classifier_t *from_c,
                                                     const data_feature_t *from_f,
                                                     const data_classifier_t *to_c,
                                                     const data_feature_t *to_f )
{
    U8_TRACE_BEGIN();

    assert ( NULL != relation_ptr );
    assert ( NULL != from_c );
    assert ( NULL != to_c );

    const data_classifier_type_t from_c_type
        = data_classifier_get_main_type( from_c );
    const data_feature_type_t from_f_type
        = (from_f==NULL)
        ? DATA_FEATURE_TYPE_VOID
        : data_feature_is_valid(from_f) ? data_feature_get_main_type( from_f ) : DATA_FEATURE_TYPE_VOID;
    const data_classifier_type_t to_c_type
        = data_classifier_get_main_type( to_c );
    const data_feature_type_t to_f_type
        = (to_f==NULL)
        ? DATA_FEATURE_TYPE_VOID
        : data_feature_is_valid(to_f) ? data_feature_get_main_type( to_f ) : DATA_FEATURE_TYPE_VOID;
    data_id_t host_id;
    data_classifier_type_t host_type;
    bool host_is_source;
    if ( host == NULL )
    {
        host_id = DATA_ID_VOID;
        host_type = DATA_CLASSIFIER_TYPE_PACKAGE;  /* a uml:Model is a uml:Package*/
        host_is_source = false;
    }
    else
    {
        host_id = data_classifier_get_data_id( host );
        host_type = data_classifier_get_main_type( host );
        host_is_source = ( data_classifier_get_row_id( from_c ) == data_classifier_get_row_id( host ) );
    }

    const u8_error_t export_err
        = xmi_element_writer_private_assemble_relationship( this_,
                                                            host_type,
                                                            host_is_source,
                                                            host_id,
                                                            relation_ptr,
                                                            from_c_type,
                                                            from_f_type,
                                                            to_c_type,
                                                            to_f_type
                                                          );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_private_assemble_relationship( xmi_element_writer_t *this_,
                                                             data_classifier_type_t host_type,
                                                             bool host_is_source,
                                                             data_id_t host_id,
                                                             const data_relationship_t *relation_ptr,
                                                             data_classifier_type_t from_c_type,
                                                             data_feature_type_t from_f_type,
                                                             data_classifier_type_t to_c_type,
                                                             data_feature_type_t to_f_type )
{
    U8_TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    /* NULL is allowed here: dest_classifier_ptr */
    u8_error_t export_err = U8_ERROR_NONE;

    const data_id_t relation_id = data_relationship_get_data_id( relation_ptr );
    const char *const relation_descr = data_relationship_get_description_const( relation_ptr );
    const size_t relation_descr_len = utf8string_get_length(relation_descr);
    const data_id_t from_classifier_id = data_relationship_get_from_classifier_data_id( relation_ptr );
    const data_id_t from_feature_id = data_relationship_get_from_feature_data_id( relation_ptr );
    const data_id_t from_end_id = data_id_is_valid( &from_feature_id ) ? from_feature_id : from_classifier_id;
    const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( relation_ptr );
    const data_id_t to_feature_id = data_relationship_get_to_feature_data_id( relation_ptr );
    const data_id_t to_end_id = data_id_is_valid( &to_feature_id ) ? to_feature_id : to_classifier_id;
    const data_relationship_type_t relation_type = data_relationship_get_main_type( relation_ptr );
    const xmi_element_info_t *relation_info;
    const u8_error_t map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                                      (host_type==DATA_CLASSIFIER_TYPE_STATE),
                                                                      relation_type,
                                                                      &relation_info
                                                                    );
    if ( map_err != 0 )
    {
        /* The caller requested to write a relationship of unknown type, error was already logged at xmi_element_writer_start_relationship */
        U8_TRACE_INFO_INT("xmi_element_writer: request to write a relationship of unknown type", relation_type );
    }

    const xmi_element_info_t *from_end_info = NULL;
    if (from_f_type == DATA_FEATURE_TYPE_VOID)
    {
        const u8_error_t map_err_2
            = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard, host_type /*wrong host here*/, from_c_type, &from_end_info );
        if ( map_err_2 != 0 )
        {
            U8_TRACE_INFO_INT("xmi_element_writer: request to write a relationship from-end of unknown c-type", from_c_type );
        }
    }
    else
    {
        const u8_error_t map_err_3
            = xmi_element_info_map_get_feature( &xmi_element_info_map_standard, from_c_type, from_f_type, &from_end_info );
        if ( map_err_3 != 0 )
        {
            U8_TRACE_INFO_INT("xmi_element_writer: request to write a relationship from-end of unknown f-type", from_f_type );
        }
    }
    assert ( from_end_info != NULL );

    const xmi_element_info_t *to_end_info = NULL;
    if (to_f_type == DATA_FEATURE_TYPE_VOID)
    {
        const u8_error_t map_err_4
            = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard, host_type /*wrong host here*/, to_c_type, &to_end_info );
        if ( map_err_4 != 0 )
        {
            U8_TRACE_INFO_INT("xmi_element_writer: request to write a relationship to-end of unknown c-type", to_c_type );
        }
    }
    else
    {
        const u8_error_t map_err_5
            = xmi_element_info_map_get_feature( &xmi_element_info_map_standard, to_c_type, to_f_type, &to_end_info );
        if ( map_err_5 != 0 )
        {
            U8_TRACE_INFO_INT("xmi_element_writer: request to write a relationship to-end of unknown f-type", to_f_type );
        }
    }
    assert ( to_end_info != NULL );

    /* evaluate if xmi requires to generate fake properties */
    const bool fake_property_ends = xmi_element_info_is_a_association( relation_info );
    const bool fake_property_at_from_end
        = fake_property_ends && ( ! xmi_element_info_is_a_property( from_end_info ) );
    const bool fake_property_at_to_end
        = fake_property_ends && ( ! xmi_element_info_is_a_property( to_end_info ) );
    /* check if suppress source end */
    const bool suppress_source
        = (( relation_type == DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION )
        || ( relation_type == DATA_RELATIONSHIP_TYPE_UML_EXTEND )
        || ( relation_type == DATA_RELATIONSHIP_TYPE_UML_INCLUDE ))
        && host_is_source;
    const bool is_annotated_element
        = (( host_type == DATA_CLASSIFIER_TYPE_COMMENT )&&( relation_type == DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY ));
    const bool is_message = ( xmi_element_info_is_a_message ( relation_info ) );

    if ( map_err != 0 )
    {
        /* The caller requested to write a relationship of unknown type, error was already logged at xmi_element_writer_start_relationship */
        U8_TRACE_INFO_INT("xmi_element_writer: request to write a relationship of unknown type", relation_type );
    }
    else if (( host_type == DATA_CLASSIFIER_TYPE_INTERACTION )&&( is_message ))
    {
        export_err |= xmi_interaction_writer_assemble_relationship( &((*this_).interaction_writer),
                                                                    host_id,
                                                                    DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake host type */
                                                                    relation_ptr,
                                                                    DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake from classifier type */
                                                                    DATA_FEATURE_TYPE_LIFELINE,  /* guess from feature type */
                                                                    DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake to classifier type */
                                                                    DATA_FEATURE_TYPE_LIFELINE  /* guess to feature type */
                                                                  );
    }
    else if (( (*this_).mode == XMI_WRITER_PASS_BASE )&&( ! is_annotated_element ))
    {
        if ( 0 != relation_descr_len )
        {
            export_err |= xmi_atom_writer_write_xmi_comment( &((*this_).atom_writer),
                                                             relation_id,
                                                             "specification",
                                                             relation_descr
                                                           );
        }

        /* source */
        if ( ! suppress_source )
        {
            /* determine from type tag */
            const char* from_type_tag;
            const u8_error_t from_type_err
                = xmi_type_converter_get_xmi_from_property_of_relationship ( &((*this_).xmi_types),
                                                                             host_type,
                                                                             relation_type,
                                                                             from_c_type,
                                                                             from_f_type,
                                                                             &from_type_tag
                                                                           );
            if ( from_type_err != 0 )
            {
                /* The caller requested to write a relationship of illegal source end type */
                U8_TRACE_INFO("xmi_element_writer: request to write a relationship connecting an illegal source end type!");
                /* update export statistics */
                data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
                /* inform the user via an XML comment: */
                export_err |= xmi_atom_writer_report_illegal_relationship_end ( &((*this_).atom_writer),
                                                                                relation_id,
                                                                                relation_type,
                                                                                host_type,
                                                                                true /* = from_end */,
                                                                                from_c_type,
                                                                                from_f_type
                                                                              );
            }

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
            export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), from_type_tag );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_START );
            if ( fake_property_at_from_end )
            {
                export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), relation_id );
                export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_ID_FRAGMENT_SOURCE_END );
            }
            else
            {
                export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), from_end_id );
            }
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_END );

            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );

            if ( fake_property_at_from_end )
            {
                export_err |= xmi_element_writer_private_fake_memberend ( this_,
                                                                          relation_id,
                                                                          relation_type,
                                                                          from_end_id,
                                                                          from_c_type,
                                                                          from_f_type,
                                                                          false /* = is_target_end */
                                                                        );
            }
        }

        /* destination */
        /* determine from type tag */
        const char* to_type_tag;
        const u8_error_t to_type_err
            = xmi_type_converter_get_xmi_to_property_of_relationship ( &((*this_).xmi_types),
                                                                       host_type,
                                                                       relation_type,
                                                                       to_c_type,
                                                                       to_f_type,
                                                                       &to_type_tag
                                                                     );
        if ( to_type_err != 0 )
        {
            /* The caller requested to write a relationship of illegal target end type */
            U8_TRACE_INFO("xmi_element_writer: request to write a relationship connecting an illegal target end type!");
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_illegal_relationship_end ( &((*this_).atom_writer),
                                                                            relation_id,
                                                                            relation_type,
                                                                            host_type,
                                                                            false /* = from_end */,
                                                                            to_c_type,
                                                                            to_f_type
                                                                          );
        }

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
        export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), to_type_tag );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_START );
        if ( fake_property_at_to_end )
        {
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), relation_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_ID_FRAGMENT_TARGET_END );
        }
        else
        {
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), to_end_id );
        }
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_END );

        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );

        if ( fake_property_at_to_end )
        {
            export_err |= xmi_element_writer_private_fake_memberend ( this_,
                                                                      relation_id,
                                                                      relation_type,
                                                                      to_end_id,
                                                                      to_c_type,
                                                                      to_f_type,
                                                                      true /* = is_target_end */
                                                                    );
        }

        /* generate extension point for use cases */
        if ( relation_type == DATA_RELATIONSHIP_TYPE_UML_EXTEND )
        {
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "extensionLocation" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_START );
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), to_end_id );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "#extensionpoint" );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_END );
            export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );
        }
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_end_relationship( xmi_element_writer_t *this_,
                                                data_classifier_type_t host_type,
                                                const data_relationship_t *relation_ptr )
{
    U8_TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    /* NULL is allowed here: dest_classifier_ptr */
    u8_error_t export_err = U8_ERROR_NONE;

    const data_relationship_type_t relation_type = data_relationship_get_main_type( relation_ptr );
    const xmi_element_info_t *relation_info;
    const u8_error_t map_err = xmi_element_info_map_get_relationship( &xmi_element_info_map_standard,
                                                                      (host_type==DATA_CLASSIFIER_TYPE_STATE),
                                                                      relation_type,
                                                                      &relation_info
                                                                    );

    if ( map_err != 0 )
    {
        /* The caller requested to write a relationship of unknown type, error was already logged at xmi_element_writer_start_relationship */
        U8_TRACE_INFO_INT("xmi_element_writer: request to write a relationship of unknown type", relation_type );
    }
    else if ( (*this_).mode == XMI_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* nesting_property;
        const u8_error_t nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_relationship( &((*this_).xmi_types),
                                                                           host_type,
                                                                           relation_type,
                                                                           &nesting_property
                                                                         );
        if ( nesting_err != 0 )
        {
            /* use a fallback */
            nesting_property = XMI_ELEMENT_PART_FALLBACK_NESTING_ELEMENT;
        }

        xml_writer_decrease_indent ( &((*this_).xml_writer) );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), nesting_property );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
    }

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_start_diagram( xmi_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    U8_TRACE_BEGIN();
    const u8_error_t export_err = U8_ERROR_NOT_YET_IMPLEMENTED;
    U8_LOG_WARNING( "xmi_element_writer_t does not export data_diagram_t" );
    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_assemble_diagram( xmi_element_writer_t *this_,
                                                const data_diagram_t *parent,
                                                const data_diagram_t *diag_ptr,
                                                const char *diagram_file_base_name )
{
    U8_TRACE_BEGIN();
    const u8_error_t export_err = U8_ERROR_NOT_YET_IMPLEMENTED;
    U8_LOG_WARNING( "xmi_element_writer_t does not export data_diagram_t" );
    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_end_diagram( xmi_element_writer_t *this_, const data_diagram_t *diag_ptr )
{
    U8_TRACE_BEGIN();
    const u8_error_t export_err = U8_ERROR_NOT_YET_IMPLEMENTED;
    U8_LOG_WARNING( "xmi_element_writer_t does not export data_diagram_t" );
    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_start_diagramelement( xmi_element_writer_t *this_,
                                                    const data_diagram_t *parent,
                                                    const data_diagramelement_t *diagramelement_ptr )
{
    U8_TRACE_BEGIN();
    const u8_error_t export_err = U8_ERROR_NOT_YET_IMPLEMENTED;
    U8_LOG_WARNING( "xmi_element_writer_t does not export data_diagramelement_t" );
    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_assemble_diagramelement( xmi_element_writer_t *this_,
                                                       const data_diagram_t *parent,
                                                       const data_diagramelement_t *diagramelement_ptr,
                                                       const data_classifier_t *occurrence,
                                                       const data_feature_t *feat_occur )
{
    U8_TRACE_BEGIN();
    /* NULL is allowed here: feat_occur */
    const u8_error_t export_err = U8_ERROR_NOT_YET_IMPLEMENTED;
    U8_LOG_WARNING( "xmi_element_writer_t does not export data_diagramelement_t" );
    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_end_diagramelement( xmi_element_writer_t *this_,
                                                  const data_diagram_t *parent,
                                                  const data_diagramelement_t *diagramelement_ptr )
{
    U8_TRACE_BEGIN();
    const u8_error_t export_err = U8_ERROR_NOT_YET_IMPLEMENTED;
    U8_LOG_WARNING( "xmi_element_writer_t does not export data_diagramelement_t" );
    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_end_main( xmi_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_UML_MODEL_END );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_write_footer( xmi_element_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_DOC_END );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_private_fake_memberend ( xmi_element_writer_t *this_,
                                                       data_id_t relationship_id,
                                                       data_relationship_type_t relationship_type,
                                                       data_id_t end_object_id,
                                                       data_classifier_type_t end_classifier_type,
                                                       data_feature_type_t end_feature_type,
                                                       bool is_target_end )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    const bool is_composition
        = ( relationship_type == DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT )  /* fallback relationship */
        ||( relationship_type == DATA_RELATIONSHIP_TYPE_UML_COMPOSITION );
    const bool is_aggregation
        = ( relationship_type == DATA_RELATIONSHIP_TYPE_UML_AGGREGATION );

    const xmi_element_info_t *classifier_info;
    u8_error_t map_err = xmi_element_info_map_get_classifier( &xmi_element_info_map_standard,
                                                              DATA_CLASSIFIER_TYPE_PACKAGE, /* if state or activity context does not matter here */
                                                              end_classifier_type,
                                                              &classifier_info
                                                            );
    if ( map_err != 0 )
    {
        U8_TRACE_INFO_INT("xmi_element_writer: request to write a member end of unknown type", end_classifier_type );
    }
    assert ( classifier_info != NULL );

    /* begin start member-end element */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_ELEMENT_OWNED_END );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

    /* write type attribute */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_START );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_NS_UML );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_TYPE_PROPERTY );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_TYPE_END );

    /* write id attribute */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
    export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), relationship_id );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                           ( is_target_end
                                           ? XMI_ELEMENT_PART_ID_FRAGMENT_TARGET_END
                                           : XMI_ELEMENT_PART_ID_FRAGMENT_SOURCE_END ) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );

    /* write association attribute */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_PROPERTY_ASSOCIATION_ATTRIBUTE );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_VALUE_START );
    export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), relationship_id );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_VALUE_END );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

    /* write aggregation attribute */
    if (( is_composition || is_aggregation )&& is_target_end )
    {
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_PROPERTY_AGGREGATION_ATTRIBUTE );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_VALUE_START );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer),
                                               is_composition
                                               ? XMI_ELEMENT_PART_PROPERTY_AGGREGATION_COMPOSITE
                                               : XMI_ELEMENT_PART_PROPERTY_AGGREGATION_SHARED
                                             );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_VALUE_END );
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
    }

    /* end start member-end element */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );
    xml_writer_increase_indent ( &((*this_).xml_writer) );

    /* start type element */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_START );
    if (( end_feature_type == DATA_FEATURE_TYPE_PROVIDED_INTERFACE )||( end_feature_type == DATA_FEATURE_TYPE_REQUIRED_INTERFACE )
        ||(( end_feature_type == DATA_FEATURE_TYPE_VOID )&&( end_classifier_type == DATA_CLASSIFIER_TYPE_INTERFACE )))
    {
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_PROPERTY_INTERFACE_ELEMENT );
    }
    else if (( end_feature_type == DATA_FEATURE_TYPE_VOID ) && xmi_element_info_is_a_class( classifier_info ) )
    {
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_PROPERTY_CLASS_ELEMENT );
    }
    else
    {
        export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_PROPERTY_TYPE_ELEMENT );
    }
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

    /* write id-ref attribute */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_START );
    export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), end_object_id );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_END );

    /* end type element */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );

    /* end member-end element */
    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_ELEMENT_OWNED_END );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_private_start_stereotype( xmi_element_writer_t *this_,
                                                        const char* profile_ns,
                                                        const utf8stringview_t profile_type,
                                                        const char* base_type,
                                                        data_id_t element_id )
{
    U8_TRACE_BEGIN();
    assert( base_type != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_START );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), profile_ns );
    export_err |= xml_writer_write_xml_enc_view ( &((*this_).xml_writer), profile_type );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_START );
    export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), element_id );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), "#" );
    export_err |= xml_writer_write_xml_enc_view ( &((*this_).xml_writer), profile_type );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_ID_END );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_START );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), base_type );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_MIDDLE );
    export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), element_id );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI2_EXT_BASE_ELEMENT_END );

    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_START_TAG_END );
    xml_writer_increase_indent ( &((*this_).xml_writer) );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t xmi_element_writer_private_end_stereotype( xmi_element_writer_t *this_,
                                                      const char* profile_ns,
                                                      const utf8stringview_t profile_type )
{
    U8_TRACE_BEGIN();
    u8_error_t export_err = U8_ERROR_NONE;

    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
    export_err |= xml_writer_write_xml_enc ( &((*this_).xml_writer), profile_ns );
    export_err |= xml_writer_write_xml_enc_view ( &((*this_).xml_writer), profile_type );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2020-2023 Andreas Warnke

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
