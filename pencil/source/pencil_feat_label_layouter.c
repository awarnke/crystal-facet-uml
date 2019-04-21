/* File: pencil_feat_label_layouter.c; Copyright and License: see below */

#include "pencil_feat_label_layouter.h"
#include "trace.h"
#include "util/string/utf8string.h"

void pencil_feat_label_layouter_init( pencil_feat_label_layouter_t *this_,
                                      pencil_layout_data_t *layout_data,
                                      pencil_size_t *pencil_size
                                    )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;

    TRACE_END();
}

void pencil_feat_label_layouter_destroy( pencil_feat_label_layouter_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_feat_label_layouter_do_layout ( pencil_feat_label_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_FEATURES );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the features by their label-box layouting needs, drop invisible relations */
    pencil_feat_label_layouter_private_propose_processing_order ( this_, &sorted );

    /* layout the features label-boxes */
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* determine pointer to feature */
        uint32_t index;
        index = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        layout_feature_t *current_feature;
        current_feature = pencil_layout_data_get_feature_ptr( (*this_).layout_data, index );

        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 8;
        geometry_rectangle_t solution[8];

        /* propose options */
        pencil_feat_label_layouter_private_propose_solutions ( this_,
                                                               current_feature,
                                                               SOLUTIONS_MAX,
                                                               solution,
                                                               &solutions_count
                                                             );

        /* select best option */
        uint32_t index_of_best;
        if ( 1 == solutions_count )
        {
            index_of_best = 0;
        }
        else
        {
            pencil_feat_label_layouter_private_select_solution ( this_,
                                                                 current_feature,
                                                                 solutions_count,
                                                                 solution,
                                                                 &index_of_best
                                                               );
        }

        /* store best option to (*this_).layout_data */
        geometry_rectangle_t *feature_label_box;
        feature_label_box = layout_feature_get_label_box_ptr( current_feature );
        geometry_rectangle_copy( feature_label_box, &(solution[index_of_best]) );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_feat_label_layouter_private_propose_processing_order ( pencil_feat_label_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert( NULL != out_sorted );

    /* sort the features by their label-box: the less simple, the earlier it shall be processed */
    uint32_t count_features;
    count_features = pencil_layout_data_get_feature_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_features; index ++ )
    {
        const layout_feature_t *current_feature;
        current_feature = pencil_layout_data_get_feature_ptr ( (*this_).layout_data, index );
        const data_feature_t *feature_data = layout_feature_get_data_ptr ( current_feature );
        assert( NULL != feature_data );
        data_feature_type_t current_type = data_feature_get_main_type( feature_data );

        int64_t simpleness = 0;

        /* determine simpleness by length of label */
        simpleness -= utf8string_get_length( data_feature_get_key_ptr( feature_data ) );
        simpleness -= utf8string_get_length( data_feature_get_value_ptr( feature_data ) );

        /* do the properties and operations first, they have a fixed positiion given by the classifier */
        if (( DATA_FEATURE_TYPE_PROPERTY == current_type )||( DATA_FEATURE_TYPE_OPERATION == current_type ))
        {
            simpleness = -100000;
        }

        /* insert relation to sorted array - except for lifelines which have no label */
        if ( DATA_FEATURE_TYPE_LIFELINE != current_type )
        {
            int insert_error;
            insert_error = universal_array_index_sorter_insert( out_sorted, index, simpleness );
            if ( 0 != insert_error )
            {
                TSLOG_WARNING( "not all relationship label-boxes are layouted" );
            }
        }
    }

    TRACE_END();
}

void pencil_feat_label_layouter_private_propose_solutions ( pencil_feat_label_layouter_t *this_,
                                                            layout_feature_t *current_feature,
                                                            uint32_t solutions_max,
                                                            geometry_rectangle_t out_solutions[],
                                                            uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != current_feature );
    assert( NULL != out_solutions );
    assert( NULL != out_solutions_count );

    const data_feature_t *feature_data = layout_feature_get_data_ptr ( current_feature );
    assert( NULL != feature_data );
    data_feature_type_t current_type = data_feature_get_main_type( feature_data );

    if (( DATA_FEATURE_TYPE_PROPERTY == current_type )||( DATA_FEATURE_TYPE_OPERATION == current_type ))
    {
        /* the label-box is identical to the feature bounds */
        assert( solutions_max >= 1 );
        geometry_rectangle_copy( &(out_solutions[0]), layout_feature_get_bounds_ptr ( current_feature ) );
        *out_solutions_count = 1;
    }
    else
    {
        /* dummy box */
        assert( solutions_max >= 1 );
        const geometry_rectangle_t * bounds = layout_feature_get_bounds_ptr ( current_feature );
        geometry_rectangle_init( &(out_solutions[0]),
                                 geometry_rectangle_get_left(bounds) + 20,
                                 geometry_rectangle_get_top(bounds) + 20,
                                 200,
                                 20
                               );
        *out_solutions_count = 1;

    }

    TRACE_END();
}

void pencil_feat_label_layouter_private_select_solution ( pencil_feat_label_layouter_t *this_,
                                                          layout_feature_t *current_feature,
                                                          uint32_t solutions_count,
                                                          const geometry_rectangle_t solutions[],
                                                          uint32_t *out_index_of_best )
{
    TRACE_BEGIN();
    assert( NULL != current_feature );
    assert( NULL != solutions );
    assert( solutions_count >= 1 );
    assert( NULL != out_index_of_best );

    static unsigned int random;
    random ++;
    *out_index_of_best = random % solutions_count;

    TRACE_END();
}


/*
 * Copyright 2019-2019 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
