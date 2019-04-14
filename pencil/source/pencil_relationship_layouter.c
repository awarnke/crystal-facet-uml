/* File: pencil_relationship_layouter.c; Copyright and License: see below */

#include "pencil_relationship_layouter.h"
#include "trace.h"
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
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
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
        uint32_t index;
        index = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        /* copy the relationship shape */
        layout_relationship_t *the_relationship;
        the_relationship = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, index );
        geometry_connector_t *relationship_shape;
        relationship_shape = layout_relationship_get_shape_ptr( the_relationship );
        geometry_connector_copy( relationship_shape, &(solution[index_of_best]) );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_relationship_layouter_private_propose_processing_order ( pencil_relationship_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_INPUT_DATA_MAX_RELATIONSHIPS );

    /* get draw area */
    geometry_rectangle_t *diagram_draw_area;
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
        diagram_draw_area = layout_diagram_get_draw_area_ptr( diagram_layout );
    }

    /* sort the relationships by their shaping-needs: the less simple, the earlier it shall be processed */
    uint32_t count_relations;
    count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        layout_relationship_t *current_relation;
        current_relation = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

        int64_t simpleness = 0;

        /* determine simpleness by relationship type */
        {
            data_relationship_type_t reltype;
            reltype = data_relationship_get_main_type( layout_relationship_get_data_ptr ( current_relation ));
            if (( DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY == reltype )
                ||( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == reltype ))
            {
                /* containment may be solved by embracing, mere dependencies are unimportant */
                simpleness += geometry_rectangle_get_width ( diagram_draw_area );
            }
        }

        /* whatever is not visible is simple */
        {
            if ( PENCIL_VISIBILITY_SHOW != layout_relationship_get_visibility ( current_relation ) )
            {
                simpleness += 2 * geometry_rectangle_get_width ( diagram_draw_area );
            }
        }

        /* determine simpleness by distance between source and destination */
        {
            const geometry_rectangle_t *source_rect;
            const geometry_rectangle_t *dest_rect;
            source_rect = layout_relationship_get_from_bounds_const ( current_relation );
            dest_rect = layout_relationship_get_to_bounds_const ( current_relation );

            simpleness -= fabs ( geometry_rectangle_get_x_center(source_rect) - geometry_rectangle_get_x_center(dest_rect) );
            simpleness -= fabs ( geometry_rectangle_get_y_center(source_rect) - geometry_rectangle_get_y_center(dest_rect) );
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

    uint32_t index;
    index = universal_array_index_sorter_get_array_index( sorted, sort_index );

    layout_relationship_t *current_relation;
    current_relation = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

    /* propose connections between source and destination */
    {
        const geometry_rectangle_t *source_rect;
        const geometry_rectangle_t *dest_rect;
        source_rect = layout_relationship_get_from_bounds_const ( current_relation );
        dest_rect = layout_relationship_get_to_bounds_const ( current_relation );

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
    }

    TRACE_END();
}

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

    /* get draw area */
    geometry_rectangle_t *diagram_draw_area;
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
        diagram_draw_area = layout_diagram_get_draw_area_ptr( diagram_layout );
    }

    /* define potential solution and rating */
    uint32_t index_of_best;
    double debts_of_best;
    index_of_best = 0;  /* in case of doubts, take the first solution */
    debts_of_best = 1000000000.0;

    /* evaluate the solutions by their overlaps with classifiers */
    for ( uint32_t solution_idx = 0; solution_idx < solutions_count; solution_idx ++ )
    {
        /* evalute the debts of this solution */
        double debts_of_current;
        debts_of_current = 0.0;

        /* avoid alternating solutions in case their debts are identical */
        debts_of_current += 0.1 * solution_idx;

        /* the more length, the more unwanted... */
        debts_of_current += geometry_connector_get_length( &(solutions[solution_idx]) );

        /* add debts for overlap to diagram boundary */
        {
            geometry_rectangle_t connectors_bounds;
            connectors_bounds = geometry_connector_get_bounding_rectangle( &(solutions[solution_idx]) );

            double current_area = geometry_rectangle_get_area ( &connectors_bounds );
            geometry_rectangle_t intersect;
            geometry_rectangle_init_by_intersect( &intersect, &connectors_bounds, diagram_draw_area );
            double intersect_area = geometry_rectangle_get_area ( &intersect );

            if ( (current_area - 0.1) > intersect_area )
            {
                debts_of_current += 1000000.0;
            }
        }

        /* iterate over all classifiers */
        uint32_t count_clasfy;
        count_clasfy = pencil_layout_data_get_classifier_count ( (*this_).layout_data );
        for ( uint32_t clasfy_index = 0; clasfy_index < count_clasfy; clasfy_index ++ )
        {
            layout_visible_classifier_t *probe_classifier;
            probe_classifier = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, clasfy_index );

            geometry_rectangle_t *classifier_bounds;
            classifier_bounds = layout_visible_classifier_get_bounds_ptr( probe_classifier );
            if ( geometry_connector_is_intersecting_rectangle( &(solutions[solution_idx]), classifier_bounds ) )
            {
                debts_of_current += 100000.0;
            }

            geometry_rectangle_t *classifier_label_box;
            classifier_label_box = layout_visible_classifier_get_label_box_ptr( probe_classifier );
            if ( geometry_connector_is_intersecting_rectangle( &(solutions[solution_idx]), classifier_label_box ) )
            {
                debts_of_current += 10000.0;
            }
        }

        /* iterate over the already created connectors (probe_sort_index < sort_index) */
        for ( uint32_t probe_sort_index = 0; probe_sort_index < sort_index; probe_sort_index ++ )
        {
            /* add debts if intersects */
            uint32_t probe_index;
            probe_index = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );
            layout_relationship_t *probe_relationship;
            probe_relationship = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, probe_index );
            geometry_connector_t *probe_shape;
            probe_shape = layout_relationship_get_shape_ptr( probe_relationship );
            uint32_t intersects;
            intersects = geometry_connector_count_connector_intersects( &(solutions[solution_idx]), probe_shape );
            debts_of_current += 1000.0 * intersects;
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

    /* the best */
    *out_index_of_best = index_of_best;

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

    double src_left = geometry_rectangle_get_left(source_rect);
    double src_x_center = geometry_rectangle_get_x_center(source_rect);
    double src_right = geometry_rectangle_get_right(source_rect);
    double src_top = geometry_rectangle_get_top(source_rect);
    double src_y_center = geometry_rectangle_get_y_center(source_rect);
    double src_bottom = geometry_rectangle_get_bottom(source_rect);

    double dst_left = geometry_rectangle_get_left(dest_rect);
    double dst_x_center = geometry_rectangle_get_x_center(dest_rect);
    double dst_right = geometry_rectangle_get_right(dest_rect);
    double dst_top = geometry_rectangle_get_top(dest_rect);
    double dst_y_center = geometry_rectangle_get_y_center(dest_rect);
    double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

    double object_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    double good_dist = 2.0 * object_dist;  /* duplicate distance: once for each side of the line */
    double gap_dist = 0.499 * object_dist;  /* half the object distance allows a line to pass between two objects */

    /* main line is vertical */
    {
        if ( dst_right + good_dist < src_left )
        {
            double x_value;

            bool space_found;
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, src_left, src_y_center, dst_right, dst_y_center );
            pencil_relationship_layouter_private_find_space_for_line ( this_,
                                                                       &search_rect,
                                                                       false /* horizontal_line */,
                                                                       gap_dist,
                                                                       &space_found,
                                                                       &x_value
                                                                     );
            geometry_rectangle_destroy( &search_rect );

            geometry_connector_reinit_vertical ( &(out_solutions[solutions_count]),
                                                 src_left,
                                                 src_y_center,
                                                 dst_right,
                                                 dst_y_center,
                                                 x_value
                                               );
            solutions_count ++;
        }
        else if ( dst_left - good_dist > src_right )
        {
            double x_value;

            bool space_found;
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, src_right, src_y_center, dst_left, dst_y_center );
            pencil_relationship_layouter_private_find_space_for_line ( this_,
                                                                       &search_rect,
                                                                       false /* horizontal_line */,
                                                                       gap_dist,
                                                                       &space_found,
                                                                       &x_value
                                                                     );
            geometry_rectangle_destroy( &search_rect );

            geometry_connector_reinit_vertical ( &(out_solutions[solutions_count]),
                                                 src_right,
                                                 src_y_center,
                                                 dst_left,
                                                 dst_y_center,
                                                 x_value
                                               );
            solutions_count ++;
        }
    }

    /* main line is horizontal */
    {
        if ( dst_bottom + good_dist < src_top )
        {
            double y_value;

            bool space_found;
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, src_x_center, src_top, dst_x_center, dst_bottom );
            pencil_relationship_layouter_private_find_space_for_line ( this_,
                                                                       &search_rect,
                                                                       true /* horizontal_line */,
                                                                       gap_dist,
                                                                       &space_found,
                                                                       &y_value
                                                                     );
            geometry_rectangle_destroy( &search_rect );

            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_top,
                                                   dst_x_center,
                                                   dst_bottom,
                                                   y_value
                                                 );
            solutions_count ++;
        }
        else if ( dst_top - good_dist > src_bottom )
        {
            double y_value;

            bool space_found;
            geometry_rectangle_t search_rect;
            geometry_rectangle_init_by_corners( &search_rect, src_x_center, src_bottom, dst_x_center, dst_top );
            pencil_relationship_layouter_private_find_space_for_line ( this_,
                                                                       &search_rect,
                                                                       true /* horizontal_line */,
                                                                       gap_dist,
                                                                       &space_found,
                                                                       &y_value
                                                                     );
            geometry_rectangle_destroy( &search_rect );

            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_bottom,
                                                   dst_x_center,
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

    double src_left = geometry_rectangle_get_left(source_rect);
    double src_x_center = geometry_rectangle_get_x_center(source_rect);
    double src_right = geometry_rectangle_get_right(source_rect);
    double src_top = geometry_rectangle_get_top(source_rect);
    double src_y_center = geometry_rectangle_get_y_center(source_rect);
    double src_bottom = geometry_rectangle_get_bottom(source_rect);

    double dst_left = geometry_rectangle_get_left(dest_rect);
    double dst_x_center = geometry_rectangle_get_x_center(dest_rect);
    double dst_right = geometry_rectangle_get_right(dest_rect);
    double dst_top = geometry_rectangle_get_top(dest_rect);
    double dst_y_center = geometry_rectangle_get_y_center(dest_rect);
    double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

    double good_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* prevent that forward and retour are on same line */
    if ( fabs( src_y_center - dst_y_center ) < 0.0001 )
    {
        /* forward way is identical to retour - may be a relation to self */
        src_y_center += good_dist/2.0;
        dst_y_center -= good_dist/2.0;
    }
    if ( fabs( src_x_center - dst_x_center ) < 0.0001 )
    {
        /* forward way is identical to retour - may be a relation to self */
        src_x_center -= good_dist/2.0;
        dst_x_center += good_dist/2.0;
    }

    /* connect via left side */
    {
        geometry_connector_reinit_vertical( &(out_solutions[solutions_count]),
                                            src_left,
                                            src_y_center,
                                            dst_left,
                                            dst_y_center,
                                            fmin( src_left, dst_left ) - good_dist
                                            );
        solutions_count ++;
    }

    /* connect via right side */
    {
        geometry_connector_reinit_vertical( &(out_solutions[solutions_count]),
                                            src_right,
                                            src_y_center,
                                            dst_right,
                                            dst_y_center,
                                            fmax( src_right, dst_right ) + good_dist
                                            );
        solutions_count ++;
    }

    /* connect via top side */
    {
        geometry_connector_reinit_horizontal( &(out_solutions[solutions_count]),
                                                src_x_center,
                                                src_top,
                                                dst_x_center,
                                                dst_top,
                                                fmin( src_top, dst_top ) - good_dist
                                            );
        solutions_count ++;
    }

    /* connect via bottom side */
    {
        geometry_connector_reinit_horizontal( &(out_solutions[solutions_count]),
                                                src_x_center,
                                                src_bottom,
                                                dst_x_center,
                                                dst_bottom,
                                                fmax( src_bottom, dst_bottom ) + good_dist
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

    double src_left = geometry_rectangle_get_left(source_rect);
    double src_x_center = geometry_rectangle_get_x_center(source_rect);
    double src_right = geometry_rectangle_get_right(source_rect);
    double src_top = geometry_rectangle_get_top(source_rect);
    double src_y_center = geometry_rectangle_get_y_center(source_rect);
    double src_bottom = geometry_rectangle_get_bottom(source_rect);

    double dst_left = geometry_rectangle_get_left(dest_rect);
    double dst_x_center = geometry_rectangle_get_x_center(dest_rect);
    double dst_right = geometry_rectangle_get_right(dest_rect);
    double dst_top = geometry_rectangle_get_top(dest_rect);
    double dst_y_center = geometry_rectangle_get_y_center(dest_rect);
    double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

    double good_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* from source to left */
    if ( dst_x_center + good_dist < src_left )
    {
        if ( dst_bottom + good_dist < src_y_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   src_y_center,
                                                   dst_x_center,
                                                   dst_bottom,
                                                   src_y_center
                                                 );
            solutions_count ++;
        }
        else if ( dst_top - good_dist > src_y_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   src_y_center,
                                                   dst_x_center,
                                                   dst_top,
                                                   src_y_center
                                                 );
            solutions_count ++;
        }
    }

    /* from source to right */
    else if ( dst_x_center - good_dist > src_right )
    {
        if ( dst_bottom + good_dist < src_y_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   src_y_center,
                                                   dst_x_center,
                                                   dst_bottom,
                                                   src_y_center
                                                 );
            solutions_count ++;
        }
        else if ( dst_top - good_dist > src_y_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   src_y_center,
                                                   dst_x_center,
                                                   dst_top,
                                                   src_y_center
                                                 );
            solutions_count ++;
        }
    }

    /* from source to upwards */
    if ( dst_y_center + good_dist < src_top )
    {
        if ( dst_right + good_dist < src_x_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_top,
                                                   dst_right,
                                                   dst_y_center,
                                                   dst_y_center
                                                 );
            solutions_count ++;
        }
        else if ( dst_left - good_dist > src_x_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_top,
                                                   dst_left,
                                                   dst_y_center,
                                                   dst_y_center
                                                 );
            solutions_count ++;
        }
    }

    /* from source to downwards */
    else if ( dst_y_center - good_dist > src_bottom )
    {
        if ( dst_right + good_dist < src_x_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_bottom,
                                                   dst_right,
                                                   dst_y_center,
                                                   dst_y_center
                                                 );
            solutions_count ++;
        }
        else if ( dst_left - good_dist > src_x_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_bottom,
                                                   dst_left,
                                                   dst_y_center,
                                                   dst_y_center
                                                 );
            solutions_count ++;
        }
    }

    *out_solutions_count = solutions_count;

    TRACE_END();
}

