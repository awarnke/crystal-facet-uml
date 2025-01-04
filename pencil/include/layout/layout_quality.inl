/* File: layout_quality.inl; Copyright and License: see below */

#include "u8/u8_trace.h"

static inline void layout_quality_init ( layout_quality_t *this_, const pencil_size_t *pencil_size )
{
    (*this_).pencil_size = pencil_size;
}

static inline layout_quality_t layout_quality_new ( const pencil_size_t *pencil_size )
{
    layout_quality_t result;
    layout_quality_init( &result, pencil_size );
    return result;
}

static inline void layout_quality_destroy ( layout_quality_t *this_ )
{
}

static inline double layout_quality_debts_class_diag( const layout_quality_t *this_,
                                                      const layout_visible_classifier_t *probe,
                                                      const layout_diagram_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_class_class( const layout_quality_t *this_,
                                                       const layout_visible_classifier_t *probe,
                                                       const layout_visible_classifier_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_feat_diag( const layout_quality_t *this_,
                                                     const layout_feature_t *probe,
                                                     const layout_diagram_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_feat_class( const layout_quality_t *this_,
                                                      const layout_feature_t *probe,
                                                      const layout_visible_classifier_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_feat_feat( const layout_quality_t *this_,
                                                     const layout_feature_t *probe,
                                                     const layout_feature_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_rel_diag( const layout_quality_t *this_,
                                                    const layout_relationship_t *probe,
                                                    const layout_diagram_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_rel_class( const layout_quality_t *this_,
                                                     const layout_relationship_t *probe,
                                                     const layout_visible_classifier_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_rel_feat( const layout_quality_t *this_,
                                                    const layout_relationship_t *probe,
                                                    const layout_feature_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_rel_rel( const layout_quality_t *this_,
                                                   const layout_relationship_t *probe,
                                                   const layout_relationship_t *other )
{
    double debts = 0.0;
    return debts;
}

static inline double layout_quality_debts_sym_sym( const layout_quality_t *this_,
                                                   const geometry_rectangle_t *probe,
                                                   const geometry_rectangle_t *other)
{
    double debts = 0.0;
    return debts;
}

static const geometry_3dir_t PENCIL_BAD_V_PATTERN1
= { .first = GEOMETRY_DIRECTION_LEFT,  .second = GEOMETRY_DIRECTION_DOWN,  .third = GEOMETRY_DIRECTION_LEFT };
static const geometry_3dir_t PENCIL_BAD_V_PATTERN2
= { .first = GEOMETRY_DIRECTION_RIGHT, .second = GEOMETRY_DIRECTION_UP,    .third = GEOMETRY_DIRECTION_RIGHT };
static const geometry_3dir_t PENCIL_BAD_H_PATTERN1
= { .first = GEOMETRY_DIRECTION_DOWN,  .second = GEOMETRY_DIRECTION_RIGHT, .third = GEOMETRY_DIRECTION_DOWN };
static const geometry_3dir_t PENCIL_BAD_H_PATTERN2
= { .first = GEOMETRY_DIRECTION_UP,    .second = GEOMETRY_DIRECTION_LEFT,  .third = GEOMETRY_DIRECTION_UP };

static inline double layout_quality_debts_conn_diag( const layout_quality_t *this_,
                                                     const geometry_connector_t *probe,
                                                     const geometry_rectangle_t *source_rect,
                                                     const geometry_rectangle_t *dest_rect,
                                                     const layout_diagram_t *other )
{
    double debts = 0.0;

    /* get information on probe */
    const geometry_rectangle_t connector_bounds
        = geometry_connector_get_bounding_rectangle( probe );

    /* get information on expected source and destination */
    const double src_center_x = geometry_rectangle_get_center_x ( source_rect );
    const double src_center_y = geometry_rectangle_get_center_y ( source_rect );
    const double dst_center_x = geometry_rectangle_get_center_x ( dest_rect );
    const double dst_center_y = geometry_rectangle_get_center_y ( dest_rect );

    /* get draw area */
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( other );
    const double diagram_draw_center_x = geometry_rectangle_get_center_x( diagram_draw_area );
    const double diagram_draw_center_y = geometry_rectangle_get_center_y( diagram_draw_area );

    /* get preferred object distance */
    const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* the more length, the more unwanted... */
    debts += geometry_connector_get_length( probe );

    /* prefer _either_ no _or_ minimum-dist lengths of parts... */
    const double HEAVIER_THAN_DETOUR = 4.0;
    const double source_length = geometry_connector_get_source_length( probe );
    if (( source_length > 0.000001 )&&( source_length < object_dist ))
    {
        debts += HEAVIER_THAN_DETOUR * ( object_dist - source_length );

    }
    const double destination_length = geometry_connector_get_destination_length( probe );
    if (( destination_length > 0.000001 )&&( destination_length < object_dist ))
    {
        debts += HEAVIER_THAN_DETOUR * ( object_dist - destination_length );
    }
    const bool no_source_or_no_dest = ( source_length < 0.000001 )||( destination_length < 0.000001 );
    const double main_length = geometry_connector_get_main_length( probe );
    if (( main_length > 0.000001 )&&( main_length < object_dist )&&( no_source_or_no_dest ))
    {
        debts += HEAVIER_THAN_DETOUR * ( object_dist - main_length );
    }

    /* prefer centered over uncentered departure and arrival */
    const double HEAVIER_THAN_CENTERED = 2.0;
    const double delta_source
    = fmin( fabs( geometry_connector_get_source_end_x( probe ) - src_center_x ),
            fabs( geometry_connector_get_source_end_y( probe ) - src_center_y ) );
    debts += delta_source * HEAVIER_THAN_CENTERED;
    const double delta_destination
    = fmin( fabs( geometry_connector_get_destination_end_x( probe ) - dst_center_x ),
            fabs( geometry_connector_get_destination_end_y( probe ) - dst_center_y ) );
    debts += delta_destination * HEAVIER_THAN_CENTERED;

    /* prefer left-hand angles over right-handed */
    const geometry_3dir_t pattern = geometry_connector_get_directions( probe );
    const bool bad_pattern_v
        = geometry_3dir_equals( &pattern, &PENCIL_BAD_V_PATTERN1 ) || geometry_3dir_equals( &pattern, &PENCIL_BAD_V_PATTERN2 );
    const bool bad_pattern_h
        = geometry_3dir_equals( &pattern, &PENCIL_BAD_H_PATTERN1 ) || geometry_3dir_equals( &pattern, &PENCIL_BAD_H_PATTERN2 );
    if ( bad_pattern_h || bad_pattern_v )
    {
        const double current_len = geometry_connector_get_length( probe );
        if ( current_len > ( 4.0 * object_dist ) )
        {
            /* probe is a long path and right-handed */
            debts += 0.2 * geometry_connector_get_length( probe );
        }
    }

    /* to avoid bad patterns: no L on top-left, no 7 on bottom-right, no r on top-right, no J on bottom-left */
    {
        const bool connector_is_left
        = geometry_rectangle_get_center_x( &connector_bounds ) < diagram_draw_center_x;
        const bool connector_is_top
        = geometry_rectangle_get_center_y( &connector_bounds ) < diagram_draw_center_y;
        if ( connector_is_left )
        {
            if ( connector_is_top )
            {
                static const geometry_3dir_t PENCIL_BAD_L_PATTERN1
                    = { .first = GEOMETRY_DIRECTION_LEFT,  .second = GEOMETRY_DIRECTION_UP,     .third = GEOMETRY_DIRECTION_CENTER };
                static const geometry_3dir_t PENCIL_BAD_L_PATTERN2
                    = { .first = GEOMETRY_DIRECTION_CENTER, .second = GEOMETRY_DIRECTION_LEFT,  .third = GEOMETRY_DIRECTION_UP };
                static const geometry_3dir_t PENCIL_BAD_L_PATTERN3
                    = { .first = GEOMETRY_DIRECTION_DOWN,   .second = GEOMETRY_DIRECTION_RIGHT, .third = GEOMETRY_DIRECTION_CENTER };
                static const geometry_3dir_t PENCIL_BAD_L_PATTERN4
                    = { .first = GEOMETRY_DIRECTION_CENTER, .second = GEOMETRY_DIRECTION_DOWN,  .third = GEOMETRY_DIRECTION_RIGHT };

                if (( geometry_3dir_equals( &pattern, &PENCIL_BAD_L_PATTERN1 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_L_PATTERN2 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_L_PATTERN3 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_L_PATTERN4 ) ))
                {
                    debts += 4.0 * geometry_connector_get_length( probe );
                }
            }
            else
            {
                static const geometry_3dir_t PENCIL_BAD_J_PATTERN1
                    = { .first = GEOMETRY_DIRECTION_DOWN,   .second = GEOMETRY_DIRECTION_LEFT,  .third = GEOMETRY_DIRECTION_CENTER };
                static const geometry_3dir_t PENCIL_BAD_J_PATTERN2
                    = { .first = GEOMETRY_DIRECTION_CENTER, .second = GEOMETRY_DIRECTION_DOWN,  .third = GEOMETRY_DIRECTION_LEFT };
                static const geometry_3dir_t PENCIL_BAD_J_PATTERN3
                    = { .first = GEOMETRY_DIRECTION_RIGHT,  .second = GEOMETRY_DIRECTION_UP,    .third = GEOMETRY_DIRECTION_CENTER };
                static const geometry_3dir_t PENCIL_BAD_J_PATTERN4
                    = { .first = GEOMETRY_DIRECTION_CENTER, .second = GEOMETRY_DIRECTION_RIGHT, .third = GEOMETRY_DIRECTION_UP };

                if (( geometry_3dir_equals( &pattern, &PENCIL_BAD_J_PATTERN1 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_J_PATTERN2 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_J_PATTERN3 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_J_PATTERN4 ) ))
                {
                    debts += 4.0 * geometry_connector_get_length( probe );
                }
            }
        }
        else
        {
            if ( connector_is_top )
            {
                static const geometry_3dir_t PENCIL_BAD_r_PATTERN1
                    = { .first = GEOMETRY_DIRECTION_UP,     .second = GEOMETRY_DIRECTION_RIGHT, .third = GEOMETRY_DIRECTION_CENTER };
                static const geometry_3dir_t PENCIL_BAD_r_PATTERN2
                    = { .first = GEOMETRY_DIRECTION_CENTER, .second = GEOMETRY_DIRECTION_UP,    .third = GEOMETRY_DIRECTION_RIGHT };
                static const geometry_3dir_t PENCIL_BAD_r_PATTERN3
                    = { .first = GEOMETRY_DIRECTION_LEFT,   .second = GEOMETRY_DIRECTION_DOWN,  .third = GEOMETRY_DIRECTION_CENTER };
                static const geometry_3dir_t PENCIL_BAD_r_PATTERN4
                    = { .first = GEOMETRY_DIRECTION_CENTER, .second = GEOMETRY_DIRECTION_LEFT,  .third = GEOMETRY_DIRECTION_DOWN };

                if (( geometry_3dir_equals( &pattern, &PENCIL_BAD_r_PATTERN1 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_r_PATTERN2 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_r_PATTERN3 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_r_PATTERN4 ) ))
                {
                    debts += 4.0 * geometry_connector_get_length( probe );
                }
            }
            else
            {
                static const geometry_3dir_t PENCIL_BAD_7_PATTERN1
                    = { .first = GEOMETRY_DIRECTION_RIGHT,  .second = GEOMETRY_DIRECTION_DOWN,  .third = GEOMETRY_DIRECTION_CENTER };
                static const geometry_3dir_t PENCIL_BAD_7_PATTERN2
                    = { .first = GEOMETRY_DIRECTION_CENTER, .second = GEOMETRY_DIRECTION_RIGHT, .third = GEOMETRY_DIRECTION_DOWN };
                static const geometry_3dir_t PENCIL_BAD_7_PATTERN3
                    = { .first = GEOMETRY_DIRECTION_UP,     .second = GEOMETRY_DIRECTION_LEFT,  .third = GEOMETRY_DIRECTION_CENTER };
                static const geometry_3dir_t PENCIL_BAD_7_PATTERN4
                    = { .first = GEOMETRY_DIRECTION_CENTER, .second = GEOMETRY_DIRECTION_UP,    .third = GEOMETRY_DIRECTION_LEFT };

                if (( geometry_3dir_equals( &pattern, &PENCIL_BAD_7_PATTERN1 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_7_PATTERN2 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_7_PATTERN3 ) )
                    || ( geometry_3dir_equals( &pattern, &PENCIL_BAD_7_PATTERN4 ) ))
                {
                    debts += 4.0 * geometry_connector_get_length( probe );
                }
            }
        }
    }

    /* add debts for overlap to diagram boundary */
    {
        if ( ! geometry_rectangle_is_containing( diagram_draw_area, &connector_bounds ) )
        {
            debts += 1000000.0;
        }
    }

    return debts;
}

static inline double layout_quality_debts_conn_class ( const layout_quality_t *this_,
                                                       const geometry_connector_t *probe,
                                                       const layout_visible_classifier_t *other )
{
    double debts = 0.0;

    const geometry_rectangle_t connector_bounds
        = geometry_connector_get_bounding_rectangle( probe );

    const geometry_rectangle_t *const classifier_space
        = layout_visible_classifier_get_space_const( other );
    if ( ! geometry_rectangle_is_containing( classifier_space, &connector_bounds ) )
    {
        const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( other );
        if ( geometry_connector_is_intersecting_rectangle( probe, classifier_symbol_box ) )
        {
            debts += 100000.0;
        }

        const geometry_rectangle_t *const classifier_label_box
        = layout_visible_classifier_get_label_box_const( other );
        if ( geometry_connector_is_intersecting_rectangle( probe, classifier_label_box ) )
        {
            debts += 10000.0;
        }
    }

    return debts;
}

static inline double layout_quality_debts_conn_sym( const layout_quality_t *this_,
                                                    const geometry_connector_t *probe,
                                                    const geometry_rectangle_t *other )
{
    double debts = 0.0;

    if ( geometry_connector_is_intersecting_rectangle( probe, other ) )
    {
        debts += 30000.0;
    }

    return debts;
}

static inline double layout_quality_debts_conn_conn( const layout_quality_t *this_,
                                                     const geometry_connector_t *probe,
                                                     const geometry_connector_t *other )
{
    double debts = 0.0;

    /* get data on probe */
    const geometry_3dir_t pattern = geometry_connector_get_directions( probe );
    const bool bad_pattern_v
    = geometry_3dir_equals( &pattern, &PENCIL_BAD_V_PATTERN1 ) || geometry_3dir_equals( &pattern, &PENCIL_BAD_V_PATTERN2 );
    const bool bad_pattern_h
    = geometry_3dir_equals( &pattern, &PENCIL_BAD_H_PATTERN1 ) || geometry_3dir_equals( &pattern, &PENCIL_BAD_H_PATTERN2 );

    const uint32_t intersects
        = geometry_connector_count_connector_intersects( probe, other );
    debts += 1000.0 * intersects;

    if ( ( bad_pattern_h || bad_pattern_v ) && ( intersects > 0 ) )
    {
        const geometry_3dir_t other_pattern = geometry_connector_get_directions( other );
        const bool bad_other_v
            = geometry_3dir_equals( &other_pattern, &PENCIL_BAD_V_PATTERN1 )
            || geometry_3dir_equals( &other_pattern, &PENCIL_BAD_V_PATTERN2 );
        const bool bad_other_h
            = geometry_3dir_equals( &other_pattern, &PENCIL_BAD_H_PATTERN1 )
            || geometry_3dir_equals( &other_pattern, &PENCIL_BAD_H_PATTERN2 );
        if (( bad_pattern_h && bad_other_v )||( bad_pattern_v && bad_other_h ))
        {
            debts += 1000000.0;
        }
    }

    return debts;
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
