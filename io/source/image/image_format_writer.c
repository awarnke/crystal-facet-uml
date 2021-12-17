/* File: image_format_writer.c; Copyright and License: see below */

#include "image/image_format_writer.h"
#include "trace.h"
#include "tslog.h"
#include <gtk/gtk.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void image_format_writer_init ( image_format_writer_t *this_,
                                data_database_reader_t *db_reader,
                                data_visible_set_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != input_data );

    (*this_).db_reader = db_reader;
    (*this_).input_data = input_data;
    geometry_rectangle_init( &((*this_).bounds), 0.0, 0.0, 800.0, 600.0 );
    pencil_diagram_maker_init( &((*this_).painter), input_data );

    TRACE_END();
}

void image_format_writer_destroy( image_format_writer_t *this_ )
{
    TRACE_BEGIN();

    pencil_diagram_maker_destroy( &((*this_).painter) );
    geometry_rectangle_destroy(&((*this_).bounds));
    (*this_).input_data = NULL;
    (*this_).db_reader = NULL;

    TRACE_END();
}

#ifndef CAIRO_HAS_SVG_SURFACE
#error "no svg"
#endif

#ifndef CAIRO_HAS_PNG_FUNCTIONS
#error "no png"
#endif

int image_format_writer_render_diagram_to_file( image_format_writer_t *this_,
                                                data_id_t diagram_id,
                                                io_file_format_t export_type,
                                                const char* target_filename,
                                                data_stat_t *io_render_stat )
{
    TRACE_BEGIN();
    assert( NULL != target_filename );
    assert( NULL != io_render_stat );
    assert( IO_FILE_FORMAT_TXT != export_type );
    assert( data_id_get_table( &diagram_id ) == DATA_TABLE_DIAGRAM );
    const data_row_id_t diagram_row_id = data_id_get_row_id( &diagram_id );
    int result = 0;

    data_visible_set_init( (*this_).input_data );
    const u8_error_t d_err = data_visible_set_load( (*this_).input_data, diagram_row_id, (*this_).db_reader );
    if( d_err != U8_ERROR_NONE )
    {
        result = -1;
        assert(false);
    }
    assert( data_visible_set_is_valid ( (*this_).input_data ) );
    result |= image_format_writer_private_render_surface_to_file( this_, export_type, target_filename, io_render_stat );
    data_visible_set_destroy( (*this_).input_data );

    TRACE_END_ERR( result );
    return result;
}

int image_format_writer_private_render_surface_to_file( image_format_writer_t *this_,
                                                        io_file_format_t export_type,
                                                        const char* target_filename,
                                                        data_stat_t *io_render_stat )
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
        data_stat_t temp_stat;
        data_stat_init( &temp_stat );
        pencil_diagram_maker_define_grid ( &((*this_).painter), (*this_).bounds );
        pencil_diagram_maker_layout_elements ( &((*this_).painter), cr, &temp_stat );
#ifdef NDEBUG
        /* in release mode, do not report layouting warnings to the user */
        data_stat_reset_series( &temp_stat, DATA_STAT_SERIES_WARNING );
#endif
        data_stat_add( io_render_stat, &temp_stat );
        data_stat_destroy( &temp_stat );
        
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


/*
Copyright 2016-2021 Andreas Warnke

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
