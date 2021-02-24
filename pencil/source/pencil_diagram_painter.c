/* File: pencil_diagram_painter.c; Copyright and License: see below */

#include "pencil_diagram_painter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_diagram_painter_init( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );

    TRACE_END();
}

void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_diagram_painter_draw ( const pencil_diagram_painter_t *this_,
                                   const layout_diagram_t *layouted_diagram,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   const pencil_size_t *pencil_size,
                                   PangoLayout *font_layout,
                                   cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_diagram );
    assert( NULL != font_layout );
    assert( NULL != cr );

    const data_diagram_t *const the_diagram = layout_diagram_get_data_const( layouted_diagram );
    const geometry_rectangle_t *const diagram_bounds = layout_diagram_get_bounds_const( layouted_diagram );

    double left = geometry_rectangle_get_left ( diagram_bounds );
    double top = geometry_rectangle_get_top ( diagram_bounds );
    double right = geometry_rectangle_get_right ( diagram_bounds );
    double bottom = geometry_rectangle_get_bottom ( diagram_bounds );
    double width = geometry_rectangle_get_width ( diagram_bounds );
    double height = geometry_rectangle_get_height ( diagram_bounds );

    TRACE_INFO_INT( "w", (int)(width) );
    TRACE_INFO_INT( "h", (int)(height) );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );
    double f_tab_size = pencil_size_get_font_tab_size( pencil_size );

    /* draw diagram border and name */
    {
        TRACE_INFO_INT("drawing diagram id",data_diagram_get_row_id(the_diagram));

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );
        if ( data_diagram_is_valid(the_diagram) )
        {
            /* draw border line */
            if ( mark_highlighted )
            {
                GdkRGBA highlight_color;
                highlight_color = pencil_size_get_highlight_color( pencil_size );
                cairo_set_source_rgba( cr, highlight_color.red, highlight_color.green, highlight_color.blue, highlight_color.alpha );
            }
            else
            {
                GdkRGBA standard_color;
                standard_color = pencil_size_get_standard_color( pencil_size );
                cairo_set_source_rgba( cr, standard_color.red, standard_color.green, standard_color.blue, standard_color.alpha );
            }
            cairo_rectangle ( cr, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
            cairo_stroke (cr);

            /* draw title corner */
            int text_width;
            int text_height;
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (font_layout, data_diagram_get_name_ptr( the_diagram ), -1);
            pango_layout_get_pixel_size (font_layout, &text_width, &text_height);
            cairo_move_to ( cr, left + gap + f_tab_size, top+gap );
            pango_cairo_show_layout (cr, font_layout);

            double title_corner_height = text_height+f_line_gap;
            double title_corner_edge45 = 0.4 * title_corner_height;
            double title_corner_width = text_width + gap + 2.0*f_tab_size + title_corner_edge45;
            if ( title_corner_width > width*0.9 )
            {
                title_corner_width = width*0.9;
            }
            cairo_move_to ( cr, left+gap, top+gap+title_corner_height );
            cairo_line_to ( cr, left+gap+title_corner_width - title_corner_edge45, top+gap+title_corner_height );
            cairo_line_to ( cr, left+gap+title_corner_width, top+gap+title_corner_height-title_corner_edge45 );
            cairo_line_to ( cr, left+gap+title_corner_width, top+gap );
            cairo_stroke (cr);
        }
        else
        {
            /* draw cross line */
            GdkRGBA standard_color;
            standard_color = pencil_size_get_standard_color( pencil_size );
            cairo_set_source_rgba( cr, standard_color.red, standard_color.green, standard_color.blue, standard_color.alpha );
            cairo_move_to ( cr, left, top );
            cairo_line_to ( cr, right, bottom );
            cairo_move_to ( cr, left, bottom );
            cairo_line_to ( cr, right, top );
            cairo_stroke (cr);
        }
    }

    /* draw id */
    {
        /* prepare text */
        const data_id_t the_id = data_diagram_get_data_id(the_diagram);

        char id_buf[DATA_ID_MAX_UTF8STRING_SIZE+5];
        utf8stringbuf_t id_str = UTF8STRINGBUF( id_buf );
        utf8stringbuf_copy_str( id_str, "{id=" );
        data_id_to_utf8stringbuf( &the_id, id_str );
        utf8stringbuf_append_str( id_str, "}" );

        int text4_width;
        int text4_height;
        pango_layout_set_font_description (font_layout, pencil_size_get_footnote_font_description(pencil_size) );
        pango_layout_set_text (font_layout, utf8stringbuf_get_string( id_str ), -1);
        pango_layout_get_pixel_size (font_layout, &text4_width, &text4_height);

        /* draw text */
        GdkRGBA grey_color;
        grey_color = pencil_size_get_gray_out_color( pencil_size );
        cairo_set_source_rgba( cr, grey_color.red, grey_color.green, grey_color.blue, grey_color.alpha );
        cairo_move_to ( cr, left + width - text4_width - gap - gap, top + gap );
        pango_cairo_show_layout (cr, font_layout);
    }

    /* mark focused and highlighted */
    {
        if ( mark_focused )
        {
            pencil_marker_mark_focused_rectangle( &((*this_).marker), *diagram_bounds, cr );
        }

        if ( mark_selected )
        {
            geometry_rectangle_t selected_rect;
            geometry_rectangle_init( &selected_rect, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
            pencil_marker_mark_selected_rectangle( &((*this_).marker), selected_rect, cr );
            geometry_rectangle_destroy( &selected_rect );
        }
    }

    TRACE_END();
}

void pencil_diagram_painter_get_drawing_space ( const pencil_diagram_painter_t *this_,
                                                const data_diagram_t *the_diagram,
                                                const pencil_size_t *pencil_size,
                                                const geometry_rectangle_t *diagram_bounds,
                                                geometry_rectangle_t *out_diagram_space )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != the_diagram );
    assert( NULL != out_diagram_space );
    assert( NULL != diagram_bounds );

    /* determine diagram bounds */
    double left = geometry_rectangle_get_left ( diagram_bounds );
    double top = geometry_rectangle_get_top ( diagram_bounds );
    double width = geometry_rectangle_get_width ( diagram_bounds );
    double height = geometry_rectangle_get_height ( diagram_bounds );
    double gap = pencil_size_get_standard_object_border( pencil_size );

    /* font metrics */
    double f_size = pencil_size_get_standard_font_size( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );
    double f_size_guess = f_size + 4.0 * f_line_gap;  /* here, we do not have a pango layout object to determine the real font size */

    /* calculate new sizes */
    double space_width = width-2.0*gap;
    double space_height = height-2.0*gap-f_size_guess-f_line_gap;
    if ( ( space_width <= 0.0 ) || ( space_height <= 0.0 ) )
    {
        geometry_rectangle_reinit_empty( out_diagram_space );
    }
    else
    {
        geometry_rectangle_reinit( out_diagram_space, left+gap, top+gap+f_size_guess+f_line_gap, space_width, space_height );
    }

    TRACE_END();
}


/*
Copyright 2017-2021 Andreas Warnke

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
