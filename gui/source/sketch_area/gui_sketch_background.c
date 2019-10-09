/* File: gui_sketch_background.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_background.h"
#include "meta/meta_info.h"
#include "trace.h"
#include <stdint.h>
#include <gtk/gtk.h>
#include <assert.h>

void gui_sketch_background_init( gui_sketch_background_t *this_, gui_resources_t *resources )
{
    TRACE_BEGIN();

    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );
    (*this_).resources = resources;

    TRACE_END();
}

void gui_sketch_background_destroy( gui_sketch_background_t *this_ )
{
    TRACE_BEGIN();

    shape_int_rectangle_destroy( &((*this_).bounds) );
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
static const double LIGHT_R = 0.8;
static const double LIGHT_G = 0.8;
static const double LIGHT_B = 0.8;
static const double LIGHT_A = 1.0;
static const double BORDER = 8;  /* border between text/icons and ground rectangle */

void gui_sketch_background_draw_introduction( gui_sketch_background_t *this_,
                                              cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    int32_t left;
    int32_t top;
    uint32_t width;
    uint32_t height;

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    height = shape_int_rectangle_get_height( &((*this_).bounds) );

    int text_area_start;

    /* if there is enough space, draw a nice picture bar on the left side */
    if ( width > 192 )
    {
        GdkPixbuf *bg_img = gui_resources_get_background_column( (*this_).resources );
        double icon_width = gdk_pixbuf_get_width ( bg_img );
        double icon_height = gdk_pixbuf_get_height ( bg_img );
        gdk_cairo_set_source_pixbuf( cr, bg_img, left, top );
        cairo_rectangle ( cr, left, top, icon_width, height );
        cairo_fill (cr);

        text_area_start = left+icon_width;

        if ( height > icon_height ) 
        {
            cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
            cairo_rectangle ( cr, left, top+icon_height, icon_width, height-icon_height );
            cairo_fill (cr);
        }
    }
    else
    {
        text_area_start = left;
    }

    cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
    cairo_rectangle ( cr, text_area_start, top, width-text_area_start, height );
    cairo_fill (cr);

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_crystal_facet_uml( (*this_).resources ),
                                                         "Welcome to",
                                                         META_INFO_PROGRAM_NAME_STR,
                                                         text_area_start+BORDER,
                                                         top+BORDER+48,
                                                         cr
                                                       );

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_file_use_db( (*this_).resources ),
                                                         "To begin, please",
                                                         "create or open a database file first.",
                                                         text_area_start+BORDER,
                                                         top+BORDER+96,
                                                         cr
                                                       );

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_message_user_doc( (*this_).resources ),
                                                         "The user manual crystal_facet_uml_user_documentation.pdf is available",
                                                         "in the net and locally at /usr/share/doc/(packages/)crystal-facet-uml",
                                                         text_area_start+BORDER,
                                                         top+BORDER+192,
                                                         cr
                                                       );

    TRACE_END();
}

