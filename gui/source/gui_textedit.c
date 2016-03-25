/* File: gui_textedit.c; Copyright and License: see below */

#include "gui_textedit.h"
#include "trace.h"
#include "data_table.h"
#include <gtk/gtk.h>
#include <stdbool.h>

void gui_textedit_init ( gui_textedit_t *this_, ctrl_controller_t *controller, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    data_diagram_init_empty( &((*this_).private_diagram_cache) );

    {
        GtkTreeIter iter;
        (*this_).diagram_types = gtk_list_store_new( 1, G_TYPE_STRING );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "LIST", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "BLOCK_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "REQUIREMENT_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_USE_CASE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_ACTIVITY_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_STATE_MACHINE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_SEQUENCE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_COMMUNICATION_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_INTERACTION_OVERVIEW_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_TIMING_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_DEPLOYMENT_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_COMPONENT_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_COMPOSITE_STRUCTURE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_PACKAGE_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_CLASS_DIAGRAM", -1 );
        gtk_list_store_append( (*this_).diagram_types, &iter);
        gtk_list_store_set ( (*this_).diagram_types, &iter, 0, "UML_PROFILE_DIAGRAM", -1 );
    }

    TRACE_END();
}

void gui_textedit_destroy ( gui_textedit_t *this_ )
{
    TRACE_BEGIN();

    data_diagram_destroy( &((*this_).private_diagram_cache) );

    g_object_unref((*this_).diagram_types);
    (*this_).diagram_types = NULL;

    TRACE_END();
}

gboolean gui_textedit_name_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

gboolean gui_textedit_description_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

gboolean gui_textedit_stereotype_focus_lost_callback ( GtkWidget *widget, GdkEvent *event, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    TRACE_END();
    return false;  /* all callbacks shall receive this signal */
}

void gui_textedit_type_changed_callback ( GtkComboBox *widget, gpointer user_data )
{
    TRACE_BEGIN();
    gui_textedit_t *this_;
    this_ = (gui_textedit_t*) user_data;

    TRACE_END();
}

void gui_textedit_commit_clicked_callback (GtkButton *button, gpointer user_data )
{
    TRACE_BEGIN();

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
            gtk_entry_set_text( GTK_ENTRY ( widget ), "" );
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
            gtk_entry_set_text( GTK_ENTRY ( widget ), "" );
            break;
    }

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
            gtk_widget_hide( GTK_WIDGET ( widget ) );
            break;
    }

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
            gtk_text_buffer_set_text ( buffer, "", -1 /*len*/ );
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
            gtk_text_buffer_set_text ( buffer, "", -1 /*len*/ );
            break;
    }

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
            break;
        case DATA_TABLE_FEATURE:
            break;
        case DATA_TABLE_RELATIONSHIP:
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            break;
        case DATA_TABLE_DIAGRAM:
            {
            }
            break;
        default:
            break;
    }

    TRACE_END();
}

void gui_textedit_private_load_object ( gui_textedit_t *this_, data_id_t id, bool force_reload )
{
    TRACE_BEGIN();

    switch ( data_id_get_table(&id) )
    {
        case DATA_TABLE_VOID:
            data_diagram_init_empty( &((*this_).private_diagram_cache) );
            break;
        case DATA_TABLE_CLASSIFIER:
            data_diagram_init_empty( &((*this_).private_diagram_cache) );
            break;
        case DATA_TABLE_FEATURE:
            data_diagram_init_empty( &((*this_).private_diagram_cache) );
            break;
        case DATA_TABLE_RELATIONSHIP:
            data_diagram_init_empty( &((*this_).private_diagram_cache) );
            break;
        case DATA_TABLE_DIAGRAMELEMENT:
            data_diagram_init_empty( &((*this_).private_diagram_cache) );
            break;
        case DATA_TABLE_DIAGRAM:
            {
                if ( force_reload || ( data_diagram_get_id( &((*this_).private_diagram_cache) ) != data_id_get_row_id(&id) ))
                {
                    data_error_t db_err;
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
            data_diagram_init_empty( &((*this_).private_diagram_cache) );
            break;
    }

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
