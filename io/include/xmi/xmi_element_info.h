/* File: xmi_element_info.h; Copyright and License: see below */

#ifndef XMI_ELEMENT_INFO_H
#define XMI_ELEMENT_INFO_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Lists static data on xmi type names, type hierarchies and property names
 */

#include "xmi_spec.h"
#include "data_classifier_type.h"
#include "data_feature_type.h"
#include "data_relationship_type.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the diagramelement functions
 */
struct xmi_element_info_struct {
    int data_type_checksum;  /*!< a checksum to assert that the program is still consistent */
    xmi_spec_t specification;  /*!< a bitfield stating which spec defines this, e.g. (XMI_SPEC_UML|XMI_SPEC_SYSML) */
    const char * profile_name;  /*!< name of the profile extension-type, NULL if not applicable */
    const char * base_name;  /*!< name of the uml element type */
    bool is_classifier;  /*!< states if the type is a decendant of uml:Classifier */
    bool is_behaviored_classifier;  /*!< states if the type is a decendant of uml:BehavioredClassifier */
    const char * property_from;  /*!< in case of a relationship, states the property-name of the source end; NULL otherwise */
    const char * property_to;  /*!< in case of a relationship, states the property-name of the target end; NULL otherwise */
};

typedef struct xmi_element_info_struct xmi_element_info_t;

#define XMI_ELEMENT_INFO_NS_UML "uml:"
#define XMI_ELEMENT_INFO_NS_SYSML "SysML:"
#define XMI_ELEMENT_INFO_NS_STDPROF "StandardProfile:"
#define XMI_ELEMENT_INFO_MAX_ENTRIES (45)
extern const xmi_element_info_t xmi_element_info_standard_list[XMI_ELEMENT_INFO_MAX_ENTRIES];

/*!
 *  \brief returns a pointer to the xmi_element_info_t struct identified by c_type.
 *
 *  \param c_type selects the xmi_element_info_t
 *  \return pointer to the selected xmi_element_info_t
 */
static inline const xmi_element_info_t * xmi_element_info_static_get_classifier ( data_classifier_type_t c_type );

/*!
 *  \brief returns a pointer to the xmi_element_info_t struct identified by f_type.
 *
 *  \param f_type selects the xmi_element_info_t
 *  \return pointer to the selected xmi_element_info_t
 */
static inline const xmi_element_info_t * xmi_element_info_static_get_feature ( data_feature_type_t f_type );

/*!
 *  \brief returns a pointer to the xmi_element_info_t struct identified by r_type.
 *
 *  \param r_type selects the xmi_element_info_t
 *  \return pointer to the selected xmi_element_info_t
 */
static inline const xmi_element_info_t * xmi_element_info_static_get_relationship ( data_relationship_type_t r_type );

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
