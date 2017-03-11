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
                                   data_diagram_t *the_diagram,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   const pencil_size_t *pencil_size,
                                   const geometry_rectangle_t *diagram_bounds,
                                   PangoLayout *layout,
                                   cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != the_diagram );
    assert( NULL != layout );
    assert( NULL != cr );
    assert( NULL != diagram_bounds );

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
        TRACE_INFO_INT("drawing diagram id",data_diagram_get_id(the_diagram));

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
            pango_layout_set_font_description (layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (layout, data_diagram_get_name_ptr( the_diagram ), -1);
            pango_layout_get_pixel_size (layout, &text_width, &text_height);
            cairo_move_to ( cr, left + gap + f_tab_size, top+gap );
            pango_cairo_show_layout (cr, layout);

            static const double EDGE_45 = 4.0;
            double title_corner_width = text_width + gap + 2.0*f_tab_size + EDGE_45;
            if ( title_corner_width > width*0.9 )
            {
                title_corner_width = width*0.9;
            }
            cairo_move_to ( cr, left+gap, top+gap+text_height+f_line_gap );
            cairo_line_to ( cr, left+title_corner_width - EDGE_45, top+gap+text_height+f_line_gap );
            cairo_line_to ( cr, left+title_corner_width, top+gap+text_height+f_line_gap-EDGE_45 );
            cairo_line_to ( cr, left+title_corner_width, top+gap );
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
                                                data_diagram_t *the_diagram,
                                                const pencil_size_t *pencil_size,
                                                const geometry_rectangle_t *diagram_bounds,
                                                geometry_rectangle_t *out_diagram_space )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != the_diagram );
    assert( NULL != out_diagram_space );
    assert( NULL != diagram_bounds );

    double left = geometry_rectangle_get_left ( diagram_bounds );
    double top = geometry_rectangle_get_top ( diagram_bounds );
    double width = geometry_rectangle_get_width ( diagram_bounds );
    double height = geometry_rectangle_get_height ( diagram_bounds );
    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_size = pencil_size_get_standard_font_size( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    geometry_rectangle_reinit( out_diagram_space, left+gap, top+gap+f_size+f_line_gap, width-2.0*gap, height-2.0*gap-f_size-f_line_gap );

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
