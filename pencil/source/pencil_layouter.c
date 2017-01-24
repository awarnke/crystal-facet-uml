/* File: pencil_layouter.c; Copyright and License: see below */

#include "pencil_layouter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_layouter_init( pencil_layouter_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    geometry_rectangle_init_empty( &((*this_).diagram_bounds) );
    pencil_size_init_empty( &((*this_).pencil_size) );
    geometry_rectangle_init_empty( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_init( &((*this_).x_scale), 0.0, 1.0 );
    geometry_non_linear_scale_init( &((*this_).y_scale), 0.0, 1.0 );
    geometry_rectangle_init_empty( &((*this_).default_classifier_size) );

    pencil_input_data_layout_init( &((*this_).layout_data) );
    (*this_).input_data = input_data;

    TRACE_END();
}

void pencil_layouter_destroy( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    geometry_rectangle_destroy( &((*this_).diagram_bounds) );
    pencil_size_destroy( &((*this_).pencil_size) );
    geometry_rectangle_destroy( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_destroy( &((*this_).x_scale) );
    geometry_non_linear_scale_destroy( &((*this_).y_scale) );
    geometry_rectangle_destroy( &((*this_).default_classifier_size) );

    pencil_input_data_layout_destroy( &((*this_).layout_data) );
    (*this_).input_data = NULL;

    TRACE_END();
}



void pencil_layouter_layout_grid ( pencil_layouter_t *this_,
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
    double draw_width = geometry_rectangle_get_width ( &((*this_).diagram_draw_area) );
    double draw_height = geometry_rectangle_get_height ( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_reinit( &((*this_).x_scale), draw_left + 0.04*draw_width, draw_right - 0.04*draw_width );
    geometry_non_linear_scale_reinit( &((*this_).y_scale), draw_top +0.04*draw_height, draw_bottom - 0.04*draw_height );

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

    TRACE_END();
}

void pencil_layouter_layout_elements ( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* adjust the default classifier rectangle */
    uint32_t count_clasfy;
    count_clasfy = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
    {
        double diagram_area = geometry_rectangle_get_area( &((*this_).diagram_draw_area) );
        double classifier_area;
        if ( count_clasfy > 0 )
        {
            classifier_area = diagram_area / count_clasfy * (0.10);
        }
        else
        {
            classifier_area = diagram_area * (0.10);
        }
        double half_width = sqrt(classifier_area);
        double half_height = half_width / 2.1;
        geometry_rectangle_reinit( &((*this_).default_classifier_size), -half_width, -half_height, 2.0 * half_width, 2.0 * half_height );
    }

    /* store the classifier bounds into input_data_layouter_t */
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        data_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );

        if (( visible_classifier2 != NULL ) && ( data_visible_classifier_is_valid( visible_classifier2 ) ))
        {
            data_classifier_t *classifier2;
            classifier2 = data_visible_classifier_get_classifier_ptr( visible_classifier2 );

            geometry_rectangle_t *classifier_bounds;
            classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), index );

            /* overwrite directly internal attributes of (*this_).layout_data */
            geometry_rectangle_replace( classifier_bounds, &((*this_).default_classifier_size) );
            int32_t order_x = data_classifier_get_x_order( classifier2 );
            int32_t order_y = data_classifier_get_y_order( classifier2 );
            double center_x = geometry_non_linear_scale_get_location( &((*this_).x_scale), order_x );
            double center_y = geometry_non_linear_scale_get_location( &((*this_).y_scale), order_y );
            geometry_rectangle_shift( classifier_bounds, center_x, center_y );
        }
    }

    /* calculate the relationship shapes */
    {
        uint32_t count_relations;
        count_relations = pencil_input_data_get_relationship_count ( (*this_).input_data );

        for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
        {
            data_relationship_t *current_relation;
            current_relation = pencil_input_data_get_relationship_ptr ( (*this_).input_data, rel_index );

            geometry_connector_t *relationship_shape;
            relationship_shape = pencil_input_data_layout_get_relationship_shape_ptr( &((*this_).layout_data), rel_index );

/* todo */
            geometry_connector_reinit_horizontal ( relationship_shape, 70.0, 50.0, 200.0, 80.0, 65.0 );
        }
    }

    TRACE_END();
}

data_id_t pencil_layouter_get_object_id_at_pos ( pencil_layouter_t *this_,
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
        result = pencil_layouter_private_get_classifier_id_at_pos( this_, x, y, dereference );

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

data_id_t pencil_layouter_private_get_classifier_id_at_pos ( pencil_layouter_t *this_,
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

                geometry_rectangle_t *classifier_bounds;

                classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr ( &((*this_).layout_data), index );

                if ( geometry_rectangle_contains( classifier_bounds, x, y ) )
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

universal_int32_pair_t pencil_layouter_get_order_at_pos ( pencil_layouter_t *this_,
                                                          double x,
                                                          double y )
{
    TRACE_BEGIN();

    universal_int32_pair_t result;
    int32_t x_order = geometry_non_linear_scale_get_order( &((*this_).x_scale), x, 3.0 );
    int32_t y_order = geometry_non_linear_scale_get_order( &((*this_).y_scale), y, 3.0 );
    universal_int32_pair_init( &result, x_order, y_order );

    TRACE_END();
    return result;
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
