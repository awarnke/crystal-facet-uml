/* File: gui_file_exporter.c; Copyright and License: see below */

#include "gui_file_exporter.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void gui_file_exporter_init ( gui_file_exporter_t *this_,
                             data_database_reader_t *db_reader,
                             gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != message_to_user );

    (*this_).db_reader = db_reader;
    (*this_).message_to_user = message_to_user;

    TRACE_END();
}

void gui_file_exporter_destroy( gui_file_exporter_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).message_to_user = NULL;

    TRACE_END();
}

void gui_file_exporter_export_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_file_exporter_t *this_ = user_data;

    switch ( response_id )
    {
        case GTK_RESPONSE_CANCEL:
        {
            TSLOG_EVENT( "GTK_RESPONSE_CANCEL" );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
        }
        break;

        case GUI_FILEMANAGER_CONST_EXPORT_PNG:
        {
            TSLOG_EVENT( "GUI_FILEMANAGER_CONST_EXPORT_PNG" );
            gchar *filename;
            filename = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
            TRACE_INFO_STR( "File chosen:", filename );

            gui_file_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 8, GUI_FILE_EXPORT_FORMAT_PNG, filename );

            g_free (filename);

            gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                 GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED,
                                                                 "Export png"
                                                               );
        }
        break;

        case GUI_FILEMANAGER_CONST_EXPORT_SVG:
        {
            TSLOG_EVENT( "GUI_FILEMANAGER_CONST_EXPORT_SVG" );
            gchar *filename;
            filename = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
            TRACE_INFO_STR( "File chosen:", filename );

            gui_file_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 8, GUI_FILE_EXPORT_FORMAT_SVG, filename );

            g_free (filename);

            gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                 GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED,
                                                                 "Export svg"
            );
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

#ifndef CAIRO_HAS_SVG_SURFACE
#error "no svg"
#endif

#ifndef CAIRO_HAS_PNG_FUNCTIONS
#error "no png"
#endif

int gui_file_exporter_private_export_image_files( gui_file_exporter_t *this_,
                                                 int64_t diagram_id,
                                                 uint32_t max_recursion,
                                                 gui_file_export_format_t export_type,
                                                 const char* target_folder )
{
    TRACE_BEGIN();
    int result = 0;


    TRACE_INFO_STR( "target_folder chosen:", target_folder );



    data_error_t db_err;
    data_small_set_t the_set;
    data_small_set_init( &the_set );
    db_err = data_database_reader_get_diagram_ids_by_parent_id ( (*this_).db_reader, DATA_ID_VOID_ID, &the_set );
    for ( uint32_t pos = 0; pos < data_small_set_get_count( &the_set ); pos ++ )
    {
        data_id_t probe;
        probe = data_small_set_get_id( &the_set, pos );
        data_id_trace( &probe );
        data_id_destroy( &probe );
    }
    data_small_set_destroy( &the_set );


    /*
     *
       cairo_status_t
       cairo_surface_write_to_png (cairo_surface_t *surfa*ce,
       const char *filename);


       cairo_surface_t *surface;
       cairo_t *cr;
       surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 80);
       cr = cairo_create (surface);
       cairo_set_font_size (cr, 32.0);
       cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
       cairo_move_to (cr, 10.0, 50.0);
       cairo_show_text (cr, "Hello, world");
       cairo_destroy (cr);
       cairo_surface_write_to_png (surface, "hello.png");
       cairo_surface_destroy (surface);



       cairo_surface_t *
       cairo_svg_surface_create (const char *filename,
       double width_in_points,
       double height_in_points);
     */


    TRACE_END_ERR( result );
    return result;
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
