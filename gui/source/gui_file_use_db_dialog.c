/* File: gui_file_use_db_dialog.c; Copyright and License: see below */

#include "gui_file_use_db_dialog.h"
#include "u8/u8_trace.h"
#include "gui_gtk.h"
#include "gui_gdk.h"
#include <stdio.h>
#include <stdbool.h>

void gui_file_use_db_dialog_init ( gui_file_use_db_dialog_t *this_,
                                   GtkWindow *parent_window,
                                   gui_file_action_t *file_action )
{
    U8_TRACE_BEGIN();

    (*this_).parent_window = parent_window;

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    (*this_).new_file_chooser = gtk_file_chooser_dialog_new ( "Select new file",
                                                              parent_window,
                                                              GTK_FILE_CHOOSER_ACTION_SAVE,
                                                              "Cancel",
                                                              GTK_RESPONSE_CANCEL,
                                                              "New",
                                                              GTK_RESPONSE_ACCEPT,
                                                              NULL
                                                            );
    /* set name postponed, see https://gitlab.gnome.org/GNOME/gtk/-/issues/4832 */
    (*this_).open_file_chooser = gtk_file_chooser_dialog_new ( "Select file to open",
                                                               parent_window,
                                                               GTK_FILE_CHOOSER_ACTION_OPEN,
                                                               "Cancel",
                                                               GTK_RESPONSE_CANCEL,
                                                               "Open",
                                                               GTK_RESPONSE_ACCEPT,
                                                               NULL
                                                             );
#else
    (*this_).new_file_dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_accept_label( (*this_).new_file_dialog, "New" );
    gtk_file_dialog_set_modal( (*this_).new_file_dialog, false );
    gtk_file_dialog_set_title( (*this_).new_file_dialog, "Select new file" );
    gtk_file_dialog_set_initial_name( (*this_).new_file_dialog, "NewModel.cfuJ" );

    (*this_).open_file_dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_accept_label( (*this_).open_file_dialog, "Open" );
    gtk_file_dialog_set_modal( (*this_).open_file_dialog, false );
    gtk_file_dialog_set_title( (*this_).open_file_dialog, "Select file to open" );
#endif

    (*this_).file_action = file_action;

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    g_signal_connect( G_OBJECT((*this_).new_file_chooser),
                      "response",
                      G_CALLBACK(gui_file_use_db_dialog_response_callback),
                      this_
                    );
    gtk_window_set_hide_on_close( GTK_WINDOW((*this_).new_file_chooser), true);
    g_signal_connect( G_OBJECT((*this_).open_file_chooser),
                      "response",
                      G_CALLBACK(gui_file_use_db_dialog_response_callback),
                      this_
                    );
    gtk_window_set_hide_on_close( GTK_WINDOW((*this_).open_file_chooser), true);
#else
    /* no signal at new FileDialog - this works with Async, see gtk_file_dialog_save */
#endif

    U8_TRACE_END();
}

void gui_file_use_db_dialog_destroy( gui_file_use_db_dialog_t *this_ )
{
    U8_TRACE_BEGIN();

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    gtk_window_destroy( GTK_WINDOW((*this_).new_file_chooser) );
    /* no need to g_object_unref ( (*this_).new_file_chooser ); here */
    gtk_window_destroy( GTK_WINDOW((*this_).open_file_chooser) );
    /* no need to g_object_unref ( (*this_).open_file_chooser ); here */
#else
    g_object_unref( (*this_).new_file_dialog );
    g_object_unref( (*this_).open_file_dialog );
#endif

    (*this_).file_action = NULL;
    (*this_).parent_window = NULL;

    U8_TRACE_END();
}

void gui_file_use_db_dialog_show( gui_file_use_db_dialog_t *this_, bool open_existing )
{
    U8_TRACE_BEGIN();

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    if( ! open_existing )
    {
        /* moved here as workaround for disabled file dialog widgets, see https://gitlab.gnome.org/GNOME/gtk/-/issues/4832 */
        gtk_file_chooser_set_current_name( GTK_FILE_CHOOSER( (*this_).new_file_chooser ), "NewModel.cfuJ" );

        gtk_widget_set_visible( GTK_WIDGET( (*this_).new_file_chooser ), TRUE );
        gtk_widget_set_sensitive( GTK_WIDGET((*this_).new_file_chooser), TRUE );  /* idea taken from gtk demo */
        GdkSurface *surface = gtk_native_get_surface( GTK_NATIVE((*this_).new_file_chooser) );
        gdk_surface_set_cursor( surface, NULL );  /* idea taken from gtk3->4 guide */
    }
    else
    {
        gtk_widget_set_visible( GTK_WIDGET( (*this_).open_file_chooser ), TRUE );
        gtk_widget_set_sensitive( GTK_WIDGET((*this_).open_file_chooser), TRUE );  /* idea taken from gtk demo */
        GdkSurface *surface = gtk_native_get_surface( GTK_NATIVE((*this_).open_file_chooser) );
        gdk_surface_set_cursor( surface, NULL );  /* idea taken from gtk3->4 guide */
    }
#else
    if ( open_existing )
    {
        gtk_file_dialog_open( (*this_).open_file_dialog,
                              (*this_).parent_window,
                              NULL,
                              &gui_file_use_db_dialog_async_ready_callback_on_open,
                              this_
                            );
    }
    else
    {
        gtk_file_dialog_save( (*this_).new_file_dialog,
                              (*this_).parent_window,
                              NULL,
                              &gui_file_use_db_dialog_async_ready_callback_on_new,
                              this_
                            );
    }
#endif

    U8_TRACE_END();
}

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))

