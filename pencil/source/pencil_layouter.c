/* File: pencil_layouter.c; Copyright and License: see below */

#include "pencil_layouter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_layouter_init( pencil_layouter_t *this_, const data_visible_set_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    pencil_size_init_empty( &((*this_).pencil_size) );
    geometry_non_linear_scale_init( &((*this_).x_scale), 0.0, 1.0 );
    geometry_non_linear_scale_init( &((*this_).y_scale), 0.0, 1.0 );
    geometry_dimensions_init_empty( &((*this_).default_classifier_size) );

    pencil_layout_data_init( &((*this_).layout_data), input_data );

    pencil_diagram_painter_init( &((*this_).diagram_painter) );

    pencil_feature_layouter_init( &((*this_).feature_layouter),
                                  &((*this_).layout_data),
                                  &((*this_).pencil_size)
                                );
    pencil_feat_label_layouter_init( &((*this_).feature_label_layouter),
                                     &((*this_).layout_data),
                                     &((*this_).pencil_size)
                                   );
    pencil_classifier_layouter_init( &((*this_).pencil_classifier_layouter),
                                     &((*this_).layout_data),
                                     &((*this_).pencil_size),
                                     &((*this_).default_classifier_size),
                                     &((*this_).x_scale),
                                     &((*this_).y_scale),
                                     &((*this_).feature_layouter)
                                   );
    pencil_relationship_layouter_init( &((*this_).pencil_relationship_layouter),
                                       &((*this_).layout_data),
                                       &((*this_).pencil_size)
                                     );
    pencil_rel_label_layouter_init( &((*this_).relationship_label_layouter),
                                    &((*this_).layout_data),
                                    &((*this_).pencil_size)
                                  );
    TRACE_END();
}

void pencil_layouter_reinit( pencil_layouter_t *this_, const data_visible_set_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    /* re-initialize the layout data objects */
    pencil_layout_data_reinit( &((*this_).layout_data), input_data );

    TRACE_END();
}

void pencil_layouter_destroy( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_rel_label_layouter_destroy( &((*this_).relationship_label_layouter) );
    pencil_relationship_layouter_destroy( &((*this_).pencil_relationship_layouter) );
    pencil_classifier_layouter_destroy( &((*this_).pencil_classifier_layouter) );
    pencil_feat_label_layouter_destroy( &((*this_).feature_label_layouter) );
    pencil_feature_layouter_destroy( &((*this_).feature_layouter) );

    pencil_diagram_painter_destroy( &((*this_).diagram_painter) );

    pencil_size_destroy( &((*this_).pencil_size) );
    geometry_non_linear_scale_destroy( &((*this_).x_scale) );
    geometry_non_linear_scale_destroy( &((*this_).y_scale) );
    geometry_dimensions_destroy( &((*this_).default_classifier_size) );

    pencil_layout_data_destroy( &((*this_).layout_data) );

    TRACE_END();
}

void pencil_layouter_define_grid ( pencil_layouter_t *this_, geometry_rectangle_t diagram_bounds )
{
    TRACE_BEGIN();

    /* get the diagram data */
    layout_diagram_t *the_diagram;
    the_diagram = pencil_layout_data_get_diagram_ptr( &((*this_).layout_data) );
    const data_diagram_t *diagram_data;
    diagram_data = layout_diagram_get_data_const ( the_diagram );

    /* update the bounding rectangle */
    geometry_rectangle_trace ( &diagram_bounds );
    layout_diagram_set_bounds( the_diagram, &diagram_bounds );

    /* calculate the pencil-sizes and the drawing rectangle */
    double width = geometry_rectangle_get_width ( &diagram_bounds );
    double height = geometry_rectangle_get_height ( &diagram_bounds );
    pencil_size_reinit( &((*this_).pencil_size), width, height );

    geometry_rectangle_t *diagram_draw_area;
    diagram_draw_area = layout_diagram_get_draw_area_ptr( the_diagram );
    pencil_diagram_painter_get_drawing_space ( &((*this_).diagram_painter),
                                               diagram_data,
                                               &((*this_).pencil_size),
                                               &diagram_bounds,
                                               diagram_draw_area
                                             );

    /* calculate the axis scales */
    geometry_rectangle_trace ( diagram_draw_area );
    double draw_left = geometry_rectangle_get_left ( diagram_draw_area );
    double draw_top = geometry_rectangle_get_top ( diagram_draw_area );
    double draw_right = geometry_rectangle_get_right ( diagram_draw_area );
    double draw_bottom = geometry_rectangle_get_bottom ( diagram_draw_area );
    geometry_non_linear_scale_reinit( &((*this_).x_scale), draw_left, draw_right );
    geometry_non_linear_scale_reinit( &((*this_).y_scale), draw_top, draw_bottom );

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_layout_data_get_visible_classifier_count ( &((*this_).layout_data) );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        const layout_visible_classifier_t *const visible_classifier
            = pencil_layout_data_get_visible_classifier_ptr ( &((*this_).layout_data), index );
        const data_classifier_t *const classifier_data
            = layout_visible_classifier_get_classifier_const( visible_classifier );
        const uint32_t visible_descendants
            = pencil_layout_data_count_descendants( &((*this_).layout_data), visible_classifier );

        /* adjust the non-linear scales for this classifier (if no contained descendants) */
        if ( 0 == visible_descendants )
        {
            geometry_non_linear_scale_add_order ( &((*this_).x_scale), data_classifier_get_x_order( classifier_data ) );
            geometry_non_linear_scale_add_order ( &((*this_).y_scale), data_classifier_get_y_order( classifier_data ) );
        }
    }

    TRACE_END();
}

