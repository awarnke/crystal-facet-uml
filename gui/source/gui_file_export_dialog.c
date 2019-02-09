/* File: gui_file_export_dialog.c; Copyright and License: see below */

#include "gui_file_export_dialog.h"
#include "trace.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_file_export_dialog_init ( gui_file_export_dialog_t *this_,
                                   data_database_reader_t *db_reader,
                                   GtkWindow *parent_window,
                                   gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();

    (*this_).export_file_chooser = gtk_file_chooser_dialog_new ( "Select Export Folder",
                                                                 parent_window,
                                                                 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                                 "Cancel",
                                                                 GTK_RESPONSE_CANCEL,
                                                                 "txt Files",
                                                                 GUI_FILE_EXPORTER_CONST_EXPORT_TXT,
                                                                 "svg Files",
                                                                 GUI_FILE_EXPORTER_CONST_EXPORT_SVG,
                                                                 "png Files",
                                                                 GUI_FILE_EXPORTER_CONST_EXPORT_PNG,
                                                                 "pdf Files",
                                                                 GUI_FILE_EXPORTER_CONST_EXPORT_PDF,
                                                                 "ps Files",
                                                                 GUI_FILE_EXPORTER_CONST_EXPORT_PS,
                                                                 NULL
                                                               );
    /*
    GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG((*this_).export_file_chooser));
    GtkWidget *docbook = gtk_check_button_new_with_label ("db");
    gtk_container_add (GTK_CONTAINER(content_area), GTK_WIDGET(docbook));
    */

    gui_file_exporter_init( &((*this_).file_exporter), db_reader, message_to_user );

    g_signal_connect( G_OBJECT((*this_).export_file_chooser), "response", G_CALLBACK(gui_file_exporter_export_response_callback), &((*this_).file_exporter) );
    g_signal_connect( G_OBJECT((*this_).export_file_chooser), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL );

    TRACE_END();
}

void gui_file_export_dialog_destroy( gui_file_export_dialog_t *this_ )
{
    TRACE_BEGIN();

    gtk_widget_destroy( (*this_).export_file_chooser );

    gui_file_exporter_destroy( &((*this_).file_exporter) );

    TRACE_END();
}

void gui_file_export_dialog_show( gui_file_export_dialog_t *this_ )
{
    TRACE_BEGIN();

    gtk_widget_show_all( GTK_WIDGET( (*this_).export_file_chooser ) );

    TRACE_END();
}


/*
Copyright 2019-2019 Andreas Warnke

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
