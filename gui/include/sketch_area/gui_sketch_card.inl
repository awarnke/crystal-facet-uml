/* File: gui_sketch_card.inl; Copyright and License: see below */

#include "tslog.h"
#include "trace.h"

static inline void gui_sketch_card_load_data( gui_sketch_card_t *this_, int64_t diagram_id, data_database_reader_t *db_reader )
{
    /* load data to be drawn */
    data_visible_set_load( &((*this_).painter_input_data), diagram_id, db_reader );
}

static inline void gui_sketch_card_invalidate_data( gui_sketch_card_t *this_ )
{
    data_visible_set_invalidate( &((*this_).painter_input_data) );
}

static inline bool gui_sketch_card_is_valid( const gui_sketch_card_t *this_ )
{
    return data_visible_set_is_valid( &((*this_).painter_input_data) );
}

static inline shape_int_rectangle_t gui_sketch_card_get_bounds( const gui_sketch_card_t *this_ )
{
    return (*this_).bounds;
}

static inline void gui_sketch_card_set_bounds( gui_sketch_card_t *this_, shape_int_rectangle_t bounds )
{
    (*this_).bounds = bounds;
}

static inline bool gui_sketch_card_is_visible( const gui_sketch_card_t *this_ )
{
    return (*this_).visible;
}

static inline void gui_sketch_card_set_visible( gui_sketch_card_t *this_, bool visible )
{
    (*this_).visible = visible;
}

static inline data_diagram_t * gui_sketch_card_get_diagram_ptr ( gui_sketch_card_t *this_ )
{
    return data_visible_set_get_diagram_ptr( &((*this_).painter_input_data) );
}

static inline const data_diagram_t * gui_sketch_card_get_diagram_const ( const gui_sketch_card_t *this_ )
{
    return data_visible_set_get_diagram_const( &((*this_).painter_input_data) );
}

static inline void gui_sketch_card_get_object_id_at_pos ( const gui_sketch_card_t *this_,
                                                          int32_t x,
                                                          int32_t y,
                                                          pencil_type_filter_t filter,
                                                          data_id_pair_t* out_selected_id,
                                                          data_id_pair_t* out_surrounding_id )
{
    pencil_error_t pen_err;
    pen_err = pencil_diagram_maker_get_object_id_at_pos( &((*this_).painter),
                                                         (double) x,
                                                         (double) y,
                                                         filter,
                                                         out_selected_id,
                                                         out_surrounding_id
                                                       );

    switch ( pen_err )
    {
        case PENCIL_ERROR_NONE:
        {
            /* success */
        }
        break;
        case PENCIL_ERROR_OUT_OF_BOUNDS:
        {
            TRACE_INFO( "PENCIL_ERROR_OUT_OF_BOUNDS in gui_sketch_card_get_object_id_at_pos" );
        }
        break;
        case PENCIL_ERROR_UNKNOWN_OBJECT:
        {
            TSLOG_ANOMALY( "PENCIL_ERROR_UNKNOWN_OBJECT in gui_sketch_card_get_object_id_at_pos" );
        }
        break;
    }
}

static inline layout_order_t gui_sketch_card_get_order_at_pos ( const gui_sketch_card_t *this_, data_id_t obj_id, int32_t x, int32_t y )
{
    layout_order_t result;
    pencil_error_t pen_err;

    pen_err = pencil_diagram_maker_get_order_at_pos ( &((*this_).painter),
                                                      obj_id,
                                                      (double) x,
                                                      (double) y,
                                                      &result
                                                    );

    switch ( pen_err )
    {
        case PENCIL_ERROR_NONE:
        {
            /* success */
        }
        break;
        case PENCIL_ERROR_OUT_OF_BOUNDS:
        {
            TRACE_INFO( "PENCIL_ERROR_OUT_OF_BOUNDS in gui_sketch_card_get_order_at_pos" );
        }
        break;
        case PENCIL_ERROR_UNKNOWN_OBJECT:
        {
            TSLOG_ANOMALY( "PENCIL_ERROR_UNKNOWN_OBJECT in gui_sketch_card_get_order_at_pos" );
        }
        break;
    }

    return result;
}

