/* File: data_diagram_type.c; Copyright and License: see below */

#include "entity/data_diagram_type.h"
#include <stddef.h>
#include <assert.h>

const data_diagram_type_t DATA_DIAGRAM_TYPE_ARRAY [DATA_DIAGRAM_TYPE_COUNT]
    = {
        DATA_DIAGRAM_TYPE_LIST,
        DATA_DIAGRAM_TYPE_BOX_DIAGRAM,
        DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM,
        DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM,
        DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM,
        DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM,
        /* DATA_DIAGRAM_TYPE_DEPRECATED_INTERACTION_OVERVIEW_DIAGRAM, */
        DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
        DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
        DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM
    };

static const char *const DATA_DIAGRAM_TYPE_NAME_ARRAY [DATA_DIAGRAM_TYPE_HASH_MOD]
    = {
        [ DATA_DIAGRAM_TYPE_LIST ] = "List",
        [ DATA_DIAGRAM_TYPE_BOX_DIAGRAM ] = "Boxes",
        [2] = "",
        [3] = "",
        [ DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM ] = "Block Definition Diagram",
        [ DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM ] = "Internal Block Diagram",
        [6] = "",
        [ DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM ] = "Parametric Diagram",
        [8] = "",
        [ DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM ] = "Requirements Diagram",
        [ DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM ] = "Use Case Diagram",
        [ DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM ] = "Activity Diagram",
        [ DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM ] = "State Diagram",
        [ DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM ] = "Sequence Diagram",
        [ DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM ] = "Communication Diagram",
        [15] = "",
        [ DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM ] = "Timing Diagram",
        [ DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM ] = "Interation Overview Diagram",
        [18] = "",
        [19] = "",
        [20] = "",
        [21] = "",
        [22] = "",
        [23] = "",
        [24] = "",
        [25] = "",
        [26] = "",
        [27] = "",
        [28] = "",
        [29] = "",
        [ DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM ] = "Deployment Diagram",
        [ DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM ] = "Component Diagram",
        [ DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM ] = "Composite Structure Diagram",
        [ DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM ] = "Package Diagram",
        [ DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM ] = "Class Diagram",
        [35] = "",
        [36] = "",
        [37] = "",
        [38] = "",
        [ DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM ] = "Profile Diagram",
    };

const char * data_diagram_type_get_name( data_diagram_type_t this_ )
{
    const char * result
        = ( this_ == DATA_DIAGRAM_TYPE_VOID ) ? "" : DATA_DIAGRAM_TYPE_NAME_ARRAY[ this_ % DATA_DIAGRAM_TYPE_HASH_MOD ];
    assert( result != NULL );
    return ( result == NULL ) ? "" : result;
}


/*
Copyright 2021-2025 Andreas Warnke

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
