/* File: pencil_classifier_2d_layouter.c; Copyright and License: see below */

#include "pencil_classifier_2d_layouter.h"
#include "layout/layout_quality.h"
#include "geometry/geometry_non_linear_scale.h"
#include "u8/u8_trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_classifier_2d_layouter_init( pencil_classifier_2d_layouter_t *this_,
                                         layout_visible_set_t *layout_data,
                                         const data_profile_part_t *profile,
                                         const pencil_size_t *pencil_size,
                                         geometry_dimensions_t *default_classifier_size,
                                         const geometry_grid_t *grid,
                                         pencil_feature_layouter_t *feature_layouter )
{
    U8_TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != profile );
    assert( NULL != pencil_size );
    assert( NULL != default_classifier_size );
    assert( NULL != grid );
    assert( NULL != feature_layouter );

    (*this_).layout_data = layout_data;
    (*this_).profile = profile;
    (*this_).pencil_size = pencil_size;
    (*this_).default_classifier_size = default_classifier_size;
    (*this_).grid = grid;
    (*this_).feature_layouter = feature_layouter;
    pencil_classifier_composer_init( &((*this_).classifier_composer) );

    /* get draw area */
    {
        layout_diagram_t *diagram_layout;
        diagram_layout = layout_visible_set_get_diagram_ptr( (*this_).layout_data );
        (*this_).diagram_draw_area = layout_diagram_get_draw_area_const( diagram_layout );
    }

    U8_TRACE_END();
}

void pencil_classifier_2d_layouter_destroy( pencil_classifier_2d_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    pencil_classifier_composer_destroy( &((*this_).classifier_composer) );

    U8_TRACE_END();
}

/* ================================ INITIAL LAYOUT ================================ */

void pencil_classifier_2d_layouter_estimate_bounds( pencil_classifier_2d_layouter_t *this_, PangoLayout *font_layout )
{
    U8_TRACE_BEGIN();

    /* store the classifier bounds into input_data_layouter_t */
    const uint32_t count_clasfy = layout_visible_set_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        layout_visible_classifier_t *const classifier_layout
             = layout_visible_set_get_visible_classifier_ptr ( (*this_).layout_data, index );

        /* set the bounds, space and label_box of the classifier layout */
        {
            const bool shows_contained_children = false;  /* if classifier has children, this will be updated later */
                                                          /* when calling pencil_classifier_composer_set_space_and_label */

            /* init by default size */
            {
                geometry_rectangle_t envelope;
                geometry_rectangle_init( &envelope,
                                         0.0,
                                         0.0,
                                         geometry_dimensions_get_width( (*this_).default_classifier_size ),
                                         geometry_dimensions_get_height( (*this_).default_classifier_size ) );

                pencil_classifier_composer_set_envelope_box( &((*this_).classifier_composer),
                                                             &envelope,
                                                             shows_contained_children,
                                                             (*this_).profile,
                                                             (*this_).pencil_size,
                                                             font_layout,
                                                             classifier_layout
                                                           );

                geometry_rectangle_destroy( &envelope );
            }

            /* check if inner space is big enough for contained features */
            {
                geometry_dimensions_t features_dim;
                geometry_dimensions_init_empty( &features_dim );
                pencil_feature_layouter_calculate_features_bounds( (*this_).feature_layouter,
                                                                   layout_visible_classifier_get_diagramelement_id( classifier_layout ),
                                                                   font_layout,
                                                                   &features_dim
                                                                 );

                const geometry_rectangle_t *const space_rect
                    = layout_visible_classifier_get_space_const( classifier_layout );
                const geometry_dimensions_t space_dim = geometry_rectangle_get_dimensions( space_rect );

                if ( ! geometry_dimensions_can_contain( &space_dim, &features_dim ) )
                {
                    geometry_rectangle_t new_space;
                    geometry_rectangle_copy( &new_space, space_rect );
                    const double delta_width
                        = geometry_dimensions_get_width( &features_dim ) - geometry_rectangle_get_width( space_rect );
                    const double delta_height
                        = geometry_dimensions_get_height( &features_dim ) - geometry_rectangle_get_height( space_rect );
                    geometry_rectangle_expand_4dir( &new_space,
                                                  (delta_width<0.0) ? 0.0 : 0.5*delta_width,
                                                  (delta_height<0.0) ? 0.0 : 0.5*delta_height );

                    pencil_classifier_composer_expand_space( &((*this_).classifier_composer),
                                                             &new_space,
                                                             shows_contained_children,
                                                             (*this_).profile,
                                                             (*this_).pencil_size,
                                                             font_layout,
                                                             classifier_layout
                                                           );

                    geometry_rectangle_destroy( &new_space );
                }

                geometry_dimensions_destroy( &features_dim );
            }
        }

        /* move the classifier rectangles to the target location */
        {
            const data_visible_classifier_t *const visible_classifier2
                = layout_visible_classifier_get_data_const ( classifier_layout );
            const data_classifier_t *const classifier2
                = data_visible_classifier_get_classifier_const( visible_classifier2 );
            const geometry_rectangle_t *const classifier_symbol_box
                = layout_visible_classifier_get_symbol_box_const( classifier_layout );

            const double act_center_x = geometry_rectangle_get_center_x( classifier_symbol_box );
            const double act_center_y = geometry_rectangle_get_center_y( classifier_symbol_box );
            const int32_t order_x = data_classifier_get_x_order( classifier2 );
            const int32_t order_y = data_classifier_get_y_order( classifier2 );
            const geometry_non_linear_scale_t *const x_scale = geometry_grid_get_x_scale_const( (*this_).grid );
            const geometry_non_linear_scale_t *const y_scale = geometry_grid_get_y_scale_const( (*this_).grid );
            const double center_x = geometry_non_linear_scale_get_location( x_scale, order_x );
            const double center_y = geometry_non_linear_scale_get_location( y_scale, order_y );
            layout_visible_classifier_shift( classifier_layout, center_x-act_center_x, center_y-act_center_y );
        }
    }

    U8_TRACE_END();
}

