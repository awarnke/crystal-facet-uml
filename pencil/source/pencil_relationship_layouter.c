/* File: pencil_relationship_layouter.c; Copyright and License: see below */

#include "pencil_relationship_layouter.h"
#include "trace/trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

void pencil_relationship_layouter_init( pencil_relationship_layouter_t *this_,
                                        pencil_layout_data_t *layout_data,
                                        pencil_size_t *pencil_size )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;

    pencil_relationship_painter_init( &((*this_).relationship_painter) );

    TRACE_END();
}

void pencil_relationship_layouter_destroy( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_relationship_painter_destroy( &((*this_).relationship_painter) );

    TRACE_END();
}

void pencil_relationship_layouter_private_do_layout ( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the relationships by their movement-needs, drop invisible relations */
    pencil_relationship_layouter_private_propose_processing_order ( this_, &sorted );

    /* shape the relationships */
    const uint32_t count_sorted
        = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* determine pointer to the_relationship */
        const uint32_t index
            = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        layout_relationship_t *const current_relationship
            = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, index );

        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 8;
        geometry_connector_t solution[8];

        /* propose options */
        pencil_relationship_layouter_private_propose_solutions ( this_,
                                                                 &sorted,
                                                                 sort_index,
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
            pencil_relationship_layouter_private_select_solution ( this_,
                                                                   &sorted,
                                                                   sort_index,
                                                                   solutions_count,
                                                                   solution,
                                                                   &index_of_best
                                                                 );
        }

        /* store best option to (*this_).layout_data */
        layout_relationship_set_shape( current_relationship, &(solution[index_of_best]) );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_relationship_layouter_private_propose_processing_order ( pencil_relationship_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert ( NULL != out_sorted );
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
            insert_error = universal_array_index_sorter_insert( out_sorted, index, simpleness );
            if ( 0 != insert_error )
            {
                TSLOG_WARNING( "not all relationships are shaped" );
            }
        }
    }

    TRACE_END();
}

