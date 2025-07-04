/* File: data_diagram_type.h; Copyright and License: see below */

#ifndef DATA_DIAGRAM_TYPE_H
#define DATA_DIAGRAM_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines types of diagrams
 */

#include <stdbool.h>

/*!
 *  \brief enumeration of all diagram types
 */
enum data_diagram_type_enum {
    DATA_DIAGRAM_TYPE_VOID = -1,  /* ! < invalid diagram */
    DATA_DIAGRAM_TYPE_LIST = 0,
    DATA_DIAGRAM_TYPE_BOX_DIAGRAM = 1,
    /*DATA_DIAGRAM_TYPE_TABLE = 2,*/  /* Table of tagged value, to show e.g. resource consumption */
    DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM = 4,
    DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM = 5,
    DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM = 7,
    DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM = 9,
    DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM = 10,
    DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM = 11,
    DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM = 12,
    DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM = 13,
    DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM = 14,
    /* DATA_DIAGRAM_TYPE_DEPRECATED_INTERACTION_OVERVIEW_DIAGRAM = 15,*/  /* deprecated since version 1.25.0 */
    DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM = 16,
    DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM = 17,  /* new since version 1.25.0 */
    DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM = 30,
    DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM = 31,
    DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM = 32,
    DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM = 33,
    DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM = 34,
    /*DATA_DIAGRAM_TYPE_UML_OBJECT_DIAGRAM = 35,*/ /* - obsolete in UML 2.5, use a class diagram instead */
    DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM = 39,
};

typedef enum data_diagram_type_enum data_diagram_type_t;

#define DATA_DIAGRAM_TYPE_COUNT (19)

/*!
 *  \brief a modulo for a hash function for unique hashing
 */
#define DATA_DIAGRAM_TYPE_HASH_MOD (40)

/*!
 *  \brief lists all values of data_diagram_type_t, excluding DATA_DIAGRAM_TYPE_VOID
 */
extern data_diagram_type_t const DATA_DIAGRAM_TYPE_ARRAY [DATA_DIAGRAM_TYPE_COUNT];

/*!
 *  \brief checks if the diagram type is an interaction diagram
 *
 *  \param this_ enumeration value
 *  \return true if sequence, timing, communication or interaction overview
 *          false otherwise
 */
static inline bool data_diagram_type_is_interaction( data_diagram_type_t this_ );

/*!
 *  \brief returns a short name for the diagram type
 *
 *  \param this_ enumeration value
 *  \return name of the type, "" if unknown or void type
 */
const char * data_diagram_type_get_name( data_diagram_type_t this_ );

#include "entity/data_diagram_type.inl"

#endif  /* DATA_DIAGRAM_TYPE_H */


/*
Copyright 2016-2025 Andreas Warnke

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
