/* File: gui_file_exporter.c; Copyright and License: see below */

#include "gui_file_exporter.h"
#include "trace.h"
#include "tslog.h"
#include <gtk/gtk.h>
#include <cairo-svg.h>
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
    (*this_).temp_filename = utf8stringbuf_init( sizeof((*this_).temp_filename_buf), (*this_).temp_filename_buf );

    geometry_rectangle_init( &((*this_).bounds), 0.0, 0.0, 800.0, 600.0 );
    pencil_input_data_init( &((*this_).painter_input_data) );
    pencil_diagram_painter_init( &((*this_).painter), &((*this_).painter_input_data) );
    pencil_description_writer_init( &((*this_).description_writer), &((*this_).painter_input_data) );

    TRACE_END();
}

void gui_file_exporter_destroy( gui_file_exporter_t *this_ )
{
    TRACE_BEGIN();

    pencil_description_writer_destroy( &((*this_).description_writer) );
    pencil_diagram_painter_destroy( &((*this_).painter) );
    pencil_input_data_destroy( &((*this_).painter_input_data) );
    geometry_rectangle_destroy(&((*this_).bounds));

    (*this_).db_reader = NULL;
    (*this_).message_to_user = NULL;

    TRACE_END();
}

void gui_file_exporter_export_response_callback( GtkDialog *dialog, gint response_id, gpointer user_data )
{
    TRACE_BEGIN();
    gui_file_exporter_t *this_ = user_data;
    int export_err;

    switch ( response_id )
    {
        case GTK_RESPONSE_CANCEL:
        {
            TSLOG_EVENT( "GTK_RESPONSE_CANCEL" );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
        }
        break;

        case GUI_FILE_EXPORTER_CONST_EXPORT_PNG:
        {
            TSLOG_EVENT( "GUI_FILE_EXPORTER_CONST_EXPORT_PNG" );
            gchar *folder_path;
            folder_path = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
            TRACE_INFO_STR( "chosen folder:", folder_path );

            export_err = gui_file_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 8, GUI_FILE_EXPORT_FORMAT_PNG, folder_path );

            g_free (folder_path);

            if ( 0 == export_err )
            {
                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED,
                                                                     "png"
                                                                   );
            }
            else
            {
                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED,
                                                                     "png"
                                                                   );
            }
        }
        break;

        case GUI_FILE_EXPORTER_CONST_EXPORT_SVG:
        {
            TSLOG_EVENT( "GUI_FILE_EXPORTER_CONST_EXPORT_SVG" );
            gchar *folder_path;
            folder_path = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
            TRACE_INFO_STR( "chosen folder:", folder_path );

            export_err = gui_file_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 8, GUI_FILE_EXPORT_FORMAT_SVG, folder_path );

            g_free (folder_path);

            if ( 0 == export_err )
            {
                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED,
                                                                     "svg"
                );
            }
            else
            {
                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED,
                                                                     "svg"
                                                                   );
            }
        }
        break;

        case GUI_FILE_EXPORTER_CONST_EXPORT_TXT:
        {
            TSLOG_EVENT( "GUI_FILE_EXPORTER_CONST_EXPORT_TXT" );
            gchar *folder_path;
            folder_path = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER(dialog) );
            gtk_widget_hide( GTK_WIDGET ( dialog ) );
            TRACE_INFO_STR( "chosen folder:", folder_path );

            export_err = gui_file_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 8, GUI_FILE_EXPORT_FORMAT_TXT, folder_path );

            g_free (folder_path);

            if ( 0 == export_err )
            {
                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED,
                                                                     "txt"
                );
            }
            else
            {
                gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                     GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                     GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED,
                                                                     "txt"
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
    assert( NULL != target_folder );
    int result = 0;

    /* draw current diagram */
    if ( DATA_ID_VOID_ID != diagram_id )
    {
        /* load data to be drawn */
        pencil_input_data_load( &((*this_).painter_input_data), diagram_id, (*this_).db_reader );
        data_diagram_t *diag_ptr;
        diag_ptr = pencil_input_data_get_diagram_ptr ( &((*this_).painter_input_data) );
        const char *diag_name;
        diag_name = data_diagram_get_name_ptr( diag_ptr );

        /* determine filename */
        utf8stringbuf_copy_str( (*this_).temp_filename, target_folder );
        utf8stringbuf_append_str( (*this_).temp_filename, "/" );
        utf8stringbuf_append_int( (*this_).temp_filename, diagram_id );
        utf8stringbuf_append_str( (*this_).temp_filename, "_" );
        gui_file_exporter_private_append_valid_chars_to_filename( this_, diag_name, (*this_).temp_filename );
        if ( GUI_FILE_EXPORT_FORMAT_SVG == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".svg" );
        }
        else if ( GUI_FILE_EXPORT_FORMAT_PNG == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".png" );
        }
        else /* GUI_FILE_EXPORT_FORMAT_TXT */
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".txt" );
        }
        TSLOG_EVENT_STR( "exporting diagram to file:", utf8stringbuf_get_string( (*this_).temp_filename ) );

        /* create surface */
        cairo_surface_t *surface;
        FILE *text_output;
        if ( GUI_FILE_EXPORT_FORMAT_SVG == export_type )
        {
            surface = (cairo_surface_t *) cairo_svg_surface_create( utf8stringbuf_get_string( (*this_).temp_filename ),
                                                                    geometry_rectangle_get_width( &((*this_).bounds) ),
                                                                    geometry_rectangle_get_height( &((*this_).bounds) )
                                                                  );
        }
        else if ( GUI_FILE_EXPORT_FORMAT_PNG == export_type )
        {
            surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32,
                                                  (uint32_t) geometry_rectangle_get_width( &((*this_).bounds) ),
                                                  (uint32_t) geometry_rectangle_get_height( &((*this_).bounds) )
                                                );
        }
        else /* GUI_FILE_EXPORT_FORMAT_TXT */
        {
            text_output = fopen( utf8stringbuf_get_string( (*this_).temp_filename ), "w" );
            if ( NULL == text_output )
            {
                TSLOG_ERROR("error creating txt.");
                result = -1;
            }
        }

        /* draw on surface */
        if ( GUI_FILE_EXPORT_FORMAT_TXT == export_type )
        {
            if ( NULL != text_output )
            {
                int write_err;
                write_err = pencil_description_writer_draw ( &((*this_).description_writer), text_output );
                if ( 0 != write_err )
                {
                    TSLOG_ERROR("error writing txt.");
                    result = -1;
                }
            }
        }
        else if ( CAIRO_STATUS_SUCCESS != cairo_surface_status( surface ) )
        {
            TSLOG_ERROR_INT( "surface could not be created", cairo_surface_status( surface ) );
            result = -1;
        }
        else
        {
            cairo_t *cr;
            cr = cairo_create (surface);

            /* draw diagram */
            /* draw paper */
            cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left( &((*this_).bounds) ),
                              geometry_rectangle_get_top( &((*this_).bounds) ),
                              geometry_rectangle_get_width( &((*this_).bounds) ),
                              geometry_rectangle_get_height( &((*this_).bounds) )
                            );
            cairo_fill (cr);

            /* layout diagram */
            pencil_diagram_painter_do_layout ( &((*this_).painter),
                                               &((*this_).painter_input_data),
                                               (*this_).bounds
            );

            /* draw the current diagram */
            data_id_t void_id;
            data_id_init_void( &void_id );
            data_small_set_t void_set;
            data_small_set_init( &void_set );
            pencil_diagram_painter_draw ( &((*this_).painter),
                                          void_id,
                                          void_id,
                                          &void_set,
                                          cr
            );

            /* finish surface */
            cairo_destroy (cr);
        }

        /* finish surface */
        if ( GUI_FILE_EXPORT_FORMAT_SVG == export_type )
        {
            cairo_surface_finish ( surface );
            cairo_surface_destroy ( surface );
        }
        else if ( GUI_FILE_EXPORT_FORMAT_PNG == export_type )
        {
            cairo_status_t png_result;
            png_result = cairo_surface_write_to_png ( surface, utf8stringbuf_get_string( (*this_).temp_filename ) );
            if ( CAIRO_STATUS_SUCCESS != png_result )
            {
                TSLOG_ERROR("error writing png.");
                result = -1;
            }
            cairo_surface_destroy ( surface );
        }
        else /* GUI_FILE_EXPORT_FORMAT_TXT */
        {
            if ( NULL != text_output )
            {
                int close_err;
                close_err = fclose( text_output );
                if ( 0 != close_err )
                {
                    TSLOG_ERROR("error finishing txt.");
                    result = -1;
                }
            }
        }
    }

    /* recursive calls of children */
    if ( max_recursion > 0 )
    {
        data_error_t db_err;
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        db_err = data_database_reader_get_diagram_ids_by_parent_id ( (*this_).db_reader, diagram_id, &the_set );
        if ( db_err != DATA_ERROR_NONE )
        {
            TSLOG_ERROR("error reading database.");
            result = -1;
        }
        else
        {
            for ( uint32_t pos = 0; pos < data_small_set_get_count( &the_set ); pos ++ )
            {
                data_id_t probe;
                probe = data_small_set_get_id( &the_set, pos );
                int64_t probe_row_id;
                probe_row_id = data_id_get_row_id( &probe );

                result |= gui_file_exporter_private_export_image_files( this_, probe_row_id, max_recursion-1, export_type, target_folder );

                data_id_destroy( &probe );
            }
        }
        data_small_set_destroy( &the_set );
    }

    TRACE_END_ERR( result );
    return result;
}

void gui_file_exporter_private_append_valid_chars_to_filename( gui_file_exporter_t *this_, const char* name, utf8stringbuf_t filename )
{
    TRACE_BEGIN();
    assert( NULL != name );
    TRACE_INFO_STR( "name:", name );

    bool finished = false;
    static const int MAX_APPEND_CHARS = 64;
    for ( int pos = 0; ( pos < MAX_APPEND_CHARS ) && ( ! finished ); pos ++ )
    {
        char probe = name[pos];
        if ( probe == '\0' )
        {
            finished = true;
        }
        else if (( 'A' <= probe ) && ( probe <= 'Z' ))
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if (( 'a' <= probe ) && ( probe <= 'z' ))
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if (( '0' <= probe ) && ( probe <= '9' ))
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if ( '-' == probe )
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if ( '_' == probe )
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if ( ' ' == probe )
        {
            utf8stringbuf_append_char( filename, '_' );
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
