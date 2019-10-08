/* File: gui_textedit.c; Copyright and License: see below */

#include "gui_textedit.h"
#include "util/gtk_helper/gtk_helper_tree_model.h"
#include "trace.h"
#include "data_table.h"
#include "data_id.h"
#include "util/string/utf8string.h"
#include <gtk/gtk.h>
#include <stdbool.h>

void gui_textedit_init ( gui_textedit_t *this_,
                         GtkLabel *id_label,
                         GtkEntry *name_entry,
                         GtkEntry *stereotype_entry,
                         GtkComboBox *type_combo_box,
                         GtkTextView *description_text_view,
                         GtkButton *commit_button,
                         ctrl_controller_t *controller,
                         data_database_reader_t *db_reader,
                         data_database_t *database,
                         gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();
    assert( NULL != id_label );
    assert( NULL != name_entry );
    assert( NULL != stereotype_entry );
    assert( NULL != type_combo_box );
    assert( NULL != description_text_view );
    assert( NULL != commit_button );

    assert( NULL != controller );
    assert( NULL != db_reader );
    assert( NULL != message_to_user );

    (*this_).id_label = id_label;
    (*this_).name_entry = name_entry;
    (*this_).stereotype_entry = stereotype_entry;
    (*this_).type_combo_box = type_combo_box;
    (*this_).description_text_view = description_text_view;
    (*this_).commit_button = commit_button;

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).database = database;
    (*this_).message_to_user = message_to_user;
    data_diagram_init_empty( &((*this_).private_diagram_cache) );
    data_classifier_init_empty( &((*this_).private_classifier_cache) );
    data_feature_init_empty( &((*this_).private_feature_cache) );
    data_relationship_init_empty( &((*this_).private_relationship_cache) );
    data_id_init_void( &((*this_).selected_object_id) );

    {
        GtkTreeIter iter;
        (*this_).no_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).no_types, &iter);
        gtk_list_store_set ( (*this_).no_types, &iter, 0, 0x0, 1, "-- n/a --", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).diagram_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_LIST, 1, "List Diagram", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_BOX_DIAGRAM, 1, "Box Diagram", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM, 1, "Block Definition Diagram (SysML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM, 1, "Internal Block Diagram (SysML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM, 1, "Parametric Diagram (SysML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM, 1, "Deployment Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM, 1, "Component Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM, 1, "Composite Structure Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, 1, "Package Diagram (SysML, UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM, 1, "Class Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM, 1, "Profile Diagram (UML)", -1 );
        /* requirements diagram: */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM, 1, "Requirements Diagram (SysML)", -1 );
        /* behavioral diagrams: */
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, 1, "Use Case Diagram (SysML, UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM, 1, "Interaction Overview Diagram (UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, 1, "Activity Diagram (SysML, UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM, 1, "State Machine Diagram (SysML, UML)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM, 1, "Communication Diagram (UML), Scenario", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM, 1, "Sequence Diagram (SysML, UML), Scenario", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM, 1, "Timing Diagram (UML), Scenario", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).classifier_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_BLOCK, 1, "Block", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY, 1, "Constraint Property/Equation (parametric)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_NODE, 1, "Node (deployment)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMPONENT, 1, "Component", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PART, 1, "Part (component instance)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_INTERFACE, 1, "Interface", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PACKAGE, 1, "Package", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_CLASS, 1, "Class", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_OBJECT, 1, "Object (class instance)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ARTIFACT, 1, "Artifact", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMMENT, 1, "Comment", -1 );
        /* requirements classifiers: */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_FEATURE, 1, "Feature", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_REQUIREMENT, 1, "Requirement", -1 );
        /* behavioral classifiers: */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTOR, 1, "Actor (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_USE_CASE, 1, "Use Case (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY, 1, "System Boundary (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE, 1, "Diagram Reference (interaction overview)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTIVITY, 1, "Activity", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION, 1, "Interruptable Region", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_FORK_NODE, 1, "Fork", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE, 1, "Join", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT, 1, "Accept Event", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT, 1, "Accept Time Event", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL, 1, "Send Signal", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE, 1, "Decision", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE, 1, "Initial Node", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE, 1, "Final Node", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_STATE, 1, "State", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY, 1, "Shallow History", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY, 1, "Deep History", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).feature_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        /* order: from close to far */
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PROPERTY, 1, "Property", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_OPERATION, 1, "Operation", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PORT, 1, "Port", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PROVIDED_INTERFACE, 1, "Provided Interface", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_REQUIRED_INTERFACE, 1, "Required Interface", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).feature_lifeline_type = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).feature_lifeline_type, &iter);
        gtk_list_store_set ( (*this_).feature_lifeline_type, &iter, 0, DATA_FEATURE_TYPE_LIFELINE, 1, "Lifeline", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).relationship_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        /* order: structural from abstract to concrete, behavioral from abstract to concrete */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, 1, "Dependency", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, 1, "Containment (deployment, package), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_DEPLOY, 1, "Deploy (deployment), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_MANIFEST, 1, "Manifest (deployment), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH, 1, "Communication Path (component)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION, 1, "Association (class, use-case)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_AGGREGATION, 1, "Aggregation (class), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, 1, "Composition (class), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION, 1, "Generalization (class), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_REALIZATION, 1, "Realization (class), no cycles", -1 );
        /* requirements relationships: */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_TRACE, 1, "Trace (requirement), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_REFINE, 1, "Refine (requirement), no cycles", -1 );
        /* behavioral relationships: */
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_EXTEND, 1, "Extend (use-case), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_INCLUDE, 1, "Include (use-case), no cycles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW, 1, "Control Flow (activity)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW, 1, "Object Flow (activity)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL, 1, "Async. Call (sequence)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL, 1, "Sync. Call (sequence)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL, 1, "Return Call (sequence)", -1 );
    }

    /* update widgets */
    gui_textedit_update_widgets( this_ );

    TRACE_END();
}