void pencil_relationship_layouter_private_propose_solutions ( pencil_relationship_layouter_t *this_,
                                                              const universal_array_index_sorter_t *sorted,
                                                              uint32_t sort_index,
                                                              uint32_t solutions_max,
                                                              geometry_connector_t out_solutions[],
                                                              uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 1 <= solutions_max );  /* general requirement to report at least one option */
    assert ( 8 <= solutions_max );  /* current implementation requires at least 8 options */

    /* get current relation */
    const uint32_t index
        = universal_array_index_sorter_get_array_index( sorted, sort_index );
    layout_relationship_t *const current_relation
        = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

    /* propose connections between source and destination */
    {
        const geometry_rectangle_t *const source_rect
            = layout_relationship_get_from_symbol_box_const ( current_relation );
        const geometry_rectangle_t *const dest_rect
            = layout_relationship_get_to_symbol_box_const ( current_relation );

        uint32_t solutions_by_ZN;
        uint32_t solutions_by_L7;
        uint32_t solutions_by_UC;

        pencil_relationship_layouter_private_connect_rectangles_by_ZN ( this_,
                                                                        source_rect,
                                                                        dest_rect,
                                                                        solutions_max,
                                                                        &(out_solutions[0]),
                                                                        &solutions_by_ZN
                                                                      );

        pencil_relationship_layouter_private_connect_rectangles_by_L7 ( this_,
                                                                        source_rect,
                                                                        dest_rect,
                                                                        solutions_max - solutions_by_ZN,
                                                                        &(out_solutions[solutions_by_ZN]),
                                                                        &solutions_by_L7
                                                                      );

        pencil_relationship_layouter_private_connect_rectangles_by_UC ( this_,
                                                                        source_rect,
                                                                        dest_rect,
                                                                        solutions_max - solutions_by_ZN - solutions_by_L7,
                                                                        &(out_solutions[solutions_by_ZN + solutions_by_L7]),
                                                                        &solutions_by_UC
                                                                      );

        *out_solutions_count = solutions_by_ZN + solutions_by_L7 + solutions_by_UC;
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

void pencil_relationship_layouter_private_select_solution ( pencil_relationship_layouter_t *this_,
                                                            const universal_array_index_sorter_t *sorted,
                                                            uint32_t sort_index,
                                                            uint32_t solutions_count,
                                                            const geometry_connector_t solutions[],
                                                            uint32_t *out_index_of_best )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != solutions );
    assert ( NULL != out_index_of_best );
    assert ( 1 <= solutions_count );

    /* get current relation data */
    const uint32_t index
        = universal_array_index_sorter_get_array_index( sorted, sort_index );
    const layout_relationship_t *const current_relation
        = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );
    const data_relationship_t *const current_relation_data
        = layout_relationship_get_data_const ( current_relation );

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );

    /* define potential solution and rating */
    uint32_t index_of_best = 0;
    double debts_of_best = DBL_MAX;

    /* evaluate the solutions by their overlaps with classifiers */
    for ( uint32_t solution_idx = 0; solution_idx < solutions_count; solution_idx ++ )
    {
        /* evalute the debts of this solution */
        double debts_of_current = 0.0;
        const geometry_connector_t *const current_solution = &(solutions[solution_idx]);

        /* avoid alternating solutions in case their debts are identical */
        debts_of_current += 0.1 * solution_idx;

        /* the more length, the more unwanted... */
        debts_of_current += geometry_connector_get_length( current_solution );

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
                const double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
                if ( current_len > ( 4.0 * object_dist ) )
                {
                    /* current_solution is a long path and right-handed */
                    debts_of_current += 0.2 * geometry_connector_get_length( current_solution );
                }
            }
        }

        /* add debts for overlap to diagram boundary */
        {
            const geometry_rectangle_t connectors_bounds
                = geometry_connector_get_bounding_rectangle( current_solution );
            if ( ! geometry_rectangle_is_containing( diagram_draw_area, &connectors_bounds ) )
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

        /* iterate over the already created connectors (probe_sort_index < sort_index) */
        for ( uint32_t probe_sort_index = 0; probe_sort_index < sort_index; probe_sort_index ++ )
        {
            /* add debts if intersects */
            const uint32_t probe_index
                = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );
            const layout_relationship_t *const probe_relationship
                = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, probe_index );
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

void pencil_relationship_layouter_private_connect_rectangles_by_ZN ( pencil_relationship_layouter_t *this_,
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
    static const double NO_TOUCH = 0.0001;

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
            pencil_relationship_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
            geometry_rectangle_destroy( &search_rect );

            const geometry_rectangle_t depart_area
                = { .left=x_value, .top=src_top, .width=(src_left-NO_TOUCH-x_value), .height=src_height};
            pencil_relationship_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );

            const geometry_rectangle_t arrive_area
                = { .left=dst_right+NO_TOUCH, .top=dst_top, .width=(x_value-dst_right-NO_TOUCH), .height=dst_height};
            pencil_relationship_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );

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
            pencil_relationship_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
            geometry_rectangle_destroy( &search_rect );

            const geometry_rectangle_t depart_area
                = { .left=src_right+NO_TOUCH, .top=src_top, .width=(x_value-src_right-NO_TOUCH), .height=src_height};
            pencil_relationship_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );

            const geometry_rectangle_t arrive_area
                = { .left=x_value, .top=dst_top, .width=(dst_left-NO_TOUCH-x_value), .height=dst_height};
            pencil_relationship_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );

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
            pencil_relationship_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
            geometry_rectangle_destroy( &search_rect );

            const geometry_rectangle_t depart_area
                = { .left=src_left, .top=y_value, .width=src_width, .height=(src_top-NO_TOUCH-y_value)};
            pencil_relationship_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );

            const geometry_rectangle_t arrive_area
                = { .left=dst_left, .top=dst_bottom+NO_TOUCH, .width=dst_width, .height=(y_value-dst_bottom-NO_TOUCH)};
            pencil_relationship_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );

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
            pencil_relationship_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
            geometry_rectangle_destroy( &search_rect );

            const geometry_rectangle_t depart_area
                = { .left=src_left, .top=src_bottom+NO_TOUCH, .width=src_width, .height=(y_value-src_bottom-NO_TOUCH)};
            pencil_relationship_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );

            const geometry_rectangle_t arrive_area
                = { .left=dst_left, .top=y_value, .width=dst_width, .height=(dst_top-NO_TOUCH-y_value)};
            pencil_relationship_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );

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

