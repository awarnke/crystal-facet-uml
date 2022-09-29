/* File: pencil_relationship_2d_layouter.c; Copyright and License: see below */

#include "pencil_relationship_2d_layouter.h"
#include "trace/trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

void pencil_relationship_2d_layouter_init( pencil_relationship_2d_layouter_t *this_,
                                           pencil_layout_data_t *layout_data,
                                           pencil_size_t *pencil_size )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    universal_array_index_sorter_init( &((*this_).sorted_relationships) );
    (*this_).sorted_rel_index = 0;

    (*this_).pencil_size = pencil_size;
    pencil_relationship_painter_init( &((*this_).relationship_painter) );

    TRACE_END();
}

void pencil_relationship_2d_layouter_destroy( pencil_relationship_2d_layouter_t *this_ )
{
    TRACE_BEGIN();

    universal_array_index_sorter_destroy( &((*this_).sorted_relationships) );

    pencil_relationship_painter_destroy( &((*this_).relationship_painter) );

    TRACE_END();
}

void pencil_relationship_2d_layouter_private_do_layout ( pencil_relationship_2d_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );

    universal_array_index_sorter_reinit( &((*this_).sorted_relationships) );

    /* sort the relationships by their movement-needs, drop invisible relations */
    pencil_relationship_2d_layouter_private_propose_processing_order ( this_ );

    /* shape the relationships */
    const uint32_t count_sorted
        = universal_array_index_sorter_get_count( &((*this_).sorted_relationships) );
    for ( (*this_).sorted_rel_index = 0; (*this_).sorted_rel_index < count_sorted; (*this_).sorted_rel_index ++ )
    {
        /* determine pointer to the_relationship */
        const uint32_t index
            = universal_array_index_sorter_get_array_index ( &((*this_).sorted_relationships),
                                                             (*this_).sorted_rel_index
                                                           );
        layout_relationship_t *const current_relationship
            = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, index );

        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 18;
        geometry_connector_t solution[18];

        /* propose options */
        pencil_relationship_2d_layouter_private_propose_solutions ( this_,
                                                                    SOLUTIONS_MAX,
                                                                    solution,
                                                                    &solutions_count
                                                                  );

        /* select best option */
        uint32_t index_of_best;
        if ( 1 == solutions_count )
        {
            index_of_best = 0;
        }
        else
        {
            pencil_relationship_2d_layouter_private_select_solution ( this_,
                                                                      solutions_count,
                                                                      solution,
                                                                      &index_of_best
                                                                    );
        }

        /* store best option to (*this_).layout_data */
        layout_relationship_set_shape( current_relationship, &(solution[index_of_best]) );
    }

    universal_array_index_sorter_reinit( &((*this_).sorted_relationships) );

    TRACE_END();
}

void pencil_relationship_2d_layouter_private_propose_processing_order ( pencil_relationship_2d_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) DATA_VISIBLE_SET_MAX_RELATIONSHIPS );

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );

    /* sort the relationships by their shaping-needs: the less simple, the earlier it shall be processed */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        layout_relationship_t *const current_relation
            = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

        int64_t simpleness = 0;

        /* determine simpleness by relationship type */
        {
            data_relationship_type_t reltype;
            reltype = data_relationship_get_main_type( layout_relationship_get_data_const ( current_relation ));
            if (( DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY == reltype )
                ||( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == reltype ))
            {
                /* containment may be solved by embracing, mere dependencies are unimportant */
                simpleness += geometry_rectangle_get_width ( diagram_draw_area );
            }
        }

        /* whatever is not visible is simple */
        {
            if (( PENCIL_VISIBILITY_SHOW != layout_relationship_get_visibility ( current_relation ) )
                && ( PENCIL_VISIBILITY_GRAY_OUT != layout_relationship_get_visibility ( current_relation ) ))
            {
                simpleness += 2 * geometry_rectangle_get_width ( diagram_draw_area );
            }
        }

        /* determine simpleness by distance between source and destination */
        {
            const geometry_rectangle_t *const source_rect
                = layout_relationship_get_from_symbol_box_const ( current_relation );
            const geometry_rectangle_t *const dest_rect
                = layout_relationship_get_to_symbol_box_const ( current_relation );

            simpleness -= fabs ( geometry_rectangle_get_center_x(source_rect) - geometry_rectangle_get_center_x(dest_rect) );
            simpleness -= fabs ( geometry_rectangle_get_center_y(source_rect) - geometry_rectangle_get_center_y(dest_rect) );
        }

        /* insert relation to sorted array, the simpler the more to the back */
        {
            int insert_error;
            insert_error = universal_array_index_sorter_insert( &((*this_).sorted_relationships), index, simpleness );
            if ( 0 != insert_error )
            {
                TSLOG_WARNING( "not all relationships are shaped" );
            }
        }
    }

    TRACE_END();
}

void pencil_relationship_2d_layouter_private_propose_solutions ( pencil_relationship_2d_layouter_t *this_,
                                                                 uint32_t solutions_max,
                                                                 geometry_connector_t out_solutions[],
                                                                 uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert ( (*this_).sorted_rel_index < universal_array_index_sorter_get_count( &((*this_).sorted_relationships) ) );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 1 <= solutions_max );  /* general requirement to report at least one option */
    assert ( 18 <= solutions_max );  /* current implementation requires at least 14 options */

    /* get current relation */
    const uint32_t index
        = universal_array_index_sorter_get_array_index( &((*this_).sorted_relationships), (*this_).sorted_rel_index );
    layout_relationship_t *const current_relation
        = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

    /* propose connections between source and destination */
    {
        const geometry_rectangle_t *const source_rect
            = layout_relationship_get_from_symbol_box_const ( current_relation );
        const geometry_rectangle_t *const dest_rect
            = layout_relationship_get_to_symbol_box_const ( current_relation );

        uint32_t solutions_by_I;
        pencil_relationship_2d_layouter_private_connect_rectangles_by_I ( this_,
                                                                          source_rect,
                                                                          dest_rect,
                                                                          solutions_max,
                                                                          &(out_solutions[0]),
                                                                          &solutions_by_I
                                                                        );

        uint32_t solutions_by_ZN;
        pencil_relationship_2d_layouter_private_connect_rectangles_by_ZN ( this_,
                                                                           source_rect,
                                                                           dest_rect,
                                                                           solutions_max - solutions_by_I,
                                                                           &(out_solutions[solutions_by_I]),
                                                                           &solutions_by_ZN
                                                                         );

        uint32_t solutions_by_L7;
        const uint32_t solutions_by_I_ZN = solutions_by_I + solutions_by_ZN;
        pencil_relationship_2d_layouter_private_connect_rectangles_by_L7 ( this_,
                                                                           source_rect,
                                                                           dest_rect,
                                                                           solutions_max - solutions_by_I_ZN,
                                                                           &(out_solutions[solutions_by_I_ZN]),
                                                                           &solutions_by_L7
                                                                         );

        uint32_t solutions_by_UC;
        const uint32_t solutions_by_I_ZN_L7 = solutions_by_I_ZN + solutions_by_L7;
        pencil_relationship_2d_layouter_private_connect_rectangles_by_UC ( this_,
                                                                           source_rect,
                                                                           dest_rect,
                                                                           solutions_max - solutions_by_I_ZN_L7,
                                                                           &(out_solutions[solutions_by_I_ZN_L7]),
                                                                           &solutions_by_UC
                                                                         );

        *out_solutions_count = solutions_by_I_ZN_L7 + solutions_by_UC;
        assert ( 1 <= *out_solutions_count );
        assert ( *out_solutions_count <= solutions_max );
    }

    TRACE_END();
}

static const geometry_3dir_t PENCIL_BAD_V_PATTERN1
   = { .first = GEOMETRY_DIRECTION_LEFT,  .second = GEOMETRY_DIRECTION_DOWN,  .third = GEOMETRY_DIRECTION_LEFT };
static const geometry_3dir_t PENCIL_BAD_V_PATTERN2
   = { .first = GEOMETRY_DIRECTION_RIGHT, .second = GEOMETRY_DIRECTION_UP,    .third = GEOMETRY_DIRECTION_RIGHT };
static const geometry_3dir_t PENCIL_BAD_H_PATTERN1
   = { .first = GEOMETRY_DIRECTION_DOWN,  .second = GEOMETRY_DIRECTION_RIGHT, .third = GEOMETRY_DIRECTION_DOWN };