void gui_textedit_destroy ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    data_id_destroy( &((*this_).selected_object_id) );
    data_diagram_destroy( &((*this_).private_diagram_cache) );
    data_classifier_destroy( &((*this_).private_classifier_cache) );
    data_feature_destroy( &((*this_).private_feature_cache) );
    data_relationship_destroy( &((*this_).private_relationship_cache) );

    g_object_unref((*this_).diagram_types);
    (*this_).diagram_types = NULL;

    g_object_unref((*this_).no_types);
    (*this_).no_types = NULL;

    g_object_unref((*this_).classifier_types);
    (*this_).classifier_types = NULL;

    g_object_unref((*this_).relationship_types);
    (*this_).relationship_types = NULL;

    g_object_unref((*this_).feature_types);
    (*this_).feature_types = NULL;

    g_object_unref((*this_).feature_lifeline_type);
    (*this_).feature_lifeline_type = NULL;

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
    (*this_).message_to_user = NULL;

    (*this_).id_label = NULL;
    (*this_).name_entry = NULL;
    (*this_).stereotype_entry = NULL;
    (*this_).description_text_view = NULL;

    TRACE_END();
}

void gui_textedit_update_widgets ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    gui_textedit_private_id_update_view( this_ );
    gui_textedit_private_name_update_view ( this_ );
    gui_textedit_private_stereotype_update_view( this_ );
    gui_textedit_private_description_update_view( this_ );
    gui_textedit_private_type_update_view( this_ );

    TRACE_END();
}

void gui_textedit_commit_changes ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    gui_textedit_private_name_commit_changes ( this_ );
    gui_textedit_private_stereotype_commit_changes ( this_ );
    gui_textedit_private_description_commit_changes ( this_ );

    TRACE_END();
}

