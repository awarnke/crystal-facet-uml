/* File: pencil_floating_label_layouter.inl; Copyright and License: see below */

#include <assert.h>

static inline void pencil_floating_label_layouter_private_propose_solution( const pencil_floating_label_layouter_t *this_,
                                                                            const geometry_anchor_t *anchor,
                                                                            const geometry_dimensions_t *preferred_dim,
                                                                            draw_feature_label_t *draw_estimator_feat,
                                                                            const data_feature_t *feature,
                                                                            draw_relationship_label_t *draw_estimator_rel,
                                                                            const data_relationship_t *relation,
                                                                            geometry_rectangle_t *out_solution )
{
    U8_TRACE_BEGIN();
    assert( (*this_).pencil_size != NULL );
    assert( (*this_).layout_data != NULL );
    assert( (*this_).profile != NULL );
    assert( (*this_).font_layout != NULL );
    assert( anchor != NULL );
    assert( preferred_dim != NULL );
    if ( draw_estimator_feat == NULL )
    {
        assert( feature == NULL );
        assert( draw_estimator_rel != NULL );
        assert( relation != NULL );
    }
    else
    {
        assert( feature != NULL );
        assert( draw_estimator_rel == NULL );
        assert( relation == NULL );
    }
    assert( out_solution != NULL );

    /* get draw area */
    const layout_diagram_t *const diagram_layout = layout_visible_set_get_diagram_ptr( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_rect = layout_diagram_get_draw_area_const( diagram_layout );

    /* Start shrinking the available solution space to anchor-alignment and diagram space. */
    /* To not accidentially shrink to wrong width/height by taking into account far-away elements, */
    /* start with a smaller rectagle of 3..5 * preferred_dim: This allows for moving by +/- 0.5 and re-sizing by factor 2..4 */
    geometry_dimensions_t search_area;  /* limit the search area to avoid that far-away elements limit width or height */
    geometry_dimensions_init( &search_area, 3.0 * geometry_dimensions_get_width( preferred_dim ), 5.0 * geometry_dimensions_get_height( preferred_dim ) );
    geometry_rectangle_t available = geometry_anchor_align_dim( anchor, &search_area );
    geometry_rectangle_init_by_intersect( &available, &available, diagram_draw_rect );

    /* iterate over all classifiers */
    const uint32_t count_classifiers = layout_visible_set_get_visible_classifier_count( (*this_).layout_data );
    for ( uint32_t classifier_index = 0; classifier_index < count_classifiers; classifier_index ++ )
    {
        const layout_visible_classifier_t *const probe_classifier
            = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, classifier_index );
        const geometry_rectangle_t *const probe_space = layout_visible_classifier_get_space_const( probe_classifier );
        if ( geometry_rectangle_contains_point( probe_space, geometry_anchor_get_point_const( anchor ) ) )
        {
            /* reference point is inside the space of probe_classifier */
            /* shrink the available solution space to the classifier space */
            geometry_rectangle_init_by_intersect( &available, &available, probe_space );
        }
        else
        {
            geometry_rectangle_init_by_difference_at_pivot( &available,
                                                            &available,
                                                            layout_visible_classifier_get_symbol_box_const( probe_classifier ),
                                                            geometry_anchor_get_point_const( anchor )
                                                          );
            geometry_rectangle_init_by_difference_at_pivot( &available,
                                                            &available,
                                                            layout_visible_classifier_get_label_box_const( probe_classifier ),
                                                            geometry_anchor_get_point_const( anchor )
                                                          );
            /*
            icon_box is covered by symbol_box
            geometry_rectangle_init_by_difference_at_pivot( &available,
                                                            &available,
                                                            layout_visible_classifier_get_icon_box_const( probe_classifier ),
                                                            geometry_anchor_get_point_const( anchor )
                                                          );
            */
        }
    }

    /* iterate over all features */
    const uint32_t count_features = layout_visible_set_get_feature_count( (*this_).layout_data );
    for ( uint32_t feature_index = 0; feature_index < count_features; feature_index ++ )
    {
        const layout_feature_t *const probe_feature
            = layout_visible_set_get_feature_ptr( (*this_).layout_data, feature_index );
        const data_feature_t *const probe_feature_data = layout_feature_get_data_const( probe_feature );
        if ( DATA_FEATURE_TYPE_LIFELINE != data_feature_get_main_type( probe_feature_data ) )
        {
            geometry_rectangle_init_by_difference_at_pivot( &available,
                                                            &available,
                                                            layout_feature_get_symbol_box_const( probe_feature ),
                                                            geometry_anchor_get_point_const( anchor )
                                                          );
            /* note: the label_box may not yet have been initialized in all cases */
        }
        else
        {
            /* ignore lifelines - otherwise the labels are shrinked to fit between lielines which looks ugly */
        }
    }

    /* iterate over all relationships */
    const uint32_t count_relationships = layout_visible_set_get_relationship_count( (*this_).layout_data );
    for ( uint32_t relationship_index = 0; relationship_index < count_relationships; relationship_index ++ )
    {
        const layout_relationship_t *const probe_relationship
            = layout_visible_set_get_relationship_ptr( (*this_).layout_data, relationship_index );
        const geometry_connector_t *const conn = layout_relationship_get_shape_const( probe_relationship );
        const geometry_rectangle_t source = geometry_connector_get_segment_bounds( conn, GEOMETRY_CONNECTOR_SEGMENT_SOURCE );
        const geometry_rectangle_t middle = geometry_connector_get_segment_bounds( conn, GEOMETRY_CONNECTOR_SEGMENT_MAIN );
        const geometry_rectangle_t dest = geometry_connector_get_segment_bounds( conn, GEOMETRY_CONNECTOR_SEGMENT_DESTINATION );
        geometry_rectangle_init_by_difference_at_pivot( &available,
                                                        &available,
                                                        &source,
                                                        geometry_anchor_get_point_const( anchor )
                                                      );
        geometry_rectangle_init_by_difference_at_pivot( &available,
                                                        &available,
                                                        &middle,
                                                        geometry_anchor_get_point_const( anchor )
                                                      );
        geometry_rectangle_init_by_difference_at_pivot( &available,
                                                        &available,
                                                        &dest,
                                                        geometry_anchor_get_point_const( anchor )
                                                      );
        /* note: the label_box may not yet have been initialized in all cases */
    }

    /* if the available solution space has a positive width, layout the label */
    const double available_width = geometry_rectangle_get_width( &available );
    const geometry_dimensions_t available_dim = geometry_rectangle_get_dimensions( &available );
    bool preferred_fits = geometry_dimensions_can_contain( &available_dim, preferred_dim )
        || ( geometry_dimensions_get_width( preferred_dim ) < 0.000000001 );
    if ( preferred_fits )
    {
        /* no need to re-calculate, just shift */
        *out_solution = geometry_anchor_align_biased_dim( anchor, preferred_dim, &available );
    }
    else if ( available_width > pencil_size_get_standard_font_size( (*this_).pencil_size ) )
    {
        /* try to fit the label into the available space */
        geometry_dimensions_t label_dim;
        if ( draw_estimator_feat != NULL )
        {
            draw_feature_label_get_key_and_value_dimensions( draw_estimator_feat,
                                                             feature,
                                                             (*this_).profile,
                                                             &available_dim,
                                                             (*this_).pencil_size,
                                                             (*this_).font_layout,
                                                             &label_dim
                                                           );
        }
        else
        {
            draw_relationship_label_get_type_and_name_dimensions( draw_estimator_rel,
                                                                  relation,
                                                                  (*this_).profile,
                                                                  &available_dim,
                                                                  (*this_).pencil_size,
                                                                  (*this_).font_layout,
                                                                  &label_dim
                                                                );
        }

        *out_solution = geometry_anchor_align_biased_dim( anchor, &label_dim, &available );

        U8_TRACE_INFO_INT_INT("pencil_floating_label: label does not fit to available space", (int) geometry_dimensions_get_width( preferred_dim ), (int) available_width );
        U8_TRACE_INFO_INT("pencil_floating_label: suitable label width found:", (int) geometry_dimensions_get_width( &label_dim ) );
    }
    else
    {
        /* there is no space, accept any overlap */
        *out_solution = geometry_anchor_align_dim( anchor, preferred_dim );
    }

    U8_TRACE_END();
}


/*
Copyright 2025-2025 Andreas Warnke

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
