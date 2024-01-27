/* File: gui_file_export_dialog.c; Copyright and License: see below */

#include "gui_file_export_dialog.h"
#include "set/data_stat.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

#if ( GTK_MAJOR_VERSION < 4 )
#define gtk_widget_set_visible(w,v) ((v)?gtk_widget_show(w):gtk_widget_hide(w))
#endif

void gui_file_export_dialog_init ( gui_file_export_dialog_t *this_,
                                   data_database_t *database,
                                   data_database_reader_t *db_reader,
                                   GtkWindow *parent_window,
                                   gui_simple_message_to_user_t *message_to_user )
{
    U8_TRACE_BEGIN();
    assert( NULL != database );
    assert( NULL != db_reader );
    assert( NULL != parent_window );
    assert( NULL != message_to_user );

    (*this_).database = database;
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
    */
    (*this_).format_docbook = gtk_check_button_new_with_label ("docbook");
    /*
    (*this_).format_doxygen = gtk_check_button_new_with_label ("doxygen");
    (*this_).format_latex = gtk_check_button_new_with_label ("latex");
    (*this_).format_rtf = gtk_check_button_new_with_label ("rtf");
    */
    (*this_).format_xhtml = gtk_check_button_new_with_label ("html");
    (*this_).format_xmi2 = gtk_check_button_new_with_label ("xmi");
    (*this_).format_json= gtk_check_button_new_with_label ("json");

    (*this_).format_pdf = gtk_check_button_new_with_label ("pdf");
    (*this_).format_png = gtk_check_button_new_with_label ("png");
    (*this_).format_ps = gtk_check_button_new_with_label ("ps");
    (*this_).format_svg = gtk_check_button_new_with_label ("svg");
    (*this_).format_txt = gtk_check_button_new_with_label ("txt");

    (*this_).options_layout = gtk_grid_new();

    (*this_).diagram_set_label = gtk_label_new ( "Diagram-sets:" );
    (*this_).document_label = gtk_label_new ( "Documents:" );
#if ((( GTK_MAJOR_VERSION == 3 ) && ( GTK_MINOR_VERSION >= 16 ))||( GTK_MAJOR_VERSION >= 4 ))
    gtk_label_set_xalign (GTK_LABEL( (*this_).document_label ), 0.0 );
    gtk_label_set_xalign (GTK_LABEL( (*this_).diagram_set_label ), 0.0 );
#else
    gtk_misc_set_alignment (GTK_MISC( (*this_).document_label ), 0.0, 0.0 );
    gtk_misc_set_alignment (GTK_MISC( (*this_).diagram_set_label ), 0.0, 0.0 );
#endif

    /* parameter info: gtk_grid_attach (GtkGrid *grid, GtkWidget *child, gint left, gint top, gint width, gint height); */
    gtk_grid_set_column_homogeneous ( GTK_GRID((*this_).options_layout), false );
    gtk_grid_set_row_homogeneous ( GTK_GRID((*this_).options_layout), false );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).diagram_set_label, 0, 0, 2, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).document_label, 0, 1, 2, 1 );

    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_docbook, 2, 1, 2, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_xhtml, 4, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_json, 5, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_xmi2, 6, 1, 2, 1 );
    /*
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_asciidoc, 4, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_doxygen, 5, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_latex, 6, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_rtf, 7, 1, 1, 1 );
    */

    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_pdf, 2, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_png, 3, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_ps, 4, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_svg, 5, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_txt, 6, 0, 1, 1 );

#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_box_append( GTK_BOX(content_area), GTK_WIDGET( (*this_).options_layout ) );
#else
    gtk_container_add( GTK_CONTAINER(content_area), GTK_WIDGET( (*this_).options_layout ) );
#endif
    /* no need to g_object_unref( content_area ); here */

    io_exporter_init( &((*this_).file_exporter), db_reader );

    g_signal_connect( G_OBJECT((*this_).export_file_chooser),
                      "response",
                      G_CALLBACK(gui_file_export_dialog_response_callback),
                      this_
                    );
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_window_set_hide_on_close( GTK_WINDOW((*this_).export_file_chooser), true);
#else
    g_signal_connect( G_OBJECT((*this_).export_file_chooser), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL );
#endif

    U8_TRACE_END();
}

void gui_file_export_dialog_destroy( gui_file_export_dialog_t *this_ )
{
    U8_TRACE_BEGIN();

    /* no need to g_object_unref ( (*this_).format_asciidoc ); here */
#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_window_destroy( GTK_WINDOW((*this_).export_file_chooser) );
#else
    gtk_widget_destroy( (*this_).export_file_chooser );
#endif
    /* no need to g_object_unref ( (*this_).export_file_chooser ); here */

    io_exporter_destroy( &((*this_).file_exporter) );

    (*this_).message_to_user = NULL;
    (*this_).database = NULL;

    U8_TRACE_END();
}

