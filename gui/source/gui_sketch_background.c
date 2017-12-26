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

static const double BLACK_R = 0.0;
static const double BLACK_G = 0.0;
static const double BLACK_B = 0.0;
static const double BLACK_A = 1.0;
static const double DARK_R = 0.3;
static const double DARK_G = 0.3;
static const double DARK_B = 0.3;
static const double DARK_A = 1.0;
static const double GREY_R = 0.7;
static const double GREY_G = 0.7;
static const double GREY_B = 0.7;
static const double GREY_A = 1.0;
static const double BORDER = 8;  /* border between text/icons and ground rectangle */

void gui_sketch_background_draw_introduction( gui_sketch_background_t *this_,
                                              int x,
                                              int y,
                                              unsigned int width,
                                              unsigned int height,
                                              cairo_t *cr )
{
    TRACE_BEGIN();

    int text_area_start;

    if ( width > 192 )
    {
        text_area_start = 64;

        cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
        cairo_rectangle ( cr, x, y, 64, height );
        cairo_fill (cr);
    }
    else
    {
        text_area_start = 0;
    }

    cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
    cairo_rectangle ( cr, text_area_start, y, width-text_area_start, height );
    cairo_fill (cr);

    GdkPixbuf *cfu_icon = gui_resources_get_crystal_facet_uml( (*this_).res );
    double cfu_icon_width = gdk_pixbuf_get_width ( cfu_icon );
    double cfu_icon_height = gdk_pixbuf_get_height ( cfu_icon );
    gdk_cairo_set_source_pixbuf( cr, cfu_icon, text_area_start+BORDER, y+BORDER+48.0 );
    cairo_rectangle ( cr, text_area_start+BORDER, y+BORDER+48,
                      text_area_start+BORDER+cfu_icon_width, y+BORDER+48+cfu_icon_height );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
    cairo_set_font_size ( cr, 12.0 );
    cairo_move_to ( cr, text_area_start+BORDER+cfu_icon_width+BORDER, y+BORDER+48 + 14 );
    cairo_show_text ( cr, "Welcome to" );
    cairo_move_to ( cr, text_area_start+BORDER+cfu_icon_width+BORDER, y+BORDER+48 + 2*14 );
    cairo_show_text ( cr, META_INFO_PROGRAM_NAME_STR );

    GdkPixbuf *use_db_icon = gui_resources_get_file_use_db( (*this_).res );
    double use_db_icon_width = gdk_pixbuf_get_width ( use_db_icon );
    double use_db_icon_height = gdk_pixbuf_get_height ( use_db_icon );
    gdk_cairo_set_source_pixbuf( cr, use_db_icon, text_area_start+BORDER, y+BORDER+96.0 );
    cairo_rectangle ( cr, text_area_start+BORDER, y+BORDER+96,
                      text_area_start+BORDER+use_db_icon_width, y+BORDER+96+use_db_icon_height );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
    cairo_move_to ( cr, text_area_start+BORDER+use_db_icon_width+BORDER, y+BORDER+96 + 14 );
    cairo_show_text ( cr, "To begin, please" );
    cairo_move_to ( cr, text_area_start+BORDER+use_db_icon_width+BORDER, y+BORDER+96 + 2*14 );
    cairo_show_text ( cr, "create or open a database file first." );

    GdkPixbuf *message_user_doc_icon = gui_resources_get_message_user_doc( (*this_).res );
    double message_user_doc_width = gdk_pixbuf_get_width ( message_user_doc_icon );
    double message_user_doc_height = gdk_pixbuf_get_height ( message_user_doc_icon );
    gdk_cairo_set_source_pixbuf( cr, message_user_doc_icon, text_area_start+BORDER, y+BORDER+192.0 );
    cairo_rectangle ( cr, text_area_start+BORDER, y+BORDER+192,
                      text_area_start+BORDER+message_user_doc_width, y+BORDER+192+message_user_doc_height );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
    cairo_move_to ( cr, text_area_start+BORDER+message_user_doc_width+BORDER, y+BORDER+192 + 14 );
    cairo_show_text ( cr, "The user manual crystal_facet_uml_user_documentation.pdf is available." );
    cairo_move_to ( cr, text_area_start+BORDER+message_user_doc_width+BORDER, y+BORDER+192 + 2*14 );
    cairo_show_text ( cr, "see http://www.andreaswarnke.de/crystal_facet_uml/html/index.html" );

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

    cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
    cairo_rectangle ( cr, x, y+(height*3)/10, width, (height*4)/10 );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, 0.4, 0.4, 0.4, 1.0 );
    cairo_rectangle ( cr, x, y+(height*7)/10, width, (height*3)/10 );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
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

    cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
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
