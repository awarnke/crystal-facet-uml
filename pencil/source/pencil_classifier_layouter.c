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
            geometry_dimensions_t features_bounds;
            geometry_dimensions_init_empty( &features_bounds );
            pencil_feature_layouter_calculate_features_bounds ( (*this_).feature_layouter,
                                                                layout_visible_classifier_get_diagramelement_id( classifier_layout ),
                                                                font_layout,
                                                                &features_bounds
                                                              );

            pencil_classifier_composer_set_all_bounds ( &((*this_).classifier_composer),
                                                        visible_classifier2,
                                                        (*this_).pencil_size,
                                                        font_layout,
                                                        (*this_).default_classifier_size,
                                                        &features_bounds,
                                                        classifier_layout
                                                      );

            geometry_dimensions_destroy( &features_bounds );
        }

        /* move the classifier rectangles to the target location */
        {
            const data_classifier_t *classifier2;
            classifier2 = data_visible_classifier_get_classifier_const( visible_classifier2 );
            const geometry_rectangle_t *const classifier_symbol_box
                = layout_visible_classifier_get_symbol_box_ptr( classifier_layout );

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

void pencil_classifier_layouter_embrace_children( pencil_classifier_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS );

    universal_array_index_sorter_t sorted_relationships;
    universal_array_index_sorter_init( &sorted_relationships );

    /* sort the relationships by their number of descendants */
    pencil_classifier_layouter_private_propose_embracing_order ( this_, &sorted_relationships );

    /* move the classifiers */
    const uint32_t count_sorted = universal_array_index_sorter_get_count( &sorted_relationships );

    /* search containment relations */
    for ( uint32_t rel_sort_idx = 0; rel_sort_idx < count_sorted; rel_sort_idx ++ )
    {
        const uint32_t rel_idx
            = universal_array_index_sorter_get_array_index( &sorted_relationships, rel_sort_idx );

        layout_relationship_t *the_relationship;
        the_relationship = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, rel_idx );
        pencil_classifier_layouter_private_try_embrace_child( this_, the_relationship );
    }

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

