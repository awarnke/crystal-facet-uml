/* File: gui_sketch_result_list.c; Copyright and License: see below */

#include "sketch/gui_sketch_result_list.h"
#include "geometry/geometry_rectangle.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "u8/u8_i32.h"
#include "gui_gdk.h"

static const int GUI_SKETCH_RESULT_LIST_PANGO_AUTO_DETECT_LENGTH = -1;  /*!< pango automatically determines the string length */
static const int GUI_SKETCH_RESULT_LIST_PANGO_UNLIMITED_WIDTH = -1;
static const int OBJ_GAP = 4;

void gui_sketch_result_list_init( gui_sketch_result_list_t *this_,
                                  const gui_resources_t *resources,
                                  gui_sketch_texture_t *texture_downloader )
{
    U8_TRACE_BEGIN();
    assert( resources != NULL );
    assert( texture_downloader != NULL );

    /* layouting information */
    (*this_).visible = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    /* data and layouting information of search results */
    (*this_).requested_page = pos_scroll_page_new( 0, false /* backwards */ );
    pos_search_result_page_init( &((*this_).page), 0 /* buffer_start */ );

    /* helper classes to perform drawing */
    gui_sketch_style_init( &((*this_).sketch_style) );
    gui_sketch_marker_init( &((*this_).sketch_marker), true );
    (*this_).resources = resources;
    gui_type_resource_list_init( &((*this_).selector), resources );
    (*this_).texture_downloader = texture_downloader;

    U8_TRACE_END();
}

void gui_sketch_result_list_destroy( gui_sketch_result_list_t *this_ )
{
    U8_TRACE_BEGIN();

    /* helper classes to perform drawing */
    (*this_).texture_downloader = NULL;
    gui_type_resource_list_destroy( &((*this_).selector) );
    (*this_).resources = NULL;
    gui_sketch_marker_destroy( &((*this_).sketch_marker) );
    gui_sketch_style_destroy( &((*this_).sketch_style) );

    /* data and layouting information of search results */
    pos_search_result_page_destroy( &((*this_).page) );

    /* layouting information */
    shape_int_rectangle_destroy( &((*this_).bounds) );

    U8_TRACE_END();
}

