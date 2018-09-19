/* File: gui_sketch_background.c; Copyright and License: see below */

#include "gui_sketch_background.h"
#include "meta/meta_info.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <assert.h>

void gui_sketch_background_init( gui_sketch_background_t *this_, gui_resources_t *resources )
{
    TRACE_BEGIN();

    (*this_).resources = resources;

    TRACE_END();
}

void gui_sketch_background_destroy( gui_sketch_background_t *this_ )
{
    TRACE_BEGIN();

    (*this_).resources = NULL;

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
static const double D_GREY_R = 0.4;
static const double D_GREY_G = 0.4;
static const double D_GREY_B = 0.4;
static const double D_GREY_A = 1.0;
static const double GREY_R = 0.7;
static const double GREY_G = 0.7;
static const double GREY_B = 0.7;
static const double GREY_A = 1.0;
static const double ORANGE_R = 1.0;
static const double ORANGE_G = 0.8;
static const double ORANGE_B = 0.5;
static const double ORANGE_A = 1.0;
static const double SKYBLUE_R = 0.6;
static const double SKYBLUE_G = 0.8;
static const double SKYBLUE_B = 1.0;
static const double SKYBLUE_A = 1.0;
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

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_crystal_facet_uml( (*this_).resources ),
                                                         "Welcome to",
                                                         META_INFO_PROGRAM_NAME_STR,
                                                         text_area_start+BORDER,
                                                         y+BORDER+48,
                                                         cr
                                                       );

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_file_use_db( (*this_).resources ),
                                                         "To begin, please",
                                                         "create or open a database file first.",
                                                         text_area_start+BORDER,
                                                         y+BORDER+96,
                                                         cr
                                                       );

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_message_user_doc( (*this_).resources ),
                                                         "The user manual crystal_facet_uml_user_documentation.pdf is available.",
                                                         "see http://www.andreaswarnke.de/crystal_facet_uml/html/index.html",
                                                         text_area_start+BORDER,
                                                         y+BORDER+192,
                                                         cr
                                                       );

    TRACE_END();
}

void gui_sketch_background_draw_navigation( gui_sketch_background_t *this_,
                                            unsigned int tree_depth,
                                            unsigned int num_children,
                                            bool create_mode,
                                            int x,
                                            int y,
                                            unsigned int width,
                                            unsigned int height,
                                            cairo_t *cr )
{
    TRACE_BEGIN();

    int block_top = y;
    if ( 0 == tree_depth )
    {
        cairo_set_source_rgba( cr, SKYBLUE_R, SKYBLUE_G, SKYBLUE_B, SKYBLUE_A );
    }
    else
    {
        cairo_set_source_rgba( cr, D_GREY_R, D_GREY_G, D_GREY_B, D_GREY_A );
    }
    cairo_rectangle ( cr, x, block_top, width, (height*3)/10 );
    cairo_fill (cr);
    block_top += (height*3)/10;

    cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
    cairo_rectangle ( cr, x, block_top, width, (height*4)/10 );
    cairo_fill (cr);
    block_top += (height*4)/10;

    if ( create_mode )
    {
        cairo_set_source_rgba( cr, ORANGE_R, ORANGE_G, ORANGE_B, ORANGE_A );
    }
    else
    {
        cairo_set_source_rgba( cr, D_GREY_R, D_GREY_G, D_GREY_B, D_GREY_A );
    }
    cairo_rectangle ( cr, x, block_top, width, height-block_top );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
    cairo_set_font_size ( cr, 12.0 );
    if ( tree_depth > 0 )
    {
        cairo_move_to ( cr, x+BORDER, y+BORDER + 14 );
        cairo_show_text ( cr, "parent diagram" );
    }
    cairo_move_to ( cr, x+BORDER, y+(height*3)/10 +BORDER+ 14 );
    cairo_show_text ( cr, "current diagram" );
    cairo_move_to ( cr, x+BORDER, y+(height*7)/10 +BORDER+ 14 );
    cairo_show_text ( cr, "children diagrams" );

    if ( ( 0 == tree_depth )&&( 0 == num_children ))
    {
        /* this is a new, empty database */
        /* print some guidance */
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_message_user_doc( (*this_).resources ),
                                                             "Quick",
                                                             "Intro:",
                                                             x+24+BORDER,
                                                             y+BORDER+48,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_tool_create_diagram( (*this_).resources ),
                                                             "Click to create a",
                                                             "diagram below the current.",
                                                             x+120+BORDER,
                                                             y+BORDER+0,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_tool_navigate( (*this_).resources ),
                                                             "Click to navigate to",
                                                             "parent/child diagrams.",
                                                             x+120+BORDER,
                                                             y+BORDER+48,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_tool_edit( (*this_).resources ),
                                                             "Single click to focus,",
                                                             "second click to mark items.",
                                                             x+320+BORDER,
                                                             y+BORDER+0,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_tool_create_object( (*this_).resources ),
                                                             "Click to create items.",
                                                             "Drag to create arrows.",
                                                             x+320+BORDER,
                                                             y+BORDER+48,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_edit_commit( (*this_).resources ),
                                                             "Changes are stored automatically.",
                                                             "Explicit safe action is optional.",
                                                             x+520+BORDER,
                                                             y+BORDER+0,
                                                             cr
        );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_file_export( (*this_).resources ),
                                                             "Select the output folder",
                                                             "to export all diagrams.",
                                                             x+520+BORDER,
                                                             y+BORDER+48,
                                                             cr
        );
    }

    TRACE_END();
}

void gui_sketch_background_draw_edit( gui_sketch_background_t *this_,
                                      bool create_mode,
                                      int x,
                                      int y,
                                      unsigned int width,
                                      unsigned int height,
                                      cairo_t *cr )
{
    TRACE_BEGIN();

    if ( create_mode )
    {
        cairo_set_source_rgba( cr, ORANGE_R, ORANGE_G, ORANGE_B, ORANGE_A );
    }
    else
    {
        cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
    }
    cairo_rectangle ( cr, x, y, width, height );
    cairo_fill (cr);

    TRACE_END();
}

void gui_sketch_background_private_draw_icon_and_message( gui_sketch_background_t *this_,
                                                          GdkPixbuf *icon_1,
                                                          const char *text_1,
                                                          const char *text_2,
                                                          int x,
                                                          int y,
                                                          cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != icon_1 );
    assert( NULL != text_1 );
    assert( NULL != text_2 );

    double icon_width = gdk_pixbuf_get_width ( icon_1 );
    double icon_height = gdk_pixbuf_get_height ( icon_1 );
    gdk_cairo_set_source_pixbuf( cr, icon_1, x, y );
    cairo_rectangle ( cr, x, y, x+icon_width, y+icon_height );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
    cairo_move_to ( cr, x+icon_width+BORDER, y + 14 );
    cairo_show_text ( cr, text_1 );
    cairo_move_to ( cr, x+icon_width+BORDER, y + 2*14 );
    cairo_show_text ( cr, text_2 );

    TRACE_END();
}


/*
Copyright 2017-2018 Andreas Warnke

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