void gui_textedit_trace ( const gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    TRACE_INFO( "gui_textedit_t" );
    TRACE_INFO( "- selected_object_id:" );
    data_id_trace( &((*this_).selected_object_id) );

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            TRACE_INFO( "- cached object: none" );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            TRACE_INFO( "- cached object:" );
            data_classifier_trace( &((*this_).private_classifier_cache) );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            TRACE_INFO( "- cached object:" );
            data_feature_trace( &((*this_).private_feature_cache) );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            TRACE_INFO( "- cached object:" );
            data_relationship_trace( &((*this_).private_relationship_cache) );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            TRACE_INFO( "- cached object: ANOMALY, UNEXPECTED VALUE OF data_table_t" );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            TRACE_INFO( "- cached object:" );
            data_diagram_trace( &((*this_).private_diagram_cache) );
        }
        break;

        default:
        {
            TRACE_INFO( "- cached object: ERROR, ILLEGAL ENUM VALUE OF data_table_t" );
        }
        break;
    }

    /* id: */
    if ( TRACE_ACTIVE )
    {
        GtkLabel *id_widget;
        const char* text;
        id_widget = GTK_LABEL( (*this_).id_label );
        text = gtk_label_get_text( id_widget );
        TRACE_INFO_STR( "- visible id:", text );
    }

    /* name: */
    if ( TRACE_ACTIVE )
    {
        GtkEntry *name_widget;
        const char* text;
        name_widget = GTK_ENTRY( (*this_).name_entry );
        text = gtk_entry_get_text( name_widget );
        TRACE_INFO_STR( "- visible name:", text );
    }

    /* stereotype: */
    if ( TRACE_ACTIVE )
    {
        GtkEntry *stereotype_widget;
        const char* text;
        stereotype_widget = GTK_ENTRY( (*this_).stereotype_entry );
        text = gtk_entry_get_text( stereotype_widget );
        TRACE_INFO_STR( "- visible stereotype:", text );
    }

    /* type: */
    if ( TRACE_ACTIVE )
    {
        GtkComboBox *type_widget;
        int obj_type;
        int index;
        type_widget = GTK_COMBO_BOX( (*this_).type_combo_box );
        index = gtk_combo_box_get_active ( type_widget );
        obj_type = gtk_helper_tree_model_get_id( gtk_combo_box_get_model( type_widget ), 0, index );
        TRACE_INFO_INT( "- visible type id:", obj_type );
    }

    /* description: */
    if ( TRACE_ACTIVE )
    {
        GtkTextView *description_widget;
        GtkTextBuffer *buffer;
        description_widget = GTK_TEXT_VIEW( (*this_).description_text_view );
        buffer = gtk_text_view_get_buffer ( description_widget );
        GtkTextIter start;
        GtkTextIter end;
        gtk_text_buffer_get_start_iter ( buffer, &start );
        gtk_text_buffer_get_end_iter ( buffer, &end );
        const char* text;
        text = gtk_text_buffer_get_text ( buffer, &start, &end, false );
        TRACE_INFO_STR( "- visible description:", text );
    }

    TRACE_END();
}

/* ================================ USER INPUT CALLBACKS ================================ */

