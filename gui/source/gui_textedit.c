/* File: gui_textedit.c; Copyright and License: see below */

#include "gui_textedit.h"
#include "util/gtk_helper/gtk_helper_tree_model.h"
#include "trace.h"
#include "data_table.h"
#include "data_id.h"
#include "util/string/utf8string.h"
#include <gtk/gtk.h>
#include <stdbool.h>

void gui_textedit_init ( gui_textedit_t *this_, ctrl_controller_t *controller, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    data_diagram_init_empty( &((*this_).private_diagram_cache) );
    data_classifier_init_empty( &((*this_).private_classifier_cache) );
    data_feature_init_empty( &((*this_).private_feature_cache) );
    data_relationship_init_empty( &((*this_).private_relationship_cache) );
    data_id_init_void( &((*this_).selected_object_id) );

    {
        GtkTreeIter iter;
        (*this_).diagram_types = gtk_list_store_new( 2, G_TYPE_INT, G_TYPE_STRING );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_LIST, 1, "LIST", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_BLOCK_DIAGRAM, 1, "BLOCK_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_REQUIREMENT_DIAGRAM, 1, "REQUIREMENT_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM, 1, "UML_USE_CASE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_ACTIVITY_DIAGRAM, 1, "UML_ACTIVITY_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_STATE_MACHINE_DIAGRAM, 1, "UML_STATE_MACHINE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM, 1, "UML_SEQUENCE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM, 1, "UML_COMMUNICATION_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_INTERACTION_OVERVIEW_DIAGRAM, 1, "UML_INTERACTION_OVERVIEW_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM, 1, "UML_TIMING_DIAGRAM", -1 );
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
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, DATA_DIAGRAM_TYPE_UML_PROFILE_DIAGRAM, 1, "UML_PROFILE_DIAGRAM", -1 );
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
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTOR, 1, "UML_ACTOR", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_USE_CASE, 1, "UML_USE_CASE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ACTIVITY, 1, "UML_ACTIVITY", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_STATE, 1, "UML_STATE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE, 1, "UML_DIAGRAM_REFERENCE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_NODE, 1, "UML_NODE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMPONENT, 1, "UML_COMPONENT", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PART, 1, "UML_PART", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_INTERFACE, 1, "UML_INTERFACE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_PACKAGE, 1, "UML_PACKAGE", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_CLASS, 1, "UML_CLASS", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_OBJECT, 1, "UML_OBJECT", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_ARTIFACT, 1, "UML_ARTIFACT", -1 );
        gtk_list_store_append( (*this_).classifier_types, &iter);
        gtk_list_store_set ( (*this_).classifier_types, &iter, 0, DATA_CLASSIFIER_TYPE_UML_COMMENT, 1, "UML_COMMENT", -1 );
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

    TRACE_END();
}

gboolean gui_textedit_name_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    const char* text;
    text = gtk_entry_get_text( GTK_ENTRY ( widget ) );

    TRACE_INFO_STR( "text:", text );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
            LOG_WARNING( "no object selected where name can be updated." );
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
                    if ( CTRL_ERROR_NONE != ctrl_err )
                    {
                        LOG_ERROR_HEX( "update name failed:", ctrl_err );
                    }
                }
            }
            break;
        case DATA_TABLE_FEATURE:
            LOG_ERROR( "not yet implemented." );
            break;
        case DATA_TABLE_RELATIONSHIP:
            LOG_ERROR( "not yet implemented." );
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            LOG_WARNING( "no object selected where name can be updated." );
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
                    if ( CTRL_ERROR_NONE != ctrl_err )
                    {
                        LOG_ERROR_HEX( "update name failed:", ctrl_err );
                    }
                }
            }
            break;
        default:
            LOG_ERROR( "invalid data in data_id_t." );
            break;
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

