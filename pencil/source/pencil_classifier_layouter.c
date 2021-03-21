/* File: pencil_classifier_layouter.c; Copyright and License: see below */

#include "pencil_classifier_layouter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_classifier_layouter_init( pencil_classifier_layouter_t *this_,
                                      pencil_layout_data_t *layout_data,
                                      const pencil_size_t *pencil_size,
                                      geometry_dimensions_t *default_classifier_size,
                                      geometry_non_linear_scale_t *x_scale,
                                      geometry_non_linear_scale_t *y_scale,
                                      pencil_feature_layouter_t *feature_layouter )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );
    assert( NULL != default_classifier_size );
    assert( NULL != x_scale );
    assert( NULL != y_scale );
    assert( NULL != feature_layouter );

    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;
    (*this_).default_classifier_size = default_classifier_size;
    (*this_).x_scale = x_scale;
    (*this_).y_scale = y_scale;
    (*this_).feature_layouter = feature_layouter;
    pencil_classifier_composer_init( &((*this_).classifier_composer) );

    /* get draw area */
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = pencil_layout_data_get_diagram_ptr( (*this_).layout_data );
        (*this_).diagram_draw_area = layout_diagram_get_draw_area_ptr( diagram_layout );
    }

    TRACE_END();
}

void pencil_classifier_layouter_destroy( pencil_classifier_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_composer_destroy( &((*this_).classifier_composer) );

    TRACE_END();
}

/* ================================ INITIAL LAYOUT ================================ */

void pencil_classifier_layouter_estimate_bounds ( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* store the classifier bounds into input_data_layouter_t */
    const uint32_t count_clasfy = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        layout_visible_classifier_t *classifier_layout;
        classifier_layout = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );
        const data_visible_classifier_t *const visible_classifier2
            = layout_visible_classifier_get_data_const ( classifier_layout );

        /* set the bounds, space and label_box of the classifier layout */
        {
            geometry_dimensions_t features_dim;
            geometry_dimensions_init_empty( &features_dim );
            pencil_feature_layouter_calculate_features_bounds ( (*this_).feature_layouter,
                                                                layout_visible_classifier_get_diagramelement_id( classifier_layout ),
                                                                font_layout,
                                                                &features_dim
                                                              );

            const bool has_contained_children = false;  /* if classifier has children, this will be updated later when calling pencil_classifier_composer_set_space_and_label */
            pencil_classifier_composer_set_all_bounds ( &((*this_).classifier_composer),
                                                        visible_classifier2,
                                                        (*this_).default_classifier_size,
                                                        &features_dim,
                                                        has_contained_children,  
                                                        (*this_).pencil_size,
                                                        font_layout,
                                                        classifier_layout
                                                      );

            geometry_dimensions_destroy( &features_dim );
        }

        /* move the classifier rectangles to the target location */
        {
            const data_classifier_t *classifier2;
            classifier2 = data_visible_classifier_get_classifier_const( visible_classifier2 );
            const geometry_rectangle_t *const classifier_symbol_box
                = layout_visible_classifier_get_symbol_box_const( classifier_layout );

            const double act_center_x = geometry_rectangle_get_center_x( classifier_symbol_box );
            const double act_center_y = geometry_rectangle_get_center_y( classifier_symbol_box );
            const int32_t order_x = data_classifier_get_x_order( classifier2 );
            const int32_t order_y = data_classifier_get_y_order( classifier2 );
            const double center_x = geometry_non_linear_scale_get_location( (*this_).x_scale, order_x );
            const double center_y = geometry_non_linear_scale_get_location( (*this_).y_scale, order_y );
            layout_visible_classifier_shift( classifier_layout, center_x-act_center_x, center_y-act_center_y );
        }
    }

    TRACE_END();
}

/* ================================ MOVE TO AVOID OVERLAPS ================================ */

void pencil_classifier_layouter_move_to_avoid_overlaps ( pencil_classifier_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the classifiers by their movement-needs */
    pencil_classifier_layouter_private_propose_move_processing_order ( this_, &sorted );

    /* move the classifiers */
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 6;
        double solution_move_dx[6];
        double solution_move_dy[6];

        /* propose options of moving left/right/up/down */
        pencil_classifier_layouter_private_propose_4dir_move_solutions ( this_,
                                                                         &sorted,
                                                                         sort_index,
                                                                         SOLUTIONS_MAX-1,
                                                                         solution_move_dx,
                                                                         solution_move_dy,
                                                                         &solutions_count
                                                                       );
        /* propose options of moving close at origin-area */
        pencil_classifier_layouter_private_propose_anchored_solution ( this_,
                                                                       &sorted,
                                                                       sort_index,
                                                                       &(solution_move_dx[solutions_count]),
                                                                       &(solution_move_dy[solutions_count])
                                                                     );
        solutions_count ++;

        /* select best option */
        uint32_t index_of_best;
        if ( 1 == solutions_count )
        {
            index_of_best = 0;
        }
        else
        {
            pencil_classifier_layouter_private_select_move_solution ( this_,
                                                                      &sorted,
                                                                      sort_index,
                                                                      solutions_count,
                                                                      solution_move_dx,
                                                                      solution_move_dy,
                                                                      &index_of_best
                                                                    );
        }

        /* perform best option */
        uint32_t index;
        index = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        /* move the classifier */
        layout_visible_classifier_t *the_classifier;
        the_classifier = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, index );
        layout_visible_classifier_shift( the_classifier, solution_move_dx[index_of_best], solution_move_dy[index_of_best] );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_classifier_layouter_private_propose_move_processing_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) DATA_VISIBLE_SET_MAX_CLASSIFIERS );

    /* sort the classifiers by their movement-needs */
    uint32_t count_clasfy;
    count_clasfy = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        const layout_visible_classifier_t *const the_classifier
            = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, index );
        const geometry_rectangle_t *const classifier_symbol_box
            = layout_visible_classifier_get_symbol_box_const( the_classifier );

        int64_t simpleness = 0;  /* the lower the number, the ealier the classifier will be processed. Unit is area(=square-length). */

        /* reduce simpleness by area outside the diagram border: the more outside diagram area, the earlier it should be moved */
        {
            geometry_rectangle_t border_intersect;
            int intersect_error2;
            intersect_error2 = geometry_rectangle_init_by_intersect( &border_intersect, classifier_symbol_box, (*this_).diagram_draw_area );
            if ( 0 != intersect_error2 )
            {
                TSLOG_WARNING( "a rectangle to be drawn is completely outside the diagram area" );
            }

            simpleness += 16.0 * geometry_rectangle_get_area( &border_intersect );
            simpleness -= 16.0 * geometry_rectangle_get_area( classifier_symbol_box );

            geometry_rectangle_destroy( &border_intersect );
        }

        /* reduce simpleness by intersects with other rectangles: the more intersects, the earlier it should be moved */
        for ( uint32_t probe_index = 0; probe_index < count_clasfy; probe_index ++ )
        {
            layout_visible_classifier_t *probe_classifier;
            probe_classifier = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, probe_index );
            const geometry_rectangle_t *const probe_symbol_box
                = layout_visible_classifier_get_symbol_box_const( probe_classifier );

            geometry_rectangle_t intersect;
            int intersect_error;
            intersect_error = geometry_rectangle_init_by_intersect( &intersect, classifier_symbol_box, probe_symbol_box );

            if ( 0 == intersect_error )
            {
                simpleness -= geometry_rectangle_get_area( &intersect );
            }

            geometry_rectangle_destroy( &intersect );
        }

        /* reduce simpleness by own size: the bigger the object, the earlier it should be moved */
        {
            const double default_classifier_area = geometry_dimensions_get_area( (*this_).default_classifier_size );
            const double classifier_area = geometry_rectangle_get_area( classifier_symbol_box );
            if (( default_classifier_area > 0.000000001 )&&( classifier_area > 0.000000001 ))
            {
                simpleness -= default_classifier_area * ( classifier_area / ( classifier_area + default_classifier_area ));
            }
        }

        /* increase simpleness if contained children: if embracing children later, layouting problems might get solved */
        {
            const double default_classifier_area = geometry_dimensions_get_area( (*this_).default_classifier_size );
            const uint32_t descendant_count
                = pencil_layout_data_count_descendants( (*this_).layout_data, the_classifier );
            if ( descendant_count != 0 )
            {
                simpleness += default_classifier_area;
            }
        }

        int insert_error;
        insert_error = universal_array_index_sorter_insert( out_sorted, index, simpleness );
        if ( 0 != insert_error )
        {
            TSLOG_WARNING( "not all rectangles are moved" );
        }
    }

    TRACE_END();
}

