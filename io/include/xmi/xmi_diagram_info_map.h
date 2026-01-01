/* File: xmi_diagram_info_map.h; Copyright and License: see below */

#ifndef XMI_DIAGRAM_INFO_MAP_H
#define XMI_DIAGRAM_INFO_MAP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Lists static data on xmi type names, type hierarchies and property names
 */

#include "xmi/xmi_spec.h"
#include "xmi/xmi_diagram_info.h"
#include "entity/data_classifier_type.h"
#include "entity/data_feature_type.h"
#include "entity/data_relationship_type.h"
#include "u8/u8_error.h"
#include <stdbool.h>

/*!
 *  \brief defines the array indices of uml elements
 */
enum xmi_diagram_info_map_index_enum {
    /* ================================ DIAGRAM ================================ */
    XMI_DIAGRAM_INFO_MAP_INDEX_LIST,
    XMI_DIAGRAM_INFO_MAP_INDEX_BOX,
    XMI_DIAGRAM_INFO_MAP_INDEX_BLOCK_DEFINITION,
    XMI_DIAGRAM_INFO_MAP_INDEX_INTERNAL_BLOCK,
    XMI_DIAGRAM_INFO_MAP_INDEX_PARAMETRIC,
    XMI_DIAGRAM_INFO_MAP_INDEX_REQUIREMENTS,
    XMI_DIAGRAM_INFO_MAP_INDEX_USE_CASE,
    XMI_DIAGRAM_INFO_MAP_INDEX_ACTIVITY,
    XMI_DIAGRAM_INFO_MAP_INDEX_STATE_MACHINE,
    XMI_DIAGRAM_INFO_MAP_INDEX_SEQUENCE,
    XMI_DIAGRAM_INFO_MAP_INDEX_COMMUNICATION,
    XMI_DIAGRAM_INFO_MAP_INDEX_TIMING,
    XMI_DIAGRAM_INFO_MAP_INDEX_INTERACTION_OVERVIEW,
    XMI_DIAGRAM_INFO_MAP_INDEX_DEPLOYMENT,
    XMI_DIAGRAM_INFO_MAP_INDEX_COMPONENT,
    XMI_DIAGRAM_INFO_MAP_INDEX_COMPOSITE_STRUCTURE,
    XMI_DIAGRAM_INFO_MAP_INDEX_PACKAGE,
    XMI_DIAGRAM_INFO_MAP_INDEX_CLASS,
    XMI_DIAGRAM_INFO_MAP_INDEX_PROFILE,
};

typedef enum xmi_diagram_info_map_index_enum xmi_diagram_info_map_index_t;

#define XMI_DIAGRAM_INFO_MAP_INDEX_MAX (19)

typedef xmi_diagram_info_t xmi_diagram_info_map_t[XMI_DIAGRAM_INFO_MAP_INDEX_MAX];

extern const xmi_diagram_info_map_t xmi_diagram_info_map_standard;

/*!
 *  \brief returns a pointer to the xmi_diagram_info_t struct identified by diagram_type.
 *
 *  \param this_ pointer to own object attributes; use e.g. the global &xmi_diagram_info_map_standard
 *  \param diagram_type selects the xmi_diagram_info_t
 *  \param[out] out_diagram_info pointer to the selected xmi_diagram_info_t
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_NOT_FOUND if no valid mapping found
 */
static inline u8_error_t xmi_diagram_info_map_get_diagram ( const xmi_diagram_info_map_t *this_,
                                                            data_diagram_type_t diagram_type,
                                                            const xmi_diagram_info_t **out_diagram_info
                                                          );

#include "xmi_diagram_info_map.inl"

#endif  /* XMI_DIAGRAM_INFO_MAP_H */


/*
Copyright 2022-2026 Andreas Warnke

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
