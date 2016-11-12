/* File: pencil_diagram_painter.c; Copyright and License: see below */

#include "pencil_diagram_painter.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_diagram_painter_init( pencil_diagram_painter_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    pencil_classifier_painter_init( &((*this_).classifier_painter) );
    pencil_private_marker_init( &((*this_).marker) );
    geometry_rectangle_init_empty( &((*this_).diagram_bounds) );
    pencil_size_init_empty( &((*this_).pencil_size) );
    geometry_rectangle_init_empty( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_init( &((*this_).x_scale), 0.0, 1.0 );
    geometry_non_linear_scale_init( &((*this_).y_scale), 0.0, 1.0 );
    geometry_rectangle_init_empty( &((*this_).default_classifier_size) );
    (*this_).input_data = input_data;

    TRACE_END();
}

void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_painter_destroy( &((*this_).classifier_painter) );
    pencil_private_marker_destroy( &((*this_).marker) );
    geometry_rectangle_destroy( &((*this_).diagram_bounds) );
    pencil_size_destroy( &((*this_).pencil_size) );
    geometry_rectangle_destroy( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_destroy( &((*this_).x_scale) );
    geometry_non_linear_scale_destroy( &((*this_).y_scale) );
    geometry_rectangle_destroy( &((*this_).default_classifier_size) );
    (*this_).input_data = NULL;

    TRACE_END();
}

void pencil_diagram_painter_do_layout ( pencil_diagram_painter_t *this_,
                                        pencil_input_data_t *input_data,
                                        geometry_rectangle_t diagram_bounds )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    /* update the pointer to the input data */
    (*this_).input_data = input_data;

    /* update the bounding rectangle */
    geometry_rectangle_replace( &((*this_).diagram_bounds), &diagram_bounds );

    /* calculate the pencil-sizes and the drawing rectangle */
    double left = geometry_rectangle_get_left ( &((*this_).diagram_bounds) );
    double top = geometry_rectangle_get_top ( &((*this_).diagram_bounds) );
    double right = geometry_rectangle_get_right ( &((*this_).diagram_bounds) );
    double bottom = geometry_rectangle_get_bottom ( &((*this_).diagram_bounds) );
    double width = geometry_rectangle_get_width ( &((*this_).diagram_bounds) );
    double height = geometry_rectangle_get_height ( &((*this_).diagram_bounds) );
    pencil_size_reinit( &((*this_).pencil_size), width, height );
    double gap = pencil_size_get_standard_object_border( &((*this_).pencil_size) );
    double f_size = pencil_size_get_standard_font_size( &((*this_).pencil_size) );
    double f_line_gap = pencil_size_get_font_line_gap( &((*this_).pencil_size) );
    geometry_rectangle_reinit( &((*this_).diagram_draw_area), left+gap, top+gap+f_size+f_line_gap, width-2.0*gap, height-2.0*gap-f_size-f_line_gap );

    /* calculate the axis scales */
    double draw_left = geometry_rectangle_get_left ( &((*this_).diagram_draw_area) );
    double draw_top = geometry_rectangle_get_top ( &((*this_).diagram_draw_area) );
    double draw_right = geometry_rectangle_get_right ( &((*this_).diagram_draw_area) );
    double draw_bottom = geometry_rectangle_get_bottom ( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_reinit( &((*this_).x_scale), draw_left, draw_right );
    geometry_non_linear_scale_reinit( &((*this_).y_scale), draw_top, draw_bottom );

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_input_data_get_visible_classifier_count ( input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = pencil_input_data_get_visible_classifier_ptr ( input_data, index );

        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            data_classifier_t *classifier;
            classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );

            /* adjust the non-linear scales for this classifier */
            geometry_non_linear_scale_add_order ( &((*this_).x_scale), data_classifier_get_x_order( classifier ) );
            geometry_non_linear_scale_add_order ( &((*this_).y_scale), data_classifier_get_y_order( classifier ) );
        }
    }

    /* adjust the default classifier rectangle */
    double diagram_area = geometry_rectangle_get_area( &((*this_).diagram_draw_area) );
    double classifier_area;
    if ( count > 0 )
    {
        classifier_area = diagram_area / count * (3.0/8.0);
    }
    else
    {
        classifier_area = diagram_area * (3.0/8.0);
    }
    double half_width = sqrt(classifier_area);
    double half_height = half_width / 4.0;
    geometry_rectangle_reinit( &((*this_).default_classifier_size), -half_width, -half_height, 2.0 * half_width, 2.0 * half_height );

    TRACE_END();
}

