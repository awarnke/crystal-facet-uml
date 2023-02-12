/* File: xmi_element_info_map.c; Copyright and License: see below */

#include "xmi/xmi_element_info_map.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

const xmi_element_info_map_t xmi_element_info_map_standard
= {
    /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF */
    /* note: if a property or association end starts with a slash(/),  */
    /*       it is a derived value and shall not be specified in xmi */
    /* note: google search pattern for examples may help: filetype:xmi */

    /* ================================ CLASSIFIER ================================ */

    [XMI_ELEMENT_INFO_MAP_INDEX_BLOCK] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Blocks */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_BLOCK,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = "Block",
        .base_name                = "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_CONSTRAINT_BLOCK] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: ConstraintBlocks */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = "ConstraintBlock",
        .base_name                = "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_REQUIREMENT] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_REQUIREMENT,
        .specification            = XMI_SPEC_SYSML,
        .profile_name             = "Requirement",
        .base_name                = "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        //.base_name                = "NamedElement",
        //.is_a                     = (XMI_ELEMENT_IS_A_NAMED_ELEMENT),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ACTOR] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_ACTOR,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Actor",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_USE_CASE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_USE_CASE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "UseCase",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIORED_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_SUBSYSTEM] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_SUBSYSTEM,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Component",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ACTIVITY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.1 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_ACTIVITY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Activity",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIOR),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_STRUCTURED_ACTIVITY_NODE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 16.14.55 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_ACTIVITY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "StructuredActivityNode",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIOR|XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_STATEMACHINE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.10 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_STATE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "StateMachine",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIOR),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_STATE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.9 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_STATE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "State",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_INTERACTION_USE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.16 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "InteractionUse",
        .is_a                     = (XMI_ELEMENT_IS_A_INTERACTION_FRAGMENT),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 19.5.10 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_NODE,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Node",
        .is_a                     = (XMI_ELEMENT_IS_A_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_COMPONENT] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.6 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_COMPONENT,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "Component",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_INTERFACE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 10.5.5 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_INTERFACE,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "Interface",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_PACKAGE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Packages */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.4.5 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_PACKAGE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Package",
        .is_a                     = (XMI_ELEMENT_IS_A_PACKAGE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_CLASS] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_CLASS,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "Class",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ARTIFACT] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 19.5.1 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_ARTIFACT,
        .specification            = XMI_SPEC_UML,
        .profile_name             = NULL,
        .base_name                = "Artifact",
        .is_a                     = (XMI_ELEMENT_IS_A_ARTIFACT),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_COMMENT] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_COMMENT,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Comment",
        .is_a                     = (XMI_ELEMENT_IS_A_COMMENT),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_STEREOTYPE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 12.3.3.1.3 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_STEREOTYPE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Stereotype",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = "profile=\"LocalProfile\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_INTERRUPTABLE_REGION] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.19 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "InterruptibleActivityRegion",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_GROUP),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_INITIAL_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.18 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "InitialNode",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_INITIAL_NODE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.6 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Pseudostate",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
        /*.additional_properties    = "kind=\"initial\"",  -- the default value must not be written */
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_FINAL_NODE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.3 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "ActivityFinalNode",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_FINAL_NODE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.2 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "FinalState",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_FORK_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.17 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_FORK_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "ForkNode",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_JOIN_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.17 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "JoinNode",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACTIVITY_DECISION_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* "DecisionNode"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
        /* "ConditionalNode"; */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 15.7.12 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "DecisionNode",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_STATE_DECISION_NODE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.6 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Pseudostate",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = "kind=\"choice\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_SHALLOW_HISTORY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.6 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Pseudostate",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = "kind=\"shallowHistory\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_DEEP_HISTORY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StateMachines */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.6 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Pseudostate",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = "kind=\"deepHistory\"",
    },
    /*
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_PARTITION] = {
        / * spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities * /
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_PARTITION,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "ActivityPartition",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_GROUP),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    */
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_EVENT] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 16.14.2 */
        /* spec-ref: https://www.omg.org/spec/SysML/1.6/PDF chapter 11.2 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "AcceptEventAction",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
        /* TODO: property trigger is required */

        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 13.4.9 */
        /*.base_name                = "SignalEvent", -- does not work: signal is a mandatory relation and is not available */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 13.4.1 */
        /*.base_name                = "AnyReceiveEvent", */
        /*.is_a                     = (XMI_ELEMENT_IS_A_EVENT), */
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_ACCEPT_TIME_EVENT] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 16.14.2 */
        /* spec-ref: https://www.omg.org/spec/SysML/1.6/PDF chapter 11.2 */
        /* A Time Event is a AcceptEventAction referencing a Trigger referencint a TimeEvent */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "AcceptEventAction",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 13.4.10 */
        /*.base_name                = "TimeEvent", */
        /*.is_a                     = (XMI_ELEMENT_IS_A_EVENT), */
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DYN_SEND_SIGNAL] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 16.14.50 */
        /* spec-ref: https://www.omg.org/spec/SysML/1.6/PDF chapter 11.2 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "SendSignalAction",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
        /* TODO: properties signal and target are required */

        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 10.5.9 */
        /* "Signal"; */
        /*.is_a                     = (XMI_ELEMENT_IS_A_CLASSIFIER), */
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_INTERACTION] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.2 */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_INTERACTION,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Interaction",
        .is_a                     = (XMI_ELEMENT_IS_A_BEHAVIOR),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },

    /* ================================ FEATURE ================================ */

    [XMI_ELEMENT_INFO_MAP_INDEX_PROPERTY] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.9.17 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_PROPERTY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Property",
        .is_a                     = (XMI_ELEMENT_IS_A_PROPERTY),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = "aggregation=\"composite\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_OPERATION] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.9.11 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_OPERATION,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Operation",
        .is_a                     = (XMI_ELEMENT_IS_A_FEATURE),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_PORT] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.14 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_PORT,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Port",
        .is_a                     = (XMI_ELEMENT_IS_A_PROPERTY),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = "aggregation=\"composite\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_INPUT_PIN] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 16.14.24 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_IN_PORT_PIN,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "InputPin",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE | XMI_ELEMENT_IS_A_TYPED_ELEMENT),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_OUTPUT_PIN] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 16.14.32 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_OUT_PORT_PIN,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "OutputPin",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_NODE | XMI_ELEMENT_IS_A_TYPED_ELEMENT),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_STATE_ENTRY] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.6 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_ENTRY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Pseudostate",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = "kind=\"entryPoint\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_STATE_EXIT] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.6 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_EXIT,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Pseudostate",
        .is_a                     = (XMI_ELEMENT_IS_A_VERTEX),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = "kind=\"exitPoint\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_LIFELINE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.17 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_LIFELINE,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = "Lifeline",
        .is_a                     = (XMI_ELEMENT_IS_A_NAMED_ELEMENT),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_PROVIDED_INTERFACE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 10.5.5 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_PROVIDED_INTERFACE,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Interface",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_REQUIRED_INTERFACE] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 10.5.5 */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_REQUIRED_INTERFACE,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Interface",
        .is_a                     = (XMI_ELEMENT_IS_A_CLASSIFIER),
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_TAGGED_VALUE] = {
        /* spec-ref: https://www.omg.org/spec/MOF/2.5.1/PDF annex A */
        .data_type_checksum       = (int) DATA_FEATURE_TYPE_TAGGED_VALUE,
        .specification            = (XMI_SPEC_MOF),
        .profile_name             = NULL,
        .base_name                = "Tag",
        .is_a                     = XMI_ELEMENT_IS_A_TAGGED_VALUE,
        .property_from            = NULL,
        .property_to              = NULL,
        .additional_properties    = NULL,
    },

    /* ================================ RELATIONSHIP ================================ */

    [XMI_ELEMENT_INFO_MAP_INDEX_DEPENDENCY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.8.4 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Dependency",
        .is_a                     = (XMI_ELEMENT_IS_A_DEPENDENCY),
        .property_from            = "client",
        .property_to              = "supplier",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ASSOCIATION] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.8.1 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Association",
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_AGGREGATION] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.5 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_AGGREGATION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Association",
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_COMPOSITION] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.5 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_COMPOSITION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Association",
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_GENERALIZATION] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classification */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Generalization",
        .is_a                     = (XMI_ELEMENT_IS_A_DIRECTED_RELATIONSHIP),
        .property_from            = "specific",
        .property_to              = "general",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_REALIZATION] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
        /* "InterfaceRealization"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
        /* "Realization"; */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_REALIZATION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Realization",
        .is_a                     = (XMI_ELEMENT_IS_A_ABSTRACTION),
        .property_from            = "client",
        .property_to              = "supplier",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ASYNC_CALL] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Interactions */
        /* "Message"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
        /* "SignalEvent"; */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.18, 17.12.22 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Message",
        .is_a                     = (XMI_ELEMENT_IS_A_MESSAGE),
        .property_from            = "sendEvent",
        .property_to              = "receiveEvent",
        .additional_properties    = "messageSort=\"asynchSignal\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_SYNC_CALL] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.18, 17.12.22 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Message",
        .is_a                     = (XMI_ELEMENT_IS_A_MESSAGE),
        .property_from            = "sendEvent",
        .property_to              = "receiveEvent",
        /*.additional_properties    = "messageSort=\"synchCall\"",*/  /* this is the default value */
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_RETURN_CALL] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.18, 17.12.22 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Message",
        .is_a                     = (XMI_ELEMENT_IS_A_MESSAGE),
        .property_from            = "sendEvent",
        .property_to              = "receiveEvent",
        .additional_properties    = "messageSort=\"reply\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_COMMUNICATION_PATH] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "CommunicationPath",
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_CONTROL_FLOW] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* XMI_TYPE_CONVERTER_NS_UML "ControlFlow"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
        /* XMI_TYPE_CONVERTER_NS_UML "SendSignalAction"; */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "ControlFlow",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_EDGE),
        .property_from            = "source",
        .property_to              = "target",
        .additional_properties    = "weight=\"1\" guard=\"true\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_OBJECT_FLOW] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* XMI_TYPE_CONVERTER_NS_UML "ObjectFlow"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
        /* XMI_TYPE_CONVERTER_NS_UML "SendObjectAction"; */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "ObjectFlow",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_EDGE),
        .property_from            = "source",
        .property_to              = "target",
        .additional_properties    = "weight=\"1\" guard=\"true\"",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_TRANSITION] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 14.5.11 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Transition",
        .is_a                     = (XMI_ELEMENT_IS_A_TRANSITION),
        .property_from            = "source",
        .property_to              = "target",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DEPLOY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_DEPLOY,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Deployment",
        .is_a                     = (XMI_ELEMENT_IS_A_DEPENDENCY),
        .property_from            = "client",
        .property_to              = "supplier",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_MANIFEST] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_MANIFEST,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Manifestation",
        .is_a                     = (XMI_ELEMENT_IS_A_ABSTRACTION),
        .property_from            = "client",
        .property_to              = "supplier",
        .additional_properties    = NULL,
    },

    [XMI_ELEMENT_INFO_MAP_INDEX_EXTEND] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_EXTEND,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Extend",
        .is_a                     = (XMI_ELEMENT_IS_A_DIRECTED_RELATIONSHIP),
        .property_from            = "extension",
        .property_to              = "extendedCase",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_INCLUDE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_INCLUDE,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Include",
        .is_a                     = (XMI_ELEMENT_IS_A_DIRECTED_RELATIONSHIP),
        .property_from            = "includingCase",
        .property_to              = "addition",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_CONTAINMENT] = {
        /* Note: This is a fallback only, if xmi_type_converter_get_xmi_nesting_property_of_classifier() fails, e.g. at circular containments */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = "Association",
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_REFINE] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_REFINE,
        .specification            = (XMI_SPEC_STANDARD),
        .profile_name             = "Refine",
        .base_name                = "Abstraction",
        .is_a                     = (XMI_ELEMENT_IS_A_ABSTRACTION),
        .property_from            = "client",
        .property_to              = "supplier",
        .additional_properties    = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_TRACE] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_TRACE,
        .specification            = (XMI_SPEC_STANDARD),
        .profile_name             = "Trace",
        .base_name                = "Abstraction",
        .is_a                     = (XMI_ELEMENT_IS_A_ABSTRACTION),
        .property_from            = "client",
        .property_to              = "supplier",
        .additional_properties    = NULL,
    },
};

const xmi_element_info_t xmi_element_info_map_unknown_type =
{
    /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
    .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_CLASS,
    .specification            = XMI_SPEC_UML,
    .profile_name             = NULL,
    .base_name                = "Class",
    .is_a                     = (XMI_ELEMENT_IS_A_CLASS),
    .property_from            = NULL,
    .property_to              = NULL,
    .additional_properties    = "error=\"unknown type\"",
};

const xmi_element_info_t xmi_element_info_map_unknown_rel_type =
{
    /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 7.8.4 */
    .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
    .specification            = (XMI_SPEC_UML),
    .profile_name             = NULL,
    .base_name                = "Dependency",
    .is_a                     = (XMI_ELEMENT_IS_A_DEPENDENCY),
    .property_from            = "client",
    .property_to              = "supplier",
    .additional_properties    = "error=\"unknown type\"",
};


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