void pencil_layouter_layout_elements ( pencil_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* get the diagram data */
    layout_diagram_t *the_diagram;
    the_diagram = pencil_layout_data_get_diagram_ptr( &((*this_).layout_data) );
    const data_diagram_t *diagram_data;
    diagram_data = layout_diagram_get_data_const ( the_diagram );
    data_diagram_type_t diag_type;
    diag_type = data_diagram_get_diagram_type ( diagram_data );

    /* adjust the default classifier rectangle */
    pencil_layouter_private_propose_default_classifier_size( this_ );

    /* store the classifier bounds into input_data_layouter_t */
    if ( DATA_DIAGRAM_TYPE_LIST == diag_type )
    {
        /* calculate the classifier shapes */
        pencil_classifier_layouter_layout_for_list( &((*this_).pencil_classifier_layouter), font_layout );

        /* calculate the feature shapes */
        pencil_feature_layouter_do_layout( &((*this_).feature_layouter), font_layout );

        /* hide relationships in simple list and box diagrams */
        pencil_relationship_layouter_layout_void( &((*this_).pencil_relationship_layouter) );

        /* layout labels of features */
        pencil_feat_label_layouter_do_layout( &((*this_).feature_label_layouter), font_layout );
    }
    else if ( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM == diag_type )
    {
        /* calculate the classifier shapes */
        pencil_classifier_layouter_layout_for_sequence( &((*this_).pencil_classifier_layouter), font_layout );

        /* calculate the feature shapes */
        pencil_feature_layouter_do_layout( &((*this_).feature_layouter), font_layout );

        /* calculate the relationship shapes for a sequence diagram */
        pencil_relationship_layouter_layout_for_sequence( &((*this_).pencil_relationship_layouter) );

        /* layout labels of relationships */
        pencil_rel_label_layouter_do_layout( &((*this_).relationship_label_layouter), font_layout );
    }
    else if ( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == diag_type )
    {
        /* calculate the classifier shapes */
        pencil_classifier_layouter_layout_for_timing( &((*this_).pencil_classifier_layouter), font_layout );

        /* calculate the feature shapes */
        pencil_feature_layouter_do_layout( &((*this_).feature_layouter), font_layout );

        /* calculate the relationship shapes for a timing diagram */
        pencil_relationship_layouter_layout_for_timing( &((*this_).pencil_relationship_layouter) );

        /* layout labels of relationships */
        pencil_rel_label_layouter_do_layout( &((*this_).relationship_label_layouter), font_layout );
    }
    else
    {
        /* store the classifier bounds into input_data_layouter_t */
        pencil_classifier_layouter_estimate_bounds( &((*this_).pencil_classifier_layouter), font_layout );

        /* move the classifiers to avoid overlaps */
        pencil_classifier_layouter_move_to_avoid_overlaps( &((*this_).pencil_classifier_layouter) );

        /* parent classifiers embrace their children step by step */
        pencil_classifier_layouter_embrace_children( &((*this_).pencil_classifier_layouter), font_layout );

        /* classifiers embrace all children at once and move them if there is space available */
        pencil_classifier_layouter_move_and_embrace_children( &((*this_).pencil_classifier_layouter), font_layout );

        /* calculate the feature shapes */
        pencil_feature_layouter_do_layout( &((*this_).feature_layouter), font_layout );

        if ( DATA_DIAGRAM_TYPE_BOX_DIAGRAM == diag_type )
        {
            /* hide relationships in simple list and box diagrams */
            pencil_relationship_layouter_layout_void( &((*this_).pencil_relationship_layouter) );
        }
        else if (( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM == diag_type )||( DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM == diag_type ))
        {
            /* calculate the relationship shapes for a communication diagram or an interaction overview diagram (scenario-relations only) */
            pencil_relationship_layouter_layout_for_communication( &((*this_).pencil_relationship_layouter) );
        }
        else
        {
            /* calculate the relationship shapes */
            pencil_relationship_layouter_layout_standard( &((*this_).pencil_relationship_layouter) );
        }

        /* hide containment relationships if children are embraced */
        pencil_classifier_layouter_hide_relations_of_embraced_children( &((*this_).pencil_classifier_layouter) );

        /* layout labels of features and relationships */
        pencil_feat_label_layouter_do_layout( &((*this_).feature_label_layouter), font_layout );
        pencil_rel_label_layouter_do_layout( &((*this_).relationship_label_layouter), font_layout );

    }

    TRACE_END();
}

