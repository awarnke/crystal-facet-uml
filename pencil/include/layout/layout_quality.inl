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

/* NO-GOs */

/* if an object is forbidden (e.g. german swastika): */
#define LAYOUT_QUALITY_WEIGHT_FORBIDDEN (1000000.0)
/* if an object is not fully contained in the diagrams drawing area: */
#define LAYOUT_QUALITY_WEIGHT_NOT_IN_DIAGRAM_AREA (1000.0)

/* OVERLAPS */

/* if an objects label or type-icon crosses another objects label or type-icon: */
#define LAYOUT_QUALITY_WEIGHT_LABEL_OVERLAP (100.0)
/* if an objects label or type-icon crosses another objects contour or connection line: */
#define LAYOUT_QUALITY_WEIGHT_LABEL_ON_LINE (10.0)
/* if an objects contour or connection line is shared with another objects contour or connection line: */
#define LAYOUT_QUALITY_WEIGHT_SHARED_LINES (10.0)
/* if an objects contour or connection line crosses another objects contour or connection line: */
#define LAYOUT_QUALITY_WEIGHT_CROSS_LINES (10.0)
/* if an objects contour or connection line crosses another objects envelope-area: */
#define LAYOUT_QUALITY_WEIGHT_CROSS_LINE_AREA (5.0)
/* if an objects envelope-area crosses another objects envelope-area: */
#define LAYOUT_QUALITY_WEIGHT_CROSS_AREAS (1.0)

/* SUBOPTIMAL LOCATIONS OR DISTANCES */

/* if an object shall be avoided to find nice solutions and not run into a local layouting optimum that is forbidden: */
#define LAYOUT_QUALITY_WEIGHT_AVOID (10.0)
/* if a location is not nice (too short line segment, too far from source or target): */
#define LAYOUT_QUALITY_WEIGHT_LOCATION (3.0)
/* if an objects contour, label or type-icon is too far from the target location or a connection line is longer than needed: */
#define LAYOUT_QUALITY_WEIGHT_DISTANCE (1.0)

static inline double layout_quality_debts_class_diag( const layout_quality_t *this_,
                                                      const layout_visible_classifier_t *probe,
                                                      const geometry_offset_t *order_target,
                                                      const layout_diagram_t *other )
{
    double debts = 0.0;

    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( other );

    const geometry_rectangle_t *const classifier_bounds
        = layout_visible_classifier_get_envelope_box_const( probe );

    /* add debts for overlap to diagram boundary */
    if ( ! geometry_rectangle_is_containing( diagram_draw_area, classifier_bounds ) )
    {
        /* it does not matter how big a classifier is - being outside is a high debt: */
        debts += LAYOUT_QUALITY_WEIGHT_NOT_IN_DIAGRAM_AREA * geometry_rectangle_get_area ( diagram_draw_area );
    }

    /* add move distance as debt */
    debts += LAYOUT_QUALITY_WEIGHT_LOCATION * fabs( geometry_offset_get_dx( order_target ) );
    debts += LAYOUT_QUALITY_WEIGHT_LOCATION * fabs( geometry_offset_get_dy( order_target ) );

    return debts;
}

