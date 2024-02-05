/* File: gui_type_resource_list.c; Copyright and License: see below */

#include "gui_type_resource_list.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <gtk/gtk.h>
#include <assert.h>

void gui_type_resource_list_init ( gui_type_resource_list_t *this_, gui_resources_t *resources )
{
    U8_TRACE_BEGIN();
    assert( resources != NULL );
    (*this_).resources = resources;

    unsigned int idx = 0;
    gui_type_resource_t *current;

    (*this_).diagram_types_start = idx;
    {
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_list = gui_resources_get_type_diag_list ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_LIST, "List Diagram", icon_diag_list );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_box = gui_resources_get_type_diag_box ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_BOX_DIAGRAM, "Box Diagram", icon_diag_box );

        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_block = gui_resources_get_type_diag_block ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM, "Block Definition Diagram (SysML)", icon_diag_block );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_internal = gui_resources_get_type_diag_internal ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM, "Internal Block Diagram (SysML)", icon_diag_internal );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_parametric = gui_resources_get_type_diag_parametric ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM, "Parametric Diagram (SysML)", icon_diag_parametric );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_deployment = gui_resources_get_type_diag_deployment ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM, "Deployment Diagram (UML)", icon_diag_deployment );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_component = gui_resources_get_type_diag_component ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM, "Component Diagram (UML)", icon_diag_component );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_composite = gui_resources_get_type_diag_composite ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM, "Composite Structure Diagram (UML)", icon_diag_composite );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_package = gui_resources_get_type_diag_package ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, "Package Diagram (SysML, UML)", icon_diag_package );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_class = gui_resources_get_type_diag_class ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM, "Class Diagram (UML)", icon_diag_class );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_profile = gui_resources_get_type_diag_profile( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM, "Profile Diagram (UML)", icon_diag_profile );

        /* requirements diagram: */
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_requirement = gui_resources_get_type_diag_requirement ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM, "Requirements Diagram (SysML)", icon_diag_requirement );

        /* behavioral diagrams: */
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_usecase = gui_resources_get_type_diag_usecase ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, "Use Case Diagram (SysML, UML)", icon_diag_usecase );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_overview = gui_resources_get_type_diag_overview ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM, "Interaction Overview Diagram (UML)", icon_diag_overview );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_activity = gui_resources_get_type_diag_activity ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, "Activity Diagram (SysML, UML)", icon_diag_activity );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_state  = gui_resources_get_type_diag_state ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM, "State Machine Diagram (SysML, UML)", icon_diag_state );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_communication = gui_resources_get_type_diag_communication ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM, "Communication Diagram (UML)", icon_diag_communication );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_sequence = gui_resources_get_type_diag_sequence ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM, "Sequence Diagram (SysML, UML)", icon_diag_sequence );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_diag_timing = gui_resources_get_type_diag_timing ( (*this_).resources );
        gui_type_resource_init_diagram( current, DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM, "Timing Diagram (UML)", icon_diag_timing );
    }
    (*this_).diagram_types_length = idx - (*this_).diagram_types_start;
    assert( GUI_TYPE_RESOURCE_LIST_DIAGRAMS == (*this_).diagram_types_length );

    (*this_).classifier_types_start = idx;
    {
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_block = gui_resources_get_type_clas_block( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_BLOCK, "Block", icon_clas_block );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_constraint = gui_resources_get_type_clas_constraint( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK, "Constraint Block", icon_clas_constraint );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_node = gui_resources_get_type_clas_node( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_NODE, "Node", icon_clas_node );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_subsystem = gui_resources_get_type_clas_subsystem( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_SUBSYSTEM, "Subsystem/Boundary", icon_clas_subsystem );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_component = gui_resources_get_type_clas_component( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_COMPONENT, "Component", icon_clas_component );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_part = gui_resources_get_type_clas_part( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_PART, "Part (component instance)", icon_clas_part );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_interface = gui_resources_get_type_clas_interface( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_INTERFACE, "Interface", icon_clas_interface );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_package = gui_resources_get_type_clas_package( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_PACKAGE, "Package", icon_clas_package );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_class = gui_resources_get_type_clas_class( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_CLASS, "Class", icon_clas_class );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_object = gui_resources_get_type_clas_object( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_OBJECT, "Object (class instance)", icon_clas_object );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_artifact = gui_resources_get_type_clas_artifact( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_ARTIFACT, "Artifact", icon_clas_artifact );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_comment = gui_resources_get_type_clas_comment( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_COMMENT, "Comment", icon_clas_comment );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_image = gui_resources_get_type_clas_image( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_IMAGE, "Image (profile)", icon_clas_image );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_stereotype = gui_resources_get_type_clas_stereotype( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_STEREOTYPE, "Stereotype (profile)", icon_clas_stereotype );
        /* requirements classifiers: */
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_requirement = gui_resources_get_type_clas_requirement( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_REQUIREMENT, "Requirement", icon_clas_requirement );
        /* behavioral classifiers: */
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_actor = gui_resources_get_type_clas_actor( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_ACTOR, "Actor", icon_clas_actor );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_usecase = gui_resources_get_type_clas_usecase( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_USE_CASE, "Use Case", icon_clas_usecase );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_diagram = gui_resources_get_type_clas_diagram( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE, "Interaction Diagram Reference", icon_clas_diagram );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_activity = gui_resources_get_type_clas_activity( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_ACTIVITY, "Activity/Action", icon_clas_activity );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_region = gui_resources_get_type_clas_region( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION, "Interruptable Region", icon_clas_region );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_fork = gui_resources_get_type_clas_fork( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_FORK_NODE, "Fork", icon_clas_fork );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_join = gui_resources_get_type_clas_join( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE, "Join", icon_clas_join );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_accept = gui_resources_get_type_clas_accept( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT, "Accept Event", icon_clas_accept );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_time = gui_resources_get_type_clas_time( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT, "Accept Time Event", icon_clas_time );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_send = gui_resources_get_type_clas_send( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL, "Send Signal", icon_clas_send );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_decision = gui_resources_get_type_clas_decision( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE, "Decision/Choice", icon_clas_decision );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_initial = gui_resources_get_type_clas_initial( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE, "Initial Node", icon_clas_initial );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_final = gui_resources_get_type_clas_final( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE, "Final Node", icon_clas_final );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_state = gui_resources_get_type_clas_state( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_STATE, "State", icon_clas_state );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_history = gui_resources_get_type_clas_history( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY, "Shallow History", icon_clas_history );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_clas_deephistory = gui_resources_get_type_clas_deephistory( (*this_).resources );
        gui_type_resource_init_classifier( current, DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY, "Deep History", icon_clas_deephistory );
    }
    (*this_).classifier_types_length = idx - (*this_).classifier_types_start;
    assert( GUI_TYPE_RESOURCE_LIST_CLASSIFIERS == (*this_).classifier_types_length );

    (*this_).feature_types_start = idx;
    {
        /* order: from close to far */
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_property = gui_resources_get_type_feat_property( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_PROPERTY, "Property", icon_feat_property );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_operation = gui_resources_get_type_feat_operation( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_OPERATION, "Operation", icon_feat_operation );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_provide = gui_resources_get_type_feat_provide( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_PROVIDED_INTERFACE, "Provided Interface", icon_feat_provide );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_require = gui_resources_get_type_feat_require( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_REQUIRED_INTERFACE, "Required Interface", icon_feat_require );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_port = gui_resources_get_type_feat_port( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_PORT, "Port", icon_feat_port );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_rx = gui_resources_get_type_feat_rx( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_IN_PORT_PIN, "Input Pin/FlowPort(SysML)", icon_feat_rx );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_tx = gui_resources_get_type_feat_tx( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_OUT_PORT_PIN, "Output Pin/FlowPort(SysML)", icon_feat_tx );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_entry = gui_resources_get_type_feat_entry( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_ENTRY, "State Entry", icon_feat_entry );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_exit = gui_resources_get_type_feat_exit( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_EXIT, "State Exit", icon_feat_exit );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_tag = gui_resources_get_type_feat_tag( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_TAGGED_VALUE, "Tagged Value", icon_feat_tag );

        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_feat_life = gui_resources_get_type_feat_life( (*this_).resources );
        gui_type_resource_init_feature( current, DATA_FEATURE_TYPE_LIFELINE, "Lifeline", icon_feat_life );
    }
    (*this_).feature_types_length = idx - (*this_).feature_types_start;
    assert( GUI_TYPE_RESOURCE_LIST_FEATURES == (*this_).feature_types_length );

    (*this_).relationship_types_start = idx;
    {
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_depend = gui_resources_get_type_rel_depend( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, "Dependency", icon_rel_depend );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_contain = gui_resources_get_type_rel_contain( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, "Containment", icon_rel_contain );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_deploy = gui_resources_get_type_rel_deploy( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_DEPLOY, "Deploy", icon_rel_deploy );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_manifest = gui_resources_get_type_rel_manifest( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_MANIFEST, "Manifest", icon_rel_manifest );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_communication_path = gui_resources_get_type_rel_communication_path( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH, "Communication Path", icon_rel_communication_path );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_associate = gui_resources_get_type_rel_associate( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION, "Association", icon_rel_associate );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_aggregate = gui_resources_get_type_rel_aggregate( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_AGGREGATION, "Aggregation", icon_rel_aggregate );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_compose = gui_resources_get_type_rel_compose( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, "Composition", icon_rel_compose );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_generalize = gui_resources_get_type_rel_generalize( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION, "Generalization", icon_rel_generalize );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_realize = gui_resources_get_type_rel_realize( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_REALIZATION, "Realization", icon_rel_realize );
        /* requirements relationships: */
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_trace = gui_resources_get_type_rel_trace( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_TRACE, "Trace (requirement)", icon_rel_trace );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_refine = gui_resources_get_type_rel_refine( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_REFINE, "Refine (requirement)", icon_rel_refine );
        /* behavioral relationships: */
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_extend = gui_resources_get_type_rel_extend( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_EXTEND, "Extend (use case)", icon_rel_extend );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_include = gui_resources_get_type_rel_include( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_INCLUDE, "Include (use case)", icon_rel_include );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_control_flow = gui_resources_get_type_rel_control_flow( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW, "Control Flow/Transition", icon_rel_control_flow );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_object_flow = gui_resources_get_type_rel_object_flow( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW, "Object Flow", icon_rel_object_flow );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_async = gui_resources_get_type_rel_async( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL, "Async. Call", icon_rel_async );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_sync = gui_resources_get_type_rel_sync( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL, "Sync. Call", icon_rel_sync );
        assert( idx < GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
        current = &((*this_).type_name_icon_list[idx]); idx++;
        GdkTexture *icon_rel_return = gui_resources_get_type_rel_return( (*this_).resources );
        gui_type_resource_init_relationship( current, DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL, "Return Call", icon_rel_return );
    }
    (*this_).relationship_types_length = idx - (*this_).relationship_types_start;
    assert( GUI_TYPE_RESOURCE_LIST_RELATIONS == (*this_).relationship_types_length );

    assert( idx == GUI_TYPE_RESOURCE_LIST_MAX_TYPES );
    assert( (*this_).diagram_types_length + (*this_).classifier_types_length + (*this_).feature_types_length + (*this_).relationship_types_length
            == GUI_TYPE_RESOURCE_LIST_MAX_TYPES );

    GdkTexture *icon_undef = gui_resources_get_type_undef( (*this_).resources );
    gui_type_resource_init_classifier( &((*this_).type_undef), DATA_CLASSIFIER_TYPE_VOID, "", icon_undef );

    U8_TRACE_END();
}