/*!
 *  \brief constants for directions of moving objects
 */
enum pencil_classifier_layouter_private_move_enum {
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_NOT = 0,  /*!< only move to visible arey - nothing more */
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_UP_MIN = 1,  /*!< moves up the minimum distance (up means smaller y-values) */
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_DOWN_MIN = 2,
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_LEFT_MIN = 3,
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN = 4,
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_MAX = 5,  /*!< constant defining the total number of available options */
};

void pencil_classifier_layouter_private_propose_4dir_move_solutions ( pencil_classifier_layouter_t *this_,
                                                                      const universal_array_index_sorter_t *sorted,
                                                                      uint32_t sort_index,
                                                                      uint32_t solutions_max,
                                                                      double out_solution_move_dx[],
                                                                      double out_solution_move_dy[],
                                                                      uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != out_solution_move_dx );
    assert ( NULL != out_solution_move_dy );
    assert ( NULL != out_solutions_count );
    assert ( 1 <= solutions_max );  /* general requirement to report at least one option */
    assert ( (unsigned int) PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_MAX <= solutions_max );  /* current implementation requires at least 5 options */

    /* get classifier to move */
    const uint32_t index
        = universal_array_index_sorter_get_array_index( sorted, sort_index );
    const layout_visible_classifier_t *const the_classifier
        = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, index );
    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( the_classifier );
    double top = geometry_rectangle_get_top ( classifier_symbol_box );
    double bottom = geometry_rectangle_get_bottom ( classifier_symbol_box );
    double left = geometry_rectangle_get_left ( classifier_symbol_box );
    double right = geometry_rectangle_get_right ( classifier_symbol_box );

    /* choose distance */
    double shift_x = 0.0;
    double shift_y = 0.0;

    /* check overlap to diagram boundary */
    {
        if ( bottom > geometry_rectangle_get_bottom( (*this_).diagram_draw_area ) )
        {
            shift_y = geometry_rectangle_get_bottom( (*this_).diagram_draw_area ) - bottom;
        }
        if ( top < geometry_rectangle_get_top( (*this_).diagram_draw_area ) )
        {
            shift_y = geometry_rectangle_get_top( (*this_).diagram_draw_area ) - top;
        }
        if ( right > geometry_rectangle_get_right( (*this_).diagram_draw_area ) )
        {
            shift_x = geometry_rectangle_get_right( (*this_).diagram_draw_area ) - right;
        }
        if ( left < geometry_rectangle_get_left( (*this_).diagram_draw_area ) )
        {
            shift_x = geometry_rectangle_get_left( (*this_).diagram_draw_area ) - left;
        }
    }

    *out_solutions_count = 1;
    out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_NOT] = shift_x;
    out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_NOT] = shift_y;

    /* determine minimum and comfort distances between classifiers */
    const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size );

    out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_UP_MIN] = shift_x;
    out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_UP_MIN] = shift_y;
    out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_DOWN_MIN] = shift_x;
    out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_DOWN_MIN] = shift_y;
    out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_LEFT_MIN] = shift_x;
    out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_LEFT_MIN] = shift_y;
    out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN] = shift_x;
    out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN] = shift_y;

    /* adjust information on current rectangle */
    top += shift_y;
    bottom += shift_y;
    left += shift_x;
    right += shift_x;

    /* check overlap to already moved classifiers */
    for ( uint32_t probe_sort_index = 0; probe_sort_index < sort_index; probe_sort_index ++ )
    {
        /* get classifier to check overlaps */
        uint32_t probe_index;
        probe_index = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );
        const layout_visible_classifier_t *const the_probe
            = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, probe_index );
        const geometry_rectangle_t *const probe_symbol_box
            = layout_visible_classifier_get_symbol_box_const( the_probe );
        const double probe_top = geometry_rectangle_get_top ( probe_symbol_box );
        const double probe_bottom = geometry_rectangle_get_bottom ( probe_symbol_box );
        const double probe_left = geometry_rectangle_get_left ( probe_symbol_box );
        const double probe_right = geometry_rectangle_get_right ( probe_symbol_box );

        if ( probe_right < left )
        {
            /* no overlap, finished. */
        }
        else if ( probe_left > right )
        {
            /* no overlap, finished. */
        }
        else if ( probe_bottom < top )
        {
            /* no overlap, finished. */
        }
        else if ( probe_top > bottom )
        {
            /* no overlap, finished. */
        }
        else if ( pencil_layout_data_is_ancestor( (*this_).layout_data, the_probe, the_classifier ) )
        {
            /* overlapping the parent is ok, finished */
        }
        else if ( pencil_layout_data_is_ancestor( (*this_).layout_data, the_classifier, the_probe ) )
        {
            /* overlapping the child is ok, finished */
        }
        else
        {
            /* there is an overlap - at least when considering the comfort zone */

            double my_shift_x_left_min;
            my_shift_x_left_min = probe_left - right - gap;
            if ( my_shift_x_left_min < out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_LEFT_MIN] )
            {
                out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_LEFT_MIN] = my_shift_x_left_min;
            }

            double my_shift_x_right_min;
            my_shift_x_right_min = probe_right - left + gap;
            if ( my_shift_x_right_min > out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN] )
            {
                out_solution_move_dx[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN] = my_shift_x_right_min;
            }

            double my_shift_y_up_min;
            my_shift_y_up_min = probe_top - bottom - gap;
            if ( my_shift_y_up_min < out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_UP_MIN] )
            {
                out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_UP_MIN] = my_shift_y_up_min;
            }

            double my_shift_y_down_min;
            my_shift_y_down_min = probe_bottom - top + gap;
            if ( my_shift_y_down_min > out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_DOWN_MIN] )
            {
                out_solution_move_dy[PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_DOWN_MIN] = my_shift_y_down_min;
            }

            *out_solutions_count = PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_MAX;

            /* trace */
            const data_visible_classifier_t *visible_classifier_data;
            visible_classifier_data = layout_visible_classifier_get_data_const ( the_probe );
            if (( visible_classifier_data != NULL ) && ( data_visible_classifier_is_valid( visible_classifier_data ) ))
            {
                const data_classifier_t *classifier_p;
                classifier_p = data_visible_classifier_get_classifier_const( visible_classifier_data );
                TRACE_INFO_STR( "- overlaps:", data_classifier_get_name_ptr( classifier_p ) );
            }
        }
    }

    /* trace */
    const data_visible_classifier_t *visible_classifier;
    visible_classifier = layout_visible_classifier_get_data_const ( the_classifier );
    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    TRACE_INFO_STR( "classifier:", data_classifier_get_name_ptr( classifier ) );

    TRACE_END();
}