void pencil_classifier_layouter_private_try_embrace_child( pencil_classifier_layouter_t *this_, layout_relationship_t *the_relationship )
{
    TRACE_BEGIN();
    assert( NULL != the_relationship );

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
            const geometry_rectangle_t *const parent_symbol_box
                = layout_visible_classifier_get_symbol_box_const ( from_classifier );
            const geometry_rectangle_t *const parent_space
                = layout_visible_classifier_get_space_const ( from_classifier );
            /*
            const geometry_rectangle_t *const child_symbol_box
                = layout_visible_classifier_get_symbol_box_const ( to_classifier );
            */
            geometry_rectangle_t child_total_bounds;
            geometry_rectangle_init_by_bounds( &child_total_bounds,
                                               layout_visible_classifier_get_symbol_box_const ( to_classifier ),
                                               layout_visible_classifier_get_label_box_const ( to_classifier )
                                             );

            /* try embrace child */
            geometry_rectangle_t probe_parent_symbol_box;  /* try out a new parent symbol_box rectangle */
            geometry_rectangle_copy( &probe_parent_symbol_box, parent_symbol_box );
            double extend_to_left = 0.0;
            double extend_to_right = 0.0;
            double extend_to_top = 0.0;
            double extend_to_bottom = 0.0;
            extend_to_left = geometry_rectangle_get_left( parent_space ) - geometry_rectangle_get_left( &child_total_bounds );
            extend_to_top = geometry_rectangle_get_top( parent_space ) - geometry_rectangle_get_top( &child_total_bounds );
            extend_to_right = geometry_rectangle_get_right( &child_total_bounds ) - geometry_rectangle_get_right( parent_space );
            extend_to_bottom = geometry_rectangle_get_bottom( &child_total_bounds ) - geometry_rectangle_get_bottom( parent_space );
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
            geometry_rectangle_expand( &probe_parent_symbol_box, extend_to_left+extend_to_right, extend_to_top+extend_to_bottom );
            geometry_rectangle_shift( &probe_parent_symbol_box, -extend_to_left, -extend_to_top );

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
                        illegal_overlap |= geometry_rectangle_is_intersecting( current_symbol_box, &probe_parent_symbol_box );
                    }
                }
            }
            /* check overlap to diagram boundary */
            if ( ! geometry_rectangle_is_containing ( (*this_).diagram_draw_area, &probe_parent_symbol_box ) )
            {
                illegal_overlap = true;
            }

            /* cancel or commit */
            if ( ! illegal_overlap )
            {
                layout_visible_classifier_expand( from_classifier, extend_to_left+extend_to_right, extend_to_top+extend_to_bottom );
                layout_visible_classifier_shift( from_classifier, -extend_to_left, -extend_to_top );
            }

            /* cleanup */
            geometry_rectangle_destroy( &probe_parent_symbol_box );
        }
        else
        {
            TRACE_INFO( "Classifier contains itself" );
        }
    }
    /* else this is not a parent child relationship */

    TRACE_END();
}

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
        static const uint32_t SOLUTIONS_MAX = 5;
        double solution_move_dx[5];
        double solution_move_dy[5];

        /* propose options */
        pencil_classifier_layouter_private_propose_move_solutions ( this_,
                                                                    &sorted,
                                                                    sort_index,
                                                                    SOLUTIONS_MAX,
                                                                    solution_move_dx,
                                                                    solution_move_dy,
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
            double default_classifier_area = geometry_dimensions_get_area( (*this_).default_classifier_size );
            double classifier_area = geometry_rectangle_get_area( classifier_symbol_box );
            if (( default_classifier_area > 0.000000001 )&&( classifier_area > 0.000000001 ))
            {
                simpleness -= default_classifier_area * ( classifier_area / ( classifier_area + default_classifier_area ));
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

void pencil_classifier_layouter_private_propose_move_solutions ( pencil_classifier_layouter_t *this_,
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

void pencil_classifier_layouter_local_move_and_grow_for_gaps( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();
    assert( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_CLASSIFIERS );

    const double MAX_TAKE_RATIO = 0.3 /*0.33333*/;
    const double gap = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );

    /* sort the classifiers by their need to grow or move */
    pencil_classifier_layouter_private_propose_grow_processing_order ( this_, &sorted_classifiers );

    /* small-move and grow the classifiers */
    const uint32_t count_sorted = universal_array_index_sorter_get_count( &sorted_classifiers );

    /* move and grow */
    for ( uint32_t classifier_sort_idx = 0; classifier_sort_idx < count_sorted; classifier_sort_idx ++ )
    {
        const uint32_t classifier_idx = universal_array_index_sorter_get_array_index( &sorted_classifiers, classifier_sort_idx );
        layout_visible_classifier_t * the_classifier = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, classifier_idx );

        /* only if the classifier has children */
        const uint32_t child_count = pencil_layout_data_count_descendants ( (*this_).layout_data, the_classifier );
        if ( child_count > 0 )
        {
            const geometry_rectangle_t *const classifier_symbol_box = layout_visible_classifier_get_symbol_box_const( the_classifier );
            const geometry_rectangle_t *const classifier_space = layout_visible_classifier_get_space_const( the_classifier );

            /* determine the space there is */
            geometry_rectangle_t max_outer_bounds;
            geometry_rectangle_t min_inner_space;
            pencil_classifier_layouter_private_get_gaps_to_classifiers( this_, classifier_idx, &max_outer_bounds, &min_inner_space );
            geometry_rectangle_trace( (*this_).diagram_draw_area );
            geometry_rectangle_trace( &max_outer_bounds );
            geometry_rectangle_trace( classifier_symbol_box );
            geometry_rectangle_trace( classifier_space );
            geometry_rectangle_trace( &min_inner_space );
            assert( geometry_rectangle_is_containing( (*this_).diagram_draw_area, &max_outer_bounds ) );
            assert( geometry_rectangle_is_containing( &max_outer_bounds, classifier_symbol_box ) );
            //assert( geometry_rectangle_is_containing( classifier_symbol_box, classifier_space ) );  // not true for e.g. decision nodes
            assert( geometry_rectangle_is_containing( classifier_space, &min_inner_space ) );

            /* propose a move and a grow */
            double additional_width = (geometry_rectangle_get_width( &max_outer_bounds ) - (2.0*gap) - geometry_rectangle_get_width( classifier_symbol_box ))
                                    * MAX_TAKE_RATIO;
            if ( additional_width < 0.0 )
            {
                additional_width = 0.0;
            }
            double additional_height = (geometry_rectangle_get_height( &max_outer_bounds ) - (2.0*gap) - geometry_rectangle_get_height( classifier_symbol_box ))
                                    * MAX_TAKE_RATIO;
            if ( additional_height < 0.0 )
            {
                additional_height = 0.0;
            }
            const double delta_x_mv = geometry_rectangle_get_center_x( &max_outer_bounds ) - geometry_rectangle_get_center_x( classifier_symbol_box );
            const double delta_y_mv = geometry_rectangle_get_center_y( &max_outer_bounds ) - geometry_rectangle_get_center_y( classifier_symbol_box );
            const double delta_x = delta_x_mv - (0.5*additional_width);
            const double delta_y = delta_y_mv - (0.5*additional_height);
            const double descendant_add_dx = geometry_rectangle_get_center_x( classifier_space ) - geometry_rectangle_get_center_x( &min_inner_space );
            const double descendant_add_dy = geometry_rectangle_get_center_y( classifier_space ) - geometry_rectangle_get_center_y( &min_inner_space );

            /* move descendants */
            pencil_classifier_layouter_private_move_embraced_descendants( this_, classifier_idx, delta_x+descendant_add_dx, delta_y+descendant_add_dy );

            /* move and resize self */
            layout_visible_classifier_shift ( the_classifier, delta_x, delta_y );
            layout_visible_classifier_expand ( the_classifier, additional_width, additional_height );
        }
    }

    /* doublecheck if title can be layed out with less lines */

    universal_array_index_sorter_destroy( &sorted_classifiers );

    TRACE_END();
}

void pencil_classifier_layouter_private_propose_grow_processing_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) DATA_VISIBLE_SET_MAX_CLASSIFIERS );

    /* sort the classifiers by their movement-needs */
    const uint32_t count_classifiers = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_classifiers; index ++ )
    {
        const layout_visible_classifier_t *const the_classifier = pencil_layout_data_get_visible_classifier_ptr( (*this_).layout_data, index );
        const geometry_rectangle_t *const classifier_symbol_box = layout_visible_classifier_get_symbol_box_const( the_classifier );

        int64_t move_need = 0;
        {
            const double classifier_area = geometry_rectangle_get_area( classifier_symbol_box );
            move_need = classifier_area;
        }

        int insert_error;
        insert_error = universal_array_index_sorter_insert( out_sorted, index, INT64_MAX - move_need );
        if ( 0 != insert_error )
        {
            TSLOG_WARNING( "not all rectangles are grown" );
        }
    }

    TRACE_END();
}