void pencil_relationship_layouter_private_find_space_for_line ( pencil_relationship_layouter_t *this_,
                                                                const geometry_rectangle_t *search_rect,
                                                                bool horizontal_line,
                                                                double min_gap,
                                                                bool *out_success,
                                                                double *out_coordinate )
{
    TRACE_BEGIN();
    assert ( NULL != search_rect );
    assert ( NULL != out_success );
    assert ( NULL != out_coordinate );

    /* start two probes at the center and move these to the boundaries when discovering overlaps */
    double smaller_probe;
    double greater_probe;
    double center;
    if ( horizontal_line )
    {
        center = geometry_rectangle_get_y_center( search_rect );
        smaller_probe = center;
        greater_probe = center;
    }
    else
    {
        center = geometry_rectangle_get_x_center( search_rect );
        smaller_probe = center;
        greater_probe = center;
    }

    /* iterate over all classifiers */
    uint32_t count_classifiers;
    count_classifiers = pencil_layout_data_get_classifier_count ( (*this_).layout_data );
    const uint32_t max_list_iteration = count_classifiers;  /* in the worst case, each iteration moves the probes by one classifier */
    bool hit = true;  /* whenever the probes hit a rectangle, hit is set to true */
    for ( uint32_t list_iteration = 0; (list_iteration < max_list_iteration) && hit; list_iteration ++ )
    {
        hit = false;
        for ( uint32_t classifier_index = 0; classifier_index < count_classifiers; classifier_index ++ )
        {
            layout_visible_classifier_t *the_classifier;
            the_classifier = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, classifier_index );
            geometry_rectangle_t *classifier_bounds;
            classifier_bounds = layout_visible_classifier_get_bounds_ptr( the_classifier );

            if ( geometry_rectangle_is_intersecting( search_rect, classifier_bounds ) )
            {
                if ( horizontal_line )
                {
                    if ( ( geometry_rectangle_get_top(classifier_bounds) - min_gap < smaller_probe )
                        && ( geometry_rectangle_get_bottom(classifier_bounds) + min_gap > smaller_probe ) )
                    {
                        smaller_probe = geometry_rectangle_get_top(classifier_bounds) - min_gap;
                        hit = true;
                    }
                    if ( ( geometry_rectangle_get_top(classifier_bounds) - min_gap < greater_probe )
                        && ( geometry_rectangle_get_bottom(classifier_bounds) + min_gap > greater_probe ) )
                    {
                        greater_probe = geometry_rectangle_get_bottom(classifier_bounds) + min_gap;
                        hit = true;
                    }
                }
                else
                {
                    if ( ( geometry_rectangle_get_left(classifier_bounds) - min_gap < smaller_probe )
                        && ( geometry_rectangle_get_right(classifier_bounds) + min_gap > smaller_probe ) )
                    {
                        smaller_probe = geometry_rectangle_get_left(classifier_bounds) - min_gap;
                        hit = true;
                    }
                    if ( ( geometry_rectangle_get_left(classifier_bounds) - min_gap < greater_probe )
                        && ( geometry_rectangle_get_right(classifier_bounds) + min_gap > greater_probe ) )
                    {
                        greater_probe = geometry_rectangle_get_right(classifier_bounds) + min_gap;
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
                *out_success = false;
                *out_coordinate = center;
            }
            else
            {
                *out_success = true;
                *out_coordinate = smaller_probe;
            }
        }
        else
        {
            if ( smaller_probe < geometry_rectangle_get_top( search_rect ) )
            {
                *out_success = true;
                *out_coordinate = greater_probe;
            }
            else
            {
                *out_success = true;
                if ( greater_probe - center > center - smaller_probe )
                {
                    *out_coordinate = smaller_probe;
                }
                else
                {
                    *out_coordinate = greater_probe;
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
                *out_success = false;
                *out_coordinate = center;
            }
            else
            {
                *out_success = true;
                *out_coordinate = smaller_probe;
            }
        }
        else
        {
            if ( smaller_probe < geometry_rectangle_get_left( search_rect ) )
            {
                *out_success = true;
                *out_coordinate = greater_probe;
            }
            else
            {
                *out_success = true;
                if ( greater_probe - center > center - smaller_probe )
                {
                    *out_coordinate = smaller_probe;
                }
                else
                {
                    *out_coordinate = greater_probe;
                }
            }
        }
    }

    TRACE_END();
}

void pencil_relationship_layouter_layout_standard( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* determine visibility */
    uint32_t count_relations;
    count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_SHOW );
    }

    pencil_relationship_layouter_private_do_layout ( this_ );

    TRACE_END();
}

