/* File: gui_sketch_background.c; Copyright and License: see below */

#include "gui_sketch_background.h"
#include "meta/meta_info.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <assert.h>

void gui_sketch_background_init( gui_sketch_background_t *this_, gui_resources_t *res )
{
    TRACE_BEGIN();

    (*this_).res = res;

    TRACE_END();
}

void gui_sketch_background_destroy( gui_sketch_background_t *this_ )
{
    TRACE_BEGIN();

    (*this_).res = NULL;

    TRACE_END();
}

void gui_sketch_background_draw_introduction( gui_sketch_background_t *this_,
                                              int x,
                                              int y,
                                              unsigned int width,
                                              unsigned int height,
                                              cairo_t *cr )
{
    TRACE_BEGIN();

    cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
    cairo_rectangle ( cr, x, y, 64, height );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, 0.7, 0.7, 0.7, 1.0 );
    cairo_rectangle ( cr, x+64, y, width-64, height );
    cairo_fill (cr);

    GdkPixbuf *cfu_icon = gui_resources_get_crystal_facet_uml( (*this_).res );
    double cfu_icon_width = gdk_pixbuf_get_width ( cfu_icon );
    double cfu_icon_height = gdk_pixbuf_get_height ( cfu_icon );
    gdk_cairo_set_source_pixbuf( cr, cfu_icon, x+72.0, y+48.0 );
    cairo_rectangle ( cr, x+72, y+48, 72 + cfu_icon_width, 48 + cfu_icon_height );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
    cairo_set_font_size ( cr, 12.0 );
    cairo_move_to ( cr, x+72 + cfu_icon_width + 8, y+48 + 14 );
    cairo_show_text ( cr, "Welcome to" );
    cairo_move_to ( cr, x+72 + cfu_icon_width + 8, y+48 + 2*14 );
    cairo_show_text ( cr, META_INFO_PROGRAM_NAME_STR );

    GdkPixbuf *use_db_icon = gui_resources_get_file_use_db( (*this_).res );
    double use_db_icon_width = gdk_pixbuf_get_width ( use_db_icon );
    double use_db_icon_height = gdk_pixbuf_get_height ( use_db_icon );
    gdk_cairo_set_source_pixbuf( cr, use_db_icon, x+72.0, y+96.0 );
    cairo_rectangle ( cr, x+72, y+96, 72 + use_db_icon_width, 96 + use_db_icon_height );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
    cairo_move_to ( cr, x+72 + use_db_icon_width + 8, y+96 + 14 );
    cairo_show_text ( cr, "To begin, please" );
    cairo_move_to ( cr, x+72 + use_db_icon_width + 8, y+96 + 2*14 );
    cairo_show_text ( cr, "create or open a database file first." );

    TRACE_END();
}

void gui_sketch_background_draw_navigation( gui_sketch_background_t *this_,
                                            int x,
                                            int y,
                                            unsigned int width,
                                            unsigned int height,
                                            cairo_t *cr )
{
    TRACE_BEGIN();

    cairo_set_source_rgba( cr, 0.4, 0.4, 0.4, 1.0 );
    cairo_rectangle ( cr, x, y, width, (height*3)/10 );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
    cairo_rectangle ( cr, x, y+(height*3)/10, width, (height*4)/10 );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, 0.4, 0.4, 0.4, 1.0 );
    cairo_rectangle ( cr, x, y+(height*7)/10, width, (height*3)/10 );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
    cairo_set_font_size ( cr, 12.0 );
    cairo_move_to ( cr, x+8, y+8 + 14 );
    cairo_show_text ( cr, "parent diagram" );
    cairo_move_to ( cr, x+8, y+(height*3)/10 + 8 + 14 );
    cairo_show_text ( cr, "current diagram" );
    cairo_move_to ( cr, x+8, y+(height*7)/10 + 8 + 14 );
    cairo_show_text ( cr, "children diagrams" );

    TRACE_END();
}

void gui_sketch_background_draw_edit( gui_sketch_background_t *this_,
                                      int x,
                                      int y,
                                      unsigned int width,
                                      unsigned int height,
                                      cairo_t *cr )
{
    TRACE_BEGIN();

    cairo_set_source_rgba( cr, 0.3, 0.3, 0.3, 1.0 );
    cairo_rectangle ( cr, x, y, width, height );
    cairo_fill (cr);

    TRACE_END();
}


/*
Copyright 2017-2017 Andreas Warnke

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