static const geometry_3dir_t PENCIL_BAD_H_PATTERN2
   = { .first = GEOMETRY_DIRECTION_UP,    .second = GEOMETRY_DIRECTION_LEFT,  .third = GEOMETRY_DIRECTION_UP };

void pencil_relationship_2d_layouter_private_select_solution ( pencil_relationship_2d_layouter_t *this_,
                                                               uint32_t solutions_count,
                                                               const geometry_connector_t solutions[],
                                                               uint32_t *out_index_of_best )
{
    TRACE_BEGIN();
    assert ( (*this_).sorted_rel_index < universal_array_index_sorter_get_count( &((*this_).sorted_relationships) ) );
    assert ( NULL != solutions );
    assert ( NULL != out_index_of_best );
    assert ( 1 <= solutions_count );

    /* get current relation data */
    const uint32_t index
        = universal_array_index_sorter_get_array_index ( &((*this_).sorted_relationships), (*this_).sorted_rel_index );
    const layout_relationship_t *const current_relation
        = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );
#if 0
    const data_relationship_t *const current_relation_data
        = layout_relationship_get_data_const ( current_relation );
#endif
    const geometry_rectangle_t *const source_rect
        = layout_relationship_get_from_symbol_box_const ( current_relation );
    const geometry_rectangle_t *const dest_rect
        = layout_relationship_get_to_symbol_box_const ( current_relation );
    const double src_center_x = geometry_rectangle_get_center_x ( source_rect );
    const double src_center_y = geometry_rectangle_get_center_y ( source_rect );
    const double dst_center_x = geometry_rectangle_get_center_x ( dest_rect );
    const double dst_center_y = geometry_rectangle_get_center_y ( dest_rect );

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );

    /* get preferred object distance */
    const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* define potential solution and rating */
    uint32_t index_of_best = 0;
    double debts_of_best = DBL_MAX;

    /* evaluate the solutions by their overlaps with classifiers */
    for ( uint32_t solution_idx = 0; solution_idx < solutions_count; solution_idx ++ )
    {
        /* evalute the debts of this solution */
        double debts_of_current = 0.0;
        const geometry_connector_t *const current_solution = &(solutions[solution_idx]);
        const geometry_rectangle_t connector_bounds
            = geometry_connector_get_bounding_rectangle( current_solution );

        /* avoid alternating solutions in case their debts are identical */
        debts_of_current += 0.1 * solution_idx;

        /* the more length, the more unwanted... */
        debts_of_current += geometry_connector_get_length( current_solution );

        /* prefer _either_ no _or_ minimum-dist lengths of parts... */
        const double HEAVIER_THAN_DETOUR = 4.0;
        const double source_length = geometry_connector_get_source_length( current_solution );
        if (( source_length > 0.000001 )&&( source_length < object_dist ))
        {
            debts_of_current += HEAVIER_THAN_DETOUR * ( object_dist - source_length );

        }
        const double destination_length = geometry_connector_get_destination_length( current_solution );
        if (( destination_length > 0.000001 )&&( destination_length < object_dist ))
        {
            debts_of_current += HEAVIER_THAN_DETOUR * ( object_dist - destination_length );
        }
        const bool no_source_or_no_dest = ( source_length < 0.000001 )||( destination_length < 0.000001 );
        const double main_length = geometry_connector_get_main_length( current_solution );
        if (( main_length > 0.000001 )&&( main_length < object_dist )&&( no_source_or_no_dest ))
        {
            debts_of_current += HEAVIER_THAN_DETOUR * ( object_dist - main_length );
        }

        /* prefer centered over uncentered departure and arrival */
        const double HEAVIER_THAN_CENTERED = 2.0;
        const double delta_source
            = fmin( fabs( geometry_connector_get_source_end_x( current_solution ) - src_center_x ),
            fabs( geometry_connector_get_source_end_y( current_solution ) - src_center_y ) );
        debts_of_current += delta_source * HEAVIER_THAN_CENTERED;
        const double delta_destination
            = fmin( fabs( geometry_connector_get_destination_end_x( current_solution ) - dst_center_x ),
            fabs( geometry_connector_get_destination_end_y( current_solution ) - dst_center_y ) );
        debts_of_current += delta_destination * HEAVIER_THAN_CENTERED;

        /* prefer left-hand angles over right-handed */
        bool bad_pattern_h = false;
        bool bad_pattern_v = false;
        {

            const geometry_3dir_t pattern = geometry_connector_get_directions( current_solution );
            bad_pattern_v = geometry_3dir_equals( &pattern, &PENCIL_BAD_V_PATTERN1 )
                || geometry_3dir_equals( &pattern, &PENCIL_BAD_V_PATTERN2 );
            bad_pattern_h = geometry_3dir_equals( &pattern, &PENCIL_BAD_H_PATTERN1 )
                || geometry_3dir_equals( &pattern, &PENCIL_BAD_H_PATTERN2 );
            if ( bad_pattern_h || bad_pattern_v )
            {
                const double current_len = geometry_connector_get_length( current_solution );
                if ( current_len > ( 4.0 * object_dist ) )
                {
                    /* current_solution is a long path and right-handed */
                    debts_of_current += 0.2 * geometry_connector_get_length( current_solution );
                }
            }
        }

        /* add debts for overlap to diagram boundary */
        {
            if ( ! geometry_rectangle_is_containing( diagram_draw_area, &connector_bounds ) )
            {
                debts_of_current += 1000000.0;
            }
        }

        /* iterate over all classifiers */
        const uint32_t count_clasfy
            = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
        for ( uint32_t clasfy_index = 0; clasfy_index < count_clasfy; clasfy_index ++ )
        {
            const layout_visible_classifier_t *const probe_classifier
                = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, clasfy_index );

            const geometry_rectangle_t *const classifier_space
                = layout_visible_classifier_get_space_const( probe_classifier );
            if ( ! geometry_rectangle_is_containing( classifier_space, &connector_bounds ) )
            {
                const geometry_rectangle_t *const classifier_symbol_box
                    = layout_visible_classifier_get_symbol_box_const( probe_classifier );
                if ( geometry_connector_is_intersecting_rectangle( current_solution, classifier_symbol_box ) )
                {
                    debts_of_current += 100000.0;
                }

                const geometry_rectangle_t *const classifier_label_box
                    = layout_visible_classifier_get_label_box_const( probe_classifier );
                if ( geometry_connector_is_intersecting_rectangle( current_solution, classifier_label_box ) )
                {
                    debts_of_current += 10000.0;
                }
            }
        }

        /* iterate over all features, check symbol boxes only, label boxes are not yet initialized */
        const uint32_t count_features
            = pencil_layout_data_get_feature_count ( (*this_).layout_data );
        for ( uint32_t f_idx = 0; f_idx < count_features; f_idx ++ )
        {
            const layout_feature_t *const feature_layout
                = pencil_layout_data_get_feature_ptr ( (*this_).layout_data, f_idx );

            const geometry_rectangle_t *const feature_symbol_box
                = layout_feature_get_symbol_box_const( feature_layout );
            if ( geometry_connector_is_intersecting_rectangle( current_solution, feature_symbol_box ) )
            {
                debts_of_current += 30000.0;
            }
        }

        /* iterate over the already created connectors (probe_sort_index < (*this_).sorted_rel_index) */
        for ( uint32_t probe_sort_index = 0; probe_sort_index < (*this_).sorted_rel_index; probe_sort_index ++ )
        {
            /* add debts if intersects */
            const uint32_t probe_index
                = universal_array_index_sorter_get_array_index( &((*this_).sorted_relationships), probe_sort_index );
            const layout_relationship_t *const probe_relationship
                = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, probe_index );
#if 0
            const data_relationship_t *const probe_relation_data
                = layout_relationship_get_data_const ( probe_relationship );
            const bool same_type = ( data_relationship_get_main_type( probe_relation_data )
                                     == data_relationship_get_main_type( current_relation_data ) );
            const bool same_from = ( data_relationship_get_from_classifier_row_id( probe_relation_data )
                                     == data_relationship_get_from_classifier_row_id( current_relation_data ) );
            const bool same_to = ( data_relationship_get_to_classifier_row_id( probe_relation_data )
                                     == data_relationship_get_to_classifier_row_id( current_relation_data ) );
            const bool one_same_end = ( same_from != same_to );
            /* if probe and current have same type and (same source classifier xor same destination classifier), overlaps are ok */
            if ( ! ( same_type && one_same_end ) )