void pencil_classifier_layouter_private_propose_anchored_solution ( pencil_classifier_layouter_t *this_,
                                                                    const universal_array_index_sorter_t *sorted,
                                                                    uint32_t sort_index,
                                                                    double * out_solution_move_dx,
                                                                    double * out_solution_move_dy )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != out_solution_move_dx );
    assert ( NULL != out_solution_move_dy );

    /* get classifier to move */
    const uint32_t index
        = universal_array_index_sorter_get_array_index( sorted, sort_index );
    const layout_visible_classifier_t *const the_classifier
        = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, index );

    /* determine the user-selected center */
    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( the_classifier );

    /* determine the space needed for the solution */
    geometry_rectangle_t classifier_total_bounds;
    geometry_rectangle_init_by_bounds( &classifier_total_bounds,
                                       classifier_symbol_box,
                                       layout_visible_classifier_get_label_box_const( the_classifier )
                                     );
    const double width = geometry_rectangle_get_width( &classifier_total_bounds );
    const double height = geometry_rectangle_get_height( &classifier_total_bounds );
    const double gap = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* wish a solution area */
    geometry_rectangle_t classifier_solution_area;
    geometry_rectangle_init ( &classifier_solution_area,
                              geometry_rectangle_get_left( &classifier_total_bounds ) - 0.5*width - gap,
                              geometry_rectangle_get_top( &classifier_total_bounds ) - 0.5*height - gap,
                              2.0*width + 2.0*gap,
                              2.0*height + 2.0*gap
                            );

    /* shrink solution area to diagram_draw_area */
    geometry_rectangle_init_by_intersect( &classifier_solution_area,
                                          &classifier_solution_area,
                                          (*this_).diagram_draw_area
                                        );

    /* check overlap to already moved classifiers */
    for ( uint32_t probe_sort_index = 0; probe_sort_index < sort_index; probe_sort_index ++ )
    {
        /* get classifier to check overlaps */
        uint32_t probe_index;
        probe_index = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );
        const layout_visible_classifier_t *const the_probe
            = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, probe_index );

        geometry_rectangle_t probe_total_bounds;
        geometry_rectangle_init_by_bounds( &probe_total_bounds,
                                           layout_visible_classifier_get_label_box_const( the_probe ),
                                           layout_visible_classifier_get_symbol_box_const( the_probe )
                                         );

        geometry_rectangle_init_by_difference( &classifier_solution_area,
                                               &classifier_solution_area,
                                               &probe_total_bounds
                                             );
    }

    /* reduce the biggest free/unoccupied box by gap */
    geometry_rectangle_shift ( &classifier_solution_area, gap, gap );
    geometry_rectangle_expand ( &classifier_solution_area, -2.0*gap, -2.0*gap );

    /* move - but not to eager - only the minumum distance */
    const bool is_x_contained
        = ( geometry_rectangle_get_left( &classifier_solution_area ) < geometry_rectangle_get_left( &classifier_total_bounds ) )
        && ( geometry_rectangle_get_right( &classifier_total_bounds ) < geometry_rectangle_get_right( &classifier_solution_area ) );
    const bool is_y_contained
        = ( geometry_rectangle_get_top( &classifier_solution_area ) < geometry_rectangle_get_top( &classifier_total_bounds ) )
        && ( geometry_rectangle_get_bottom( &classifier_total_bounds ) < geometry_rectangle_get_bottom( &classifier_solution_area ) );
    if ( is_x_contained )
    {
        *out_solution_move_dx = 0.0;
    }
    else
    {
        const double sol_center_x = geometry_rectangle_get_center_x( &classifier_solution_area );
        const double cur_center_x = geometry_rectangle_get_center_x( &classifier_total_bounds );
        *out_solution_move_dx = ( sol_center_x < cur_center_x )
            ? geometry_rectangle_get_right( &classifier_solution_area )
            - geometry_rectangle_get_right( &classifier_total_bounds )
            : geometry_rectangle_get_left( &classifier_solution_area )
            - geometry_rectangle_get_left( &classifier_total_bounds );
        geometry_rectangle_trace( &classifier_solution_area );
        geometry_rectangle_trace( &classifier_total_bounds );
    }
    if ( is_y_contained )
    {
        *out_solution_move_dy = 0.0;
    }
    else
    {
        const double sol_center_y = geometry_rectangle_get_center_y( &classifier_solution_area );
        const double cur_center_y = geometry_rectangle_get_center_y( &classifier_total_bounds );
        *out_solution_move_dy = ( sol_center_y < cur_center_y )
            ? geometry_rectangle_get_bottom( &classifier_solution_area )
            - geometry_rectangle_get_bottom( &classifier_total_bounds )
            : geometry_rectangle_get_top( &classifier_solution_area )
            - geometry_rectangle_get_top( &classifier_total_bounds );
    }

    /* trace */
    const data_visible_classifier_t *visible_classifier;
    visible_classifier = layout_visible_classifier_get_data_const ( the_classifier );
    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    TRACE_INFO_STR( "classifier:", data_classifier_get_name_ptr( classifier ) );

    TRACE_END();
}

