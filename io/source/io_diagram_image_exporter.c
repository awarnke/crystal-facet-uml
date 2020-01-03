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
                                      data_visible_set_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    //(*this_).input_data = input_data;
    geometry_rectangle_init( &((*this_).bounds), 0.0, 0.0, 800.0, 600.0 );
    pencil_diagram_maker_init( &((*this_).painter), input_data );

    TRACE_END();
}

void io_diagram_image_exporter_destroy( io_diagram_image_exporter_t *this_ )
{
    TRACE_BEGIN();

    pencil_diagram_maker_destroy( &((*this_).painter) );
    geometry_rectangle_destroy(&((*this_).bounds));
    //(*this_).input_data = NULL;

    TRACE_END();
}

#ifndef CAIRO_HAS_SVG_SURFACE
#error "no svg"
#endif

#ifndef CAIRO_HAS_PNG_FUNCTIONS
#error "no png"
#endif

int io_diagram_image_exporter_render_surface_to_file( io_diagram_image_exporter_t *this_,
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
        pencil_diagram_maker_define_grid ( &((*this_).painter),
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


/*
Copyright 2016-2020 Andreas Warnke

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