void gui_sketch_result_list_do_layout( gui_sketch_result_list_t *this_, cairo_t *cr )
{
    U8_TRACE_BEGIN();

    /* bounds */
    int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
    uint_fast32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
    int_fast32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
    uint_fast32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

    /* create the font_layout */
    PangoLayout *font_layout;
    {
        font_layout = pango_cairo_create_layout (cr);
        const PangoFontDescription *const std_font
            = gui_sketch_style_get_standard_font_description( &((*this_).sketch_style ) );
        pango_layout_set_font_description( font_layout, std_font );
    }

    /* measure a previous page button heights */
    GdkTexture *prev_gray_icon = gui_resources_get_sketch_page_up_gray( (*this_).resources );
    const uint_fast32_t prev_icon_height = gdk_texture_get_height( prev_gray_icon );
    const uint_fast32_t prev_icon_width = gdk_texture_get_width( prev_gray_icon );
    const shape_int_rectangle_t prev_icon_box = (shape_int_rectangle_t) {
        .left = left + ( width - prev_icon_width ) / 2,
        .top = top,
        .width = prev_icon_width,
        .height = prev_icon_height };
    pos_search_result_page_set_button_prev_box( &((*this_).page), &prev_icon_box );

    /* measure a next page button heights */
    GdkTexture *next_gray_icon = gui_resources_get_sketch_page_down_gray( (*this_).resources );
    const uint_fast32_t next_icon_height = gdk_texture_get_height( next_gray_icon );
    const uint_fast32_t next_icon_width = gdk_texture_get_width( next_gray_icon );
    const shape_int_rectangle_t next_icon_box = (shape_int_rectangle_t) {
        .left = left + ( width - next_icon_width ) / 2,
        .top = top + height - next_icon_height,
        .width = next_icon_width,
        .height = next_icon_height };
    pos_search_result_page_set_button_next_box( &((*this_).page), &next_icon_box );

    /* initialize a possible result - in case the buffer is empty and the for loop later is not executed */
    const int32_t buffer_start = pos_search_result_page_get_buffer_start( &((*this_).page) );
    const uint32_t buffer_length = pos_search_result_page_get_buffer_length( &((*this_).page) );
    assert( buffer_length <= POS_SEARCH_RESULT_PAGE_MAX_PAGE_SIZE );
    const bool buffer_more_after = pos_search_result_page_get_buffer_more_after( &((*this_).page) );
    pos_search_result_page_set_page_start( &((*this_).page), buffer_start );  /* default value for case of empty buffer */
    pos_search_result_page_set_page_length( &((*this_).page), buffer_length );  /* default value for case of empty buffer */
    pos_search_result_page_set_has_prev_page( &((*this_).page), ( buffer_start != 0 ) );  /* default value for case of empty buffer */

    /* do not fully trust the consistency between (*this_).requested_page and (*this_).page. */
    /* These information come from different sources. */
    pos_scroll_page_trace( &((*this_).requested_page) );
    const int_fast32_t requested_anchor_idx = pos_scroll_page_get_anchor_index( &((*this_).requested_page) );
    const bool valid_anchor
        = (( buffer_start <= requested_anchor_idx )&&( requested_anchor_idx < ( buffer_start + buffer_length )));
    assert( valid_anchor || ( buffer_length == 0 ) );
    const int_fast32_t anchor_idx = valid_anchor ? requested_anchor_idx : buffer_start;
    const bool requested_backwards = pos_scroll_page_get_backwards( &((*this_).requested_page) );
    const bool backwards = valid_anchor ? requested_backwards : false;

    /* each visible element, do layout */
    if ( backwards )
    {
        /* y positions */
        int_fast32_t y_pos = shape_int_rectangle_get_bottom( &((*this_).bounds) ) - next_icon_height - OBJ_GAP;
        const int_fast32_t top_border = shape_int_rectangle_get_top( &((*this_).bounds) ) + prev_icon_height + OBJ_GAP;
        bool page_full = false;
        for ( int32_t index = anchor_idx; ( index >= buffer_start ) && ( ! page_full ); index -- )
        {
            pos_search_result_t *const search_result
                = pos_search_result_page_get_search_result_layout_ptr( &((*this_).page), index );
            gui_sketch_result_list_private_layout_element( this_, search_result, &y_pos, backwards, font_layout );
            if ( y_pos < top_border )
            {
                page_full = true;
            }
            else
            {
                /* define the range of visible elements */
                pos_search_result_page_set_page_start( &((*this_).page), index );
                pos_search_result_page_set_page_length( &((*this_).page), anchor_idx - index + 1 );
                pos_search_result_page_set_has_prev_page( &((*this_).page), ( index != 0 ) );
            }
        }
        const bool more_after_anchor = ( anchor_idx + 1 ) < ( buffer_start + buffer_length );
        pos_search_result_page_set_has_next_page( &((*this_).page), buffer_more_after || more_after_anchor );
    }
    else
    {
        /* y positions */
        int_fast32_t y_pos = shape_int_rectangle_get_top( &((*this_).bounds) ) + prev_icon_height + OBJ_GAP;
        const int_fast32_t bottom_border = shape_int_rectangle_get_bottom( &((*this_).bounds) ) - next_icon_height - OBJ_GAP;
        bool page_full = false;
        for ( uint32_t buffer_idx = 0; ( buffer_idx < buffer_length ) && ( ! page_full ); buffer_idx ++ )
        {
            const uint32_t index = buffer_start + buffer_idx;
            pos_search_result_t *const search_result
                = pos_search_result_page_get_search_result_layout_ptr( &((*this_).page), index );
            gui_sketch_result_list_private_layout_element( this_, search_result, &y_pos, backwards, font_layout );
            if ( y_pos > bottom_border )
            {
                page_full = true;
            }
            else
            {
                /* define the range of visible elements */
                pos_search_result_page_set_page_start( &((*this_).page), buffer_start );
                pos_search_result_page_set_page_length( &((*this_).page), index - buffer_start + 1 );
            }
        }
        pos_search_result_page_set_has_next_page( &((*this_).page), buffer_more_after || page_full );
    }

    /* trace available buffer and visible page */
    U8_TRACE_INFO_INT_INT( "gui_sketch_result_list_do_layout: available buffer (start,len):", buffer_start, buffer_length );
    U8_TRACE_INFO_INT_INT( "gui_sketch_result_list_do_layout: visible page     (start,len):",
                           pos_search_result_page_get_page_start( &((*this_).page) ),
                           pos_search_result_page_get_page_length( &((*this_).page) )
                         );

    /* release the font_layout */
    g_object_unref(font_layout);

    U8_TRACE_END();
}