gboolean gui_textedit_name_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( GTK_ENTRY( widget ) == GTK_ENTRY( (*this_).name_entry ) );

    gui_textedit_private_name_commit_changes( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

gboolean gui_textedit_stereotype_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( GTK_ENTRY( widget ) == GTK_ENTRY( (*this_).stereotype_entry ) );

    gui_textedit_private_stereotype_commit_changes( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

void gui_textedit_type_changed_callback ( GtkComboBox *widget, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( GTK_COMBO_BOX( widget ) == GTK_COMBO_BOX( (*this_).type_combo_box ) );

    gui_textedit_private_type_commit_changes( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

gboolean gui_textedit_description_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( GTK_TEXT_VIEW( widget ) == GTK_TEXT_VIEW( (*this_).description_text_view ) );

    gui_textedit_private_description_commit_changes( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

void gui_textedit_commit_clicked_callback (GtkButton *button, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( GTK_BUTTON( button ) == GTK_BUTTON( (*this_).commit_button ) );

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    gui_textedit_commit_changes ( this_ );

    data_error_t d_err;
    d_err = DATA_ERROR_NONE;
    d_err |= data_database_trace_stats( (*this_).database );
    d_err |= data_database_flush_caches( (*this_).database );
    d_err |= data_database_trace_stats( (*this_).database );
    if ( DATA_ERROR_NONE != d_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_WRITE_ERROR
        );
    }
    else
    {
        #ifndef NDEBUG
        /* in debug mode, also check consistency of database */
        char repair_log_buffer[2000] = "";
        utf8stringbuf_t repair_log = UTF8STRINGBUF( repair_log_buffer );
        uint32_t found_errors;
        uint32_t fixed_errors;
        ctrl_controller_repair_database( (*this_).controller, false /* no repair, just test */, &found_errors, &fixed_errors, repair_log );
        fprintf( stdout, "\n\n%s\n", utf8stringbuf_get_string(repair_log) );
        if (( found_errors != 0 ) || ( fixed_errors != 0 ))
        {
            gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                 GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR,
                                                                 "ctrl_controller_repair_database() reported inconsistent db!"
            );
        }
        #endif
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

/* ================================ SELECTION or MODEL CHANGED CALLBACKS ================================ */

#if 1

void gui_textedit_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != id );
    assert ( NULL != widget );

    /* is a new object selected ? */
    if ( ! data_id_equals ( &((*this_).selected_object_id), id ) )
    {
        /* store all changes on the old object */
        gui_textedit_commit_changes( this_ );

        /* load the new object */
        data_id_trace( id );
        gui_textedit_private_load_object( this_, *id );

        /* update all widgets now */
        gui_textedit_update_widgets( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != msg );
    assert ( NULL != widget );

    data_id_t id;
    data_change_event_type_t evt_type;
    id = data_change_message_get_modified( msg );
    evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE )
    {
        /* store all changes on the old object */
        gui_textedit_commit_changes( this_ );
    }
    else if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_CLOSED )
    {
        data_id_t nothing;
        data_id_init_void( &nothing );
        gui_textedit_private_load_object( this_, nothing );  /* clear cached data */
        gui_textedit_update_widgets ( this_ );
    }
    else if ( data_id_equals( &id, &((*this_).selected_object_id) ) )
    {
        data_change_message_trace( msg );
        gui_textedit_private_load_object( this_, id );  /* checks if object still exists */
        gui_textedit_update_widgets ( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

#else

void gui_textedit_id_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != id );
    assert ( NULL != widget );

    /* is a new object selected ? */
    if ( ! data_id_equals ( &((*this_).selected_object_id), id ) )
    {
        /* store all changes (of the other widgets) on the old object */
        gui_textedit_commit_changes( this_ );

        /* load the new object */
        data_id_trace( id );
        gui_textedit_private_load_object( this_, *id );

        /* update all widgets now (only now, after committing all changes) */
        gui_textedit_update_widgets( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_name_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != id );
    assert ( NULL != widget );

    /* is a new object selected ? */
    if ( ! data_id_equals ( &((*this_).selected_object_id), id ) )
    {
        /* store all changes on the old object */
        gui_textedit_commit_changes( this_ );

        /* load the new object */
        data_id_trace( id );
        gui_textedit_private_load_object( this_, *id );

        /* update all widgets now (only now,  after committing all changes) */
        gui_textedit_update_widgets( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_stereotype_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != id );
    assert ( NULL != widget );

    /* is a new object selected ? */
    if ( ! data_id_equals ( &((*this_).selected_object_id), id ) )
    {
        /* store all changes on the old object */
        gui_textedit_commit_changes( this_ );

        /* load the new object */
        data_id_trace( id );
        gui_textedit_private_load_object( this_, *id );

        /* update all widgets now (only now,  after committing all changes) */
        gui_textedit_update_widgets( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_type_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != id );
    assert ( NULL != widget );

    /* is a new object selected ? */
    if ( ! data_id_equals ( &((*this_).selected_object_id), id ) )
    {
        /* store all changes on the old object */
        gui_textedit_commit_changes( this_ );

        /* load the new object */
        data_id_trace( id );
        gui_textedit_private_load_object( this_, *id );

        /* update all widgets now (only now,  after committing all changes) */
        gui_textedit_update_widgets( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_description_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != id );
    assert ( NULL != widget );

    /* is a new object selected ? */
    if ( ! data_id_equals ( &((*this_).selected_object_id), id ) )
    {
        /* store all changes on the old object */
        gui_textedit_commit_changes( this_ );

        /* load the new object */
        data_id_trace( id );
        gui_textedit_private_load_object( this_, *id );

        /* update all widgets now (only now,  after committing all changes) */
        gui_textedit_update_widgets( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_id_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != msg );
    assert ( NULL != widget );

    data_id_t id;
    data_change_event_type_t evt_type;
    id = data_change_message_get_modified( msg );
    evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE )
    {
        /* nothing to do: id is not editable */
    }

    if (( evt_type == DATA_CHANGE_EVENT_TYPE_DB_CLOSED )
        || ( data_id_equals( &id, &((*this_).selected_object_id) ) ))
    {
        data_change_message_trace( msg );
        gui_textedit_private_load_object( this_, id );  /* check if object still exists */
        gui_textedit_private_id_update_view ( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_name_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != msg );
    assert ( NULL != widget );

    data_id_t id;
    data_change_event_type_t evt_type;
    id = data_change_message_get_modified( msg );
    evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE )
    {
        data_change_message_trace( msg );
        gui_textedit_private_name_commit_changes( this_ );
    }

    if (( evt_type == DATA_CHANGE_EVENT_TYPE_DB_CLOSED )
        || ( data_id_equals( &id, &((*this_).selected_object_id) ) ))
    {
        data_change_message_trace( msg );
        gui_textedit_private_load_object( this_, id );
        gui_textedit_private_name_update_view ( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_stereotype_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != msg );
    assert ( NULL != widget );

    data_id_t id;
    data_change_event_type_t evt_type;
    id = data_change_message_get_modified( msg );
    evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE )
    {
        data_change_message_trace( msg );
        gui_textedit_private_stereotype_commit_changes( this_ );
    }

    if (( evt_type == DATA_CHANGE_EVENT_TYPE_DB_CLOSED )
        || ( data_id_equals( &id, &((*this_).selected_object_id) ) ))
    {
        data_change_message_trace( msg );
        gui_textedit_private_load_object( this_, id );
        gui_textedit_private_stereotype_update_view ( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_type_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != msg );
    assert ( NULL != widget );

    data_id_t id;
    data_change_event_type_t evt_type;
    id = data_change_message_get_modified( msg );
    evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE )
    {
        data_change_message_trace( msg );
        /* the type has nothing to commit on db close */
    }

    if (( evt_type == DATA_CHANGE_EVENT_TYPE_DB_CLOSED )
        || ( data_id_equals( &id, &((*this_).selected_object_id) ) ))
    {
        data_change_message_trace( msg );
        gui_textedit_private_load_object( this_, id );
        gui_textedit_private_type_update_view( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_description_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    assert ( NULL != this_ );
    assert ( NULL != msg );
    assert ( NULL != widget );

    data_id_t id;
    data_change_event_type_t evt_type;
    id = data_change_message_get_modified( msg );
    evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE )
    {
        data_change_message_trace( msg );
        gui_textedit_private_description_commit_changes( this_ );
    }

    if (( evt_type == DATA_CHANGE_EVENT_TYPE_DB_CLOSED )
        || ( data_id_equals( &id, &((*this_).selected_object_id) ) ))
    {
        data_change_message_trace( msg );
        gui_textedit_private_load_object( this_, id );
        gui_textedit_private_description_update_view( this_ );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

#endif

/* ================================ PRIVATE METHODS ================================ */

void gui_textedit_private_load_object ( gui_textedit_t *this_, data_id_t id )
{
    TRACE_BEGIN();

    /* before overwriting the current data, trace this_: */
    gui_textedit_trace( this_ );

    switch ( data_id_get_table(&id) )
    {
        case DATA_TABLE_VOID:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
            data_id_reinit_void( &((*this_).selected_object_id) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            /* load the object */
            {
                data_error_t db_err;

                data_classifier_destroy( &((*this_).private_classifier_cache) );
                db_err= data_database_reader_get_classifier_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_classifier_cache) );
                data_id_replace( &((*this_).selected_object_id), &id );

                if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
                {
                    TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading a classifier" );
                    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                         GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                                         GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED,
                                                                         NULL
                    );
                }
                if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED)) )
                {
                    /* error at loading */
                    data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
                    data_id_reinit_void( &((*this_).selected_object_id) );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            /* load the object */
            {
                data_error_t db_err;

                data_feature_destroy( &((*this_).private_feature_cache) );
                db_err= data_database_reader_get_feature_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_feature_cache) );
                data_id_replace( &((*this_).selected_object_id), &id );

                if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
                {
                    TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading a feature" );
                    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                         GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                                         GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED,
                                                                         NULL
                    );
                }
                if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED)) )
                {
                    /* error at loading */
                    data_feature_reinit_empty( &((*this_).private_feature_cache) );
                    data_id_reinit_void( &((*this_).selected_object_id) );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );

            /* load the object */
            {
                data_error_t db_err;

                data_relationship_destroy( &((*this_).private_relationship_cache) );
                db_err= data_database_reader_get_relationship_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_relationship_cache) );
                data_id_replace( &((*this_).selected_object_id), &id );

                if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
                {
                    TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading a relationship" );
                    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                         GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                                         GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED,
                                                                         NULL
                    );
                }
                if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED)) )
                {
                    /* error at loading */
                    data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
                    data_id_reinit_void( &((*this_).selected_object_id) );
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
            data_id_reinit_void( &((*this_).selected_object_id) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            /* load the object */
            {
                data_error_t db_err;

                data_diagram_destroy( &((*this_).private_diagram_cache) );
                db_err= data_database_reader_get_diagram_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_diagram_cache) );
                data_id_replace( &((*this_).selected_object_id), &id );

                if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
                {
                    TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading a diagram" );
                    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                         GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                                         GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED,
                                                                         NULL
                    );
                }
                if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED)) )
                {
                    /* error at loading */
                    data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
                    data_id_reinit_void( &((*this_).selected_object_id) );
                }
            }
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );

            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
            data_id_reinit_void( &((*this_).selected_object_id) );
        }
        break;
    }

    /* after loading the current data, trace this_: */
    gui_textedit_trace( this_ );

    TRACE_END();
}