void pencil_classifier_layouter_private_get_gaps_to_classifiers( const pencil_classifier_layouter_t *this_,
                                                                 uint32_t ref_classifier_idx,
                                                                 geometry_rectangle_t* out_max_outer_bounds,
                                                                 geometry_rectangle_t* out_min_inner_space )
{
    TRACE_BEGIN();

    assert ( out_max_outer_bounds != NULL );
    assert ( out_min_inner_space != NULL );

    const uint32_t count_classifiers = pencil_layout_data_get_visible_classifier_count ( (*this_).layout_data );
    assert ( ref_classifier_idx < count_classifiers );
    const layout_visible_classifier_t *const ref_classifier = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, ref_classifier_idx );
    const geometry_rectangle_t *const ref_classifier_symbol_box = layout_visible_classifier_get_symbol_box_const( ref_classifier );
    const geometry_rectangle_t *const ref_classifier_space = layout_visible_classifier_get_space_const( ref_classifier );
    const double ref_left = geometry_rectangle_get_left( ref_classifier_symbol_box );
    const double ref_right = geometry_rectangle_get_right( ref_classifier_symbol_box );
    const double ref_top = geometry_rectangle_get_top( ref_classifier_symbol_box );
    const double ref_bottom = geometry_rectangle_get_bottom( ref_classifier_symbol_box );

    /* init out params */
    geometry_rectangle_copy( out_max_outer_bounds, (*this_).diagram_draw_area );
    geometry_rectangle_init( out_min_inner_space,
                             geometry_rectangle_get_center_x( ref_classifier_space ),
                             geometry_rectangle_get_center_y( ref_classifier_space ),
                             0.0,
                             0.0
                           );
    const double SMALL_GAP = 0.00001;

    /* check all classifiers */
    for ( uint32_t index = 0; index < count_classifiers; index ++ )
    {
        const layout_visible_classifier_t *const probe_classifier = pencil_layout_data_get_visible_classifier_const( (*this_).layout_data, index );
        const geometry_rectangle_t *const probe_symbol_box = layout_visible_classifier_get_symbol_box_const( probe_classifier );
        geometry_rectangle_t probe_total_bounds;
        geometry_rectangle_init_by_bounds( &probe_total_bounds,
                                           layout_visible_classifier_get_symbol_box_const ( probe_classifier ),
                                           layout_visible_classifier_get_label_box_const ( probe_classifier )
                                         );

        const bool is_ancestor = pencil_layout_data_is_ancestor ( (*this_).layout_data, probe_classifier, ref_classifier );
        const bool is_descendant = pencil_layout_data_is_ancestor ( (*this_).layout_data, ref_classifier, probe_classifier );
        bool finished = false;

        if ( index == ref_classifier_idx )
        {
            /* current index is myself, nothing to do */
            finished = true;
        }
        else if ( is_ancestor )
        {
            const geometry_rectangle_t *const probe_space = layout_visible_classifier_get_space_const( probe_classifier );
            /* probe is ancestor to ref */
            if ( geometry_rectangle_is_containing( probe_space, ref_classifier_symbol_box ) )
            {
                /* currently, ref is within the ancestors(probe) space, this shall not change */
                geometry_rectangle_init_by_intersect( out_max_outer_bounds, out_max_outer_bounds, probe_space );
                finished = true;
            }
            else
            {
                /* handle probe like any non-related classifier */
                finished = false;
            }
        }
        else if ( is_descendant )
        {
            /* probe is decendant of ref */
            if ( geometry_rectangle_is_containing( ref_classifier_space, &probe_total_bounds ) )
            {
                /* currently, probe is within the ancestors(ref) space, this shall not change */
                geometry_rectangle_init_by_bounds( out_min_inner_space, out_min_inner_space, &probe_total_bounds );
                finished = true;
            }
            else
            {
                /* handle probe like any non-related classifier */
                finished = false;
            }

        }

        if ( ! finished )
        {
            if ( ! geometry_rectangle_is_intersecting( probe_symbol_box, out_max_outer_bounds ) )
            {
                /* nothing to do */
                finished = true;
            }
        }

        if ( ! finished )
        {
            /* probe is unrelated to ref */
            const double probe_left = geometry_rectangle_get_left( &probe_total_bounds );
            const double probe_right = geometry_rectangle_get_right( &probe_total_bounds );
            const double probe_top = geometry_rectangle_get_top( &probe_total_bounds );
            const double probe_bottom = geometry_rectangle_get_bottom( &probe_total_bounds );
            const double max_out_left = geometry_rectangle_get_left( out_max_outer_bounds );
            const double max_out_right = geometry_rectangle_get_right( out_max_outer_bounds );
            const double max_out_top = geometry_rectangle_get_top( out_max_outer_bounds );
            const double max_out_bottom = geometry_rectangle_get_bottom( out_max_outer_bounds );
            bool adapt_top = false;
            bool adapt_bottom = false;
            bool adapt_left = false;
            bool adapt_right = false;
            if ( probe_right < ref_left )
            {
                const double delta_x = ref_left - probe_right;
                if ( probe_bottom < ref_top )
                {
                    const double delta_y = ref_top - probe_bottom;
                    if ( delta_y > delta_x )
                    {
                        /* local maximum is to adapt top */
                        adapt_top = true;
                    }
                    else
                    {
                        adapt_left = true;
                    }
                }
                else if ( probe_top > ref_bottom )
                {
                    const double delta_y = probe_top - ref_bottom;
                    if ( delta_y > delta_x )
                    {
                        /* local maximum is to adapt bottom */
                        adapt_bottom = true;
                    }
                    else
                    {
                        adapt_left = true;
                    }
                }
                else
                {
                    /* probe is to the left */
                    adapt_left = true;
                }
            }
            else if ( probe_left > ref_right )
            {
                const double delta_x = probe_left - ref_right;
                if ( probe_bottom < ref_top )
                {
                    const double delta_y = ref_top - probe_bottom;
                    if ( delta_y > delta_x )
                    {
                        /* local maximum is to adapt top */
                        adapt_top = true;
                    }
                    else
                    {
                        adapt_right = true;
                    }
                }
                else if ( probe_top > ref_bottom )
                {
                    const double delta_y = probe_top - ref_bottom;
                    if ( delta_y > delta_x )
                    {
                        /* local maximum is to adapt bottom */
                        adapt_bottom = true;
                    }
                    else
                    {
                        adapt_right = true;
                    }
                }
                else
                {
                    /* probe is to the right */
                    adapt_right = true;
                }
            }
            else
            {
                if ( probe_bottom < ref_top )
                {
                    /* probe is to the top */
                    adapt_top = true;
                }
                else if ( probe_top > ref_bottom )
                {
                    /* probe in to the bottom */
                    adapt_bottom = true;
                }
                else
                {
                    /* something went wrong, there is already an overlap */
                    TRACE_INFO_INT_INT( "unexpected: There is an overlap between two classifiers.", ref_classifier_idx, index );
                    geometry_rectangle_copy( out_max_outer_bounds, ref_classifier_symbol_box );
                }
            }
            if ( adapt_top )
            {
                geometry_rectangle_set_top( out_max_outer_bounds, probe_bottom+SMALL_GAP );
                geometry_rectangle_set_height( out_max_outer_bounds, max_out_bottom-probe_bottom-SMALL_GAP );
            }
            if ( adapt_bottom )
            {
                geometry_rectangle_set_height( out_max_outer_bounds, probe_top-SMALL_GAP-max_out_top );
            }
            if ( adapt_left )
            {
                geometry_rectangle_set_left( out_max_outer_bounds, probe_right+SMALL_GAP );
                geometry_rectangle_set_width( out_max_outer_bounds, max_out_right-probe_right-SMALL_GAP );
            }
            if ( adapt_right )
            {
                geometry_rectangle_set_width( out_max_outer_bounds, probe_left-SMALL_GAP-max_out_left );
            }
        }
    }

    TRACE_END();
}

