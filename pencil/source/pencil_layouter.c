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

    pencil_diagram_painter_init( &((*this_).diagram_painter) );

    pencil_relationship_layouter_init( &((*this_).pencil_relationship_layouter),
                                       input_data,
                                       &((*this_).layout_data),
                                       &((*this_).pencil_size),
                                       &((*this_).diagram_draw_area)
                                     );
    pencil_classifier_layouter_init( &((*this_).pencil_classifier_layouter),
                                     input_data,
                                     &((*this_).layout_data),
                                     &((*this_).pencil_size),
                                     &((*this_).diagram_draw_area),
                                     &((*this_).default_classifier_size),
                                     &((*this_).x_scale),
                                     &((*this_).y_scale)
                                   );

    TRACE_END();
}

void pencil_layouter_destroy( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_layouter_destroy( &((*this_).pencil_classifier_layouter) );
    pencil_relationship_layouter_destroy( &((*this_).pencil_relationship_layouter) );

    pencil_diagram_painter_destroy( &((*this_).diagram_painter) );

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
    double width = geometry_rectangle_get_width ( &((*this_).diagram_bounds) );
    double height = geometry_rectangle_get_height ( &((*this_).diagram_bounds) );
    pencil_size_reinit( &((*this_).pencil_size), width, height );

    pencil_diagram_painter_get_drawing_space ( &((*this_).diagram_painter),
                                               pencil_input_data_get_diagram_ptr ( input_data ),
                                               &((*this_).pencil_size),
                                               &((*this_).diagram_bounds),
                                               &((*this_).diagram_draw_area) );

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

    TRACE_END();
}

void pencil_layouter_layout_elements ( pencil_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* adjust the default classifier rectangle */
    pencil_layouter_private_propose_default_classifier_size( this_ );

    /* store the classifier bounds into input_data_layouter_t */
    pencil_classifier_layouter_estimate_bounds( &((*this_).pencil_classifier_layouter), font_layout );

    /* parent classifiers embrace their children */
    pencil_classifier_layouter_embrace_children( &((*this_).pencil_classifier_layouter) );

    /* move the classifiers to avoid overlaps */
    pencil_classifier_layouter_move_to_avoid_overlaps( &((*this_).pencil_classifier_layouter) );

    /* calculate the relationship shapes */
    pencil_relationship_layouter_do_layout( &((*this_).pencil_relationship_layouter) );

    /* hide containment relationships if children are embraced */
    pencil_classifier_layouter_hide_relations_of_embraced_children( &((*this_).pencil_classifier_layouter) );

    TRACE_END();
}

void pencil_layouter_private_propose_default_classifier_size ( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* adjust the default classifier rectangle */
    uint32_t count_clasfy;
    count_clasfy = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );

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

    TRACE_END();
}

pencil_error_t pencil_layouter_get_object_id_at_pos ( pencil_layouter_t *this_,
                                                      double x,
                                                      double y,
                                                      double snap_distance,
                                                      pencil_visible_object_id_t* out_selected_id,
                                                      pencil_visible_object_id_t* out_surrounding_id )
{
    TRACE_BEGIN();
    assert( NULL != out_selected_id );
    assert( NULL != out_surrounding_id );

    pencil_error_t result = PENCIL_ERROR_NONE;
    pencil_visible_object_id_reinit_void( out_selected_id );
    pencil_visible_object_id_reinit_void( out_surrounding_id );
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );

    if ( geometry_rectangle_contains( &((*this_).diagram_bounds), x, y ) && data_diagram_is_valid(diag) )
    {
        /* check the relationship shapes */
        {
            uint32_t count_relations;
            count_relations = pencil_input_data_get_relationship_count ( (*this_).input_data );
            uint32_t matching_relations_found = 0;

            for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
            {
                geometry_connector_t *relationship_shape;
                relationship_shape = pencil_input_data_layout_get_relationship_shape_ptr( &((*this_).layout_data), rel_index );

                if ( geometry_connector_is_close( relationship_shape, x, y, snap_distance ) )
                {
                    /* ensure that every relation at that location can be selected by small mouse movements */
                    if ( ((uint32_t)(x+y))%(matching_relations_found+1) == 0 )
                    {
                        data_relationship_t *current_relation;
                        current_relation = pencil_input_data_get_relationship_ptr ( (*this_).input_data, rel_index );

                        pencil_visible_object_id_reinit_by_table_and_id ( out_selected_id,
                                                                        DATA_TABLE_RELATIONSHIP,
                                                                        data_relationship_get_id( current_relation ),
                                                                        DATA_TABLE_RELATIONSHIP,
                                                                        data_relationship_get_id( current_relation )
                                                                        );
                    }
                    matching_relations_found ++;
                }
            }
        }

        /* determine a classifier at the given position */
        if ( ! pencil_visible_object_id_is_valid( out_selected_id ) )
        {
            result = pencil_layouter_private_get_classifier_id_at_pos( this_,
                                                                       x,
                                                                       y,
                                                                       out_selected_id,
                                                                       out_surrounding_id
                                                                     );
        }

        /* fallback: return the diagram */
        if ( ! pencil_visible_object_id_is_valid( out_selected_id ) )
        {
            pencil_visible_object_id_reinit_by_table_and_id ( out_selected_id,
                                                              DATA_TABLE_DIAGRAM,
                                                              data_diagram_get_id(diag),
                                                              DATA_TABLE_DIAGRAM,
                                                              data_diagram_get_id(diag)
                                                            );
        }
        if ( ! pencil_visible_object_id_is_valid( out_surrounding_id ) )
        {
            pencil_visible_object_id_reinit_by_table_and_id ( out_surrounding_id,
                                                              DATA_TABLE_DIAGRAM,
                                                              data_diagram_get_id(diag),
                                                              DATA_TABLE_DIAGRAM,
                                                              data_diagram_get_id(diag)
                                                            );
        }
    }
    else
    {
        TRACE_INFO( "given location outside diagram or no diagram chosen" );
        result = PENCIL_ERROR_OUT_OF_BOUNDS;
    }

    TRACE_END_ERR( result );
    return result;
}

