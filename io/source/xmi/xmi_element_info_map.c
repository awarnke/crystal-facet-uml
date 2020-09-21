/* File: xmi_element_info_map.c; Copyright and License: see below */

#include "xmi/xmi_element_info_map.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

#define XMI_ELEMENT_INFO_NS_UML "uml:"
#define XMI_ELEMENT_INFO_NS_SYSML "SysML:"
#define XMI_ELEMENT_INFO_NS_STDPROF "StandardProfile:"

const xmi_element_info_map_t xmi_element_info_map_standard
= {
    /* ================================ CLASSIFIER ================================ */

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
        .is_a                     = (XMI_ELEMENT_IS_A_NODE),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ACTIVITY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_ACTIVITY,
        .specification            = (XMI_SPEC_UML | XMI_SPEC_SYSML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Activity",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY),
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
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY),
        .property_from            = NULL,
        .property_to              = NULL,
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_NODE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_CLASSIFIER_TYPE_UML_NODE,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML   "Node",
        .is_a                     = (XMI_ELEMENT_IS_A_NODE),
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
        .is_a                     = (XMI_ELEMENT_IS_A_ARTIFACT),
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

    /* ================================ FEATURE ================================ */

    /* ================================ RELATIONSHIP ================================ */

    [XMI_ELEMENT_INFO_MAP_INDEX_DEPENDENCY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Dependency",
        .is_a                     = (XMI_ELEMENT_IS_A_DEPENDENCY),
        .property_from            = "source",  /* "client",   */
        .property_to              = "target",  /* "supplier", */
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ASSOCIATION] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: StructuredClassifiers */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Association",
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_AGGREGATION] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.5 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_AGGREGATION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Association",  /* TODO an Aggregation is an Association with special properties */
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_COMPOSITION] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 11.5 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_COMPOSITION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Association",  /* TODO a Composition is an Association with special properties */
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_GENERALIZATION] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Classification */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Generalization",
        .is_a                     = (XMI_ELEMENT_IS_A_DIRECTED_RELATIONSHIP),
        .property_from            = "specific",
        .property_to              = "general",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_REALIZATION] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: SimpleClassifiers */
        /* XMI_ELEMENT_INFO_NS_UML "InterfaceRealization"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonStructure */
        /* XMI_ELEMENT_INFO_NS_UML "Realization"; */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_REALIZATION,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Realization",
        .is_a                     = (XMI_ELEMENT_IS_A_ABSTRACTION),
        .property_from            = "source",  /* "client",   */
        .property_to              = "target",  /* "supplier", */
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_ASYNC_CALL] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Interactions */
        /* XMI_ELEMENT_INFO_NS_UML "Message"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: CommonBehavior */
        /* XMI_ELEMENT_INFO_NS_UML "SignalEvent"; */
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.18, 17.12.22 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Interaction",  /* TODO: applicable only in an interaction context, see asynchSignal */
        /*.base_name                = XMI_ELEMENT_INFO_NS_UML     "Message",*/  /* TODO: applicable only in an interaction context, see asynchSignal */
        .is_a                     = (XMI_ELEMENT_IS_A_NAMED_ELEMENT),
        .property_from            = "lifeline",
        .property_to              = "lifeline",
        /*.extra_attribute          = "messageSort=\"asynchSignal\"",*/
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_SYNC_CALL] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.18, 17.12.22 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Interaction",  /* TODO: applicable only in an interaction context, see synchCall */
        /*.base_name                = XMI_ELEMENT_INFO_NS_UML     "Message",*/  /* TODO: applicable only in an interaction context, see synchCall */
        .is_a                     = (XMI_ELEMENT_IS_A_NAMED_ELEMENT),
        .property_from            = "lifeline",
        .property_to              = "lifeline",
        /*.extra_attribute          = "messageSort=\"synchCall\"",*/
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_RETURN_CALL] = {
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 17.12.18, 17.12.22 */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Interaction",  /* TODO: applicable only in an interaction context, see reply */
        /*.base_name                = XMI_ELEMENT_INFO_NS_UML     "Message",*/  /* TODO: applicable only in an interaction context, see reply */
        .is_a                     = (XMI_ELEMENT_IS_A_NAMED_ELEMENT),
        .property_from            = "lifeline",
        .property_to              = "lifeline",
        /*.extra_attribute          = "messageSort=\"reply\"",*/
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_COMMUNICATION_PATH] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "CommunicationPath",
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_CONTROL_FLOW] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* XMI_TYPE_CONVERTER_NS_UML "ControlFlow"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
        /* XMI_TYPE_CONVERTER_NS_UML "SendSignalAction"; */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "ControlFlow",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_EDGE),
        .property_from            = "x-source",
        .property_to              = "x-target",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_OBJECT_FLOW] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Activities */
        /* XMI_TYPE_CONVERTER_NS_UML "ObjectFlow"; */
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Actions */
        /* XMI_TYPE_CONVERTER_NS_UML "SendObjectAction"; */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "ObjectFlow",
        .is_a                     = (XMI_ELEMENT_IS_A_ACTIVITY_EDGE),
        .property_from            = "x-source",
        .property_to              = "x-target",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_DEPLOY] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_DEPLOY,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Deployment",
        .is_a                     = (XMI_ELEMENT_IS_A_DEPENDENCY),
        .property_from            = "source",  /* "client",   */
        .property_to              = "target",  /* "supplier", */
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_MANIFEST] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: Deployments */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_MANIFEST,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Manifestation",
        .is_a                     = (XMI_ELEMENT_IS_A_ABSTRACTION),
        .property_from            = "source",  /* "client",   */
        .property_to              = "target",  /* "supplier", */
    },

    [XMI_ELEMENT_INFO_MAP_INDEX_EXTEND] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_EXTEND,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Extend",
        .is_a                     = (XMI_ELEMENT_IS_A_DIRECTED_RELATIONSHIP),
        .property_from            = "extension",
        .property_to              = "extendedCase",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_INCLUDE] = {
        /* spec: https://www.omg.org/spec/UML/20161101/UML.xmi (v2.5.1) pkg: UseCases */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_INCLUDE,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Include",
        .is_a                     = (XMI_ELEMENT_IS_A_DIRECTED_RELATIONSHIP),
        .property_from            = "includingCase",
        .property_to              = "addition",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_CONTAINMENT] = {
        /* Note: This is a fallback only, if xmi_type_converter_get_xmi_nesting_property_of_classifier() fails, e.g. at circular containments */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT,
        .specification            = (XMI_SPEC_UML),
        .profile_name             = NULL,
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Association",  /* TODO a Composition is an Association with special properties */
        .is_a                     = (XMI_ELEMENT_IS_A_ASSOCIATION),
        /* spec-ref: https://www.omg.org/spec/UML/2.5.1/PDF chapter 9.5.3, 9.8.3 */
        .property_from            = "memberEnd",
        .property_to              = "memberEnd",
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_REFINE] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_REFINE,
        .specification            = (XMI_SPEC_STANDARD),
        .profile_name             = XMI_ELEMENT_INFO_NS_STDPROF "Refine",
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Abstraction",
        .is_a                     = (XMI_ELEMENT_IS_A_ABSTRACTION),
        .property_from            = "source",  /* "client",   */
        .property_to              = "target",  /* "supplier", */
    },
    [XMI_ELEMENT_INFO_MAP_INDEX_TRACE] = {
        /* spec: https://www.omg.org/spec/SysML/20181001/SysML.xmi (v1.6) pkg: Requirements */
        .data_type_checksum       = (int) DATA_RELATIONSHIP_TYPE_UML_TRACE,
        .specification            = (XMI_SPEC_STANDARD),
        .profile_name             = XMI_ELEMENT_INFO_NS_STDPROF "Trace",
        .base_name                = XMI_ELEMENT_INFO_NS_UML     "Abstraction",
        .is_a                     = (XMI_ELEMENT_IS_A_ABSTRACTION),
        .property_from            = "source",  /* "client",   */
        .property_to              = "target",  /* "supplier", */
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