void pencil_relationship_layouter_private_connect_rectangles_by_UC ( pencil_relationship_layouter_t *this_,
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

    /* prevent that forward and retour are on same line */
    double src_y = src_center_y;
    double dst_y = dst_center_y;
    if ( fabs( src_center_y - dst_center_y ) < NO_TOUCH )
    {
        /* forward way is identical to retour - may be a relation to self */
        src_y += gap_dist;
        dst_y -= gap_dist;
    }
    double src_x = src_center_x;
    double dst_x = dst_center_x;
    if ( fabs( src_center_x - dst_center_x ) < NO_TOUCH )
    {
        /* forward way is identical to retour - may be a relation to self */
        src_x -= gap_dist;
        dst_x += gap_dist;
    }

    /* connect via left side */
    {
        /* define defaults */
        double x_value = fmin( src_left, dst_left ) - object_dist;

        /* optimize coordinates */
        geometry_rectangle_t search_rect;
        geometry_rectangle_init_by_corners( &search_rect, draw_left, src_y, x_value, dst_y );
        pencil_relationship_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
        geometry_rectangle_destroy( &search_rect );

        const geometry_rectangle_t depart_area
            = { .left=x_value, .top=src_top, .width=(src_left-NO_TOUCH-x_value), .height=src_height};
        pencil_relationship_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );

        const geometry_rectangle_t arrive_area
            = { .left=x_value, .top=dst_top, .width=(dst_left-NO_TOUCH-x_value), .height=dst_height};
        pencil_relationship_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );

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

        /* optimize coordinates */
        geometry_rectangle_t search_rect;
        geometry_rectangle_init_by_corners( &search_rect, x_value, src_y, draw_right, dst_y );
        pencil_relationship_layouter_private_find_space_for_v_line ( this_, &search_rect, gap_dist, &x_value );
        geometry_rectangle_destroy( &search_rect );

        const geometry_rectangle_t depart_area
            = { .left=src_right+NO_TOUCH, .top=src_top, .width=(x_value-src_right-NO_TOUCH), .height=src_height};
        pencil_relationship_layouter_private_find_space_for_h_line ( this_, &depart_area, gap_dist, &src_y );

        const geometry_rectangle_t arrive_area
            = { .left=dst_right+NO_TOUCH, .top=dst_top, .width=(x_value-dst_right-NO_TOUCH), .height=dst_height};
        pencil_relationship_layouter_private_find_space_for_h_line ( this_, &arrive_area, gap_dist, &dst_y );

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

        /* optimize coordinates */
        geometry_rectangle_t search_rect;
        geometry_rectangle_init_by_corners( &search_rect, src_x, draw_top, dst_x, y_value );
        pencil_relationship_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
        geometry_rectangle_destroy( &search_rect );

        const geometry_rectangle_t depart_area
            = { .left=src_left, .top=y_value, .width=src_width, .height=(src_top-NO_TOUCH-y_value)};
        pencil_relationship_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );

        const geometry_rectangle_t arrive_area
            = { .left=dst_left, .top=y_value, .width=dst_width, .height=(dst_top-NO_TOUCH-y_value)};
        pencil_relationship_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );

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
        double y_value = fmax( src_bottom, dst_bottom ) - object_dist;

        /* optimize coordinates */
        geometry_rectangle_t search_rect;
        geometry_rectangle_init_by_corners( &search_rect, src_x, y_value, dst_x, draw_bottom );
        pencil_relationship_layouter_private_find_space_for_h_line ( this_, &search_rect, gap_dist, &y_value );
        geometry_rectangle_destroy( &search_rect );

        const geometry_rectangle_t depart_area
            = { .left=src_left, .top=(src_bottom+NO_TOUCH), .width=src_width, .height=(y_value-src_bottom-NO_TOUCH)};
        pencil_relationship_layouter_private_find_space_for_v_line ( this_, &depart_area, gap_dist, &src_x );

        const geometry_rectangle_t arrive_area
            = { .left=dst_left, .top=(dst_bottom+NO_TOUCH), .width=dst_width, .height=(y_value-dst_bottom-NO_TOUCH)};
        pencil_relationship_layouter_private_find_space_for_v_line ( this_, &arrive_area, gap_dist, &dst_x );

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

