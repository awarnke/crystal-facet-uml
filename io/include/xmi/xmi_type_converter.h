/* File: xmi_type_converter.h; Copyright and License: see below */

#ifndef XMI_TYPE_CONVERTER_H
#define XMI_TYPE_CONVERTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief converts crystal facet uml types to xmi (uml and sysml) types
 */

#include "xmi/xmi_spec.h"
#include "data_diagram_type.h"
#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include <stdbool.h>

/*!
 *  \brief attributes of the xmi type converter
 */
struct xmi_type_converter_struct {
    int dummy;  /*!< xmi_type_converter_t currently has not member attributes */
};

typedef struct xmi_type_converter_struct xmi_type_converter_t;

/*!
 *  \brief initializes the xmi type converter
 *
 *  \param this_ pointer to own object attributes
 */
void xmi_type_converter_init( xmi_type_converter_t *this_ );

/*!
 *  \brief destroys the xmi type converter
 *
 *  \param this_ pointer to own object attributes
 */
void xmi_type_converter_destroy( xmi_type_converter_t *this_ );

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief gets the xmi specification bitset for data_classifier_type_t types
 *
 *  \param this_ pointer to own object attributes
 *  \param c_type data_classifier_type_t to evaluate
 *  \result xmi specification bitset
 */
xmi_spec_t xmi_type_converter_get_xmi_spec_of_classifier ( xmi_type_converter_t *this_, data_classifier_type_t c_type );

/*!
 *  \brief gets a string representation of a data_classifier_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param c_type data_classifier_type_t to convert to a string
 *  \param spec control, according to which specification the xmi type shall be returned. profiles define extensions to base types in mof+uml.
 *  \result string representation of c_type, empty string if not available
 */
const char* xmi_type_converter_get_xmi_type_of_classifier ( xmi_type_converter_t *this_,
                                                            data_classifier_type_t c_type,
                                                            xmi_spec_t spec
                                                          );

/*!
 *  \brief gets a string representation of the xmi nesting-type of a data_classifier_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the nesting-type shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-type shall be determined
 *  \result string representation of the nesting-link type, empty string if not available
 */
const char* xmi_type_converter_get_xmi_nesting_property_of_classifier ( xmi_type_converter_t *this_,
                                                                        data_classifier_type_t parent_type,
                                                                        data_classifier_type_t child_type
                                                                      );

/* ================================ FEATURE ================================ */

/*!
 *  \brief gets the xmi specification bitset for data_classifier_type_t types
 *
 *  \param this_ pointer to own object attributes
 *  \param f_type data_feature_type_t to evaluate
 *  \result xmi specification bitset
 */
xmi_spec_t xmi_type_converter_get_xmi_spec_of_feature ( xmi_type_converter_t *this_, data_feature_type_t f_type );

/*!
 *  \brief gets a string representation of a data_feature_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param f_type data_feature_type_t to convert to a string
 *  \param spec control, according to which specification the xmi type shall be returned. profiles define extensions to base types in mof+uml.
 *  \result string representation of f_type, empty string if not available
 */
const char* xmi_type_converter_get_xmi_type_of_feature ( xmi_type_converter_t *this_,
                                                         data_feature_type_t f_type,
                                                         xmi_spec_t spec
                                                       );

/*!
 *  \brief gets a string representation of the xmi owning-type of a data_feature_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param f_type data_feature_type_t of the child of which the owning-type shall be determined
 *  \result string representation of the owning-link type, empty string if not available
 */
const char* xmi_type_converter_get_xmi_owning_property_of_feature ( xmi_type_converter_t *this_, data_feature_type_t f_type );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief gets the xmi specification bitset for data_relationship_type_t types
 *
 *  \param this_ pointer to own object attributes
 *  \param r_type data_relationship_type_t to evaluate
 *  \result xmi specification bitset
 */
xmi_spec_t xmi_type_converter_get_xmi_spec_of_relationship ( xmi_type_converter_t *this_, data_relationship_type_t r_type );

/*!
 *  \brief gets a string representation of a data_relationship_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param r_type data_relationship_type_t to convert to a string
 *  \param spec control, according to which specification the xmi type shall be returned. profiles define extensions to base types in mof+uml.
 *  \result string representation of r_type, empty string if not available
 */
const char* xmi_type_converter_get_xmi_type_of_relationship ( xmi_type_converter_t *this_,
                                                              data_relationship_type_t r_type,
                                                              xmi_spec_t spec
                                                            );

/*!
 *  \brief gets a string representation of the from property of a data_relationship_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param r_type data_relationship_type_t for which to define the from property
 *  \result string representation of the from property of the r_type
 */
const char* xmi_type_converter_get_xmi_from_property_of_relationship ( xmi_type_converter_t *this_,
                                                                       data_relationship_type_t r_type
                                                                     );

/*!
 *  \brief gets a string representation of the to property of a data_relationship_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param r_type data_relationship_type_t for which to define the to property
 *  \result string representation of the to property of the r_type
 */
const char* xmi_type_converter_get_xmi_to_property_of_relationship ( xmi_type_converter_t *this_,
                                                                     data_relationship_type_t r_type
                                                                   );

/*!
 *  \brief gets a string representation of an end property of a data_relationship_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param r_type data_relationship_type_t for which to define the end property
 *  \param from_end true if the source(from) end, is requested, false if the target(to) end is requested.
 *  \result string representation of the from property of the r_type
 */
const char* xmi_type_converter_private_get_xmi_end_property_of_relationship ( xmi_type_converter_t *this_,
                                                                              data_relationship_type_t r_type,
                                                                              bool from_end
                                                                            );

#endif  /* XMI_TYPE_CONVERTER_H */


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