#endif
            {
                const geometry_connector_t *const probe_shape
                    = layout_relationship_get_shape_const( probe_relationship );
                const uint32_t intersects
                    = geometry_connector_count_connector_intersects( current_solution, probe_shape );
                debts_of_current += 1000.0 * intersects;

                if ( ( bad_pattern_h || bad_pattern_v ) && ( intersects > 0 ) )
                {
                    const geometry_3dir_t probe_pattern = geometry_connector_get_directions( probe_shape );
                    const bool bad_probe_v = geometry_3dir_equals( &probe_pattern, &PENCIL_BAD_V_PATTERN1 )
                        || geometry_3dir_equals( &probe_pattern, &PENCIL_BAD_V_PATTERN2 );
                    const bool bad_probe_h = geometry_3dir_equals( &probe_pattern, &PENCIL_BAD_H_PATTERN1 )
                        || geometry_3dir_equals( &probe_pattern, &PENCIL_BAD_H_PATTERN2 );
                    if (( bad_pattern_h && bad_probe_v )||( bad_pattern_v && bad_probe_h ))
                    {
                         debts_of_current += 1000000.0;
                    }
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

    /* the best */
    *out_index_of_best = index_of_best;
    geometry_connector_trace( &(solutions[index_of_best]) );

    TRACE_END();
}

void pencil_relationship_2d_layouter_private_connect_rectangles_by_I ( pencil_relationship_2d_layouter_t *this_,
                                                                       const geometry_rectangle_t *source_rect,
                                                                       const geometry_rectangle_t *dest_rect,
                                                                       uint32_t solutions_max,
                                                                       geometry_connector_t out_solutions[],
                                                                       uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != source_rect );
    assert( NULL != dest_rect );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 4 <= solutions_max );  /* current implementation requires at least 4 options */

    uint32_t solutions_count = 0;

    const double src_left = geometry_rectangle_get_left(source_rect);
    const double src_right = geometry_rectangle_get_right(source_rect);
    const double src_top = geometry_rectangle_get_top(source_rect);
    const double src_bottom = geometry_rectangle_get_bottom(source_rect);

    const double dst_left = geometry_rectangle_get_left(dest_rect);
    const double dst_right = geometry_rectangle_get_right(dest_rect);
    const double dst_top = geometry_rectangle_get_top(dest_rect);
    const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

    const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    const double gap_dist = 0.499 * object_dist;  /* half the object distance allows a line to pass between two objects */

    /* if applicable, add a solution where line is vertical */
    if (( src_right >= dst_left )&&( src_left <= dst_right ))
    {
        const double min_left = fmax( src_left, dst_left );
        const double max_right = fmin( src_right, dst_right );

        if ( dst_bottom + object_dist < src_top )
        {
            /* define defaults */
            double x_value = ( min_left + max_right ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, min_left, dst_bottom, max_right, src_top );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
            geometry_rectangle_destroy( &search_rect );

            /* add solution */
            geometry_connector_reinit_vertical ( &(out_solutions[solutions_count]),
                                                 x_value,
                                                 src_top,
                                                 x_value,
                                                 dst_bottom,
                                                 x_value
                                               );
            solutions_count ++;
        }
        else if ( dst_top - object_dist > src_bottom )
        {
            /* define defaults */
            double x_value = ( min_left + max_right ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, min_left, dst_top, max_right, src_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
            geometry_rectangle_destroy( &search_rect );

            /* add solution */
            geometry_connector_reinit_vertical ( &(out_solutions[solutions_count]),
                                                 x_value,
                                                 src_bottom,
                                                 x_value,
                                                 dst_top,
                                                 x_value
                                               );
            solutions_count ++;
        }
        else
        {
            if ( fabs( src_top - dst_top ) > object_dist )
            {
                /* define defaults */
                double x_value = ( min_left + max_right ) / 2.0;

                /* optimize coordinates */
                geometry_rectangle_t search_rect;
                geometry_rectangle_init_by_corners( &search_rect, min_left, dst_top, max_right, src_top );
                pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
                geometry_rectangle_destroy( &search_rect );

                /* add solution */
                geometry_connector_reinit_vertical ( &(out_solutions[solutions_count]),
                                                    x_value,
                                                    src_top,
                                                    x_value,
                                                    dst_top,
                                                    x_value
                                                  );
                solutions_count ++;
            }

            if ( fabs( src_bottom - dst_bottom ) > object_dist )
            {
                /* define defaults */
                double x_value = ( min_left + max_right ) / 2.0;

                /* optimize coordinates */
                geometry_rectangle_t search_rect;
                geometry_rectangle_init_by_corners( &search_rect, min_left, src_bottom, max_right, dst_bottom );
                pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
                geometry_rectangle_destroy( &search_rect );

                /* add solution */
                geometry_connector_reinit_vertical ( &(out_solutions[solutions_count]),
                                                    x_value,
                                                    src_bottom,
                                                    x_value,
                                                    dst_bottom,
                                                    x_value
                                                  );
                solutions_count ++;
            }
        }
    }

    /* if applicable, add a solution where line is horizontal */
    if (( src_bottom >= dst_top )&&( src_top <= dst_bottom ))
    {
        const double min_top = fmax( src_top, dst_top );
        const double max_bottom = fmin( src_bottom, dst_bottom );

        if ( dst_right + object_dist < src_left )
        {
            /* define defaults */
            double y_value = ( min_top + max_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, dst_right, min_top, src_left, max_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
            geometry_rectangle_destroy( &search_rect );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   y_value,
                                                   dst_right,
                                                   y_value,
                                                   y_value
                                                 );
            solutions_count ++;
        }
        else if ( dst_left - object_dist > src_right )
        {
            /* define defaults */
            double y_value = ( min_top + max_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, dst_left, min_top, src_right, max_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
            geometry_rectangle_destroy( &search_rect );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   y_value,
                                                   dst_left,
                                                   y_value,
                                                   y_value
                                                 );
            solutions_count ++;
        }
        else
        {
            if ( fabs( src_left - dst_left ) > object_dist )
            {
                /* define defaults */
                double y_value = ( min_top + max_bottom ) / 2.0;

                /* optimize coordinates */
                geometry_rectangle_t search_rect;
                geometry_rectangle_init_by_corners( &search_rect, src_left, min_top, dst_left, max_bottom );
                pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
                geometry_rectangle_destroy( &search_rect );

                /* add solution */
                geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                       src_left,
                                                       y_value,
                                                       dst_left,
                                                       y_value,
                                                       y_value
                                                     );
                solutions_count ++;
            }

            if ( fabs( src_right - dst_right ) > object_dist )
            {
                /* define defaults */
                double y_value = ( min_top + max_bottom ) / 2.0;

                /* optimize coordinates */
                geometry_rectangle_t search_rect;
                geometry_rectangle_init_by_corners( &search_rect, src_right, min_top, dst_right, max_bottom );
                pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
                geometry_rectangle_destroy( &search_rect );

                /* add solution */
                geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                       src_right,
                                                       y_value,
                                                       dst_right,
                                                       y_value,
                                                       y_value
                                                     );
                solutions_count ++;
            }
        }
    }

    *out_solutions_count = solutions_count;

    TRACE_END();
}

