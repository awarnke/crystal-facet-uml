/* File: gui_sketch_area.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline data_diagram_t *gui_sketch_area_get_selected_diagram_ptr ( gui_sketch_area_t *this_ )
{
    gui_sketch_card_t *result_card;
    result_card = &((*this_).cards[GUI_SKETCH_AREA_CONST_SELECTED_CARD]);
    return gui_sketch_card_get_diagram_ptr( result_card );
}

static inline int64_t gui_sketch_area_get_selected_diagram_id ( gui_sketch_area_t *this_ )
{
    int64_t selected_diagram_id;
    data_diagram_t *selected_diag;
    selected_diag = gui_sketch_area_get_selected_diagram_ptr( this_ );
    selected_diagram_id = data_diagram_get_id( selected_diag );
    return selected_diagram_id;
}

static inline void gui_sketch_area_set_listener ( gui_sketch_area_t *this_, unsigned int index, GObject *listener )
{
    if ( index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS )
    {
        (*this_).listener[index] = listener;
    }
}

static inline void gui_sketch_area_remove_listener ( gui_sketch_area_t *this_, unsigned int index )
{
    if ( index < GUI_SKETCH_AREA_CONST_MAX_LISTENERS )
    {
        (*this_).listener[index] = NULL;
    }
}

static inline data_id_t gui_sketch_area_get_diagram_id_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    data_id_t result;
    data_id_init_void( &result );

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        gui_sketch_card_t *card;
        card = &((*this_).cards[idx]);
        shape_int_rectangle_t card_bounds;
        card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            data_diagram_t *selected_diag;
            selected_diag = gui_sketch_card_get_diagram_ptr( card );
            data_id_reinit( &result, DATA_TABLE_DIAGRAM, data_diagram_get_id( selected_diag ) );
        }
    }
    return result;
}

static inline void gui_sketch_area_get_object_id_at_pos ( gui_sketch_area_t *this_,
                                                          int32_t x,
                                                          int32_t y,
                                                          pencil_visible_object_id_t* out_selected_id,
                                                          pencil_visible_object_id_t* out_surrounding_id )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    assert( NULL != out_selected_id );
    assert( NULL != out_surrounding_id );
    pencil_visible_object_id_init_void( out_selected_id );
    pencil_visible_object_id_init_void( out_surrounding_id );

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        gui_sketch_card_t *card;
        card = &((*this_).cards[idx]);
        shape_int_rectangle_t card_bounds;
        card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            gui_sketch_card_get_object_id_at_pos ( card, x, y, out_selected_id, out_surrounding_id );
            if ( ! pencil_visible_object_id_is_valid( out_selected_id ) )
            {
                data_diagram_t *selected_diag;
                selected_diag = gui_sketch_card_get_diagram_ptr( card );
                pencil_visible_object_id_reinit_by_table_and_id( out_selected_id,
                                                                 DATA_TABLE_DIAGRAM,
                                                                 data_diagram_get_id( selected_diag ),
                                                                 DATA_TABLE_DIAGRAM,
                                                                 data_diagram_get_id( selected_diag )
                                                               );
            }
        }
    }
}

static inline gui_sketch_card_t *gui_sketch_area_get_card_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    gui_sketch_card_t *result = NULL;

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        gui_sketch_card_t *card;
        card = &((*this_).cards[idx]);
        shape_int_rectangle_t card_bounds;
        card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            result = card;
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
