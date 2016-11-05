/* File: pencil_diagram_painter.c; Copyright and License: see below */

#include "pencil_diagram_painter.h"
#include "pencil_size.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void pencil_diagram_painter_init( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_painter_init( &((*this_).classifier_painter) );
    pencil_private_marker_init( &((*this_).marker) );
    geometry_rectangle_init_empty( &((*this_).diagram_bounds) );
    geometry_non_linear_scale_init( &((*this_).x_scale), 0.0, 1.0 );
    geometry_non_linear_scale_init( &((*this_).y_scale), 0.0, 1.0 );

    TRACE_END();
}

void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_painter_destroy( &((*this_).classifier_painter) );
    pencil_private_marker_destroy( &((*this_).marker) );
    geometry_rectangle_destroy( &((*this_).diagram_bounds) );
    geometry_non_linear_scale_destroy( &((*this_).x_scale) );
    geometry_non_linear_scale_destroy( &((*this_).y_scale) );

    TRACE_END();
}

void pencil_diagram_painter_do_layout ( pencil_diagram_painter_t *this_,
                                        pencil_input_data_t *input_data,
                                        geometry_rectangle_t diagram_bounds )
{
    TRACE_BEGIN();

    (*this_).input_data = input_data;
    geometry_rectangle_replace( &((*this_).diagram_bounds), &diagram_bounds );
    geometry_non_linear_scale_reinit( &((*this_).x_scale), 0.0, 1.0 );
    geometry_non_linear_scale_reinit( &((*this_).y_scale), 0.0, 1.0 );

    TRACE_END();
}

void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   data_id_t mark_focused,
                                   data_id_t mark_highlighted,
                                   data_small_set_t *mark_selected,
                                   cairo_t *cr )
{
    TRACE_BEGIN();

    pencil_size_t pencil_size_object;
    pencil_size_t *pencil_size = &pencil_size_object;

    double left = geometry_rectangle_get_left ( &((*this_).diagram_bounds) );
    double top = geometry_rectangle_get_top ( &((*this_).diagram_bounds) );
    double right = geometry_rectangle_get_right ( &((*this_).diagram_bounds) );
    double bottom = geometry_rectangle_get_bottom ( &((*this_).diagram_bounds) );
    double width = geometry_rectangle_get_width ( &((*this_).diagram_bounds) );
    double height = geometry_rectangle_get_height ( &((*this_).diagram_bounds) );
    pencil_size_init( pencil_size, width, height );

    TRACE_INFO_INT( "w", (int)(width) );
    TRACE_INFO_INT( "h", (int)(height) );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_size = pencil_size_get_standard_font_size( pencil_size );
    double f_ascent = pencil_size_get_standard_font_ascent( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    /* draw diagram border and name */
    {
        data_diagram_t *diag;
        diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );
        TRACE_INFO_INT("drawing diagram id",data_diagram_get_id(diag));

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );
        if ( data_diagram_is_valid(diag) )
        {
            /* draw border line */
            if ( data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ))
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
            cairo_move_to ( cr, left+gap, top+gap+f_size+f_line_gap );
            cairo_line_to ( cr, left+(width/3.0), top+gap+f_size+f_line_gap );
            cairo_line_to ( cr, left+(width/3.0)+4.0, top+gap+f_size+f_line_gap-4.0 );
            cairo_line_to ( cr, left+(width/3.0)+4.0, top+gap );
            cairo_stroke (cr);

            cairo_set_font_size ( cr, f_size );
            cairo_move_to ( cr, left+2.0*gap, top+gap+f_ascent+f_line_gap );
            cairo_show_text ( cr, data_diagram_get_name_ptr( diag ) );
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

    double tenth_width = width / 10.0;

    /* draw all contained classifiers */
    if (( width > 10.0 ) && ( height > 25.0 ))
    {
        geometry_rectangle_t classifier_bounds;
        geometry_rectangle_init ( &classifier_bounds, left + 2.0 * gap + tenth_width, top + 2.0 * gap + f_size + f_line_gap, width - 4.0 * gap - 2.0 * tenth_width, height - 4.0 * gap - f_size - f_line_gap );
        pencil_classifier_painter_draw ( &((*this_).classifier_painter),
                                         (*this_).input_data,
                                         mark_focused,
                                         mark_highlighted,
                                         mark_selected,
                                         pencil_size,
                                         cr,
                                         classifier_bounds
                                       );
        geometry_rectangle_destroy( &classifier_bounds );
    }

    /* mark focused and highlighted */
    {
        data_diagram_t *diag2;
        diag2 = pencil_input_data_get_diagram_ptr( (*this_).input_data );
        if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag2) ))
        {
            pencil_private_marker_mark_focused_rectangle( &((*this_).marker), (*this_).diagram_bounds, cr );
        }

        if ( data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag2) ))
        {
            geometry_rectangle_t selected_rect;
            geometry_rectangle_init( &selected_rect, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
            pencil_private_marker_mark_selected_rectangle( &((*this_).marker), selected_rect, cr );
            geometry_rectangle_destroy( &selected_rect );
        }
    }

    pencil_size_destroy( pencil_size );
    TRACE_END();
}