void pencil_relationship_layouter_private_connect_rectangles_by_L7 ( pencil_relationship_layouter_t *this_,
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

    const double dst_left = geometry_rectangle_get_left(dest_rect);
    const double dst_center_x = geometry_rectangle_get_center_x(dest_rect);
    const double dst_right = geometry_rectangle_get_right(dest_rect);
    const double dst_top = geometry_rectangle_get_top(dest_rect);
    const double dst_center_y = geometry_rectangle_get_center_y(dest_rect);
    const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

    const double good_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* if applicable, add a solution from source to left */
    if ( dst_center_x + good_dist < src_left )
    {
        if ( dst_bottom + good_dist < src_center_y )
        {
            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   src_center_y,
                                                   dst_center_x,
                                                   dst_bottom,
                                                   src_center_y
                                                 );
            solutions_count ++;
        }
        else if ( dst_top - good_dist > src_center_y )
        {
            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   src_center_y,
                                                   dst_center_x,
                                                   dst_top,
                                                   src_center_y
                                                 );
            solutions_count ++;
        }
    }

    /* else-if applicable, add a solution from source to right */
    else if ( dst_center_x - good_dist > src_right )
    {
        if ( dst_bottom + good_dist < src_center_y )
        {
            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   src_center_y,
                                                   dst_center_x,
                                                   dst_bottom,
                                                   src_center_y
                                                 );
            solutions_count ++;
        }
        else if ( dst_top - good_dist > src_center_y )
        {
            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   src_center_y,
                                                   dst_center_x,
                                                   dst_top,
                                                   src_center_y
                                                 );
            solutions_count ++;
        }
    }

    /* if applicable, add a solution from source to upwards */
    if ( dst_center_y + good_dist < src_top )
    {
        if ( dst_right + good_dist < src_center_x )
        {
            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_center_x,
                                                   src_top,
                                                   dst_right,
                                                   dst_center_y,
                                                   dst_center_y
                                                 );
            solutions_count ++;
        }
        else if ( dst_left - good_dist > src_center_x )
        {
            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_center_x,
                                                   src_top,
                                                   dst_left,
                                                   dst_center_y,
                                                   dst_center_y
                                                 );
            solutions_count ++;
        }
    }

    /* else-if applicable, add a solution from source to downwards */
    else if ( dst_center_y - good_dist > src_bottom )
    {
        if ( dst_right + good_dist < src_center_x )
        {
            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_center_x,
                                                   src_bottom,
                                                   dst_right,
                                                   dst_center_y,
                                                   dst_center_y
                                                 );
            solutions_count ++;
        }
        else if ( dst_left - good_dist > src_center_x )
        {
            /* add solution */
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_center_x,
                                                   src_bottom,
                                                   dst_left,
                                                   dst_center_y,
                                                   dst_center_y
                                                 );
            solutions_count ++;
        }
    }

    *out_solutions_count = solutions_count;

    TRACE_END();
}

