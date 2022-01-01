/* File: pencil_classifier_2d_layouter.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

/* ================================ INITIAL LAYOUT ================================ */

/* ================================ MOVE TO AVOID OVERLAPS ================================ */

/* ================================ EMBRACE CHILDREN STEP BY STEP ================================ */

/* ================================ EMBRACE CHILDREN COMMON ================================ */

/* ================================ EMBRACE AND MOVE CHILDREN TOGETHER ================================ */

static inline geometry_rectangle_t pencil_classifier_2d_layouter_private_calc_descendant_envelope( pencil_classifier_2d_layouter_t *this_,
                                                                                                   const layout_visible_classifier_t *ancestor_classifier )
{
    assert( ancestor_classifier != NULL );
    geometry_rectangle_t descendant_envelope;
    bool descendant_envelope_initialized = false;

    const uint32_t count_classifiers = pencil_layout_data_get_visible_classifier_count( (*this_).layout_data );
    for ( uint32_t classifier_search_idx = 0; classifier_search_idx < count_classifiers; classifier_search_idx ++ )
    {
        const layout_visible_classifier_t *const probe_classifier
            = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, classifier_search_idx );
        if ( pencil_layout_data_is_ancestor( (*this_).layout_data, ancestor_classifier, probe_classifier ) )
        {
            const geometry_rectangle_t probe_envelope = layout_visible_classifier_get_envelope_box( probe_classifier );
            if ( ! descendant_envelope_initialized )
            {
                geometry_rectangle_copy( &descendant_envelope, &probe_envelope );
                descendant_envelope_initialized = true;
            }
            else
            {
                geometry_rectangle_init_by_bounds ( &descendant_envelope, &descendant_envelope, &probe_envelope );
            }
        }
    }
    if ( ! descendant_envelope_initialized )
    {
        descendant_envelope = layout_visible_classifier_get_envelope_box( ancestor_classifier );
    }

    return descendant_envelope;
}

static inline geometry_rectangle_t pencil_classifier_2d_layouter_private_calc_outer_space( pencil_classifier_2d_layouter_t *this_,
                                                                                           const geometry_rectangle_t *start_rect,
                                                                                           const layout_visible_classifier_t *the_classifier )
{
    assert( start_rect != NULL );
    assert( the_classifier != NULL );

    /* fetch data on parent classifier */
    const geometry_rectangle_t parent_envelope = layout_visible_classifier_get_envelope_box( the_classifier );
    const geometry_rectangle_t *const parent_space = layout_visible_classifier_get_space_const( the_classifier );
    const double parent_space_width_diff
        = geometry_rectangle_get_width(&parent_envelope) - geometry_rectangle_get_width(parent_space);
    const double parent_space_height_diff
        = geometry_rectangle_get_height(&parent_envelope) - geometry_rectangle_get_height(parent_space);

    /* determine outer space around start_rect rectangle */
    geometry_rectangle_t outer_space;
    {
        geometry_rectangle_copy( &outer_space, start_rect );
        const double children_envelope_w = geometry_rectangle_get_width(start_rect);
        const double children_envelope_h = geometry_rectangle_get_height(start_rect);
        geometry_rectangle_shift ( &outer_space,
                                   -0.5*children_envelope_w-parent_space_width_diff,
                                   -0.5*children_envelope_h-parent_space_height_diff
                                 );
        geometry_rectangle_enlarge ( &outer_space,
                                    children_envelope_w+2.0*parent_space_width_diff,
                                    children_envelope_h+2.0*parent_space_height_diff
                                  );

        geometry_rectangle_init_by_intersect( &outer_space, &outer_space, (*this_).diagram_draw_area );

        const uint32_t count_classifiers = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
        for ( uint32_t probe_index = 0; probe_index < count_classifiers; probe_index ++ )
        {
            /* get classifier to check overlaps */
            const layout_visible_classifier_t *const the_probe
                = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, probe_index );

            const bool ignore
                = pencil_layout_data_is_ancestor ( (*this_).layout_data, the_classifier, the_probe )
                || pencil_layout_data_is_ancestor ( (*this_).layout_data, the_probe, the_classifier ) /* ancestor may already encapsulate probe */
                || ( layout_visible_classifier_is_equal_diagramelement_id( the_classifier, the_probe ));
            if ( ! ignore )
            {
                const geometry_rectangle_t probe_envelope = layout_visible_classifier_get_envelope_box( the_probe );
                geometry_rectangle_init_by_difference( &outer_space, &outer_space, &probe_envelope );
            }
        }
    }

    return outer_space;
}

static inline void pencil_classifier_2d_layouter_private_move_descendants( pencil_classifier_2d_layouter_t *this_,
                                                                           const layout_visible_classifier_t *ancestor_classifier,
                                                                           double delta_x,
                                                                           double delta_y )
{
    assert( ancestor_classifier != NULL );

    /* check all classifiers */
    const uint32_t count_classifiers = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_classifiers; index ++ )
    {
        layout_visible_classifier_t *const probe_classifier = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, index );
        const bool is_descendant = pencil_layout_data_is_ancestor ( (*this_).layout_data, ancestor_classifier, probe_classifier );
        if ( is_descendant )
        {
            layout_visible_classifier_shift ( probe_classifier, delta_x, delta_y );
        }
    }
}


/*
Copyright 2018-2022 Andreas Warnke

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