void gui_textedit_private_name_commit_changes ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    GtkEntry *name_widget;
    const char* text;
    name_widget = GTK_ENTRY( (*this_).name_entry );
    text = gtk_entry_get_text( name_widget );

    TRACE_INFO_STR( "text:", text );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* nothing to do */
            TRACE_INFO( "no object selected where name can be updated." );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            const char* unchanged_text;
            unchanged_text = data_classifier_get_name_ptr( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_classifier_name ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_DUPLICATE_NAME == ctrl_err )
                {
                    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                         GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                         GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,
                                                                         text
                    );
                }
                else if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update name failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const char* unchanged_text;
            unchanged_text = data_feature_get_key_ptr( &((*this_).private_feature_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_feature_key ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update key/name failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            const char* unchanged_text;
            unchanged_text = data_relationship_get_name_ptr( &((*this_).private_relationship_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_relationship_name ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update name failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            TSLOG_WARNING( "no object selected where name can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            const char* unchanged_text;
            unchanged_text = data_diagram_get_name_ptr( &((*this_).private_diagram_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_diagram_controller_update_diagram_name ( diag_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update name failed:", ctrl_err );
                }
            }
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_END();
}

void gui_textedit_private_stereotype_commit_changes ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    GtkEntry *stereotype_widget;
    const char* text;
    stereotype_widget = GTK_ENTRY( (*this_).stereotype_entry );
    text = gtk_entry_get_text( stereotype_widget );

    TRACE_INFO_STR( "text:", text );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* nothing to do */
            TRACE_INFO( "no object selected where stereotype can be updated." );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            const char* unchanged_text;
            unchanged_text = data_classifier_get_stereotype_ptr( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_classifier_stereotype ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update stereotype failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const char* unchanged_text;
            unchanged_text = data_feature_get_value_ptr( &((*this_).private_feature_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_feature_value ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update value/stereotype failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            TRACE_INFO( "no object selected where stereotype can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            TSLOG_WARNING( "no object selected where stereotype can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            TRACE_INFO( "no object selected where stereotype can be updated." );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_END();
}

void gui_textedit_private_type_commit_changes ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    GtkComboBox *type_widget;
    int obj_type;
    int index;
    type_widget = GTK_COMBO_BOX( (*this_).type_combo_box );
    index = gtk_combo_box_get_active ( type_widget );
    obj_type = gtk_helper_tree_model_get_id( gtk_combo_box_get_model( type_widget ), 0, index );

    TRACE_INFO_INT( "obj_type:", obj_type );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* nothing to do */
            TRACE_INFO( "no object selected where type can be updated." );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            data_classifier_type_t unchanged_main_type;
            unchanged_main_type = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
            if ( obj_type != unchanged_main_type )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_classifier_main_type ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update main type failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            data_feature_type_t unchanged_main_type;
            unchanged_main_type = data_feature_get_main_type( &((*this_).private_feature_cache) );
            if ( obj_type != unchanged_main_type )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_feature_main_type ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update main type failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            data_relationship_type_t unchanged_main_type;
            unchanged_main_type = data_relationship_get_main_type( &((*this_).private_relationship_cache) );
            if ( obj_type != unchanged_main_type )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_relationship_main_type ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update main type failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            TSLOG_WARNING( "no object selected where type can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            data_diagram_type_t unchanged_type;
            unchanged_type = data_diagram_get_diagram_type( &((*this_).private_diagram_cache) );
            if ( obj_type != unchanged_type )
            {
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_diagram_controller_update_diagram_type ( diag_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update type failed:", ctrl_err );
                }
            }
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_END();
}

void gui_textedit_private_description_commit_changes ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();
    GtkTextView *description_widget;
    GtkTextBuffer *buffer;
    description_widget = GTK_TEXT_VIEW( (*this_).description_text_view );
    buffer = gtk_text_view_get_buffer ( description_widget );

    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_start_iter ( buffer, &start );
    gtk_text_buffer_get_end_iter ( buffer, &end );

    const char* text;
    text = gtk_text_buffer_get_text ( buffer, &start, &end, false );

    TRACE_INFO_STR( "text:", text );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* nothing to do */
            TRACE_INFO( "no object selected where description can be updated." );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            const char* unchanged_text;
            unchanged_text = data_classifier_get_description_ptr( &((*this_).private_classifier_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_classifier_description ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update description failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            const char* unchanged_text;
            unchanged_text = data_feature_get_description_ptr( &((*this_).private_feature_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_feature_description ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update description failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            const char* unchanged_text;
            unchanged_text = data_relationship_get_description_ptr( &((*this_).private_relationship_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_classifier_controller_t *class_ctrl;
                class_ctrl = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_classifier_controller_update_relationship_description ( class_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update description failed:", ctrl_err );
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            /* (*this_).selected_object_id should not be of type DATA_TABLE_DIAGRAMELEMENT */
            TSLOG_WARNING( "no object selected where description can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            const char* unchanged_text;
            unchanged_text = data_diagram_get_description_ptr( &((*this_).private_diagram_cache) );
            if ( ! utf8string_equals_str( text, unchanged_text ) )
            {
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                ctrl_err = ctrl_diagram_controller_update_diagram_description ( diag_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), text );
                if ( CTRL_ERROR_STRING_BUFFER_EXCEEDED == ctrl_err )
                {
                    gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                    );
                }
                else if ( CTRL_ERROR_NONE != ctrl_err )
                {
                    TSLOG_ERROR_HEX( "update description failed:", ctrl_err );
                }
            }
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_END();
}

void gui_textedit_private_id_update_view ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();
    GtkLabel *id_widget;
    id_widget = GTK_LABEL( (*this_).id_label );

    char data_id_string_buf[DATA_ID_MAX_UTF8STRING_SIZE];
    utf8stringbuf_t data_id_string = UTF8STRINGBUF( data_id_string_buf );
    utf8stringbuf_clear( data_id_string );
    if ( data_id_is_valid( &((*this_).selected_object_id) ) )
    {
        utf8error_t str_err = data_id_to_utf8stringbuf ( &((*this_).selected_object_id), data_id_string );
        if ( UTF8ERROR_SUCCESS != str_err )
        {
            TSLOG_ERROR_HEX( "data_id_to_utf8stringbuf failed:", str_err );
        }
    }
    gtk_label_set_text ( id_widget, utf8stringbuf_get_string( data_id_string ) );

    TRACE_END();
}

void gui_textedit_private_name_update_view ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();
    GtkEntry *name_widget;
    name_widget = GTK_ENTRY( (*this_).name_entry );

    switch ( data_id_get_table( &((*this_).selected_object_id )) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( name_widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show ( GTK_WIDGET ( name_widget ) );

            const char* text;
            text = data_classifier_get_name_ptr( &((*this_).private_classifier_cache) );
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), text );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show ( GTK_WIDGET ( name_widget ) );

            const char* text;
            text = data_feature_get_key_ptr( &((*this_).private_feature_cache) );
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), text );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_show ( GTK_WIDGET ( name_widget ) );

            const char* text;
            text = data_relationship_get_name_ptr( &((*this_).private_relationship_cache) );
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), text );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide ( GTK_WIDGET ( name_widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_show ( GTK_WIDGET ( name_widget ) );

            const char* text;
            text = data_diagram_get_name_ptr( &((*this_).private_diagram_cache) );
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), text );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
            gtk_entry_set_text( GTK_ENTRY ( name_widget ), "" );
        }
        break;
    }

    TRACE_END();
}

void gui_textedit_private_stereotype_update_view ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();
    GtkEntry *stereotype_widget;
    stereotype_widget = GTK_ENTRY( (*this_).stereotype_entry );

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( stereotype_widget ) );
            /*gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );*/
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show( GTK_WIDGET ( stereotype_widget ) );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), true );

            const char* text;
            text = data_classifier_get_stereotype_ptr( &((*this_).private_classifier_cache) );
            gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), text );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show( GTK_WIDGET ( stereotype_widget ) );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), true );

            const char* text;
            text = data_feature_get_value_ptr( &((*this_).private_feature_cache) );
            gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), text );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            /*gtk_widget_hide( GTK_WIDGET ( stereotype_widget ) );*/
            /* -- do not hide - otherwise the user interface looks inhomogenous -- */
            gtk_widget_show( GTK_WIDGET ( stereotype_widget ) );
            gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );

        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide( GTK_WIDGET ( stereotype_widget ) );
            /*gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );*/
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            /*gtk_widget_hide( GTK_WIDGET ( stereotype_widget ) );*/
            /* -- do not hide - otherwise the user interface looks inhomogenous -- */
            gtk_widget_show( GTK_WIDGET ( stereotype_widget ) );
            gtk_entry_set_text( GTK_ENTRY ( stereotype_widget ), "    -- n/a --" );
            gtk_editable_set_editable ( GTK_EDITABLE ( stereotype_widget ), false );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_END();
}

