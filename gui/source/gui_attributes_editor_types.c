/* File: gui_attributes_editor_types_types.c; Copyright and License: see below */

#include "gui_attributes_editor_types.h"
#include "util/gtk_helper/gtk_helper_tree_model.h"
#include "trace.h"
#include "data_table.h"
#include "data_id.h"
#include <gtk/gtk.h>

void gui_attributes_editor_types_init ( gui_attributes_editor_types_t *this_, gui_resources_t *resources )
{
    TRACE_BEGIN();

    assert( NULL != resources );

    (*this_).resources = resources;

    {
        GtkTreeIter iter;
        (*this_).no_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );
        gtk_list_store_append( (*this_).no_types, &iter);
        const GdkPixbuf *icon_undef_type = gui_resources_get_type_undef( (*this_).resources );
        gtk_list_store_set ( (*this_).no_types, &iter, 0, 0x0, 1, "-- n/a --", 2, icon_undef_type, -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).diagram_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );

        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_list = gui_resources_get_type_diag_list ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_LIST, 1, "List Diagram", 2, icon_diag_list, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_box = gui_resources_get_type_diag_box ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_BOX_DIAGRAM, 1, "Box Diagram", 2, icon_diag_box, -1 );

        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_block = gui_resources_get_type_diag_block ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM, 1, "Block Definition Diagram (SysML)", 2, icon_diag_block, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_internal = gui_resources_get_type_diag_internal ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM, 1, "Internal Block Diagram (SysML)", 2, icon_diag_internal, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_parametric = gui_resources_get_type_diag_parametric ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM, 1, "Parametric Diagram (SysML)", 2, icon_diag_parametric, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_deployment = gui_resources_get_type_diag_deployment ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM, 1, "Deployment Diagram (UML)", 2, icon_diag_deployment, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_component = gui_resources_get_type_diag_component ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM, 1, "Component Diagram (UML)", 2, icon_diag_component, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_composite = gui_resources_get_type_diag_composite ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM, 1, "Composite Structure Diagram (UML)", 2, icon_diag_composite, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_package = gui_resources_get_type_diag_package ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, 1, "Package Diagram (SysML, UML)", 2, icon_diag_package, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_class = gui_resources_get_type_diag_class ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM, 1, "Class Diagram (UML)", 2, icon_diag_class, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_profile = gui_resources_get_type_diag_profile( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM, 1, "Profile Diagram (UML)", 2, icon_diag_profile, -1 );

        /* requirements diagram: */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_requirement = gui_resources_get_type_diag_requirement ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM, 1, "Requirements Diagram (SysML)", 2, icon_diag_requirement, -1 );

        /* behavioral diagrams: */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_usecase = gui_resources_get_type_diag_usecase ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, 1, "Use Case Diagram (SysML, UML)", 2, icon_diag_usecase, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_overview = gui_resources_get_type_diag_overview ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM, 1, "Interaction Overview Diagram (UML)", 2, icon_diag_overview, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_activity = gui_resources_get_type_diag_activity ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, 1, "Activity Diagram (SysML, UML)", 2, icon_diag_activity, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_state  = gui_resources_get_type_diag_state ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM, 1, "State Machine Diagram (SysML, UML)", 2, icon_diag_state, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_communication = gui_resources_get_type_diag_communication ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM, 1, "Communication Diagram (UML), Scenario", 2, icon_diag_communication, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_sequence = gui_resources_get_type_diag_sequence ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM, 1, "Sequence Diagram (SysML, UML), Scenario", 2, icon_diag_sequence, -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        const GdkPixbuf *icon_diag_timing = gui_resources_get_type_diag_timing ( (*this_).resources );
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM, 1, "Timing Diagram (UML), Scenario", 2, icon_diag_timing, -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).classifier_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        const GdkPixbuf *icon_undef_type = gui_resources_get_type_undef( (*this_).resources );

        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_block = gui_resources_get_type_clas_block( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_BLOCK, 1, "Block", 2, icon_clas_block, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_constraint = gui_resources_get_type_clas_constraint( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY, 1, "Constraint Property/Equation (parametric)", 2, icon_clas_constraint, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_node = gui_resources_get_type_clas_node( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_NODE, 1, "Node (deployment)", 2, icon_clas_node, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_component = gui_resources_get_type_clas_component( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMPONENT, 1, "Component", 2, icon_clas_component, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_part = gui_resources_get_type_clas_part( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PART, 1, "Part (component instance)", 2, icon_clas_part, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_class = gui_resources_get_type_clas_class( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_INTERFACE, 1, "Interface", 2, icon_clas_class, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_package = gui_resources_get_type_clas_package( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PACKAGE, 1, "Package", 2, icon_clas_package, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_CLASS, 1, "Class", 2, icon_clas_class, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_OBJECT, 1, "Object (class instance)", 2, icon_clas_part, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_artifact = gui_resources_get_type_clas_artifact( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ARTIFACT, 1, "Artifact", 2, icon_clas_artifact, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_comment = gui_resources_get_type_clas_comment( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMMENT, 1, "Comment", 2, icon_clas_comment, -1 );
        /* requirements classifiers: */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_requirement = gui_resources_get_type_clas_requirement( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_FEATURE, 1, "Feature", 2, icon_clas_requirement, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_REQUIREMENT, 1, "Requirement", 2, icon_clas_requirement, -1 );
        /* behavioral classifiers: */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_actor = gui_resources_get_type_clas_actor( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTOR, 1, "Actor (use case)", 2, icon_clas_actor, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_usecase = gui_resources_get_type_clas_usecase( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_USE_CASE, 1, "Use Case (use case)", 2, icon_clas_usecase, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_boundary = gui_resources_get_type_clas_boundary( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY, 1, "System Boundary (use case)", 2, icon_clas_boundary, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_diagram = gui_resources_get_type_clas_diagram( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE, 1, "Diagram Reference (interaction overview)", 2, icon_clas_diagram, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_state = gui_resources_get_type_clas_state( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTIVITY, 1, "Activity", 2, icon_clas_state, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_region = gui_resources_get_type_clas_region( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION, 1, "Interruptable Region", 2, icon_clas_region, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_fork = gui_resources_get_type_clas_fork( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_FORK_NODE, 1, "Fork", 2, icon_clas_fork, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_join = gui_resources_get_type_clas_join( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE, 1, "Join", 2, icon_clas_join, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_accept = gui_resources_get_type_clas_accept( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT, 1, "Accept Event", 2, icon_clas_accept, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_time = gui_resources_get_type_clas_time( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT, 1, "Accept Time Event", 2, icon_clas_time, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_send = gui_resources_get_type_clas_send( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL, 1, "Send Signal", 2, icon_clas_send, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_decision = gui_resources_get_type_clas_decision( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE, 1, "Decision", 2, icon_clas_decision, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_initial = gui_resources_get_type_clas_initial( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE, 1, "Initial Node", 2, icon_clas_initial, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_final = gui_resources_get_type_clas_final( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE, 1, "Final Node", 2, icon_clas_final, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_STATE, 1, "State", 2, icon_clas_state, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_history = gui_resources_get_type_clas_history( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY, 1, "Shallow History", 2, icon_clas_history, -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        const GdkPixbuf *icon_clas_deephistory = gui_resources_get_type_clas_deephistory( (*this_).resources );
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY, 1, "Deep History", 2, icon_clas_deephistory, -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).feature_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );
        /* order: from close to far */
        gtk_list_store_append( (*this_).feature_types, &iter);
        const GdkPixbuf *icon_feat_property = gui_resources_get_type_feat_property( (*this_).resources );
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PROPERTY, 1, "Property", 2, icon_feat_property, -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        const GdkPixbuf *icon_feat_operation = gui_resources_get_type_feat_operation( (*this_).resources );
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_OPERATION, 1, "Operation", 2, icon_feat_operation, -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        const GdkPixbuf *icon_feat_port = gui_resources_get_type_feat_port( (*this_).resources );
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PORT, 1, "Port", 2, icon_feat_port, -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        const GdkPixbuf *icon_feat_provide = gui_resources_get_type_feat_provide( (*this_).resources );
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PROVIDED_INTERFACE, 1, "Provided Interface", 2, icon_feat_provide, -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        const GdkPixbuf *icon_feat_require = gui_resources_get_type_feat_require( (*this_).resources );
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_REQUIRED_INTERFACE, 1, "Required Interface", 2, icon_feat_require, -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).feature_lifeline_type = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );
        gtk_list_store_append( (*this_).feature_lifeline_type, &iter);
        const GdkPixbuf *icon_feat_life = gui_resources_get_type_feat_life( (*this_).resources );
        gtk_list_store_set ( (*this_).feature_lifeline_type, &iter, 0, DATA_FEATURE_TYPE_LIFELINE, 1, "Lifeline", 2, icon_feat_life, -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).relationship_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );
        const GdkPixbuf *icon_simple_arrow = gui_resources_get_type_rel_associate( (*this_).resources );

        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_depend = gui_resources_get_type_rel_depend( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, 1, "Dependency", 2, icon_rel_depend, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_contain = gui_resources_get_type_rel_contain( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, 1, "Containment (deployment, package), no cycles", 2, icon_rel_contain, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_deploy = gui_resources_get_type_rel_deploy( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_DEPLOY, 1, "Deploy (deployment), no cycles", 2, icon_rel_deploy, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_manifest = gui_resources_get_type_rel_manifest( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_MANIFEST, 1, "Manifest (deployment), no cycles", 2, icon_rel_manifest, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH, 1, "Communication Path (component)", 2, icon_simple_arrow, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION, 1, "Association (class, use-case)", 2, icon_simple_arrow, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_aggregate = gui_resources_get_type_rel_aggregate( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_AGGREGATION, 1, "Aggregation (class), no cycles", 2, icon_rel_aggregate, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_compose = gui_resources_get_type_rel_compose( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, 1, "Composition (class), no cycles", 2, icon_rel_compose, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_generalize = gui_resources_get_type_rel_generalize( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION, 1, "Generalization (class), no cycles", 2, icon_rel_generalize, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_realize = gui_resources_get_type_rel_realize( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_REALIZATION, 1, "Realization (class), no cycles", 2, icon_rel_realize, -1 );
        /* requirements relationships: */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_trace = gui_resources_get_type_rel_trace( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_TRACE, 1, "Trace (requirement), no cycles", 2, icon_rel_trace, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_refine = gui_resources_get_type_rel_refine( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_REFINE, 1, "Refine (requirement), no cycles", 2, icon_rel_refine, -1 );
        /* behavioral relationships: */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_extend = gui_resources_get_type_rel_extend( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_EXTEND, 1, "Extend (use-case), no cycles", 2, icon_rel_extend, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_include = gui_resources_get_type_rel_include( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_INCLUDE, 1, "Include (use-case), no cycles", 2, icon_rel_include, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW, 1, "Control Flow (activity)", 2, icon_simple_arrow, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW, 1, "Object Flow (activity)", 2, icon_simple_arrow, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL, 1, "Async. Call (sequence)", 2, icon_simple_arrow, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_sync = gui_resources_get_type_rel_sync( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL, 1, "Sync. Call (sequence)", 2, icon_rel_sync, -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        const GdkPixbuf *icon_rel_return = gui_resources_get_type_rel_return( (*this_).resources );
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL, 1, "Return Call (sequence)", 2, icon_rel_return, -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).shortlist_types = gtk_list_store_new( 3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        const GdkPixbuf *icon_undef_type = gui_resources_get_type_undef( (*this_).resources );
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_LIST, 1, "List Diagram", 2, icon_undef_type, -1 );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_BOX_DIAGRAM, 1, "Box Diagram", 2, icon_undef_type, -1 );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        const GdkPixbuf *icon_diag_list = gui_resources_get_type_diag_list ( (*this_).resources );
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_LIST, 1, "List Diagram", 2, icon_diag_list, -1 );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        const GdkPixbuf *icon_diag_box = gui_resources_get_type_diag_box ( (*this_).resources );
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_BOX_DIAGRAM, 1, "Box Diagram", 2, icon_diag_box, -1 );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        const GdkPixbuf *icon_diag_usecase = gui_resources_get_type_diag_usecase ( (*this_).resources );
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, 1, "Use Case Diagram (SysML, UML)", 2, icon_diag_usecase, -1 );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM, 1, "Interaction Overview Diagram (UML)", 2, icon_undef_type, -1 );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, 1, "Activity Diagram (SysML, UML)", 2, icon_undef_type, -1 );
        gtk_list_store_append( (*this_).shortlist_types, &iter);
        const GdkPixbuf *icon_diag_state  = gui_resources_get_type_diag_state ( (*this_).resources );
        gtk_list_store_set ( (*this_).shortlist_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM, 1, "State Machine Diagram (SysML, UML)", 2, icon_diag_state, -1 );
    }

    TRACE_END();
}

void gui_attributes_editor_types_destroy ( gui_attributes_editor_types_t *this_ )
{
    TRACE_BEGIN();

    g_object_unref((*this_).no_types);
    (*this_).no_types = NULL;

    g_object_unref((*this_).diagram_types);
    (*this_).diagram_types = NULL;

    g_object_unref((*this_).classifier_types);
    (*this_).classifier_types = NULL;

    g_object_unref((*this_).feature_types);
    (*this_).feature_types = NULL;

    g_object_unref((*this_).feature_lifeline_type);
    (*this_).feature_lifeline_type = NULL;

    g_object_unref((*this_).relationship_types);
    (*this_).relationship_types = NULL;

    g_object_unref((*this_).shortlist_types);
    (*this_).shortlist_types = NULL;

    (*this_).resources = NULL;

    TRACE_END();
}


/*
Copyright 2016-2020 Andreas Warnke

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