void gui_sketch_result_list_private_layout_element ( gui_sketch_result_list_t *this_,
                                                     pos_search_result_t *element,
                                                     int_fast32_t *io_y_pos,
                                                     bool upwards,
                                                     PangoLayout *font_layout )
{
    U8_TRACE_BEGIN();
    assert( NULL != element );
    assert( NULL != io_y_pos );
    assert( NULL != font_layout );

    int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
    uint_fast32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
    const data_search_result_t *result = pos_search_result_get_data_const( element );

    /* determine icon dimensions */
    const data_type_t result_type = data_search_result_get_match_type( result );
    gui_type_resource_t *const type_data
        = gui_type_resource_list_get_type ( &((*this_).selector), result_type );
    GdkTexture *const icon = gui_type_resource_get_icon( type_data );
    const uint_fast32_t icon_width = gdk_texture_get_width( icon );
    const uint_fast32_t icon_height = gdk_texture_get_height( icon );

    /* determine label dimensions */
    int_fast32_t proposed_pango_width = width - icon_width - (4*OBJ_GAP);
    pango_layout_set_text( font_layout,
                           data_search_result_get_match_name_const( result ),
                           GUI_SKETCH_RESULT_LIST_PANGO_AUTO_DETECT_LENGTH
                         );
    pango_layout_set_width(font_layout, proposed_pango_width * PANGO_SCALE );
    int text_width;
    int text_height;
    pango_layout_get_pixel_size(font_layout, &text_width, &text_height);

    /* calculate element bounds */
    const uint_fast32_t element_height = u8_i32_max2( icon_height, text_height );
    const int_fast32_t element_top = (*io_y_pos) - ( upwards ? ( OBJ_GAP + element_height ) : (-OBJ_GAP) );

    /* set icon dimensions */
    const shape_int_rectangle_t icon_box = (shape_int_rectangle_t) {
        .left = left + OBJ_GAP,
        .top = element_top,
        .width = icon_width,
        .height = icon_height };
    pos_search_result_set_icon_box( element, &icon_box );

    /* set label dimensions */
    int_fast32_t x_pos = shape_int_rectangle_get_right( &icon_box );
    const shape_int_rectangle_t label_box = (shape_int_rectangle_t) {
        .left = x_pos + OBJ_GAP,
        .top = element_top,
        .width = text_width,
        .height = text_height };
    pos_search_result_set_label_box( element, &label_box );

    /* update y_pos */
    *io_y_pos = ( upwards ? element_top : ( element_top + element_height ) );

    U8_TRACE_END();
}

