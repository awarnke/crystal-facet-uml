/* File: gui_file_export_dialog.c; Copyright and License: see below */

#include "gui_file_export_dialog.h"
#include "set/data_stat.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "gui_gtk.h"
#include "gui_gdk.h"
#include <stdio.h>
#include <stdbool.h>

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
    (*this_).parent_window = parent_window;
    (*this_).message_to_user = message_to_user;

    (*this_).format_docbook = gtk_check_button_new_with_label ("docbook");
    (*this_).format_html = gtk_check_button_new_with_label ("html");
    (*this_).format_xmi2 = gtk_check_button_new_with_label ("xmi");
    (*this_).format_json= gtk_check_button_new_with_label ("json");

    (*this_).format_pdf = gtk_check_button_new_with_label ("pdf");
    (*this_).format_png = gtk_check_button_new_with_label ("png");
    (*this_).format_ps = gtk_check_button_new_with_label ("ps");
    (*this_).format_svg = gtk_check_button_new_with_label ("svg");
    (*this_).format_txt = gtk_check_button_new_with_label ("txt");

    gtk_check_button_set_active( GTK_CHECK_BUTTON((*this_).format_html), TRUE );

    (*this_).options_layout = gtk_grid_new();

    (*this_).diagram_set_label = gtk_label_new ( "Diagram-sets:" );
    (*this_).document_label = gtk_label_new ( "Documents:" );
    gtk_label_set_xalign (GTK_LABEL( (*this_).document_label ), 0.0 );
    gtk_label_set_xalign (GTK_LABEL( (*this_).diagram_set_label ), 0.0 );
    (*this_).export_button = GTK_BUTTON( gtk_button_new() );
    gtk_button_set_label( (*this_).export_button, "Export to folder..." );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).export_button ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).export_button ), false );

    /* parameter info: gtk_grid_attach (GtkGrid *grid, GtkWidget *child, gint left, gint top, gint width, gint height); */
    gtk_grid_set_column_homogeneous ( GTK_GRID((*this_).options_layout), false );
    gtk_grid_set_row_homogeneous ( GTK_GRID((*this_).options_layout), false );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).diagram_set_label, 0, 0, 2, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).document_label, 0, 1, 2, 1 );

    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_docbook, 2, 1, 2, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_html, 4, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_json, 5, 1, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_xmi2, 6, 1, 2, 1 );

    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_pdf, 2, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_png, 3, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_ps, 4, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_svg, 5, 0, 1, 1 );
    gtk_grid_attach( GTK_GRID((*this_).options_layout), (*this_).format_txt, 6, 0, 1, 1 );

    gtk_grid_attach( GTK_GRID((*this_).options_layout), GTK_WIDGET( (*this_).export_button ), 8, 0, 1, 2 );

    gtk_widget_set_visible( (*this_).options_layout, FALSE );

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    (*this_).export_file_chooser = gtk_file_chooser_dialog_new ( "Select Export Folder",
                                                                 parent_window,
                                                                 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                                 "Cancel",
                                                                 GTK_RESPONSE_CANCEL,
                                                                 "Export Files",
                                                                 GTK_RESPONSE_ACCEPT,
                                                                 NULL
                                                               );

#if 0
    GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG((*this_).export_file_chooser));
    gtk_box_append( GTK_BOX(content_area), GTK_WIDGET( (*this_).options_layout ) );
    /* no need to g_object_unref( content_area ); here */
#endif
#else
    (*this_).export_file_dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_accept_label( (*this_).export_file_dialog, "Export Files" );
    gtk_file_dialog_set_modal( (*this_).export_file_dialog, false );
    gtk_file_dialog_set_title( (*this_).export_file_dialog, "Select Export Folder" );
#endif

    io_exporter_init( &((*this_).file_exporter), db_reader );

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    g_signal_connect( G_OBJECT((*this_).export_file_chooser),
                      "response",
                      G_CALLBACK(gui_file_export_dialog_response_callback),
                      this_
                    );
    gtk_window_set_hide_on_close( GTK_WINDOW((*this_).export_file_chooser), true);