u8_error_t pencil_relationship_layouter_private_find_space_for_line ( pencil_relationship_layouter_t *this_,
                                                                      const geometry_rectangle_t *search_rect,
                                                                      bool horizontal_line,
                                                                      double min_gap,
                                                                      double *io_coordinate )
{
    TRACE_BEGIN();
    assert ( NULL != search_rect );
    assert ( NULL != io_coordinate );
    u8_error_t err = U8_ERROR_NONE;

    /* start two probes at the center and move these to the boundaries when discovering overlaps */
    const double center = *io_coordinate;
    double smaller_probe;
    double greater_probe;
    if ( horizontal_line )
    {
        assert( center > geometry_rectangle_get_top( search_rect ) - 0.000000001 );
        assert( center < geometry_rectangle_get_bottom( search_rect ) + 0.000000001 );
        smaller_probe = center;
        greater_probe = center;
    }
    else
    {
        assert( center > geometry_rectangle_get_left( search_rect ) - 0.000000001 );
        assert( center < geometry_rectangle_get_right( search_rect ) + 0.000000001 );
        smaller_probe = center;
        greater_probe = center;
    }

    /* iterate over all classifiers */
    const uint32_t count_classifiers
        = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
    const uint32_t max_list_iteration = count_classifiers;  /* in the worst case, each iteration moves the probes by one classifier */
    bool hit = true;  /* whenever the probes hit a rectangle, hit is set to true */
    for ( uint32_t list_iteration = 0; (list_iteration < max_list_iteration) && hit; list_iteration ++ )
    {
        hit = false;
        for ( uint32_t classifier_index = 0; classifier_index < count_classifiers; classifier_index ++ )
        {
            const layout_visible_classifier_t *const the_classifier
                = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, classifier_index );
            const geometry_rectangle_t *const classifier_symbol_box
                = layout_visible_classifier_get_symbol_box_const( the_classifier );
            const geometry_rectangle_t *const classifier_space
                = layout_visible_classifier_get_space_const( the_classifier );

            if ( geometry_rectangle_is_contiguous( search_rect, classifier_symbol_box ) )
            {
                if ( horizontal_line )
                {
                    const double line_left = geometry_rectangle_get_left( search_rect );
                    const double line_right = geometry_rectangle_get_right( search_rect );
                    const bool is_smaller_contained
                        = geometry_rectangle_contains( classifier_space, line_left, smaller_probe )
                        && geometry_rectangle_contains( classifier_space, line_right, smaller_probe );
                    if ( ( ! is_smaller_contained )
                        && ( geometry_rectangle_get_top(classifier_symbol_box) - min_gap < smaller_probe )
                        && ( geometry_rectangle_get_bottom(classifier_symbol_box) + min_gap > smaller_probe ) )
                    {
                        smaller_probe = geometry_rectangle_get_top(classifier_symbol_box) - min_gap;
                        hit = true;
                    }
                    const bool is_greater_contained
                        = geometry_rectangle_contains( classifier_space, line_left, greater_probe )
                        && geometry_rectangle_contains( classifier_space, line_right, greater_probe );
                    if ( ( ! is_greater_contained )
                        && ( geometry_rectangle_get_top(classifier_symbol_box) - min_gap < greater_probe )
                        && ( geometry_rectangle_get_bottom(classifier_symbol_box) + min_gap > greater_probe ) )
                    {
                        greater_probe = geometry_rectangle_get_bottom(classifier_symbol_box) + min_gap;
                        hit = true;
                    }
                }
                else
                {
                    const double line_top = geometry_rectangle_get_top( search_rect );
                    const double line_bottom = geometry_rectangle_get_bottom( search_rect );
                    const bool is_smaller_contained
                        = geometry_rectangle_contains( classifier_space, smaller_probe, line_top )
                        && geometry_rectangle_contains( classifier_space, smaller_probe, line_bottom );
                    if ( ( ! is_smaller_contained )
                        && ( geometry_rectangle_get_left(classifier_symbol_box) - min_gap < smaller_probe )
                        && ( geometry_rectangle_get_right(classifier_symbol_box) + min_gap > smaller_probe ) )
                    {
                        smaller_probe = geometry_rectangle_get_left(classifier_symbol_box) - min_gap;
                        hit = true;
                    }
                    const bool is_greater_contained
                        = geometry_rectangle_contains( classifier_space, greater_probe, line_top )
                        && geometry_rectangle_contains( classifier_space, greater_probe, line_bottom );
                    if ( ( ! is_greater_contained )
                        && ( geometry_rectangle_get_left(classifier_symbol_box) - min_gap < greater_probe )
                        && ( geometry_rectangle_get_right(classifier_symbol_box) + min_gap > greater_probe ) )
                    {
                        greater_probe = geometry_rectangle_get_right(classifier_symbol_box) + min_gap;
                        hit = true;
                    }
                }
            }
        }
    }

    /* check success */
    if ( horizontal_line )
    {
        if ( greater_probe > geometry_rectangle_get_bottom( search_rect ) )
        {
            if ( smaller_probe < geometry_rectangle_get_top( search_rect ) )
            {
                err = U8_ERROR_NOT_FOUND;
            }
            else
            {
                *io_coordinate = smaller_probe;
            }
        }
        else
        {
            if ( smaller_probe < geometry_rectangle_get_top( search_rect ) )
            {
                *io_coordinate = greater_probe;
            }
            else
            {
                if ( greater_probe - center > center - smaller_probe )
                {
                    *io_coordinate = smaller_probe;
                }
                else
                {
                    *io_coordinate = greater_probe;
                }
            }
        }
    }
    else
    {
        if ( greater_probe > geometry_rectangle_get_right( search_rect ) )
        {
            if ( smaller_probe < geometry_rectangle_get_left( search_rect ) )
            {
                err = U8_ERROR_NOT_FOUND;
            }
            else
            {
                *io_coordinate = smaller_probe;
            }
        }
        else
        {
            if ( smaller_probe < geometry_rectangle_get_left( search_rect ) )
            {
                *io_coordinate = greater_probe;
            }
            else
            {
                if ( greater_probe - center > center - smaller_probe )
                {
                    *io_coordinate = smaller_probe;
                }
                else
                {
                    *io_coordinate = greater_probe;
                }
            }
        }
    }

    TRACE_END_ERR(err);
    return err;
}

