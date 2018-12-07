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
                                      geometry_rectangle_t *default_classifier_size,
                                      geometry_non_linear_scale_t *x_scale,
                                      geometry_non_linear_scale_t *y_scale,
                                      pencil_feature_layouter_t *feature_layouter )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );
    assert( NULL != feature_layouter );

    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;
    (*this_).default_classifier_size = default_classifier_size;
    (*this_).x_scale = x_scale;
    (*this_).y_scale = y_scale;
    (*this_).feature_layouter = feature_layouter;
    pencil_classifier_painter_init( &((*this_).classifier_painter) );

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

    pencil_classifier_painter_destroy( &((*this_).classifier_painter) );

    TRACE_END();
}

void pencil_classifier_layouter_estimate_bounds ( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* store the classifier bounds into input_data_layouter_t */
    uint32_t count_clasfy;
    count_clasfy = pencil_layout_data_get_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        layout_visible_classifier_t *classifier_layout;
        classifier_layout = pencil_layout_data_get_classifier_ptr ( (*this_).layout_data, index );
        const data_visible_classifier_t *visible_classifier2;
        visible_classifier2 = layout_visible_classifier_get_data_ptr ( classifier_layout );

        const data_classifier_t *classifier2;
        classifier2 = data_visible_classifier_get_classifier_const( visible_classifier2 );

        /* get the bounds and inner space rectangles to modify */
        geometry_rectangle_t *classifier_bounds;
        classifier_bounds = layout_visible_classifier_get_bounds_ptr( classifier_layout );
        geometry_rectangle_t *classifier_space;
        classifier_space = layout_visible_classifier_get_space_ptr( classifier_layout );

        /* determine the minimum size of the classifier */
        /* and initialize bounds and space */
        pencil_classifier_painter_get_minimum_bounds ( &((*this_).classifier_painter),
                                                       visible_classifier2,
                                                       (*this_).pencil_size,
                                                       font_layout,
                                                       classifier_bounds,
                                                       classifier_space
                                                     );

        /* determine the minimum size of the contained features */
        double extend_to_right = 0.0;
        double extend_to_bottom = 0.0;
        {
            double min_space_width = geometry_rectangle_get_width( classifier_space );

            geometry_rectangle_t features_bounds;
            geometry_rectangle_init_empty( &features_bounds );
            pencil_feature_layouter_calculate_features_bounds ( (*this_).feature_layouter,
                                                                layout_visible_classifier_get_diagramelement_id( classifier_layout ),
                                                                font_layout,
                                                                &features_bounds
                                                              );

            /* adjust width and height to feature bounds */
            double feat_width = geometry_rectangle_get_width( &features_bounds );
            if ( min_space_width < feat_width )
            {
                extend_to_right = feat_width - min_space_width;
            }
            extend_to_bottom += geometry_rectangle_get_height( &features_bounds );

            geometry_rectangle_destroy( &features_bounds );
        }

        /* determine minimum size */
        double min_width = geometry_rectangle_get_width( classifier_bounds );
        double min_height = geometry_rectangle_get_height( classifier_bounds );
        min_height += extend_to_bottom;
        min_width += extend_to_right;

        /* determine default size */
        double def_width = geometry_rectangle_get_width( (*this_).default_classifier_size );
        double def_height = geometry_rectangle_get_height( (*this_).default_classifier_size );

        /* adjust width and height to default or to minumum bounds */
        if ( def_width > min_width )
        {
            extend_to_right += def_width - min_width;
        }
        if ( def_height > min_height )
        {
            extend_to_bottom += def_height - min_height;
        }
        geometry_rectangle_expand( classifier_bounds, extend_to_right, extend_to_bottom );
        geometry_rectangle_expand( classifier_space, extend_to_right, extend_to_bottom );

        /* move the classifier rectangles */
        double width = geometry_rectangle_get_width( classifier_bounds );
        double height = geometry_rectangle_get_height( classifier_bounds );
        int32_t order_x = data_classifier_get_x_order( classifier2 );
        int32_t order_y = data_classifier_get_y_order( classifier2 );
        double center_x = geometry_non_linear_scale_get_location( (*this_).x_scale, order_x );
        double center_y = geometry_non_linear_scale_get_location( (*this_).y_scale, order_y );
        geometry_rectangle_shift( classifier_bounds, center_x-0.5*width, center_y-0.5*height );
        geometry_rectangle_shift( classifier_space, center_x-0.5*width, center_y-0.5*height );
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
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted_relationships );

    /* search containment relations */
    for ( uint32_t rel_sort_idx = 0; rel_sort_idx < count_sorted; rel_sort_idx ++ )
    {
        uint32_t rel_idx;
        rel_idx = universal_array_index_sorter_get_array_index( &sorted_relationships, rel_sort_idx );

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

    uint32_t rel_count = pencil_layout_data_get_relationship_count( (*this_).layout_data );
    for ( uint32_t rel_idx = 0; rel_idx < rel_count; rel_idx ++ )
    {
        layout_relationship_t *the_relationship;
        the_relationship = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, rel_idx );

        /* count the descendants */
        layout_visible_classifier_t *from_classifier;
        from_classifier = layout_relationship_get_from_classifier_ptr( the_relationship );
        uint32_t from_descendant_count;
        from_descendant_count = pencil_layout_data_count_descendants( (*this_).layout_data, from_classifier );

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

    data_relationship_type_t the_type;
    the_type = data_relationship_get_main_type ( layout_relationship_get_data_ptr( the_relationship ));

    if ( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == the_type )
    {
        layout_visible_classifier_t *from_classifier;
        layout_visible_classifier_t *to_classifier;
        from_classifier = layout_relationship_get_from_classifier_ptr( the_relationship );
        to_classifier = layout_relationship_get_to_classifier_ptr( the_relationship );
        if ( from_classifier != to_classifier )
        {
            geometry_rectangle_t *parent_bounds;
            parent_bounds = layout_visible_classifier_get_bounds_ptr ( from_classifier );
            geometry_rectangle_t *parent_space;
            parent_space = layout_visible_classifier_get_space_ptr ( from_classifier );
            geometry_rectangle_t *child_bounds;
            child_bounds = layout_visible_classifier_get_bounds_ptr ( to_classifier );

            /* try embrace child */
            geometry_rectangle_t probe_parent_bounds;  /* try out a new parent bounds rectangle */
            geometry_rectangle_t probe_parent_space;  /* try out a new parent space rectangle */
            geometry_rectangle_copy( &probe_parent_bounds, parent_bounds );
            geometry_rectangle_copy( &probe_parent_space, parent_space );
            double extend_to_left = 0.0;
            double extend_to_right = 0.0;
            double extend_to_top = 0.0;
            double extend_to_bottom = 0.0;
            extend_to_left = geometry_rectangle_get_left( parent_space ) - geometry_rectangle_get_left( child_bounds );
            extend_to_top = geometry_rectangle_get_top( parent_space ) - geometry_rectangle_get_top( child_bounds );
            extend_to_right = geometry_rectangle_get_right( child_bounds ) - geometry_rectangle_get_right( parent_space );
            extend_to_bottom = geometry_rectangle_get_bottom( child_bounds ) - geometry_rectangle_get_bottom( parent_space );
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
            geometry_rectangle_expand( &probe_parent_bounds, extend_to_left+extend_to_right, extend_to_top+extend_to_bottom );
            geometry_rectangle_shift( &probe_parent_bounds, -extend_to_left, -extend_to_top );
            geometry_rectangle_expand( &probe_parent_space, extend_to_left+extend_to_right, extend_to_top+extend_to_bottom );
            geometry_rectangle_shift( &probe_parent_space, -extend_to_left, -extend_to_top );

            /* check what else would be embraced */
            bool illegal_overlap = false;
            uint32_t count_clasfy;
            count_clasfy = pencil_layout_data_get_classifier_count ( (*this_).layout_data );
            for ( uint32_t c_index = 0; c_index < count_clasfy; c_index ++ )
            {
                layout_visible_classifier_t *probe_classifier;
                probe_classifier = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, c_index );

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
                        geometry_rectangle_t *current_bounds;
                        current_bounds = layout_visible_classifier_get_bounds_ptr ( probe_classifier );
                        illegal_overlap |= geometry_rectangle_is_intersecting( current_bounds, &probe_parent_bounds );
                    }
                }
            }

            /* cancel or commit */
            if ( ! illegal_overlap )
            {
                geometry_rectangle_replace ( parent_bounds, &probe_parent_bounds );
                geometry_rectangle_replace ( parent_space, &probe_parent_space );
            }

            /* cleanup */
            geometry_rectangle_destroy( &probe_parent_bounds );
            geometry_rectangle_destroy( &probe_parent_space );
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
    uint32_t rel_count = pencil_layout_data_get_relationship_count( (*this_).layout_data );
    for ( uint32_t rel_idx = 0; rel_idx < rel_count; rel_idx ++ )
    {
        layout_relationship_t *the_relationship;
        the_relationship = pencil_layout_data_get_relationship_ptr( (*this_).layout_data, rel_idx );
        const data_relationship_t *the_rel_data;
        the_rel_data = layout_relationship_get_data_ptr( the_relationship );

        data_relationship_type_t the_type;
        pencil_visibility_t visibility;
        the_type = data_relationship_get_main_type ( the_rel_data );
        visibility = layout_relationship_get_visibility( the_relationship );

        if (( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == the_type )
            && ( PENCIL_VISIBILITY_SHOW == visibility ))
        {
            layout_visible_classifier_t *from_classifier;
            layout_visible_classifier_t *to_classifier;
            from_classifier = layout_relationship_get_from_classifier_ptr( the_relationship );
            to_classifier = layout_relationship_get_to_classifier_ptr( the_relationship );
            if ( from_classifier != to_classifier )
            {
                geometry_rectangle_t *parent_space;
                parent_space = layout_visible_classifier_get_space_ptr ( from_classifier );
                geometry_rectangle_t *child_bounds;
                child_bounds = layout_visible_classifier_get_bounds_ptr ( to_classifier );

                /* hide if parent embraced child completely */
                if ( geometry_rectangle_is_containing( parent_space, child_bounds ) )
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
    pencil_classifier_layouter_private_propose_processing_order ( this_, &sorted );

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
        pencil_classifier_layouter_private_propose_solutions ( this_,
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
            pencil_classifier_layouter_private_select_solution ( this_,
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
        the_classifier = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, index );
        geometry_rectangle_t *classifier_bounds;
        classifier_bounds = layout_visible_classifier_get_bounds_ptr( the_classifier );
        geometry_rectangle_shift ( classifier_bounds, solution_move_dx[index_of_best], solution_move_dy[index_of_best] );
        /* move all contained features */
        geometry_rectangle_t *classifier_space;
        classifier_space = layout_visible_classifier_get_space_ptr( the_classifier );
        geometry_rectangle_shift ( classifier_space, solution_move_dx[index_of_best], solution_move_dy[index_of_best] );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_classifier_layouter_private_propose_processing_order ( pencil_classifier_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_INPUT_DATA_MAX_CLASSIFIERS );

    /* sort the classifiers by their movement-needs */
    uint32_t count_clasfy;
    count_clasfy = pencil_layout_data_get_classifier_count ( (*this_).layout_data );
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        layout_visible_classifier_t *the_classifier;
        the_classifier = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, index );
        geometry_rectangle_t *classifier_bounds;
        classifier_bounds = layout_visible_classifier_get_bounds_ptr( the_classifier );

        int64_t simpleness = 0;  /* the lower the number, the ealier the classifier will be processed. Unit is area(=square-length). */

        /* reduce simpleness by area outside the diagram border: the more outside diagram area, the earlier it should be moved */
        {
            geometry_rectangle_t border_intersect;
            int intersect_error2;
            intersect_error2 = geometry_rectangle_init_by_intersect( &border_intersect, classifier_bounds, (*this_).diagram_draw_area );
            if ( 0 != intersect_error2 )
            {
                TSLOG_WARNING( "a rectangle to be drawn is completely outside the diagram area" );
            }

            simpleness += 16.0 * geometry_rectangle_get_area( &border_intersect );
            simpleness -= 16.0 * geometry_rectangle_get_area( classifier_bounds );

            geometry_rectangle_destroy( &border_intersect );
        }

        /* reduce simpleness by intersects with other rectangles: the more intersects, the earlier it should be moved */
        for ( uint32_t probe_index = 0; probe_index < count_clasfy; probe_index ++ )
        {
            layout_visible_classifier_t *probe_classifier;
            probe_classifier = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, probe_index );
            geometry_rectangle_t *probe_bounds;
            probe_bounds = layout_visible_classifier_get_bounds_ptr( probe_classifier );

            geometry_rectangle_t intersect;
            int intersect_error;
            intersect_error = geometry_rectangle_init_by_intersect( &intersect, classifier_bounds, probe_bounds );

            if ( 0 == intersect_error )
            {
                simpleness -= geometry_rectangle_get_area( &intersect );
            }

            geometry_rectangle_destroy( &intersect );
        }

        /* reduce simpleness by own size: the bigger the object, the earlier it should be moved */
        {
            double default_classifier_area = geometry_rectangle_get_area( (*this_).default_classifier_size );
            double classifier_area = geometry_rectangle_get_area( classifier_bounds );
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

void pencil_classifier_layouter_private_propose_solutions ( pencil_classifier_layouter_t *this_,
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
    uint32_t index;
    index = universal_array_index_sorter_get_array_index( sorted, sort_index );
    layout_visible_classifier_t *the_classifier;
    the_classifier = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, index );
    geometry_rectangle_t *classifier_bounds;
    classifier_bounds = layout_visible_classifier_get_bounds_ptr( the_classifier );
    double top;
    double bottom;
    double left;
    double right;
    top = geometry_rectangle_get_top ( classifier_bounds );
    bottom = geometry_rectangle_get_bottom ( classifier_bounds );
    left = geometry_rectangle_get_left ( classifier_bounds );
    right = geometry_rectangle_get_right ( classifier_bounds );

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
    double gap = pencil_size_get_standard_object_border( (*this_).pencil_size );

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
        layout_visible_classifier_t *the_probe;
        the_probe = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, probe_index );
        geometry_rectangle_t *probe_bounds;
        probe_bounds = layout_visible_classifier_get_bounds_ptr( the_probe );
        double probe_top;
        double probe_bottom;
        double probe_left;
        double probe_right;
        probe_top = geometry_rectangle_get_top ( probe_bounds );
        probe_bottom = geometry_rectangle_get_bottom ( probe_bounds );
        probe_left = geometry_rectangle_get_left ( probe_bounds );
        probe_right = geometry_rectangle_get_right ( probe_bounds );

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
            visible_classifier_data = layout_visible_classifier_get_data_ptr ( the_probe );
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
    visible_classifier = layout_visible_classifier_get_data_ptr ( the_classifier );
    const data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_const( visible_classifier );
    TRACE_INFO_STR( "classifier:", data_classifier_get_name_ptr( classifier ) );

    TRACE_END();
}

void pencil_classifier_layouter_private_select_solution ( pencil_classifier_layouter_t *this_,
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
    uint32_t index;
    index = universal_array_index_sorter_get_array_index( sorted, sort_index );
    layout_visible_classifier_t *the_classifier;
    the_classifier = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, index );
    geometry_rectangle_t *classifier_bounds;
    classifier_bounds = layout_visible_classifier_get_bounds_ptr( the_classifier );

    /* check all solutions */
    for ( uint32_t solution_index = 0; solution_index < solutions_count; solution_index ++ )
    {
        /* calculate the solution rectangle */
        geometry_rectangle_t solution_bounds;
        geometry_rectangle_copy( &solution_bounds, classifier_bounds );
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

                layout_visible_classifier_t *the_probe;
                the_probe = pencil_layout_data_get_classifier_ptr( (*this_).layout_data, probe_index );
                geometry_rectangle_t *probe_bounds;
                probe_bounds = layout_visible_classifier_get_bounds_ptr( the_probe );

                geometry_rectangle_t probe_intersect;
                int intersect_err;
                intersect_err = geometry_rectangle_init_by_intersect( &probe_intersect, &solution_bounds, probe_bounds );
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

void pencil_classifier_layouter_layout_for_list( pencil_classifier_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* sort the classifiers according to their list_order */
    universal_array_index_sorter_t sorted_classifiers;
    universal_array_index_sorter_init( &sorted_classifiers );
    pencil_classifier_layouter_private_sort_classifiers_by_list_order( this_, &sorted_classifiers );

    /* get the diagram coordinates */
    double diag_x;
    double diag_y;
    double diag_w;
    double diag_h;
    diag_x = geometry_rectangle_get_left ( (*this_).diagram_draw_area );
    diag_y = geometry_rectangle_get_top ( (*this_).diagram_draw_area );
    diag_w = geometry_rectangle_get_width ( (*this_).diagram_draw_area );
    diag_h = geometry_rectangle_get_height ( (*this_).diagram_draw_area );

    /* get preferred object distance */
    double obj_dist;
    obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* adapt drawing area */
    diag_x += obj_dist;
    diag_w -= 2.0 * obj_dist;
    diag_h -= obj_dist;

    /* store the classifier bounds into input_data_layouter_t */
    uint32_t count_clasfy;
    count_clasfy = universal_array_index_sorter_get_count ( &sorted_classifiers );
    for ( uint32_t sort_idx = 0; sort_idx < count_clasfy; sort_idx ++ )
    {
        uint32_t index = universal_array_index_sorter_get_array_index( &sorted_classifiers, sort_idx );

        layout_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_layout_data_get_classifier_ptr ( (*this_).layout_data, index );

        /*
        const data_classifier_t *classifier2;
        classifier2 = layout_visible_classifier_get_classifier_ptr( visible_classifier2 );
        */

        /* get the bounds and inner space rectangles to modify */
        geometry_rectangle_t *classifier_bounds;
        classifier_bounds = layout_visible_classifier_get_bounds_ptr( visible_classifier2 );
        geometry_rectangle_t *classifier_space;
        classifier_space = layout_visible_classifier_get_space_ptr( visible_classifier2 );

        /* define the bounding box */
        geometry_rectangle_init( classifier_bounds,
                                 diag_x,
                                 diag_y+((sort_idx*diag_h)/count_clasfy),
                                 diag_w,
                                 (diag_h/count_clasfy)
                                );

        /* update inner space */
        pencil_classifier_painter_get_drawing_space ( &((*this_).classifier_painter),
                                                      layout_visible_classifier_get_data_ptr( visible_classifier2 ),
                                                      (*this_).pencil_size,
                                                      classifier_bounds,
                                                      font_layout,
                                                      classifier_space
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
    double diag_x;
    double diag_y;
    double diag_w;
    double diag_h;
    diag_x = geometry_rectangle_get_left ( (*this_).diagram_draw_area );
    diag_y = geometry_rectangle_get_top ( (*this_).diagram_draw_area );
    diag_w = geometry_rectangle_get_width ( (*this_).diagram_draw_area );
    diag_h = geometry_rectangle_get_height ( (*this_).diagram_draw_area );

    /* get preferred object distance */
    double obj_dist;
    obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* adapt drawing area */
    diag_x += obj_dist;
    diag_w -= 2.0 * obj_dist;
    diag_h -= obj_dist;

    /* store the classifier bounds into input_data_layouter_t */
    uint32_t count_clasfy;
    count_clasfy = universal_array_index_sorter_get_count ( &sorted_classifiers );
    for ( uint32_t sort_idx = 0; sort_idx < count_clasfy; sort_idx ++ )
    {
        uint32_t index = universal_array_index_sorter_get_array_index( &sorted_classifiers, sort_idx );

        layout_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_layout_data_get_classifier_ptr ( (*this_).layout_data, index );

        /*
        const data_classifier_t *classifier2;
        classifier2 = layout_visible_classifier_get_classifier_ptr( visible_classifier2 );
        */

        /* get the bounds and inner space rectangles to modify */
        geometry_rectangle_t *classifier_bounds;
        classifier_bounds = layout_visible_classifier_get_bounds_ptr( visible_classifier2 );
        geometry_rectangle_t *classifier_space;
        classifier_space = layout_visible_classifier_get_space_ptr( visible_classifier2 );

        /* define the bounding box */
        geometry_rectangle_init( classifier_bounds,
                                 diag_x+((sort_idx*diag_w)/count_clasfy),
                                 diag_y,
                                 (diag_w/count_clasfy),
                                 diag_h/8
                               );

        /* update inner space */
        pencil_classifier_painter_get_drawing_space ( &((*this_).classifier_painter),
                                                      layout_visible_classifier_get_data_ptr( visible_classifier2 ),
                                                      (*this_).pencil_size,
                                                      classifier_bounds,
                                                      font_layout,
                                                      classifier_space
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
    double diag_x;
    double diag_y;
    double diag_w;
    double diag_h;
    diag_x = geometry_rectangle_get_left ( (*this_).diagram_draw_area );
    diag_y = geometry_rectangle_get_top ( (*this_).diagram_draw_area );
    diag_w = geometry_rectangle_get_width ( (*this_).diagram_draw_area );
    diag_h = geometry_rectangle_get_height ( (*this_).diagram_draw_area );

    /* get preferred object distance */
    double obj_dist;
    obj_dist = pencil_size_get_preferred_object_distance( (*this_).pencil_size );

    /* adapt drawing area */
    diag_x += obj_dist;
    diag_w -= 2.0 * obj_dist;
    diag_h -= obj_dist;

    /* store the classifier bounds into input_data_layouter_t */
    uint32_t count_clasfy;
    count_clasfy = universal_array_index_sorter_get_count ( &sorted_classifiers );
    for ( uint32_t sort_idx = 0; sort_idx < count_clasfy; sort_idx ++ )
    {
        uint32_t index = universal_array_index_sorter_get_array_index( &sorted_classifiers, sort_idx );

        layout_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_layout_data_get_classifier_ptr ( (*this_).layout_data, index );

        /*
        const data_classifier_t *classifier2;
        classifier2 = layout_visible_classifier_get_classifier_ptr( visible_classifier2 );
        */

        /* get the bounds and inner space rectangles to modify */
        geometry_rectangle_t *classifier_bounds;
        classifier_bounds = layout_visible_classifier_get_bounds_ptr( visible_classifier2 );
        geometry_rectangle_t *classifier_space;
        classifier_space = layout_visible_classifier_get_space_ptr( visible_classifier2 );

        /* define the bounding box */
        geometry_rectangle_init( classifier_bounds,
                                 diag_x,
                                 diag_y+((sort_idx*diag_h)/count_clasfy),
                                 diag_w/4,
                                 (diag_h/count_clasfy)
                               );

        /* update inner space */
        pencil_classifier_painter_get_drawing_space ( &((*this_).classifier_painter),
                                                      layout_visible_classifier_get_data_ptr( visible_classifier2 ),
                                                      (*this_).pencil_size,
                                                      classifier_bounds,
                                                      font_layout,
                                                      classifier_space
                                                    );
    }

    /* cleanup sorted array indices */
    universal_array_index_sorter_destroy( &sorted_classifiers );

    TRACE_END();
}


/*
Copyright 2017-2018 Andreas Warnke

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