void pencil_classifier_layouter_layout_for_list( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );
    pencil_classifier_layouter_private_sort_classifiers_by_list_order( this_, &sorted_classifiers );

    /* get the diagram coordinates */
    double diag_x = geometry_rectangle_get_left ( (*this_).diagram_draw_area );
    double diag_y = geometry_rectangle_get_top ( (*this_).diagram_draw_area );
    double diag_w = geometry_rectangle_get_width ( (*this_).diagram_draw_area );
    double diag_h = geometry_rectangle_get_height ( (*this_).diagram_draw_area );

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* adapt drawing area */
    diag_x += obj_dist;
    diag_w -= 2.0 * obj_dist;
    diag_h -= obj_dist;

    /* store the classifier symbol_box into input_data_layouter_t */
    const uint32_t count_clasfy = universal_array_index_sorter_get_count ( &sorted_classifiers );
    for ( uint32_t sort_idx = 0; sort_idx < count_clasfy; sort_idx ++ )
    {
        uint32_t index = universal_array_index_sorter_get_array_index( &sorted_classifiers, sort_idx );

        layout_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        /* get the symbol_box to modify */
        geometry_rectangle_t *classifier_symbol_box
            = layout_visible_classifier_get_symbol_box_ptr( visible_classifier2 );

        /* define the bounding box */
        geometry_rectangle_init( classifier_symbol_box,
                                 diag_x,
                                 diag_y+((sort_idx*diag_h)/count_clasfy),
                                 diag_w,
                                 (diag_h/count_clasfy)
                               );

        /* update inner space and label_box */
        pencil_classifier_composer_set_space_and_label ( &((*this_).classifier_composer),
                                                         layout_visible_classifier_get_data_const( visible_classifier2 ),
                                                         (*this_).pencil_size,
                                                         font_layout,
                                                         visible_classifier2
                                                       );
    }

    /* cleanup sorted array indices */
    universal_array_index_sorter_destroy( &sorted_classifiers );

    TRACE_END();
}