void gui_type_resource_list_destroy ( gui_type_resource_list_t *this_ )
{
    U8_TRACE_BEGIN();

    gui_type_resource_destroy( &((*this_).type_undef) );

    U8_TRACE_END();
}

gui_type_resource_t * gui_type_resource_list_get_type ( gui_type_resource_list_t *this_, data_table_t table, int type )
{
    U8_TRACE_BEGIN();

    gui_type_resource_t *result;

    switch (table) {
        case DATA_TABLE_VOID:
        {
            result = &((*this_).type_undef);
            U8_LOG_ANOMALY("unexpected data_table_t in gui_type_resource_list_get_icon");
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            result = gui_type_resource_list_get_classifier_type( this_, (data_classifier_type_t) type );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            result = gui_type_resource_list_get_feature_type( this_, (data_feature_type_t) type );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            result = gui_type_resource_list_get_relationship_type( this_, (data_relationship_type_t) type );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            result = &((*this_).type_undef);
            U8_LOG_WARNING("unexpected data_table_t in gui_type_resource_list_get_icon");
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            result = gui_type_resource_list_get_diagram_type( this_, (data_diagram_type_t) type );
        }
        break;

        default:
        {
            result = &((*this_).type_undef);
            U8_LOG_WARNING("unexpected data_table_t in gui_type_resource_list_get_icon");
        }
        break;
    }

    U8_TRACE_END();
    return result;
}


/*
Copyright 2020-2024 Andreas Warnke

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