void pencil_classifier_layouter_private_select_move_solution ( pencil_classifier_layouter_t *this_,
                                                               const universal_array_index_sorter_t *sorted,
                                                               uint32_t sort_index,
                                                               uint32_t solutions_count,
                                                               const double solution_move_dx[],
                                                               const double solution_move_dy[],
                                                               uint32_t *out_index_of_best )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != solution_move_dx );
    assert ( NULL != solution_move_dy );
    assert ( NULL != out_index_of_best );
    assert ( 1 <= solutions_count );

    /* define potential solution and rating */
    uint32_t index_of_best;
    double debts_of_best;
    index_of_best = 0;  /* in case of doubts, take the first solution */
    debts_of_best = 1000000000.0;

    /* get classifier to move */
    const uint32_t index
        = universal_array_index_sorter_get_array_index( sorted, sort_index );
    const layout_visible_classifier_t *const the_classifier
        = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, index );
    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( the_classifier );

    /* check all solutions */
    for ( uint32_t solution_index = 0; solution_index < solutions_count; solution_index ++ )
    {
        /* calculate the solution rectangle */
        geometry_rectangle_t solution_bounds;
        geometry_rectangle_copy( &solution_bounds, classifier_symbol_box );
        geometry_rectangle_shift ( &solution_bounds, solution_move_dx[solution_index], solution_move_dy[solution_index] );

        /* evalute the debts of this solution */
        double debts_of_current;
        debts_of_current = 0.0;

        /* add move distance as debt */
        debts_of_current += fabs ( solution_move_dx[solution_index] );
        debts_of_current += fabs ( solution_move_dy[solution_index] );

        /* add debts for overlap to diagram boundary */
        {
            static const double DIAG_BOUNDS_SEVERITY = 32.0;

            double current_area = geometry_rectangle_get_area ( &solution_bounds );
            geometry_rectangle_t intersect;
            geometry_rectangle_init_by_intersect( &intersect, &solution_bounds, (*this_).diagram_draw_area );
            double intersect_area = geometry_rectangle_get_area ( &intersect );

            debts_of_current += DIAG_BOUNDS_SEVERITY * ( current_area - intersect_area );
        }

        /* check overlap to other classifiers */
        for ( uint32_t probe_sort_index = 0; probe_sort_index < universal_array_index_sorter_get_count( sorted ); probe_sort_index ++ )
        {
            if ( probe_sort_index != sort_index )  /* skip self */
            {
                /* get classifier to check overlaps */
                uint32_t probe_index;
                probe_index = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );

                const layout_visible_classifier_t *const the_probe
                    = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, probe_index );
                const geometry_rectangle_t *const probe_symbol_box
                    = layout_visible_classifier_get_symbol_box_const( the_probe );

                geometry_rectangle_t probe_intersect;
                int intersect_err;
                intersect_err = geometry_rectangle_init_by_intersect( &probe_intersect, &solution_bounds, probe_symbol_box );
                if ( 0 == intersect_err )
                {
                    /* there is an intersect */
                    if ( pencil_layout_data_is_ancestor( (*this_).layout_data, the_classifier, the_probe ) )
                    {
                        /* no debt: parent my overlap children */
                    }
                    else if ( pencil_layout_data_is_ancestor( (*this_).layout_data, the_probe, the_classifier ) )
                    {
                        /* no debt: child may overlap parent */
                    }
                    else
                    {
                        /* already processed classifiers have higher severity because these do not move anymore */
                        double severity = ( probe_sort_index < sort_index ) ? 4.0 : 1.0;
                        double probe_intersect_area = geometry_rectangle_get_area ( &probe_intersect );
                        debts_of_current += severity * probe_intersect_area;
                    }
                }
                /* else no intersect/overlap */
            }
        }

        /* finish evaluating this solution */
        geometry_rectangle_destroy( &solution_bounds );
        if ( debts_of_current < debts_of_best )
        {
            debts_of_best = debts_of_current;
            index_of_best = solution_index;
        }
    }

    *out_index_of_best = index_of_best;

    TRACE_END();
}

/* ================================ EMBRACE CHILDREN STEP BY STEP ================================ */

void pencil_classifier_layouter_embrace_children( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();
    assert( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );

    universal_array_index_sorter_t sorted_relationships;
    universal_array_index_sorter_init( &sorted_relationships );

    /* sort the relationships by their number of descendants */
    pencil_classifier_layouter_private_propose_embracing_order ( this_, &sorted_relationships );

    /* init the set of classifiers that has embraced children */
    data_small_set_t has_embraced_children;
    data_small_set_init( &has_embraced_children );

    /* move the classifiers */
    const uint32_t count_sorted = universal_array_index_sorter_get_count( &sorted_relationships );
    for ( uint32_t rel_sort_idx = 0; rel_sort_idx < count_sorted; rel_sort_idx ++ )
    {
        const uint32_t rel_idx
            = universal_array_index_sorter_get_array_index( &sorted_relationships, rel_sort_idx );
        layout_relationship_t *const the_relationship
            = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, rel_idx );
        assert ( the_relationship != NULL );
        const data_relationship_t *const rel_data = layout_relationship_get_data_const ( the_relationship );
        assert ( rel_data != NULL );
        const data_id_t rel_from_id = data_relationship_get_from_classifier_data_id ( rel_data );

        const int failure
            = pencil_classifier_layouter_private_try_embrace_child( this_,
                                                                    the_relationship,
                                                                    ! data_small_set_contains( &has_embraced_children, rel_from_id )
                                                                  );
        if ( ! failure )
        {
            data_small_set_add_obj( &has_embraced_children, rel_from_id );

            /* re-calculate the label-box and thereby update the space-box of the parent */
            layout_visible_classifier_t *const from_classifier
                = layout_relationship_get_from_classifier_ptr( the_relationship );
            const bool has_contained_children = true;
            pencil_classifier_composer_set_space_and_label( &((*this_).classifier_composer),
                                                            layout_visible_classifier_get_data_const( from_classifier ),
                                                            has_contained_children,
                                                            (*this_).pencil_size,
                                                            font_layout,
                                                            from_classifier
                                                          );
        }
    }

    data_small_set_destroy( &has_embraced_children );

    universal_array_index_sorter_destroy( &sorted_relationships );

    TRACE_END();
}

