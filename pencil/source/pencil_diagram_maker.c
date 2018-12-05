/* File: pencil_diagram_maker.c; Copyright and License: see below */

#include "pencil_diagram_maker.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_diagram_maker_draw ( pencil_diagram_maker_t *this_,
                                 data_id_t mark_focused,
                                 data_id_t mark_highlighted,
                                 data_small_set_t *mark_selected,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    PangoLayout *layout;
    layout = pango_cairo_create_layout (cr);

    /* get layout data object */
    pencil_layout_data_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    /* get diagram bounds */
    layout_diagram_t *diagram_layout;
    diagram_layout = pencil_layout_data_get_diagram_ptr( layout_data );
    geometry_rectangle_t *diagram_bounds = layout_diagram_get_bounds_ptr ( diagram_layout );
    double width = geometry_rectangle_get_width ( diagram_bounds );
    double height = geometry_rectangle_get_height ( diagram_bounds );

    pencil_size_t *pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );

    /* draw diagram */
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );
    pencil_diagram_painter_draw ( &((*this_).diagram_painter),
                                  diagram_layout,
                                  data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ),
                                  pencil_size,
                                  layout,
                                  cr
                                );

    if (( width > 20.0 ) && ( height > 20.0 ))
    {
        /* draw all contained classifiers */
        pencil_diagram_maker_private_draw_classifiers ( this_,
                                                        mark_focused,
                                                        mark_highlighted,
                                                        mark_selected,
                                                        layout,
                                                        cr
                                                      );

        /* draw all contained features */
        pencil_diagram_maker_private_draw_features ( this_,
                                                     mark_focused,
                                                     mark_highlighted,
                                                     mark_selected,
                                                     layout,
                                                     cr
                                                   );

        /* draw all contained relationships */
        pencil_diagram_maker_private_draw_relationships ( this_,
                                                          mark_focused,
                                                          mark_highlighted,
                                                          mark_selected,
                                                          layout,
                                                          cr
                                                        );
    }

    g_object_unref (layout);

    TRACE_END();
}

void pencil_diagram_maker_private_draw_classifiers ( pencil_diagram_maker_t *this_,
                                                     data_id_t mark_focused,
                                                     data_id_t mark_highlighted,
                                                     data_small_set_t *mark_selected,
                                                     PangoLayout *layout,
                                                     cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    pencil_layout_data_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_layout_data_get_classifier_count ( layout_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        layout_visible_classifier_t *classifier_layout;
        classifier_layout = pencil_layout_data_get_classifier_ptr( layout_data, index );
        /*
        const data_visible_classifier_t *visible_classifier;
        visible_classifier = layout_visible_classifier_get_data_ptr ( classifier_layout );

        const data_classifier_t *classifier;
        const data_diagramelement_t *diagramelement;
        classifier = data_visible_classifier_get_classifier_const( visible_classifier );
        diagramelement = data_visible_classifier_get_diagramelement_const( visible_classifier );

        data_diagramelement_flag_t display_flags;
        display_flags = data_diagramelement_get_display_flags( diagramelement );
        */

        pencil_size_t *pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );

        pencil_classifier_painter_draw( &((*this_).classifier_painter),
                                        classifier_layout,
                                        mark_focused,
                                        mark_highlighted,
                                        mark_selected,
                                        pencil_size,
                                        layout,
                                        cr
                                        );
    }

    TRACE_END();
}

