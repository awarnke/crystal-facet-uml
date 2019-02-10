/* File: io_diagram_image_exporter.c; Copyright and License: see below */

#include "io_diagram_image_exporter.h"
#include "trace.h"
#include "tslog.h"
#include <gtk/gtk.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void io_diagram_image_exporter_init ( io_diagram_image_exporter_t *this_,
                                      data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    (*this_).db_reader = db_reader;
    (*this_).temp_filename = utf8stringbuf_init( sizeof((*this_).temp_filename_buf), (*this_).temp_filename_buf );

    geometry_rectangle_init( &((*this_).bounds), 0.0, 0.0, 800.0, 600.0 );
    pencil_input_data_init( &((*this_).painter_input_data) );
    pencil_diagram_maker_init( &((*this_).painter), &((*this_).painter_input_data) );
    pencil_description_writer_init( &((*this_).description_writer), &((*this_).painter_input_data) );

    TRACE_END();
}

void io_diagram_image_exporter_destroy( io_diagram_image_exporter_t *this_ )
{
    TRACE_BEGIN();

    pencil_description_writer_destroy( &((*this_).description_writer) );
    pencil_diagram_maker_destroy( &((*this_).painter) );
    pencil_input_data_destroy( &((*this_).painter_input_data) );
    geometry_rectangle_destroy(&((*this_).bounds));

    (*this_).db_reader = NULL;

    TRACE_END();
}

int io_diagram_image_exporter_export_files( io_diagram_image_exporter_t *this_,
                                            io_file_format_t export_type,
                                            const char* target_folder )
{
    TRACE_BEGIN();
    int export_err = 0;

    if ( ( export_type & IO_FILE_FORMAT_SVG ) != 0 )
    {
        export_err |= io_diagram_image_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_SVG, target_folder );
    }

    if ( ( export_type & IO_FILE_FORMAT_PDF ) != 0 )
    {
        export_err |= io_diagram_image_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_PDF, target_folder );
    }

    if ( ( export_type & IO_FILE_FORMAT_PS ) != 0 )
    {
        export_err |= io_diagram_image_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_PS, target_folder );
    }

    if ( ( export_type & IO_FILE_FORMAT_PNG ) != 0 )
    {
        export_err |= io_diagram_image_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_PNG, target_folder );
    }

    if ( ( export_type & IO_FILE_FORMAT_TXT ) != 0 )
    {
        export_err |= io_diagram_image_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_TXT, target_folder );
    }

    TRACE_END_ERR(export_err);
    return export_err;
}

#ifndef CAIRO_HAS_SVG_SURFACE
#error "no svg"
#endif

#ifndef CAIRO_HAS_PNG_FUNCTIONS
#error "no png"
#endif

int io_diagram_image_exporter_private_export_image_files( io_diagram_image_exporter_t *this_,
                                                  int64_t diagram_id,
                                                  uint32_t max_recursion,
                                                  io_file_format_t export_type,
                                                  const char* target_folder )
{
    TRACE_BEGIN();
    int result = 0;

    /* draw current diagram */
    if (( DATA_ID_VOID_ID != diagram_id )&&( NULL != target_folder ))
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
        {
            data_id_t the_id;
            data_id_init( &the_id, DATA_TABLE_DIAGRAM, diagram_id );
            data_id_to_utf8stringbuf( &the_id, (*this_).temp_filename );
        }
        utf8stringbuf_append_str( (*this_).temp_filename, "_" );
        io_diagram_image_exporter_private_append_valid_chars_to_filename( this_, diag_name, (*this_).temp_filename );
        if ( IO_FILE_FORMAT_SVG == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".svg" );
        }
        else if ( IO_FILE_FORMAT_PNG == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".png" );
        }
        else if ( IO_FILE_FORMAT_PDF == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".pdf" );
        }
        else if ( IO_FILE_FORMAT_PS == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".ps" );
        }
        else /* IO_FILE_FORMAT_TXT */
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".txt" );
        }
        TSLOG_EVENT_STR( "exporting diagram to file:", utf8stringbuf_get_string( (*this_).temp_filename ) );

        /* create surface */
        if (( IO_FILE_FORMAT_SVG == export_type )
            || ( IO_FILE_FORMAT_PDF == export_type )
            || ( IO_FILE_FORMAT_PS == export_type )
            || ( IO_FILE_FORMAT_PNG == export_type ) )
        {
            result = io_diagram_image_exporter_private_render_surface_to_file( this_,
                                                                       export_type,
                                                                       utf8stringbuf_get_string( (*this_).temp_filename )
                                                                     );
        }
        else /* IO_FILE_FORMAT_TXT */
        {
            FILE *text_output;

            /* open file */
            text_output = fopen( utf8stringbuf_get_string( (*this_).temp_filename ), "w" );
            if ( NULL == text_output )
            {
                TSLOG_ERROR("error creating txt.");
                result = -1;
            }
            else
            {
                /* write file */
                int write_err;
                write_err = pencil_description_writer_draw ( &((*this_).description_writer), text_output );
                if ( 0 != write_err )
                {
                    TSLOG_ERROR("error writing txt.");
                    result = -1;
                }

                /* close file */
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

    /* recursion to children */
    if ( NULL != target_folder )
    {
        /* recursive calls of children */
        if (( result == 0 )&&( max_recursion > 0 ))
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

                    result |= io_diagram_image_exporter_private_export_image_files( this_, probe_row_id, max_recursion-1, export_type, target_folder );

                    data_id_destroy( &probe );
                }
            }
            data_small_set_destroy( &the_set );
        }
    }
    else
    {
        TSLOG_WARNING("io_diagram_image_exporter_private_export_image_files called with target_folder==NULL");
        result = -1;
    }

    TRACE_END_ERR( result );
    return result;
}