pencil_error_t pencil_layouter_private_get_classifier_id_at_pos ( pencil_layouter_t *this_,
                                                                  double x,
                                                                  double y,
                                                                  pencil_visible_object_id_t* out_selected_id,
                                                                  pencil_visible_object_id_t* out_surrounding_id )
{
    TRACE_BEGIN();
    assert( NULL != out_selected_id );
    assert( NULL != out_surrounding_id );

    pencil_error_t result = PENCIL_ERROR_NONE;

    if ( geometry_rectangle_contains( &((*this_).diagram_draw_area), x, y ) )
    {
        /* iterate over all classifiers */
        uint32_t count;
        count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
        double surrounding_classifier_area = geometry_rectangle_get_area( &((*this_).diagram_draw_area) );

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
                geometry_rectangle_t *classifier_space;
                classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr ( &((*this_).layout_data), index );
                classifier_space = pencil_input_data_layout_get_classifier_space_ptr ( &((*this_).layout_data), index );

                if ( geometry_rectangle_contains( classifier_bounds, x, y ) )
                {
                    if ( geometry_rectangle_contains( classifier_space, x, y ) )
                    {
                        /* check all contained features */
                        uint32_t linenumber = 0;
                        double lineheight = pencil_size_get_standard_font_size( &((*this_).pencil_size) )
                        + 2.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size) );
                        for ( uint32_t f_idx = 0; f_idx < pencil_input_data_get_feature_count ( (*this_).input_data ); f_idx ++ )
                        {
                            data_feature_t *the_feature;
                            the_feature = pencil_input_data_get_feature_ptr ( (*this_).input_data, f_idx );
                            if ( data_feature_get_classifier_id( the_feature ) == data_classifier_get_id( classifier ) )
                            {
                                geometry_rectangle_t feature_bounds;
                                feature_bounds = pencil_layouter_get_feature_bounds( this_,
                                                                                     data_classifier_get_id( classifier ),
                                                                                     index,
                                                                                     f_idx,
                                                                                     linenumber
                                                                                   );
                                if ( geometry_rectangle_contains( &feature_bounds, x, y ) )
                                {
                                    /* feature is found */
                                    pencil_visible_object_id_reinit_by_table_and_id ( out_selected_id,
                                                                                      DATA_TABLE_FEATURE,
                                                                                      data_feature_get_id( the_feature ),
                                                                                      DATA_TABLE_FEATURE,
                                                                                      data_feature_get_id( the_feature )
                                                                                    );
                                }
                                linenumber ++;
                                geometry_rectangle_destroy( &feature_bounds );
                            }
                        }
                        /* surrounding classifier is found. select it if it is the smallest found area */
                        double current_classifier_area = geometry_rectangle_get_area( classifier_space );
                        if ( current_classifier_area < surrounding_classifier_area )
                        {
                            surrounding_classifier_area = current_classifier_area;
                            pencil_visible_object_id_reinit_by_table_and_id ( out_surrounding_id,
                                                                            DATA_TABLE_DIAGRAMELEMENT,
                                                                            data_diagramelement_get_id( diagramelement ),
                                                                            DATA_TABLE_CLASSIFIER,
                                                                            data_classifier_get_id( classifier )
                                                                            );
                        }
                    }
                    else
                    {
                        /* classifier is found */
                        pencil_visible_object_id_reinit_by_table_and_id ( out_selected_id,
                                                                          DATA_TABLE_DIAGRAMELEMENT,
                                                                          data_diagramelement_get_id( diagramelement ),
                                                                          DATA_TABLE_CLASSIFIER,
                                                                          data_classifier_get_id( classifier )
                                                                        );
                    }
                }
            }
            else
            {
                TSLOG_ERROR("invalid visible classifier in array!");
            }
        }
    }

    TRACE_END_ERR( result );
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
