/* File: gui_sketch_background.c; Copyright and License: see below */

#include "sketch/gui_sketch_background.h"
#include "meta/meta_info.h"
#include "u8/u8_trace.h"
#include <stdint.h>
#include "gui_gtk.h"
#include <assert.h>

#if 0
/* to convert the source file from e.g. char to int, you may uncomment this here: */
void gui_sketch_background_private_write_as_ints( gui_sketch_background_t *this_ )
{
    fprintf(stdout,"%s","\n    ");
    cairo_surface_t *bg_img = gui_resources_get_sketch_background( (*this_).resources );
    int icon_width = cairo_image_surface_get_width( bg_img );
    int icon_height = cairo_image_surface_get_height( bg_img );
    unsigned char *data = cairo_image_surface_get_data( bg_img );
    for ( int y = 0; y < icon_height; y++ )
    {
        for ( int x = 0; x < icon_width; x++ )
        {
            guchar b = data[(4*(x+(y*icon_width)))+0];
            guchar g = data[(4*(x+(y*icon_width)))+1];
            guchar r = data[(4*(x+(y*icon_width)))+2];
            guchar a = data[(4*(x+(y*icon_width)))+3];
            fprintf(stdout,"O3R(%3d,%3d,%3d,%3d), ", r, g, b, a);
            if ( (x & 7) == 7 )
            {
                fprintf(stdout,"%s","\n    ");
            }
        }
    }
}
#endif

void gui_sketch_background_init( gui_sketch_background_t *this_,
                                 const gui_resources_t *resources,
                                 gui_sketch_texture_t *texture_downloader )
{
    U8_TRACE_BEGIN();
    assert( resources != NULL );
    assert( texture_downloader != NULL );

    shape_int_rectangle_init( &((*this_).card_bounds), 0, 0, 0, 0 );
    shape_int_rectangle_init( &((*this_).label_bounds), 0, 0, 0, 0 );
    (*this_).resources = resources;
    (*this_).texture_downloader = texture_downloader;
#if 0
    gui_sketch_background_private_write_as_ints( this_ );
#endif

    U8_TRACE_END();
}

void gui_sketch_background_destroy( gui_sketch_background_t *this_ )
{
    U8_TRACE_BEGIN();

    shape_int_rectangle_destroy( &((*this_).card_bounds) );
    shape_int_rectangle_destroy( &((*this_).label_bounds) );
    (*this_).resources = NULL;
    (*this_).texture_downloader = NULL;

    U8_TRACE_END();
}

static const double BLACK_R = 0.0;
static const double BLACK_G = 0.0;
static const double BLACK_B = 0.0;
static const double BLACK_A = 1.0;
static const double DARK_R = 0.375;
static const double DARK_G = 0.375;
static const double DARK_B = 0.375;
static const double DARK_A = 1.0;
static const double GREY_R = 0.75;
static const double GREY_G = 0.75;
static const double GREY_B = 0.75;
static const double GREY_A = 1.0;
static const double ORANGE_R = 1.0;
static const double ORANGE_G = 0.75;
static const double ORANGE_B = 0.5;
static const double ORANGE_A = 1.0;
static const double LIGHT_R = 0.875;
static const double LIGHT_G = 0.875;
static const double LIGHT_B = 0.875;
static const double LIGHT_A = 1.0;
static const double BORDER = 8;  /* border between text/icons and ground rectangle */

