/* File: xmi_diagram_info_map.c; Copyright and License: see below */

#include "xmi/xmi_diagram_info_map.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

const xmi_diagram_info_map_t xmi_diagram_info_map_standard
= {
    /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF Annex B.3 */
    /* spec-ref: SysML_1.6_formal-19-11-01.pdf Annex B.2 */

    /* ================================ DIAGRAM ================================ */

    [XMI_DIAGRAM_INFO_MAP_INDEX_LIST] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_LIST,
        .specification            = 0,
        .profile_name             = "CFUListDiagram",
        .base_name                = "UMLDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_BOX] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_BOX_DIAGRAM,
        .specification            = 0,
        .profile_name             = "CFUBoxDiagram",
        .base_name                = "UMLDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_BLOCK_DEFINITION] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = "SysMLBlockDefinitionDiagram",
        .base_name                = "UMLClassDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_INTERNAL_BLOCK] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = "SysMLInternalBlockDiagram",
        .base_name                = "UMLCompositeStructureDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_PARAMETRIC] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = "SysMLParametricDiagram",
        .base_name                = "UMLCompositeStructureDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_REQUIREMENTS] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = "SysMLRequirementDiagram",
        .base_name                = "UMLClassDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_USE_CASE] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "UMLUseCaseDiagram",  /* SysMLUseCaseDiagram */
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_ACTIVITY] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLActivityDiagram",  /* SysMLActivityDiagram */
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_STATE_MACHINE] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "UMLStateMachineDiagram",  /* SysMLStateMachineDiagram */
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_SEQUENCE] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "UMLInteractionDiagram/sequence",  /* SysMLInteractionDiagram/sequence */
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_COMMUNICATION] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLInteractionDiagram/communication",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_TIMING] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLInteractionDiagram/timing",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_INTERACTION_OVERVIEW] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLInteractionDiagram/overview",
    },
    /* UMLInteractionDiagram/table */
    [XMI_DIAGRAM_INFO_MAP_INDEX_DEPLOYMENT] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLDeploymentDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_COMPONENT] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLComponentDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_COMPOSITE_STRUCTURE] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLCompositeStructureDiagram",
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_PACKAGE] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "UMLPackageDiagram",  /* SysMLPackageDiagram */
    },
    [XMI_DIAGRAM_INFO_MAP_INDEX_CLASS] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLClassDiagram",
    },
    /* UMLObjectDiagram */
    [XMI_DIAGRAM_INFO_MAP_INDEX_PROFILE] = {
        .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "UMLProfileDiagram",
    },
};

const xmi_diagram_info_t xmi_diagram_info_map_unknown_type =
{
    .data_type_checksum       = (int) DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,
    .specification            = XMI_SPEC_UML,
    .profile_name             = NULL,
    .base_name                = "UMLDiagram",
};


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
