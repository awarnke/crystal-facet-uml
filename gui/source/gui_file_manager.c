/* File: gui_file_manager.c; Copyright and License: see below */

#include "gui_file_manager.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_file_manager_init ( gui_file_manager_t *this_, ctrl_controller_t *controller, data_database_t *database )
{
    TRACE_BEGIN();

    TRACE_END();
}

void gui_file_manager_destroy( gui_file_manager_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void gui_file_manager_use_db_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_file_manager_t *this_ = user_data;

    switch ( response_id )
    {
        case GTK_RESPONSE_ACCEPT:
        {
            LOG_EVENT( "GTK_RESPONSE_ACCEPT" );
            gchar *filename;
            filename = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            TRACE_INFO_STR( "File chosen:", filename );
            g_free (filename);
        }
        break;

        case GTK_RESPONSE_CANCEL:
        {
            LOG_EVENT( "GTK_RESPONSE_CANCEL" );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
        }
        break;

        case GTK_RESPONSE_DELETE_EVENT:
        {
            LOG_EVENT( "GTK_RESPONSE_DELETE_EVENT" );
        }
        break;

        default:
        {
            LOG_ERROR( "unexpected response_id" );
        }
    }

    TRACE_END();
}

void gui_file_manager_export_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_file_manager_t *this_ = user_data;

    switch ( response_id )
    {
        case GTK_RESPONSE_CANCEL:
        {
            LOG_EVENT( "GTK_RESPONSE_CANCEL" );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
        }
        break;

        case GUI_FILEMANAGER_CONST_EXPORT_PNG:
        {
            LOG_EVENT( "GUI_FILEMANAGER_CONST_EXPORT_PNG" );
            gchar *filename;
            filename = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            TRACE_INFO_STR( "File chosen:", filename );
            g_free (filename);
        }
        break;

        case GUI_FILEMANAGER_CONST_EXPORT_SVG:
        {
            LOG_EVENT( "GUI_FILEMANAGER_CONST_EXPORT_SVG" );
            gchar *filename;
            filename = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            TRACE_INFO_STR( "File chosen:", filename );
            g_free (filename);
        }
        break;

        case GTK_RESPONSE_DELETE_EVENT:
        {
            LOG_EVENT( "GTK_RESPONSE_DELETE_EVENT" );
        }
        break;

        default:
        {
            LOG_ERROR( "unexpected response_id" );
        }
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