void pencil_diagram_maker_private_draw_features ( pencil_diagram_maker_t *this_,
                                                  data_id_t mark_focused,
                                                  data_id_t mark_highlighted,
                                                  data_small_set_t *mark_selected,
                                                  PangoLayout *layout,
                                                  cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    pencil_layout_data_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    /* iterate over all features */
    uint32_t count;
    count = pencil_layout_data_get_feature_count ( layout_data );
    for ( uint32_t f_idx = 0; f_idx < count; f_idx ++ )
    {
        /* get feature */
        layout_feature_t *the_feature;
        the_feature = pencil_layout_data_get_feature_ptr( layout_data, f_idx );

        /* determine display flags of diagramelement */
        layout_visible_classifier_t *classifier_layout;
        classifier_layout = layout_feature_get_classifier_ptr ( the_feature );
        const data_diagramelement_t *diagramelement;
        diagramelement = layout_visible_classifier_get_diagramelement_ptr( classifier_layout );
        data_diagramelement_flag_t display_flags;
        display_flags = data_diagramelement_get_display_flags( diagramelement );

        /* draw features */
        pencil_feature_painter_draw ( &((*this_).feature_painter),
                                      the_feature,
                                      data_id_equals_id( &mark_focused, DATA_TABLE_FEATURE, layout_feature_get_feature_id(the_feature) ),
                                      data_id_equals_id( &mark_highlighted, DATA_TABLE_FEATURE, layout_feature_get_feature_id( the_feature ) ),
                                      data_small_set_contains_row_id( mark_selected, DATA_TABLE_FEATURE, layout_feature_get_feature_id(the_feature) ),
                                      (0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_GREY_OUT )),
                                      pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) ),
                                      layout,
                                      cr
                                    );
    }

    TRACE_END();
}

void pencil_diagram_maker_private_draw_relationships ( pencil_diagram_maker_t *this_,
                                                       data_id_t mark_focused,
                                                       data_id_t mark_highlighted,
                                                       data_small_set_t *mark_selected,
                                                       PangoLayout *layout,
                                                       cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != mark_selected );
    assert( NULL != cr );

    pencil_layout_data_t *layout_data;
    layout_data = pencil_layouter_get_layout_data_ptr ( &((*this_).layouter) );

    uint32_t rel_count;
    rel_count = pencil_layout_data_get_relationship_count ( layout_data );
    for ( uint32_t index = 0; index < rel_count; index ++ )
    {
        pencil_visibility_t show_relation;
        const data_relationship_t *the_relationship;
        layout_relationship_t *relationship_layout;
        relationship_layout = pencil_layout_data_get_relationship_ptr ( layout_data, index );
        the_relationship = layout_relationship_get_data_ptr ( relationship_layout );
        show_relation = layout_relationship_get_visibility ( relationship_layout );
        if ( PENCIL_VISIBILITY_IMPLICIT == show_relation )
        {
            if ( data_id_equals_id( &mark_focused, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) )
                || data_id_equals_id( &mark_highlighted, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) )
                || data_small_set_contains_row_id( mark_selected, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) ) )
            {
                /* the implicit relationship is focused or marked or highlighted */
                show_relation = PENCIL_VISIBILITY_SHOW;
            }
            else
            {
                if ( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &mark_highlighted ) )
                {
                    int64_t diagramelement_id = data_id_get_row_id( &mark_highlighted );
                    data_visible_classifier_t *visible_clsfy;
                    visible_clsfy = pencil_input_data_get_visible_classifier_by_id_ptr ( (*this_).input_data, diagramelement_id );
                    if ( visible_clsfy != NULL )
                    {
                        if ( data_visible_classifier_is_valid( visible_clsfy ) )
                        {
                            data_classifier_t *classifier;
                            classifier = data_visible_classifier_get_classifier_ptr( visible_clsfy );
                            if (( data_classifier_get_id( classifier ) == data_relationship_get_from_classifier_id( the_relationship ) )
                                ||( data_classifier_get_id( classifier ) == data_relationship_get_to_classifier_id( the_relationship ) ))
                            {
                                /* the implicit relationship has highlighted from or to classifier */
                                show_relation = PENCIL_VISIBILITY_SHOW;
                            }
                        }
                    }

                }
            }
        }
        if ( PENCIL_VISIBILITY_SHOW == show_relation )
        {
            pencil_size_t *pencil_size = pencil_layouter_get_pencil_size_ptr( &((*this_).layouter) );
            pencil_relationship_painter_draw ( &((*this_).relationship_painter),
                                               relationship_layout,
                                               data_id_equals_id( &mark_focused, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) ),
                                               data_id_equals_id( &mark_highlighted, DATA_TABLE_RELATIONSHIP, data_relationship_get_id( the_relationship ) ),
                                               data_small_set_contains_row_id( mark_selected, DATA_TABLE_RELATIONSHIP, data_relationship_get_id(the_relationship) ),
                                               pencil_size,
                                               layout,
                                               cr
                                             );
        }
    }

    TRACE_END();
}

