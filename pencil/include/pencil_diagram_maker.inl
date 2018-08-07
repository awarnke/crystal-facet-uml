/* File: pencil_diagram_maker.inl; Copyright and License: see below */

#include "pencil_diagram_maker.h"
#include "trace.h"
#include <assert.h>

static inline void pencil_diagram_maker_init( pencil_diagram_maker_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    pencil_diagram_painter_init( &((*this_).diagram_painter) );
    pencil_classifier_painter_init( &((*this_).classifier_painter) );
    pencil_relationship_painter_init( &((*this_).relationship_painter) );
    pencil_feature_painter_init( &((*this_).feature_painter) );

    pencil_layouter_init( &((*this_).layouter), input_data );

    (*this_).input_data = input_data;

    TRACE_END();
}

static inline void pencil_diagram_maker_destroy( pencil_diagram_maker_t *this_ )
{
    TRACE_BEGIN();

    pencil_diagram_painter_destroy( &((*this_).diagram_painter) );
    pencil_classifier_painter_destroy( &((*this_).classifier_painter) );
    pencil_relationship_painter_destroy( &((*this_).relationship_painter) );
    pencil_feature_painter_destroy( &((*this_).feature_painter) );

    pencil_layouter_destroy( &((*this_).layouter) );

    (*this_).input_data = NULL;

    TRACE_END();
}

static inline void pencil_diagram_maker_layout_grid ( pencil_diagram_maker_t *this_,
                                                      pencil_input_data_t *input_data,
                                                      geometry_rectangle_t diagram_bounds )
{
    pencil_layouter_reinit ( &((*this_).layouter), input_data );
    pencil_layouter_layout_grid ( &((*this_).layouter), diagram_bounds );
}

static inline void pencil_diagram_maker_layout_elements ( pencil_diagram_maker_t *this_, cairo_t *cr )
{
    PangoLayout *font_layout;
    font_layout = pango_cairo_create_layout (cr);

    pencil_layouter_layout_elements ( &((*this_).layouter), font_layout );

    g_object_unref (font_layout);
}

static inline pencil_error_t pencil_diagram_maker_get_object_id_at_pos ( pencil_diagram_maker_t *this_,
                                                                         double x,
                                                                         double y,
                                                                         data_id_pair_t* out_selected_id,
                                                                         data_id_pair_t* out_surrounding_id )
{
    return pencil_layouter_get_object_id_at_pos ( &((*this_).layouter),
                                                  x,
                                                  y,
                                                  3.0,
                                                  out_selected_id,
                                                  out_surrounding_id
                                                );
}

static inline pencil_error_t pencil_diagram_maker_get_order_at_pos ( pencil_diagram_maker_t *this_,
                                                                     data_id_t obj_id,
                                                                     double x,
                                                                     double y,
                                                                     layout_order_t* out_layout_order )
{
    int32_t x_order;
    int32_t y_order;

    pencil_error_t err;
    err = pencil_layouter_get_order_at_pos ( &((*this_).layouter), x, y, 3.15, &x_order, &y_order );

    layout_order_init( out_layout_order, PENCIL_LAYOUT_ORDER_TYPE_X_Y, x_order, y_order );
    return err;
}

static inline void pencil_diagram_maker_is_pos_on_grid ( pencil_diagram_maker_t *this_,
                                                         double x,
                                                         double y,
                                                         bool *out_x_on_grid,
                                                         bool *out_y_on_grid )
{
    pencil_layouter_is_pos_on_grid ( &((*this_).layouter), x, y, 3.13, out_x_on_grid, out_y_on_grid );
}

static inline pencil_error_t pencil_diagram_maker_move_object_to_order ( pencil_diagram_maker_t *this_,
                                                                         data_id_t obj_id,
                                                                         layout_order_t *order )
{
    pencil_error_t result = PENCIL_ERROR_NONE;

    layout_order_type_t order_type = layout_order_get_order_type( order );
    if (( PENCIL_LAYOUT_ORDER_TYPE_NONE != order_type )
        && ( PENCIL_LAYOUT_ORDER_TYPE_OUT_OF_RANGE != order_type ))
    {
        data_table_t table = data_id_get_table ( &obj_id );
        int64_t row_id = data_id_get_row_id ( &obj_id );
        switch ( table )
        {
            case DATA_TABLE_CLASSIFIER:
            {
                int32_t x_order = layout_order_get_first( order );
                int32_t y_order = layout_order_get_second( order );

                data_classifier_t *move_me;
                move_me = pencil_input_data_get_classifier_ptr( (*this_).input_data, row_id );
                if ( move_me == NULL )
                {
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

            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {

            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {

            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
                /* pencil cannot move diagrams */
                result = PENCIL_ERROR_UNKNOWN_OBJECT;
            }
            break;

            default:
            {
                result = PENCIL_ERROR_UNKNOWN_OBJECT;
            }
            break;
        }

    }
    else
    {
        TSLOG_WARNING( "pencil input data does not contain the object to be moved, or out of range" );
        data_id_trace( &obj_id );
        result = PENCIL_ERROR_OUT_OF_BOUNDS;
    }

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