void pencil_relationship_layouter_private_make_all_visible ( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* determine visibility */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        layout_relationship_t *the_relation = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );
        layout_visible_classifier_t *from_layout = layout_relationship_get_from_classifier_ptr ( the_relation );
        layout_visible_classifier_t *to_layout = layout_relationship_get_to_classifier_ptr ( the_relation );
        assert( from_layout != NULL );
        assert( to_layout != NULL );

        {
            const data_visible_classifier_t *from_data = layout_visible_classifier_get_data_const( from_layout );
            const data_visible_classifier_t *to_data = layout_visible_classifier_get_data_const( to_layout );
            const data_diagramelement_t *from_diagele = data_visible_classifier_get_diagramelement_const( from_data );
            const data_diagramelement_t *to_diagele = data_visible_classifier_get_diagramelement_const( to_data );
            data_diagramelement_flag_t from_flags = data_diagramelement_get_display_flags ( from_diagele );
            data_diagramelement_flag_t to_flags = data_diagramelement_get_display_flags ( to_diagele );
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
    }

    TRACE_END();
}

void pencil_relationship_layouter_layout_standard( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_relationship_layouter_private_make_all_visible( this_ );

    pencil_relationship_layouter_private_do_layout ( this_ );

    TRACE_END();
}

void pencil_relationship_layouter_layout_void( pencil_relationship_layouter_t *this_ )
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
    pencil_relationship_layouter_private_do_layout ( this_ );

    TRACE_END();
}

