/* File: xmi_element_info_map.c; Copyright and License: see below */

#include "xmi/xmi_element_info_map.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

const xmi_element_info_map_t xmi_element_info_map_standard
= {
    [XMI_ELEMENT_INFO_MAP_INDEX_BLOCK] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Blocks */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_BLOCK,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = XMI_ELEMENT_INFO_NS_SYSML "Block",
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_CONSTRAINT_PROPERTY] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: ConstraintBlocks */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = XMI_ELEMENT_INFO_NS_SYSML "ConstraintBlock",
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_REQUIREMENT] = {
            /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_REQUIREMENT,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = XMI_ELEMENT_INFO_NS_SYSML "AbstractRequirement", /* the validation tool seems to not allow Requirement */
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
    },

    [XMI_ELEMENT_INFO_MAP_INDEX_ACTOR] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_ACTOR,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Actor",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_USE_CASE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_USE_CASE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "UseCase",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_SYSTEM_BOUNDARY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Node", /* TODO: check type */
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ACTIVITY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_ACTIVITY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Activity",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIOR),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_STATE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_STATE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "State",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DIAGRAM_REFERENCE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Activity",  /* TODO: check type */
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIOR),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_NODE,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Node",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_COMPONENT,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Component",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_INTERFACE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_INTERFACE,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Interface",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_PACKAGE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Packages */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_PACKAGE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Package",  /* TODO: spec ok but is ignored at import to EA, maybe a packageImport-tag needed */
        .is_a                     = (XMI_ELEMENT_IS_A_PACKAGE),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_CLASS] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_CLASS,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ARTIFACT] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_ARTIFACT,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Artifact",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_COMMENT] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_COMMENT,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Comment",  /* TODO: spec ok but is ignored at import to EA */
        .is_a                     = (XMI_ELEMENT_IS_A_COMMENT),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_INTERRUPTABLE_REGION] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "InterruptibleActivityRegion",
        .is_a                     = (XMI_ELEMENT_IS_A_NAMED_ELEMENT),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_INITIAL_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "InitialNode",  /* TODO: spec ok but is ignored at import to EA */
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_FINAL_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* XMI_ELEMENT_INFO_NS_UML "FinalNode"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
        /* XMI_ELEMENT_INFO_NS_UML "FinalState"; */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "ActivityFinalNode",  /* TODO: spec ok but is ignored at import to EA */
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_FORK_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_FORK_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "ForkNode",  /* TODO: spec ok but is ignored at import to EA */
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_JOIN_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "JoinNode",  /* TODO: spec ok but is ignored at import to EA */
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_DECISION_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* XMI_ELEMENT_INFO_NS_UML "DecisionNode"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
        /* XMI_ELEMENT_INFO_NS_UML "ConditionalNode"; */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "DecisionNode",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_SHALLOW_HISTORY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Pseudostate",  /* TODO: PseudostateKind = shallowHistory shall be set */
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_DEEP_HISTORY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Pseudostate",  /* TODO: PseudostateKind = deepHistory shall be set */
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    /*
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_PARTITION] = {
        / * spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities * /
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_PARTITION,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "ActivityPartition",
        .is_a                     = (XMI_ELEMENT_IS_A_NAMED_ELEMENT),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    */
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_EVENT] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
        /* XMI_ELEMENT_INFO_NS_UML "Reception"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
        /* XMI_ELEMENT_INFO_NS_UML "AcceptEventAction"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
        /* result = XMI_ELEMENT_INFO_NS_UML "SignalEvent"; */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "SignalEvent",
        .is_a                     = (XMI_ELEMENT_IS_A_EVENT),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_TIME_EVENT] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "TimeEvent",
        .is_a                     = (XMI_ELEMENT_IS_A_EVENT),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_SEND_SIGNAL] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
        /* result = XMI_ELEMENT_INFO_NS_UML "Signal"; */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Signal",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
    },
};


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
