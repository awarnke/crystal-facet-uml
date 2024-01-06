/* File: xmi_diagram_info.h; Copyright and License: see below */

#ifndef XMI_DIAGRAM_INFO_H
#define XMI_DIAGRAM_INFO_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores static data on xmi diagram types
 */

#include "xmi_spec.h"
#include "data_diagram_type.h"
#include <stdbool.h>

/*!
 *  \brief defines static data on xmi type name, type hierarchy and property names
 */
struct xmi_diagram_info_struct {
    int data_type_checksum;  /*!< a checksum to assert that the program is still consistent */
    xmi_spec_t specification;  /*!< a bitfield stating which spec defines this, e.g. (XMI_SPEC_UML|XMI_SPEC_SYSML) */
    const char * profile_name;  /*!< name of the profile extension-type, NULL if not applicable */
    const char * base_name;  /*!< name of the uml diagram type */
};

typedef struct xmi_diagram_info_struct xmi_diagram_info_t;

/*!
 *  \brief gets the data_type_checksum.
 *
 *  \param this_ pointer to own object attributes
 *  \return data_type_checksum
 */
static inline int xmi_diagram_info_get_data_type_checksum ( const xmi_diagram_info_t *this_ );

/*!
 *  \brief gets the specification.
 *
 *  \param this_ pointer to own object attributes
 *  \return specification
 */
static inline xmi_spec_t xmi_diagram_info_get_specification ( const xmi_diagram_info_t *this_ );

/*!
 *  \brief gets the profile_name.
 *
 *  \param this_ pointer to own object attributes
 *  \return profile_name or NULL if not applicable
 */
static inline const char * xmi_diagram_info_get_profile_name ( const xmi_diagram_info_t *this_ );

/*!
 *  \brief gets the base_name.
 *
 *  \param this_ pointer to own object attributes
 *  \return base_name
 */
static inline const char * xmi_diagram_info_get_base_name ( const xmi_diagram_info_t *this_ );

/*!
 *  \brief gets the profile_name if not NULL, the base_name otherwise.
 *
 *  \param this_ pointer to own object attributes
 *  \return profile_name or base_name
 */
static inline const char * xmi_diagram_info_get_name ( const xmi_diagram_info_t *this_ );

#include "xmi_diagram_info.inl"

#endif  /* XMI_DIAGRAM_INFO_H */


/*
Copyright 2022-2024 Andreas Warnke

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