static inline double layout_quality_debts_class_class( const layout_quality_t *this_,
                                                       const layout_visible_classifier_t *probe,
                                                       const layout_visible_classifier_t *other,
                                                       const layout_visible_set_t *layout_data )
{
    double debts = 0.0;

    const geometry_rectangle_t *const probe_symbol_box
        = layout_visible_classifier_get_symbol_box_const( probe );
    const geometry_rectangle_t *const other_symbol_box
        = layout_visible_classifier_get_symbol_box_const( other );

    geometry_rectangle_t probe_intersect;
    const int intersect_err
        = geometry_rectangle_init_by_intersect( &probe_intersect, probe_symbol_box, other_symbol_box );
    if ( 0 == intersect_err )
    {
        /* there is an intersect */
        if ( layout_visible_set_is_ancestor( layout_data, probe, other ) )
        {
            /* no debt: parent my overlap children */
        }
        else if ( layout_visible_set_is_ancestor( layout_data, other, probe ) )
        {
            /* no debt: child may overlap parent */
        }
        else
        {
            const double probe_intersect_area = geometry_rectangle_get_area ( &probe_intersect );
            debts += LAYOUT_QUALITY_WEIGHT_CROSS_AREAS * probe_intersect_area;
        }
    }
    /* else no intersect/overlap of symbol box */

    /* independant of relationship between classifiers, overlapping labels are not good */
    const geometry_rectangle_t *const probe_icon_box
        = layout_visible_classifier_get_icon_box_const( probe );
    const geometry_rectangle_t *const probe_label_box
        = layout_visible_classifier_get_label_box_const( probe );
    debts += layout_quality_debts_label_class( this_, probe_icon_box, other );
    debts += layout_quality_debts_label_class( this_, probe_label_box, other );

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

    /* get preferred object distance and line-corrdor width */
    const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    const double line_corridor = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* add debts for exceeding the diagram draw area */
    if ( ! geometry_rectangle_is_containing( diagram_draw_area, &connector_bounds ) )
    {
        /* high debt */
        debts += LAYOUT_QUALITY_WEIGHT_NOT_IN_DIAGRAM_AREA * geometry_rectangle_get_area(diagram_draw_area);
    }

    /* the more length, the more unwanted... */
    debts += LAYOUT_QUALITY_WEIGHT_DISTANCE * geometry_connector_get_length( probe ) * line_corridor;

    /* prefer _either_ no _or_ minimum-dist lengths of parts, otherwise line too close to object... */
    const double source_length = geometry_connector_get_source_length( probe );
    if (( source_length > 0.000001 )&&( source_length < object_dist ))
    {
        debts += LAYOUT_QUALITY_WEIGHT_SHARED_LINES * ( object_dist - source_length ) * line_corridor;

    }
    const double destination_length = geometry_connector_get_destination_length( probe );
    if (( destination_length > 0.000001 )&&( destination_length < object_dist ))
    {
        debts += LAYOUT_QUALITY_WEIGHT_SHARED_LINES * ( object_dist - destination_length ) * line_corridor;
    }
    const bool no_source_or_no_dest = ( source_length < 0.000001 )||( destination_length < 0.000001 );
    const double main_length = geometry_connector_get_main_length( probe );
    if (( main_length > 0.000001 )&&( main_length < object_dist )&&( no_source_or_no_dest ))
    {
        debts += LAYOUT_QUALITY_WEIGHT_SHARED_LINES * ( object_dist - main_length ) * line_corridor;
    }

    /* prefer centered over uncentered departure and arrival */
    const double delta_source
        = fmin( fabs( geometry_connector_get_source_end_x( probe ) - src_center_x ),
                fabs( geometry_connector_get_source_end_y( probe ) - src_center_y ) );
    debts += LAYOUT_QUALITY_WEIGHT_LOCATION * delta_source * ( line_corridor );
    const double delta_destination
        = fmin( fabs( geometry_connector_get_destination_end_x( probe ) - dst_center_x ),
                fabs( geometry_connector_get_destination_end_y( probe ) - dst_center_y ) );
    debts += LAYOUT_QUALITY_WEIGHT_LOCATION * delta_destination * ( line_corridor );

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
            /* to avoid overreactions, we assume a line width of 0.1 only */
            debts += LAYOUT_QUALITY_WEIGHT_AVOID * 0.1 * geometry_connector_get_length( probe );
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
                    debts += LAYOUT_QUALITY_WEIGHT_AVOID * 1.0 * geometry_connector_get_length( probe );
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
                    debts += LAYOUT_QUALITY_WEIGHT_AVOID * 1.0 * geometry_connector_get_length( probe );
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
                    debts += LAYOUT_QUALITY_WEIGHT_AVOID * 1.0 * geometry_connector_get_length( probe );
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
                    debts += LAYOUT_QUALITY_WEIGHT_AVOID * 1.0 * geometry_connector_get_length( probe );
                }
            }
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
        const double line_corridor = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
        const double line_width = pencil_size_get_standard_line_width( (*this_).pencil_size );

        const geometry_rectangle_t *const classifier_symbol_box
            = layout_visible_classifier_get_symbol_box_const( other );
        debts += LAYOUT_QUALITY_WEIGHT_CROSS_LINE_AREA
            * geometry_connector_get_transit_length( probe, classifier_symbol_box ) * line_corridor;
        const double same_path
            = geometry_connector_get_same_path_length_rect( probe,
                                                            classifier_symbol_box,
                                                            5.0 * line_width
                                                          );
         /* ^ max_distance is 5x line width because the contour line of a classifier is 3x linewidth within the bounds */
        debts += LAYOUT_QUALITY_WEIGHT_SHARED_LINES * same_path * line_corridor;

        const geometry_rectangle_t *const classifier_icon_box
            = layout_visible_classifier_get_icon_box_const( other );
        debts += LAYOUT_QUALITY_WEIGHT_LABEL_ON_LINE
            * geometry_connector_get_transit_length( probe, classifier_icon_box ) * line_corridor;

        const geometry_rectangle_t *const classifier_label_box
            = layout_visible_classifier_get_label_box_const( other );
        debts += LAYOUT_QUALITY_WEIGHT_LABEL_ON_LINE
            * geometry_connector_get_transit_length( probe, classifier_label_box ) * line_corridor;
    }

    return debts;
}