int io_diagram_image_exporter_private_render_surface_to_file( io_diagram_image_exporter_t *this_,
                                                              io_file_format_t export_type,
                                                              const char* target_filename )
{
    TRACE_BEGIN();
    assert( NULL != target_filename );
    assert( IO_FILE_FORMAT_TXT != export_type );
    int result = 0;

    /* create surface */
    cairo_surface_t *surface;
    if ( IO_FILE_FORMAT_SVG == export_type )
    {
        surface = (cairo_surface_t *) cairo_svg_surface_create( target_filename,
                                                                geometry_rectangle_get_width( &((*this_).bounds) ),
                                                                geometry_rectangle_get_height( &((*this_).bounds) )
                                                              );
    }
    else if ( IO_FILE_FORMAT_PDF == export_type )
    {
        surface = (cairo_surface_t *) cairo_pdf_surface_create ( target_filename,
                                                                 geometry_rectangle_get_width( &((*this_).bounds) ),
                                                                 geometry_rectangle_get_height( &((*this_).bounds) )
                                                               );
    }
    else if ( IO_FILE_FORMAT_PS == export_type )
    {
        surface = (cairo_surface_t *) cairo_ps_surface_create ( target_filename,
                                                                geometry_rectangle_get_width( &((*this_).bounds) ),
                                                                geometry_rectangle_get_height( &((*this_).bounds) )
                                                              );
    }
    else /*if ( IO_FILE_FORMAT_PNG == export_type )*/
    {
        surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32,
                                              (uint32_t) geometry_rectangle_get_width( &((*this_).bounds) ),
                                              (uint32_t) geometry_rectangle_get_height( &((*this_).bounds) )
                                            );
    }

    /* draw on surface */
    if ( CAIRO_STATUS_SUCCESS != cairo_surface_status( surface ) )
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
        pencil_diagram_maker_layout_grid ( &((*this_).painter),
                                           &((*this_).painter_input_data),
                                           (*this_).bounds
                                         );
        pencil_diagram_maker_layout_elements ( &((*this_).painter), cr );

        /* draw the current diagram */
        data_id_t void_id;
        data_id_init_void( &void_id );
        data_small_set_t void_set;
        data_small_set_init( &void_set );
        pencil_diagram_maker_draw ( &((*this_).painter),
                                    void_id,
                                    void_id,
                                    &void_set,
                                    cr
                                  );

        /* finish drawing context */
        cairo_destroy (cr);

        /* finish surface */
        if ( IO_FILE_FORMAT_PNG == export_type )
        {
            cairo_status_t png_result;
            png_result = cairo_surface_write_to_png ( surface, target_filename );
            if ( CAIRO_STATUS_SUCCESS != png_result )
            {
                TSLOG_ERROR("error writing png.");
                result = -1;
            }
        }
        else
        {
            cairo_surface_finish ( surface );
        }
    }

    cairo_surface_destroy ( surface );

    TRACE_END_ERR( result );
    return result;
}

void io_diagram_image_exporter_private_append_valid_chars_to_filename( io_diagram_image_exporter_t *this_,
                                                                       const char* name,
                                                                       utf8stringbuf_t filename )
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
Copyright 2016-2019 Andreas Warnke

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
