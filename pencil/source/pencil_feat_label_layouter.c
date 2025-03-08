/* File: pencil_feat_label_layouter.c; Copyright and License: see below */

#include "pencil_feat_label_layouter.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8string.h"

void pencil_feat_label_layouter_init( pencil_feat_label_layouter_t *this_,
                                      layout_visible_set_t *layout_data,
                                      const data_profile_part_t *profile,
                                      const pencil_size_t *pencil_size )
{
    U8_TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != profile );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).profile = profile;
    (*this_).pencil_size = pencil_size;
    draw_feature_label_init( &((*this_).draw_feature_label) );
    pencil_floating_label_layouter_init ( &((*this_).label_layout_helper), pencil_size );

    U8_TRACE_END();
}

void pencil_feat_label_layouter_reinit( pencil_feat_label_layouter_t *this_,
                                        layout_visible_set_t *layout_data,
                                        const data_profile_part_t *profile,
                                        const pencil_size_t *pencil_size )
{
    U8_TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != profile );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).profile = profile;
    (*this_).pencil_size = pencil_size;

    U8_TRACE_END();
}

void pencil_feat_label_layouter_destroy( pencil_feat_label_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_floating_label_layouter_destroy ( &((*this_).label_layout_helper) );
    draw_feature_label_destroy( &((*this_).draw_feature_label) );

    U8_TRACE_END();
}

void pencil_feat_label_layouter_do_layout ( pencil_feat_label_layouter_t *this_, PangoLayout *font_layout )
{
    U8_TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) LAYOUT_VISIBLE_SET_MAX_FEATURES );
    assert( NULL != font_layout );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the features by their label-box layouting needs, drop invisible relations */
    pencil_feat_label_layouter_private_propose_processing_order ( this_, &sorted );

    /* layout the features label-boxes */
    const uint32_t count_sorted
        = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* determine pointer to feature */
        const uint32_t index
            = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        layout_feature_t *current_feature;
        current_feature = layout_visible_set_get_feature_ptr( (*this_).layout_data, index );
        geometry_point_t feature_middle = layout_feature_get_symbol_middle ( current_feature );

        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 8;
        geometry_rectangle_t solution[8];

        /* propose options */
        pencil_feat_label_layouter_private_propose_solutions( this_,
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
            pencil_floating_label_layouter_select_solution( &((*this_).label_layout_helper),
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

    U8_TRACE_END();
}

void pencil_feat_label_layouter_private_propose_processing_order ( pencil_feat_label_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_sorted );

    /* sort the features by their label-box: the less simple, the earlier it shall be processed */
    const uint32_t count_features
        = layout_visible_set_get_feature_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_features; index ++ )
    {
        const layout_feature_t *const current_feature
            = layout_visible_set_get_feature_ptr ( (*this_).layout_data, index );
        const data_feature_t *const feature_data = layout_feature_get_data_const ( current_feature );
        assert( NULL != feature_data );
        const data_feature_type_t current_type = data_feature_get_main_type( feature_data );

        int64_t simpleness = 0;

        /* determine simpleness by length of label */
        simpleness -= utf8string_get_length( data_feature_get_key_const( feature_data ) );
        simpleness -= utf8string_get_length( data_feature_get_value_const( feature_data ) );

        /* do the properties and operations first, they have a fixed positiion given by the classifier */
        if ( ! data_feature_type_outside_compartment( current_type ) )
        {
            simpleness = -100000;
        }

        /* insert relation to sorted array - except for lifelines which have no label */
        if ( DATA_FEATURE_TYPE_LIFELINE != current_type )
        {
            const int insert_error
                = universal_array_index_sorter_insert( out_sorted, index, simpleness );
            if ( 0 != insert_error )
            {
                U8_LOG_WARNING( "not all relationship label-boxes are layouted" );
            }
        }
    }

    U8_TRACE_END();
}

