/* File: pencil_feature_layouter.c; Copyright and License: see below */

#include "pencil_feature_layouter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_feature_layouter_init( pencil_feature_layouter_t *this_,
                                   pencil_layout_data_t *layout_data,
                                   pencil_size_t *pencil_size )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;
    pencil_feature_painter_init( &((*this_).feature_painter) );

    TRACE_END();

}

void pencil_feature_layouter_destroy( pencil_feature_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_feature_painter_destroy( &((*this_).feature_painter) );

    TRACE_END();
}

void pencil_feature_layouter_do_layout ( pencil_feature_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();
    assert( UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= PENCIL_LAYOUT_DATA_MAX_FEATURES );

    universal_array_index_sorter_t sorted_features;
    universal_array_index_sorter_init( &sorted_features );

    /* sort the features by their diagramelements */
    uint32_t count_features;
    count_features = pencil_layout_data_get_feature_count ( (*this_).layout_data );
    for ( uint32_t f_idx = 0; f_idx < count_features; f_idx ++ )
    {
        layout_feature_t *feature_layout;
        feature_layout = pencil_layout_data_get_feature_ptr ( (*this_).layout_data, f_idx );
        const data_feature_t *the_feature;
        the_feature = layout_feature_get_data_ptr ( feature_layout );
        layout_visible_classifier_t *layout_classifier;
        layout_classifier = layout_feature_get_classifier_ptr ( feature_layout );

        int err;
        double weight;  /* assuming 52 bit fraction, accuracy is sufficient to multiply a row_id and a list_order */
        weight = (((double)layout_visible_classifier_get_diagramelement_id( layout_classifier )) * (UINT32_MAX+1.0)) + data_feature_get_list_order( the_feature );
        err = universal_array_index_sorter_insert( &sorted_features, f_idx, weight );
        if ( 0 != err )
        {
            TSLOG_ERROR ( "universal_array_index_sorter_t list is full." );
        }
    }

    /* get diagram draw area */
    geometry_rectangle_t *diagram_draw_area;
    data_diagram_type_t diag_type;
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
        diagram_draw_area = layout_diagram_get_draw_area_ptr( diagram_layout );
        const data_diagram_t *diagram_data;
        diagram_data = layout_diagram_get_data_ptr ( diagram_layout );
        diag_type = data_diagram_get_diagram_type ( diagram_data );
    }

    /* get preferred object distance */
    double obj_dist;
    double gap;
    obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    gap = pencil_size_get_standard_object_border( (*this_).pencil_size );

    /* layout the features */
    int64_t last_diagramelement_id = DATA_ID_VOID_ID;
    double y_position_of_next_feature = 0.0;
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted_features );
    for ( uint32_t feature_sort_idx = 0; feature_sort_idx < count_sorted; feature_sort_idx ++ )
    {
        /* get index */
        uint32_t feature_idx;
        feature_idx = universal_array_index_sorter_get_array_index( &sorted_features, feature_sort_idx );

        /* get feature */
        layout_feature_t *feature_layout;
        feature_layout = pencil_layout_data_get_feature_ptr ( (*this_).layout_data, feature_idx );
        const data_feature_t *the_feature;
        the_feature = layout_feature_get_data_ptr ( feature_layout );
        layout_visible_classifier_t *layout_classifier;
        layout_classifier = layout_feature_get_classifier_ptr ( feature_layout );

        /* determine y position */
        int64_t current_diagramelement_id = layout_visible_classifier_get_diagramelement_id( layout_classifier );
        if ( current_diagramelement_id != last_diagramelement_id )
        {
            y_position_of_next_feature = 0.0;
            last_diagramelement_id = current_diagramelement_id;
        }

        /* lifeline */
        if ( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type (the_feature) )
        {
            /* layout feature into parent classifier */
            geometry_rectangle_t *c_bounds = layout_visible_classifier_get_bounds_ptr ( layout_classifier );

            if ( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == diag_type )
            {
                layout_feature_set_direction ( feature_layout, PENCIL_LAYOUT_DIRECTION_RIGHT );
                double c_right = geometry_rectangle_get_right( c_bounds );
                double c_top = geometry_rectangle_get_top( c_bounds );
                double c_height = geometry_rectangle_get_height( c_bounds );
                double dda_right = geometry_rectangle_get_right ( diagram_draw_area );
                geometry_rectangle_t lifeline_bounds;
                geometry_rectangle_init ( &lifeline_bounds,
                                          c_right,
                                          c_top + (0.375 * c_height),
                                          dda_right - c_right - obj_dist,
                                          0.25 * c_height
                                        );
                layout_feature_set_bounds ( feature_layout, &lifeline_bounds );
            }
            else if ( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM == diag_type )
            {
                layout_feature_set_direction ( feature_layout, PENCIL_LAYOUT_DIRECTION_CENTER );
                double c_left = geometry_rectangle_get_left( c_bounds );
                double c_top = geometry_rectangle_get_top( c_bounds );
                double c_width = geometry_rectangle_get_width( c_bounds );
                double c_height = geometry_rectangle_get_height( c_bounds );
                geometry_rectangle_t lifeline_bounds;
                geometry_rectangle_init ( &lifeline_bounds,
                                          c_left + gap,
                                          c_top + gap,
                                          c_width - gap - gap,
                                          c_height - gap - gap
                                        );
                layout_feature_set_bounds ( feature_layout, &lifeline_bounds );
            }
            else
            {
                layout_feature_set_direction ( feature_layout, PENCIL_LAYOUT_DIRECTION_DOWN );
                double c_bottom = geometry_rectangle_get_bottom( c_bounds );
                double c_left = geometry_rectangle_get_left( c_bounds );
                double c_width = geometry_rectangle_get_width( c_bounds );
                double dda_bottom = geometry_rectangle_get_bottom ( diagram_draw_area );
                geometry_rectangle_t lifeline_bounds;
                geometry_rectangle_init ( &lifeline_bounds,
                                          c_left + (0.375 * c_width),
                                          c_bottom,
                                          0.25 * c_width,
                                          dda_bottom - c_bottom - obj_dist
                                        );
                layout_feature_set_bounds ( feature_layout, &lifeline_bounds );
            }
        }
        else if ( DATA_FEATURE_TYPE_PORT == data_feature_get_main_type (the_feature) )
        {
            double nominal_fontsize;
            nominal_fontsize = pencil_size_get_standard_font_size( (*this_).pencil_size );

            /* determine the minimum bounds of the port-feature */
            geometry_rectangle_t f_min_bounds;
            pencil_feature_painter_get_minimum_bounds ( &((*this_).feature_painter),
                                                        the_feature,
                                                        (*this_).pencil_size,
                                                        font_layout,
                                                        &f_min_bounds
            );

            /* layout port-feature into parent classifier */
            geometry_rectangle_t *classifier_space = layout_visible_classifier_get_space_ptr ( layout_classifier );
            geometry_rectangle_t *classifier_bounds = layout_visible_classifier_get_bounds_ptr ( layout_classifier );
            geometry_rectangle_t f_bounds;
            geometry_rectangle_init ( &f_bounds,
                                      geometry_rectangle_get_left( classifier_bounds ) - 0.5 * nominal_fontsize,
                                      geometry_rectangle_get_top( classifier_space ) + y_position_of_next_feature,
                                      geometry_rectangle_get_width( &f_min_bounds ) + nominal_fontsize + gap,
                                      geometry_rectangle_get_height( &f_min_bounds )
            );
            layout_feature_set_bounds ( feature_layout, &f_bounds );
            layout_feature_set_direction ( feature_layout, PENCIL_LAYOUT_DIRECTION_RIGHT );

            /* adjust y position of next feature */
            y_position_of_next_feature += geometry_rectangle_get_height( &f_bounds );
        }
        else  /* not a lifeline nor a port */
        {
            /* determine the minimum bounds of the feature */
            geometry_rectangle_t f_min_bounds;
            pencil_feature_painter_get_minimum_bounds ( &((*this_).feature_painter),
                                                        the_feature,
                                                        (*this_).pencil_size,
                                                        font_layout,
                                                        &f_min_bounds
                                                      );

            /* layout feature into parent classifier */
            geometry_rectangle_t *c_space = layout_visible_classifier_get_space_ptr ( layout_classifier );
            geometry_rectangle_t f_bounds;
            geometry_rectangle_init ( &f_bounds,
                                      geometry_rectangle_get_left( c_space ),
                                      geometry_rectangle_get_top( c_space ) + y_position_of_next_feature,
                                      geometry_rectangle_get_width( c_space ),
                                      geometry_rectangle_get_height( &f_min_bounds )
                                    );
            layout_feature_set_bounds ( feature_layout, &f_bounds );
            layout_feature_set_direction ( feature_layout, PENCIL_LAYOUT_DIRECTION_CENTER );

            /* adjust y position of next feature */
            y_position_of_next_feature += geometry_rectangle_get_height( &f_bounds );
        }
    }

    universal_array_index_sorter_destroy( &sorted_features );

    TRACE_END();
}

