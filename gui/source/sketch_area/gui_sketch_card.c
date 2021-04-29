/* File: gui_sketch_card.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_card.h"
#include "pencil_diagram_maker.h"
#include "util/geometry/geometry_rectangle.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>

void gui_sketch_card_init( gui_sketch_card_t *this_ )
{
    TRACE_BEGIN();

    (*this_).visible = false;
    (*this_).dirty_elements_layout = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );
    data_visible_set_init( &((*this_).painter_input_data) );
    pencil_diagram_maker_init( &((*this_).painter), &((*this_).painter_input_data) );

    TRACE_END();
}

void gui_sketch_card_destroy( gui_sketch_card_t *this_ )
{
    TRACE_BEGIN();

    pencil_diagram_maker_destroy( &((*this_).painter) );
    data_visible_set_destroy( &((*this_).painter_input_data) );
    shape_int_rectangle_destroy(&((*this_).bounds));

    TRACE_END();
}

void gui_sketch_card_draw ( gui_sketch_card_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    if ( (*this_).visible )
    {
        const int32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
        const int32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
        const uint32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
        const uint32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

        /* get marked ids */
        const data_id_t mark_focused = gui_marked_set_get_focused( marker );
        const data_id_t mark_highlighted = gui_marked_set_get_highlighted( marker );
        const data_small_set_t *mark_selected_set = gui_marked_set_get_selected_set_ptr( marker );
        
        /* layout elements if necessary */
        if ( (*this_).dirty_elements_layout )
        {
            pencil_diagram_maker_layout_elements ( &((*this_).painter), cr, NULL );
            (*this_).dirty_elements_layout = false;
        }

        /* draw paper */
        const data_diagram_t *const diag = data_visible_set_get_diagram_const ( &((*this_).painter_input_data) );
        const data_id_t diag_id = data_diagram_get_data_id( diag );
        if ( data_id_equals( &diag_id, &mark_highlighted ) )
        {
            const double LINE_W = 1.0;
            cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );
            cairo_rectangle ( cr, left-2, top-1, LINE_W, height+2 );
            cairo_rectangle ( cr, left-2, top-2, width+4, LINE_W );
            cairo_rectangle ( cr, left+width+1, top-1, LINE_W, height+2 );
            cairo_rectangle ( cr, left-2, top+height+1, width+4, LINE_W );
            cairo_fill (cr);
        }
        cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );
        cairo_rectangle ( cr, left, top, width, height );
        cairo_fill (cr);

        /* draw the current diagram */
        pencil_diagram_maker_draw ( &((*this_).painter),
                                    mark_focused,
                                    mark_highlighted,
                                    mark_selected_set,
                                    cr
                                  );
    }

    TRACE_END();
}

void gui_sketch_card_move_object_to_order ( gui_sketch_card_t *this_,
                                            data_id_t obj_id,
                                            const layout_order_t *order )
{
    TRACE_BEGIN();
    assert( NULL != order );

    const data_table_t table = data_id_get_table ( &obj_id );
    const data_row_id_t row_id = data_id_get_row_id ( &obj_id );

    layout_order_type_t order_type = layout_order_get_order_type( order );
    switch ( order_type )
    {
        case PENCIL_LAYOUT_ORDER_TYPE_X_Y:
        {
            switch ( table )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    const int32_t x_order = layout_order_get_first( order );
                    const int32_t y_order = layout_order_get_second( order );

                    data_classifier_t *const move_me = data_visible_set_get_classifier_by_id_ptr( &((*this_).painter_input_data), row_id );
                    if ( move_me == NULL )
                    {
                        TSLOG_WARNING( "pencil input data does not contain the object to be moved" );
                        data_id_trace( &obj_id );
                    }
                    else
                    {
                        data_classifier_set_x_order( move_me, x_order );
                        data_classifier_set_y_order( move_me, y_order );

                        /* success */
                        (*this_).dirty_elements_layout = true;
                    }
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    TSLOG_WARNING( "object to be x/y-moved has no x/y coordinates: feature" );
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    TSLOG_WARNING( "object to be x/y-moved has no x/y coordinates: relationship" );
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    TSLOG_WARNING( "not implemented to move diagramelements. use the classifier instead." );
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* pencil cannot move diagrams */
                    TSLOG_WARNING( "object to be x/y-moved has unexpected type: diagram" );
                }
                break;

                default:
                {
                    TSLOG_WARNING( "object to be x/y-moved has illegal type" );
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
                    const int32_t list_order = layout_order_get_first( order );

                    data_classifier_t *const move_me = data_visible_set_get_classifier_by_id_ptr( &((*this_).painter_input_data), row_id );
                    if ( move_me == NULL )
                    {
                        TSLOG_WARNING( "pencil input data does not contain the classifier to be moved" );
                        data_id_trace( &obj_id );
                    }
                    else
                    {
                        data_classifier_set_list_order( move_me, list_order );

                        /* success */
                        (*this_).dirty_elements_layout = true;
                    }
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    const int32_t list_order = layout_order_get_first( order );

                    data_feature_t *const move_me = data_visible_set_get_feature_by_id_ptr( &((*this_).painter_input_data), row_id );
                    if ( move_me == NULL )
                    {
                        TSLOG_WARNING( "pencil input data does not contain the feature to be moved" );
                        data_id_trace( &obj_id );
                    }
                    else
                    {
                        data_feature_set_list_order( move_me, list_order );

                        /* success */
                        (*this_).dirty_elements_layout = true;
                    }
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    const int32_t list_order = layout_order_get_first( order );

                    data_relationship_t *const move_me = data_visible_set_get_relationship_by_id_ptr( &((*this_).painter_input_data), row_id );
                    if ( move_me == NULL )
                    {
                        TSLOG_WARNING( "pencil input data does not contain the relationship to be moved" );
                        data_id_trace( &obj_id );
                    }
                    else
                    {
                        data_relationship_set_list_order( move_me, list_order );

                        /* success */
                        (*this_).dirty_elements_layout = true;
                    }
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    TSLOG_WARNING( "not implemented to move diagramelements. use the classifier instead." );
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* pencil cannot move diagrams */
                    TSLOG_WARNING( "object to be x/y-moved has unexpected type: diagram" );
                }
                break;

                default:
                {
                    TSLOG_WARNING( "object to be x/y-moved has illegal type" );
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
        }
        break;
    }

    TRACE_END();
}


/*
Copyright 2016-2021 Andreas Warnke

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