void gui_sketch_background_draw_introduction( gui_sketch_background_t *this_,
                                              cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const int32_t left = shape_int_rectangle_get_left( &((*this_).card_bounds) );
    const int32_t top = shape_int_rectangle_get_top( &((*this_).card_bounds) );
    const uint32_t width = shape_int_rectangle_get_width( &((*this_).card_bounds) );
    const uint32_t height = shape_int_rectangle_get_height( &((*this_).card_bounds) );

    int32_t text_area_start;

    /* if there is enough space, draw a nice picture bar on the left side */
    if ( width > 192 )
    {
        cairo_surface_t *bg_img = gui_resources_get_sketch_background( (*this_).resources );
        int icon_width = cairo_image_surface_get_width( bg_img );
        int icon_height = cairo_image_surface_get_height( bg_img );
        const unsigned int tiles = (height+(icon_height-1))/icon_height;
        for ( unsigned int tile = 0; tile < tiles; tile ++ )
        {
            cairo_set_source_surface( cr, bg_img, left, top + ( tile * icon_height ) );
            cairo_rectangle ( cr, left, top + ( tile * icon_height ), icon_width, icon_height );
            cairo_fill (cr);
        }
        text_area_start = left+icon_width;
    }
    else
    {
        text_area_start = left;
    }

    cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
    cairo_rectangle ( cr, text_area_start, top, width-text_area_start, height );
    cairo_fill (cr);

    const int TAB_ROW0_Y = 48;
    const int TAB_ROW1_Y = 96;
    const int TAB_ROW2_Y = 192;
    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_crystal_facet_uml( (*this_).resources ),
                                                         "Welcome to",
                                                         META_INFO_PROGRAM_NAME_STR,
                                                         text_area_start+BORDER,
                                                         top+BORDER+TAB_ROW0_Y,
                                                         cr
                                                       );

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_file_new( (*this_).resources ),
                                                         "To begin, please",
                                                         "create a new database file first.",
                                                         text_area_start+BORDER,
                                                         top+BORDER+TAB_ROW1_Y,
                                                         cr
                                                       );

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_message_user_doc( (*this_).resources ),
                                                         "The user manual crystal-facet-uml_documentation.pdf is available",
#ifdef __linux__
                                                         "in the net and locally at /usr/share/doc/(packages/)crystal-facet-uml",
#else
                                                         "in the net",
#endif
                                                         text_area_start+BORDER,
                                                         top+BORDER+TAB_ROW2_Y,
                                                         cr
                                                       );

    U8_TRACE_END();
}

void gui_sketch_background_draw_navigation( gui_sketch_background_t *this_,
                                            unsigned int tree_depth,
                                            unsigned int num_children,
                                            cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    /* draw background of nav_tree list labels */
    const int32_t label_left = shape_int_rectangle_get_left( &((*this_).label_bounds) );
    const int32_t label_top = shape_int_rectangle_get_top( &((*this_).label_bounds) );
    const uint32_t label_width = shape_int_rectangle_get_width( &((*this_).label_bounds) );
    const uint32_t label_height = shape_int_rectangle_get_height( &((*this_).label_bounds) );
    cairo_set_source_rgba( cr, LIGHT_R, LIGHT_G, LIGHT_B, LIGHT_A );
    cairo_rectangle ( cr, label_left, label_top, label_width, label_height );
    cairo_fill (cr);

    /* draw background of cards area */
    const int32_t left = shape_int_rectangle_get_left( &((*this_).card_bounds) );
    const int32_t top = shape_int_rectangle_get_top( &((*this_).card_bounds) );
    const uint32_t width = shape_int_rectangle_get_width( &((*this_).card_bounds) );
    const uint32_t height = shape_int_rectangle_get_height( &((*this_).card_bounds) );

    if ( 0 == tree_depth )
    {
        cairo_set_source_rgba( cr, LIGHT_R, LIGHT_G, LIGHT_B, LIGHT_A );
    }
    else
    {
        cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
    }
    cairo_rectangle ( cr, left, top, width, height );
    cairo_fill (cr);

    if ( ( 0 == tree_depth )&&( 0 == num_children ))
    {
        /* this is a new, empty database */

        gui_sketch_background_private_draw_quick_introduction( this_, cr );
    }

    U8_TRACE_END();
}

void gui_sketch_background_draw_search( gui_sketch_background_t *this_, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    /* draw background of search result list list labels */
    const int32_t label_left = shape_int_rectangle_get_left( &((*this_).label_bounds) );
    const int32_t label_top = shape_int_rectangle_get_top( &((*this_).label_bounds) );
    const uint32_t label_width = shape_int_rectangle_get_width( &((*this_).label_bounds) );
    const uint32_t label_height = shape_int_rectangle_get_height( &((*this_).label_bounds) );
    cairo_set_source_rgba( cr, LIGHT_R, LIGHT_G, LIGHT_B, LIGHT_A );
    cairo_rectangle ( cr, label_left, label_top, label_width, label_height );
    cairo_fill (cr);

    /* draw background of cards area */
    const int32_t left = shape_int_rectangle_get_left( &((*this_).card_bounds) );
    const int32_t top = shape_int_rectangle_get_top( &((*this_).card_bounds) );
    const uint32_t width = shape_int_rectangle_get_width( &((*this_).card_bounds) );
    const uint32_t height = shape_int_rectangle_get_height( &((*this_).card_bounds) );

    cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
    cairo_rectangle ( cr, left, top, width, height );
    cairo_fill (cr);

    U8_TRACE_END();
}

