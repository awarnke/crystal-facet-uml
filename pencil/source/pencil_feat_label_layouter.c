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
    draw_feature_label_init( &((*this_).draw_feature_label) );
    pencil_label_layout_helper_init ( &((*this_).label_layout_helper) );

    TRACE_END();
}

void pencil_feat_label_layouter_destroy( pencil_feat_label_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_label_layout_helper_destroy ( &((*this_).label_layout_helper) );
    draw_feature_label_destroy( &((*this_).draw_feature_label) );

    TRACE_END();
}

void pencil_feat_label_layouter_do_layout ( pencil_feat_label_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_FEATURES );
    assert( NULL != font_layout );

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
        geometry_point_t feature_middle = layout_feature_get_symbol_middle ( current_feature );

        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 8;
        geometry_rectangle_t solution[8];

        /* propose options */
        pencil_feat_label_layouter_private_propose_solutions ( this_,
                                                               current_feature,
                                                               font_layout,
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
            pencil_label_layout_helper_select_solution ( &((*this_).label_layout_helper),
                                                         (*this_).layout_data,
                                                         feature_middle,
                                                         solutions_count,
                                                         solution,
                                                         &index_of_best
                                                       );
        }

        /* store best option to (*this_).layout_data */
        layout_feature_set_label_box( current_feature, &(solution[index_of_best]) );
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
        const data_feature_t *feature_data = layout_feature_get_data_const ( current_feature );
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
                                                            PangoLayout *font_layout,
                                                            uint32_t solutions_max,
                                                            geometry_rectangle_t out_solutions[],
                                                            uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != current_feature );
    assert( NULL != font_layout );
    assert( NULL != out_solutions );
    assert( NULL != out_solutions_count );

    const data_feature_t *feature_data = layout_feature_get_data_const ( current_feature );
    assert( NULL != feature_data );
    data_feature_type_t current_type = data_feature_get_main_type( feature_data );

    if (( DATA_FEATURE_TYPE_PROPERTY == current_type )||( DATA_FEATURE_TYPE_OPERATION == current_type ))
    {
        /* the label-box is identical to the feature bounds */
        assert( solutions_max >= 1 );
        geometry_rectangle_copy( &(out_solutions[0]), layout_feature_get_symbol_box_const ( current_feature ) );
        *out_solutions_count = 1;
    }
    else
    {
        /* key and value dimensions */
        double text_width;
        double text_height;
        draw_feature_label_get_key_and_value_dimensions( &((*this_).draw_feature_label),
                                                         feature_data,
                                                         (*this_).pencil_size,
                                                         font_layout,
                                                         &text_width,
                                                         &text_height
                                                       );
        const double half_width = text_width/2.0;
        const double half_height = text_height/2.0;

        const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size );
        const geometry_rectangle_t *const  bounds = layout_feature_get_symbol_box_const ( current_feature );
        const double left = geometry_rectangle_get_left( bounds );
        const double top = geometry_rectangle_get_top( bounds );
        const double center_x = geometry_rectangle_get_center_x( bounds );
        const double center_y = geometry_rectangle_get_center_y( bounds );
        const double bottom = geometry_rectangle_get_bottom( bounds );
        const double right = geometry_rectangle_get_right( bounds );

        assert( solutions_max >= 8 );
        /* top */
        geometry_rectangle_init( &(out_solutions[0]),
                                 center_x - half_width,
                                 top - text_height - gap,
                                 text_width,
                                 text_height
                               );
        /* bottom */
        geometry_rectangle_init( &(out_solutions[1]),
                                 center_x - half_width,
                                 bottom + gap,
                                 text_width,
                                 text_height
                               );
        /* left */
        geometry_rectangle_init( &(out_solutions[2]),
                                 left - text_width - gap,
                                 center_y - half_height,
                                 text_width,
                                 text_height
                               );
        /* right */
        geometry_rectangle_init( &(out_solutions[3]),
                                 right + gap,
                                 center_y - half_height,
                                 text_width,
                                 text_height
                               );
        /* top-left */
        geometry_rectangle_init( &(out_solutions[4]),
                                 left - text_width,
                                 top - text_height,
                                 text_width,
                                 text_height
                               );
        /* top-right */
        geometry_rectangle_init( &(out_solutions[5]),
                                 right,
                                 top - text_height,
                                 text_width,
                                 text_height
                               );
        /* bottom-left */
        geometry_rectangle_init( &(out_solutions[6]),
                                 left - text_width,
                                 bottom,
                                 text_width,
                                 text_height
                               );
        /* bottom-right */
        geometry_rectangle_init( &(out_solutions[7]),
                                 right,
                                 bottom,
                                 text_width,
                                 text_height
                               );
        *out_solutions_count = 8;
    }

    TRACE_END();
}


/*
 * Copyright 2019-2021 Andreas Warnke
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