void pencil_relationship_2d_layouter_private_connect_rectangles_by_ZN ( pencil_relationship_2d_layouter_t *this_,
                                                                        const geometry_rectangle_t *source_rect,
                                                                        const geometry_rectangle_t *dest_rect,
                                                                        uint32_t solutions_max,
                                                                        geometry_connector_t out_solutions[],
                                                                        uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != source_rect );
    assert( NULL != dest_rect );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 2 <= solutions_max );  /* current implementation requires at least 2 options */

    uint32_t solutions_count = 0;

    const double src_left = geometry_rectangle_get_left(source_rect);
    const double src_center_x = geometry_rectangle_get_center_x(source_rect);
    const double src_right = geometry_rectangle_get_right(source_rect);
    const double src_top = geometry_rectangle_get_top(source_rect);
    const double src_center_y = geometry_rectangle_get_center_y(source_rect);
    const double src_bottom = geometry_rectangle_get_bottom(source_rect);
    const double src_width = geometry_rectangle_get_width(source_rect);
    const double src_height = geometry_rectangle_get_height(source_rect);

    const double dst_left = geometry_rectangle_get_left(dest_rect);
    const double dst_center_x = geometry_rectangle_get_center_x(dest_rect);
    const double dst_right = geometry_rectangle_get_right(dest_rect);
    const double dst_top = geometry_rectangle_get_top(dest_rect);
    const double dst_center_y = geometry_rectangle_get_center_y(dest_rect);
    const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);
    const double dst_width = geometry_rectangle_get_width(dest_rect);
    const double dst_height = geometry_rectangle_get_height(dest_rect);

    const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    const double good_dist = 2.0 * object_dist;  /* duplicate distance: once for each side of the line */
    const double gap_dist = 0.499 * object_dist;  /* half the object distance allows a line to pass between two objects */

    /* if applicable, add a solution where main line is vertical */
    {
        if ( dst_right + good_dist < src_left )
        {
            /* define defaults */
            double x_value = ( src_left + dst_right ) / 2.0;
            double src_y = src_center_y;
            double dst_y = dst_center_y;

            /* optimize coordinates */
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, src_left, src_y, dst_right, dst_y );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
            geometry_rectangle_destroy( &search_rect );

            const geometry_rectangle_t depart_area
                = { .left=x_value, .top=src_top, .width=(src_left-x_value), .height=src_height};
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );

            const geometry_rectangle_t arrive_area
                = { .left=dst_right, .top=dst_top, .width=(x_value-dst_right), .height=dst_height};
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );

            /* add solution */
            geometry_connector_reinit_vertical ( &(out_solutions[solutions_count]),
                                                 src_left,
                                                 src_y,
                                                 dst_right,
                                                 dst_y,
                                                 x_value
                                               );
            solutions_count ++;
        }
        else if ( dst_left - good_dist > src_right )
        {
            /* define defaults */
            double x_value = ( src_right + dst_left ) / 2.0;
            double src_y = src_center_y;
            double dst_y = dst_center_y;

            /* optimize coordinates */
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, src_right, src_y, dst_left, dst_y );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
            geometry_rectangle_destroy( &search_rect );

            const geometry_rectangle_t depart_area
                = { .left=src_right, .top=src_top, .width=(x_value-src_right), .height=src_height};
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );

            const geometry_rectangle_t arrive_area
                = { .left=x_value, .top=dst_top, .width=(dst_left-x_value), .height=dst_height};
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );

            /* add solution */
            geometry_connector_reinit_vertical ( &(out_solutions[solutions_count]),
                                                 src_right,
                                                 src_y,
                                                 dst_left,
                                                 dst_y,
                                                 x_value
                                               );
            solutions_count ++;
        }
    }

    /* if applicable, add a solution where main line is horizontal */
    {
        if ( dst_bottom + good_dist < src_top )
        {
            /* define defaults */
            double y_value = ( src_top + dst_bottom ) / 2.0;
            double src_x = src_center_x;
            double dst_x = dst_center_x;

            /* optimize coordinates */
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, src_x, src_top, dst_x, dst_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
            geometry_rectangle_destroy( &search_rect );

            const geometry_rectangle_t depart_area
                = { .left=src_left, .top=y_value, .width=src_width, .height=(src_top-y_value)};
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );

            const geometry_rectangle_t arrive_area
                = { .left=dst_left, .top=dst_bottom, .width=dst_width, .height=(y_value-dst_bottom)};
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x,
                                                   src_top,
                                                   dst_x,
                                                   dst_bottom,
                                                   y_value
                                                 );
            solutions_count ++;
        }
        else if ( dst_top - good_dist > src_bottom )
        {
            /* define defaults */
            double y_value = ( src_bottom + dst_top ) / 2.0;
            double src_x = src_center_x;
            double dst_x = dst_center_x;

            /* optimize coordinates */
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, src_x, src_bottom, dst_x, dst_top );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
            geometry_rectangle_destroy( &search_rect );

            const geometry_rectangle_t depart_area
                = { .left=src_left, .top=src_bottom, .width=src_width, .height=(y_value-src_bottom)};
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );

            const geometry_rectangle_t arrive_area
                = { .left=dst_left, .top=y_value, .width=dst_width, .height=(dst_top-y_value)};
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x,
                                                   src_bottom,
                                                   dst_x,
                                                   dst_top,
                                                   y_value
                                                 );
            solutions_count ++;
        }
    }

    *out_solutions_count = solutions_count;

    TRACE_END();
}

void pencil_relationship_2d_layouter_private_connect_rectangles_by_UC ( pencil_relationship_2d_layouter_t *this_,
                                                                        const geometry_rectangle_t *source_rect,
                                                                        const geometry_rectangle_t *dest_rect,
                                                                        uint32_t solutions_max,
                                                                        geometry_connector_t out_solutions[],
                                                                        uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != source_rect );
    assert( NULL != dest_rect );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 4 <= solutions_max );  /* current implementation calculates exactly 4 options */

    uint32_t solutions_count = 0;

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );
    const double draw_left = geometry_rectangle_get_left( diagram_draw_area );
    const double draw_right = geometry_rectangle_get_right( diagram_draw_area );
    const double draw_top = geometry_rectangle_get_top( diagram_draw_area );
    const double draw_bottom = geometry_rectangle_get_bottom( diagram_draw_area );

    const double src_left = geometry_rectangle_get_left(source_rect);
    const double src_center_x = geometry_rectangle_get_center_x(source_rect);
    const double src_right = geometry_rectangle_get_right(source_rect);
    const double src_top = geometry_rectangle_get_top(source_rect);
    const double src_center_y = geometry_rectangle_get_center_y(source_rect);
    const double src_bottom = geometry_rectangle_get_bottom(source_rect);
    const double src_width = geometry_rectangle_get_width(source_rect);
    const double src_height = geometry_rectangle_get_height(source_rect);

    const double dst_left = geometry_rectangle_get_left(dest_rect);
    const double dst_center_x = geometry_rectangle_get_center_x(dest_rect);
    const double dst_right = geometry_rectangle_get_right(dest_rect);
    const double dst_top = geometry_rectangle_get_top(dest_rect);
    const double dst_center_y = geometry_rectangle_get_center_y(dest_rect);
    const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);
    const double dst_width = geometry_rectangle_get_width(dest_rect);
    const double dst_height = geometry_rectangle_get_height(dest_rect);

    const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    const double gap_dist = 0.499 * object_dist;  /* half the object distance allows a line to pass between two objects */
    static const double NO_TOUCH = 0.0001;

    /* connect via left side */
    {
        /* define defaults */
        double x_value = fmin( src_left, dst_left ) - object_dist;
        double src_y = src_center_y;
        double dst_y = dst_center_y;
        if ( fabs( src_center_y - dst_center_y ) < NO_TOUCH )
        {
            /* forward way is identical to retour - may be a relation to self */
            src_y = fmin( src_center_y + gap_dist, src_bottom );
            dst_y = fmax( dst_center_y - gap_dist, dst_top );
        }

        /* optimize coordinates */
        geometry_rectangle_t search_rect;
        geometry_rectangle_init_by_corners( &search_rect, draw_left, src_y, x_value, dst_y );
        pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
        geometry_rectangle_destroy( &search_rect );

        const geometry_rectangle_t depart_area
            = { .left=x_value, .top=src_top, .width=(src_left-x_value), .height=src_height};
        pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );

        const geometry_rectangle_t arrive_area
            = { .left=x_value, .top=dst_top, .width=(dst_left-x_value), .height=dst_height};
        pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );

        /* add solution */
        geometry_connector_reinit_vertical( &(out_solutions[solutions_count]),
                                            src_left,
                                            src_y,
                                            dst_left,
                                            dst_y,
                                            x_value
                                          );
        solutions_count ++;
    }

    /* connect via right side */
    {
        /* define defaults */
        double x_value = fmax( src_right, dst_right ) + object_dist;
        double src_y = src_center_y;
        double dst_y = dst_center_y;
        if ( fabs( src_center_y - dst_center_y ) < NO_TOUCH )
        {
            /* forward way is identical to retour - may be a relation to self */
            src_y = fmin( src_center_y + gap_dist, src_bottom );
            dst_y = fmax( dst_center_y - gap_dist, dst_top );
        }

        /* optimize coordinates */
        geometry_rectangle_t search_rect;
        geometry_rectangle_init_by_corners( &search_rect, x_value, src_y, draw_right, dst_y );
        pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
        geometry_rectangle_destroy( &search_rect );

        const geometry_rectangle_t depart_area
            = { .left=src_right, .top=src_top, .width=(x_value-src_right), .height=src_height};
        pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );

        const geometry_rectangle_t arrive_area
            = { .left=dst_right, .top=dst_top, .width=(x_value-dst_right), .height=dst_height};
        pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );

        /* add solution */
        geometry_connector_reinit_vertical( &(out_solutions[solutions_count]),
                                            src_right,
                                            src_y,
                                            dst_right,
                                            dst_y,
                                            x_value
                                          );
        solutions_count ++;
    }

    /* connect via top side */
    {
        /* define defaults */
        double y_value = fmin( src_top, dst_top ) - object_dist;
        double src_x = src_center_x;
        double dst_x = dst_center_x;
        if ( fabs( src_center_x - dst_center_x ) < NO_TOUCH )
        {
            /* forward way is identical to retour - may be a relation to self */
            src_x = fmax( src_center_x - gap_dist, src_left );
            dst_x = fmin( dst_center_x + gap_dist, dst_right );
        }

        /* optimize coordinates */
        geometry_rectangle_t search_rect;
        geometry_rectangle_init_by_corners( &search_rect, src_x, draw_top, dst_x, y_value );
        pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
        geometry_rectangle_destroy( &search_rect );

        const geometry_rectangle_t depart_area
            = { .left=src_left, .top=y_value, .width=src_width, .height=(src_top-y_value)};
        pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );

        const geometry_rectangle_t arrive_area
            = { .left=dst_left, .top=y_value, .width=dst_width, .height=(dst_top-y_value)};
        pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );

        /* add solution */
        geometry_connector_reinit_horizontal( &(out_solutions[solutions_count]),
                                              src_x,
                                              src_top,
                                              dst_x,
                                              dst_top,
                                              y_value
                                            );
        solutions_count ++;
    }

    /* connect via bottom side */
    {
        /* define defaults */
        double y_value = fmax( src_bottom, dst_bottom ) + object_dist;
        double src_x = src_center_x;
        double dst_x = dst_center_x;
        if ( fabs( src_center_x - dst_center_x ) < NO_TOUCH )
        {
            /* forward way is identical to retour - may be a relation to self */
            src_x = fmax( src_center_x - gap_dist, src_left );
            dst_x = fmin( dst_center_x + gap_dist, dst_right );
        }

        /* optimize coordinates */
        geometry_rectangle_t search_rect;
        geometry_rectangle_init_by_corners( &search_rect, src_x, y_value, dst_x, draw_bottom );
        pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
        geometry_rectangle_destroy( &search_rect );

        const geometry_rectangle_t depart_area
            = { .left=src_left, .top=src_bottom, .width=src_width, .height=(y_value-src_bottom)};
        pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );

        const geometry_rectangle_t arrive_area
            = { .left=dst_left, .top=dst_bottom, .width=dst_width, .height=(y_value-dst_bottom)};
        pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );

        /* add solution */
        geometry_connector_reinit_horizontal( &(out_solutions[solutions_count]),
                                              src_x,
                                              src_bottom,
                                              dst_x,
                                              dst_bottom,
                                              y_value
                                            );
        solutions_count ++;
    }

    *out_solutions_count = solutions_count;

    TRACE_END();
}

