/* File: xmi_element_writer.inl; Copyright and License: see below */

#include "xmi/xmi_element_part.h"
#include "xmi/xmi_xml.h"

static inline io_writer_pass_t xmi_element_writer_get_mode( xmi_element_writer_t *this_ )
{
    return (*this_).mode;
}

static inline void xmi_element_writer_set_mode( xmi_element_writer_t *this_, io_writer_pass_t mode )
{
    (*this_).mode = mode;
}

static inline xml_writer_t *xmi_element_writer_get_xml_writer_ptr( xmi_element_writer_t *this_ )
{
    return &((*this_).xml_writer);
}

static inline bool xmi_element_writer_can_classifier_nest_classifier ( xmi_element_writer_t *this_,
                                                                       data_classifier_type_t parent_type,
                                                                       data_classifier_type_t child_type )
{
    const bool result = xmi_type_converter_can_nest_classifier( &((*this_).xmi_types), parent_type, child_type );
    return result;
}

static inline bool xmi_element_writer_can_classifier_nest_relationship ( xmi_element_writer_t *this_,
                                                                         data_classifier_type_t parent_type,
                                                                         data_relationship_type_t child_type )
{
    const bool result = xmi_type_converter_can_nest_relationship( &((*this_).xmi_types), parent_type, child_type );
    return result;
}

static inline int xmi_element_writer_private_fake_memberend ( xmi_element_writer_t *this_,
                                                              data_id_t relationship_id,
                                                              data_relationship_type_t relationship_type,
                                                              data_id_t classifier_id,
                                                              bool is_target_end )
{
    int export_err = 0;
    const bool is_composition 
        = ( relationship_type == DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT )  /* fallback relationship */
        ||( relationship_type == DATA_RELATIONSHIP_TYPE_UML_COMPOSITION );
    const bool is_aggregation 
        = ( relationship_type == DATA_RELATIONSHIP_TYPE_UML_AGGREGATION );
    
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
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_PROPERTY_TYPE_ELEMENT );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_ATTR_SEPARATOR );
    
    /* write id-ref attribute */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_START );
    export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), classifier_id );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_XML_ATTR_IDREF_END );

    /* end type element */
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_EMPTY_TAG_END );
    
    /* end member-end element */
    xml_writer_decrease_indent ( &((*this_).xml_writer) );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_NL );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_START );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XMI_ELEMENT_PART_ELEMENT_OWNED_END );
    export_err |= xml_writer_write_plain ( &((*this_).xml_writer), XML_WRITER_END_TAG_END );
    
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
