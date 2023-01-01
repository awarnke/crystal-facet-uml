/* File: gui_file_use_db_dialog.c; Copyright and License: see below */

#include "gui_file_use_db_dialog.h"
#include "u8/u8_trace.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_file_use_db_dialog_init ( gui_file_use_db_dialog_t *this_,
                                   ctrl_controller_t *controller,
                                   io_data_file_t *database,
                                   GtkWindow *parent_window,
                                   gui_simple_message_to_user_t *message_to_user )
{
    U8_TRACE_BEGIN();

    (*this_).use_db_file_chooser = gtk_file_chooser_dialog_new ( "Select DB to use",
                                                                 parent_window,
                                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
                                                                 "Cancel",
                                                                 GTK_RESPONSE_CANCEL,
                                                                 "Use DB-File",
                                                                 GTK_RESPONSE_ACCEPT,
                                                                 NULL
                                                               );
    /* set name postponed, see https://gitlab.gnome.org/GNOME/gtk/-/issues/4832 */

    gui_file_db_manager_init( &((*this_).file_manager), controller, database, message_to_user );

    g_signal_connect( G_OBJECT((*this_).use_db_file_chooser),
                      "response",
                      G_CALLBACK(gui_file_use_db_dialog_response_callback),
                      this_
                    );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_window_set_hide_on_close( GTK_WINDOW((*this_).use_db_file_chooser), true);
#else
    g_signal_connect( G_OBJECT((*this_).use_db_file_chooser), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL );
#endif
    U8_TRACE_END();
}

void gui_file_use_db_dialog_destroy( gui_file_use_db_dialog_t *this_ )
{
    U8_TRACE_BEGIN();

#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_window_destroy( GTK_WINDOW((*this_).use_db_file_chooser) );
#else
    gtk_widget_destroy( (*this_).use_db_file_chooser );
#endif
    /* no need to g_object_unref ( (*this_).use_db_file_chooser ); here */

    gui_file_db_manager_destroy( &((*this_).file_manager) );

    U8_TRACE_END();
}

void gui_file_use_db_dialog_show( gui_file_use_db_dialog_t *this_, bool open_existing )
{
    U8_TRACE_BEGIN();

    gtk_file_chooser_set_action( GTK_FILE_CHOOSER( (*this_).use_db_file_chooser ),
                                 open_existing ? GTK_FILE_CHOOSER_ACTION_OPEN : GTK_FILE_CHOOSER_ACTION_SAVE
                               );
    if( ! open_existing )
    {
        /* moved here as workaround for disabled file dialog widgets, see https://gitlab.gnome.org/GNOME/gtk/-/issues/4832 */
        gtk_file_chooser_set_current_name( GTK_FILE_CHOOSER( (*this_).use_db_file_chooser ), "untitled.cfuJ" );
    }

#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_widget_show( GTK_WIDGET( (*this_).use_db_file_chooser ) );

    gtk_widget_set_sensitive( GTK_WIDGET((*this_).use_db_file_chooser), TRUE );  /* idea taken from gtk demo */

    GdkSurface *surface = gtk_native_get_surface( GTK_NATIVE((*this_).use_db_file_chooser) );
    gdk_surface_set_cursor( surface, NULL );  /* idea taken from gtk3->4 guide */
#else
    gtk_widget_show_all( GTK_WIDGET( (*this_).use_db_file_chooser ) );
#endif

    U8_TRACE_END();
}

void gui_file_use_db_dialog_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_file_use_db_dialog_t *this_ = user_data;

    switch ( response_id )
    {
        case GTK_RESPONSE_ACCEPT:
        {
            U8_LOG_EVENT( "GTK_RESPONSE_ACCEPT" );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );

            gchar *filename = NULL;
#if ( GTK_MAJOR_VERSION >= 4 )
            GFile *selected_file = gtk_file_chooser_get_file( GTK_FILE_CHOOSER(dialog) );
            if ( selected_file != NULL )
            {
                filename = g_file_get_path( selected_file );
            }
#else
            filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dialog) );
#endif
            if ( filename != NULL )
            {
                U8_TRACE_INFO_STR( "File chosen:", filename );

                gui_file_db_manager_use_db( &((*this_).file_manager), filename );

                g_free (filename);
            }
            else
            {
                U8_LOG_WARNING( "Use DB dialog returned no file name" );
            }
#if ( GTK_MAJOR_VERSION >= 4 )
            if ( selected_file != NULL )
            {
                g_object_unref( selected_file );
            }
#endif
        }
        break;

        case GTK_RESPONSE_CANCEL:
        {
            U8_LOG_EVENT( "GTK_RESPONSE_CANCEL" );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
        }
        break;

        case GTK_RESPONSE_DELETE_EVENT:
        {
            U8_LOG_EVENT( "GTK_RESPONSE_DELETE_EVENT" );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "unexpected response_id" );
        }
    }

    U8_TRACE_END();
}


/*
Copyright 2019-2023 Andreas Warnke

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
