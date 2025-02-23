/* File: pencil_floating_label_layouter.c; Copyright and License: see below */

#include "pencil_floating_label_layouter.h"
#include "layout/layout_quality.h"
#include "u8/u8_trace.h"
#include "utf8stringbuf/utf8string.h"

void pencil_floating_label_layouter_init( pencil_floating_label_layouter_t *this_, const pencil_size_t *pencil_size )
{
    U8_TRACE_BEGIN();

    (*this_).pencil_size = pencil_size;
    U8_TRACE_END();
}

void pencil_floating_label_layouter_destroy( pencil_floating_label_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_END();
}

void pencil_floating_label_layouter_propose_solution_feat( const pencil_floating_label_layouter_t *this_,
                                                           const geometry_anchor_t *anchor,
                                                           const geometry_dimensions_t *preferred_dim,
                                                           const draw_feature_label_t *draw_estimator,
                                                           const layout_feature_t *feature,
                                                           const data_profile_part_t *profile,
                                                           PangoLayout *font_layout,
                                                           geometry_rectangle_t *out_solution )
{
    U8_TRACE_BEGIN();

    pencil_floating_label_layouter_private_propose_solution( this_,
                                                             anchor,
                                                             preferred_dim,
                                                             draw_estimator,
                                                             feature,
                                                             NULL,
                                                             NULL,
                                                             profile,
                                                             font_layout,
                                                             out_solution
                                                           );

    U8_TRACE_END();
}

void pencil_floating_label_layouter_propose_solution_rel( const pencil_floating_label_layouter_t *this_,
                                                          const geometry_anchor_t *anchor,
                                                          const geometry_dimensions_t *preferred_dim,
                                                          const draw_relationship_label_t *draw_estimator,
                                                          const layout_relationship_t *relation,
                                                          const data_profile_part_t *profile,
                                                          PangoLayout *font_layout,
                                                          geometry_rectangle_t *out_solution )
{
    U8_TRACE_BEGIN();

    pencil_floating_label_layouter_private_propose_solution( this_,
                                                             anchor,
                                                             preferred_dim,
                                                             NULL,
                                                             NULL,
                                                             draw_estimator,
                                                             relation,
                                                             profile,
                                                             font_layout,
                                                             out_solution
                                                           );

    U8_TRACE_END();
}

void pencil_floating_label_layouter_private_propose_solution( const pencil_floating_label_layouter_t *this_,
                                                              const geometry_anchor_t *anchor,
                                                              const geometry_dimensions_t *preferred_dim,
                                                              const draw_feature_label_t *draw_estimator_feat,
                                                              const layout_feature_t *feature,
                                                              const draw_relationship_label_t *draw_estimator_rel,
                                                              const layout_relationship_t *relation,
                                                              const data_profile_part_t *profile,
                                                              PangoLayout *font_layout,
                                                              geometry_rectangle_t *out_solution )
{
    U8_TRACE_BEGIN();

    U8_TRACE_END();
}

void pencil_floating_label_layouter_select_solution( pencil_floating_label_layouter_t *this_,
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

        const layout_quality_t quality = layout_quality_new( (*this_).pencil_size );
        debts_of_current += layout_quality_debts_label_diag( &quality, current_solution, &target_point, diagram_layout );

        /* iterate over all classifiers */
        const uint32_t count_clasfy
            = layout_visible_set_get_visible_classifier_count ( layout_data );
        for ( uint32_t clasfy_index = 0; clasfy_index < count_clasfy; clasfy_index ++ )
        {
            const layout_visible_classifier_t *const probe_classifier
                = layout_visible_set_get_visible_classifier_ptr( layout_data, clasfy_index );

            debts_of_current += layout_quality_debts_label_class( &quality, current_solution, probe_classifier );
        }

        /* iterate over all features */
        const uint32_t count_feat
            = layout_visible_set_get_feature_count ( layout_data );
        for ( uint32_t feat_index = 0; feat_index < count_feat; feat_index ++ )
        {
            const layout_feature_t *const probe_feature
                = layout_visible_set_get_feature_ptr( layout_data, feat_index );

            debts_of_current += layout_quality_debts_label_feat( &quality, current_solution, probe_feature );
        }

        /* iterate over all relationships */
        const uint32_t count_relations
            = layout_visible_set_get_relationship_count ( layout_data );
        for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
        {
            /* add debts if intersects */
            const layout_relationship_t *const probe_relationship
                = layout_visible_set_get_relationship_ptr( layout_data, rel_index );

            debts_of_current += layout_quality_debts_label_rel( &quality, current_solution, probe_relationship );
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
