/* File: gui_sketch_result_list.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_result_list.h"
#include "geometry/geometry_rectangle.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "gui_sketch_int_compare.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <gdk/gdk.h>

static const int GUI_SKETCH_RESULT_LIST_PANGO_AUTO_DETECT_LENGTH = -1;  /*!< pango automatically determines the string length */
static const int GUI_SKETCH_RESULT_LIST_PANGO_UNLIMITED_WIDTH = -1;
static const int OBJ_GAP = 4;

void gui_sketch_result_list_init( gui_sketch_result_list_t *this_, gui_resources_t *resources )
{
    TRACE_BEGIN();
    assert( resources != NULL );

    DATA_SEARCH_RESULT_LIST_INIT( &((*this_).result_list), (*this_).result_list_buf );

    (*this_).visible = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    (*this_).element_count = 0;

    gui_sketch_style_init( &((*this_).sketch_style) );
    gui_sketch_marker_init( &((*this_).sketch_marker), true );
    (*this_).resources = resources;
    gui_resource_selector_init( &((*this_).selector), resources );

    TRACE_END();
}

void gui_sketch_result_list_destroy( gui_sketch_result_list_t *this_ )
{
    TRACE_BEGIN();

    gui_resource_selector_destroy( &((*this_).selector) );
    (*this_).resources = NULL;
    gui_sketch_marker_destroy( &((*this_).sketch_marker) );
    gui_sketch_style_destroy( &((*this_).sketch_style) );

    gui_sketch_result_list_invalidate_data( this_ );

    shape_int_rectangle_destroy( &((*this_).bounds) );

    TRACE_END();
}

void gui_sketch_result_list_do_layout( gui_sketch_result_list_t *this_, cairo_t *cr )
{
    TRACE_BEGIN();

    /* create the font_layout */
    PangoLayout *font_layout;
    {
        font_layout = pango_cairo_create_layout (cr);
        const PangoFontDescription *const std_font
            = gui_sketch_style_get_standard_font_description( &((*this_).sketch_style ) );
        pango_layout_set_font_description( font_layout, std_font );
    }

    int32_t y_pos = shape_int_rectangle_get_top( &((*this_).bounds) );

    const unsigned int count = data_search_result_list_get_length( &((*this_).result_list) );
    assert( count <= GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS );
    (*this_).element_count = count;
    for ( unsigned int idx = 0; idx < count; idx ++ )
    {
        const data_search_result_t *result = data_search_result_list_get_const( &((*this_).result_list), idx );
        pos_search_result_init( &((*this_).element_pos[idx]), result );
        gui_sketch_result_list_private_layout_element( this_, &((*this_).element_pos[idx]), &y_pos, font_layout );
    }

    /* release the font_layout */
    g_object_unref(font_layout);

    TRACE_END();
}

void gui_sketch_result_list_private_layout_element ( gui_sketch_result_list_t *this_,
                                                     pos_search_result_t *element,
                                                     int32_t *io_y_pos,
                                                     PangoLayout *font_layout )
{
    TRACE_BEGIN();
    assert( NULL != element );
    assert( NULL != io_y_pos );
    assert( NULL != font_layout );

    int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
    uint_fast32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
    const data_search_result_t *result = pos_search_result_get_data_const( element );

    /* determine icon dimensions */
    {
        const data_table_t result_table = data_search_result_get_match_table( result );
        const int result_type = data_search_result_get_match_type( result );
        const GdkPixbuf *icon = gui_resource_selector_get_icon ( &((*this_).selector), result_table, result_type );
        const double icon_width = gdk_pixbuf_get_width( icon );
        const double icon_height = gdk_pixbuf_get_height( icon );

        const shape_int_rectangle_t new_icon_box = (shape_int_rectangle_t) {
            .left=left+OBJ_GAP,
            .top=(*io_y_pos)+OBJ_GAP,
            .width=icon_width+0.999,
            .height=icon_height+0.999 };
        pos_search_result_set_icon_box( element, &new_icon_box );
    }

    /* determine label dimensions */
    const shape_int_rectangle_t *const icon_box = pos_search_result_get_icon_box_const( element );
    {
        int_fast32_t proposed_pango_width = width - shape_int_rectangle_get_width(icon_box) - (4*OBJ_GAP);
        pango_layout_set_text( font_layout,
                                data_search_result_get_match_name_const( result ),
                                GUI_SKETCH_RESULT_LIST_PANGO_AUTO_DETECT_LENGTH
                                );
        pango_layout_set_width(font_layout, proposed_pango_width * PANGO_SCALE );
        int text_width;
        int text_height;
        pango_layout_get_pixel_size(font_layout, &text_width, &text_height);

        int_fast32_t x_pos = shape_int_rectangle_get_right(icon_box);

        const shape_int_rectangle_t new_label_box = (shape_int_rectangle_t) {
            .left=x_pos+OBJ_GAP,
            .top=(*io_y_pos)+OBJ_GAP,
            .width=text_width,
            .height=text_height };
        pos_search_result_set_label_box( element, &new_label_box );
    }

    const shape_int_rectangle_t *const label_box = pos_search_result_get_label_box_const( element );
    *io_y_pos
        = gui_sketch_int_compare_max_i32( shape_int_rectangle_get_bottom(icon_box), shape_int_rectangle_get_bottom(label_box) )
        + OBJ_GAP;

    TRACE_END();
}

static const double GREY_R = 0.8;
static const double GREY_G = 0.8;
static const double GREY_B = 0.8;
static const double GREY_A = 1.0;