void gui_sketch_background_draw_edit( gui_sketch_background_t *this_, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const int32_t left = shape_int_rectangle_get_left( &((*this_).card_bounds) );
    const int32_t top = shape_int_rectangle_get_top( &((*this_).card_bounds) );
    const uint32_t width = shape_int_rectangle_get_width( &((*this_).card_bounds) );
    const uint32_t height = shape_int_rectangle_get_height( &((*this_).card_bounds) );

    cairo_set_source_rgba( cr, DARK_R, DARK_G, DARK_B, DARK_A );
    cairo_rectangle ( cr, left, top, width, height );
    cairo_fill (cr);

    U8_TRACE_END();
}

void gui_sketch_background_draw_create( gui_sketch_background_t *this_, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const int32_t left = shape_int_rectangle_get_left( &((*this_).card_bounds) );
    const int32_t top = shape_int_rectangle_get_top( &((*this_).card_bounds) );
    const uint32_t width = shape_int_rectangle_get_width( &((*this_).card_bounds) );
    const uint32_t height = shape_int_rectangle_get_height( &((*this_).card_bounds) );

    cairo_set_source_rgba( cr, ORANGE_R, ORANGE_G, ORANGE_B, ORANGE_A );
    cairo_rectangle ( cr, left, top, width, height );
    cairo_fill (cr);

    U8_TRACE_END();
}

void gui_sketch_background_draw_appears_link( gui_sketch_background_t *this_,
                                              const shape_int_rectangle_t *label_box,
                                              const shape_int_rectangle_t *card_box,
                                              cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != label_box );
    assert( NULL != card_box );
    assert( NULL != cr );

    const int32_t label_top = shape_int_rectangle_get_top( label_box );
    const uint32_t label_right = shape_int_rectangle_get_right( label_box );
    const uint32_t label_bottom = shape_int_rectangle_get_bottom( label_box );
    const int32_t card_left = shape_int_rectangle_get_left( card_box );
    const int32_t card_top = shape_int_rectangle_get_top( card_box );
    const uint32_t card_right = shape_int_rectangle_get_right( card_box );
    const uint32_t card_bottom = shape_int_rectangle_get_bottom( card_box );

    cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
    cairo_move_to( cr, label_right, label_top );
    if ( label_top > card_bottom )
    {
        const int_fast32_t big_delta_x = card_right - label_right;
        const int_fast32_t small_delta_x = card_left - label_right;
        const int_fast32_t big_delta_y = label_bottom - card_bottom;
        const int_fast32_t small_delta_y = label_top - card_bottom;
        const int_fast32_t big_mid_x = label_right + ( big_delta_x * 3 ) / 4;
        const int_fast32_t small_mid_x = label_right + ( small_delta_x * 3 ) / 4;
        const int_fast32_t big_mid_y = card_bottom + ( big_delta_y * 3 ) / 4;
        const int_fast32_t small_mid_y = card_bottom + ( small_delta_y * 3 ) / 4;
        cairo_curve_to( cr, small_mid_x, label_top, card_left, small_mid_y, card_left, card_bottom );
        cairo_line_to( cr, card_right, card_bottom );
        cairo_curve_to( cr, card_right, big_mid_y, big_mid_x, label_bottom, label_right, label_bottom );
    }
    else if ( label_bottom < card_top )
    {
        const int_fast32_t big_delta_x = card_right - label_right;
        const int_fast32_t small_delta_x = card_left - label_right;
        const int_fast32_t big_delta_y = card_top - label_top;
        const int_fast32_t small_delta_y = card_top - label_bottom;
        const int_fast32_t big_mid_x = label_right + ( big_delta_x * 3 ) / 4;
        const int_fast32_t small_mid_x = label_right + ( small_delta_x * 3 ) / 4;
        const int_fast32_t big_mid_y = card_top - ( big_delta_y * 3 ) / 4;
        const int_fast32_t small_mid_y = card_top - ( small_delta_y * 3 ) / 4;
        cairo_curve_to( cr, big_mid_x, label_top, card_right, big_mid_y, card_right, card_top );
        cairo_line_to( cr, card_left, card_top );
        cairo_curve_to( cr, card_left, small_mid_y, small_mid_x, label_bottom, label_right, label_bottom );
    }
    else
    {
        const int32_t mid_x = ( label_right + card_left ) / 2;
        cairo_curve_to( cr, mid_x, label_top, mid_x, card_top, card_left, card_top );
        cairo_line_to( cr, card_left, card_bottom );
        cairo_curve_to( cr, mid_x, card_bottom, mid_x, label_bottom, label_right, label_bottom );
    }
    cairo_fill( cr );

    U8_TRACE_END();
}