/* ================================ MOVE TO AVOID OVERLAPS ================================ */

void pencil_classifier_2d_layouter_move_to_avoid_overlaps ( pencil_classifier_2d_layouter_t *this_ )
{
    U8_TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the classifiers by their movement-needs */
    pencil_classifier_2d_layouter_private_propose_move_processing_order ( this_, &sorted );

    /* move the classifiers */
    const uint32_t count_sorted = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 6;
        double solution_move_dx[6];
        double solution_move_dy[6];

        /* propose options of moving left/right/up/down */
        pencil_classifier_2d_layouter_private_propose_4dir_move_solutions( this_,
                                                                           &sorted,
                                                                           sort_index,
                                                                           SOLUTIONS_MAX-1,
                                                                           solution_move_dx,
                                                                           solution_move_dy,
                                                                           &solutions_count
                                                                         );
        /* propose options of moving close at origin-area */
        pencil_classifier_2d_layouter_private_propose_anchored_solution( this_,
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
            pencil_classifier_2d_layouter_private_select_move_solution( this_,
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
        the_classifier = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, index );
        layout_visible_classifier_shift( the_classifier, solution_move_dx[index_of_best], solution_move_dy[index_of_best] );
    }

    universal_array_index_sorter_destroy( &sorted );

    U8_TRACE_END();
}

