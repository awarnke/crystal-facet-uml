/* File: gui_sketch_card.inl; Copyright and License: see below */

#include "tslog.h"

static inline void gui_sketch_card_load_data( gui_sketch_card_t *this_, int64_t diagram_id, data_database_reader_t *db_reader )
{
    /* load data to be drawn */
    pencil_input_data_load( &((*this_).painter_input_data), diagram_id, db_reader );
}

static inline void gui_sketch_card_invalidate_data( gui_sketch_card_t *this_ )
{
    pencil_input_data_invalidate( &((*this_).painter_input_data) );
}

static inline bool gui_sketch_card_is_valid( gui_sketch_card_t *this_ )
{
    return pencil_input_data_is_valid( &((*this_).painter_input_data) );
}

static inline shape_int_rectangle_t gui_sketch_card_get_bounds( gui_sketch_card_t *this_ )
{
    return (*this_).bounds;
}

static inline void gui_sketch_card_set_bounds( gui_sketch_card_t *this_, shape_int_rectangle_t bounds )
{
    (*this_).bounds = bounds;
}

static inline bool gui_sketch_card_is_visible( gui_sketch_card_t *this_ )
{
    return (*this_).visible;
}

static inline void gui_sketch_card_set_visible( gui_sketch_card_t *this_, bool visible )
{
    (*this_).visible = visible;
}

static inline data_diagram_t *gui_sketch_card_get_diagram_ptr ( gui_sketch_card_t *this_ )
{
    return pencil_input_data_get_diagram_ptr( &((*this_).painter_input_data) );
}

static inline data_id_t gui_sketch_card_get_object_id_at_pos ( gui_sketch_card_t *this_, int32_t x, int32_t y, bool dereference )
{
    data_id_t selected_obj_id;
    data_id_t surrounding_obj_id;

    pencil_error_t err;
    err = pencil_diagram_maker_get_object_id_at_pos( &((*this_).painter),
                                                     (double) x,
                                                     (double) y,
                                                     dereference,
                                                     &selected_obj_id,
                                                     &surrounding_obj_id
                                                   );

    return selected_obj_id;
}

static inline universal_int32_pair_t gui_sketch_card_get_order_at_pos ( gui_sketch_card_t *this_, int32_t x, int32_t y )
{
    universal_int32_pair_t result;
    int32_t x_order;
    int32_t y_order;

    pencil_error_t err;
    err = pencil_diagram_maker_get_order_at_pos( &((*this_).painter),
                                                 (double) x,
                                                 (double) y,
                                                 &x_order,
                                                 &y_order
                                               );

    universal_int32_pair_init( &result, x_order, y_order );
    return result;
}

static inline universal_bool_list_t gui_sketch_card_is_pos_on_grid ( gui_sketch_card_t *this_, int32_t x, int32_t y )
{
    universal_bool_list_t result;
    bool x_on_grid;
    bool y_on_grid;

    pencil_diagram_maker_is_pos_on_grid( &((*this_).painter), (double) x, (double) y, &x_on_grid, &y_on_grid );

    universal_bool_list_init_pair( &result, x_on_grid, y_on_grid );
    return result;
}

static inline void gui_sketch_card_move_classifier_to_order ( gui_sketch_card_t *this_, int32_t row_id, int32_t x_order, int32_t y_order )
{
    data_classifier_t *move_me;
    move_me = pencil_input_data_get_classifier_ptr( &((*this_).painter_input_data), row_id );
    if ( move_me == NULL )
    {
        TSLOG_WARNING_INT( "pencil input data does not contain the CLASSIFIER to be moved", row_id );
    }
    else
    {
        data_classifier_set_x_order( move_me, x_order );
        data_classifier_set_y_order( move_me, y_order );

        (*this_).dirty_elements_layout = true;
    }
}

static inline void gui_sketch_card_do_layout( gui_sketch_card_t *this_, cairo_t *cr )
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
    pencil_diagram_maker_layout_grid ( &((*this_).painter),
                                         &((*this_).painter_input_data),
                                         destination
                                       );
    pencil_diagram_maker_layout_elements ( &((*this_).painter), cr );
    (*this_).dirty_elements_layout = false;

    geometry_rectangle_destroy( &destination );
}

static inline int32_t gui_sketch_card_get_highest_list_order( gui_sketch_card_t *this_ )
{
    int32_t result = 0;

    if ( pencil_input_data_is_valid( &((*this_).painter_input_data) ) )
    {
        for ( uint32_t f_idx = 0; f_idx < pencil_input_data_get_feature_count( &((*this_).painter_input_data) ); f_idx ++ )
        {
            data_feature_t *feat;
            feat = pencil_input_data_get_feature_ptr ( &((*this_).painter_input_data), f_idx );
            if ( data_feature_get_list_order( feat ) > result )
            {
                result = data_feature_get_list_order( feat );
            }
        }
        for ( uint32_t rs_idx = 0; rs_idx < pencil_input_data_get_relationship_count( &((*this_).painter_input_data) ); rs_idx ++ )
        {
            data_relationship_t *relation;
            relation = pencil_input_data_get_relationship_ptr ( &((*this_).painter_input_data), rs_idx );
            if ( data_relationship_get_list_order( relation ) > result )
            {
                result = data_relationship_get_list_order( relation );
            }
        }
    }

    return result;
}


/*
Copyright 2016-2017 Andreas Warnke

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
