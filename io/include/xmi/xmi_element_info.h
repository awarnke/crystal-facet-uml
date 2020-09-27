/* File: xmi_element_info.h; Copyright and License: see below */

#ifndef XMI_ELEMENT_INFO_H
#define XMI_ELEMENT_INFO_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores static data on xmi type names, type hierarchies and property names
 */

#include "xmi_spec.h"
#include "xmi/xmi_element_is_a.h"
#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include <stdbool.h>

/*!
 *  \brief defines static data on xmi type name, type hierarchy and property names
 */
struct xmi_element_info_struct {
    int data_type_checksum;  /*!< a checksum to assert that the program is still consistent */
    xmi_spec_t specification;  /*!< a bitfield stating which spec defines this, e.g. (XMI_SPEC_UML|XMI_SPEC_SYSML) */
    const char * profile_name;  /*!< name of the profile extension-type, NULL if not applicable */
    const char * base_name;  /*!< name of the uml element type */
    xmi_element_is_a_t is_a;  /*!< states if the type is a decendant of which parent types */
    const char * property_from;  /*!< in case of a relationship, states the property-name of the source end; NULL otherwise */
    const char * property_to;  /*!< in case of a relationship, states the property-name of the target end; NULL otherwise */
    const char * additional_properties;  /*!< additional xml attributes that are fix for this type; NULL if none */
};

typedef struct xmi_element_info_struct xmi_element_info_t;

/*!
 *  \brief gets the data_type_checksum.
 *
 *  \param this_ pointer to own object attributes
 *  \return data_type_checksum
 */
static inline int xmi_element_info_get_data_type_checksum ( const xmi_element_info_t *this_ );

/*!
 *  \brief gets the specification.
 *
 *  \param this_ pointer to own object attributes
 *  \return specification
 */
static inline xmi_spec_t xmi_element_info_get_specification ( const xmi_element_info_t *this_ );

/*!
 *  \brief gets the profile_name.
 *
 *  \param this_ pointer to own object attributes
 *  \return profile_name or NULL if not applicable
 */
static inline const char * xmi_element_info_get_profile_name ( const xmi_element_info_t *this_ );

/*!
 *  \brief gets the base_name.
 *
 *  \param this_ pointer to own object attributes
 *  \return base_name
 */
static inline const char * xmi_element_info_get_base_name ( const xmi_element_info_t *this_ );

/*!
 *  \brief gets the property_from.
 *
 *  \param this_ pointer to own object attributes
 *  \return property_from or NULL if not applicable
 */
static inline const char * xmi_element_info_get_property_from ( const xmi_element_info_t *this_ );

/*!
 *  \brief gets the property_to.
 *
 *  \param this_ pointer to own object attributes
 *  \return property_to or NULL if not applicable
 */
static inline const char * xmi_element_info_get_property_to ( const xmi_element_info_t *this_ );

/*!
 *  \brief gets the additional_properties.
 *
 *  \param this_ pointer to own object attributes
 *  \return additional_properties or NULL if not applicable
 */
static inline const char * xmi_element_info_get_additional_properties ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_CLASSIFIER bits set
 */
static inline bool xmi_element_info_is_a_classifier ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a comment.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_COMMENT bits set
 */
static inline bool xmi_element_info_is_a_comment ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a behavior.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_BEHAVIOR bits set
 */
static inline bool xmi_element_info_is_a_behavior ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a named_element.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_NAMED_ELEMENT bits set
 */
static inline bool xmi_element_info_is_a_named_element ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a package.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_PACKAGE bits set
 */
static inline bool xmi_element_info_is_a_package ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a packageable_element.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_PACKAGEABLE_ELEMENT bits set
 */
static inline bool xmi_element_info_is_a_packageable_element ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a node.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_NODE bits set
 */
static inline bool xmi_element_info_is_a_node ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a activity.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_ACTIVITY bits set
 */
static inline bool xmi_element_info_is_a_activity ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a activity_node.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_ACTIVITY_NODE bits set
 */
static inline bool xmi_element_info_is_a_activity_node ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a artifact.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_ARTIFACT bits set
 */
static inline bool xmi_element_info_is_a_artifact ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a behaviored_classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER bits set
 */
static inline bool xmi_element_info_is_a_behaviored_classifier ( const xmi_element_info_t *this_ );

/*!
 *  \brief checks if the type is a class.
 *
 *  \param this_ pointer to own object attributes
 *  \return true if is_a has XMI_ELEMENT_IS_A_CLASS bits set
 */
static inline bool xmi_element_info_is_a_class ( const xmi_element_info_t *this_ );

#include "xmi_element_info.inl"

#endif  /* XMI_ELEMENT_INFO_H */


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