void pencil_classifier_2d_layouter_private_propose_move_processing_order ( pencil_classifier_2d_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) DATA_VISIBLE_SET_MAX_CLASSIFIERS );

    /* sort the classifiers by their movement-needs */
    uint32_t count_clasfy;
    count_clasfy = layout_visible_set_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        const layout_visible_classifier_t *const the_classifier
            = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, index );
        const geometry_rectangle_t *const classifier_envelope_box
            = layout_visible_classifier_get_envelope_box_const( the_classifier );

        int64_t simpleness = 0;  /* the lower the number, the ealier the classifier will be processed. Unit is area(=square-length). */

        /* reduce simpleness by area outside the diagram border: the more outside diagram area, the earlier it should be moved */
        {
            geometry_rectangle_t border_intersect;
            int intersect_error2;
            intersect_error2 = geometry_rectangle_init_by_intersect( &border_intersect, classifier_envelope_box, (*this_).diagram_draw_area );
            if ( 0 != intersect_error2 )
            {
                U8_LOG_WARNING( "a rectangle to be drawn is completely outside the diagram area" );
            }

            simpleness += 16.0 * geometry_rectangle_get_area( &border_intersect );
            simpleness -= 16.0 * geometry_rectangle_get_area( classifier_envelope_box );

            geometry_rectangle_destroy( &border_intersect );
        }

        /* reduce simpleness by intersects with other rectangles: the more intersects, the earlier it should be moved */
        for ( uint32_t probe_index = 0; probe_index < count_clasfy; probe_index ++ )
        {
            layout_visible_classifier_t *probe_classifier;
            probe_classifier = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, probe_index );
            const geometry_rectangle_t *const probe_envelope_box
                = layout_visible_classifier_get_envelope_box_const( probe_classifier );

            geometry_rectangle_t intersect;
            int intersect_error;
            intersect_error = geometry_rectangle_init_by_intersect( &intersect, classifier_envelope_box, probe_envelope_box );

            if ( 0 == intersect_error )
            {
                simpleness -= geometry_rectangle_get_area( &intersect );
            }

            geometry_rectangle_destroy( &intersect );
        }

        /* reduce simpleness by own size: the bigger the object, the earlier it should be moved */
        {
            const double default_classifier_area = geometry_dimensions_get_area( (*this_).default_classifier_size );
            const double classifier_area = geometry_rectangle_get_area( classifier_envelope_box );
            if (( default_classifier_area > 0.000000001 )&&( classifier_area > 0.000000001 ))
            {
                simpleness -= default_classifier_area * ( classifier_area / ( classifier_area + default_classifier_area ));
            }
        }

        /* increase simpleness if contained children: if embracing children later, layouting problems might get solved */
        {
            const double default_classifier_area = geometry_dimensions_get_area( (*this_).default_classifier_size );
            const uint32_t descendant_count
                = layout_visible_set_count_descendants( (*this_).layout_data, the_classifier );
            if ( descendant_count != 0 )
            {
                simpleness += default_classifier_area;
            }
        }

        const u8_error_t insert_error = universal_array_index_sorter_insert( out_sorted, index, simpleness );
        if ( U8_ERROR_NONE != insert_error )
        {
            U8_LOG_WARNING( "not all rectangles are moved" );
        }
    }

    U8_TRACE_END();
}

/*!
 *  \brief constants for directions of moving objects
 */
enum pencil_classifier_2d_layouter_private_move_enum {
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_NOT = 0,  /*!< only move to visible arey - nothing more */
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_UP_MIN = 1,  /*!< moves up the minimum distance (up means smaller y-values) */
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_DOWN_MIN = 2,
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_LEFT_MIN = 3,
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN = 4,
    PENCIL_CLASSIFIER_LAYOUTER_PRIVATE_MOVE_MAX = 5,  /*!< constant defining the total number of available options */
};