void pencil_classifier_layouter_private_propose_embracing_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert( NULL != out_sorted );

    const uint32_t rel_count = pencil_layout_data_get_relationship_count( (*this_).layout_data );
    for ( uint32_t rel_idx = 0; rel_idx < rel_count; rel_idx ++ )
    {
        const layout_relationship_t *const the_relationship
            = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, rel_idx );

        /* count the descendants */
        const layout_visible_classifier_t *const from_classifier
            = layout_relationship_get_from_classifier_ptr( the_relationship );
        const uint32_t from_descendant_count
            = pencil_layout_data_count_descendants( (*this_).layout_data, from_classifier );

        /* sort it into the array by the number of decendants: */
        /* the less descendants the earlier it shall be processed. */
        int err;
        err = universal_array_index_sorter_insert( out_sorted, rel_idx, (double)from_descendant_count );
        if ( 0 != err )
        {
            TSLOG_ERROR ( "universal_array_index_sorter_t list is full." );
        }
    }

    TRACE_END();
}

int pencil_classifier_layouter_private_try_embrace_child( pencil_classifier_layouter_t *this_, layout_relationship_t *the_relationship, bool move )
{
    TRACE_BEGIN();
    assert( NULL != the_relationship );
    int result_err = -1;

    const data_relationship_type_t the_type
        = data_relationship_get_main_type ( layout_relationship_get_data_const( the_relationship ));

    if ( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == the_type )
    {
        layout_visible_classifier_t *const from_classifier
            = layout_relationship_get_from_classifier_ptr( the_relationship );
        const layout_visible_classifier_t *const to_classifier
            = layout_relationship_get_to_classifier_ptr( the_relationship );
        if ( from_classifier != to_classifier )
        {
            const geometry_rectangle_t parent_envelope = layout_visible_classifier_calc_envelope_box( from_classifier );
            const geometry_rectangle_t *const parent_space
                = layout_visible_classifier_get_space_const ( from_classifier );
            const geometry_rectangle_t child_envelope = layout_visible_classifier_calc_envelope_box( to_classifier );

            /* try embrace child */
            geometry_rectangle_t probe_parent_envelope;  /* try out a new parent symbol_box rectangle */
            geometry_rectangle_copy( &probe_parent_envelope, &parent_envelope );
            double extend_to_left = 0.0;
            double extend_to_right = 0.0;
            double extend_to_top = 0.0;
            double extend_to_bottom = 0.0;
            extend_to_left = geometry_rectangle_get_left( parent_space ) - geometry_rectangle_get_left( &child_envelope );
            extend_to_top = geometry_rectangle_get_top( parent_space ) - geometry_rectangle_get_top( &child_envelope );
            extend_to_right = geometry_rectangle_get_right( &child_envelope ) - geometry_rectangle_get_right( parent_space );
            extend_to_bottom = geometry_rectangle_get_bottom( &child_envelope ) - geometry_rectangle_get_bottom( parent_space );
            if ( ! move )
            {
                if ( extend_to_left < 0.0 )
                {
                    extend_to_left = 0.0;
                }
                if ( extend_to_top < 0.0 )
                {
                    extend_to_top = 0.0;
                }
                if ( extend_to_right < 0.0 )
                {
                    extend_to_right = 0.0;
                }
                if ( extend_to_bottom < 0.0 )
                {
                    extend_to_bottom = 0.0;
                }
            }
            geometry_rectangle_expand( &probe_parent_envelope, extend_to_left+extend_to_right, extend_to_top+extend_to_bottom );
            geometry_rectangle_shift( &probe_parent_envelope, -extend_to_left, -extend_to_top );

            /* check what else would be embraced */
            bool illegal_overlap = false;
            uint32_t count_clasfy;
            count_clasfy = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
            for ( uint32_t c_index = 0; c_index < count_clasfy; c_index ++ )
            {
                layout_visible_classifier_t *probe_classifier;
                probe_classifier = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, c_index );

                if (( probe_classifier != from_classifier )&&( probe_classifier != to_classifier ))
                {
                    if ( pencil_layout_data_is_ancestor ( (*this_).layout_data, from_classifier, probe_classifier ) )
                    {
                        /* it is ok to embrace also other children, no illegal_overlap */
                    }
                    else if ( pencil_layout_data_is_ancestor ( (*this_).layout_data, probe_classifier, from_classifier ) )
                    {
                        /* it is ok if parent is already contained in grand-parent classifier, no illegal_overlap */
                    }
                    else
                    {
                        const geometry_rectangle_t *const current_symbol_box
                            = layout_visible_classifier_get_symbol_box_const ( probe_classifier );
                        illegal_overlap |= geometry_rectangle_is_intersecting( current_symbol_box, &probe_parent_envelope );
                    }
                }
            }
            /* check overlap to diagram boundary */
            if ( ! geometry_rectangle_is_containing ( (*this_).diagram_draw_area, &probe_parent_envelope ) )
            {
                illegal_overlap = true;
            }

            /* cancel or commit */
            if ( ! illegal_overlap )
            {
                layout_visible_classifier_expand( from_classifier, extend_to_left+extend_to_right, extend_to_top+extend_to_bottom );
                layout_visible_classifier_shift( from_classifier, -extend_to_left, -extend_to_top );
                result_err = 0;
            }

            /* cleanup */
            geometry_rectangle_destroy( &probe_parent_envelope );
        }
        else
        {
            TRACE_INFO( "Classifier contains itself" );
        }
    }
    /* else this is not a parent child relationship */

    TRACE_END_ERR( result_err );
    return result_err;
}

/* ================================ EMBRACE CHILDREN COMMON ================================ */

void pencil_classifier_layouter_hide_relations_of_embraced_children( pencil_classifier_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* search containment relations */
    const uint32_t rel_count = pencil_layout_data_get_relationship_count( (*this_).layout_data );
    for ( uint32_t rel_idx = 0; rel_idx < rel_count; rel_idx ++ )
    {
        const layout_relationship_t *const the_relationship
            = pencil_layout_data_get_relationship_const( (*this_).layout_data, rel_idx );
        const data_relationship_t *const the_rel_data
            = layout_relationship_get_data_const( the_relationship );

        const data_relationship_type_t the_type = data_relationship_get_main_type ( the_rel_data );
        const pencil_visibility_t visibility = layout_relationship_get_visibility( the_relationship );

        if (( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == the_type )
            && (( PENCIL_VISIBILITY_SHOW == visibility )||(PENCIL_VISIBILITY_GRAY_OUT == visibility) ))
        {
            const layout_visible_classifier_t *const from_classifier
                = layout_relationship_get_from_classifier_ptr( the_relationship );
            const layout_visible_classifier_t *const to_classifier
                = layout_relationship_get_to_classifier_ptr( the_relationship );
            if ( from_classifier != to_classifier )
            {
                const geometry_rectangle_t *const parent_space
                    = layout_visible_classifier_get_space_const ( from_classifier );
                const geometry_rectangle_t *const child_symbol_box
                    = layout_visible_classifier_get_symbol_box_const ( to_classifier );

                /* hide if parent embraced child completely */
                if ( geometry_rectangle_is_containing( parent_space, child_symbol_box ) )
                {
                    pencil_layout_data_set_relationship_visibility( (*this_).layout_data, rel_idx, PENCIL_VISIBILITY_IMPLICIT );
                    TRACE_INFO( "Containment relation is PENCIL_VISIBILITY_IMPLICIT" );
                }
            }
        }
    }

    TRACE_END();
}