static const double snap_to_grid_distance_for_dropping = 3.15;  /* plus/minus three pixels shall snap to grid */
                                                                /* this is the expected accuracy for mouse input devices */
                                                                /* this value is bigger than snap_to_grid_distance_for_drag_marker */
                                                                /* to ensure object really snaps when marked so */

pencil_error_t pencil_diagram_maker_get_order_at_pos ( pencil_diagram_maker_t *this_,
                                                       data_id_t obj_id,
                                                       double x,
                                                       double y,
                                                       layout_order_t* out_layout_order )
{
    TRACE_BEGIN();
    assert( NULL != out_layout_order );

    pencil_error_t result = PENCIL_ERROR_NONE;

    data_table_t table = data_id_get_table ( &obj_id );
    switch ( table )
    {
        case DATA_TABLE_CLASSIFIER:
        {
            result = pencil_layouter_get_classifier_order_at_pos ( &((*this_).layouter),
                                                                   x,
                                                                   y,
                                                                   snap_to_grid_distance_for_dropping,
                                                                   out_layout_order
                                                                 );
        }
        break;

        case DATA_TABLE_FEATURE:
        {
            int64_t feature_id = data_id_get_row_id ( &obj_id );
            data_feature_t *the_feature;
            the_feature = pencil_input_data_get_feature_by_id_ptr ( (*this_).input_data, feature_id );
            if( NULL != the_feature )
            {
                result = pencil_layouter_get_feature_order_at_pos ( &((*this_).layouter),
                                                                    the_feature,
                                                                    x,
                                                                    y,
                                                                    out_layout_order
                                                                  );
            }
            else
            {
                TSLOG_ANOMALY( "feature to move does not exist in input_data or has no classifier in input_data." );
                layout_order_init_empty( out_layout_order );
                result = PENCIL_ERROR_UNKNOWN_OBJECT;
            }
        }
        break;

        case DATA_TABLE_RELATIONSHIP:
        {
            int64_t relationship_id = data_id_get_row_id ( &obj_id );
            result = pencil_layouter_get_relationship_order_at_pos ( &((*this_).layouter),
                                                                     relationship_id,
                                                                     x,
                                                                     y,
                                                                     out_layout_order
                                                                   );
        }
        break;

        case DATA_TABLE_DIAGRAMELEMENT:
        {
            TSLOG_WARNING( "not implemented to move diagramelements. use the classifier instead." );
            layout_order_init_empty( out_layout_order );
            result = PENCIL_ERROR_UNKNOWN_OBJECT;
        }
        break;

        case DATA_TABLE_DIAGRAM:
        {
            /* pencil cannot move diagrams */
            TSLOG_WARNING( "object to be re-positioned has unexpected type: diagram" );
            layout_order_init_empty( out_layout_order );
            result = PENCIL_ERROR_UNKNOWN_OBJECT;
        }
        break;

        default:
        {
            TSLOG_WARNING( "object to be re-positioned has illegal type" );
            layout_order_init_empty( out_layout_order );
            result = PENCIL_ERROR_UNKNOWN_OBJECT;
        }
        break;
    }

    TRACE_END_ERR(result);
    return result;
}

