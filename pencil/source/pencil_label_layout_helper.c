/* File: pencil_label_layout_helper.c; Copyright and License: see below */

#include "pencil_label_layout_helper.h"
#include "trace.h"
#include "util/string/utf8string.h"

void pencil_label_layout_helper_init( pencil_label_layout_helper_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_label_layout_helper_destroy( pencil_label_layout_helper_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_label_layout_helper_select_solution ( pencil_label_layout_helper_t *this_,
                                                  pencil_layout_data_t *layout_data,
                                                  geometry_point_t target_point,
                                                  uint32_t solutions_count,
                                                  const geometry_rectangle_t solutions[],
                                                  uint32_t *out_index_of_best )
{
    TRACE_BEGIN();
    assert( NULL != solutions );
    assert( solutions_count >= 1 );
    assert( NULL != out_index_of_best );

    /* get draw area */
    geometry_rectangle_t *diagram_draw_area;
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = pencil_layout_data_get_diagram_ptr( layout_data );
        diagram_draw_area = layout_diagram_get_draw_area_ptr( diagram_layout );
    }

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
        uint32_t count_clasfy;
        count_clasfy = pencil_layout_data_get_classifier_count ( layout_data );
        for ( uint32_t clasfy_index = 0; clasfy_index < count_clasfy; clasfy_index ++ )
        {
            layout_visible_classifier_t *probe_classifier;
            probe_classifier = pencil_layout_data_get_classifier_ptr( layout_data, clasfy_index );

            geometry_rectangle_t *classifier_bounds;
            classifier_bounds = layout_visible_classifier_get_bounds_ptr( probe_classifier );
            if ( geometry_rectangle_is_intersecting( current_solution, classifier_bounds ) )
            {
                debts_of_current += geometry_rectangle_get_intersect_area( current_solution, classifier_bounds ); /* low debt */
            }

            geometry_rectangle_t *classifier_label_box;
            classifier_label_box = layout_visible_classifier_get_label_box_ptr( probe_classifier );
            if ( geometry_rectangle_is_intersecting( current_solution, classifier_label_box ) )
            {
                debts_of_current += 100.0 * geometry_rectangle_get_intersect_area( current_solution, classifier_label_box ); /* medium debt */
            }
        }

        /* iterate over all features */
        uint32_t count_feat;
        count_feat = pencil_layout_data_get_feature_count ( layout_data );
        for ( uint32_t feat_index = 0; feat_index < count_feat; feat_index ++ )
        {
            layout_feature_t *probe_feature;
            probe_feature = pencil_layout_data_get_feature_ptr( layout_data, feat_index );

            geometry_rectangle_t *feature_bounds;
            feature_bounds = layout_feature_get_bounds_ptr( probe_feature );
            if ( geometry_rectangle_is_intersecting( current_solution, feature_bounds ) )
            {
                debts_of_current += 100.0 * geometry_rectangle_get_intersect_area( current_solution, feature_bounds ); /* medium debt */
            }

            geometry_rectangle_t *feature_label_box;
            feature_label_box = layout_feature_get_label_box_ptr( probe_feature );
            if ( geometry_rectangle_is_intersecting( current_solution, feature_label_box ) )
            {
                debts_of_current += 100.0 * geometry_rectangle_get_intersect_area( current_solution, feature_label_box ); /* medium debt */
            }
        }

        /* iterate over all relationships */
        uint32_t count_relations;
        count_relations = pencil_layout_data_get_relationship_count ( layout_data );
        for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
        {
            /* add debts if intersects */
            layout_relationship_t *probe_relationship;
            probe_relationship = pencil_layout_data_get_relationship_ptr( layout_data, rel_index );
            if ( PENCIL_VISIBILITY_SHOW == layout_relationship_get_visibility( probe_relationship ) )
            {
                geometry_connector_t *probe_shape;
                probe_shape = layout_relationship_get_shape_ptr( probe_relationship );
                if ( geometry_connector_is_intersecting_rectangle( probe_shape, current_solution ) )
                {
                    debts_of_current += geometry_rectangle_get_area( current_solution );  /* relationship bounds intersects are not so bad ... low debt */
                }

                geometry_rectangle_t *relationship_label_box;
                relationship_label_box = layout_relationship_get_label_box_ptr( probe_relationship );
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
    *out_index_of_best = index_of_best;

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
