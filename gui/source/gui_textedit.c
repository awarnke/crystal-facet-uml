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
                         GtkEntry *name_entry,
                         GtkTextView *description_text_view,
                         GtkEntry *stereotype_entry,
                         ctrl_controller_t *controller,
                         data_database_reader_t *db_reader,
                         data_database_t *database,
                         gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();
    assert( NULL != name_entry );
    assert( NULL != description_text_view );
    assert( NULL != stereotype_entry );

    assert( NULL != controller );
    assert( NULL != db_reader );
    assert( NULL != message_to_user );

    (*this_).name_entry = name_entry;
    (*this_).description_text_view = description_text_view;
    (*this_).stereotype_entry = stereotype_entry;
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
        (*this_).diagram_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_LIST, 1, "LIST (not yet implemented)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_BOX_DIAGRAM, 1, "BOX_DIAGRAM (not yet implemented)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_BLOCK_DEFINITION_DIAGRAM, 1, "SYSML_BLOCK_DEFINITION_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_INTERNAL_BLOCK_DIAGRAM, 1, "SYSML_INTERNAL_BLOCK_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_PARAMETRIC_DIAGRAM, 1, "SYSML_PARAMETRIC_DIAGRAM (not yet implemented)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_SYSML_REQUIREMENTS_DIAGRAM, 1, "SYSML_REQUIREMENTS_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, 1, "UML_USE_CASE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, 1, "UML_ACTIVITY_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM, 1, "UML_STATE_MACHINE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM, 1, "UML_SEQUENCE_DIAGRAM (not yet implemented)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM, 1, "UML_COMMUNICATION_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM, 1, "UML_INTERACTION_OVERVIEW_DIAGRAM (not yet implemented)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM, 1, "UML_TIMING_DIAGRAM (not yet implemented)", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_DEPLOYMENT_DIAGRAM, 1, "UML_DEPLOYMENT_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMPONENT_DIAGRAM, 1, "UML_COMPONENT_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMPOSITE_STRUCTURE_DIAGRAM, 1, "UML_COMPOSITE_STRUCTURE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PACKAGE_DIAGRAM, 1, "UML_PACKAGE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM, 1, "UML_CLASS_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM, 1, "UML_PROFILE_DIAGRAM (not yet implemented)", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).classifier_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_BLOCK, 1, "BLOCK", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_FEATURE, 1, "FEATURE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_REQUIREMENT, 1, "REQUIREMENT", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTOR, 1, "UML_ACTOR (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_USE_CASE, 1, "UML_USE_CASE (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY, 1, "UML_SYSTEM_BOUNDARY (use case)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTIVITY, 1, "UML_ACTIVITY", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_STATE, 1, "UML_STATE", -1 );
        /*
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE, 1, "UML_DIAGRAM_REFERENCE", -1 );
        --> t.b.d: exists in uml but how to handle this in crystal facet uml?
        */
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_NODE, 1, "UML_NODE (deployment)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMPONENT, 1, "UML_COMPONENT", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PART, 1, "UML_PART (component instance)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_INTERFACE, 1, "UML_INTERFACE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PACKAGE, 1, "UML_PACKAGE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_CLASS, 1, "UML_CLASS", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_OBJECT, 1, "UML_OBJECT (class instance)", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ARTIFACT, 1, "UML_ARTIFACT", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMMENT, 1, "UML_COMMENT", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).feature_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PROPERTY, 1, "PROPERTY (class,block)", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_OPERATION, 1, "OPERATION (class)", -1 );
        gtk_list_store_append( (*this_).feature_types, &iter);
        gtk_list_store_set ( (*this_).feature_types, &iter, 0, DATA_FEATURE_TYPE_PORT, 1, "PORT (component,block)", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).feature_lifeline_type = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).feature_lifeline_type, &iter);
        gtk_list_store_set ( (*this_).feature_lifeline_type, &iter, 0, DATA_FEATURE_TYPE_LIFELINE, 1, "LIFELINE (sequence,timing)", -1 );
    }

    {
        GtkTreeIter iter;
        (*this_).relationship_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY, 1, "UML_DEPENDENCY", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION, 1, "UML_ASSOCIATION (class, use-case)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_AGGREGATION, 1, "UML_AGGREGATION (class, no circles)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, 1, "UML_COMPOSITION (class, no circles)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION, 1, "UML_GENERALIZATION (class, no circles)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_REALIZATION, 1, "UML_REALIZATION (class, no circles)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT, 1, "UML_CONTAINMENT (package, no cicles)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL, 1, "UML_ASYNC_CALL (sequence)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL, 1, "UML_SYNC_CALL (sequence)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL, 1, "UML_RETURN_CALL (sequence)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH, 1, "UML_COMMUNICATION_PATH (component)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW, 1, "UML_CONTROL_FLOW (activity)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW, 1, "UML_OBJECT_FLOW (activity)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_DEPLOY, 1, "UML_DEPLOY (deployment, no circles)", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_MANIFEST, 1, "UML_MANIFEST (deployment), no circles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_EXTEND, 1, "UML_EXTEND (use-case), no circles", -1 );
        gtk_list_store_append( (*this_).relationship_types, &iter);
        gtk_list_store_set ( (*this_).relationship_types, &iter, 0, DATA_RELATIONSHIP_TYPE_UML_INCLUDE, 1, "UML_INCLUDE (use-case), no circles", -1 );
    }

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

    (*this_).name_entry = NULL;
    (*this_).description_text_view = NULL;
    (*this_).stereotype_entry = NULL;

    TRACE_END();
}