void pencil_relationship_layouter_layout_void( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* hide all relationships */
    uint32_t count_relations;
    count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
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

    /* get draw area */
    geometry_rectangle_t *diagram_draw_area;
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
        diagram_draw_area = layout_diagram_get_draw_area_ptr( diagram_layout );
    }

    /* layout the relationships */
    uint32_t count_relations;
    count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    TRACE_INFO_INT ( "count_relations:", count_relations );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        /* get the relationship to layout */
        layout_relationship_t *the_relationship;
        the_relationship = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

        /* determine visibility */
        if ( ( NULL == layout_relationship_get_from_feature_ptr ( the_relationship ) )
            && ( NULL == layout_relationship_get_to_feature_ptr ( the_relationship ) ) )
        {
            /* this is a globally visible relation, not local/scenario-based */
            pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
        }
        else
        {
            pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_SHOW );
        }

        /* calculate layout */
        {
            /* determine y-coordinate */
            const data_relationship_t *the_relationdata = layout_relationship_get_data_ptr( the_relationship );
            int32_t list_order = data_relationship_get_list_order ( the_relationdata );
            double y_value_rel = (list_order/((double)UINT32_MAX))+0.5;
            double draw_top = geometry_rectangle_get_top(diagram_draw_area);
            double draw_bottom = geometry_rectangle_get_bottom(diagram_draw_area);
            double y_value = ( (draw_bottom - draw_top) * y_value_rel ) + draw_top;

            /* get source and destination rectangles */
            const geometry_rectangle_t *source_rect;
            const geometry_rectangle_t *dest_rect;
            source_rect = layout_relationship_get_from_bounds_const ( the_relationship );
            dest_rect = layout_relationship_get_to_bounds_const ( the_relationship );

            /* calculate coordinates */
            /*double src_left = geometry_rectangle_get_left(source_rect);*/
            double src_x_center = geometry_rectangle_get_x_center(source_rect);
            /*double src_right = geometry_rectangle_get_right(source_rect);*/
            double src_top = geometry_rectangle_get_top(source_rect);
            /*double src_y_center = geometry_rectangle_get_y_center(source_rect);*/
            double src_bottom = geometry_rectangle_get_bottom(source_rect);

            /*double dst_left = geometry_rectangle_get_left(dest_rect);*/
            double dst_x_center = geometry_rectangle_get_x_center(dest_rect);
            /*double dst_right = geometry_rectangle_get_right(dest_rect);*/
            double dst_top = geometry_rectangle_get_top(dest_rect);
            /*double dst_y_center = geometry_rectangle_get_y_center(dest_rect);*/
            double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

            double src_y_value = ( y_value < src_top ) ? src_top : ( y_value > src_bottom ) ? src_bottom : y_value;
            double dst_y_value = ( y_value < dst_top ) ? dst_top : ( y_value > dst_bottom ) ? dst_bottom : y_value;

            /* define relation */
            geometry_connector_t *relationship_shape;
            relationship_shape = layout_relationship_get_shape_ptr( the_relationship );
            geometry_connector_init_horizontal ( relationship_shape,
                                                 src_x_center,
                                                 src_y_value,
                                                 dst_x_center,
                                                 dst_y_value,
                                                 y_value
                                               );
        }
    }

    TRACE_END();
}