void gui_sketch_result_list_draw ( gui_sketch_result_list_t *this_, const gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != marker );
    assert( NULL != cr );

    if ( (*this_).visible )
    {
        PangoLayout *font_layout = pango_cairo_create_layout (cr);
        {
            const PangoFontDescription *const std_font
                = gui_sketch_style_get_standard_font_description( &((*this_).sketch_style ) );
            pango_layout_set_font_description ( font_layout, std_font );
        }

        /* draw background */
        {
            const int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
            const int_fast32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
            const uint_fast32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
            const uint_fast32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

            cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
            cairo_rectangle ( cr, left, top, width, height );
            cairo_fill (cr);
        }

        /* draw icons and text */
        const unsigned int count = (*this_).element_count;
        assert( count <= GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS );
        if ( count == 0 )
        {
            const int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
            const int_fast32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
            const GdkPixbuf *undef_icon = gui_resources_get_type_undef( (*this_).resources );
            double icon_width = gdk_pixbuf_get_width ( undef_icon );
            double icon_height = gdk_pixbuf_get_height ( undef_icon );

            /* draw text first, use the above set color and font */
            const GdkRGBA std_color = gui_sketch_style_get_standard_color( &((*this_).sketch_style) );
            cairo_set_source_rgba( cr, std_color.red, std_color.green, std_color.blue, std_color.alpha );
            cairo_move_to( cr, left+OBJ_GAP+icon_width, top+OBJ_GAP );
            pango_layout_set_text( font_layout, "no results", GUI_SKETCH_RESULT_LIST_PANGO_AUTO_DETECT_LENGTH );
            pango_layout_set_width(font_layout, GUI_SKETCH_RESULT_LIST_PANGO_UNLIMITED_WIDTH );
            pango_cairo_show_layout( cr, font_layout );

            /* draw the icon */
            const int x = left+OBJ_GAP;
            const int y = top+OBJ_GAP;
            gdk_cairo_set_source_pixbuf( cr, undef_icon, x, y );
            cairo_rectangle ( cr, x, y, x+icon_width, y+icon_height );
            cairo_fill (cr);
        }
        else
        {
            for ( unsigned int idx = 0; idx < count; idx ++ )
            {
                const pos_search_result_t *const element = &((*this_).element_pos[idx]);
                gui_sketch_result_list_private_draw_element( this_, element, marker, font_layout, cr );
            }
        }

        g_object_unref(font_layout);
    }

    TRACE_END();
}

void gui_sketch_result_list_private_draw_element( gui_sketch_result_list_t *this_,
                                                  const pos_search_result_t *element,
                                                  const gui_marked_set_t *marker,
                                                  PangoLayout *font_layout,
                                                  cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != element );
    assert( NULL != marker );
    assert( NULL != font_layout );

    const data_search_result_t *const result = pos_search_result_get_data_const( element );

    /* draw marker and set color */
    {
        shape_int_rectangle_t destination_rect;
        shape_int_rectangle_init_by_bounds( &destination_rect,
                                            pos_search_result_get_icon_box_const(element),
                                            pos_search_result_get_label_box_const(element)
                                          );

        const data_id_t highlighted = gui_marked_set_get_highlighted( marker );
        gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                        data_search_result_get_match_id( result ),
                                        marker,
                                        destination_rect,
                                        cr
                                      );
        if ( data_id_equals( &highlighted, data_search_result_get_diagram_id_const( result ) ) )
        {
            const GdkRGBA high_color = gui_sketch_style_get_highlight_color( &((*this_).sketch_style) );
            cairo_set_source_rgba( cr, high_color.red, high_color.green, high_color.blue, high_color.alpha );
        }

        shape_int_rectangle_destroy( &destination_rect );
    }

    /* draw text first, use the above set color and font */
    {
        /* what to draw */
        const char *const label = data_search_result_get_match_name_const( result );

        /* where to draw to */
        const shape_int_rectangle_t *const label_box
            = pos_search_result_get_label_box_const( element );

        /* do draw */
        cairo_move_to( cr, shape_int_rectangle_get_left(label_box), shape_int_rectangle_get_top(label_box) );
        pango_layout_set_text( font_layout, label, GUI_SKETCH_RESULT_LIST_PANGO_AUTO_DETECT_LENGTH );
        const unsigned int text_width
            = shape_int_rectangle_get_width(label_box)
            +(2.0*OBJ_GAP);  /* add gap to avoid line breaks by rounding errors and whitespace character widths */
        pango_layout_set_width(font_layout, text_width * PANGO_SCALE );
        pango_cairo_show_layout( cr, font_layout );
    }

    /* draw the icon */
    {
        /* what to draw */
        const data_table_t result_table = data_search_result_get_match_table( result );
        const int result_type = data_search_result_get_match_type( result );
        const GdkPixbuf *icon = gui_resource_selector_get_icon ( &((*this_).selector), result_table, result_type );

        /* where to draw to */
        const shape_int_rectangle_t *const icon_box
            = pos_search_result_get_icon_box_const( element );
        const int x = shape_int_rectangle_get_left(icon_box);
        const int y = shape_int_rectangle_get_top(icon_box);
        double icon_width = gdk_pixbuf_get_width ( icon );
        double icon_height = gdk_pixbuf_get_height ( icon );

        /* do draw */
        gdk_cairo_set_source_pixbuf( cr, icon, x, y );
        cairo_rectangle ( cr, x, y, x+icon_width, y+icon_height );
        cairo_fill (cr);
    }

    TRACE_END();
}


/*
Copyright 2018-2022 Andreas Warnke

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