static inline double layout_quality_debts_conn_sym( const layout_quality_t *this_,
                                                    const geometry_connector_t *probe,
                                                    const geometry_rectangle_t *other )
{
    double debts = 0.0;

    const double line_corridor = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    const double line_width = pencil_size_get_standard_line_width( (*this_).pencil_size );

    debts += LAYOUT_QUALITY_WEIGHT_CROSS_LINE_AREA
        * geometry_connector_get_transit_length( probe, other ) * line_corridor;
    const double same_path
        = geometry_connector_get_same_path_length_rect( probe, other, 5.0 * line_width );
    /* ^ max_distance is 5x line width because the contour line of a classifier is 3x linewidth within the bounds */
    debts += LAYOUT_QUALITY_WEIGHT_SHARED_LINES * same_path * line_corridor;

    return debts;
}

static inline double layout_quality_debts_conn_conn( const layout_quality_t *this_,
                                                     const geometry_connector_t *probe,
                                                     const geometry_connector_t *other )
{
    double debts = 0.0;

    const double line_corridor = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    const double line_width = pencil_size_get_standard_line_width( (*this_).pencil_size );

    /* get data on probe */
    const geometry_3dir_t pattern = geometry_connector_get_directions( probe );
    const bool bad_pattern_v
        = geometry_3dir_equals( &pattern, &PENCIL_BAD_V_PATTERN1 ) || geometry_3dir_equals( &pattern, &PENCIL_BAD_V_PATTERN2 );
    const bool bad_pattern_h
        = geometry_3dir_equals( &pattern, &PENCIL_BAD_H_PATTERN1 ) || geometry_3dir_equals( &pattern, &PENCIL_BAD_H_PATTERN2 );

    const uint32_t intersects
        = geometry_connector_count_connector_intersects( probe, other );
    debts += LAYOUT_QUALITY_WEIGHT_CROSS_LINES * intersects * ( line_corridor * line_corridor );
    const double same_path
        = geometry_connector_get_same_path_length_conn( probe, other, 3.0 * line_width );
    /* ^ max_distance is 3x line width for 1) own line, 2) minimal gap and 3) other line */
    debts += LAYOUT_QUALITY_WEIGHT_SHARED_LINES * same_path * line_corridor;

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
            const geometry_rectangle_t probe_bounds
                = geometry_connector_get_bounding_rectangle( probe );
            const geometry_rectangle_t other_bounds
                = geometry_connector_get_bounding_rectangle( other );

            debts += LAYOUT_QUALITY_WEIGHT_FORBIDDEN * geometry_rectangle_get_area( &probe_bounds );
            debts += LAYOUT_QUALITY_WEIGHT_FORBIDDEN * geometry_rectangle_get_area( &other_bounds );
        }
    }

    return debts;
}

static inline double layout_quality_debts_label_diag( const layout_quality_t *this_,
                                                      const geometry_rectangle_t *probe,
                                                      const geometry_point_t *target_point,
                                                      const layout_diagram_t *other )
{
    double debts = 0.0;

    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( other );

    /* check distance to target point */
    const geometry_point_t probe_middle = geometry_rectangle_get_center( probe );
    debts += LAYOUT_QUALITY_WEIGHT_DISTANCE * geometry_point_calc_chess_distance ( target_point, &probe_middle );

    /* add debts for exceeding the diagram draw area */
    if ( ! geometry_rectangle_is_containing( diagram_draw_area, probe ) )
    {
        /* high debt */
        debts += LAYOUT_QUALITY_WEIGHT_NOT_IN_DIAGRAM_AREA * geometry_rectangle_get_area(diagram_draw_area);
    }

    return debts;
}