#else
    /* no signal at new FileDialog - this works with Async, see gtk_file_dialog_save */
#endif
    g_signal_connect( G_OBJECT((*this_).export_button), "clicked", G_CALLBACK( gui_file_export_dialog_export_btn_callback ), this_ );

    U8_TRACE_END();
}

void gui_file_export_dialog_destroy( gui_file_export_dialog_t *this_ )
{
    U8_TRACE_BEGIN();

    /* no need to g_object_unref ( (*this_).format_html ); here */
#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    gtk_window_destroy( GTK_WINDOW((*this_).export_file_chooser) );
    /* no need to g_object_unref ( (*this_).export_file_chooser ); here */
#else
    g_object_unref( (*this_).export_file_dialog );
#endif

    io_exporter_destroy( &((*this_).file_exporter) );

    (*this_).message_to_user = NULL;
    (*this_).parent_window = NULL;
    (*this_).database = NULL;

    U8_TRACE_END();
}

GtkWidget *gui_file_export_dialog_get_options( gui_file_export_dialog_t *this_ )
{
    return GTK_WIDGET( (*this_).options_layout );
}

void gui_file_export_dialog_export_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_file_export_dialog_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    gui_file_export_dialog_show ( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_file_export_dialog_show( gui_file_export_dialog_t *this_ )
{
    U8_TRACE_BEGIN();

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))
    gtk_widget_set_visible( GTK_WIDGET( (*this_).export_file_chooser ), TRUE );
    gtk_widget_set_sensitive( GTK_WIDGET((*this_).export_file_chooser), TRUE );  /* idea taken from gtk demo */

    GdkSurface *surface = gtk_native_get_surface( GTK_NATIVE((*this_).export_file_chooser) );
    gdk_surface_set_cursor( surface, NULL );  /* idea taken from gtk3->4 guide */
#else
    gtk_file_dialog_select_folder( (*this_).export_file_dialog,
                                   (*this_).parent_window,
                                   NULL,
                                   &gui_file_export_dialog_async_ready_callback,
                                   this_
                                 );
#endif

    U8_TRACE_END();
}

#if (( GTK_MAJOR_VERSION == 4 )&&( GTK_MINOR_VERSION < 10 ))