void gui_sketch_background_private_draw_quick_introduction( gui_sketch_background_t *this_, cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );

    const int32_t left = shape_int_rectangle_get_left( &((*this_).card_bounds) );
    //const int32_t top = shape_int_rectangle_get_top( &((*this_).card_bounds) );
    //const uint32_t width = shape_int_rectangle_get_width( &((*this_).card_bounds) );
    const uint32_t height = shape_int_rectangle_get_height( &((*this_).card_bounds) );

    const int32_t TAB_HEIGHT = 144 + BORDER;
    const int32_t TAB_WIDTH = 640 + BORDER;
    const int32_t TAB_X = left + 16;
    const int32_t TAB_Y = height - TAB_HEIGHT - 16;
    const int32_t TAB_COL0_X = TAB_X + BORDER + 14;
    const int32_t TAB_COL1_X = TAB_X + BORDER + 112;
    const int32_t TAB_COL2_X = TAB_X + BORDER + 340;
    const int32_t TAB_ROW0_Y = TAB_Y + BORDER + 0;
    const int32_t TAB_ROW1_Y = TAB_Y + BORDER + 48;
    const int32_t TAB_ROW2_Y = TAB_Y + BORDER + 96;

    cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
    cairo_rectangle ( cr, TAB_X, TAB_Y, TAB_WIDTH, TAB_HEIGHT );
    cairo_fill (cr);

    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_message_user_doc( (*this_).resources ),
                                                         "Quick",
                                                         "Intro:",
                                                         TAB_COL0_X,
                                                         TAB_ROW0_Y,
                                                         cr
                                                       );
    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_view_navigate( (*this_).resources ),
                                                         "Click on a diagram to navigate,",
                                                         "on '+' to create a new diagram.",
                                                         TAB_COL1_X,
                                                         TAB_ROW0_Y,
                                                         cr
                                                       );
    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_view_edit( (*this_).resources ),
                                                         "Click on an element to edit",
                                                         "name, type and description.",
                                                         TAB_COL1_X,
                                                         TAB_ROW1_Y,
                                                         cr
                                                       );
    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_view_edit( (*this_).resources ),
                                                         "Drag an element to change",
                                                         "its position.",
                                                         TAB_COL2_X,
                                                         TAB_ROW1_Y,
                                                         cr
                                                       );
    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_view_create( (*this_).resources ),
                                                         "Click to create items.",
                                                         "Drag to create arrows.",
                                                         TAB_COL1_X,
                                                         TAB_ROW2_Y,
                                                         cr
                                                       );
    gui_sketch_background_private_draw_icon_and_message( this_,
                                                         gui_resources_get_file_export( (*this_).resources ),
                                                         "Select the output folder",
                                                         "to export all diagrams.",
                                                         TAB_COL2_X,
                                                         TAB_ROW2_Y,
                                                         cr
                                                       );

    U8_TRACE_END();
}

void gui_sketch_background_private_draw_icon_and_message( gui_sketch_background_t *this_,
                                                          GdkTexture *icon_1,
                                                          const char *text_1,
                                                          const char *text_2,
                                                          int x,
                                                          int y,
                                                          cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != icon_1 );
    assert( NULL != text_1 );
    assert( NULL != text_2 );

    const double icon_width = gdk_texture_get_width ( icon_1 );
    gui_sketch_texture_draw( (*this_).texture_downloader, icon_1, x, y, cr );

    cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
    cairo_set_font_size ( cr, 12.0 );
    cairo_move_to ( cr, x+icon_width+BORDER, y + 14 );
    cairo_show_text ( cr, text_1 );
    cairo_move_to ( cr, x+icon_width+BORDER, y + 2*14 );
    cairo_show_text ( cr, text_2 );

    U8_TRACE_END();
}


/*
Copyright 2017-2026 Andreas Warnke

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