void pencil_relationship_2d_layouter_private_connect_rectangles_by_L7 ( pencil_relationship_2d_layouter_t *this_,
                                                                        const geometry_rectangle_t *source_rect,
                                                                        const geometry_rectangle_t *dest_rect,
                                                                        uint32_t solutions_max,
                                                                        geometry_connector_t out_solutions[],
                                                                        uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != source_rect );
    assert( NULL != dest_rect );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 8 <= solutions_max );  /* current implementation requires at least 2 options */

    uint32_t solutions_count = 0;

    const double src_left = geometry_rectangle_get_left(source_rect);
    const double src_right = geometry_rectangle_get_right(source_rect);
    const double src_top = geometry_rectangle_get_top(source_rect);
    const double src_bottom = geometry_rectangle_get_bottom(source_rect);

    const double dst_left = geometry_rectangle_get_left(dest_rect);
    const double dst_right = geometry_rectangle_get_right(dest_rect);
    const double dst_top = geometry_rectangle_get_top(dest_rect);
    const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

    const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    const double gap_dist = 0.499 * object_dist;  /* half the object distance allows a line to pass between two objects */

    /* pre-calculate some intermediate values on source rect */
    const bool src_left_to_outside = dst_left < src_left;  /* connector starts towards outside of source rect */
    const bool src_right_to_outside = dst_right > src_right;  /* connector starts towards outside of source rect */
    const bool src_top_to_outside = src_top > dst_top;  /* connector arrives from outside at dest rect */
    const bool src_bottom_to_outside = src_bottom < dst_bottom;  /* connector arrives from outside at dest rect */
    /* pre-calculate some intermediate values on destination rect */
    const bool dst_left_from_outside = src_left < dst_left;  /* connector starts towards outside of source rect */
    const bool dst_right_from_outside = src_right > dst_right;  /* connector starts towards outside of source rect */
    const bool dst_top_from_outside = dst_top > src_top;  /* connector arrives from outside at dest rect */
    const bool dst_bottom_from_outside = dst_bottom < src_bottom;  /* connector arrives from outside at dest rect */

    /* add two solutions from source-left */
    {
        const double search_left = dst_left;
        const double search_right = src_left_to_outside ? fmin( src_left, dst_right ) : dst_right;
        /* add a solution from source-left to destination-bottom */
        {
            const double search_top = dst_bottom_from_outside ? fmax( src_top, dst_bottom ) : src_top;
            const double search_bottom = src_bottom;

            /* define defaults */
            double dst_x = ( search_left + search_right ) / 2.0;
            double src_y = ( search_top + search_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t depart_area;
            geometry_rectangle_init_by_corners( &depart_area, dst_x, src_top, src_left, src_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );
            geometry_rectangle_destroy( &depart_area );

            geometry_rectangle_t arrive_area;
            geometry_rectangle_init_by_corners( &arrive_area, dst_left, src_y, dst_right, dst_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );
            geometry_rectangle_destroy( &arrive_area );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   src_y,
                                                   dst_x,
                                                   dst_bottom,
                                                   src_y
                                                 );
            solutions_count ++;
        }
        /* add a solution from source-left to destination-top */
        {
            const double search_top = src_top;
            const double search_bottom = dst_top_from_outside ? fmin( src_bottom, dst_top ) : src_bottom;

            /* define defaults */
            double dst_x = ( search_left + search_right ) / 2.0;
            double src_y = ( search_top + search_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t depart_area;
            geometry_rectangle_init_by_corners( &depart_area, dst_x, src_top, src_left, src_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );
            geometry_rectangle_destroy( &depart_area );

            geometry_rectangle_t arrive_area;
            geometry_rectangle_init_by_corners( &arrive_area, dst_left, src_y, dst_right, dst_top );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );
            geometry_rectangle_destroy( &arrive_area );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   src_y,
                                                   dst_x,
                                                   dst_top,
                                                   src_y
                                                 );
            solutions_count ++;
        }
    }

    /* add two solutions from source-right */
    {
        const double search_left = src_right_to_outside ? fmax( src_right, dst_left ) : dst_left;
        const double search_right = dst_right;
        /* add a solution from source-right to destination-bottom */
        {
            const double search_top = dst_bottom_from_outside ? fmax( src_top, dst_bottom ) : src_top;
            const double search_bottom = src_bottom;

            /* define defaults */
            double dst_x = ( search_left + search_right ) / 2.0;
            double src_y = ( search_top + search_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t depart_area;
            geometry_rectangle_init_by_corners( &depart_area, dst_x, src_top, src_right, src_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );
            geometry_rectangle_destroy( &depart_area );

            geometry_rectangle_t arrive_area;
            geometry_rectangle_init_by_corners( &arrive_area, dst_left, src_y, dst_right, dst_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );
            geometry_rectangle_destroy( &arrive_area );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   src_y,
                                                   dst_x,
                                                   dst_bottom,
                                                   src_y
                                                 );
            solutions_count ++;
        }
        /* add a solution from source-right to destination-top */
        {
            const double search_top = src_top;
            const double search_bottom = dst_top_from_outside ? fmin( src_bottom, dst_top ) : src_bottom;

            /* define defaults */
            double dst_x = ( search_left + search_right ) / 2.0;
            double src_y = ( search_top + search_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t depart_area;
            geometry_rectangle_init_by_corners( &depart_area, dst_x, src_top, src_right, src_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );
            geometry_rectangle_destroy( &depart_area );

            geometry_rectangle_t arrive_area;
            geometry_rectangle_init_by_corners( &arrive_area, dst_left, src_y, dst_right, dst_top );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );
            geometry_rectangle_destroy( &arrive_area );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   src_y,
                                                   dst_x,
                                                   dst_top,
                                                   src_y
                                                 );
            solutions_count ++;
        }
    }

    /* add two solutions from source-top */
    {
        const double search_top = dst_top;
        const double search_bottom = src_top_to_outside ? fmin( src_top, dst_bottom ) : dst_bottom;
        /* add a solution from source-top to destination-right */
        {
            const double search_left = dst_right_from_outside ? fmax( dst_right, src_left ) : src_left;
            const double search_right = src_right;

            /* define defaults */
            double src_x = ( search_left + search_right ) / 2.0;
            double dst_y = ( search_top + search_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t depart_area;
            geometry_rectangle_init_by_corners( &depart_area, src_left, src_top, src_right, dst_y );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );
            geometry_rectangle_destroy( &depart_area );

            geometry_rectangle_t arrive_area;
            geometry_rectangle_init_by_corners( &arrive_area, src_x, dst_top, dst_right, dst_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );
            geometry_rectangle_destroy( &arrive_area );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x,
                                                   src_top,
                                                   dst_right,
                                                   dst_y,
                                                   dst_y
                                                 );
            solutions_count ++;
        }
        /* add a solution from source-top to destination-left */
        {
            const double search_left = src_left;
            const double search_right = dst_left_from_outside ? fmin( dst_left, src_right ) : src_right;

            /* define defaults */
            double src_x = ( search_left + search_right ) / 2.0;
            double dst_y = ( search_top + search_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t depart_area;
            geometry_rectangle_init_by_corners( &depart_area, src_left, src_top, src_right, dst_y );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );
            geometry_rectangle_destroy( &depart_area );

            geometry_rectangle_t arrive_area;
            geometry_rectangle_init_by_corners( &arrive_area, src_x, dst_top, dst_left, dst_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );
            geometry_rectangle_destroy( &arrive_area );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x,
                                                   src_top,
                                                   dst_left,
                                                   dst_y,
                                                   dst_y
                                                 );
            solutions_count ++;
        }
    }

    /* add two solutions from source-bottom */
    {
        const double search_top = src_bottom_to_outside ? fmax( src_bottom, dst_top ) : dst_top;
        const double search_bottom = dst_bottom;
        /* add a solution from source-bottom to destination-right */
        {
            const double search_left = dst_right_from_outside ? fmax( dst_right, src_left ) : src_left;
            const double search_right = src_right;

            /* define defaults */
            double src_x = ( search_left + search_right ) / 2.0;
            double dst_y = ( search_top + search_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t depart_area;
            geometry_rectangle_init_by_corners( &depart_area, src_left, src_bottom, src_right, dst_y );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );
            geometry_rectangle_destroy( &depart_area );

            geometry_rectangle_t arrive_area;
            geometry_rectangle_init_by_corners( &arrive_area, src_x, dst_top, dst_right, dst_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );
            geometry_rectangle_destroy( &arrive_area );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x,
                                                   src_bottom,
                                                   dst_right,
                                                   dst_y,
                                                   dst_y
                                                 );
            solutions_count ++;
        }
        /* add a solution from source-bottom to destination-left */
        {
            const double search_left = src_left;
            const double search_right = dst_left_from_outside ? fmin( dst_left, src_right ) : src_right;

            /* define defaults */
            double src_x = ( search_left + search_right ) / 2.0;
            double dst_y = ( search_top + search_bottom ) / 2.0;

            /* optimize coordinates */
            geometry_rectangle_t depart_area;
            geometry_rectangle_init_by_corners( &depart_area, src_left, src_bottom, src_right, dst_y );
            pencil_relationship_2d_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );
            geometry_rectangle_destroy( &depart_area );

            geometry_rectangle_t arrive_area;
            geometry_rectangle_init_by_corners( &arrive_area, src_x, dst_top, dst_left, dst_bottom );
            pencil_relationship_2d_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );
            geometry_rectangle_destroy( &arrive_area );

            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x,
                                                   src_bottom,
                                                   dst_left,
                                                   dst_y,
                                                   dst_y
                                                 );
            solutions_count ++;
        }
    }

    *out_solutions_count = solutions_count;

    TRACE_END();
}

