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
    assert( NULL != db_reader );
    assert( NULL != parent_window );
    assert( NULL != message_to_user );

    (*this_).message_to_user = message_to_user;

    (*this_).export_file_chooser = gtk_file_chooser_dialog_new ( "Select Export Folder",
                                                                 parent_window,
                                                                 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                                 "Cancel",
                                                                 GTK_RESPONSE_CANCEL,
                                                                 "Export Files",
                                                                 GTK_RESPONSE_ACCEPT,
                                                                 NULL
                                                               );
    
    GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG((*this_).export_file_chooser));
    
    /*
    (*this_).format_asciidoc = gtk_check_button_new_with_label ("asciidoc");
    (*this_).format_docbook = gtk_check_button_new_with_label ("docbook");
    (*this_).format_doxygen = gtk_check_button_new_with_label ("doxygen");
    (*this_).format_latex = gtk_check_button_new_with_label ("latex");
    (*this_).format_rtf = gtk_check_button_new_with_label ("rtf");
    (*this_).format_xhtml = gtk_check_button_new_with_label ("xhtml");
    */
    
    (*this_).format_pdf = gtk_check_button_new_with_label ("pdf");
    (*this_).format_png = gtk_check_button_new_with_label ("png");
    (*this_).format_ps = gtk_check_button_new_with_label ("ps");
    (*this_).format_svg = gtk_check_button_new_with_label ("svg");
    (*this_).format_txt = gtk_check_button_new_with_label ("txt");

    (*this_).flowbox_diagram_set = GTK_FLOW_BOX( gtk_flow_box_new ());
    /*
    (*this_).flowbox_document = GTK_FLOW_BOX( gtk_flow_box_new ());
    */

    (*this_).diagram_set_label = gtk_label_new ( "Diagram-sets:" );
    /*
    (*this_).document_label = gtk_label_new ( "Documents (not yet implemented):" );
#if (( GTK_MAJOR_VERSION == 3 ) && ( GTK_MINOR_VERSION >= 16 ))
    gtk_label_set_xalign (GTK_LABEL( (*this_).document_label ), 0.0 );
    gtk_label_set_xalign (GTK_LABEL( (*this_).diagram_set_label ), 0.0 );
#else
    gtk_misc_set_alignment (GTK_MISC( (*this_).document_label ), 0.0, 0.0 );
    gtk_misc_set_alignment (GTK_MISC( (*this_).diagram_set_label ), 0.0, 0.0 );
#endif
    */

    /*
    gtk_flow_box_insert ( (*this_).flowbox_document, (*this_).document_label, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_document, (*this_).format_asciidoc, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_document, (*this_).format_docbook, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_document, (*this_).format_doxygen, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_document, (*this_).format_latex, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_document, (*this_).format_rtf, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_document, (*this_).format_xhtml, -1 );
    */
    
    gtk_flow_box_insert ( (*this_).flowbox_diagram_set, (*this_).diagram_set_label, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_diagram_set, (*this_).format_pdf, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_diagram_set, (*this_).format_png, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_diagram_set, (*this_).format_ps, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_diagram_set, (*this_).format_svg, -1 );
    gtk_flow_box_insert ( (*this_).flowbox_diagram_set, (*this_).format_txt, -1 );

    gtk_container_add ( GTK_CONTAINER(content_area), GTK_WIDGET( (*this_).flowbox_diagram_set ) );
    /*
    gtk_container_add ( GTK_CONTAINER(content_area), GTK_WIDGET( (*this_).flowbox_document ) );
    */
    /* no need to g_object_unref( content_area ); here */

    io_diagram_image_exporter_init( &((*this_).file_exporter), db_reader );

    g_signal_connect( G_OBJECT((*this_).export_file_chooser), "response", G_CALLBACK(gui_file_export_dialog_response_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).export_file_chooser), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL );

    TRACE_END();
}

void gui_file_export_dialog_destroy( gui_file_export_dialog_t *this_ )
{
    TRACE_BEGIN();

    /* no need to g_object_unref ( (*this_).format_asciidoc ); here */
    gtk_widget_destroy( (*this_).export_file_chooser );
    /* no need to g_object_unref ( (*this_).export_file_chooser ); here */

    io_diagram_image_exporter_destroy( &((*this_).file_exporter) );

    (*this_).message_to_user = NULL;

    TRACE_END();
}

void gui_file_export_dialog_show( gui_file_export_dialog_t *this_ )
{
    TRACE_BEGIN();

    gtk_widget_show_all( GTK_WIDGET( (*this_).export_file_chooser ) );

    TRACE_END();
}

void gui_file_export_dialog_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_file_export_dialog_t *this_ = user_data;

    switch ( response_id )
    {
        case GTK_RESPONSE_CANCEL:
        {
            TSLOG_EVENT( "GTK_RESPONSE_CANCEL" );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
        }
        break;

        case GTK_RESPONSE_ACCEPT:
        {
            TSLOG_EVENT( "GTK_RESPONSE_ACCEPT" );
            int export_err;
            io_file_format_t selected_format;
            gchar *folder_path;
            folder_path = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
            TRACE_INFO_STR( "chosen folder:", folder_path );

            selected_format = IO_FILE_FORMAT_NONE;
            if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON((*this_).format_pdf) )) { selected_format |= IO_FILE_FORMAT_PDF; }
            if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON((*this_).format_png) )) { selected_format |= IO_FILE_FORMAT_PNG; }
            if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON((*this_).format_ps) )) { selected_format |= IO_FILE_FORMAT_PS; }
            if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON((*this_).format_svg) )) { selected_format |= IO_FILE_FORMAT_SVG; }
            if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON((*this_).format_txt) )) { selected_format |= IO_FILE_FORMAT_TXT; }

            export_err = io_diagram_image_exporter_export_files( &((*this_).file_exporter), selected_format, folder_path );

            g_free (folder_path);

            char temp_format_buf[64];
            utf8stringbuf_t temp_fileformat = UTF8STRINGBUF( temp_format_buf );
            io_file_format_to_string( selected_format, temp_fileformat );

            if ( 0 == export_err )
            {
                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED,
                                                                     utf8stringbuf_get_string( temp_fileformat )
                                                                   );
            }
            else
            {
                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED,
                                                                     utf8stringbuf_get_string( temp_fileformat )
                                                                   );
            }
        }
        break;

        case GTK_RESPONSE_DELETE_EVENT:
        {
            TSLOG_EVENT( "GTK_RESPONSE_DELETE_EVENT" );
        }
        break;

        default:
        {
            TSLOG_ERROR( "unexpected response_id" );
        }
    }

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