/* ================================ EMBRACE AND MOVE CHILDREN TOGETHER ================================ */

void pencil_classifier_layouter_move_and_embrace_children( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();
    assert( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS );

    const double TAKE_RATIO = (1.0/3.0);
    const double LEAVE_RATIO = (1.0-TAKE_RATIO);
    /* const double gap = pencil_size_get_preferred_object_distance( (*this_).pencil_size ); */

    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );

    /* sort the classifiers by their need to move and to embrace */
    pencil_classifier_layouter_private_propose_move_embrace_order ( this_, &sorted_classifiers );

    /* small-move and embrace the child classifiers */
    const uint32_t count_sorted = universal_array_index_sorter_get_count( &sorted_classifiers );
    for ( uint32_t classifier_sort_idx = 0; classifier_sort_idx < count_sorted; classifier_sort_idx ++ )
    {
        const uint32_t classifier_idx = universal_array_index_sorter_get_array_index( &sorted_classifiers, classifier_sort_idx );
        layout_visible_classifier_t * the_classifier = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, classifier_idx );

        /* only if the classifier has children */
        const uint32_t child_count = pencil_layout_data_count_descendants ( (*this_).layout_data, the_classifier );
        if ( child_count > 0 )
        {
            /* fetch data on parent classifier */
            const geometry_rectangle_t parent_envelope = layout_visible_classifier_calc_envelope_box( the_classifier );
            const geometry_rectangle_t *const parent_space = layout_visible_classifier_get_space_const( the_classifier );
            const double parent_space_width_diff
                = geometry_rectangle_get_width(&parent_envelope) - geometry_rectangle_get_width(parent_space);
            const double parent_space_height_diff
                = geometry_rectangle_get_height(&parent_envelope) - geometry_rectangle_get_height(parent_space);

            /* get envelope rectangle of all children */
            const geometry_rectangle_t children_envelope
                = pencil_classifier_layouter_private_calc_descendant_envelope( this_, the_classifier );

            /* determine outer space around envelope rectangle */
            const geometry_rectangle_t outer_space
                = pencil_classifier_layouter_private_calc_outer_space( this_, &children_envelope, the_classifier );

            const double parent_min_width = parent_space_width_diff + geometry_rectangle_get_width(&children_envelope);
            const double parent_min_height = parent_space_height_diff + geometry_rectangle_get_height(&children_envelope);

            /* if success, move children and parent */
            const double outer_border_x = (geometry_rectangle_get_width( &outer_space ) - parent_min_width)/2.0;
            const double outer_border_y = (geometry_rectangle_get_height( &outer_space ) - parent_min_height)/2.0;
            if (( outer_border_x > 0.0 )&&( outer_border_y > 0.0 ))
            {
                /* prepare to move+expand the parent */
                const double delta_x
                    = geometry_rectangle_get_left(&outer_space) - geometry_rectangle_get_left(&parent_envelope)
                    + (LEAVE_RATIO*outer_border_x);
                const double delta_y
                    = geometry_rectangle_get_top(&outer_space) - geometry_rectangle_get_top(&parent_envelope)
                    + (LEAVE_RATIO*outer_border_y);
                const double delta_width
                    = geometry_rectangle_get_width(&outer_space) - geometry_rectangle_get_width(&parent_envelope)
                    - 2.0*LEAVE_RATIO*outer_border_x;
                const double delta_height
                    = geometry_rectangle_get_height(&outer_space) - geometry_rectangle_get_height(&parent_envelope)
                    - 2.0*LEAVE_RATIO*outer_border_y;

                /* move+expand the parent */
                layout_visible_classifier_shift ( the_classifier, delta_x, delta_y );
                layout_visible_classifier_expand ( the_classifier, delta_width, delta_height );

                /* re-calculate the label-box and thereby update the space-box of the parent */
                const bool has_contained_children = true;
                pencil_classifier_composer_set_space_and_label( &((*this_).classifier_composer),
                                                                layout_visible_classifier_get_data_const( the_classifier ),
                                                                has_contained_children,
                                                                (*this_).pencil_size,
                                                                font_layout,
                                                                the_classifier
                                                              );

                /* determine the descendants move deltas */
                const geometry_rectangle_t *const parent_new_space = layout_visible_classifier_get_space_const( the_classifier );
                const double descendant_add_dx = geometry_rectangle_get_center_x( parent_new_space ) - geometry_rectangle_get_center_x( &children_envelope );
                const double descendant_add_dy = geometry_rectangle_get_center_y( parent_new_space ) - geometry_rectangle_get_center_y( &children_envelope );

                /* move the descendants */
                pencil_classifier_layouter_private_move_descendants( this_, the_classifier, descendant_add_dx, descendant_add_dy );
            }

        }
    }

    universal_array_index_sorter_destroy( &sorted_classifiers );

    TRACE_END();
}

void pencil_classifier_layouter_private_propose_move_embrace_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) DATA_VISIBLE_SET_MAX_CLASSIFIERS );

    /* sort the classifiers by their movement-needs */
    const uint32_t count_classifiers = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_classifiers; index ++ )
    {
        const layout_visible_classifier_t *const the_classifier = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, index );

        int64_t lazy_move = 0;

        /* grand-parents must be moved after parents (becasue parents are not yet well layouted) */
        const uint32_t child_count = pencil_layout_data_count_descendants ( (*this_).layout_data, the_classifier );
        lazy_move = child_count;

        int insert_error;
        insert_error = universal_array_index_sorter_insert( out_sorted, index, lazy_move );
        if ( 0 != insert_error )
        {
            TSLOG_WARNING( "not all rectangles are grown" );
        }
    }

    TRACE_END();
}

/* ================================ LAYOUT FOR SCENARIO ================================ */