void pencil_classifier_layouter_layout_for_sequence( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );
    pencil_classifier_layouter_private_sort_classifiers_by_list_order( this_, &sorted_classifiers );

    /* get the diagram coordinates */
    double diag_x = geometry_rectangle_get_left ( (*this_).diagram_draw_area );
    double diag_y = geometry_rectangle_get_top ( (*this_).diagram_draw_area );
    double diag_w = geometry_rectangle_get_width ( (*this_).diagram_draw_area );
    double diag_h = geometry_rectangle_get_height ( (*this_).diagram_draw_area );

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* adapt drawing area */
    diag_x += obj_dist;
    diag_w -= 2.0 * obj_dist;
    diag_h -= obj_dist;

    /* store the classifier bounds into input_data_layouter_t */
    const uint32_t count_clasfy = universal_array_index_sorter_get_count ( &sorted_classifiers );
    for ( uint32_t sort_idx = 0; sort_idx < count_clasfy; sort_idx ++ )
    {
        uint32_t index = universal_array_index_sorter_get_array_index( &sorted_classifiers, sort_idx );

        layout_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        /* get the symbol_box and inner space rectangles to modify */
        geometry_rectangle_t *classifier_symbol_box
           = layout_visible_classifier_get_symbol_box_ptr( visible_classifier2 );

        /* define the bounding box */
        geometry_rectangle_init( classifier_symbol_box,
                                 diag_x+((sort_idx*diag_w)/count_clasfy),
                                 diag_y,
                                 (diag_w/count_clasfy),
                                 diag_h/8
                               );

        /* update inner space and label_box */
        pencil_classifier_composer_set_space_and_label ( &((*this_).classifier_composer),
                                                         layout_visible_classifier_get_data_const( visible_classifier2 ),
                                                         (*this_).pencil_size,
                                                         font_layout,
                                                         visible_classifier2
                                                       );
    }

    /* cleanup sorted array indices */
    universal_array_index_sorter_destroy( &sorted_classifiers );

    TRACE_END();
}

