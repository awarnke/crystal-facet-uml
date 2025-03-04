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
                                                           layout_visible_set_t *layout_data,
                                                           const geometry_anchor_t *anchor,
                                                           const geometry_dimensions_t *preferred_dim,
                                                           draw_feature_label_t *draw_estimator,
                                                           const data_feature_t *feature,
                                                           const data_profile_part_t *profile,
                                                           PangoLayout *font_layout,
                                                           geometry_rectangle_t *out_solution )
{
    U8_TRACE_BEGIN();

    pencil_floating_label_layouter_private_propose_solution( this_,
                                                             layout_data,
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
                                                          layout_visible_set_t *layout_data,
                                                          const geometry_anchor_t *anchor,
                                                          const geometry_dimensions_t *preferred_dim,
                                                          draw_relationship_label_t *draw_estimator,
                                                          const data_relationship_t *relation,
                                                          const data_profile_part_t *profile,
                                                          PangoLayout *font_layout,
                                                          geometry_rectangle_t *out_solution )
{
    U8_TRACE_BEGIN();

    pencil_floating_label_layouter_private_propose_solution( this_,
                                                             layout_data,
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

void pencil_floating_label_layouter_select_solution( pencil_floating_label_layouter_t *this_,
                                                     layout_visible_set_t *layout_data,
                                                     geometry_point_t target_point,
                                                     uint32_t solutions_count,
                                                     const geometry_rectangle_t solutions[],
                                                     uint32_t *out_index_of_best )
{
    U8_TRACE_BEGIN();
    assert( NULL != layout_data );
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
    for ( uint32_t solution_index = 0; solution_index < solutions_count; solution_index ++ )
    {
        /* evalute the debts of this solution */
        double debts_of_current = 0.0;
        const geometry_rectangle_t * const current_solution = &(solutions[solution_index]);

        /* avoid alternating solutions in case their debts are identical */
        debts_of_current += 0.1 * solution_index;

        const layout_quality_t quality = layout_quality_new( (*this_).pencil_size );
        debts_of_current += layout_quality_debts_label_diag( &quality, current_solution, &target_point, diagram_layout );

        /* iterate over all classifiers */
        const uint32_t count_classifiers
            = layout_visible_set_get_visible_classifier_count ( layout_data );
        for ( uint32_t classifier_index = 0; classifier_index < count_classifiers; classifier_index ++ )
        {
            const layout_visible_classifier_t *const probe_classifier
                = layout_visible_set_get_visible_classifier_ptr( layout_data, classifier_index );

            debts_of_current += layout_quality_debts_label_class( &quality, current_solution, probe_classifier );
        }

        /* iterate over all features */
        const uint32_t count_features
            = layout_visible_set_get_feature_count ( layout_data );
        for ( uint32_t feature_index = 0; feature_index < count_features; feature_index ++ )
        {
            const layout_feature_t *const probe_feature
                = layout_visible_set_get_feature_ptr( layout_data, feature_index );

            debts_of_current += layout_quality_debts_label_feat( &quality, current_solution, probe_feature );
        }

        /* iterate over all relationships */
        const uint32_t count_relationships
            = layout_visible_set_get_relationship_count ( layout_data );
        for ( uint32_t relationship_index = 0; relationship_index < count_relationships; relationship_index ++ )
        {
            /* add debts if intersects */
            const layout_relationship_t *const probe_relationship
                = layout_visible_set_get_relationship_ptr( layout_data, relationship_index );

            debts_of_current += layout_quality_debts_label_rel( &quality, current_solution, probe_relationship );
        }

        /* update best solution */
        if ( debts_of_current < debts_of_best )
        {
            index_of_best = solution_index;
            debts_of_best = debts_of_current;
        }
    }

    /* make all alternatives visible */
#ifndef NDEBUG
#if 0
    static unsigned int random;
    random ++;
    index_of_best = random % 13 % solutions_count;
#endif
#endif

    /* output the best */
    U8_TRACE_INFO_INT_INT("floating_label_layout index, debt:", index_of_best, (int32_t)debts_of_best)
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