void pencil_layouter_private_propose_default_classifier_size ( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* determine grid cell size */
    const double grid_width = geometry_non_linear_scale_get_grid_distances ( &((*this_).x_scale) );
    const double grid_height = geometry_non_linear_scale_get_grid_distances ( &((*this_).y_scale) );

    /* determine standard gap between objects */
    const double gap = pencil_size_get_preferred_object_distance( &((*this_).pencil_size) );

    /* set the default size to grid cell minus a gap on each side, minus extra gap on top for containers */
    geometry_dimensions_t *const default_size = &((*this_).default_classifier_size);
    geometry_dimensions_reinit( default_size, grid_width, grid_height );
    const double x_space = 3.0 * gap;  /* space for enclosing parents and for relationships */
    const double y_space = 4.0 * gap;  /* space for enclosing parents (including title-line) and for relationships */
    geometry_dimensions_expand ( default_size, -x_space, -y_space ); /* ensures non-negative values */

    /* for aesthetic reasons, ensure that the default dimension is more wide than high */
    const double w = geometry_dimensions_get_width( default_size );
    const double h = geometry_dimensions_get_height( default_size );
    if ( w * 0.75 < h )
    {
        geometry_dimensions_reinit( default_size, w, w * 0.75 );
    }

    TRACE_END();
}

pencil_error_t pencil_layouter_get_object_id_at_pos ( const pencil_layouter_t *this_,
                                                      double x,
                                                      double y,
                                                      double snap_distance,
                                                      pencil_type_filter_t filter,
                                                      data_id_pair_t* out_selected_id,
                                                      data_id_pair_t* out_surrounding_id )
{
    TRACE_BEGIN();
    assert( NULL != out_selected_id );
    assert( NULL != out_surrounding_id );

    pencil_error_t result = PENCIL_ERROR_NONE;
    data_id_pair_reinit_void( out_selected_id );
    data_id_pair_reinit_void( out_surrounding_id );
    const layout_diagram_t *the_diagram;
    the_diagram = pencil_layout_data_get_diagram_const( &((*this_).layout_data) );
    const data_diagram_t *diagram_data;
    diagram_data = layout_diagram_get_data_const ( the_diagram );

    /* get bounding box */
    const geometry_rectangle_t *diagram_bounds;
    diagram_bounds = layout_diagram_get_bounds_const( the_diagram );

    if ( geometry_rectangle_contains( diagram_bounds, x, y ) )
    {
        /* check the relationship shapes */
        {
            result = pencil_layouter_private_get_relationship_id_at_pos( this_,
                                                                         x,
                                                                         y,
                                                                         snap_distance,
                                                                         out_selected_id
                                                                       );
        }

        /* determine a feature at the given position */
        if ( ! data_id_pair_is_valid( out_selected_id ) )
        {
            result = pencil_layouter_private_get_feature_id_at_pos( this_,
                                                                    x,
                                                                    y,
                                                                    filter,
                                                                    out_selected_id,
                                                                    out_surrounding_id
                                                                  );
        }

        /* determine a classifier at the given position */
        if ( ! data_id_pair_is_valid( out_selected_id ) )
        {
            result = pencil_layouter_private_get_classifier_id_at_pos( this_,
                                                                       x,
                                                                       y,
                                                                       out_selected_id,
                                                                       out_surrounding_id
                                                                     );
        }

        /* fallback: return the diagram */
        if ( ! data_id_pair_is_valid( out_selected_id ) )
        {
            data_id_pair_reinit_by_table_and_id ( out_selected_id,
                                                  DATA_TABLE_DIAGRAM,
                                                  data_diagram_get_row_id(diagram_data),
                                                  DATA_TABLE_VOID,
                                                  DATA_ROW_ID_VOID
                                                );
        }
        if ( ! data_id_pair_is_valid( out_surrounding_id ) )
        {
            data_id_pair_reinit_by_table_and_id ( out_surrounding_id,
                                                  DATA_TABLE_DIAGRAM,
                                                  data_diagram_get_row_id(diagram_data),
                                                  DATA_TABLE_VOID,
                                                  DATA_ROW_ID_VOID
                                                );
        }
    }
    else
    {
        TRACE_INFO( "given location outside diagram or no diagram chosen" );
        result = PENCIL_ERROR_OUT_OF_BOUNDS;
    }

    TRACE_END_ERR( result );
    return result;
}

