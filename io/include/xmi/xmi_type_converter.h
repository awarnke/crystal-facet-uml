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

/* NOTE: When searching in the internet for xmi samples, use the following 12 characters: filetype:xmi */

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
 *  \return xmi specification bitset
 */
xmi_spec_t xmi_type_converter_get_xmi_spec_of_classifier ( xmi_type_converter_t *this_, data_classifier_type_t c_type );

/*!
 *  \brief gets a string representation of a data_classifier_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type for some classifiers, the result depends on the parent type, e.g. state or activity
 *  \param c_type data_classifier_type_t to convert to a string
 *  \param spec control, according to which specification the xmi type shall be returned. profiles define extensions to base types in mof+uml.
 *  \return string representation of c_type, empty string if not available
 */
const char* xmi_type_converter_get_xmi_type_of_classifier ( xmi_type_converter_t *this_,
                                                            data_classifier_type_t parent_type,
                                                            data_classifier_type_t c_type,
                                                            xmi_spec_t spec
                                                          );

/*!
 *  \brief gets a string representation of a classifiers nesting-type of a child classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the nesting-type shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-type shall be determined
 *  \param out_xmi_name representation of the nesting-link type, empty string if not available (result != 0)
 *  \return 0 in case of success, -1 in case of error.
 */
int xmi_type_converter_get_xmi_nesting_property_of_classifier ( xmi_type_converter_t *this_,
                                                                data_classifier_type_t parent_type,
                                                                data_classifier_type_t child_type,
                                                                char const * *out_xmi_name
                                                              );
/*!
 *  \brief checks if a parent classifier may nest a child classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the nesting-ability shall be determined
 *  \param child_type data_classifier_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting is allowed
 */
static inline bool xmi_type_converter_can_nest_classifier ( xmi_type_converter_t *this_,
                                                            data_classifier_type_t parent_type,
                                                            data_classifier_type_t child_type
                                                          );

/*!
 *  \brief gets a string representation of a classifiers owning-type of a feature
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the owning-type shall be determined
 *  \param feature_type data_feature_type_t of the child of which the owning-type shall be determined
 *  \param out_xmi_name representation of the owning-link type, even in case of a type mismatch a string is provided
 *  \return 0 in case of success, -1 in case of type mismatch between parent and owned-child
 */
int xmi_type_converter_get_xmi_owning_property_of_feature ( xmi_type_converter_t *this_,
                                                            data_classifier_type_t parent_type,
                                                            data_feature_type_t feature_type,
                                                            char const * *out_xmi_name
                                                          );

/*!
 *  \brief gets a string representation of a classifiers nesting-type of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param hosting_type data_classifier_type_t of the host of which the nesting-type shall be determined
 *  \param child_type data_relationship_type_t of the nested child of which the nesting-type shall be determined
 *  \param out_xmi_name representation of the nesting-link type, empty string if not available (result != 0)
 *  \return 0 in case of success
 */
int xmi_type_converter_get_xmi_nesting_property_of_relationship ( xmi_type_converter_t *this_,
                                                                  data_classifier_type_t hosting_type,
                                                                  data_relationship_type_t child_type,
                                                                  char const * *out_xmi_name
                                                                );

/*!
 *  \brief checks if a parent classifier may nest a child relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param hosting_type data_classifier_type_t of the host of which the nesting-ability shall be determined
 *  \param child_type data_relationship_type_t of the nested child of which the nesting-ability shall be determined
 *  \return true if nesting or any relationship is allowed
 */
static inline bool xmi_type_converter_can_nest_relationship ( xmi_type_converter_t *this_,
                                                              data_classifier_type_t hosting_type,
                                                              data_relationship_type_t child_type
                                                            );

/* ================================ FEATURE ================================ */

/*!
 *  \brief gets the xmi specification bitset for data_classifier_type_t types
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_type data_feature_type_t to evaluate
 *  \return xmi specification bitset
 */
xmi_spec_t xmi_type_converter_get_xmi_spec_of_feature ( xmi_type_converter_t *this_, data_feature_type_t feature_type );

/*!
 *  \brief gets a string representation of a data_feature_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type data_classifier_type_t of the parent of which the owning-type shall be determined
 *  \param feature_type data_feature_type_t to convert to a string
 *  \param spec control, according to which specification the xmi type shall be returned. profiles define extensions to base types in mof+uml.
 *  \return string representation of feature_type, empty string if not available
 */