gboolean gui_textedit_description_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer ( GTK_TEXT_VIEW( widget ) );

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
            LOG_WARNING( "no object selected where description can be updated." );
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
                    if ( CTRL_ERROR_NONE != ctrl_err )
                    {
                        LOG_ERROR_HEX( "update name failed:", ctrl_err );
                    }
                }
            }
            break;
        case DATA_TABLE_FEATURE:
            LOG_ERROR( "not yet implemented." );
            break;
        case DATA_TABLE_RELATIONSHIP:
            LOG_ERROR( "not yet implemented." );
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            LOG_WARNING( "no object selected where description can be updated." );
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
                    if ( CTRL_ERROR_NONE != ctrl_err )
                    {
                        LOG_ERROR_HEX( "update description failed:", ctrl_err );
                    }
                }
            }
            break;
        default:
            LOG_ERROR( "invalid data in data_id_t." );
            break;
    }

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

gboolean gui_textedit_stereotype_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    const char* text;
    text = gtk_entry_get_text( GTK_ENTRY ( widget ) );

    TRACE_INFO_STR( "text:", text );

    ctrl_error_t ctrl_err;
    switch ( data_id_get_table( &((*this_).selected_object_id) ) )
    {
        case DATA_TABLE_VOID:
            LOG_WARNING( "no object selected where stereotype can be updated." );
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
                    if ( CTRL_ERROR_NONE != ctrl_err )
                    {
                        LOG_ERROR_HEX( "update stereotype failed:", ctrl_err );
                    }
                }
            }
            break;
        case DATA_TABLE_FEATURE:
            LOG_ERROR( "not yet implemented." );
            break;
        case DATA_TABLE_RELATIONSHIP:
            LOG_ERROR( "not yet implemented." );
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            LOG_WARNING( "no object selected where stereotype can be updated." );
            break;
        case DATA_TABLE_DIAGRAM:
            LOG_WARNING( "no object selected where stereotype can be updated." );
            break;
        default:
            LOG_ERROR( "invalid data in data_id_t." );
            break;
    }

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
            LOG_WARNING( "no object selected where type can be updated." );
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
                        LOG_ERROR_HEX( "update main type failed:", ctrl_err );
                    }
                }
            }
            break;
        case DATA_TABLE_FEATURE:
            LOG_ERROR( "not yet implemented." );
            break;
        case DATA_TABLE_RELATIONSHIP:
            LOG_ERROR( "not yet implemented." );
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            LOG_WARNING( "no object selected where type can be updated." );
            break;
        case DATA_TABLE_DIAGRAM:
            {
                data_diagram_type_t unchanged_type;
                unchanged_type = data_diagram_get_type( &((*this_).private_diagram_cache) );
                if ( obj_type != unchanged_type )
                {
                    ctrl_diagram_controller_t *diag_ctrl;
                    diag_ctrl = ctrl_controller_get_diagram_control_ptr ( (*this_).controller );

                    ctrl_err = ctrl_diagram_controller_update_diagram_type ( diag_ctrl, data_id_get_row_id( &((*this_).selected_object_id) ), obj_type );
                    if ( CTRL_ERROR_NONE != ctrl_err )
                    {
                        LOG_ERROR_HEX( "update type failed:", ctrl_err );
                    }
                }
            }
            break;
        default:
            LOG_ERROR( "invalid data in data_id_t." );
            break;
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_textedit_commit_clicked_callback (GtkButton *button, gpointer user_data )
{
    TRACE_BEGIN();

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
            gtk_entry_set_text( GTK_ENTRY ( widget ), "" );
            break;
        case DATA_TABLE_CLASSIFIER:
            {
                const char* text;
                text = data_classifier_get_name_ptr( &((*this_).private_classifier_cache) );
                gtk_entry_set_text( GTK_ENTRY ( widget ), text );
            }
            break;
        case DATA_TABLE_FEATURE:
            gtk_entry_set_text( GTK_ENTRY ( widget ), "" );
            break;
        case DATA_TABLE_RELATIONSHIP:
            gtk_entry_set_text( GTK_ENTRY ( widget ), "" );
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            gtk_entry_set_text( GTK_ENTRY ( widget ), "" );
            break;
        case DATA_TABLE_DIAGRAM:
            {
                const char* text;
                text = data_diagram_get_name_ptr( &((*this_).private_diagram_cache) );
                gtk_entry_set_text( GTK_ENTRY ( widget ), text );
            }
            break;
        default:
            LOG_ERROR( "invalid data in data_id_t." );
            gtk_entry_set_text( GTK_ENTRY ( widget ), "" );
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
            gtk_widget_hide( GTK_WIDGET ( widget ) );
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
            gtk_widget_hide( GTK_WIDGET ( widget ) );
            break;
        case DATA_TABLE_RELATIONSHIP:
            gtk_widget_hide( GTK_WIDGET ( widget ) );
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            gtk_widget_hide( GTK_WIDGET ( widget ) );
            break;
        case DATA_TABLE_DIAGRAM:
            gtk_widget_hide( GTK_WIDGET ( widget ) );
            break;
        default:
            LOG_ERROR( "invalid data in data_id_t." );
            gtk_widget_hide( GTK_WIDGET ( widget ) );
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
            gtk_text_buffer_set_text ( buffer, "", -1 /*len*/ );
            break;
        case DATA_TABLE_CLASSIFIER:
            {
                const char* text;
                text = data_classifier_get_description_ptr( &((*this_).private_classifier_cache) );

                gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
            }
            break;
        case DATA_TABLE_FEATURE:
            gtk_text_buffer_set_text ( buffer, "", -1 /*len*/ );
            break;
        case DATA_TABLE_RELATIONSHIP:
            gtk_text_buffer_set_text ( buffer, "", -1 /*len*/ );
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            gtk_text_buffer_set_text ( buffer, "", -1 /*len*/ );
            break;
        case DATA_TABLE_DIAGRAM:
            {
                const char* text;
                text = data_diagram_get_description_ptr( &((*this_).private_diagram_cache) );

                gtk_text_buffer_set_text ( buffer, text, -1 /*len*/ );
            }
            break;
        default:
            LOG_ERROR( "invalid data in data_id_t." );
            gtk_text_buffer_set_text ( buffer, "", -1 /*len*/ );
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
            break;
        case DATA_TABLE_CLASSIFIER:
            {
                data_classifier_type_t class_type;
                class_type = data_classifier_get_main_type( &((*this_).private_classifier_cache) );
                int index;
                index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).classifier_types ), 0, class_type );
                gtk_combo_box_set_model( GTK_COMBO_BOX( widget ), GTK_TREE_MODEL( (*this_).classifier_types ) );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
            }
            break;
        case DATA_TABLE_FEATURE:
            break;
        case DATA_TABLE_RELATIONSHIP:
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            break;
        case DATA_TABLE_DIAGRAM:
            {
                data_diagram_type_t diag_type;
                diag_type = data_diagram_get_type( &((*this_).private_diagram_cache) );
                int index;
                index = gtk_helper_tree_model_get_index( GTK_TREE_MODEL( (*this_).diagram_types ), 0, diag_type );
                gtk_combo_box_set_model( GTK_COMBO_BOX( widget ), GTK_TREE_MODEL( (*this_).diagram_types ) );
                gtk_combo_box_set_active ( GTK_COMBO_BOX( widget ), index );
            }
            break;
        default:
            LOG_ERROR( "invalid data in data_id_t." );
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
                data_diagram_destroy( &((*this_).private_diagram_cache) );
                data_classifier_destroy( &((*this_).private_classifier_cache) );
                data_feature_destroy( &((*this_).private_feature_cache) );
                data_relationship_destroy( &((*this_).private_relationship_cache) );

                data_diagram_init_empty( &((*this_).private_diagram_cache) );
                data_classifier_init_empty( &((*this_).private_classifier_cache) );
                data_feature_init_empty( &((*this_).private_feature_cache) );
                data_relationship_init_empty( &((*this_).private_relationship_cache) );
            }
            break;
        case DATA_TABLE_CLASSIFIER:
            {
                data_diagram_destroy( &((*this_).private_diagram_cache) );
                data_feature_destroy( &((*this_).private_feature_cache) );
                data_relationship_destroy( &((*this_).private_relationship_cache) );

                data_diagram_init_empty( &((*this_).private_diagram_cache) );
                data_feature_init_empty( &((*this_).private_feature_cache) );
                data_relationship_init_empty( &((*this_).private_relationship_cache) );

                if ( force_reload || ( data_classifier_get_id( &((*this_).private_classifier_cache) ) != data_id_get_row_id(&id) ))
                {
                    data_error_t db_err;

                    data_classifier_destroy( &((*this_).private_classifier_cache) );
                    db_err= data_database_reader_get_classifier_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_classifier_cache) );
                    if ( DATA_ERROR_NONE != db_err )
                    {
                        /* error at loading */
                        data_classifier_destroy( &((*this_).private_classifier_cache) );
                        data_classifier_init_empty( &((*this_).private_classifier_cache) );
                    }
                }
            }
            break;
        case DATA_TABLE_FEATURE:
            {
                data_diagram_destroy( &((*this_).private_diagram_cache) );
                data_classifier_destroy( &((*this_).private_classifier_cache) );
                data_feature_destroy( &((*this_).private_feature_cache) );
                data_relationship_destroy( &((*this_).private_relationship_cache) );

                data_diagram_init_empty( &((*this_).private_diagram_cache) );
                data_classifier_init_empty( &((*this_).private_classifier_cache) );
                data_feature_init_empty( &((*this_).private_feature_cache) );
                data_relationship_init_empty( &((*this_).private_relationship_cache) );
            }
            break;
        case DATA_TABLE_RELATIONSHIP:
            {
                data_diagram_destroy( &((*this_).private_diagram_cache) );
                data_classifier_destroy( &((*this_).private_classifier_cache) );
                data_feature_destroy( &((*this_).private_feature_cache) );
                data_relationship_destroy( &((*this_).private_relationship_cache) );

                data_diagram_init_empty( &((*this_).private_diagram_cache) );
                data_classifier_init_empty( &((*this_).private_classifier_cache) );
                data_feature_init_empty( &((*this_).private_feature_cache) );
                data_relationship_init_empty( &((*this_).private_relationship_cache) );
            }
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            {
                data_diagram_destroy( &((*this_).private_diagram_cache) );
                data_classifier_destroy( &((*this_).private_classifier_cache) );
                data_feature_destroy( &((*this_).private_feature_cache) );
                data_relationship_destroy( &((*this_).private_relationship_cache) );

                data_diagram_init_empty( &((*this_).private_diagram_cache) );
                data_classifier_init_empty( &((*this_).private_classifier_cache) );
                data_feature_init_empty( &((*this_).private_feature_cache) );
                data_relationship_init_empty( &((*this_).private_relationship_cache) );
            }
            break;
        case DATA_TABLE_DIAGRAM:
            {
                data_classifier_destroy( &((*this_).private_classifier_cache) );
                data_feature_destroy( &((*this_).private_feature_cache) );
                data_relationship_destroy( &((*this_).private_relationship_cache) );

                data_classifier_init_empty( &((*this_).private_classifier_cache) );
                data_feature_init_empty( &((*this_).private_feature_cache) );
                data_relationship_init_empty( &((*this_).private_relationship_cache) );

                if ( force_reload || ( data_diagram_get_id( &((*this_).private_diagram_cache) ) != data_id_get_row_id(&id) ))
                {
                    data_error_t db_err;

                    data_diagram_destroy( &((*this_).private_diagram_cache) );
                    db_err= data_database_reader_get_diagram_by_id ( (*this_).db_reader, data_id_get_row_id(&id), &((*this_).private_diagram_cache) );
                    if ( DATA_ERROR_NONE != db_err )
                    {
                        /* error at loading */
                        data_diagram_destroy( &((*this_).private_diagram_cache) );
                        data_diagram_init_empty( &((*this_).private_diagram_cache) );
                    }
                }
            }
            break;
        default:
            {
                LOG_ERROR( "invalid data in data_id_t." );

                data_diagram_destroy( &((*this_).private_diagram_cache) );
                data_classifier_destroy( &((*this_).private_classifier_cache) );
                data_feature_destroy( &((*this_).private_feature_cache) );
                data_relationship_destroy( &((*this_).private_relationship_cache) );

                data_classifier_init_empty( &((*this_).private_classifier_cache) );
                data_diagram_init_empty( &((*this_).private_diagram_cache) );
                data_feature_init_empty( &((*this_).private_feature_cache) );
                data_relationship_init_empty( &((*this_).private_relationship_cache) );
            }
            break;
    }

    data_id_destroy( &((*this_).selected_object_id) );
    (*this_).selected_object_id = id;

    TRACE_END();
}


/*
Copyright 2016-2016 Andreas Warnke

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