void pencil_classifier_2d_layouter_private_propose_4dir_move_solutions( pencil_classifier_2d_layouter_t *this_,
                                                                        const universal_array_index_sorter_t *sorted,
                                                                        uint32_t sort_index,
                                                                        uint32_t solutions_max,
                                                                        double out_solution_move_dx[],
                                                                        double out_solution_move_dy[],
                                                                        uint32_t *out_solutions_count )
{
    U8_TRACE_BEGIN();
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
        = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, index );
    const geometry_rectangle_t *const classifier_envelope_box
        = layout_visible_classifier_get_envelope_box_const( the_classifier );
    double top = geometry_rectangle_get_top ( classifier_envelope_box );
    double bottom = geometry_rectangle_get_bottom ( classifier_envelope_box );
    double left = geometry_rectangle_get_left ( classifier_envelope_box );
    double right = geometry_rectangle_get_right ( classifier_envelope_box );

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
        const uint32_t probe_index
             = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );
        const layout_visible_classifier_t *const the_probe
            = layout_visible_set_get_visible_classifier_const( (*this_).layout_data, probe_index );
        const geometry_rectangle_t *const probe_envelope_box
            = layout_visible_classifier_get_envelope_box_const( the_probe );
        const double probe_top = geometry_rectangle_get_top ( probe_envelope_box );
        const double probe_bottom = geometry_rectangle_get_bottom ( probe_envelope_box );
        const double probe_left = geometry_rectangle_get_left ( probe_envelope_box );
        const double probe_right = geometry_rectangle_get_right ( probe_envelope_box );

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
        else if ( layout_visible_set_is_ancestor( (*this_).layout_data, the_probe, the_classifier ) )
        {
            /* overlapping the parent is ok, finished */
        }
        else if ( layout_visible_set_is_ancestor( (*this_).layout_data, the_classifier, the_probe ) )
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
                U8_TRACE_INFO_STR( "- overlaps:", data_classifier_get_name_const( classifier_p ) );
            }
        }
    }

    /* trace */
    const data_visible_classifier_t *visible_classifier;
    visible_classifier = layout_visible_classifier_get_data_const ( the_classifier );
    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    U8_TRACE_INFO_STR( "classifier:", data_classifier_get_name_const( classifier ) );

    U8_TRACE_END();
}

void pencil_classifier_2d_layouter_private_propose_anchored_solution( pencil_classifier_2d_layouter_t *this_,
                                                                      const universal_array_index_sorter_t *sorted,
                                                                      uint32_t sort_index,
                                                                      double * out_solution_move_dx,
                                                                      double * out_solution_move_dy )
{
    U8_TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != out_solution_move_dx );
    assert ( NULL != out_solution_move_dy );

    /* get classifier to move */
    const uint32_t index
        = universal_array_index_sorter_get_array_index( sorted, sort_index );
    const layout_visible_classifier_t *const the_classifier
        = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, index );

    /* determine the space needed for the solution */
    const geometry_rectangle_t *const classifier_envelope_box
        = layout_visible_classifier_get_envelope_box_const( the_classifier );
    const double width = geometry_rectangle_get_width( classifier_envelope_box );
    const double height = geometry_rectangle_get_height( classifier_envelope_box );
    const double gap = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* wish a solution area */
    geometry_rectangle_t classifier_solution_area;
    geometry_rectangle_init ( &classifier_solution_area,
                              geometry_rectangle_get_left( classifier_envelope_box ) - 0.5*width - gap,
                              geometry_rectangle_get_top( classifier_envelope_box ) - 0.5*height - gap,
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
        const uint32_t probe_index
            = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );
        const layout_visible_classifier_t *const the_probe
            = layout_visible_set_get_visible_classifier_const( (*this_).layout_data, probe_index );

        geometry_rectangle_t probe_total_bounds;
        geometry_rectangle_init_by_bounds( &probe_total_bounds,
                                           layout_visible_classifier_get_label_box_const( the_probe ),
                                           layout_visible_classifier_get_envelope_box_const( the_probe )
                                         );

        geometry_rectangle_init_by_difference( &classifier_solution_area,
                                               &classifier_solution_area,
                                               &probe_total_bounds
                                             );
    }

    /* reduce the biggest free/unoccupied box by gap */
    geometry_rectangle_shift ( &classifier_solution_area, gap, gap );
    geometry_rectangle_enlarge ( &classifier_solution_area, -2.0*gap, -2.0*gap );

    /* move - but not to eager - only the minumum distance */
    const bool is_x_contained
        = ( geometry_rectangle_get_left( &classifier_solution_area ) < geometry_rectangle_get_left( classifier_envelope_box ) )
        && ( geometry_rectangle_get_right( classifier_envelope_box ) < geometry_rectangle_get_right( &classifier_solution_area ) );
    const bool is_y_contained
        = ( geometry_rectangle_get_top( &classifier_solution_area ) < geometry_rectangle_get_top( classifier_envelope_box ) )
        && ( geometry_rectangle_get_bottom( classifier_envelope_box ) < geometry_rectangle_get_bottom( &classifier_solution_area ) );
    if ( is_x_contained )
    {
        *out_solution_move_dx = 0.0;
    }
    else
    {
        const double sol_center_x = geometry_rectangle_get_center_x( &classifier_solution_area );
        const double cur_center_x = geometry_rectangle_get_center_x( classifier_envelope_box );
        *out_solution_move_dx = ( sol_center_x < cur_center_x )
            ? geometry_rectangle_get_right( &classifier_solution_area )
            - geometry_rectangle_get_right( classifier_envelope_box )
            : geometry_rectangle_get_left( &classifier_solution_area )
            - geometry_rectangle_get_left( classifier_envelope_box );
        geometry_rectangle_trace( &classifier_solution_area );
        geometry_rectangle_trace( classifier_envelope_box );
    }
    if ( is_y_contained )
    {
        *out_solution_move_dy = 0.0;
    }
    else
    {
        const double sol_center_y = geometry_rectangle_get_center_y( &classifier_solution_area );
        const double cur_center_y = geometry_rectangle_get_center_y( classifier_envelope_box );
        *out_solution_move_dy = ( sol_center_y < cur_center_y )
            ? geometry_rectangle_get_bottom( &classifier_solution_area )
            - geometry_rectangle_get_bottom( classifier_envelope_box )
            : geometry_rectangle_get_top( &classifier_solution_area )
            - geometry_rectangle_get_top( classifier_envelope_box );
    }

    /* trace */
    const data_visible_classifier_t *visible_classifier;
    visible_classifier = layout_visible_classifier_get_data_const ( the_classifier );
    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    U8_TRACE_INFO_STR( "classifier:", data_classifier_get_name_const( classifier ) );

    U8_TRACE_END();
}