const char* xmi_type_converter_get_xmi_type_of_feature ( xmi_type_converter_t *this_,
                                                         data_classifier_type_t parent_type,
                                                         data_feature_type_t feature_type,
                                                         xmi_spec_t spec
                                                       );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief gets the xmi specification bitset for data_relationship_type_t types
 *
 *  \param this_ pointer to own object attributes
 *  \param r_type data_relationship_type_t to evaluate
 *  \return xmi specification bitset
 */
xmi_spec_t xmi_type_converter_get_xmi_spec_of_relationship ( xmi_type_converter_t *this_, data_relationship_type_t r_type );

/*!
 *  \brief gets a string representation of a data_relationship_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param hosting_type data_classifier_type_t of the host of which the nesting-type shall be determined
 *  \param r_type data_relationship_type_t to convert to a string
 *  \param spec control, according to which specification the xmi type shall be returned. profiles define extensions to base types in mof+uml.
 *  \return string representation of r_type, empty string if not available
 */
const char* xmi_type_converter_get_xmi_type_of_relationship ( xmi_type_converter_t *this_,
                                                              data_classifier_type_t hosting_type,
                                                              data_relationship_type_t r_type,
                                                              xmi_spec_t spec
                                                            );

/*!
 *  \brief gets a string representation of the from property of a data_relationship_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param hosting_type data_classifier_type_t of the host
 *  \param rel_type data_relationship_type_t for which to determine the from property
 *  \param from_classifier_type the type of classifier at relationship-end
 *  \param from_feature_type the type of feature at relationship-end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \param out_xmi_name string representation of the from property of the rel_type, empty string if not available
 *  \return 0 in case of success, -1 in case of error. Even in case of error, a non-empty out_xmi_name may be returned.
 */
static inline int xmi_type_converter_get_xmi_from_property_of_relationship ( xmi_type_converter_t *this_,
                                                                             data_classifier_type_t hosting_type,
                                                                             data_relationship_type_t rel_type,
                                                                             data_classifier_type_t from_classifier_type,
                                                                             data_feature_type_t from_feature_type,
                                                                             char const * *out_xmi_name
                                                                           );

/*!
 *  \brief gets a string representation of the to property of a data_relationship_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param hosting_type data_classifier_type_t of the host
 *  \param rel_type data_relationship_type_t for which to determine the to property
 *  \param to_classifier_type the type of classifier at relationship-end
 *  \param to_feature_type the type of feature at relationship-end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \param out_xmi_name string representation of the to property of the rel_type, empty string if not available
 *  \return 0 in case of success, -1 in case of error. Even in case of error, a non-empty out_xmi_name may be returned.
 */
static inline int xmi_type_converter_get_xmi_to_property_of_relationship ( xmi_type_converter_t *this_,
                                                                           data_classifier_type_t hosting_type,
                                                                           data_relationship_type_t rel_type,
                                                                           data_classifier_type_t to_classifier_type,
                                                                           data_feature_type_t to_feature_type,
                                                                           char const * *out_xmi_name
                                                                         );

/*!
 *  \brief gets a string representation of an end property of a data_relationship_type_t
 *
 *  \param this_ pointer to own object attributes
 *  \param hosting_type data_classifier_type_t of the host
 *  \param rel_type data_relationship_type_t of the nested relationship
 *  \param from_end true if the source(from) end, is requested, false if the target(to) end is requested.
 *  \param end_classifier_type the type of classifier at relationship-end
 *  \param end_feature_type the type of feature at relationship-end; DATA_FEATURE_TYPE_VOID if no feature specified
 *  \param out_xmi_name string representation of the end property of the rel_type, empty string if not available
 *  \return 0 in case of success, -1 in case of error. Even in case of error, a non-empty out_xmi_name may be returned.
 */
int xmi_type_converter_private_get_xmi_end_property_of_relationship ( xmi_type_converter_t *this_,
                                                                      data_classifier_type_t hosting_type,
                                                                      data_relationship_type_t rel_type,
                                                                      bool from_end,
                                                                      data_classifier_type_t end_classifier_type,
                                                                      data_feature_type_t end_feature_type,
                                                                      char const * *out_xmi_name
                                                                    );

#include "xmi_type_converter.inl"

#endif  /* XMI_TYPE_CONVERTER_H */


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