data_id_t pencil_diagram_painter_get_object_id_at_pos ( pencil_diagram_painter_t *this_,
                                                        double x,
                                                        double y,
                                                        bool dereference )
{
    TRACE_BEGIN();

    data_id_t result;
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );
    pencil_size_t pencil_size_object;
    pencil_size_t *pencil_size = &pencil_size_object;
    pencil_size_init( pencil_size, geometry_rectangle_get_width( &((*this_).diagram_bounds) ), geometry_rectangle_get_height( &((*this_).diagram_bounds) ) );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_size = pencil_size_get_standard_font_size( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );
    double width = geometry_rectangle_get_width ( &((*this_).diagram_bounds) );
    double tenth_width = width / 10.0;

    if ( geometry_rectangle_contains( &((*this_).diagram_bounds), x, y ) && data_diagram_is_valid(diag) )
    {
        double left = geometry_rectangle_get_left ( &((*this_).diagram_bounds) );
        double top = geometry_rectangle_get_top ( &((*this_).diagram_bounds) );
        double width = geometry_rectangle_get_width ( &((*this_).diagram_bounds) );
        double height = geometry_rectangle_get_height ( &((*this_).diagram_bounds) );

        geometry_rectangle_t classifier_bounds;
        geometry_rectangle_init( &classifier_bounds,
                                 left + 2.0 * gap + tenth_width,
                                 top + 2.0 * gap + f_size + f_line_gap,
                                 width - 4.0 * gap - 2.0 * tenth_width,
                                 height - 4.0 * gap - f_size - f_line_gap
                               );

        result = pencil_diagram_painter_private_get_classifier_id_at_pos( this_, x, y, dereference );

        geometry_rectangle_destroy( &classifier_bounds );

        if ( ! data_id_is_valid( &result ) )
        {
            data_id_destroy( &result );
            data_diagram_t *pencil_input_data_get_diagram_ptr ( pencil_input_data_t *this_ );
            data_id_init( &result, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) );
        }
    }
    else
    {
        TRACE_INFO( "no object at given location or no diagram chosen" );
        data_id_init_void( &result );
    }

    TRACE_END();
    return result;
}

data_id_t pencil_diagram_painter_private_get_classifier_id_at_pos ( pencil_diagram_painter_t *this_,
                                                                    double x,
                                                                    double y,
                                                                    bool dereference )
{
    TRACE_BEGIN();
    data_id_t result;
    data_id_init_void( &result );

    if ( geometry_rectangle_contains( &((*this_).diagram_bounds), x, y ) )
    {
        double top;
        double height;
        top = geometry_rectangle_get_top ( &((*this_).diagram_bounds) );
        height = geometry_rectangle_get_height ( &((*this_).diagram_bounds) );

        uint32_t count;
        count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
        uint32_t index = (uint32_t) (((y-top) * count) / height);
        if ( index < count )
        {
            data_visible_classifier_t *visible_classifier;
            visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );
            if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
            {
                if ( dereference )
                {
                    data_classifier_t *classifier;
                    classifier = data_visible_classifier_get_classifier_ptr ( visible_classifier );
                    data_id_reinit( &result, DATA_TABLE_CLASSIFIER, data_classifier_get_id( classifier ) );
                }
                else
                {
                    data_diagramelement_t *diagramelement;
                    diagramelement = data_visible_classifier_get_diagramelement_ptr ( visible_classifier );
                    data_id_reinit( &result, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) );
                }
            }
        }
    }

    TRACE_END();
    return result;
}

/*
Copyright 2016-2016 Andreas Warnke

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