void pencil_classifier_2d_layouter_private_select_move_solution( pencil_classifier_2d_layouter_t *this_,
                                                                 const universal_array_index_sorter_t *sorted,
                                                                 uint32_t sort_index,
                                                                 uint32_t solutions_count,
                                                                 const double solution_move_dx[],
                                                                 const double solution_move_dy[],
                                                                 uint32_t *out_index_of_best )
{
    U8_TRACE_BEGIN();
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
        = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, index );
    const geometry_rectangle_t *const classifier_envelope_box
        = layout_visible_classifier_get_envelope_box_const( the_classifier );

    /* check all solutions */
    for ( uint32_t solution_index = 0; solution_index < solutions_count; solution_index ++ )
    {
        /* calculate the solution rectangle */
        geometry_rectangle_t solution_bounds;
        geometry_rectangle_copy( &solution_bounds, classifier_envelope_box );
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
                const uint32_t probe_index
                    = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );

                const layout_visible_classifier_t *const the_probe
                    = layout_visible_set_get_visible_classifier_const( (*this_).layout_data, probe_index );
                const geometry_rectangle_t *const probe_envelope_box
                    = layout_visible_classifier_get_envelope_box_const( the_probe );

                geometry_rectangle_t probe_intersect;
                const int intersect_err
                    = geometry_rectangle_init_by_intersect( &probe_intersect, &solution_bounds, probe_envelope_box );
                if ( 0 == intersect_err )
                {
                    /* there is an intersect */
                    if ( layout_visible_set_is_ancestor( (*this_).layout_data, the_classifier, the_probe ) )
                    {
                        /* no debt: parent my overlap children */
                    }
                    else if ( layout_visible_set_is_ancestor( (*this_).layout_data, the_probe, the_classifier ) )
                    {
                        /* no debt: child may overlap parent */
                    }
                    else
                    {
                        /* already processed classifiers have higher severity because these do not move anymore */
                        const double severity = ( probe_sort_index < sort_index ) ? 4.0 : 1.0;
                        const double probe_intersect_area = geometry_rectangle_get_area ( &probe_intersect );
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

    U8_TRACE_END();
}

/* ================================ EMBRACE CHILDREN STEP BY STEP ================================ */

void pencil_classifier_2d_layouter_embrace_children( pencil_classifier_2d_layouter_t *this_, PangoLayout *font_layout )
{
    U8_TRACE_BEGIN();
    assert( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) LAYOUT_VISIBLE_SET_MAX_RELATIONSHIPS );

    universal_array_index_sorter_t sorted_relationships;
    universal_array_index_sorter_init( &sorted_relationships );

    /* sort the relationships by their number of descendants */
    pencil_classifier_2d_layouter_private_propose_embracing_order ( this_, &sorted_relationships );

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
            = layout_visible_set_get_relationship_ptr( (*this_).layout_data, rel_idx );
        assert ( the_relationship != NULL );
        const data_relationship_t *const rel_data = layout_relationship_get_data_const ( the_relationship );
        assert ( rel_data != NULL );
        const data_id_t rel_from_id = data_relationship_get_from_classifier_data_id ( rel_data );

        const int failure
            = pencil_classifier_2d_layouter_private_try_embrace_child( this_,
                                                                       the_relationship,
                                                                       ! data_small_set_contains( &has_embraced_children, rel_from_id ),
                                                                       font_layout
                                                                     );
        if ( ! failure )
        {
            /* only in case of success, children are counted as embraced */
            data_small_set_add_obj( &has_embraced_children, rel_from_id );
        }
    }

    data_small_set_destroy( &has_embraced_children );

    universal_array_index_sorter_destroy( &sorted_relationships );

    U8_TRACE_END();
}

