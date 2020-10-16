/* File: pencil_classifier_layouter.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline void pencil_classifier_layouter_private_sort_classifiers_by_list_order( const pencil_classifier_layouter_t *this_,
                                                                                      universal_array_index_sorter_t *out_sorted_classifiers )
{
    assert ( ((uint32_t)UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE) >= ((uint32_t)DATA_VISIBLE_SET_MAX_CLASSIFIERS) );

    universal_array_index_sorter_reinit( out_sorted_classifiers );
    const uint32_t count_clasfy = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t c_idx = 0; c_idx < count_clasfy; c_idx ++ )
    {
        const layout_visible_classifier_t *const visible_classifier_probe
            = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, c_idx );

        const data_classifier_t *const classifier_probe
            = layout_visible_classifier_get_classifier_const( visible_classifier_probe );

        int err;
        const double weight = (const double) data_classifier_get_list_order( classifier_probe );
        err = universal_array_index_sorter_insert( out_sorted_classifiers, c_idx, weight );
        if ( 0 != err )
        {
            TSLOG_ERROR ( "universal_array_index_sorter_t list is full." );
        }
    }
}

static inline geometry_rectangle_t pencil_classifier_layouter_private_calc_descendant_envelope( pencil_classifier_layouter_t *this_,
                                                                                                const layout_visible_classifier_t *ancestor_classifier
                                                                                              )
{
    assert( ancestor_classifier != NULL );
    geometry_rectangle_t descendant_envelope;
    bool descendant_envelope_initialized = false;

    const uint32_t count_classifiers = pencil_layout_data_get_visible_classifier_count( (*this_).layout_data );
    for ( uint32_t classifier_search_idx = 0; classifier_search_idx < count_classifiers; classifier_search_idx ++ )
    {
        const layout_visible_classifier_t * probe_classifier
            = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, classifier_search_idx );
        if ( pencil_layout_data_is_ancestor( (*this_).layout_data, ancestor_classifier, probe_classifier ) )
        {
            const geometry_rectangle_t probe_envelope = layout_visible_classifier_calc_envelope_box( probe_classifier );
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
        descendant_envelope = layout_visible_classifier_calc_envelope_box( ancestor_classifier );
    }

    return descendant_envelope;
}

static inline void pencil_classifier_layouter_private_move_descendants( pencil_classifier_layouter_t *this_,
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
Copyright 2018-2020 Andreas Warnke

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
