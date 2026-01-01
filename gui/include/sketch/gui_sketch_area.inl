/* File: gui_sketch_area.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

static inline data_diagram_t *gui_sketch_area_private_get_focused_diagram_ptr ( gui_sketch_area_t *this_ )
{
    gui_sketch_card_t *result_card;
    result_card = &((*this_).cards[GUI_SKETCH_AREA_CONST_FOCUSED_CARD]);
    return gui_sketch_card_get_diagram_ptr( result_card );
}

static inline data_row_t gui_sketch_area_private_get_focused_diagram_id ( gui_sketch_area_t *this_ )
{
    const data_diagram_t *focused_diag = gui_sketch_area_private_get_focused_diagram_ptr( this_ );
    const data_row_t focused_diagram_id = data_diagram_get_row_id( focused_diag );
    return focused_diagram_id;
}

static inline void gui_sketch_area_private_get_diagram_and_object_id_at_pos ( gui_sketch_area_t *this_,
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
        data_id_replace( out_object_id, out_diagram_id );
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
            data_id_replace( out_object_id, out_diagram_id );
            break;
        }
    }
}

static inline void gui_sketch_area_private_get_element_id_at_pos ( gui_sketch_area_t *this_,
                                                                   int32_t x,
                                                                   int32_t y,
                                                                   bool filter_lifelines,
                                                                   data_full_id_t* out_element_id,
                                                                   data_id_t* out_diagram_id )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    assert( NULL != out_element_id );
    assert( NULL != out_diagram_id );
    data_full_id_reinit_void( out_element_id );
    data_id_reinit_void( out_diagram_id );

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        const gui_sketch_card_t *const card = &((*this_).cards[idx]);
        const shape_int_rectangle_t card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            *out_diagram_id = gui_sketch_card_get_diagram_id( card );
            const layout_subelement_id_t subelement
                = gui_sketch_card_get_element_at_pos( card, x, y, filter_lifelines );
            if ( LAYOUT_SUBELEMENT_KIND_SPACE == layout_subelement_id_get_kind( &subelement ) )
            {
                /* do not report space areas, in EDIT mode these are of no concern, */
                /* in CREATE mode during dragging, these are also of no concern. */
            }
            else
            {
                data_full_id_replace( out_element_id, layout_subelement_id_get_id_const( &subelement ) );
            }
            break;
        }
    }
}

static inline gui_sketch_card_t *gui_sketch_area_private_get_card_at_pos ( gui_sketch_area_t *this_, int32_t x, int32_t y )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    gui_sketch_card_t *result = NULL;

    for ( int idx = 0; idx < (*this_).card_num; idx ++ )
    {
        gui_sketch_card_t *const card = &((*this_).cards[idx]);
        const shape_int_rectangle_t card_bounds = gui_sketch_card_get_bounds( card );
        if ( shape_int_rectangle_contains( &card_bounds, x, y ) )
        {
            result = card;
            break;
        }
    }
    return result;
}

static inline u8_error_t gui_sketch_area_private_get_card_of_id ( gui_sketch_area_t *this_, const data_id_t* diagram_id, const gui_sketch_card_t** out_card_ptr )
{
    assert( (*this_).card_num <= GUI_SKETCH_AREA_CONST_MAX_CARDS );
    assert( diagram_id != NULL );
    assert( out_card_ptr != NULL );
    u8_error_t result = U8_ERROR_NOT_FOUND;

    for ( int idx = 0; ( idx < (*this_).card_num )&&( result != U8_ERROR_NONE ); idx ++ )
    {
        const gui_sketch_card_t *const card = &((*this_).cards[idx]);
        const data_id_t diag_id = gui_sketch_card_get_diagram_id ( card );
        if ( data_id_equals( &diag_id, diagram_id ) )
        {
            *out_card_ptr = card;
            result = U8_ERROR_NONE;
        }
    }

    return result;
}


/*
Copyright 2016-2026 Andreas Warnke

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