u8_error_t pencil_relationship_2d_layouter_private_find_space_for_line ( pencil_relationship_2d_layouter_t *this_,
                                                                         const geometry_rectangle_t *search_rect,
                                                                         bool horizontal_line,
                                                                         double min_gap,
                                                                         double *io_coordinate )
{
    TRACE_BEGIN();
    assert( (*this_).sorted_rel_index < universal_array_index_sorter_get_count( &((*this_).sorted_relationships) ) );
    assert ( NULL != search_rect );
    assert ( NULL != io_coordinate );
    u8_error_t err = U8_ERROR_NONE;

    /* start two probes at the center and move these to the boundaries when discovering overlaps */
    const double center = *io_coordinate;
    if ( horizontal_line )
    {
        assert( center > geometry_rectangle_get_top( search_rect ) - 0.000000001 );
        assert( center < geometry_rectangle_get_bottom( search_rect ) + 0.000000001 );
    }
    else
    {
        assert( center > geometry_rectangle_get_left( search_rect ) - 0.000000001 );
        assert( center < geometry_rectangle_get_right( search_rect ) + 0.000000001 );
    }
    double good_smaller = center;  /* a coordinate top/left of major obstacles */
    double good_greater = center;  /* a coordinate bottom/right of major obstacles */
    double best_smaller = center;  /* a coordinate top/left of any obstacle */
    double best_greater = center;  /* a coordinate bottom/right of any obstacle */

    /* the rectangle where each classifier within is checked for intersections: */
    geometry_rectangle_t consider_rect;
    geometry_rectangle_copy( &consider_rect, search_rect );
    if ( horizontal_line )
    {
        geometry_rectangle_set_top( &consider_rect, geometry_rectangle_get_top( search_rect ) - min_gap );
        geometry_rectangle_set_height( &consider_rect, geometry_rectangle_get_height( search_rect ) + 2.0 * min_gap );

    }
    else
    {
        geometry_rectangle_set_left( &consider_rect, geometry_rectangle_get_left( search_rect ) - min_gap );
        geometry_rectangle_set_width( &consider_rect, geometry_rectangle_get_width( search_rect ) + 2.0 * min_gap );
    }

    /* iterate till no hit anymore */
    const uint32_t max_list_iteration = 8;  /* in any case, do not iterate ofer the list more than 8 times */
    bool hit = true;  /* whenever the probes hit a rectangle, hit is set to true */
    for ( uint32_t list_iteration = 0; (list_iteration < max_list_iteration) && hit; list_iteration ++ )
    {
        hit = false;

        /* move away from classifiers */
        const uint32_t count_classifiers = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
        for ( uint32_t classifier_index = 0; classifier_index < count_classifiers; classifier_index ++ )
        {
            const layout_visible_classifier_t *const the_classifier
                = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, classifier_index );

            const geometry_rectangle_t *const classifier_symbol_box
                = layout_visible_classifier_get_symbol_box_const( the_classifier );
            const geometry_rectangle_t *const classifier_space
                = layout_visible_classifier_get_space_const( the_classifier );
            /* Note: This algorithm ignores if the current classifier is parent container of source or destination */
            if ( geometry_rectangle_is_intersecting( &consider_rect, classifier_symbol_box ) )
            {
                const double clas_symbol_box_smaller
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_top(classifier_symbol_box) - min_gap )
                    : ( geometry_rectangle_get_left(classifier_symbol_box) - min_gap );
                const double clas_symbol_box_greater
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_bottom(classifier_symbol_box) + min_gap )
                    : ( geometry_rectangle_get_right(classifier_symbol_box) + min_gap );

                const geometry_rectangle_t good_smaller_rect
                    = { .left = horizontal_line ? geometry_rectangle_get_left( search_rect ) : good_smaller,
                        .top = horizontal_line ? good_smaller : geometry_rectangle_get_top( search_rect ),
                        .width = horizontal_line ? geometry_rectangle_get_width( search_rect ) : 0.0,
                        .height = horizontal_line ? 0.0 : geometry_rectangle_get_height( search_rect )
                    };
                if ( ( ! geometry_rectangle_is_containing( classifier_space, &good_smaller_rect ) )
                    && ( clas_symbol_box_smaller < good_smaller ) && ( good_smaller < clas_symbol_box_greater ) )
                {
                    good_smaller = clas_symbol_box_smaller;
                    hit = true;
                }
                const geometry_rectangle_t good_greater_rect
                    = { .left = horizontal_line ? geometry_rectangle_get_left( search_rect ) : good_greater,
                        .top = horizontal_line ? good_greater : geometry_rectangle_get_top( search_rect ),
                        .width = horizontal_line ? geometry_rectangle_get_width( search_rect ) : 0.0,
                        .height = horizontal_line ? 0.0 : geometry_rectangle_get_height( search_rect )
                    };
                if ( ( ! geometry_rectangle_is_containing( classifier_space, &good_greater_rect ) )
                    && ( clas_symbol_box_smaller < good_greater ) && ( good_greater < clas_symbol_box_greater ) )
                {
                    good_greater = clas_symbol_box_greater;
                    hit = true;
                }
                const geometry_rectangle_t best_smaller_rect
                    = { .left = horizontal_line ? geometry_rectangle_get_left( search_rect ) : best_smaller,
                        .top = horizontal_line ? best_smaller : geometry_rectangle_get_top( search_rect ),
                        .width = horizontal_line ? geometry_rectangle_get_width( search_rect ) : 0.0,
                        .height = horizontal_line ? 0.0 : geometry_rectangle_get_height( search_rect )
                    };
                if ( ( ! geometry_rectangle_is_containing( classifier_space, &best_smaller_rect ) )
                    && ( clas_symbol_box_smaller < best_smaller ) && ( best_smaller < clas_symbol_box_greater ) )
                {
                    best_smaller = clas_symbol_box_smaller;
                    hit = true;
                }
                const geometry_rectangle_t best_greater_rect
                    = { .left = horizontal_line ? geometry_rectangle_get_left( search_rect ) : best_greater,
                        .top = horizontal_line ? best_greater : geometry_rectangle_get_top( search_rect ),
                        .width = horizontal_line ? geometry_rectangle_get_width( search_rect ) : 0.0,
                        .height = horizontal_line ? 0.0 : geometry_rectangle_get_height( search_rect )
                    };
                if ( ( ! geometry_rectangle_is_containing( classifier_space, &best_greater_rect ) )
                    && ( clas_symbol_box_smaller < best_greater ) && ( best_greater < clas_symbol_box_greater ) )
                {
                    best_greater = clas_symbol_box_greater;
                    hit = true;
                }
            }

            const geometry_rectangle_t *const classifier_label_box
                = layout_visible_classifier_get_label_box_const( the_classifier );
            if ( geometry_rectangle_is_intersecting( &consider_rect, classifier_label_box ) )
            {
                const double clas_label_smaller
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_top(classifier_label_box) - min_gap )
                    : ( geometry_rectangle_get_left(classifier_label_box) - min_gap );
                const double clas_label_greater
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_bottom(classifier_label_box) + min_gap )
                    : ( geometry_rectangle_get_right(classifier_label_box) + min_gap );

                if ( ( clas_label_smaller < good_smaller ) && ( good_smaller < clas_label_greater ) )
                {
                    good_smaller = clas_label_smaller;
                    hit = true;
                }
                if ( ( clas_label_smaller < good_greater ) && ( good_greater < clas_label_greater ) )
                {
                    good_greater = clas_label_greater;
                    hit = true;
                }
                if ( ( clas_label_smaller < best_smaller ) && ( best_smaller < clas_label_greater ) )
                {
                    best_smaller = clas_label_smaller;
                    hit = true;
                }
                if ( ( clas_label_smaller < best_greater ) && ( best_greater < clas_label_greater ) )
                {
                    best_greater = clas_label_greater;
                    hit = true;
                }
            }
        }

        /* move away from features, check symbol boxes only, label boxes are not yet initialized */
        const uint32_t count_features = pencil_layout_data_get_feature_count ( (*this_).layout_data );
        for ( uint32_t f_idx = 0; f_idx < count_features; f_idx ++ )
        {
            const layout_feature_t *const feature_layout
                = pencil_layout_data_get_feature_ptr ( (*this_).layout_data, f_idx );

            const geometry_rectangle_t *const feature_symbol_box
                = layout_feature_get_symbol_box_const( feature_layout );
            if ( geometry_rectangle_is_intersecting( &consider_rect, feature_symbol_box ) )
            {
                const double feature_smaller
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_top(feature_symbol_box) - min_gap )
                    : ( geometry_rectangle_get_left(feature_symbol_box) - min_gap );
                const double feature_greater
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_bottom(feature_symbol_box) + min_gap )
                    : ( geometry_rectangle_get_right(feature_symbol_box) + min_gap );

                if ( ( feature_smaller < good_smaller ) && ( good_smaller < feature_greater ) )
                {
                    good_smaller = feature_smaller;
                    hit = true;
                }
                if ( ( feature_smaller < good_greater ) && ( good_greater < feature_greater ) )
                {
                    good_greater = feature_greater;
                    hit = true;
                }
                if ( ( feature_smaller < best_smaller ) && ( best_smaller < feature_greater ) )
                {
                    best_smaller = feature_smaller;
                    hit = true;
                }
                if ( ( feature_smaller < best_greater ) && ( best_greater < feature_greater ) )
                {
                    best_greater = feature_greater;
                    hit = true;
                }
            }
        }

        /* move away from already layed-out parallel relationship-segments; */
        /* already done: exist_sort_index < (*this_).sorted_rel_index */
        for ( uint32_t exist_sort_index = 0; ( exist_sort_index < (*this_).sorted_rel_index ); exist_sort_index ++ )
        {
            const uint32_t exist_index
                = universal_array_index_sorter_get_array_index( &((*this_).sorted_relationships), exist_sort_index );
            const layout_relationship_t *const exist_relationship
                = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, exist_index );
            /* Note: This algorithm ignores the relationship types (same_type), sources and destinations (one_same_end) */

            const geometry_connector_t *const exist_shape = layout_relationship_get_shape_const( exist_relationship );
            if ( geometry_connector_is_intersecting_rectangle( exist_shape, &consider_rect ) )
            {
                const geometry_rectangle_t seg_1
                    = geometry_connector_get_segment_bounds( exist_shape, GEOMETRY_CONNECTOR_SEGMENT_SOURCE );
                const bool seg_1_is_intersecting
                    = geometry_rectangle_is_intersecting( &seg_1, &consider_rect );
                const double seg_1_smaller
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_top( &seg_1 ) - min_gap )
                    : ( geometry_rectangle_get_left( &seg_1 ) - min_gap );
                const double seg_1_greater
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_bottom( &seg_1 ) + min_gap )
                    : ( geometry_rectangle_get_right( &seg_1 ) + min_gap );
                if ( seg_1_is_intersecting
                    && ( seg_1_smaller < best_smaller ) && ( best_smaller < seg_1_greater ) )
                {
                    best_smaller = seg_1_smaller;
                    hit = true;
                }
                if ( seg_1_is_intersecting
                    && ( seg_1_smaller < best_greater ) && ( best_greater < seg_1_greater ) )
                {
                    best_greater = seg_1_greater;
                    hit = true;
                }
                const geometry_rectangle_t seg_2
                    = geometry_connector_get_segment_bounds( exist_shape, GEOMETRY_CONNECTOR_SEGMENT_MAIN );
                const bool seg_2_is_intersecting
                    = geometry_rectangle_is_intersecting( &seg_2, &consider_rect );
                const double seg_2_smaller
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_top( &seg_2 ) - min_gap )
                    : ( geometry_rectangle_get_left( &seg_2 ) - min_gap );
                const double seg_2_greater
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_bottom( &seg_2 ) + min_gap )
                    : ( geometry_rectangle_get_right( &seg_2 ) + min_gap );
                if ( seg_2_is_intersecting
                    && ( seg_2_smaller < best_smaller ) && ( best_smaller < seg_2_greater ) )
                {
                    best_smaller = seg_2_smaller;
                    hit = true;
                }
                if ( seg_2_is_intersecting
                    && ( seg_2_smaller < best_greater ) && ( best_greater < seg_2_greater ) )
                {
                    best_greater = seg_2_greater;
                    hit = true;
                }
                const geometry_rectangle_t seg_3
                    = geometry_connector_get_segment_bounds( exist_shape, GEOMETRY_CONNECTOR_SEGMENT_DESTINATION );
                const bool seg_3_is_intersecting
                    = geometry_rectangle_is_intersecting( &seg_3, &consider_rect );
                const double seg_3_smaller
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_top( &seg_3 ) - min_gap )
                    : ( geometry_rectangle_get_left( &seg_3 ) - min_gap );
                const double seg_3_greater
                    = horizontal_line /* do vertical search if line is horizontal */
                    ? ( geometry_rectangle_get_bottom( &seg_3 ) + min_gap )
                    : ( geometry_rectangle_get_right( &seg_3 ) + min_gap );
                if ( seg_3_is_intersecting
                    && ( seg_3_smaller < best_smaller ) && ( best_smaller < seg_3_greater ) )
                {
                    best_smaller = seg_3_smaller;
                    hit = true;
                }
                if ( seg_3_is_intersecting
                    && ( seg_3_smaller < best_greater ) && ( best_greater < seg_3_greater ) )
                {
                    best_greater = seg_3_greater;
                    hit = true;
                }

                const geometry_3dir_t exist_dirs = geometry_connector_get_directions( exist_shape );
                if ( horizontal_line )
                {
                    const double exist_source_y = geometry_connector_get_main_line_source_y ( exist_shape );
                    const double exist_destination_y = geometry_connector_get_main_line_destination_y ( exist_shape );
                    if ( geometry_3dir_is_first_h( &exist_dirs ) || geometry_3dir_is_second_h( &exist_dirs ) )
                    {
                        if (( exist_source_y - min_gap < good_smaller )&&( good_smaller < exist_source_y + min_gap ))
                        {
                            good_smaller = exist_source_y - min_gap;
                            hit = true;
                        }
                        if (( exist_source_y - min_gap < good_greater )&&( good_greater < exist_source_y + min_gap ))
                        {
                            good_greater = exist_source_y + min_gap;
                            hit = true;
                        }
                    }
                    if ( geometry_3dir_is_third_h( &exist_dirs ) ) /* third segment only, second is already evaluated above */
                    {
                        if (( exist_destination_y - min_gap < good_smaller )&&( good_smaller < exist_destination_y + min_gap ))
                        {
                            good_smaller = exist_destination_y - min_gap;
                            hit = true;
                        }
                        if (( exist_destination_y - min_gap < good_greater )&&( good_greater < exist_destination_y + min_gap ))
                        {
                            good_greater = exist_destination_y + min_gap;
                            hit = true;
                        }
                    }
                }
                else
                {
                    const double exist_source_x = geometry_connector_get_main_line_source_x ( exist_shape );
                    const double exist_destination_x = geometry_connector_get_main_line_destination_x ( exist_shape );
                    if ( geometry_3dir_is_first_v( &exist_dirs ) || geometry_3dir_is_second_v( &exist_dirs ) )
                    {
                        if (( exist_source_x - min_gap < good_smaller )&&( good_smaller < exist_source_x + min_gap ))
                        {
                            good_smaller = exist_source_x - min_gap;
                            hit = true;
                        }
                        if (( exist_source_x - min_gap < good_greater )&&( good_greater < exist_source_x + min_gap ))
                        {
                            good_greater = exist_source_x + min_gap;
                            hit = true;
                        }
                    }
                    if ( geometry_3dir_is_third_v( &exist_dirs ) ) /* third segment only, second is already evaluated above */
                    {
                        if (( exist_destination_x - min_gap < good_smaller )&&( good_smaller < exist_destination_x + min_gap ))
                        {
                            good_smaller = exist_destination_x - min_gap;
                            hit = true;
                        }
                        if (( exist_destination_x - min_gap < good_greater )&&( good_greater < exist_destination_x + min_gap ))
                        {
                            good_greater = exist_destination_x + min_gap;
                            hit = true;
                        }
                    }
                }
            }
        }
    }

    /* check success */
    const double minimum_result
        = horizontal_line
        ? geometry_rectangle_get_top( search_rect )
        : geometry_rectangle_get_left( search_rect );
    const double maximum_result
        = horizontal_line
        ? geometry_rectangle_get_bottom( search_rect )
        : geometry_rectangle_get_right( search_rect );

    if ( best_greater < maximum_result )
    {
        if ( best_smaller > minimum_result )
        {
            /* best_greater and best_smaller are both in range; */
            /* select the one with smaller distance to the center: */
            if ( best_greater - center > center - best_smaller )
            {
                *io_coordinate = best_smaller;
            }
            else
            {
                *io_coordinate = best_greater;
            }
        }
        else  /* best_greater is in range */
        {
            *io_coordinate = best_greater;
        }
    }
    else
    {
        if ( best_smaller > minimum_result )
        {
            /* best_smaller is in range */
            *io_coordinate = best_smaller;
        }
        else
        {
            /* BOTH BEST VALUES ARE OUT OF RANGE */
            /* CHECK GOOD VALUES: */
            if ( good_greater > maximum_result )
            {
                if ( good_smaller < minimum_result )
                {
                    err = U8_ERROR_NOT_FOUND;
                }
                else  /* good_smaller is in range */
                {
                    *io_coordinate = good_smaller;
                }
            }
            else  /* good_greater is in range */
            {
                if ( good_smaller < minimum_result )
                {
                    *io_coordinate = good_greater;
                }
                else
                {
                    /* good_smaller and good_greater are both in range; */
                    /* select the one with smaller distance to the center: */
                    if ( good_greater - center > center - good_smaller )
                    {
                        *io_coordinate = good_smaller;
                    }
                    else
                    {
                        *io_coordinate = good_greater;
                    }
                }
            }
        }
    }

    geometry_rectangle_destroy( &consider_rect );

    TRACE_END_ERR(err);
    return err;
}