void gui_file_export_dialog_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    U8_TRACE_BEGIN();
    gui_file_export_dialog_t *this_ = user_data;

    gtk_widget_set_visible( (*this_).options_layout, FALSE );

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
            u8_error_t export_err = U8_ERROR_NONE;
            data_stat_t export_stat;
            data_stat_init ( &export_stat );
            io_file_format_t selected_format = IO_FILE_FORMAT_NONE;

            gchar *folder_path = NULL;
            GFile *selected_file = NULL;
            selected_file = gtk_file_chooser_get_file( GTK_FILE_CHOOSER(dialog) );
            if ( selected_file != NULL )
            {
                folder_path = g_file_get_path( selected_file );
            }
            gtk_widget_set_visible( GTK_WIDGET ( dialog ), FALSE );
            if ( folder_path != NULL )
            {
                U8_TRACE_INFO_STR( "chosen folder:", folder_path );

                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_pdf) )) { selected_format |= IO_FILE_FORMAT_PDF; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_png) )) { selected_format |= IO_FILE_FORMAT_PNG; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_ps) )) { selected_format |= IO_FILE_FORMAT_PS; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_svg) )) { selected_format |= IO_FILE_FORMAT_SVG; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_txt) )) { selected_format |= IO_FILE_FORMAT_TXT; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_docbook) )) { selected_format |= IO_FILE_FORMAT_DOCBOOK; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_html) )) { selected_format |= IO_FILE_FORMAT_HTML; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_xmi2) )) { selected_format |= IO_FILE_FORMAT_XMI2; }
                if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_json) )) { selected_format |= IO_FILE_FORMAT_JSON; }

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

                /* determine the database file path for a document title and export: */
                const char *db_path = data_database_get_filename_ptr( (*this_).database );
                if ( data_database_is_open((*this_).database) )
                {
                    export_err = io_exporter_export_files( &((*this_).file_exporter), selected_format, folder_path, db_path, &export_stat );
                }
                else
                {
                    export_err = U8_ERROR_NO_DB;
                }

                /* finished, notify user */
                char temp_format_buf[64];
                utf8stringbuf_t temp_fileformat = UTF8STRINGBUF( temp_format_buf );
                io_file_format_to_string( selected_format, temp_fileformat );

                if ( U8_ERROR_NONE == export_err )
                {
                    gui_simple_message_to_user_show_message_with_names_and_stat( (*this_).message_to_user,
                                                                                 GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                                 GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED,
                                                                                 utf8stringbuf_get_string( &temp_fileformat ),
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

#else  /* GTK >= 4.10 */

void gui_file_export_dialog_async_ready_callback( GObject* source_object,
                                                  GAsyncResult* res,
                                                  gpointer user_data )
{
    U8_TRACE_BEGIN();

    gui_file_export_dialog_t *this_ = user_data;

    gtk_widget_set_visible( (*this_).options_layout, FALSE );

    GError* error = NULL;
    GFile *result = gtk_file_dialog_select_folder_finish( GTK_FILE_DIALOG(source_object), res, &error );
    if ( error != NULL )
    {
        /* User pressed cancel */
        U8_TRACE_INFO_STR( "unexpected response from file dialog:", error->message );
        g_error_free( error );
    }
    if ( result != NULL )
    {
        gchar *folder_path = g_file_get_path ( result );
        if ( folder_path != NULL )
        {
            u8_error_t export_err = U8_ERROR_NONE;
            data_stat_t export_stat;
            data_stat_init ( &export_stat );
            io_file_format_t selected_format = IO_FILE_FORMAT_NONE;

            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_pdf) )) { selected_format |= IO_FILE_FORMAT_PDF; }
            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_png) )) { selected_format |= IO_FILE_FORMAT_PNG; }
            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_ps) )) { selected_format |= IO_FILE_FORMAT_PS; }
            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_svg) )) { selected_format |= IO_FILE_FORMAT_SVG; }
            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_txt) )) { selected_format |= IO_FILE_FORMAT_TXT; }
            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_docbook) )) { selected_format |= IO_FILE_FORMAT_DOCBOOK; }
            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_html) )) { selected_format |= IO_FILE_FORMAT_HTML; }
            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_xmi2) )) { selected_format |= IO_FILE_FORMAT_XMI2; }
            if ( gtk_check_button_get_active( GTK_CHECK_BUTTON((*this_).format_json) )) { selected_format |= IO_FILE_FORMAT_JSON; }

            /* react immediately, handle events */
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

            /* determine the database file path for a document title and export: */
            const char *db_path = data_database_get_filename_ptr( (*this_).database );
            if ( data_database_is_open((*this_).database) )
            {
                export_err = io_exporter_export_files( &((*this_).file_exporter),
                                                       selected_format,
                                                       folder_path,
                                                       db_path,
                                                       &export_stat
                                                     );
            }
            else
            {
                export_err = U8_ERROR_NO_DB;
            }

            /* finished, notify user */
            char temp_format_buf[64];
            utf8stringbuf_t temp_fileformat = UTF8STRINGBUF( temp_format_buf );
            io_file_format_to_string( selected_format, temp_fileformat );

            if ( U8_ERROR_NONE == export_err )
            {
                gui_simple_message_to_user_show_message_with_names_and_stat( (*this_).message_to_user,
                                                                             GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                             GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED,
                                                                             utf8stringbuf_get_string( &temp_fileformat ),
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

            data_stat_trace( &export_stat );
            data_stat_destroy ( &export_stat );
            g_free (folder_path);
        }
        g_object_unref( result );
    }

    U8_TRACE_END();
}

#endif  /* GTK ? 4.10 */


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