pencil_error_t pencil_diagram_maker_move_object_to_order ( pencil_diagram_maker_t *this_,
                                                           data_id_t obj_id,
                                                           layout_order_t *order )
{
    TRACE_BEGIN();
    assert( NULL != order );

    pencil_error_t result = PENCIL_ERROR_NONE;

    data_table_t table = data_id_get_table ( &obj_id );
    int64_t row_id = data_id_get_row_id ( &obj_id );

    layout_order_type_t order_type = layout_order_get_order_type( order );
    switch ( order_type )
    {
        case PENCIL_LAYOUT_ORDER_TYPE_X_Y:
        {
            switch ( table )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    int32_t x_order = layout_order_get_first( order );
                    int32_t y_order = layout_order_get_second( order );

                    data_classifier_t *move_me;
                    move_me = pencil_input_data_get_classifier_by_id_ptr( (*this_).input_data, row_id );
                    if ( move_me == NULL )
                    {
                        TSLOG_WARNING( "pencil input data does not contain the object to be moved" );
                        data_id_trace( &obj_id );
                        result = PENCIL_ERROR_OUT_OF_BOUNDS;
                    }
                    else
                    {
                        data_classifier_set_x_order( move_me, x_order );
                        data_classifier_set_y_order( move_me, y_order );
                    }
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    TSLOG_WARNING( "object to be x/y-moved has no x/y coordinates: feature" );
                    result = PENCIL_ERROR_OUT_OF_BOUNDS;
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    TSLOG_WARNING( "object to be x/y-moved has no x/y coordinates: relationship" );
                    result = PENCIL_ERROR_OUT_OF_BOUNDS;
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    TSLOG_WARNING( "not implemented to move diagramelements. use the classifier instead." );
                    result = PENCIL_ERROR_UNKNOWN_OBJECT;
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* pencil cannot move diagrams */
                    TSLOG_WARNING( "object to be x/y-moved has unexpected type: diagram" );
                    result = PENCIL_ERROR_UNKNOWN_OBJECT;
                }
                break;

                default:
                {
                    TSLOG_WARNING( "object to be x/y-moved has illegal type" );
                    result = PENCIL_ERROR_UNKNOWN_OBJECT;
                }
                break;
            }
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_LIST:
        {
            switch ( table )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    int32_t list_order = layout_order_get_first( order );

                    data_classifier_t *move_me;
                    move_me = pencil_input_data_get_classifier_by_id_ptr( (*this_).input_data, row_id );
                    if ( move_me == NULL )
                    {
                        TSLOG_WARNING( "pencil input data does not contain the classifier to be moved" );
                        data_id_trace( &obj_id );
                        result = PENCIL_ERROR_OUT_OF_BOUNDS;
                    }
                    else
                    {
                        data_classifier_set_list_order( move_me, list_order );
                    }
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    int32_t list_order = layout_order_get_first( order );

                    data_feature_t *move_me;
                    move_me = pencil_input_data_get_feature_by_id_ptr( (*this_).input_data, row_id );
                    if ( move_me == NULL )
                    {
                        TSLOG_WARNING( "pencil input data does not contain the feature to be moved" );
                        data_id_trace( &obj_id );
                        result = PENCIL_ERROR_OUT_OF_BOUNDS;
                    }
                    else
                    {
                        data_feature_set_list_order( move_me, list_order );
                    }
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    int32_t list_order = layout_order_get_first( order );

                    data_relationship_t *move_me;
                    move_me = pencil_input_data_get_relationship_by_id_ptr( (*this_).input_data, row_id );
                    if ( move_me == NULL )
                    {
                        TSLOG_WARNING( "pencil input data does not contain the relationship to be moved" );
                        data_id_trace( &obj_id );
                        result = PENCIL_ERROR_OUT_OF_BOUNDS;
                    }
                    else
                    {
                        data_relationship_set_list_order( move_me, list_order );
                    }
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    TSLOG_WARNING( "not implemented to move diagramelements. use the classifier instead." );
                    result = PENCIL_ERROR_UNKNOWN_OBJECT;
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* pencil cannot move diagrams */
                    TSLOG_WARNING( "object to be x/y-moved has unexpected type: diagram" );
                    result = PENCIL_ERROR_UNKNOWN_OBJECT;
                }
                break;

                default:
                {
                    TSLOG_WARNING( "object to be x/y-moved has illegal type" );
                    result = PENCIL_ERROR_UNKNOWN_OBJECT;
                }
                break;
            }
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_NONE:
        {
            /* nothing to do */
            /* no error */
            TSLOG_ANOMALY( "object to be moved has no movement data" );
        }
        break;

        case PENCIL_LAYOUT_ORDER_TYPE_OUT_OF_RANGE:
        default:
        {
            TSLOG_WARNING( "object to be x/y-moved has illegal movement data" );
            result = PENCIL_ERROR_OUT_OF_BOUNDS;
        }
        break;
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2018 Andreas Warnke

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
