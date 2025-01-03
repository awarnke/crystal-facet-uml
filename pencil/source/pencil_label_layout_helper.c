/* File: pencil_label_layout_helper.c; Copyright and License: see below */

#include "pencil_label_layout_helper.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8string.h"

void pencil_label_layout_helper_init( pencil_label_layout_helper_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */

    U8_TRACE_END();
}

void pencil_label_layout_helper_destroy( pencil_label_layout_helper_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_END();
}

void pencil_label_layout_helper_select_solution ( pencil_label_layout_helper_t *this_,
                                                  layout_visible_set_t *layout_data,
                                                  geometry_point_t target_point,
                                                  uint32_t solutions_count,
                                                  const geometry_rectangle_t solutions[],
                                                  uint32_t *out_index_of_best )
{
    U8_TRACE_BEGIN();
    assert( NULL != solutions );
    assert( solutions_count >= 1 );
    assert( NULL != out_index_of_best );

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = layout_visible_set_get_diagram_ptr( layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );

    /* define potential solution and rating */
    uint32_t index_of_best = 0;
    double debts_of_best = DBL_MAX;

    /* evaluate the solutions by their overlaps with classifiers */
    for ( uint32_t solution_idx = 0; solution_idx < solutions_count; solution_idx ++ )
    {
        /* evalute the debts of this solution */
        double debts_of_current = 0.0;
        const geometry_rectangle_t * const current_solution = &(solutions[solution_idx]);

        /* avoid alternating solutions in case their debts are identical */
        debts_of_current += 0.1 * solution_idx;

        /* check distance to target point */
        const geometry_point_t solution_middle = geometry_rectangle_get_center( current_solution );
        debts_of_current += geometry_point_calc_chess_distance ( &target_point, &solution_middle );

        /* add debts for overlap to diagram boundary */
        if ( ! geometry_rectangle_is_containing( diagram_draw_area, current_solution ) )
        {
            debts_of_current += 100.0 * geometry_rectangle_get_area(diagram_draw_area); /* high debt */
        }

        /* iterate over all classifiers */
        const uint32_t count_clasfy
            = layout_visible_set_get_visible_classifier_count ( layout_data );
        for ( uint32_t clasfy_index = 0; clasfy_index < count_clasfy; clasfy_index ++ )
        {
            const layout_visible_classifier_t *const probe_classifier
                = layout_visible_set_get_visible_classifier_ptr( layout_data, clasfy_index );

            const geometry_rectangle_t *const classifier_symbol_box
                = layout_visible_classifier_get_symbol_box_const( probe_classifier );
            if ( geometry_rectangle_is_intersecting( current_solution, classifier_symbol_box ) )
            {
                /* overlaps to the symbol box are bad only if not contained in space area */
                const geometry_rectangle_t *const classifier_space
                    = layout_visible_classifier_get_space_const( probe_classifier );
                if ( ! geometry_rectangle_is_containing( classifier_space, current_solution ) )
                {
                    debts_of_current += geometry_rectangle_get_intersect_area( current_solution, classifier_symbol_box ); /* low debt */
                }
            }

            const geometry_rectangle_t *const classifier_label_box
                = layout_visible_classifier_get_label_box_const( probe_classifier );
            if ( geometry_rectangle_is_intersecting( current_solution, classifier_label_box ) )
            {
                debts_of_current += 100.0 * geometry_rectangle_get_intersect_area( current_solution, classifier_label_box ); /* medium debt */
            }
        }

        /* iterate over all features */
        const uint32_t count_feat
            = layout_visible_set_get_feature_count ( layout_data );
        for ( uint32_t feat_index = 0; feat_index < count_feat; feat_index ++ )
        {
            const layout_feature_t *const probe_feature
                = layout_visible_set_get_feature_ptr( layout_data, feat_index );
            const data_feature_t *const probe_f_data
                = layout_feature_get_data_const( probe_feature );

            const geometry_rectangle_t *const feature_symbol_box
                = layout_feature_get_symbol_box_const( probe_feature );
            if ( geometry_rectangle_is_intersecting( current_solution, feature_symbol_box ) )
            {
                if ( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( probe_f_data ) )
                {
                    debts_of_current += geometry_rectangle_get_intersect_area( current_solution, feature_symbol_box ); /* low debt */
                }
                else
                {
                    debts_of_current += 100.0 * geometry_rectangle_get_intersect_area( current_solution, feature_symbol_box ); /* medium debt */
                }
            }

            const geometry_rectangle_t *const feature_label_box
                = layout_feature_get_label_box_const( probe_feature );
            if ( geometry_rectangle_is_intersecting( current_solution, feature_label_box ) )
            {
                debts_of_current += 100.0 * geometry_rectangle_get_intersect_area( current_solution, feature_label_box ); /* medium debt */
            }
        }

        /* iterate over all relationships */
        const uint32_t count_relations
            = layout_visible_set_get_relationship_count ( layout_data );
        for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
        {
            /* add debts if intersects */
            const layout_relationship_t *const probe_relationship
                = layout_visible_set_get_relationship_ptr( layout_data, rel_index );
            if (( PENCIL_VISIBILITY_SHOW == layout_relationship_get_visibility( probe_relationship ) )
                || ( PENCIL_VISIBILITY_GRAY_OUT == layout_relationship_get_visibility( probe_relationship ) ))
            {
                const geometry_connector_t *const probe_shape
                    = layout_relationship_get_shape_const( probe_relationship );
                if ( geometry_connector_is_intersecting_rectangle( probe_shape, current_solution ) )
                {
                    debts_of_current += geometry_rectangle_get_area( current_solution );  /* relationship bounds intersects are not so bad ... low debt */
                }

                const geometry_rectangle_t *const relationship_label_box
                    = layout_relationship_get_label_box_const( probe_relationship );
                if ( geometry_rectangle_is_intersecting( current_solution, relationship_label_box ) )
                {
                    debts_of_current += 100.0 * geometry_rectangle_get_intersect_area( current_solution, relationship_label_box ); /* medium debt */
                }
            }
        }

        /* update best solution */
        if ( debts_of_current < debts_of_best )
        {
            index_of_best = solution_idx;
            debts_of_best = debts_of_current;
        }
    }

    /*
    static unsigned int random;
    random ++;
    index_of_best = random % solutions_count;
    */

    /* output the best */
    U8_TRACE_INFO_INT_INT("label layout idx, debt:", index_of_best, (int32_t)debts_of_best)
    *out_index_of_best = index_of_best;

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