void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   data_id_t mark_focused,
                                   data_id_t mark_highlighted,
                                   data_small_set_t *mark_selected,
                                   cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    pencil_size_t *pencil_size = &((*this_).pencil_size);

    double left = geometry_rectangle_get_left ( &((*this_).diagram_bounds) );
    double top = geometry_rectangle_get_top ( &((*this_).diagram_bounds) );
    double right = geometry_rectangle_get_right ( &((*this_).diagram_bounds) );
    double bottom = geometry_rectangle_get_bottom ( &((*this_).diagram_bounds) );
    double width = geometry_rectangle_get_width ( &((*this_).diagram_bounds) );
    double height = geometry_rectangle_get_height ( &((*this_).diagram_bounds) );

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

    /* draw all contained classifiers */
    if (( width > 10.0 ) && ( height > 25.0 ))
    {
        pencil_diagram_painter_private_draw_classifiers ( this_,
                                                          mark_focused,
                                                          mark_highlighted,
                                                          mark_selected,
                                                          cr
                                                        );
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

    TRACE_END();
}

void pencil_diagram_painter_private_draw_classifiers ( pencil_diagram_painter_t *this_,
                                                       data_id_t mark_focused,
                                                       data_id_t mark_highlighted,
                                                       data_small_set_t *mark_selected,
                                                       cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    geometry_rectangle_t focused_rect;
    geometry_rectangle_init_empty( &focused_rect );

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );

        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            data_classifier_t *classifier;
            data_diagramelement_t *diagramelement;
            classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
            diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );

            geometry_rectangle_t classifier_bounds;
            classifier_bounds = pencil_diagram_painter_private_get_classifier_bounds( this_, classifier );

            pencil_classifier_painter_draw( &((*this_).classifier_painter),
                                            visible_classifier,
                                            data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ),
                                            data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) ),
                                            data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ),
                                            &((*this_).pencil_size),
                                            cr,
                                            classifier_bounds );

            if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ))
            {
                geometry_rectangle_replace( &focused_rect, &classifier_bounds );
            }

            geometry_rectangle_destroy( &classifier_bounds );
        }
        else
        {
            TSLOG_ERROR("invalid visible classifier in array!");
        }
    }

    /* mark focused */
    if ( ! geometry_rectangle_is_empty( &focused_rect ) )
    {
        pencil_private_marker_mark_focused_rectangle( &((*this_).marker), focused_rect, cr );
    }

    geometry_rectangle_destroy( &focused_rect );
    TRACE_END();
}

geometry_rectangle_t pencil_diagram_painter_private_get_classifier_bounds ( pencil_diagram_painter_t *this_, data_classifier_t *classifier )
{
    TRACE_BEGIN();
    assert( NULL != classifier );

    geometry_rectangle_t result_rect;
    geometry_rectangle_copy( &result_rect, &((*this_).default_classifier_size) );
    int32_t order_x = data_classifier_get_x_order( classifier );
    int32_t order_y = data_classifier_get_y_order( classifier );
    double center_x = geometry_non_linear_scale_get_location( &((*this_).x_scale), order_x );
    double center_y = geometry_non_linear_scale_get_location( &((*this_).y_scale), order_y );
    geometry_rectangle_shift( &result_rect, center_x, center_y );

    TRACE_END();
    return result_rect;
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

    if ( geometry_rectangle_contains( &((*this_).diagram_bounds), x, y ) && data_diagram_is_valid(diag) )
    {
        result = pencil_diagram_painter_private_get_classifier_id_at_pos( this_, x, y, dereference );

        if ( ! data_id_is_valid( &result ) )
        {
            data_id_reinit( &result, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) );
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

    if ( geometry_rectangle_contains( &((*this_).diagram_draw_area), x, y ) )
    {
        /* iterate over all classifiers */
        uint32_t count;
        count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
        for ( uint32_t index = 0; index < count; index ++ )
        {
            data_visible_classifier_t *visible_classifier;
            visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );

            if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
            {
                data_classifier_t *classifier;
                data_diagramelement_t *diagramelement;
                classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
                diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );

                geometry_rectangle_t classifier_bounds;
                classifier_bounds = pencil_diagram_painter_private_get_classifier_bounds( this_, classifier );

                if ( geometry_rectangle_contains( &classifier_bounds, x, y ) )
                {
                    if ( dereference )
                    {
                        data_id_reinit( &result, DATA_TABLE_CLASSIFIER, data_classifier_get_id( classifier ) );
                    }
                    else
                    {
                        data_id_reinit( &result, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) );
                    }
                }

                geometry_rectangle_destroy( &classifier_bounds );
            }
            else
            {
                TSLOG_ERROR("invalid visible classifier in array!");
            }
        }
    }

    TRACE_END();
    return result;
}

universal_int32_pair_t pencil_diagram_painter_get_order_at_pos ( pencil_diagram_painter_t *this_,
                                                                 double x,
                                                                 double y )
{
    TRACE_BEGIN();

    universal_int32_pair_t result;
    int32_t x_order = geometry_non_linear_scale_get_order( &((*this_).x_scale), x );
    int32_t y_order = geometry_non_linear_scale_get_order( &((*this_).y_scale), y );
    universal_int32_pair_init( &result, x_order, y_order );

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