void pencil_feat_label_layouter_private_propose_solutions ( pencil_feat_label_layouter_t *this_,
                                                            layout_feature_t *current_feature,
                                                            PangoLayout *font_layout,
                                                            uint32_t solutions_max,
                                                            geometry_rectangle_t out_solutions[],
                                                            uint32_t *out_solutions_count )
{
    U8_TRACE_BEGIN();
    assert( NULL != current_feature );
    assert( NULL != font_layout );
    assert( NULL != out_solutions );
    assert( NULL != out_solutions_count );

    const data_feature_t *feature_data = layout_feature_get_data_const ( current_feature );
    assert( NULL != feature_data );
    const data_feature_type_t current_type = data_feature_get_main_type( feature_data );

    if ( data_feature_type_inside_compartment( current_type ) )
    {
        /* the label-box is already calculated by pencil_feature_layouter_do_layout() */
        /* which is called before pencil_feat_label_layouter_do_layout() */
        assert( solutions_max >= 1 );
        geometry_rectangle_copy( &(out_solutions[0]), layout_feature_get_label_box_const ( current_feature ) );
        *out_solutions_count = 1;
    }
    else
    {
        /* key and value dimensions */
        const geometry_dimensions_t label_dim_proposal = {
            .width = 25.0 * pencil_size_get_standard_font_size( (*this_).pencil_size ),
            .height = pencil_size_get_standard_font_size( (*this_).pencil_size )
        };
        geometry_dimensions_t preferred_label_dim;
        draw_feature_label_get_key_and_value_dimensions( &((*this_).draw_feature_label),
                                                         feature_data,
                                                         (*this_).profile,
                                                         &label_dim_proposal,
                                                         (*this_).pencil_size,
                                                         font_layout,
                                                         &preferred_label_dim
                                                       );

        const double small_gap = 0.000000001;
        const geometry_rectangle_t *const  bounds = layout_feature_get_symbol_box_const ( current_feature );
        const double left = geometry_rectangle_get_left( bounds );
        const double top = geometry_rectangle_get_top( bounds );
        const double bottom = geometry_rectangle_get_bottom( bounds );
        const double right = geometry_rectangle_get_right( bounds );

        assert( solutions_max >= 8 );
        /* top-left */
        {
            geometry_anchor_t anchor_0;
            geometry_anchor_t anchor_1;

            geometry_anchor_init( &anchor_0,
                                  left - small_gap,
                                  top - small_gap,
                                  GEOMETRY_H_ALIGN_RIGHT,  /* the reference point is the right side of the label */
                                  GEOMETRY_V_ALIGN_CENTER
                                );
            geometry_anchor_init( &anchor_1,
                                  left - small_gap,
                                  top - small_gap,
                                  GEOMETRY_H_ALIGN_CENTER,
                                  GEOMETRY_V_ALIGN_BOTTOM  /* the reference point is the bottom side of the label */
                                );

            pencil_floating_label_layouter_propose_solution_feat( &((*this_).label_layout_helper),
                                                                  (*this_).layout_data,
                                                                  &anchor_0,
                                                                  &preferred_label_dim,
                                                                  &((*this_).draw_feature_label),
                                                                  feature_data,
                                                                  (*this_).profile,
                                                                  font_layout,
                                                                  &(out_solutions[0])
                                                                );
            pencil_floating_label_layouter_propose_solution_feat( &((*this_).label_layout_helper),
                                                                  (*this_).layout_data,
                                                                  &anchor_1,
                                                                  &preferred_label_dim,
                                                                  &((*this_).draw_feature_label),
                                                                  feature_data,
                                                                  (*this_).profile,
                                                                  font_layout,
                                                                  &(out_solutions[1])
                                                                );
        }

        /* top-right */
        {
            geometry_anchor_t anchor_2;
            geometry_anchor_t anchor_3;

            geometry_anchor_init( &anchor_2,
                                  right + small_gap,
                                  top - small_gap,
                                  GEOMETRY_H_ALIGN_LEFT,  /* the reference point is the left side of the label */
                                  GEOMETRY_V_ALIGN_CENTER
                                );
            geometry_anchor_init( &anchor_3,
                                  right + small_gap,
                                  top - small_gap,
                                  GEOMETRY_H_ALIGN_CENTER,
                                  GEOMETRY_V_ALIGN_BOTTOM  /* the reference point is the bottom side of the label */
                                );

            pencil_floating_label_layouter_propose_solution_feat( &((*this_).label_layout_helper),
                                                                  (*this_).layout_data,
                                                                  &anchor_2,
                                                                  &preferred_label_dim,
                                                                  &((*this_).draw_feature_label),
                                                                  feature_data,
                                                                  (*this_).profile,
                                                                  font_layout,
                                                                  &(out_solutions[2])
                                                                );
            pencil_floating_label_layouter_propose_solution_feat( &((*this_).label_layout_helper),
                                                                  (*this_).layout_data,
                                                                  &anchor_3,
                                                                  &preferred_label_dim,
                                                                  &((*this_).draw_feature_label),
                                                                  feature_data,
                                                                  (*this_).profile,
                                                                  font_layout,
                                                                  &(out_solutions[3])
                                                                );
        }

        /* bottom-left */
        {
            geometry_anchor_t anchor_4;
            geometry_anchor_t anchor_5;

            geometry_anchor_init( &anchor_4,
                                  left - small_gap,
                                  bottom + small_gap,
                                  GEOMETRY_H_ALIGN_RIGHT,  /* the reference point is the right side of the label */
                                  GEOMETRY_V_ALIGN_CENTER
                                );
            geometry_anchor_init( &anchor_5,
                                  left - small_gap,
                                  bottom + small_gap,
                                  GEOMETRY_H_ALIGN_CENTER,
                                  GEOMETRY_V_ALIGN_TOP  /* the reference point is the top side of the label */
                                );

            pencil_floating_label_layouter_propose_solution_feat( &((*this_).label_layout_helper),
                                                                  (*this_).layout_data,
                                                                  &anchor_4,
                                                                  &preferred_label_dim,
                                                                  &((*this_).draw_feature_label),
                                                                  feature_data,
                                                                  (*this_).profile,
                                                                  font_layout,
                                                                  &(out_solutions[4])
                                                                );
            pencil_floating_label_layouter_propose_solution_feat( &((*this_).label_layout_helper),
                                                                  (*this_).layout_data,
                                                                  &anchor_5,
                                                                  &preferred_label_dim,
                                                                  &((*this_).draw_feature_label),
                                                                  feature_data,
                                                                  (*this_).profile,
                                                                  font_layout,
                                                                  &(out_solutions[5])
                                                                );
        }

        /* bottom-right */
        {
            geometry_anchor_t anchor_6;
            geometry_anchor_t anchor_7;

            geometry_anchor_init( &anchor_6,
                                  right + small_gap,
                                  bottom + small_gap,
                                  GEOMETRY_H_ALIGN_LEFT,  /* the reference point is the left side of the label */
                                  GEOMETRY_V_ALIGN_CENTER
                                );
            geometry_anchor_init( &anchor_7,
                                  right + small_gap,
                                  bottom + small_gap,
                                  GEOMETRY_H_ALIGN_CENTER,
                                  GEOMETRY_V_ALIGN_TOP  /* the reference point is the top side of the label */
                                );

            pencil_floating_label_layouter_propose_solution_feat( &((*this_).label_layout_helper),
                                                                  (*this_).layout_data,
                                                                  &anchor_6,
                                                                  &preferred_label_dim,
                                                                  &((*this_).draw_feature_label),
                                                                  feature_data,
                                                                  (*this_).profile,
                                                                  font_layout,
                                                                  &(out_solutions[6])
                                                                );
            pencil_floating_label_layouter_propose_solution_feat( &((*this_).label_layout_helper),
                                                                  (*this_).layout_data,
                                                                  &anchor_7,
                                                                  &preferred_label_dim,
                                                                  &((*this_).draw_feature_label),
                                                                  feature_data,
                                                                  (*this_).profile,
                                                                  font_layout,
                                                                  &(out_solutions[7])
                                                                );
        }

        *out_solutions_count = 8;
    }

    U8_TRACE_END();
}


/*
 * Copyright 2019-2025 Andreas Warnke
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
