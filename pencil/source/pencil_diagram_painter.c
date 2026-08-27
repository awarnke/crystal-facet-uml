/* File: pencil_diagram_painter.c; Copyright and License: see below */

#include "pencil_diagram_painter.h"
#include "u8/u8_trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_diagram_painter_init( pencil_diagram_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    draw_diagram_label_init( &((*this_).draw_diagram_label) );
    draw_diagram_ornaments_init( &((*this_).draw_diagram_ornaments) );

    U8_TRACE_END();
}

void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ )
{
    U8_TRACE_BEGIN();

    draw_diagram_ornaments_destroy( &((*this_).draw_diagram_ornaments) );
    draw_diagram_label_init( &((*this_).draw_diagram_label) );
    pencil_marker_destroy( &((*this_).marker) );

    U8_TRACE_END();
}

void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   const layout_diagram_t *layouted_diagram,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   const data_profile_part_t *profile,
                                   const pencil_size_t *pencil_size,
                                   PangoLayout *font_layout,
                                   cairo_t *cr )
{
    U8_TRACE_BEGIN();
    assert( NULL != layouted_diagram );
    assert( NULL != profile );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != cr );

    const data_diagram_t *const the_diagram = layout_diagram_get_data_const( layouted_diagram );
    const geometry_rectangle_t *const diagram_bounds = layout_diagram_get_bounds_const( layouted_diagram );
    const geometry_rectangle_t *const label_box = layout_diagram_get_label_box_const( layouted_diagram );

    const double left = geometry_rectangle_get_left ( diagram_bounds );
    const double top = geometry_rectangle_get_top ( diagram_bounds );
    const double right = geometry_rectangle_get_right ( diagram_bounds );
    const double bottom = geometry_rectangle_get_bottom ( diagram_bounds );
    const double width = geometry_rectangle_get_width ( diagram_bounds );
    const double height = geometry_rectangle_get_height ( diagram_bounds );

    U8_TRACE_INFO_INT( "w", (int)(width) );
    U8_TRACE_INFO_INT( "h", (int)(height) );

    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* draw diagram border and name */
    {
        U8_TRACE_INFO_INT("drawing diagram id",data_diagram_get_row(the_diagram));

        const double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );
        if ( data_diagram_is_valid(the_diagram) )
        {
            /* draw border line */
            const GdkRGBA fg_color
                = mark_highlighted
                ? pencil_size_get_highlight_color( pencil_size )
                : pencil_size_get_standard_color( pencil_size );
            cairo_set_source_rgba( cr, fg_color.red, fg_color.green, fg_color.blue, fg_color.alpha );
            cairo_rectangle ( cr, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
            cairo_stroke (cr);

            /* draw title corner */
            draw_diagram_label_draw_type_and_name( &((*this_).draw_diagram_label),
                                                   the_diagram,
                                                   profile,
                                                   &fg_color,
                                                   label_box,
                                                   pencil_size,
                                                   font_layout,
                                                   cr
                                                 );

            const double title_corner_height = geometry_rectangle_get_height( label_box ) + 2.0 * gap;
            const double title_corner_edge45 = 0.4 * title_corner_height;
            double title_corner_width
                = geometry_rectangle_get_width( label_box ) + 3.0 * gap + title_corner_edge45;
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
            const GdkRGBA standard_color = pencil_size_get_standard_color( pencil_size );
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
        geometry_rectangle_t label_bounds;
        geometry_rectangle_copy( &label_bounds , diagram_bounds );
        geometry_rectangle_shift( &label_bounds, -2.0 * gap, 2.0 * gap );
        const GdkRGBA grey_color = pencil_size_get_gray_out_color( pencil_size );
        draw_diagram_label_draw_id( &((*this_).draw_diagram_label),
                                    the_diagram,
                                    &grey_color,   /* fg_color */
                                    &label_bounds,
                                    pencil_size,
                                    font_layout,
                                    cr
                                  );
        geometry_rectangle_destroy( &label_bounds );
    }

    /* draw ornaments */
    if ( data_diagram_is_valid( the_diagram ) )
    {
        /* draw color */
        const GdkRGBA fg_color
            = mark_highlighted
            ? pencil_size_get_highlight_color( pencil_size )
            : pencil_size_get_standard_color( pencil_size );
        cairo_set_source_rgba( cr, fg_color.red, fg_color.green, fg_color.blue, fg_color.alpha );

        /* get locations */
        const geometry_rectangle_t *const diagram_draw_area = layout_diagram_get_draw_area_const( layouted_diagram );
        const double draw_left = geometry_rectangle_get_left ( diagram_draw_area );
        const double draw_bottom = geometry_rectangle_get_bottom ( diagram_draw_area );
        const double draw_width = geometry_rectangle_get_width ( diagram_draw_area );

        if ( data_diagram_get_diagram_type( the_diagram ) == DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM )
        {
            const double obj_dist = pencil_size_get_preferred_object_distance( pencil_size );
            const double used_draw_width = draw_width - 2.0 * obj_dist;
            /* const double phi = 1.6180339; */
            const double minor_ratio = (1.0 - 0.6180339);
            const double classifier_width = used_draw_width * minor_ratio / 2.0;
            const geometry_rectangle_t scale_bounds
                = { .left = draw_left + obj_dist + classifier_width + obj_dist,
                    .top = draw_bottom - obj_dist,
                    .width = used_draw_width - classifier_width - obj_dist,
                    .height = obj_dist
                  };
            draw_diagram_ornaments_draw_scale( &((*this_).draw_diagram_ornaments),
                                               &scale_bounds,
                                               pencil_size,
                                               cr
                                             );
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