void pencil_relationship_layouter_layout_for_sequence( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_relationship_layouter_private_make_all_visible( this_ );

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = pencil_layout_data_get_diagram_const( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );

    /* layout the relationships */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count( (*this_).layout_data );
    TRACE_INFO_INT( "count_relations:", count_relations );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        /* get the relationship to layout */
        layout_relationship_t *const the_relationship
            = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, index );

        /* adjust visibility */
        if ( ( NULL == layout_relationship_get_from_feature_ptr( the_relationship ) )
            && ( NULL == layout_relationship_get_to_feature_ptr( the_relationship ) ) )
        {
            /* this is a globally visible relation, not local/scenario-based */
            pencil_layout_data_set_relationship_visibility( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
        }

        /* calculate layout */
        {
            /* determine y-coordinate */
            const data_relationship_t *const the_relationdata
                = layout_relationship_get_data_const( the_relationship );
            int32_t list_order = data_relationship_get_list_order( the_relationdata );
            const double y_value_rel = (list_order/((double)UINT32_MAX))+0.5;
            const double draw_top = geometry_rectangle_get_top(diagram_draw_area);
            const double draw_bottom = geometry_rectangle_get_bottom(diagram_draw_area);
            const double y_value = ( (draw_bottom - draw_top) * y_value_rel ) + draw_top;

            /* get source and destination rectangles */
            const geometry_rectangle_t *const source_rect
                = layout_relationship_get_from_symbol_box_const ( the_relationship );
            const geometry_rectangle_t *const dest_rect
                = layout_relationship_get_to_symbol_box_const ( the_relationship );

            /* calculate coordinates */
            const double src_left = geometry_rectangle_get_left(source_rect);
            const double src_center_x = geometry_rectangle_get_center_x(source_rect);
            const double src_right = geometry_rectangle_get_right(source_rect);
            const double src_top = geometry_rectangle_get_top(source_rect);
            /*const double src_center_y = geometry_rectangle_get_center_y(source_rect);*/
            const double src_bottom = geometry_rectangle_get_bottom(source_rect);

            const double dst_left = geometry_rectangle_get_left(dest_rect);
            const double dst_center_x = geometry_rectangle_get_center_x(dest_rect);
            const double dst_right = geometry_rectangle_get_right(dest_rect);
            const double dst_top = geometry_rectangle_get_top(dest_rect);
            /*const double dst_center_y = geometry_rectangle_get_center_y(dest_rect);*/
            const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

            /* is interaction relation */
            const data_relationship_type_t rel_type
                = data_relationship_get_main_type( the_relationdata );
            const bool is_interaction
                = ( rel_type == DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL );

            const double src_from_y
               = ( y_value < src_top ) ? src_top : ( y_value > src_bottom ) ? src_bottom : y_value;
            const double dst_to_y
               = ( y_value < dst_top ) ? dst_top : ( y_value > dst_bottom ) ? dst_bottom : y_value;
            const double src_from_x
                = (( y_value < src_top )||( y_value > src_bottom )|| is_interaction )
                ? src_center_x
                : ( src_center_x < dst_center_x )
                ? src_right
                : src_left;
            const double dst_to_x
                = (( y_value < dst_top )||( y_value > dst_bottom )|| is_interaction )
                ? dst_center_x
                : ( src_center_x < dst_center_x )
                ? dst_left
                : dst_right;

            /* determine minimum arrow size for message/call to self */
            const double good_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

            /* define relation */
            geometry_connector_t relationship_shape;
            if ( fabs( src_center_x - dst_center_x ) < 0.0001 )
            {
                /* message/call to self */
                geometry_connector_init_vertical ( &relationship_shape,
                                                   src_center_x,
                                                   src_from_y - (good_dist/2.0),
                                                   dst_center_x,
                                                   dst_to_y + (good_dist/2.0),
                                                   src_center_x + good_dist /* the main connector shall be right to the start/end points */
                                                 );
            }
            else
            {
                /* normal message/call */
                geometry_connector_init_horizontal ( &relationship_shape,
                                                     src_from_x,
                                                     src_from_y,
                                                     dst_to_x,
                                                     dst_to_y,
                                                     y_value
                                                   );
            }
            layout_relationship_set_shape( the_relationship, &relationship_shape );
            geometry_connector_destroy( &relationship_shape );
        }
    }

    TRACE_END();
}