void pencil_classifier_layouter_layout_for_timing( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );
    pencil_classifier_layouter_private_sort_classifiers_by_list_order( this_, &sorted_classifiers );

    /* get the diagram coordinates */
    double diag_x = geometry_rectangle_get_left ( (*this_).diagram_draw_area );
    double diag_y = geometry_rectangle_get_top ( (*this_).diagram_draw_area );
    double diag_w = geometry_rectangle_get_width ( (*this_).diagram_draw_area );
    double diag_h = geometry_rectangle_get_height ( (*this_).diagram_draw_area );

    /* get preferred object distance */
    const double obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* adapt drawing area */
    diag_x += obj_dist;
    diag_w -= 2.0 * obj_dist;
    diag_h -= obj_dist;

    /* store the classifier symbol_box into input_data_layouter_t */
    const uint32_t count_clasfy = universal_array_index_sorter_get_count ( &sorted_classifiers );
    for ( uint32_t sort_idx = 0; sort_idx < count_clasfy; sort_idx ++ )
    {
        uint32_t index = universal_array_index_sorter_get_array_index( &sorted_classifiers, sort_idx );

        layout_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_layout_data_get_visible_classifier_ptr ( (*this_).layout_data, index );

        /* get the symbol_box and inner space rectangles to modify */
        geometry_rectangle_t *classifier_symbol_box
            = layout_visible_classifier_get_symbol_box_ptr( visible_classifier2 );

        /* define the bounding box */
        geometry_rectangle_init( classifier_symbol_box,
                                 diag_x,
                                 diag_y+((sort_idx*diag_h)/count_clasfy),
                                 diag_w/4,
                                 (diag_h/count_clasfy)
                               );

        /* update inner space and label_box */
        pencil_classifier_composer_set_space_and_label ( &((*this_).classifier_composer),
                                                         layout_visible_classifier_get_data_const( visible_classifier2 ),
                                                         (*this_).pencil_size,
                                                         font_layout,
                                                         visible_classifier2
                                                       );
    }

    /* cleanup sorted array indices */
    universal_array_index_sorter_destroy( &sorted_classifiers );

    TRACE_END();
}


/*
Copyright 2017-2020 Andreas Warnke

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