void gui_textedit_private_name_commit_changes ( gui_textedit_t *this_, GtkEntry *name_widget )
{
    TRACE_BEGIN();

    const char* text;
    text = gtk_entry_get_text( name_widget );

    TRACE_INFO_STR( "text:", text );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            TSLOG_WARNING( "no object selected where name can be updated." );
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

gboolean gui_textedit_name_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    gui_textedit_private_name_commit_changes( this_,  GTK_ENTRY ( widget ) );

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

void gui_textedit_private_description_commit_changes ( gui_textedit_t *this_, GtkTextView *description_widget )
{
    TRACE_BEGIN();
    GtkTextBuffer *buffer;
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
            TSLOG_WARNING( "no object selected where description can be updated." );
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

gboolean gui_textedit_description_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    gui_textedit_private_description_commit_changes( this_, GTK_TEXT_VIEW( widget ) );

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

void gui_textedit_private_stereotype_commit_changes ( gui_textedit_t *this_, GtkEntry *stereotype_widget )
{
    TRACE_BEGIN();

    const char* text;
    text = gtk_entry_get_text( stereotype_widget );

    TRACE_INFO_STR( "text:", text );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            TSLOG_WARNING( "no object selected where stereotype can be updated." );
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
            TSLOG_WARNING( "no object selected where stereotype can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            TSLOG_WARNING( "no object selected where stereotype can be updated." );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            TSLOG_WARNING( "no object selected where stereotype can be updated." );
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

gboolean gui_textedit_stereotype_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    gui_textedit_private_stereotype_commit_changes( this_, GTK_ENTRY ( widget ) );

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

void gui_textedit_type_changed_callback ( GtkComboBox *widget, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    data_diagram_type_t obj_type;
    int index;
    index = gtk_combo_box_get_active ( GTK_COMBO_BOX( widget ) );
    obj_type = gtk_helper_tree_model_get_id( gtk_combo_box_get_model( GTK_COMBO_BOX( widget ) ), 0, index );

    TRACE_INFO_INT( "obj_type:", obj_type );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
        {
            TSLOG_WARNING( "no object selected where type can be updated." );
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

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_commit_clicked_callback (GtkButton *button, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    gui_textedit_private_name_commit_changes ( this_, (*this_).name_entry );
    gui_textedit_private_stereotype_commit_changes ( this_, (*this_).stereotype_entry );
    gui_textedit_private_description_commit_changes ( this_, (*this_).description_text_view );

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

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_name_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    /* load data */
    data_id_trace( id );
    gui_textedit_private_load_object( this_, *id, false /* force_reload */ );

    switch ( data_id_get_table(id) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_classifier_get_name_ptr( &((*this_).private_classifier_cache) );
            gtk_entry_set_text( GTK_ENTRY ( widget ), text );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_feature_get_key_ptr( &((*this_).private_feature_cache) );
            gtk_entry_set_text( GTK_ENTRY ( widget ), text );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_relationship_get_name_ptr( &((*this_).private_relationship_cache) );
            gtk_entry_set_text( GTK_ENTRY ( widget ), text );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide ( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_diagram_get_name_ptr( &((*this_).private_diagram_cache) );
            gtk_entry_set_text( GTK_ENTRY ( widget ), text );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
            gtk_entry_set_text( GTK_ENTRY ( widget ), "" );
        }
        break;
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_stereotype_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    /* load data */
    data_id_trace( id );
    gui_textedit_private_load_object( this_, *id, false /* force_reload */ );

    switch ( data_id_get_table(id) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_classifier_get_stereotype_ptr( &((*this_).private_classifier_cache) );
            gtk_entry_set_text( GTK_ENTRY ( widget ), text );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_feature_get_value_ptr( &((*this_).private_feature_cache) );
            gtk_entry_set_text( GTK_ENTRY ( widget ), text );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_hide( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_hide( GTK_WIDGET ( widget ) );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_description_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer ( GTK_TEXT_VIEW( widget ) );

    /* load data */
    data_id_trace( id );
    gui_textedit_private_load_object( this_, *id, false /* force_reload */ );

    switch ( data_id_get_table(id) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_classifier_get_description_ptr( &((*this_).private_classifier_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_feature_get_description_ptr( &((*this_).private_feature_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            const char* text;
            text = data_relationship_get_description_ptr( &((*this_).private_relationship_cache) );
            gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide ( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

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

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_type_selected_object_changed_callback( GtkWidget *widget, data_id_t *id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    /* load data */
    data_id_trace( id );
    gui_textedit_private_load_object( this_, *id, false /* force_reload */ );

    switch ( data_id_get_table(id) )
    {
        case DATA_TABLE_VOID:
        {
            /* prevent that a user accitentially enters text to a non-existing object */
            gtk_widget_hide ( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            data_classifier_type_t class_type;
            class_type = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
            int index;
            index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).classifier_types ), 0, class_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( widget ), GTK_TREE_MODEL( (*this_).classifier_types ) );
            gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            data_feature_type_t feature_type;
            feature_type = data_feature_get_main_type( &((*this_).private_feature_cache) );
            if ( DATA_FEATURE_TYPE_LIFELINE == feature_type )
            {
                int index2;
                index2 = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).feature_lifeline_type ), 0, feature_type );
                gtk_combo_box_set_model( GTK_COMBO_BOX( widget ), GTK_TREE_MODEL( (*this_).feature_lifeline_type ) );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index2 );
            }
            else
            {
                int index;
                index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).feature_types ), 0, feature_type );
                gtk_combo_box_set_model( GTK_COMBO_BOX( widget ), GTK_TREE_MODEL( (*this_).feature_types ) );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            data_relationship_type_t relationship_type;
            relationship_type = data_relationship_get_main_type( &((*this_).private_relationship_cache) );
            int index;
            index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).relationship_types ), 0, relationship_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( widget ), GTK_TREE_MODEL( (*this_).relationship_types ) );
            gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            gtk_widget_hide ( GTK_WIDGET ( widget ) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            gtk_widget_show ( GTK_WIDGET ( widget ) );

            data_diagram_type_t diag_type;
            diag_type = data_diagram_get_diagram_type( &((*this_).private_diagram_cache) );
            int index;
            index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).diagram_types ), 0, diag_type );
            gtk_combo_box_set_model( GTK_COMBO_BOX( widget ), GTK_TREE_MODEL( (*this_).diagram_types ) );
            gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid data in data_id_t." );
        }
        break;
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_private_load_object ( gui_textedit_t *this_, data_id_t id, bool force_reload )
{
    TRACE_BEGIN();

    if ( ! data_id_equals( &((*this_).selected_object_id), &id ) )
    {
        force_reload = true;
    }

    switch ( data_id_get_table(&id) )
    {
        case DATA_TABLE_VOID:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
        }
        break;

        case DATA_TABLE_CLASSIFIER:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            if ( force_reload || ( data_classifier_get_id( &((*this_).private_classifier_cache) ) != data_id_get_row_id(&id) ))
            {
                data_error_t db_err;

                data_classifier_destroy( &((*this_).private_classifier_cache) );
                db_err= data_database_reader_get_classifier_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_classifier_cache) );

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
                }
            }
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            if ( force_reload || ( data_feature_get_id( &((*this_).private_feature_cache) ) != data_id_get_row_id(&id) ))
            {
                data_error_t db_err;

                data_feature_destroy( &((*this_).private_feature_cache) );
                db_err= data_database_reader_get_feature_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_feature_cache) );

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
                }
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );

            if ( force_reload || ( data_relationship_get_id( &((*this_).private_relationship_cache) ) != data_id_get_row_id(&id) ))
            {
                data_error_t db_err;

                data_relationship_destroy( &((*this_).private_relationship_cache) );
                db_err= data_database_reader_get_relationship_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_relationship_cache) );

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
                }
            }
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            data_diagram_reinit_empty( &((*this_).private_diagram_cache) );
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            data_classifier_reinit_empty( &((*this_).private_classifier_cache) );
            data_feature_reinit_empty( &((*this_).private_feature_cache) );
            data_relationship_reinit_empty( &((*this_).private_relationship_cache) );

            if ( force_reload || ( data_diagram_get_id( &((*this_).private_diagram_cache) ) != data_id_get_row_id(&id) ))
            {
                data_error_t db_err;

                data_diagram_destroy( &((*this_).private_diagram_cache) );
                db_err= data_database_reader_get_diagram_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_diagram_cache) );

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
        }
        break;
    }

    data_id_destroy( &((*this_).selected_object_id) );
    (*this_).selected_object_id = id;

    TRACE_END();
}