void pencil_relationship_layouter_layout_for_timing( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* get draw area */
    geometry_rectangle_t *diagram_draw_area;
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
        diagram_draw_area = layout_diagram_get_draw_area_ptr( diagram_layout );
    }

    /* layout the relationships */
    uint32_t count_relations;
    count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    TRACE_INFO_INT ( "count_relations:", count_relations );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        /* get the relationship to layout */
        layout_relationship_t *the_relationship;
        the_relationship = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

        /* determine visibility */
        if ( ( NULL == layout_relationship_get_from_feature_ptr ( the_relationship ) )
            && ( NULL == layout_relationship_get_to_feature_ptr ( the_relationship ) ) )
        {
            /* this is a globally visible relation, not local/scenario-based */
            pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
        }
        else
        {
            pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_SHOW );
        }

        /* calculate layout */
        {
            /* determine x-coordinate */
            const data_relationship_t *the_relationdata = layout_relationship_get_data_ptr( the_relationship );
            int32_t list_order = data_relationship_get_list_order ( the_relationdata );
            double x_value_rel = (list_order/((double)UINT32_MAX))+0.5;
            double draw_left = geometry_rectangle_get_left(diagram_draw_area);
            double draw_right = geometry_rectangle_get_right(diagram_draw_area);
            double x_value = ( (draw_right - draw_left) * x_value_rel ) + draw_left;

            /* get source and destination rectangles */
            const geometry_rectangle_t *source_rect;
            const geometry_rectangle_t *dest_rect;
            source_rect = layout_relationship_get_from_bounds_const ( the_relationship );
            dest_rect = layout_relationship_get_to_bounds_const ( the_relationship );

            /* calculate coordinates */
            double src_left = geometry_rectangle_get_left(source_rect);
            /*double src_x_center = geometry_rectangle_get_x_center(source_rect);*/
            double src_right = geometry_rectangle_get_right(source_rect);
            /*double src_top = geometry_rectangle_get_top(source_rect);*/
            double src_y_center = geometry_rectangle_get_y_center(source_rect);
            /*double src_bottom = geometry_rectangle_get_bottom(source_rect);*/

            double dst_left = geometry_rectangle_get_left(dest_rect);
            /*double dst_x_center = geometry_rectangle_get_x_center(dest_rect);*/
            double dst_right = geometry_rectangle_get_right(dest_rect);
            /*double dst_top = geometry_rectangle_get_top(dest_rect);*/
            double dst_y_center = geometry_rectangle_get_y_center(dest_rect);
            /*double dst_bottom = geometry_rectangle_get_bottom(dest_rect);*/

            double src_x_value = ( x_value < src_left ) ? src_left : ( x_value > src_right ) ? src_right : x_value;
            double dst_x_value = ( x_value < dst_left ) ? dst_left : ( x_value > dst_right ) ? dst_right : x_value;

            /* define relation */
            geometry_connector_t *relationship_shape;
            relationship_shape = layout_relationship_get_shape_ptr( the_relationship );
            geometry_connector_init_vertical ( relationship_shape,
                                               src_x_value,
                                               src_y_center,
                                               dst_x_value,
                                               dst_y_center,
                                               x_value
                                             );
        }
    }

    TRACE_END();
}

void pencil_relationship_layouter_layout_for_communication( pencil_relationship_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* hide some relationships */
    uint32_t count_relations;
    count_relations = pencil_layout_data_get_relationship_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        layout_relationship_t *the_relationship;
        the_relationship = pencil_layout_data_get_relationship_ptr ( (*this_).layout_data, index );

        /* determine visibility */
        if ( ( NULL == layout_relationship_get_from_feature_ptr ( the_relationship ) )
            && ( NULL == layout_relationship_get_to_feature_ptr ( the_relationship ) ) )
        {
            /* this is a globally visible relation, not local/scenario-based */
            pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_IMPLICIT );
        }
        else
        {
            pencil_layout_data_set_relationship_visibility ( (*this_).layout_data, index, PENCIL_VISIBILITY_SHOW );
        }
    }

    /* layout the visible relationships */
    pencil_relationship_layouter_private_do_layout ( this_ );

    TRACE_END();
}


/*
Copyright 2017-2019 Andreas Warnke

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
