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

    /* sort the features by their classifiers */
    uint32_t count_features;
    count_features = pencil_layout_data_get_feature_count ( (*this_).layout_data );
    for ( uint32_t f_idx = 0; f_idx < count_features; f_idx ++ )
    {
        layout_feature_t *feature_layout;
        feature_layout = pencil_layout_data_get_feature_ptr ( (*this_).layout_data, f_idx );
        const data_feature_t *the_feature;
        the_feature = layout_feature_get_data_ptr ( feature_layout );

        int err;
        double weight;
        weight = (((double)data_feature_get_classifier_id( the_feature )) * UINT32_MAX) + data_feature_get_list_order( the_feature );
        err = universal_array_index_sorter_insert( &sorted_features, f_idx, weight );
        if ( 0 != err )
        {
            TSLOG_ERROR ( "universal_array_index_sorter_t list is full." );
        }
    }

    /* layout the features */
    int64_t last_classifier_id = DATA_ID_VOID_ID;
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

        /* determine list position */
        int64_t current_classifier_id = data_feature_get_classifier_id( the_feature );
        if ( current_classifier_id != last_classifier_id )
        {
            y_position_of_next_feature = 0.0;
            last_classifier_id = current_classifier_id;
        }

        /* determine the minimum bounds of the feature */
        geometry_rectangle_t f_min_bounds;
        pencil_feature_painter_get_minimum_bounds ( &((*this_).feature_painter),
                                                    the_feature,
                                                    (*this_).pencil_size,
                                                    font_layout,
                                                    &f_min_bounds
                                                  );

        /* layout feature into parent classifier */
        layout_visible_classifier_t *classifier_layout;
        classifier_layout = layout_feature_get_classifier_ptr ( feature_layout );
        geometry_rectangle_t *c_space = layout_visible_classifier_get_space_ptr ( classifier_layout );
        geometry_rectangle_t f_bounds;
        geometry_rectangle_init ( &f_bounds,
                                  geometry_rectangle_get_left( c_space ),
                                  geometry_rectangle_get_top( c_space ) + y_position_of_next_feature,
                                  geometry_rectangle_get_width( c_space ),
                                  geometry_rectangle_get_height( &f_min_bounds )
                                );
        layout_feature_set_bounds ( feature_layout, &f_bounds );

        /* adjust y position of next feature */
        y_position_of_next_feature += geometry_rectangle_get_height( &f_min_bounds );
    }

    universal_array_index_sorter_destroy( &sorted_features );

    TRACE_END();
}

void pencil_feature_layouter_calculate_features_bounds ( pencil_feature_layouter_t *this_,
                                                         int64_t classifier_id,
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

        if ( data_feature_get_classifier_id( the_feature ) == classifier_id )
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
