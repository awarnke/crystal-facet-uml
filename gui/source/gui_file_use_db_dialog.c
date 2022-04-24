/* File: gui_file_use_db_dialog.c; Copyright and License: see below */

#include "gui_file_use_db_dialog.h"
#include "trace.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_file_use_db_dialog_init ( gui_file_use_db_dialog_t *this_,
                                   ctrl_controller_t *controller,
                                   data_database_t *database,
                                   GtkWindow *parent_window,
                                   gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();

    (*this_).use_db_file_chooser = gtk_file_chooser_dialog_new ( "Select DB to use",
                                                                 parent_window,
                                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
                                                                 "Cancel",
                                                                 GTK_RESPONSE_CANCEL,
                                                                 "Create/Use DB-File",
                                                                 GTK_RESPONSE_ACCEPT,
                                                                 NULL
                                                               );
#if ( GTK_MAJOR_VERSION >= 4 )
#else
    gtk_file_chooser_set_current_name( GTK_FILE_CHOOSER( (*this_).use_db_file_chooser ), "untitled.cfu1" );
#endif

    gui_file_db_manager_init( &((*this_).file_manager), controller, database, message_to_user );

    g_signal_connect( G_OBJECT((*this_).use_db_file_chooser), "response", G_CALLBACK(gui_file_db_manager_use_db_response_callback), &((*this_).file_manager) );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_window_set_hide_on_close( GTK_WINDOW((*this_).use_db_file_chooser), true);
#else
    g_signal_connect( G_OBJECT((*this_).use_db_file_chooser), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL );
#endif
    TRACE_END();
}

void gui_file_use_db_dialog_destroy( gui_file_use_db_dialog_t *this_ )
{
    TRACE_BEGIN();

#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_window_destroy( GTK_WINDOW((*this_).use_db_file_chooser) );
#else
    gtk_widget_destroy( (*this_).use_db_file_chooser );
#endif
    /* no need to g_object_unref ( (*this_).use_db_file_chooser ); here */

    gui_file_db_manager_destroy( &((*this_).file_manager) );

    TRACE_END();
}

void gui_file_use_db_dialog_show( gui_file_use_db_dialog_t *this_ )
{
    TRACE_BEGIN();

#if ( GTK_MAJOR_VERSION >= 4 )
    /* workaround for disabled file dialog widgets and accepting bug https://gitlab.gnome.org/GNOME/gtk/-/issues/4832 */
    gtk_file_chooser_set_current_name( GTK_FILE_CHOOSER( (*this_).use_db_file_chooser ), "untitled.cfu1" );

    gtk_widget_show( GTK_WIDGET( (*this_).use_db_file_chooser ) );
#ifdef __linux__
#else
    /* This was a workaround in cfu 1.36.0 for gtk 4.6.1, not needed anymore for gtk 4.6.2 */

    //gtk_widget_set_receives_default( GTK_WIDGET( (*this_).use_db_file_chooser ), TRUE );  /* this may be needed on windows ? */
    //gtk_window_minimize( GTK_WINDOW((*this_).use_db_file_chooser) );  /* workaround needed for win/gtk4.6.1 environment */
    //gtk_window_present( GTK_WINDOW((*this_).use_db_file_chooser) );  /* shows and positions nicely */


    //gtk_widget_set_can_target( GTK_WIDGET((*this_).use_db_file_chooser), TRUE );  /* this may be needed on windows ? */
    //gtk_widget_set_focus_on_click( GTK_WIDGET((*this_).use_db_file_chooser), TRUE );  /* this may be needed on windows ? */
#endif
    gtk_widget_set_sensitive( GTK_WIDGET((*this_).use_db_file_chooser), TRUE );  /* idea taken from gtk demo */

    GdkSurface *surface = gtk_native_get_surface( GTK_NATIVE((*this_).use_db_file_chooser) );
    gdk_surface_set_cursor( surface, NULL );  /* idea taken from gtk3->4 guide */

    //const cairo_rectangle_int_t rect
    //    = { .x = 0, .y = 0, .width = gdk_surface_get_width( surface ), .height = gdk_surface_get_height( surface ) };
    //cairo_region_t *region = cairo_region_create_rectangle( &rect );
    //gdk_surface_set_input_region( surface, region );
    //cairo_region_destroy( region );
#else
    gtk_widget_show_all( GTK_WIDGET( (*this_).use_db_file_chooser ) );
#endif

    TRACE_END();
}


/*
Copyright 2019-2022 Andreas Warnke

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