void pencil_classifier_layouter_layout_for_list( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    /* const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size ); */

    /* get the draw coordinates */
    geometry_rectangle_t draw_area;
    geometry_rectangle_copy( &draw_area, (*this_).diagram_draw_area );
    geometry_rectangle_expand( &draw_area, 0.0, (- obj_dist) );
    double diag_w = geometry_rectangle_get_width( &draw_area );
    double diag_h = geometry_rectangle_get_height( &draw_area );

    const double phi = 1.6180339; /* b=0.618, a=1.0, a+b=1.618 => ((a+b)/a)==(a/b) */
    const double golden_ratio_width = diag_w/phi;
    const double golden_ratio_height = diag_h/phi;

    /* calculate preferred classifier bounds/envelopes */
    const uint_fast32_t c_count = pencil_layout_data_get_visible_classifier_count( (*this_).layout_data );
    for ( uint_fast32_t plain_idx = 0; plain_idx < c_count; plain_idx ++ )
    {
        layout_visible_classifier_t *visible_classifier1;
        visible_classifier1 = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, plain_idx );
        
        /* set the preferred bounds, space and label_box of the classifier layout */
        {
            const data_visible_classifier_t *const visible_classifier_data
                = layout_visible_classifier_get_data_const ( visible_classifier1 );
            geometry_dimensions_t preferred_dim;
            geometry_dimensions_init( &preferred_dim, golden_ratio_width, (golden_ratio_height/c_count) );
            geometry_dimensions_t features_dim;
            geometry_dimensions_init_empty( &features_dim );

            const bool has_contained_children = false;  /* this diagram type does not embrace children */
            pencil_classifier_composer_set_all_bounds( &((*this_).classifier_composer),
                                                       visible_classifier_data,
                                                       &preferred_dim,
                                                       &features_dim,
                                                       has_contained_children,  
                                                       (*this_).pencil_size,
                                                       font_layout,
                                                       visible_classifier1
                                                     );

            geometry_dimensions_destroy( &features_dim );
            geometry_dimensions_destroy( &preferred_dim );
        }
    }
    
    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );
    pencil_classifier_layouter_private_sort_classifiers_by_list_order( this_, &sorted_classifiers );
    
    /* layout list */
    pencil_classifier_layouter_private_layout_vertical( this_,
                                                        &sorted_classifiers,
                                                        &draw_area,
                                                        GEOMETRY_H_ALIGN_CENTER
                                                      );
    
    /* cleanup sorted array indices */
    universal_array_index_sorter_destroy( &sorted_classifiers );
    geometry_rectangle_destroy( &draw_area );

    TRACE_END();
}

void pencil_classifier_layouter_layout_for_sequence( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    /* const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size ); */

    /* get the diagram coordinates */
    geometry_rectangle_t draw_area;
    geometry_rectangle_copy( &draw_area, (*this_).diagram_draw_area );
    geometry_rectangle_shift( &draw_area, obj_dist, 0.0 );
    geometry_rectangle_expand( &draw_area, (-2.0 * obj_dist), (- obj_dist) );
    double diag_x = geometry_rectangle_get_left( &draw_area );
    double diag_y = geometry_rectangle_get_top( &draw_area );
    double diag_w = geometry_rectangle_get_width( &draw_area );
    double diag_h = geometry_rectangle_get_height( &draw_area );

    const double phi = 1.6180339; /* b=0.618, a=1.0, a+b=1.618 => ((a+b)/a)==(a/b) */
    const double golden_ratio_width = diag_w/phi;
    const double golden_ratio_height = diag_h/phi;
    
    /* calculate preferred classifier bounds */
    uint_fast32_t cnt_notes_reqs = 0;  /* comments and requirements */
    uint_fast32_t cnt_diag_refs = 0;  /* interaction diagram references */
    uint_fast32_t cnt_others = 0;  /* interaction diagram references */
    const uint_fast32_t c_count = pencil_layout_data_get_visible_classifier_count( (*this_).layout_data );
    for ( uint_fast32_t plain_idx = 0; plain_idx < c_count; plain_idx ++ )
    {
        layout_visible_classifier_t *visible_classifier1;
        visible_classifier1 = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, plain_idx );
        const data_visible_classifier_t *const visible_classifier_data
            = layout_visible_classifier_get_data_const ( visible_classifier1 );
        
        /* set the preferred bounds, space and label_box of the classifier layout */
        {
            geometry_dimensions_t preferred_dim;
            geometry_dimensions_init( &preferred_dim, (diag_w/c_count), (diag_h/10.0) );
            geometry_dimensions_t features_dim;
            geometry_dimensions_init_empty( &features_dim );

            const bool has_contained_children = false;  /* this diagram type does not embrace children */
            pencil_classifier_composer_set_all_bounds( &((*this_).classifier_composer),
                                                       visible_classifier_data,
                                                       &preferred_dim,
                                                       &features_dim,
                                                       has_contained_children,  
                                                       (*this_).pencil_size,
                                                       font_layout,
                                                       visible_classifier1
                                                     );

            geometry_dimensions_destroy( &features_dim );
            geometry_dimensions_destroy( &preferred_dim );
        }
        
        /* collect classifier type statistics */
        const data_classifier_t *const classifier1 = data_visible_classifier_get_classifier_const( visible_classifier_data );
        const data_classifier_type_t c1_type = data_classifier_get_main_type ( classifier1 );
        if (( c1_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
            || ( c1_type == DATA_CLASSIFIER_TYPE_COMMENT ))
        {
            cnt_notes_reqs ++;
        }
        else if ( c1_type == DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE )
        {
            cnt_diag_refs ++;
        }
        else
        {
            cnt_others ++;
        }
    }
    assert( cnt_notes_reqs + cnt_diag_refs + cnt_others == c_count );
    
    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );
    pencil_classifier_layouter_private_sort_classifiers_by_list_order( this_, &sorted_classifiers );

    /* store the classifier bounds into input_data_layouter_t */
    const uint_fast32_t count_classifiers = universal_array_index_sorter_get_count ( &sorted_classifiers );
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( &sorted_classifiers, sort_idx );

        layout_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        /* get the symbol_box and inner space rectangles to modify */
        geometry_rectangle_t *classifier_symbol_box
           = layout_visible_classifier_get_symbol_box_ptr( visible_classifier2 );

        /* define the bounding box */
        geometry_rectangle_init( classifier_symbol_box,
                                 diag_x+((sort_idx*diag_w)/count_classifiers),
                                 diag_y,
                                 (diag_w/count_classifiers),
                                 diag_h/8
                               );

        /* update inner space and label_box */
        const bool has_contained_children = false;
        pencil_classifier_composer_set_space_and_label( &((*this_).classifier_composer),
                                                        layout_visible_classifier_get_data_const( visible_classifier2 ),
                                                        has_contained_children,
                                                        (*this_).pencil_size,
                                                        font_layout,
                                                        visible_classifier2
                                                      );
    }

    /* cleanup sorted array indices */
    universal_array_index_sorter_destroy( &sorted_classifiers );
    geometry_rectangle_destroy( &draw_area );

    TRACE_END();
}