void pencil_relationship_2d_layouter_private_make_all_visible ( pencil_relationship_2d_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* determine visibility */
    const uint32_t count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        layout_relationship_t *const the_relation = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );
        const layout_visible_classifier_t *const from_layout = layout_relationship_get_from_classifier_ptr ( the_relation );
        const layout_visible_classifier_t *const to_layout = layout_relationship_get_to_classifier_ptr ( the_relation );
        assert( from_layout != NULL );
        assert( to_layout != NULL );
        const data_visible_classifier_t *const from_data = layout_visible_classifier_get_data_const( from_layout );
        const data_visible_classifier_t *const to_data = layout_visible_classifier_get_data_const( to_layout );
        const data_diagramelement_t *const from_diagele = data_visible_classifier_get_diagramelement_const( from_data );
        const data_diagramelement_t *const to_diagele = data_visible_classifier_get_diagramelement_const( to_data );
        const data_diagramelement_flag_t from_flags = data_diagramelement_get_display_flags ( from_diagele );
        const data_diagramelement_flag_t to_flags = data_diagramelement_get_display_flags ( to_diagele );
        if (( 0 != ( DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT & from_flags ))
            || ( 0 != ( DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT & to_flags )))
        {
            layout_relationship_set_visibility ( the_relation, PENCIL_VISIBILITY_GRAY_OUT );
        }
        else
        {
            layout_relationship_set_visibility ( the_relation, PENCIL_VISIBILITY_SHOW );
        }
    }

    TRACE_END();
}