static inline double layout_quality_debts_label_class( const layout_quality_t *this_,
                                                       const geometry_rectangle_t *probe,
                                                       const layout_visible_classifier_t *other )
{
    double debts = 0.0;

    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( other );
    if ( geometry_rectangle_is_intersecting( probe, classifier_symbol_box ) )
    {
        /* overlaps to the symbol box are bad only if not contained in space area */
        const geometry_rectangle_t *const classifier_space
            = layout_visible_classifier_get_space_const( other );
        if ( ! geometry_rectangle_is_containing( classifier_space, probe ) )
        {
            /* lower debt */
            debts += LAYOUT_QUALITY_WEIGHT_LABEL_ON_LINE * geometry_rectangle_get_intersect_area( probe, classifier_symbol_box );
        }
    }

    const geometry_rectangle_t *const classifier_label_box
        = layout_visible_classifier_get_label_box_const( other );
    if ( geometry_rectangle_is_intersecting( probe, classifier_label_box ) )
    {
        /* medium debt */
        debts += LAYOUT_QUALITY_WEIGHT_LABEL_OVERLAP * geometry_rectangle_get_intersect_area( probe, classifier_label_box );
    }

    const geometry_rectangle_t *const classifier_icon_box
    = layout_visible_classifier_get_icon_box_const( other );
    if ( geometry_rectangle_is_intersecting( probe, classifier_icon_box ) )
    {
        /* medium debt */
        debts += LAYOUT_QUALITY_WEIGHT_LABEL_OVERLAP * geometry_rectangle_get_intersect_area( probe, classifier_icon_box );
    }

    return debts;
}

static inline double layout_quality_debts_label_feat( const layout_quality_t *this_,
                                                      const geometry_rectangle_t *probe,
                                                      const layout_feature_t *other )
{
    double debts = 0.0;

    const geometry_rectangle_t *const feature_symbol_box
        = layout_feature_get_symbol_box_const( other );
    if ( geometry_rectangle_is_intersecting( probe, feature_symbol_box ) )
    {
        /* no special handling for lifelines */
        /* const data_feature_t *const probe_f_data = layout_feature_get_data_const( other ); */
        /* if ( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( probe_f_data ) ) { } else { } */
        debts += LAYOUT_QUALITY_WEIGHT_LABEL_ON_LINE * geometry_rectangle_get_intersect_area( probe, feature_symbol_box );
    }

    const geometry_rectangle_t *const feature_label_box
        = layout_feature_get_label_box_const( other );
    if ( geometry_rectangle_is_intersecting( probe, feature_label_box ) )
    {
        /* medium debt */
        debts += LAYOUT_QUALITY_WEIGHT_LABEL_OVERLAP * geometry_rectangle_get_intersect_area( probe, feature_label_box );
    }

    return debts;
}

static inline double layout_quality_debts_label_rel( const layout_quality_t *this_,
                                                     const geometry_rectangle_t *probe,
                                                     const layout_relationship_t *other )
{
    double debts = 0.0;

    if (( PENCIL_VISIBILITY_SHOW == layout_relationship_get_visibility( other ) )
        || ( PENCIL_VISIBILITY_GRAY_OUT == layout_relationship_get_visibility( other ) ))
    {
        const double line_corridor = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

        const geometry_connector_t *const other_shape
            = layout_relationship_get_shape_const( other );
        debts += LAYOUT_QUALITY_WEIGHT_CROSS_LINE_AREA
            * geometry_connector_get_transit_length( other_shape, probe ) * line_corridor;

        const geometry_rectangle_t *const relationship_label_box
            = layout_relationship_get_label_box_const( other );
        if ( geometry_rectangle_is_intersecting( probe, relationship_label_box ) )
        {
            /* medium debt */
            debts += LAYOUT_QUALITY_WEIGHT_LABEL_OVERLAP * geometry_rectangle_get_intersect_area( probe, relationship_label_box );
        }
    }

    return debts;
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