void gui_textedit_private_type_update_view ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();
    GtkComboBox *type_widget;
    type_widget = GTK_COMBO_BOX( (*this_).type_combo_box );

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( (*this_).no_types ) );
            /* prevent that a user accitentially enters a type for a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( type_widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show ( GTK_WIDGET ( type_widget ) );

            data_classifier_type_t class_type;
            class_type = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
            int index;
            index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).classifier_types ), 0, class_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( (*this_).classifier_types ) );
            gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show ( GTK_WIDGET ( type_widget ) );

            data_feature_type_t feature_type;
            feature_type = data_feature_get_main_type( &((*this_).private_feature_cache) );
            if ( DATA_FEATURE_TYPE_LIFELINE == feature_type )
            {
                int index2;
                index2 = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).feature_lifeline_type ), 0, feature_type );
                gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( (*this_).feature_lifeline_type ) );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index2 );
            }
            else
            {
                int index;
                index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).feature_types ), 0, feature_type );
                gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( (*this_).feature_types ) );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index );
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_show ( GTK_WIDGET ( type_widget ) );

            data_relationship_type_t relationship_type;
            relationship_type = data_relationship_get_main_type( &((*this_).private_relationship_cache) );
            int index;
            index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).relationship_types ), 0, relationship_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( (*this_).relationship_types ) );
            gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( (*this_).no_types ) );
            gtk_widget_hide ( GTK_WIDGET ( type_widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_show ( GTK_WIDGET ( type_widget ) );

            data_diagram_type_t diag_type;
            diag_type = data_diagram_get_diagram_type( &((*this_).private_diagram_cache) );
            int index;
            index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).diagram_types ), 0, diag_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( type_widget ), GTK_TREE_MODEL( (*this_).diagram_types ) );
            gtk_combo_box_set_active ( GTK_COMBO_BOX( type_widget ), index );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_END();
}

void gui_textedit_private_description_update_view ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    GtkTextView *description_widget;
    GtkTextBuffer *buffer;
    description_widget = GTK_TEXT_VIEW( (*this_).description_text_view );
    buffer = gtk_text_view_get_buffer ( GTK_TEXT_VIEW( description_widget ) );

    switch ( data_id_get_table( &((*this_).selected_object_id ) ) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( description_widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show ( GTK_WIDGET ( description_widget ) );

            const char* text;
            text = data_classifier_get_description_ptr( &((*this_).private_classifier_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show ( GTK_WIDGET ( description_widget ) );

            const char* text;
            text = data_feature_get_description_ptr( &((*this_).private_feature_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_show ( GTK_WIDGET ( description_widget ) );

            const char* text;
            text = data_relationship_get_description_ptr( &((*this_).private_relationship_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide ( GTK_WIDGET ( description_widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_show ( GTK_WIDGET ( description_widget ) );

            const char* text;
            text = data_diagram_get_description_ptr( &((*this_).private_diagram_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke

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
