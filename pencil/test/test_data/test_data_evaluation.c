/* File: test_data_evaluation.c; Copyright and License: see below */

#include "test_data/test_data_evaluation.h"
#include "filter/pencil_rules.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

void test_data_evaluation_init( test_data_evaluation_t *this_ )
{
    U8_TRACE_BEGIN();

    data_rules_init ( &((*this_).filter_rules) );

    U8_TRACE_END();
}

void test_data_evaluation_destroy( test_data_evaluation_t *this_ )
{
    U8_TRACE_BEGIN();
    U8_TRACE_END();
}

void test_data_evaluation_analyze( const test_data_evaluation_t *this_,
                                   const layout_visible_set_t *input_layout,
                                   data_stat_t *io_layout_stat,
                                   void (*overlap_callback)(void *data, const geometry_rectangle_t *a, const geometry_rectangle_t *b),
                                   void *data )
{
    U8_TRACE_BEGIN();
    assert( input_layout != NULL );
    assert( (*input_layout).visible_classifier_count <= LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS );
    assert( (*input_layout).feature_count <= LAYOUT_VISIBLE_SET_MAX_FEATURES );
    assert( (*input_layout).relationship_count <= LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS );
    assert( io_layout_stat != NULL );
    pencil_rules_t pencil_rules;
    pencil_rules_init( &pencil_rules );

    /* check if diagram is valid */

    if ( (*input_layout).diagram_valid )
    {
        data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_EXPORTED );

        const geometry_rectangle_t *const diag_bounds = layout_diagram_get_bounds_const( &((*input_layout).diagram_layout) );
        const geometry_rectangle_t *const diag_space = layout_diagram_get_draw_area_const( &((*input_layout).diagram_layout) );
        const data_diagram_t *const diag_data = layout_diagram_get_data_const ( &((*input_layout).diagram_layout) );
        const data_diagram_type_t diag_type = data_diagram_get_diagram_type ( diag_data );

        /* check classifiers against diagram */

        for ( uint_fast32_t c_idx = 0; c_idx < (*input_layout).visible_classifier_count; c_idx ++ )
        {
            const layout_visible_classifier_t *const classifier = &((*input_layout).visible_classifier_layout[c_idx]);
            const geometry_rectangle_t *const c_symbox
                = layout_visible_classifier_get_symbol_box_const( classifier );
            const geometry_rectangle_t *const c_label
               = layout_visible_classifier_get_label_box_const( classifier );
            const geometry_rectangle_t *const c_space
               = layout_visible_classifier_get_space_const( classifier );

            if ( geometry_rectangle_is_containing( diag_space, c_symbox )
                && geometry_rectangle_is_containing( diag_space, c_label ) )
            {
                data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_EXPORTED );

                /* check current classifier against already processed classifiers */

                for ( uint_fast32_t probe_idx = 0; probe_idx < c_idx; probe_idx ++ )
                {
                    const layout_visible_classifier_t *const probe = &((*input_layout).visible_classifier_layout[probe_idx]);
                    const geometry_rectangle_t *const probe_symbox
                        = layout_visible_classifier_get_symbol_box_const( probe );
                    const geometry_rectangle_t *const probe_label
                        = layout_visible_classifier_get_label_box_const( probe );
                    const geometry_rectangle_t *const probe_space
                        = layout_visible_classifier_get_space_const( probe );

                    const bool symbox_overlaps
                        = geometry_rectangle_is_intersecting( c_symbox, probe_symbox );
                    const bool mixed_overlaps
                        = ( geometry_rectangle_is_intersecting( c_symbox, probe_label )
                        || geometry_rectangle_is_intersecting( c_label, probe_symbox ) );
                    const bool label_overlaps
                        = geometry_rectangle_is_intersecting( c_label, probe_label );

                    if ( symbox_overlaps || mixed_overlaps || label_overlaps )
                    {
                        const bool probe_is_ancestor
                            = layout_visible_set_is_ancestor( input_layout,
                                                              probe,      /* ancestor */
                                                              classifier  /* descendant */
                                                            );
                        const bool probe_is_descendant
                            = layout_visible_set_is_ancestor( input_layout,
                                                              classifier,  /* ancestor */
                                                              probe        /* descendant */
                                                            );
                        const bool probe_contains_c
                            = ( geometry_rectangle_is_containing( probe_space, c_symbox )
                            && geometry_rectangle_is_containing( probe_space, c_label ) );
                        const bool c_contains_probe
                            = ( geometry_rectangle_is_containing( c_space, probe_symbox )
                            && geometry_rectangle_is_containing( c_space, probe_label ) );
                        if ( probe_is_ancestor && probe_contains_c )
                        {
                            /* ok */
                        }
                        else if ( probe_is_descendant && c_contains_probe )
                        {
                            /* ok */
                        }
                        else
                        {
                            data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_WARNING );
                            (*overlap_callback)( data, probe_space, c_symbox );
                            (*overlap_callback)( data, probe_space, c_label );
                            (*overlap_callback)( data, c_space, probe_symbox );
                            (*overlap_callback)( data, c_space, probe_label );
                        }
                    }
                }

            }
            else if ( geometry_rectangle_is_containing( diag_bounds, c_symbox )
                && geometry_rectangle_is_containing( diag_bounds, c_label ) )
            {
                data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_EXPORTED );
                data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_WARNING );
                (*overlap_callback)( data, diag_bounds, c_symbox );
                (*overlap_callback)( data, diag_bounds, c_label );
            }
            else
            {
                data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_ERROR );
                (*overlap_callback)( data, diag_space, diag_space );
            }

        }

        /* check features against diagram */

        for ( uint_fast32_t f_idx = 0; f_idx < (*input_layout).feature_count; f_idx ++ )
        {
            const layout_feature_t *const feature = &((*input_layout).feature_layout[f_idx]);
            const data_feature_t *const feature_data = layout_feature_get_data_const( feature );
            const data_feature_type_t feature_type = data_feature_get_main_type( feature_data );
            const data_row_t feature_id = layout_feature_get_feature_id( feature );
            const bool feature_visible
                = data_rules_diagram_shows_feature( &((*input_layout).filter_rules),
                                                    (*input_layout).input_data,
                                                    feature_id
                                                  );
            const data_stat_table_t feat_or_lifeline
                = ( feature_type == DATA_FEATURE_TYPE_LIFELINE ) ? DATA_STAT_TABLE_LIFELINE : DATA_STAT_TABLE_FEATURE;

            const geometry_rectangle_t *const f_symbox
                = layout_feature_get_symbol_box_const( feature );
            const geometry_rectangle_t *const f_label
                = layout_feature_get_label_box_const( feature );

            if ( ! feature_visible )
            {
                /* nothing to do, feature is not visible in this diagram */
            }
            else if ( ( geometry_rectangle_is_empty( f_symbox )
                || geometry_rectangle_is_containing( diag_space, f_symbox ))
                && geometry_rectangle_is_containing( diag_space, f_label ) )
            {
                data_stat_inc_count( io_layout_stat, feat_or_lifeline, DATA_STAT_SERIES_EXPORTED );

                /* check features against classifiers */

                for ( uint_fast32_t probe_idx = 0; probe_idx < (*input_layout).visible_classifier_count; probe_idx ++ )
                {
                    const layout_visible_classifier_t *const probe = &((*input_layout).visible_classifier_layout[probe_idx]);
                    const data_classifier_t *const probe_data = layout_visible_classifier_get_classifier_const( probe );
                    const data_classifier_type_t probe_type = data_classifier_get_main_type( probe_data );
                    /* determine if this probe classifier shall be drawn in the same rectangle as the lifeline */
                    const bool feature_is_proxy_for_probe
                        = pencil_rules_feature_is_implicit_proxy( &pencil_rules, feature_type, probe_type, diag_type );
                    const geometry_rectangle_t *const probe_symbox
                        = layout_visible_classifier_get_symbol_box_const( probe );
                    const geometry_rectangle_t *const probe_label
                        = layout_visible_classifier_get_label_box_const( probe );
                    const geometry_rectangle_t *const c_space
                        = layout_visible_classifier_get_space_const( probe );

                    const bool symbox_overlaps
                        = geometry_rectangle_is_intersecting( f_symbox, probe_symbox )
                        && ( ! geometry_rectangle_is_containing( c_space, f_symbox ) );
                    const bool f_sym_overlaps_c_label
                        = geometry_rectangle_is_intersecting( f_symbox, probe_label );
                    const bool f_label_overlaps_c_sym
                        = geometry_rectangle_is_intersecting( f_label, probe_symbox )
                        && ( ! geometry_rectangle_is_containing( c_space, f_label ) );
                    const bool label_overlaps
                        = geometry_rectangle_is_intersecting( f_label, probe_label );

                    if ( ! feature_is_proxy_for_probe )
                    {
                        if ( f_sym_overlaps_c_label || f_label_overlaps_c_sym || label_overlaps )
                        {
                            data_stat_inc_count( io_layout_stat, feat_or_lifeline, DATA_STAT_SERIES_WARNING );
                            (*overlap_callback)( data, f_symbox, probe_label );
                            (*overlap_callback)( data, f_label, probe_symbox );
                            (*overlap_callback)( data, f_label, probe_label );
                        }
                        else if ( symbox_overlaps )
                        {
                            const layout_visible_classifier_t *const f_parent
                                = layout_feature_get_classifier_const ( feature );
                            const bool probe_is_parent
                                = layout_visible_classifier_is_equal_diagramelement_id( f_parent, probe );
                            if ( probe_is_parent )
                            {
                                /* ok: a feature may overlap its own parent classifier */
                            }
                            else
                            {
                                data_stat_inc_count( io_layout_stat, feat_or_lifeline, DATA_STAT_SERIES_WARNING );
                                (*overlap_callback)( data, f_symbox, probe_symbox );
                            }
                        }
                    }
                }

                /* check current feature against already processed features */

                for ( uint_fast32_t probe_idx = 0; probe_idx < f_idx; probe_idx ++ )
                {
                    const layout_feature_t *const probe = &((*input_layout).feature_layout[probe_idx]);
                    const data_row_t probe_id = layout_feature_get_feature_id( probe );
                    const bool probe_visible
                        = data_rules_diagram_shows_feature( &((*input_layout).filter_rules),
                                                            (*input_layout).input_data,
                                                            probe_id
                                                          );
                    const geometry_rectangle_t *const probe_symbox
                         = layout_feature_get_symbol_box_const( probe );
                    const geometry_rectangle_t *const probe_label
                        = layout_feature_get_label_box_const( probe );

                    if ( probe_visible )
                    {
                        const bool symbox_overlaps
                            = geometry_rectangle_is_intersecting( f_symbox, probe_symbox );
                        const bool mixed_overlaps
                            = ( geometry_rectangle_is_intersecting( f_symbox, probe_label )
                            || geometry_rectangle_is_intersecting( f_label, probe_symbox ) );
                        const bool label_overlaps
                            = geometry_rectangle_is_intersecting( f_label, probe_label );

                        if ( mixed_overlaps || label_overlaps )
                        {
                            data_stat_inc_count( io_layout_stat, feat_or_lifeline, DATA_STAT_SERIES_WARNING );
                            (*overlap_callback)( data, f_symbox, probe_label );
                            (*overlap_callback)( data, f_label, probe_symbox );
                            (*overlap_callback)( data, f_label, probe_label );
                        }
                        else if ( symbox_overlaps )
                        {
                            const layout_visible_classifier_t *const f_parent
                                = layout_feature_get_classifier_const ( feature );
                            const layout_visible_classifier_t *const probe_parent
                                = layout_feature_get_classifier_const ( probe );

                            const bool same_parent
                                = layout_visible_classifier_is_equal_diagramelement_id( f_parent, probe_parent );
                            if ( same_parent )
                            {
                                /* ok, not a layouting issue */
                            }
                            else
                            {
                                data_stat_inc_count( io_layout_stat, feat_or_lifeline, DATA_STAT_SERIES_WARNING );
                                (*overlap_callback)( data, f_symbox, probe_symbox );
                            }
                        }
                    }
                }
            }
            else if ( geometry_rectangle_is_containing( diag_bounds, f_symbox )
                && geometry_rectangle_is_containing( diag_bounds, f_label ) )
            {
                data_stat_inc_count( io_layout_stat, feat_or_lifeline, DATA_STAT_SERIES_EXPORTED );
                data_stat_inc_count( io_layout_stat, feat_or_lifeline, DATA_STAT_SERIES_WARNING );
                (*overlap_callback)( data, diag_bounds, f_symbox );
                (*overlap_callback)( data, diag_bounds, f_label );
            }
            else
            {
                data_stat_inc_count( io_layout_stat, feat_or_lifeline, DATA_STAT_SERIES_ERROR );
                (*overlap_callback)( data, diag_space, diag_space );
            }
        }

        /* check relationships against diagram */

        for ( uint_fast32_t r_idx = 0; r_idx < (*input_layout).relationship_count; r_idx ++ )
        {
            const layout_relationship_t *const relationship = &((*input_layout).relationship_layout[r_idx]);
            const data_row_t relationship_id = layout_relationship_get_relationship_id( relationship );
            const bool relationship_visible
                = data_rules_diagram_shows_relationship( &((*input_layout).filter_rules),
                                                         (*input_layout).input_data,
                                                         relationship_id
                                                       );
            const geometry_rectangle_t *const r_label
                = layout_relationship_get_label_box_const( relationship );
            const geometry_connector_t *const r_shape
                = layout_relationship_get_shape_const( relationship );
            const geometry_rectangle_t r_bounds = geometry_connector_get_bounding_rectangle( r_shape );

            if ( ! relationship_visible )
            {
                /* nothing to do, relationship is not visible in this diagram */
            }
            else if ( geometry_rectangle_is_containing( diag_space, &r_bounds )
                 && geometry_rectangle_is_containing( diag_space, r_label ) )
            {
                data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_EXPORTED );

                /* check relationships against classifiers */

                for ( uint_fast32_t probe_idx = 0; probe_idx < (*input_layout).visible_classifier_count; probe_idx ++ )
                {
                    const layout_visible_classifier_t *const probe = &((*input_layout).visible_classifier_layout[probe_idx]);
                    const geometry_rectangle_t *const probe_symbox
                        = layout_visible_classifier_get_symbol_box_const( probe );
                    const geometry_rectangle_t *const probe_label
                        = layout_visible_classifier_get_label_box_const( probe );
                    const geometry_rectangle_t *const probe_space
                        = layout_visible_classifier_get_space_const( probe );

                    const bool label_overlaps_label
                        = geometry_rectangle_is_intersecting( r_label, probe_label );
                    const bool label_overlaps_symbox
                        = ( geometry_rectangle_is_intersecting( r_label, probe_symbox )
                        && ! geometry_rectangle_is_containing( probe_space, r_label ) );
                    const bool shape_overlaps_label
                        = geometry_connector_is_intersecting_rectangle( r_shape, probe_label );

                    if ( label_overlaps_label || label_overlaps_symbox || shape_overlaps_label )
                    {
                        data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
                        (*overlap_callback)( data, r_label, probe_label );
                        (*overlap_callback)( data, r_label, probe_symbox );
                        (*overlap_callback)( data, &r_bounds, probe_label );
                    }
                }

                /* check relationships against features */

                for ( uint_fast32_t probe_idx = 0; probe_idx < (*input_layout).feature_count; probe_idx ++ )
                {
                    const layout_feature_t *const probe = &((*input_layout).feature_layout[probe_idx]);
                    const data_feature_t *const probe_data = layout_feature_get_data_const( probe );
                    const data_feature_type_t probe_type = data_feature_get_main_type( probe_data );
                    const data_row_t probe_id = layout_feature_get_feature_id( probe );
                    const bool probe_visible
                        = data_rules_diagram_shows_feature( &((*input_layout).filter_rules),
                                                            (*input_layout).input_data,
                                                            probe_id
                                                          );
                    const geometry_rectangle_t *const probe_symbox
                         = layout_feature_get_symbol_box_const( probe );
                    const geometry_rectangle_t *const probe_label
                        = layout_feature_get_label_box_const( probe );

                    if ( probe_visible && ( DATA_FEATURE_TYPE_LIFELINE != probe_type ) )
                    {
                        const bool label_overlaps_label
                            = geometry_rectangle_is_intersecting( r_label, probe_label );
                        const bool label_overlaps_symbox
                            = geometry_rectangle_is_intersecting( r_label, probe_symbox );
                        const bool shape_overlaps_label
                            = geometry_connector_is_intersecting_rectangle( r_shape, probe_label );

                        if (  label_overlaps_label || label_overlaps_symbox || shape_overlaps_label )
                        {
                            data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
                            (*overlap_callback)( data, r_label, probe_label );
                            (*overlap_callback)( data, r_label, probe_symbox );
                            (*overlap_callback)( data, &r_bounds, probe_label );
                        }
                    }
                }

                /* check current relationship against already processed relationships */

                for ( uint_fast32_t probe_idx = 0; probe_idx < r_idx; probe_idx ++ )
                {
                    const layout_relationship_t *const probe = &((*input_layout).relationship_layout[probe_idx]);
                    const data_row_t probe_id = layout_relationship_get_relationship_id( probe );
                    const bool probe_visible
                        = data_rules_diagram_shows_relationship( &((*input_layout).filter_rules),
                                                                 (*input_layout).input_data,
                                                                 probe_id
                                                               );
                    const geometry_rectangle_t *const probe_label
                        = layout_relationship_get_label_box_const( probe );
                    const geometry_connector_t *const probe_shape
                        = layout_relationship_get_shape_const( probe );

                    if ( probe_visible )
                    {
                        const bool label_overlaps
                            = geometry_rectangle_is_intersecting( r_label, probe_label );
                        const bool mixed_overlaps
                            = ( geometry_connector_is_intersecting_rectangle( r_shape, probe_label )
                            || geometry_connector_is_intersecting_rectangle( probe_shape, r_label ) );

                        if ( label_overlaps || mixed_overlaps )
                        {
                            data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
                            (*overlap_callback)( data, r_label, probe_label );
                            (*overlap_callback)( data, &r_bounds, probe_label );
                            const geometry_rectangle_t probe_bounds
                                = geometry_connector_get_bounding_rectangle( probe_shape );
                            (*overlap_callback)( data, &probe_bounds, r_label );
                        }
                    }
                }

            }
            else if ( geometry_rectangle_is_containing( diag_bounds, &r_bounds )
                && geometry_rectangle_is_containing( diag_bounds, r_label ) )
            {
                data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_EXPORTED );
                data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
                (*overlap_callback)( data, diag_bounds, &r_bounds );
                (*overlap_callback)( data, diag_bounds, r_label );
            }
            else
            {
                data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR );
                (*overlap_callback)( data, diag_space, diag_space );
            }
        }
    }
    else
    {
        data_stat_inc_count( io_layout_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_ERROR );
    }

    pencil_rules_destroy( &pencil_rules );
    U8_TRACE_END();
}


/*
Copyright 2017-2025 Andreas Warnke

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