void pencil_relationship_2d_layouter_layout_standard( pencil_relationship_2d_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_relationship_2d_layouter_private_make_all_visible( this_ );

    pencil_relationship_2d_layouter_private_do_layout ( this_ );

    TRACE_END();
}

void pencil_relationship_2d_layouter_layout_void( pencil_relationship_2d_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* hide all relationships */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        /*
        pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_HIDE );
        */
        pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
    }

    /* layout the relationships (needed for PENCIL_VISIBILITY_IMPLICIT) */
    pencil_relationship_2d_layouter_private_do_layout ( this_ );

    TRACE_END();
}

void pencil_relationship_2d_layouter_layout_for_communication( pencil_relationship_2d_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_relationship_2d_layouter_private_make_all_visible( this_ );

    /* hide some relationships */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        layout_relationship_t *const the_relationship
            = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

        /* adjust visibility */
        if ( ( NULL == layout_relationship_get_from_feature_ptr ( the_relationship ) )
            && ( NULL == layout_relationship_get_to_feature_ptr ( the_relationship ) ) )
        {
            /* this is a globally visible relation, not local/scenario-based */
            pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
        }
    }

    /* layout the visible relationships */
    pencil_relationship_2d_layouter_private_do_layout ( this_ );

    TRACE_END();
}


/*
Copyright 2017-2022 Andreas Warnke

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