void gui_file_export_dialog_show( gui_file_export_dialog_t *this_ )
{
    U8_TRACE_BEGIN();

#if ( GTK_MAJOR_VERSION >= 4 )
    gtk_widget_set_visible( GTK_WIDGET( (*this_).export_file_chooser ), TRUE );
    gtk_widget_set_sensitive( GTK_WIDGET((*this_).export_file_chooser), TRUE );  /* idea taken from gtk demo */

    GdkSurface *surface = gtk_native_get_surface( GTK_NATIVE((*this_).export_file_chooser) );
    gdk_surface_set_cursor( surface, NULL );  /* idea taken from gtk3->4 guide */
#else
    gtk_widget_show_all( GTK_WIDGET( (*this_).export_file_chooser ) );
#endif

    U8_TRACE_END();
}

void gui_file_export_dialog_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_file_export_dialog_t *this_ = user_data;

    switch ( response_id )
    {
        case GTK_RESPONSE_CANCEL:
        {
            U8_LOG_EVENT( "GTK_RESPONSE_CANCEL" );
            gtk_widget_set_visible( GTK_WIDGET ( dialog ), FALSE );
        }
        break;

        case GTK_RESPONSE_ACCEPT:
        {
            U8_LOG_EVENT( "GTK_RESPONSE_ACCEPT" );
            int export_err;
            data_stat_t export_stat;
            data_stat_init ( &export_stat );
            io_file_format_t selected_format;

            gchar *folder_path = NULL;
            GFile *selected_file = NULL;
#if ( GTK_MAJOR_VERSION >= 4 )
            selected_file = gtk_file_chooser_get_file( GTK_FILE_CHOOSER(dialog) );
            if ( selected_file != NULL )
            {
                folder_path = g_file_get_path( selected_file );
            }
#else
            folder_path = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dialog) );
#endif
            gtk_widget_set_visible( GTK_WIDGET ( dialog ), FALSE );
            if ( folder_path != NULL )
            {
                U8_TRACE_INFO_STR( "chosen folder:", folder_path );

                selected_format = IO_FILE_FORMAT_NONE;
    #if ( GTK_MAJOR_VERSION >= 4 )
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_pdf) )) { selected_format |= IO_FILE_FORMAT_PDF; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_png) )) { selected_format |= IO_FILE_FORMAT_PNG; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_ps) )) { selected_format |= IO_FILE_FORMAT_PS; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_svg) )) { selected_format |= IO_FILE_FORMAT_SVG; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_txt) )) { selected_format |= IO_FILE_FORMAT_TXT; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_docbook) )) { selected_format |= IO_FILE_FORMAT_DOCBOOK; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_xhtml) )) { selected_format |= IO_FILE_FORMAT_HTML; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_xmi2) )) { selected_format |= IO_FILE_FORMAT_XMI2; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_json) )) { selected_format |= IO_FILE_FORMAT_JSON; }
    #else
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_pdf) )) { selected_format |= IO_FILE_FORMAT_PDF; }
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_png) )) { selected_format |= IO_FILE_FORMAT_PNG; }
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_ps) )) { selected_format |= IO_FILE_FORMAT_PS; }
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_svg) )) { selected_format |= IO_FILE_FORMAT_SVG; }
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_txt) )) { selected_format |= IO_FILE_FORMAT_TXT; }
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_docbook) )) { selected_format |= IO_FILE_FORMAT_DOCBOOK; }
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_xhtml) )) { selected_format |= IO_FILE_FORMAT_HTML; }
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_xmi2) )) { selected_format |= IO_FILE_FORMAT_XMI2; }
                if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON((*this_).format_json) )) { selected_format |= IO_FILE_FORMAT_JSON; }
    #endif

                /* react immediately */
                gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                                   GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                   GUI_SIMPLE_MESSAGE_CONTENT_EXPORTING_WAIT,
                                                                   folder_path
                                                                 );
                bool events_handled = true;
                for ( uint_fast8_t max_loop = 40; events_handled && ( max_loop > 0 ); max_loop-- )
                {
                    events_handled = g_main_context_iteration( NULL, /*may_block*/ FALSE );
                }

                /* determine the database file path */
                const char *db_path = data_database_get_filename_ptr( (*this_).database );

                if ( data_database_is_open((*this_).database) )
                {
                    export_err = io_exporter_export_files( &((*this_).file_exporter), selected_format, folder_path, db_path, &export_stat );
                }
                else
                {
                    export_err = -1;
                }

                char temp_format_buf[64];
                utf8stringbuf_t temp_fileformat = UTF8STRINGBUF( temp_format_buf );
                io_file_format_to_string( selected_format, temp_fileformat );

                if ( 0 == export_err )
                {
                    gui_simple_message_to_user_show_message_with_names_and_stat( (*this_).message_to_user,
                                                                                 GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                                 GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED,
                                                                                 utf8stringbuf_get_string( temp_fileformat ),
                                                                                 &export_stat
                                                                               );

                }
                else
                {
                    gui_simple_message_to_user_show_message_with_name( (*this_).message_to_user,
                                                                       GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                       GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED,
                                                                       folder_path
                                                                     );
                }
                g_free (folder_path);
            }
            else
            {
                U8_LOG_WARNING( "Export dialog returned no folder path" );
            }
            if ( selected_file != NULL )
            {
                g_object_unref( selected_file );
            }
            data_stat_trace( &export_stat );
            data_stat_destroy ( &export_stat );
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
Copyright 2019-2024 Andreas Warnke

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