void pencil_feature_layouter_calculate_features_bounds ( pencil_feature_layouter_t *this_,
                                                         int64_t diagramelement_id,
                                                         PangoLayout *font_layout,
                                                         geometry_rectangle_t *out_features_bounds )
{
    TRACE_BEGIN();
    assert( NULL != font_layout );
    assert( NULL != out_features_bounds );

    double left = 0.0;
    double top = 0.0;
    double width = 0.0;
    double height = 0.0;

    /* search all contained features */
    uint32_t count_features;
    count_features = pencil_layout_data_get_feature_count ( (*this_).layout_data );
    for ( uint32_t f_idx = 0; f_idx < count_features; f_idx ++ )
    {
        layout_feature_t *feature_layout;
        feature_layout = pencil_layout_data_get_feature_ptr ( (*this_).layout_data, f_idx );
        const data_feature_t *the_feature;
        the_feature = layout_feature_get_data_ptr ( feature_layout );
        layout_visible_classifier_t *layout_classifier;
        layout_classifier = layout_feature_get_classifier_ptr ( feature_layout );

        if ( diagramelement_id == layout_visible_classifier_get_diagramelement_id( layout_classifier ) )
        {
            geometry_rectangle_t min_feature_bounds;
            geometry_rectangle_init_empty( &min_feature_bounds );
            pencil_feature_painter_get_minimum_bounds ( &((*this_).feature_painter),
                                                        the_feature,
                                                        (*this_).pencil_size,
                                                        font_layout,
                                                        &min_feature_bounds
                                                      );

            double current_w = geometry_rectangle_get_width( &min_feature_bounds );
            width = ( width < current_w ) ? current_w : width;
            height += geometry_rectangle_get_height( &min_feature_bounds );

            geometry_rectangle_destroy( &min_feature_bounds );
        }
    }

    geometry_rectangle_reinit( out_features_bounds, left, top, width, height );
    TRACE_END();
}


/*
Copyright 2017-2018 Andreas Warnke

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