void gui_textedit_name_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    data_id_t id;
    id = data_change_message_get_modified( msg );

    if ( data_id_equals( &id, &((*this_).selected_object_id) ) )
    {
        /* reload currently visible data */
        data_id_trace( &id );
        gui_textedit_private_load_object( this_, id, true /* force_reload */ );

        /* update data */
        switch ( data_id_get_table(&id) )
        {
            case DATA_TABLE_VOID:
            {
            }
            break;

            case DATA_TABLE_CLASSIFIER:
            {
                const char* text;
                text = data_classifier_get_name_ptr( &((*this_).private_classifier_cache) );
                gtk_entry_set_text( GTK_ENTRY ( widget ), text );
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                const char* text;
                text = data_feature_get_key_ptr( &((*this_).private_feature_cache) );
                gtk_entry_set_text( GTK_ENTRY ( widget ), text );
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                const char* text;
                text = data_relationship_get_name_ptr( &((*this_).private_relationship_cache) );
                gtk_entry_set_text( GTK_ENTRY ( widget ), text );
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
                const char* text;
                text = data_diagram_get_name_ptr( &((*this_).private_diagram_cache) );
                gtk_entry_set_text( GTK_ENTRY ( widget ), text );
            }
            break;

            default:
            {
                TSLOG_ERROR( "invalid data in data_id_t." );
            }
            break;
        }
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_stereotype_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    data_id_t id;
    id = data_change_message_get_modified( msg );

    if ( data_id_equals( &id, &((*this_).selected_object_id) ) )
    {
        /* reload currently visible data */
        data_id_trace( &id );
        gui_textedit_private_load_object( this_, id, true /* force_reload */ );

        /* update data */
        switch ( data_id_get_table(&id) )
        {
            case DATA_TABLE_VOID:
            {
            }
            break;

            case DATA_TABLE_CLASSIFIER:
            {
                const char* text;
                text = data_classifier_get_stereotype_ptr( &((*this_).private_classifier_cache) );
                gtk_entry_set_text( GTK_ENTRY ( widget ), text );
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                const char* text;
                text = data_feature_get_value_ptr( &((*this_).private_feature_cache) );
                gtk_entry_set_text( GTK_ENTRY ( widget ), text );
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
            }
            break;

            default:
            {
                TSLOG_ERROR( "invalid data in data_id_t." );
            }
            break;
        }
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_description_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    data_id_t id;
    id = data_change_message_get_modified( msg );
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer ( GTK_TEXT_VIEW( widget ) );

    if ( data_id_equals( &id, &((*this_).selected_object_id) ) )
    {
        /* reload currently visible data */
        data_id_trace( &id );
        gui_textedit_private_load_object( this_, id, true /* force_reload */ );

        /* update data */
        switch ( data_id_get_table(&id) )
        {
            case DATA_TABLE_VOID:
            {
            }
            break;

            case DATA_TABLE_CLASSIFIER:
            {
                const char* text;
                text = data_classifier_get_description_ptr( &((*this_).private_classifier_cache) );

                gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                const char* text;
                text = data_feature_get_description_ptr( &((*this_).private_feature_cache) );

                gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                const char* text;
                text = data_relationship_get_description_ptr( &((*this_).private_relationship_cache) );

                gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
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
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_type_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    data_id_t id;
    id = data_change_message_get_modified( msg );

    if ( data_id_equals( &id, &((*this_).selected_object_id) ) )
    {
        /* reload currently visible data */
        data_id_trace( &id );
        gui_textedit_private_load_object( this_, id, true /* force_reload */ );

        /* update data */
        switch ( data_id_get_table(&id) )
        {
            case DATA_TABLE_VOID:
            {
            }
            break;

            case DATA_TABLE_CLASSIFIER:
            {
                data_classifier_type_t class_type;
                class_type = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
                int index;
                index = gtk_helper_tree_model_get_index( gtk_combo_box_get_model( GTK_COMBO_BOX( widget ) ), 0, class_type );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                data_feature_type_t feature_type;
                feature_type = data_feature_get_main_type( &((*this_).private_feature_cache) );
                int index;
                index = gtk_helper_tree_model_get_index( gtk_combo_box_get_model( GTK_COMBO_BOX( widget ) ), 0, feature_type );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                data_relationship_type_t relationship_type;
                relationship_type = data_relationship_get_main_type( &((*this_).private_relationship_cache) );
                int index;
                index = gtk_helper_tree_model_get_index( gtk_combo_box_get_model( GTK_COMBO_BOX( widget ) ), 0, relationship_type );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
                data_diagram_type_t diag_type;
                diag_type = data_diagram_get_diagram_type( &((*this_).private_diagram_cache) );
                int index;
                index = gtk_helper_tree_model_get_index( gtk_combo_box_get_model( GTK_COMBO_BOX( widget ) ), 0, diag_type );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
            }
            break;

            default:
            {
                TSLOG_ERROR( "invalid data in data_id_t." );
            }
            break;
        }
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}


/*
Copyright 2016-2018 Andreas Warnke

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