void gui_sketch_background_draw_navigation( gui_sketch_background_t *this_,
                                            unsigned int tree_depth,
                                            unsigned int num_children,
                                            bool search_mode,
                                            cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    int32_t left;
    int32_t top;
    uint32_t width;
    uint32_t height;

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    height = shape_int_rectangle_get_height( &((*this_).bounds) );

    int block_top = top;
    if ( 0 == tree_depth )
    {
        cairo_set_source_rgba( cr, LIGHT_R, LIGHT_G, LIGHT_B, LIGHT_A );
    }
    else
    {
        cairo_set_source_rgba( cr, D_GREY_R, D_GREY_G, D_GREY_B, D_GREY_A );
    }
    cairo_rectangle ( cr, left, block_top, width, (height*1)/4 );
    cairo_fill (cr);
    block_top += (height*1)/4;

    cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
    cairo_rectangle ( cr, left, block_top, width, (height*2)/4 );
    cairo_fill (cr);
    block_top += (height*2)/4;

    cairo_set_source_rgba( cr, D_GREY_R, D_GREY_G, D_GREY_B, D_GREY_A );
    cairo_rectangle ( cr, left, block_top, width, height-block_top );
    cairo_fill (cr);

    if ( ( 0 == tree_depth )&&( 0 == num_children ))
    {
        /* this is a new, empty database */

        /* print some guidance */
        const int TAB_COL0_X = 14;
        const int TAB_COL1_X = 112;
        const int TAB_COL2_X = 320;
        const int TAB_ROW0_Y = 0;
        const int TAB_ROW1_Y = 48;
        const int TAB_ROW2_Y = 96;
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_message_user_doc( (*this_).resources ),
                                                             "Quick",
                                                             "Intro:",
                                                             left+TAB_COL0_X+BORDER,
                                                             top+BORDER+TAB_ROW1_Y,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_tool_navigate( (*this_).resources ),
                                                             "Click on a diagram to navigate,",
                                                             "on '+' to create a new diagram.",
                                                             left+TAB_COL1_X+BORDER,
                                                             top+BORDER+TAB_ROW0_Y,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_tool_edit( (*this_).resources ),
                                                             "Single click to focus,",
                                                             "second click to mark items.",
                                                             left+TAB_COL1_X+BORDER,
                                                             top+BORDER+TAB_ROW1_Y,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_tool_create( (*this_).resources ),
                                                             "Click to create items.",
                                                             "Drag to create arrows.",
                                                             left+TAB_COL1_X+BORDER,
                                                             top+BORDER+TAB_ROW2_Y,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_file_export( (*this_).resources ),
                                                             "Select the output folder",
                                                             "to export all(!) diagrams.",
                                                             left+TAB_COL2_X+BORDER,
                                                             top+BORDER+TAB_ROW1_Y,
                                                             cr
                                                           );
        gui_sketch_background_private_draw_icon_and_message( this_,
                                                             gui_resources_get_edit_commit( (*this_).resources ),
                                                             "Changes are stored automatically.",
                                                             "Explicit safe action is optional.",
                                                             left+TAB_COL2_X+BORDER,
                                                             top+BORDER+TAB_ROW2_Y,
                                                             cr
                                                           );
    }

    TRACE_END();
}

void gui_sketch_background_draw_edit( gui_sketch_background_t *this_,
                                      bool create_mode,
                                      cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    int32_t left;
    int32_t top;
    uint32_t width;
    uint32_t height;

    left = shape_int_rectangle_get_left( &((*this_).bounds) );
    top = shape_int_rectangle_get_top( &((*this_).bounds) );
    width = shape_int_rectangle_get_width( &((*this_).bounds) );
    height = shape_int_rectangle_get_height( &((*this_).bounds) );

    if ( create_mode )
    {
        cairo_set_source_rgba( cr, ORANGE_R, ORANGE_G, ORANGE_B, ORANGE_A );
    }
    else
    {
        cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
    }
    cairo_rectangle ( cr, left, top, width, height );
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
    assert( NULL != cr );
    assert( NULL != icon_1 );
    assert( NULL != text_1 );
    assert( NULL != text_2 );

    double icon_width = gdk_pixbuf_get_width ( icon_1 );
    double icon_height = gdk_pixbuf_get_height ( icon_1 );
    gdk_cairo_set_source_pixbuf( cr, icon_1, x, y );
    cairo_rectangle ( cr, x, y, x+icon_width, y+icon_height );
    cairo_fill (cr);

    cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
    cairo_set_font_size ( cr, 12.0 );
    cairo_move_to ( cr, x+icon_width+BORDER, y + 14 );
    cairo_show_text ( cr, text_1 );
    cairo_move_to ( cr, x+icon_width+BORDER, y + 2*14 );
    cairo_show_text ( cr, text_2 );

    TRACE_END();
}


/*
Copyright 2017-2019 Andreas Warnke

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