void gui_file_use_db_dialog_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_file_use_db_dialog_t *this_ = user_data;

    switch ( response_id )
    {
        case GTK_RESPONSE_ACCEPT:
        {
            U8_LOG_EVENT( "GTK_RESPONSE_ACCEPT" );
            gtk_widget_set_visible( GTK_WIDGET ( dialog ), FALSE );

            gchar *filename = NULL;
            GFile *selected_file = gtk_file_chooser_get_file( GTK_FILE_CHOOSER(dialog) );
            if ( selected_file != NULL )
            {
                filename = g_file_get_path( selected_file );
            }
            if ( filename != NULL )
            {
                U8_TRACE_INFO_STR( "File chosen:", filename );

                gui_file_action_use_db( (*this_).file_action, filename );

                g_free (filename);
            }
            else
            {
                U8_LOG_WARNING( "Use DB dialog returned no file name" );
            }
            if ( selected_file != NULL )
            {
                g_object_unref( selected_file );
            }
        }
        break;

        case GTK_RESPONSE_CANCEL:
        {
            U8_LOG_EVENT( "GTK_RESPONSE_CANCEL" );
            gtk_widget_set_visible( GTK_WIDGET ( dialog ), FALSE );
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

#else  /* GTK >= 4.10 */

void gui_file_use_db_dialog_async_ready_callback_on_open( GObject* source_object,
                                                          GAsyncResult* res,
                                                          gpointer user_data )
{
    U8_TRACE_BEGIN();

    gui_file_use_db_dialog_t *this_ = user_data;
    GError* error = NULL;
    GFile *result = gtk_file_dialog_open_finish( GTK_FILE_DIALOG(source_object), res, &error );
    if ( error != NULL )
    {
        /* User pressed cancel */
        U8_TRACE_INFO_STR( "unexpected response from file dialog:", error->message );
        g_error_free( error );
    }
    else if ( result != NULL )
    {
        gchar *folder_path = g_file_get_path ( result );
        if ( folder_path != NULL )
        {
            U8_LOG_EVENT( "User selected a database file to open." );
            U8_TRACE_INFO_STR( "File to open:", folder_path );

            /* react immediately, handle events */
            bool events_handled = true;
            for ( uint_fast8_t max_loop = 40; events_handled && ( max_loop > 0 ); max_loop-- )
            {
                events_handled = g_main_context_iteration( NULL, /*may_block*/ FALSE );
            }

            gui_file_action_use_db( (*this_).file_action, folder_path );

            g_free (folder_path);
        }
    }
    if ( result != NULL )
    {
        g_object_unref( result );
    }

    U8_TRACE_END();
}

void gui_file_use_db_dialog_async_ready_callback_on_new( GObject* source_object,
                                                         GAsyncResult* res,
                                                         gpointer user_data )
{
    U8_TRACE_BEGIN();

    gui_file_use_db_dialog_t *this_ = user_data;
    GError* error = NULL;
    GFile *result = gtk_file_dialog_save_finish( GTK_FILE_DIALOG(source_object), res, &error );
    if ( error != NULL )
    {
        /* User pressed cancel */
        U8_TRACE_INFO_STR( "unexpected response from file dialog:", error->message );
        g_error_free( error );
    }
    else if ( result != NULL )
    {
        gchar *folder_path = g_file_get_path ( result );
        if ( folder_path != NULL )
        {
            U8_LOG_EVENT( "User selected a new database file." );
            U8_TRACE_INFO_STR( "File to create:", folder_path );

            /* react immediately, handle events */
            bool events_handled = true;
            for ( uint_fast8_t max_loop = 40; events_handled && ( max_loop > 0 ); max_loop-- )
            {
                events_handled = g_main_context_iteration( NULL, /*may_block*/ FALSE );
            }

            gui_file_action_use_db( (*this_).file_action, folder_path );

            g_free (folder_path);
        }
    }
    if ( result != NULL )
    {
        g_object_unref( result );
    }

    U8_TRACE_END();
}

#endif


/*
Copyright 2019-2025 Andreas Warnke

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