void pencil_relationship_layouter_layout_for_timing( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_relationship_layouter_private_make_all_visible( this_ );

    /* get draw area */
    const layout_diagram_t *const diagram_layout
        = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
    const geometry_rectangle_t *const diagram_draw_area
        = layout_diagram_get_draw_area_const( diagram_layout );

    /* layout the relationships */
    const uint32_t count_relations
        = pencil_layout_data_get_relationship_count( (*this_).layout_data );
    TRACE_INFO_INT( "count_relations:", count_relations );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        /* get the relationship to layout */
        layout_relationship_t *const the_relationship
            = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, index );

        /* adjust visibility */
        if ( ( NULL == layout_relationship_get_from_feature_ptr( the_relationship ) )
            && ( NULL == layout_relationship_get_to_feature_ptr( the_relationship ) ) )
        {
            /* this is a globally visible relation, not local/scenario-based */
            pencil_layout_data_set_relationship_visibility( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
        }

        /* calculate layout */
        {
            /* determine x-coordinate */
            const data_relationship_t *const the_relationdata
               = layout_relationship_get_data_const( the_relationship );
            const int32_t list_order = data_relationship_get_list_order ( the_relationdata );
            const double x_value_rel = (list_order/((double)UINT32_MAX))+0.5;
            const double draw_left = geometry_rectangle_get_left(diagram_draw_area);
            const double draw_right = geometry_rectangle_get_right(diagram_draw_area);
            const double x_value = ( (draw_right - draw_left) * x_value_rel ) + draw_left;

            /* get source and destination rectangles */
            const geometry_rectangle_t *const source_rect
                = layout_relationship_get_from_symbol_box_const( the_relationship );
            const geometry_rectangle_t *const dest_rect
                = layout_relationship_get_to_symbol_box_const( the_relationship );

            /* calculate coordinates */
            const double src_left = geometry_rectangle_get_left(source_rect);
            /*const double src_center_x = geometry_rectangle_get_center_x(source_rect);*/
            const double src_right = geometry_rectangle_get_right(source_rect);
            const double src_top = geometry_rectangle_get_top(source_rect);
            const double src_center_y = geometry_rectangle_get_center_y(source_rect);
            const double src_bottom = geometry_rectangle_get_bottom(source_rect);

            const double dst_left = geometry_rectangle_get_left(dest_rect);
            /*const double dst_center_x = geometry_rectangle_get_center_x(dest_rect);*/
            const double dst_right = geometry_rectangle_get_right(dest_rect);
            const double dst_top = geometry_rectangle_get_top(dest_rect);
            const double dst_center_y = geometry_rectangle_get_center_y(dest_rect);
            const double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

            /* is interaction relation */
            const data_relationship_type_t rel_type
                = data_relationship_get_main_type( the_relationdata );
            const bool is_interaction
                = ( rel_type == DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW )
                || ( rel_type == DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL );

            const double src_from_x
                = ( x_value < src_left ) ? src_left : ( x_value > src_right ) ? src_right : x_value;
            const double dst_to_x
                = ( x_value < dst_left ) ? dst_left : ( x_value > dst_right ) ? dst_right : x_value;
            const double src_from_y
                = (( x_value < src_left )||( x_value > src_right )|| is_interaction )
                ? src_center_y
                : ( src_center_y < dst_center_y )
                ? src_bottom
                : src_top;
            const double dst_to_y
                = (( x_value < dst_left )||( x_value > dst_right )|| is_interaction )
                ? dst_center_y
                : ( src_center_y < dst_center_y )
                ? dst_top
                : dst_bottom;

            /* determine minimum arrow size for self transition */
            const double good_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

            /* define relation */
            geometry_connector_t relationship_shape;
            if ( fabs( src_center_y - dst_center_y ) < 0.0001 )
            {
                /* transition to self */
                geometry_connector_init_horizontal ( &relationship_shape,
                                                     src_from_x - (good_dist/2.0),
                                                     src_center_y,
                                                     dst_to_x + (good_dist/2.0),
                                                     dst_center_y,
                                                     src_center_y - good_dist /* the main connector shall be above the start/end points */
                                                   );
            }
            else
            {
                /* normal transition */
                geometry_connector_init_vertical ( &relationship_shape,
                                                   src_from_x,
                                                   src_from_y,
                                                   dst_to_x,
                                                   dst_to_y,
                                                   x_value
                                                 );
            }
            layout_relationship_set_shape( the_relationship, &relationship_shape );
            geometry_connector_destroy( &relationship_shape );
        }
    }

    TRACE_END();
}

void pencil_relationship_layouter_layout_for_communication( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_relationship_layouter_private_make_all_visible( this_ );

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
    pencil_relationship_layouter_private_do_layout ( this_ );

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