void pencil_classifier_2d_layouter_private_propose_embracing_order ( pencil_classifier_2d_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_sorted );

    const uint32_t rel_count = layout_visible_set_get_relationship_count( (*this_).layout_data );
    for ( uint32_t rel_idx = 0; rel_idx < rel_count; rel_idx ++ )
    {
        const layout_relationship_t *const the_relationship
            = layout_visible_set_get_relationship_ptr( (*this_).layout_data, rel_idx );

        /* count the descendants */
        const layout_visible_classifier_t *const from_classifier
            = layout_relationship_get_from_classifier_ptr( the_relationship );
        const uint32_t from_descendant_count
            = layout_visible_set_count_descendants( (*this_).layout_data, from_classifier );

        /* sort it into the array by the number of decendants: */
        /* the less descendants the earlier it shall be processed. */
        const u8_error_t err = universal_array_index_sorter_insert( out_sorted, rel_idx, (double)from_descendant_count );
        if ( U8_ERROR_NONE != err )
        {
            U8_LOG_ERROR ( "universal_array_index_sorter_t list is full." );
        }
    }

    U8_TRACE_END();
}

int pencil_classifier_2d_layouter_private_try_embrace_child( pencil_classifier_2d_layouter_t *this_,
                                                             layout_relationship_t *the_relationship,
                                                             bool move,
                                                             PangoLayout *font_layout )
{
    U8_TRACE_BEGIN();
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
            layout_visible_classifier_t probe_parent_layout;
            layout_visible_classifier_copy( &probe_parent_layout, from_classifier );
            const geometry_rectangle_t * parent_space
                = layout_visible_classifier_get_space_const( &probe_parent_layout );
            const geometry_rectangle_t child_envelope = layout_visible_classifier_get_envelope_box( to_classifier );
            geometry_rectangle_t probe_space;
            if ( move )
            {
                geometry_rectangle_copy( &probe_space, &child_envelope );
            }
            else
            {
                geometry_rectangle_init_by_bounds( &probe_space, parent_space, &child_envelope );
            }

            pencil_classifier_composer_expand_space( &((*this_).classifier_composer),
                                                     &probe_space,
                                                     true,  /* = shows_contained_children */
                                                     (*this_).profile,
                                                     (*this_).pencil_size,
                                                     font_layout,
                                                     &probe_parent_layout
                                                   );

            const geometry_rectangle_t probe_parent_envelope
                = layout_visible_classifier_get_envelope_box( &probe_parent_layout );

            /* check what else would be embraced */
            bool illegal_overlap = false;
            const uint32_t count_clasfy
                = layout_visible_set_get_visible_classifier_count ( (*this_).layout_data );
            for ( uint32_t c_index = 0; c_index < count_clasfy; c_index ++ )
            {
                layout_visible_classifier_t *probe_classifier;
                probe_classifier = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, c_index );

                if (( probe_classifier != from_classifier )&&( probe_classifier != to_classifier ))
                {
                    if ( layout_visible_set_is_ancestor( (*this_).layout_data, from_classifier, probe_classifier ) )
                    {
                        /* it is ok to embrace also other children, no illegal_overlap */
                    }
                    else if ( layout_visible_set_is_ancestor( (*this_).layout_data, probe_classifier, from_classifier ) )
                    {
                        /* it is ok if parent is already contained in grand-parent classifier, no illegal_overlap */
                    }
                    else
                    {
                        const geometry_rectangle_t *const current_envelope_box
                            = layout_visible_classifier_get_envelope_box_const ( probe_classifier );
                        illegal_overlap |= geometry_rectangle_is_intersecting( current_envelope_box, &probe_parent_envelope );
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
                layout_visible_classifier_replacemove( from_classifier, &probe_parent_layout );
                result_err = 0;
            }
            else
            {
                layout_visible_classifier_destroy( &probe_parent_layout );
            }

            /* cleanup */
            geometry_rectangle_destroy( &probe_space );
        }
        else
        {
            U8_TRACE_INFO( "Classifier contains itself" );
        }
    }
    /* else this is not a parent child relationship */

    U8_TRACE_END_ERR( result_err );
    return result_err;
}

