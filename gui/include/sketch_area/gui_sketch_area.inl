/* File: gui_sketch_area.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline data_diagram_t *gui_sketch_area_get_focused_diagram_ptr ( gui_sketch_area_t *this_ )
{
    gui_sketch_card_t *result_card;
    result_card = &((*this_).cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]);
    return gui_sketch_card_get_diagram_ptr( result_card );
}

static inline data_row_id_t gui_sketch_area_get_focused_diagram_id ( gui_sketch_area_t *this_ )
{
    const data_diagram_t *focused_diag = gui_sketch_area_get_focused_diagram_ptr( this_ );
    const data_row_id_t focused_diagram_id = data_diagram_get_row_id( focused_diag );
    return focused_diagram_id;
}

static inline data_id_t gui_sketch_area_get_diagram_id_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    data_id_t result;
    data_id_init_void( &result );

    if ( gui_sketch_nav_tree_is_visible( &((*this_).nav_tree) ) )
    {
        gui_sketch_nav_tree_get_object_id_at_pos( &((*this_).nav_tree), x, y, &result );
    }
    else if ( gui_sketch_result_list_is_visible( &((*this_).result_list) ) )
    {
        gui_sketch_result_list_get_diagram_id_at_pos ( &((*this_).result_list), x, y, &result );
    }

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        const gui_sketch_card_t *card = &((*this_).cards[idx]);
        const shape_int_rectangle_t card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            const data_diagram_t *selected_diag
                = gui_sketch_card_get_diagram_const( card );
            data_id_reinit( &result, DATA_TABLE_DIAGRAM, data_diagram_get_row_id( selected_diag ) );
            break;
        }
    }
    return result;
}

static inline void gui_sketch_area_get_diagram_and_object_id_at_pos ( gui_sketch_area_t *this_, 
                                                                      int32_t x, 
                                                                      int32_t y,
                                                                      data_id_t* out_diagram_id,
                                                                      data_id_t* out_object_id )
{
    assert( NULL != out_diagram_id );
    assert( NULL != out_object_id );
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    data_id_reinit_void( out_diagram_id );
    data_id_reinit_void( out_object_id );

    if ( gui_sketch_nav_tree_is_visible( &((*this_).nav_tree) ) )
    {
        gui_sketch_nav_tree_get_object_id_at_pos( &((*this_).nav_tree), x, y, out_diagram_id );
    }
    else if ( gui_sketch_result_list_is_visible( &((*this_).result_list) ) )
    {
        gui_sketch_result_list_get_object_id_at_pos ( &((*this_).result_list), 
                                                      x,
                                                      y, 
                                                      out_object_id,  /* = out_selected_id */
                                                      out_diagram_id  /* = out_diagram_id */
                                                    );
    }

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        const gui_sketch_card_t *card = &((*this_).cards[idx]);
        const shape_int_rectangle_t card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            const data_diagram_t *selected_diag
                 = gui_sketch_card_get_diagram_const( card );
            data_id_reinit( out_diagram_id, DATA_TABLE_DIAGRAM, data_diagram_get_row_id( selected_diag ) );
            break;
        }
    }
}

static inline void gui_sketch_area_private_get_object_id_at_pos ( gui_sketch_area_t *this_,
                                                                  int32_t x,
                                                                  int32_t y,
                                                                  pencil_type_filter_t filter,
                                                                  data_id_pair_t* out_selected_id )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    assert( NULL != out_selected_id );
    data_id_pair_reinit_void( out_selected_id );

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        const gui_sketch_card_t *card = &((*this_).cards[idx]);
        const shape_int_rectangle_t card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            data_id_pair_t surrounding_id;  /* dummy */
            gui_sketch_card_get_object_id_at_pos ( card, x, y, filter, out_selected_id, &surrounding_id );
            break;
        }
    }
}

static inline void gui_sketch_area_private_get_object_ids_at_pos ( gui_sketch_area_t *this_,
                                                                   int32_t x,
                                                                   int32_t y,
                                                                   pencil_type_filter_t filter,
                                                                   data_id_pair_t* out_selected_id,
                                                                   data_id_pair_t* out_surrounding_id )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    assert( NULL != out_selected_id );
    assert( NULL != out_surrounding_id );
    data_id_pair_init_void( out_selected_id );
    data_id_pair_init_void( out_surrounding_id );

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        const gui_sketch_card_t *card = &((*this_).cards[idx]);
        const shape_int_rectangle_t card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            gui_sketch_card_get_object_id_at_pos ( card, x, y, filter, out_selected_id, out_surrounding_id );
            break;
        }
    }
}

static inline gui_sketch_card_t *gui_sketch_area_get_card_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    gui_sketch_card_t *result = NULL;

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        gui_sketch_card_t *card = &((*this_).cards[idx]);
        const shape_int_rectangle_t card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            result = card;
            break;
        }
    }
    return result;
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