pencil_error_t pencil_layouter_private_get_classifier_id_at_pos ( const pencil_layouter_t *this_,
                                                                  double x,
                                                                  double y,
                                                                  data_id_pair_t* out_selected_id,
                                                                  data_id_pair_t* out_surrounding_id )
{
    TRACE_BEGIN();
    assert( NULL != out_selected_id );
    assert( NULL != out_surrounding_id );

    pencil_error_t result = PENCIL_ERROR_OUT_OF_BOUNDS;

    /* get draw area */
    const layout_diagram_t *the_diagram;
    the_diagram = pencil_layout_data_get_diagram_const( &((*this_).layout_data) );
    const geometry_rectangle_t *diagram_draw_area;
    diagram_draw_area = layout_diagram_get_draw_area_const( the_diagram );

    if ( geometry_rectangle_contains( diagram_draw_area, x, y ) )
    {
        /* iterate over all classifiers */
        uint32_t count;
        count = pencil_layout_data_get_visible_classifier_count ( &((*this_).layout_data) );
        double surrounding_classifier_area = geometry_rectangle_get_area( diagram_draw_area );

        for ( uint32_t index = 0; index < count; index ++ )
        {
            const layout_visible_classifier_t *visible_classifier;
            visible_classifier = pencil_layout_data_get_visible_classifier_const ( &((*this_).layout_data), index );
            const geometry_rectangle_t *const classifier_symbol_box
                = layout_visible_classifier_get_symbol_box_const ( visible_classifier );
            const geometry_rectangle_t *const classifier_space
                = layout_visible_classifier_get_space_const ( visible_classifier );

            if ( geometry_rectangle_contains( classifier_symbol_box, x, y ) )
            {
                if ( geometry_rectangle_contains( classifier_space, x, y ) )
                {
                    /* surrounding classifier is found. select it if it is the smallest found area */
                    double current_classifier_area = geometry_rectangle_get_area( classifier_space );
                    if ( current_classifier_area < surrounding_classifier_area )
                    {
                        surrounding_classifier_area = current_classifier_area;
                        data_id_pair_reinit_by_table_and_id ( out_surrounding_id,
                                                              DATA_TABLE_DIAGRAMELEMENT,
                                                              layout_visible_classifier_get_diagramelement_id( visible_classifier ),
                                                              DATA_TABLE_CLASSIFIER,
                                                              layout_visible_classifier_get_classifier_id( visible_classifier )
                                                            );
                    }
                }
                else
                {
                    /* classifier is found */
                    data_id_pair_reinit_by_table_and_id ( out_selected_id,
                                                          DATA_TABLE_DIAGRAMELEMENT,
                                                          layout_visible_classifier_get_diagramelement_id( visible_classifier ),
                                                          DATA_TABLE_CLASSIFIER,
                                                          layout_visible_classifier_get_classifier_id( visible_classifier )
                                                        );

                    result = PENCIL_ERROR_NONE;
                }
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

pencil_error_t pencil_layouter_private_get_feature_id_at_pos ( const pencil_layouter_t *this_,
                                                               double x,
                                                               double y,
                                                               pencil_type_filter_t filter,
                                                               data_id_pair_t* out_selected_id,
                                                               data_id_pair_t* out_surrounding_id )
{
    TRACE_BEGIN();
    assert( NULL != out_selected_id );
    assert( NULL != out_surrounding_id );

    pencil_error_t result = PENCIL_ERROR_OUT_OF_BOUNDS;

    /* check all contained features */
    const uint32_t f_count = pencil_layout_data_get_feature_count( &((*this_).layout_data) );
    for ( uint32_t f_idx = 0; f_idx < f_count; f_idx ++ )
    {
        const layout_feature_t *const the_feature
            = pencil_layout_data_get_feature_const ( &((*this_).layout_data), f_idx );
        const geometry_rectangle_t *const feature_symbol_box
            = layout_feature_get_symbol_box_const ( the_feature );

        if ( geometry_rectangle_contains( feature_symbol_box, x, y ) )
        {
            /* feature is found */
            const data_feature_t *const data_feature
                = layout_feature_get_data_const ( the_feature );
            const layout_visible_classifier_t *const layout_classifier
                = layout_feature_get_classifier_const ( the_feature );
            if (( PENCIL_TYPE_FILTER_LIFELINE == filter )
                &&( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type( data_feature ) ))
            {
                data_id_pair_reinit_by_table_and_id ( out_selected_id,
                                                      DATA_TABLE_DIAGRAMELEMENT,
                                                      layout_visible_classifier_get_diagramelement_id( layout_classifier ),
                                                      DATA_TABLE_CLASSIFIER,
                                                      layout_visible_classifier_get_classifier_id( layout_classifier )
                                                    );
            }
            else
            {
                data_id_pair_reinit_by_table_and_id ( out_selected_id,
                                                      DATA_TABLE_FEATURE,
                                                      layout_feature_get_feature_id( the_feature ),
                                                      DATA_TABLE_CLASSIFIER,
                                                      data_feature_get_classifier_row_id( data_feature )
                                                    );
            }

            data_id_pair_reinit_by_table_and_id ( out_surrounding_id,
                                                  DATA_TABLE_DIAGRAMELEMENT,
                                                  layout_visible_classifier_get_diagramelement_id( layout_classifier ),
                                                  DATA_TABLE_CLASSIFIER,
                                                  layout_visible_classifier_get_classifier_id( layout_classifier )
                                                );

            result = PENCIL_ERROR_NONE;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

pencil_error_t pencil_layouter_private_get_relationship_id_at_pos ( const pencil_layouter_t *this_,
                                                                    double x,
                                                                    double y,
                                                                    double snap_distance,
                                                                    data_id_pair_t* out_selected_id )
{
    TRACE_BEGIN();
    assert( NULL != out_selected_id );

    pencil_error_t result = PENCIL_ERROR_OUT_OF_BOUNDS;

    uint32_t count_relations;
    count_relations = pencil_layout_data_get_relationship_count ( &((*this_).layout_data) );
    uint32_t matching_relations_found = 0;
    for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
    {
        const layout_relationship_t *the_relationship;
        the_relationship = pencil_layout_data_get_relationship_const( &((*this_).layout_data), rel_index );
        const geometry_connector_t *relationship_shape;
        relationship_shape = layout_relationship_get_shape_const( the_relationship );

        if ( geometry_connector_is_close( relationship_shape, x, y, snap_distance ) )
        {
            /* ensure that every relation at that location can be selected by small mouse movements */
            if ( ((uint32_t)(x+y))%(matching_relations_found+1) == 0 )
            {
                const layout_relationship_t *current_relation;
                current_relation = pencil_layout_data_get_relationship_const ( &((*this_).layout_data), rel_index );
                const data_relationship_t *relation_data;
                relation_data = layout_relationship_get_data_const( current_relation );

                data_id_pair_reinit_by_table_and_id ( out_selected_id,
                                                      DATA_TABLE_RELATIONSHIP,
                                                      data_relationship_get_row_id( relation_data ),
                                                      DATA_TABLE_VOID,
                                                      DATA_ROW_ID_VOID
                                                    );
                result = PENCIL_ERROR_NONE;
            }
            matching_relations_found ++;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

pencil_error_t pencil_layouter_get_classifier_order_at_pos ( const pencil_layouter_t *this_,
                                                             double x,
                                                             double y,
                                                             double snap_distance,
                                                             layout_order_t* out_layout_order )
{
    TRACE_BEGIN();
    assert ( NULL != out_layout_order );

    pencil_error_t result = PENCIL_ERROR_NONE;

    /* get the bounding box of the diagram */
    const layout_diagram_t *the_diagram;
    the_diagram = pencil_layout_data_get_diagram_const( &((*this_).layout_data) );
    const geometry_rectangle_t *diagram_bounds;
    diagram_bounds = layout_diagram_get_bounds_const( the_diagram );
    const geometry_rectangle_t *diagram_draw_area;
    diagram_draw_area = layout_diagram_get_draw_area_const( the_diagram );

    /* get the diagram type */
    const data_diagram_t *diagram_data;
    diagram_data = layout_diagram_get_data_const ( the_diagram );
    data_diagram_type_t diag_type;
    diag_type = data_diagram_get_diagram_type ( diagram_data );

    if ( ! geometry_rectangle_contains( diagram_bounds, x, y ) )
    {
        layout_order_init_empty( out_layout_order );
        result = PENCIL_ERROR_OUT_OF_BOUNDS;
    }
    else
    {
        if ( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM == diag_type )
        {
            /* classifiers are a horizontal list */
            double draw_left = geometry_rectangle_get_left(diagram_draw_area);
            double draw_right = geometry_rectangle_get_right(diagram_draw_area);
            int32_t list_order;
            if ( x <= draw_left )
            {
                list_order = INT32_MIN;
            }
            else if ( x >= draw_right )
            {
                list_order = INT32_MAX;
            }
            else
            {
                list_order = ((uint32_t)(( x - draw_left ) / ( draw_right - draw_left ) * UINT32_MAX));
                list_order += INT32_MIN;
            }
            layout_order_init_list( out_layout_order, list_order );
        }
        else if (( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == diag_type )
            || ( DATA_DIAGRAM_TYPE_LIST == diag_type ))
        {
            /* classifiers are a vertical list */
            double draw_top = geometry_rectangle_get_top(diagram_draw_area);
            double draw_bottom = geometry_rectangle_get_bottom(diagram_draw_area);
            int32_t list_order;
            if ( y <= draw_top )
            {
                list_order = INT32_MIN;
            }
            else if ( y >= draw_bottom )
            {
                list_order = INT32_MAX;
            }
            else
            {
                list_order = ((uint32_t)(( y - draw_top ) / ( draw_bottom - draw_top ) * UINT32_MAX));
                list_order += INT32_MIN;
            }
            layout_order_init_list( out_layout_order, list_order );
        }
        else
        {
            /* classifiers are x/y arranged */
            int32_t x_order;
            int32_t y_order;
            x_order = geometry_non_linear_scale_get_order( &((*this_).x_scale), x, snap_distance );
            y_order = geometry_non_linear_scale_get_order( &((*this_).y_scale), y, snap_distance );
            layout_order_init_x_y( out_layout_order, x_order, y_order );
        }
    }

    TRACE_END_ERR( result );
    return result;
}

pencil_error_t pencil_layouter_get_feature_order_at_pos ( const pencil_layouter_t *this_,
                                                          const data_feature_t *feature_ptr,
                                                          double x,
                                                          double y,
                                                          layout_order_t* out_layout_order )
{
    TRACE_BEGIN();
    assert ( NULL != out_layout_order );
    assert ( NULL != feature_ptr );

    pencil_error_t result = PENCIL_ERROR_NONE;

    /* get data of feature */
    data_row_id_t parent_classifier_id;
    data_feature_type_t feature_type;
    feature_type = data_feature_get_main_type ( feature_ptr );
    parent_classifier_id = data_feature_get_classifier_row_id ( feature_ptr );

    /* get the bounding box of the diagram */
    const layout_diagram_t *the_diagram;
    the_diagram = pencil_layout_data_get_diagram_const( &((*this_).layout_data) );
    const geometry_rectangle_t *diagram_bounds;
    diagram_bounds = layout_diagram_get_bounds_const( the_diagram );

    if ( ! geometry_rectangle_contains( diagram_bounds, x, y ) )
    {
        layout_order_init_empty( out_layout_order );
        result = PENCIL_ERROR_OUT_OF_BOUNDS;
    }
    else if ( DATA_ROW_ID_VOID == parent_classifier_id ) {
        TSLOG_WARNING( "feature to move has no parent classifier!" );
        layout_order_init_empty( out_layout_order );
        result = PENCIL_ERROR_UNKNOWN_OBJECT;
    }
    else
    {
        /* iterate over all classifiers, search the closest_parent_instance */
        const layout_visible_classifier_t *closest_parent_instance = NULL;
        uint32_t classfy_count;
        classfy_count = pencil_layout_data_get_visible_classifier_count ( &((*this_).layout_data) );
        for ( uint32_t classfy_index = 0; classfy_index < classfy_count; classfy_index ++ )
        {
            const layout_visible_classifier_t *visible_classifier;
            visible_classifier = pencil_layout_data_get_visible_classifier_const ( &((*this_).layout_data), classfy_index );
            data_row_id_t classfy_id;
            classfy_id = layout_visible_classifier_get_classifier_id ( visible_classifier );
            if ( parent_classifier_id == classfy_id )
            {
                if ( NULL == closest_parent_instance )
                {
                    closest_parent_instance = visible_classifier;
                }
                else
                {
                    const geometry_rectangle_t *const classfier_symbol_box
                        = layout_visible_classifier_get_symbol_box_const ( visible_classifier );
                    const geometry_rectangle_t *const closest_parent_symbol_box
                        = layout_visible_classifier_get_symbol_box_const ( closest_parent_instance );
                    const double classfy_distance = geometry_rectangle_calc_chess_distance( classfier_symbol_box, x, y );
                    double closest_parent_distance = geometry_rectangle_calc_chess_distance( closest_parent_symbol_box, x, y );
                    if ( classfy_distance < closest_parent_distance )
                    {
                        closest_parent_instance = visible_classifier;
                    }
                }
            }
        }

        if ( NULL != closest_parent_instance )
        {
            switch (feature_type)
            {
                case DATA_FEATURE_TYPE_PROPERTY:  /* or */
                case DATA_FEATURE_TYPE_OPERATION:
                {
                    int32_t max_order_above = INT32_MIN;
                    int32_t min_order_below = INT32_MAX;
                    /* iterate over all contained features */
                    const uint32_t f_count = pencil_layout_data_get_feature_count( &((*this_).layout_data) );
                    for ( uint32_t f_idx = 0; f_idx < f_count; f_idx ++ )
                    {
                        /* check if feature belongs to same parent classifier */
                        const layout_feature_t *const the_feature
                            = pencil_layout_data_get_feature_const ( &((*this_).layout_data), f_idx );
                        const layout_visible_classifier_t *const vis_classfy
                            = layout_feature_get_classifier_const ( the_feature );
                        if ( closest_parent_instance == vis_classfy )
                        {
                            /* check if feature is not the moved one */
                            const data_feature_t *const data_feature
                                = layout_feature_get_data_const ( the_feature );
                            if ( data_feature_get_row_id ( feature_ptr ) != data_feature_get_row_id ( data_feature ) )
                            {
                                const int32_t list_order
                                    = data_feature_get_list_order( data_feature );
                                const geometry_rectangle_t *const feature_symbol_box
                                    = layout_feature_get_symbol_box_const ( the_feature );
                                if ( y < geometry_rectangle_get_center_y( feature_symbol_box ) )
                                {
                                    if ( list_order < min_order_below ) { min_order_below = list_order; }
                                }
                                else
                                {
                                    if ( list_order > max_order_above ) { max_order_above = list_order; }
                                }
                            }
                        }
                    }

                    if ( max_order_above == INT32_MIN )
                    {
                        if ( min_order_below == INT32_MAX )
                        {
                            /* nothing above, nothing below */
                            layout_order_init_list( out_layout_order, 0 );
                        }
                        else
                        {
                            /* nothing above */
                            layout_order_init_list( out_layout_order, min_order_below - 32768 );
                        }
                    }
                    else
                    {
                        if ( min_order_below == INT32_MAX )
                        {
                            /* nothing below */
                            layout_order_init_list( out_layout_order, max_order_above + 32768 );
                        }
                        else
                        {
                            /* regular interval */
                            layout_order_init_list( out_layout_order, (max_order_above + min_order_below)/2 );
                        }
                    }
                }
                break;

                case DATA_FEATURE_TYPE_PORT:  /* or */
                case DATA_FEATURE_TYPE_PROVIDED_INTERFACE:  /* or */
                case DATA_FEATURE_TYPE_REQUIRED_INTERFACE:
                {
                    const geometry_rectangle_t *const closest_parent_symbol_box
                        = layout_visible_classifier_get_symbol_box_const ( closest_parent_instance );
                    const double center_x = geometry_rectangle_get_center_x( closest_parent_symbol_box );
                    const double center_y = geometry_rectangle_get_center_y( closest_parent_symbol_box );
                    const double width = geometry_rectangle_get_width( closest_parent_symbol_box );
                    const double height = geometry_rectangle_get_height( closest_parent_symbol_box );
                    const double delta_x = x - center_x;
                    const double delty_y = y - center_y;
                    const double relative_delta_x = delta_x * height;
                    const double relative_delta_y = delty_y * width;
                    const double distance_x = ( x < center_x ) ? (center_x - x) : (x - center_x);
                    const double distance_y = ( y < center_y ) ? (center_y - y) : (y - center_y);
                    const double relative_dist_x = distance_x * height;
                    const double relative_dist_y = distance_y * width;
                    int32_t order;
                    if ( relative_dist_x > relative_dist_y )
                    {
                        if ( x < center_x )
                        {
                            /* x,y is on left side, order is between 0 and INT32_MAX/2 */
                            order = INT32_MAX*(relative_delta_y/(relative_dist_x+0.1)*0.25+0.25);
                        }
                        else
                        {
                            /* x,y is on right side, order is between INT32_MIN and INT32_MIN/2 */
                            order = INT32_MIN*(relative_delta_y/(relative_dist_x+0.1)*0.25+0.75);
                        }
                    }
                    else
                    {
                        if ( y < center_y )
                        {
                            /* x,y is on upper side, order is between INT32_MIN/2 and 0 */
                            order = INT32_MIN*(relative_delta_x/(relative_dist_y+0.1)*0.25+0.25);
                        }
                        else
                        {
                            /* x,y is on lower side, order is between INT32_MAX/2 and INT32_MAX */
                            order = INT32_MAX*(relative_delta_x/(relative_dist_y+0.1)*0.25+0.75);
                        }
                    }
                    layout_order_init_list( out_layout_order, order );
                }
                break;

                case DATA_FEATURE_TYPE_LIFELINE:
                {
                    TRACE_INFO( "feature to move is a lifeline and therefore cannot move." );
                    layout_order_init_empty( out_layout_order );
                    result = PENCIL_ERROR_UNKNOWN_OBJECT;
                }
                break;

                default:
                {
                    TSLOG_WARNING( "feature to move has illegal/unknown type!" );
                    layout_order_init_empty( out_layout_order );
                    result = PENCIL_ERROR_UNKNOWN_OBJECT;
                }
                break;
            }
        }
        else
        {
            TSLOG_WARNING( "parent classifier of feature is not visible; possibly array size too small?" );
            layout_order_init_empty( out_layout_order );
            result = PENCIL_ERROR_UNKNOWN_OBJECT;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

pencil_error_t pencil_layouter_get_relationship_order_at_pos ( const pencil_layouter_t *this_,
                                                               data_row_id_t relationship_id,
                                                               double x,
                                                               double y,
                                                               layout_order_t* out_layout_order )
{
    TRACE_BEGIN();
    assert ( NULL != out_layout_order );

    pencil_error_t result = PENCIL_ERROR_NONE;

    /* get the bounding box of the diagram */
    const layout_diagram_t *the_diagram;
    the_diagram = pencil_layout_data_get_diagram_const( &((*this_).layout_data) );
    const geometry_rectangle_t *diagram_bounds;
    diagram_bounds = layout_diagram_get_bounds_const( the_diagram );
    const geometry_rectangle_t *diagram_draw_area;
    diagram_draw_area = layout_diagram_get_draw_area_const( the_diagram );

    /* get the diagram type */
    const data_diagram_t *diagram_data;
    diagram_data = layout_diagram_get_data_const ( the_diagram );
    data_diagram_type_t diag_type;
    diag_type = data_diagram_get_diagram_type ( diagram_data );

    if ( ! geometry_rectangle_contains( diagram_bounds, x, y ) )
    {
        layout_order_init_empty( out_layout_order );
        result = PENCIL_ERROR_OUT_OF_BOUNDS;
    }
    else
    {
        if (( DATA_DIAGRAM_TYPE_BOX_DIAGRAM == diag_type )
            || ( DATA_DIAGRAM_TYPE_LIST == diag_type ))
        {
            /* relationships are hidden in lists and box-diagrams */
            layout_order_init_empty( out_layout_order );
            result = PENCIL_ERROR_OUT_OF_BOUNDS;
        }
        else if (( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM == diag_type )||( DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM == diag_type ))
        {
            /* communication diagrams and interaction overview diagrams do not care about list_orders of relationships */
            layout_order_init_empty( out_layout_order );
        }
        else if ( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM == diag_type )
        {
            double draw_top = geometry_rectangle_get_top(diagram_draw_area);
            double draw_bottom = geometry_rectangle_get_bottom(diagram_draw_area);
            int32_t list_order;
            if ( y <= draw_top )
            {
                list_order = INT32_MIN;
            }
            else if ( y >= draw_bottom )
            {
                list_order = INT32_MAX;
            }
            else
            {
                list_order = ((uint32_t)(( y - draw_top ) / ( draw_bottom - draw_top ) * UINT32_MAX));
                list_order += INT32_MIN;
            }
            layout_order_init_list( out_layout_order, list_order );
        }
        else if ( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == diag_type )
        {
            double draw_left = geometry_rectangle_get_left(diagram_draw_area);
            double draw_right = geometry_rectangle_get_right(diagram_draw_area);
            int32_t list_order;
            if ( x <= draw_left )
            {
                list_order = INT32_MIN;
            }
            else if ( x >= draw_right )
            {
                list_order = INT32_MAX;
            }
            else
            {
                list_order = ((uint32_t)(( x - draw_left ) / ( draw_right - draw_left ) * UINT32_MAX));
                list_order += INT32_MIN;
            }
            layout_order_init_list( out_layout_order, list_order );
        }
        else
        {
            /* all other diagram types do not care about list_orders of relationships */
            layout_order_init_empty( out_layout_order );
        }
    }

    TRACE_END_ERR( result );
    return result;
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