void gui_sketch_result_list_draw ( gui_sketch_result_list_t *this_, const gui_marked_set_t *marker, cairo_t *cr )
{
    U8_TRACE_BEGIN();
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

        /* draw icons and text */
        const uint32_t page_start = pos_search_result_page_get_page_start( &((*this_).page) );
        const uint32_t page_length = pos_search_result_page_get_page_length( &((*this_).page) );
        assert( page_length <= POS_SEARCH_RESULT_PAGE_MAX_PAGE_SIZE );
        if ( ( page_start + page_length ) == 0 )
        {
            const int_fast32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
            const int_fast32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
            GdkTexture *undef_icon = gui_resources_get_type_undef( (*this_).resources );
            const uint_fast32_t icon_width = gdk_texture_get_width ( undef_icon );

            /* draw text first, use the above set color and font */
            const GdkRGBA std_color = gui_sketch_style_get_standard_color( &((*this_).sketch_style) );
            cairo_set_source_rgba( cr, std_color.red, std_color.green, std_color.blue, std_color.alpha );
            cairo_move_to( cr, left+OBJ_GAP+icon_width, top+OBJ_GAP );
            pango_layout_set_text( font_layout, "no results", GUI_SKETCH_RESULT_LIST_PANGO_AUTO_DETECT_LENGTH );
            pango_layout_set_width(font_layout, GUI_SKETCH_RESULT_LIST_PANGO_UNLIMITED_WIDTH );
            pango_cairo_show_layout( cr, font_layout );

            /* draw the icon */
            const int_fast32_t x = left + OBJ_GAP;
            const int_fast32_t y = top + OBJ_GAP;
            gui_sketch_texture_draw( (*this_).texture_downloader, undef_icon, x, y, cr );
        }
        else
        {
            /* draw search results */
            for ( uint32_t idx = 0; idx < page_length; idx ++ )
            {
                const pos_search_result_t *const element
                    = pos_search_result_page_get_search_result_layout_const( &((*this_).page), page_start + idx );
                gui_sketch_result_list_private_draw_element( this_, element, marker, font_layout, cr );
            }

            /* draw prev and next page buttons */
            const gui_sketch_action_t btn_act = gui_marked_set_get_highlighted_button( marker );
            if ( pos_search_result_page_has_prev_page( &((*this_).page) ) )
            {
                const shape_int_rectangle_t *const prev_box
                    = pos_search_result_page_get_button_prev_box_const( &((*this_).page) );
                GdkTexture *const prev_icon
                    = ( btn_act == GUI_SKETCH_ACTION_PREVIOUS_PAGE )
                    ? gui_resources_get_sketch_page_up_bold( (*this_).resources )
                    : gui_resources_get_sketch_page_up_gray( (*this_).resources );
                gui_sketch_texture_draw( (*this_).texture_downloader,
                                         prev_icon,
                                         shape_int_rectangle_get_left( prev_box ),
                                         shape_int_rectangle_get_top( prev_box ),
                                         cr
                                       );
            }
            if ( pos_search_result_page_has_next_page( &((*this_).page) ) )
            {
                const shape_int_rectangle_t *const next_box
                    = pos_search_result_page_get_button_next_box_const( &((*this_).page) );
                GdkTexture *const next_icon
                    = ( btn_act == GUI_SKETCH_ACTION_NEXT_PAGE )
                    ? gui_resources_get_sketch_page_down_bold( (*this_).resources )
                    : gui_resources_get_sketch_page_down_gray( (*this_).resources );
                gui_sketch_texture_draw( (*this_).texture_downloader,
                                         next_icon,
                                         shape_int_rectangle_get_left( next_box ),
                                         shape_int_rectangle_get_top( next_box ),
                                         cr
                                       );
            }
        }

        g_object_unref(font_layout);
    }

    U8_TRACE_END();
}

void gui_sketch_result_list_private_draw_element( gui_sketch_result_list_t *this_,
                                                  const pos_search_result_t *element,
                                                  const gui_marked_set_t *marker,
                                                  PangoLayout *font_layout,
                                                  cairo_t *cr )
{
    U8_TRACE_BEGIN();
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
        const data_type_t result_type = data_search_result_get_match_type( result );
        gui_type_resource_t *const type_data
            = gui_type_resource_list_get_type ( &((*this_).selector), result_type );
        GdkTexture *const icon = gui_type_resource_get_icon( type_data );

        /* where to draw to */
        const shape_int_rectangle_t *const icon_box
            = pos_search_result_get_icon_box_const( element );
        const int x = shape_int_rectangle_get_left(icon_box);
        const int y = shape_int_rectangle_get_top(icon_box);
        /* double icon_width = gdk_texture_get_width ( icon ); */
        /* double icon_height = gdk_texture_get_width ( icon ); */

        /* do draw */
        gui_sketch_texture_draw( (*this_).texture_downloader, icon, x, y, cr );
    }

    U8_TRACE_END();
}


/*
Copyright 2018-2026 Andreas Warnke

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