static inline int32_t gui_sketch_card_get_feature_order_at_pos ( const gui_sketch_card_t *this_,
                                                                 const data_feature_t *feature_ptr,
                                                                 int32_t x,
                                                                 int32_t y )
{
    assert ( NULL != feature_ptr );

    layout_order_t result;
    pencil_error_t pen_err;

    pen_err = pencil_diagram_maker_get_feature_order_at_pos ( &((*this_).painter),
                                                              feature_ptr,
                                                              (double) x,
                                                              (double) y,
                                                              &result
                                                            );

    switch ( pen_err )
    {
        case PENCIL_ERROR_NONE:
        {
            /* success */
        }
        break;
        case PENCIL_ERROR_OUT_OF_BOUNDS:
        {
            TRACE_INFO( "PENCIL_ERROR_OUT_OF_BOUNDS in gui_sketch_card_get_feature_order_at_pos" );
        }
        break;
        case PENCIL_ERROR_UNKNOWN_OBJECT:
        {
            TSLOG_ANOMALY( "PENCIL_ERROR_UNKNOWN_OBJECT in gui_sketch_card_get_feature_order_at_pos" );
        }
        break;
    }

    return layout_order_get_first( &result );
}

static inline universal_bool_list_t gui_sketch_card_is_pos_on_grid ( const gui_sketch_card_t *this_, int32_t x, int32_t y )
{
    universal_bool_list_t result;
    bool x_on_grid;
    bool y_on_grid;

    pencil_diagram_maker_is_pos_on_grid( &((*this_).painter), (double) x, (double) y, &x_on_grid, &y_on_grid );

    universal_bool_list_init_pair( &result, x_on_grid, y_on_grid );
    return result;
}

static inline void gui_sketch_card_do_layout( gui_sketch_card_t *this_, cairo_t *cr )
{
    if ( gui_sketch_card_is_valid( this_ ) )
    {
        /* layout loaded classifiers */
        int32_t left;
        int32_t top;
        uint32_t width;
        uint32_t height;

        left = shape_int_rectangle_get_left( &((*this_).bounds) );
        top = shape_int_rectangle_get_top( &((*this_).bounds) );
        width = shape_int_rectangle_get_width( &((*this_).bounds) );
        height = shape_int_rectangle_get_height( &((*this_).bounds) );

        geometry_rectangle_t destination;
        geometry_rectangle_init( &destination, left, top, width, height );
        pencil_diagram_maker_define_grid ( &((*this_).painter),
                                           destination
                                         );
        pencil_diagram_maker_layout_elements ( &((*this_).painter), cr );
        (*this_).dirty_elements_layout = false;

        geometry_rectangle_destroy( &destination );
    }
    else
    {
        TRACE_INFO( "gui_sketch_card_do_layout called on invalid card." );
    }
}

static inline int32_t gui_sketch_card_get_highest_list_order( const gui_sketch_card_t *this_ )
{
    int32_t result = 0;

    if ( data_visible_set_is_valid( &((*this_).painter_input_data) ) )
    {
        for ( uint32_t f_idx = 0; f_idx < data_visible_set_get_feature_count( &((*this_).painter_input_data) ); f_idx ++ )
        {
            const data_feature_t *feat;
            feat = data_visible_set_get_feature_const ( &((*this_).painter_input_data), f_idx );
            if ( data_feature_get_list_order( feat ) > result )
            {
                result = data_feature_get_list_order( feat );
            }
        }
        for ( uint32_t rs_idx = 0; rs_idx < data_visible_set_get_relationship_count( &((*this_).painter_input_data) ); rs_idx ++ )
        {
            const data_relationship_t *relation;
            relation = data_visible_set_get_relationship_const ( &((*this_).painter_input_data), rs_idx );
            if ( data_relationship_get_list_order( relation ) > result )
            {
                result = data_relationship_get_list_order( relation );
            }
        }
    }

    return result;
}


/*
Copyright 2016-2019 Andreas Warnke

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