/* ================================ EMBRACE CHILDREN COMMON ================================ */

void pencil_classifier_2d_layouter_hide_relations_of_embraced_children( pencil_classifier_2d_layouter_t *this_ )
{
    U8_TRACE_BEGIN();

    /* search containment relations */
    const uint32_t rel_count = layout_visible_set_get_relationship_count( (*this_).layout_data );
    for ( uint32_t rel_idx = 0; rel_idx < rel_count; rel_idx ++ )
    {
        const layout_relationship_t *const the_relationship
            = layout_visible_set_get_relationship_const( (*this_).layout_data, rel_idx );
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

                /* hide if parent embraced child(symbol) completely */
                if ( geometry_rectangle_is_containing( parent_space, child_symbol_box ) )
                {
                    layout_visible_set_set_relationship_visibility( (*this_).layout_data, rel_idx, PENCIL_VISIBILITY_IMPLICIT );
                    U8_TRACE_INFO( "Containment relation is PENCIL_VISIBILITY_IMPLICIT" );
                }
            }
        }
    }

    U8_TRACE_END();
}

/* ================================ EMBRACE AND MOVE CHILDREN TOGETHER ================================ */

void pencil_classifier_2d_layouter_move_and_embrace_children( pencil_classifier_2d_layouter_t *this_, PangoLayout *font_layout )
{
    U8_TRACE_BEGIN();
    assert( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) LAYOUT_VISIBLE_SET_MAX_CLASSIFIERS );

    const double TAKE_RATIO = (1.0/3.0);
    const double LEAVE_RATIO = (1.0-TAKE_RATIO);
    /* const double gap = pencil_size_get_preferred_object_distance( (*this_).pencil_size ); */

    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );

    /* sort the classifiers by their need to move and to embrace */
    pencil_classifier_2d_layouter_private_propose_move_embrace_order ( this_, &sorted_classifiers );

    /* small-move and embrace the child classifiers */
    const uint32_t count_sorted = universal_array_index_sorter_get_count( &sorted_classifiers );
    for ( uint32_t classifier_sort_idx = 0; classifier_sort_idx < count_sorted; classifier_sort_idx ++ )
    {
        const uint32_t classifier_idx = universal_array_index_sorter_get_array_index( &sorted_classifiers, classifier_sort_idx );
        layout_visible_classifier_t *const the_classifier
            = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, classifier_idx );

        /* only if the classifier has children */
        const uint32_t child_count = layout_visible_set_count_descendants ( (*this_).layout_data, the_classifier );
        if ( child_count > 0 )
        {
            /* get envelope rectangle of all children */
            const geometry_rectangle_t children_envelope
                = pencil_classifier_2d_layouter_private_calc_descendant_envelope( this_, the_classifier );

            /* determine outer space around children envelope rectangle */
            const geometry_rectangle_t outer_space
                = pencil_classifier_2d_layouter_private_calc_outer_space( this_, &children_envelope, the_classifier );

            /* place the children into the (probe-)parent */
            layout_visible_classifier_t probe_parent_layout;
            layout_visible_classifier_copy( &probe_parent_layout, the_classifier );
            pencil_classifier_composer_expand_space( &((*this_).classifier_composer),
                                                     &children_envelope,
                                                     true,  /* = shows_contained_children */
                                                     (*this_).profile,
                                                     (*this_).pencil_size,
                                                     font_layout,
                                                     &probe_parent_layout
                                                   );
            const geometry_rectangle_t probe_parent_envelope
                = layout_visible_classifier_get_envelope_box( &probe_parent_layout );

            /* check if parent fits into into outer_space */
            const double outer_border_x
                = (geometry_rectangle_get_width( &outer_space ) - geometry_rectangle_get_width(&probe_parent_envelope))/2.0;
            const double outer_border_y
                = (geometry_rectangle_get_height( &outer_space ) - geometry_rectangle_get_height(&probe_parent_envelope))/2.0;
            if (( outer_border_x > 0.0 )&&( outer_border_y > 0.0 ))
            {
                /* prepare to move+expand the parent */
                geometry_rectangle_t new_envelope;
                geometry_rectangle_copy( &new_envelope, &outer_space );
                geometry_rectangle_shift( &new_envelope, (LEAVE_RATIO*outer_border_x), (LEAVE_RATIO*outer_border_y) );
                geometry_rectangle_enlarge( &new_envelope, -2.0*(LEAVE_RATIO*outer_border_x), -2.0*(LEAVE_RATIO*outer_border_y) );

                /* move+expand the parent */
                pencil_classifier_composer_set_envelope_box( &((*this_).classifier_composer),
                                                             &new_envelope,
                                                             true,  /* = shows_contained_children */
                                                             (*this_).profile,
                                                             (*this_).pencil_size,
                                                             font_layout,
                                                             the_classifier
                                                           );

                /* cleanup move+expand the parent */
                geometry_rectangle_destroy( &new_envelope );

                /* determine the descendants move deltas */
                const geometry_rectangle_t *const parent_new_space = layout_visible_classifier_get_space_const( the_classifier );
                const double descendant_add_dx = geometry_rectangle_get_center_x( parent_new_space ) - geometry_rectangle_get_center_x( &children_envelope );
                const double descendant_add_dy = geometry_rectangle_get_center_y( parent_new_space ) - geometry_rectangle_get_center_y( &children_envelope );

                /* move the descendants */
                pencil_classifier_2d_layouter_private_move_descendants( this_, the_classifier, descendant_add_dx, descendant_add_dy );
            }

            /* cleanup */
            layout_visible_classifier_destroy( &probe_parent_layout );
        }
    }

    universal_array_index_sorter_destroy( &sorted_classifiers );

    U8_TRACE_END();
}

void pencil_classifier_2d_layouter_private_propose_move_embrace_order ( pencil_classifier_2d_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    U8_TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) DATA_VISIBLE_SET_MAX_CLASSIFIERS );

    /* sort the classifiers by their movement-needs */
    const uint32_t count_classifiers = layout_visible_set_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_classifiers; index ++ )
    {
        const layout_visible_classifier_t *const the_classifier = layout_visible_set_get_visible_classifier_ptr( (*this_).layout_data, index );

        int64_t lazy_move = 0;

        /* grand-parents must be moved after parents (becasue parents are not yet well layouted) */
        const uint32_t child_count = layout_visible_set_count_descendants ( (*this_).layout_data, the_classifier );
        lazy_move = child_count;

        const u8_error_t insert_error = universal_array_index_sorter_insert( out_sorted, index, lazy_move );
        if ( U8_ERROR_NONE != insert_error )
        {
            U8_LOG_WARNING( "not all rectangles are grown" );
        }
    }

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