void pencil_classifier_layouter_layout_for_timing( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );
    /* const double gap = pencil_size_get_standard_object_border( (*this_).pencil_size ); */

    /* get the diagram coordinates */
    geometry_rectangle_t draw_area;
    geometry_rectangle_copy( &draw_area, (*this_).diagram_draw_area );
    geometry_rectangle_shift( &draw_area, obj_dist, 0.0 );
    geometry_rectangle_expand( &draw_area, (-2.0 * obj_dist), (- obj_dist) );
    double diag_w = geometry_rectangle_get_width( &draw_area );
    double diag_h = geometry_rectangle_get_height( &draw_area );

    const double phi = 1.6180339; /* b=0.618, a=1.0, a+b=1.618 => ((a+b)/a)==(a/b) */
    const double golden_ratio_width = diag_w/phi;
    const double golden_ratio_height = diag_h/phi;
    
    /* calculate preferred classifier bounds */
    uint_fast32_t cnt_notes_reqs = 0;  /* comments and requirements */
    uint_fast32_t cnt_diag_refs = 0;  /* interaction diagram references */
    uint_fast32_t cnt_others = 0;  /* interaction diagram references */
    const uint_fast32_t c_count = pencil_layout_data_get_visible_classifier_count( (*this_).layout_data );
    for ( uint_fast32_t plain_idx = 0; plain_idx < c_count; plain_idx ++ )
    {
        layout_visible_classifier_t *visible_classifier1;
        visible_classifier1 = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, plain_idx );
        const data_visible_classifier_t *const visible_classifier_data
            = layout_visible_classifier_get_data_const ( visible_classifier1 );
        
        /* set the preferred bounds, space and label_box of the classifier layout */
        {
            geometry_dimensions_t preferred_dim;
            geometry_dimensions_init( &preferred_dim, (diag_w-golden_ratio_width)/2.0, (golden_ratio_height/c_count) );
            geometry_dimensions_t features_dim;
            geometry_dimensions_init_empty( &features_dim );

            const bool has_contained_children = false;  /* this diagram type does not embrace children */
            pencil_classifier_composer_set_all_bounds( &((*this_).classifier_composer),
                                                       visible_classifier_data,
                                                       &preferred_dim,
                                                       &features_dim,
                                                       has_contained_children,  
                                                       (*this_).pencil_size,
                                                       font_layout,
                                                       visible_classifier1
                                                     );

            geometry_dimensions_destroy( &features_dim );
            geometry_dimensions_destroy( &preferred_dim );
        }
        
        /* collect classifier type statistics */
        const data_classifier_t *const classifier1 = data_visible_classifier_get_classifier_const( visible_classifier_data );
        const data_classifier_type_t c1_type = data_classifier_get_main_type ( classifier1 );
        if (( c1_type == DATA_CLASSIFIER_TYPE_REQUIREMENT )
            || ( c1_type == DATA_CLASSIFIER_TYPE_COMMENT ))
        {
            cnt_notes_reqs ++;
        }
        else if ( c1_type == DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE )
        {
            cnt_diag_refs ++;
        }
        else
        {
            cnt_others ++;
        }
    }
    assert( cnt_notes_reqs + cnt_diag_refs + cnt_others == c_count );
    
    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );
    pencil_classifier_layouter_private_sort_classifiers_by_list_order( this_, &sorted_classifiers );
    
    /* layout list */
    pencil_classifier_layouter_private_layout_vertical( this_,
                                                        &sorted_classifiers,
                                                        &draw_area,
                                                        GEOMETRY_H_ALIGN_LEFT
                                                      );

    /* cleanup sorted array indices */
    universal_array_index_sorter_destroy( &sorted_classifiers );
    geometry_rectangle_destroy( &draw_area );

    TRACE_END();
}

void pencil_classifier_layouter_private_layout_horizontal( const pencil_classifier_layouter_t *this_,
                                                           const universal_array_index_sorter_t *classifier_list,
                                                           const geometry_rectangle_t *dest_rect,
                                                           geometry_v_align_t v_alignment )
{
    TRACE_BEGIN();


    TRACE_END();
}

void pencil_classifier_layouter_private_layout_vertical( const pencil_classifier_layouter_t *this_,
                                                         const universal_array_index_sorter_t *classifier_list,
                                                         const geometry_rectangle_t *dest_rect,
                                                         geometry_h_align_t h_alignment )
{
    TRACE_BEGIN();

    /* get the destination rectangle coordinates */
    double diag_y = geometry_rectangle_get_top( dest_rect );
    double diag_h = geometry_rectangle_get_height( dest_rect );

    /* calculate sum of wished envelope heights */
    double total_wish_height = 0.0;
    const uint_fast32_t count_classifiers = universal_array_index_sorter_get_count ( classifier_list );
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( classifier_list, sort_idx );
        const layout_visible_classifier_t *const visible_classifier1
            = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, index );
        
        /* update sum of wished envelope heights */
        const geometry_rectangle_t envelope
            = layout_visible_classifier_calc_envelope_box( visible_classifier1 );
        total_wish_height += geometry_rectangle_get_height( &envelope );
    }
    
    /* store the classifier symbol_box into input_data_layouter_t */
    double current_y = diag_y;
    for ( uint_fast32_t sort_idx = 0; sort_idx < count_classifiers; sort_idx ++ )
    {
        uint_fast32_t index = universal_array_index_sorter_get_array_index( classifier_list, sort_idx );
        layout_visible_classifier_t *const visible_classifier2
            = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        const geometry_rectangle_t envelope
            = layout_visible_classifier_calc_envelope_box( visible_classifier2 );
        const double envelope_top = geometry_rectangle_get_top( &envelope );
        const double envelope_height = geometry_rectangle_get_height( &envelope );
        const double envelope_left = geometry_rectangle_get_left( &envelope );
        const double envelope_width = geometry_rectangle_get_width( &envelope );
        
        /* const double wish2avail_ratio = (diag_h == 0.0) ? 1.0 : (total_wish_height/diag_h); */
        const double avail2wish_ratio = (total_wish_height == 0.0) ? 1.0 : (diag_h/total_wish_height);
        const double available_height = envelope_height * avail2wish_ratio;
        const double y_align_ratio = (count_classifiers==1) ? 0.5 : sort_idx/(count_classifiers-1.0);
        const double y_align_envelope = envelope_height * y_align_ratio;
        const double y_align_available = available_height * y_align_ratio;
        
        const double delta_y = (current_y+y_align_available) - (envelope_top+y_align_envelope);
        
        const double dest_left = geometry_h_align_get_left( &h_alignment, 
                                                            envelope_width,
                                                            geometry_rectangle_get_left( dest_rect ),
                                                            geometry_rectangle_get_width( dest_rect )
                                                          );
        const double delta_x = dest_left - envelope_left;
        
        layout_visible_classifier_shift( visible_classifier2, delta_x, delta_y );
        
        current_y += available_height;
    }

    TRACE_END();
}


/*
Copyright 2017-2021 Andreas Warnke

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
