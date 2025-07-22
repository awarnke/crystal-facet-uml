/* File: gui_sketch_result_list.inl; Copyright and License: see below */

#include "u8/u8_log.h"

static inline void gui_sketch_result_list_load_data( gui_sketch_result_list_t *this_,
                                                     const pos_scroll_page_t *requested_page,
                                                     uint32_t result_buffer_start,
                                                     const data_search_result_list_t *result_list,
                                                     data_database_reader_t *db_reader )
{
    assert( requested_page != NULL );
    assert( result_list != NULL );
    assert( db_reader != NULL );

    /* reset page */
    pos_search_result_page_reinit( &((*this_).page), result_buffer_start );
    (*this_).requested_page = *requested_page;

    /* fill page */
    u8_error_t err = U8_ERROR_NONE;
    const uint_fast32_t length = data_search_result_list_get_length( result_list );
    for ( uint_fast32_t index = 0; index < length; index ++ )
    {
        data_search_result_t const *const src_res = data_search_result_list_get_const( result_list, index );
        err |= pos_search_result_page_add_search_result( &((*this_).page), src_res );
    }
    if ( err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR( "Not all search results taken over by gui_sketch_result_list_t widget." );
    }
}

static inline void gui_sketch_result_list_invalidate_data( gui_sketch_result_list_t *this_ )
{
    /* reset page */
    pos_search_result_page_reinit( &((*this_).page), 0 /* buffer_start */ );
}

static inline shape_int_rectangle_t gui_sketch_result_list_get_bounds( gui_sketch_result_list_t *this_ )
{
    return (*this_).bounds;
}

static inline void gui_sketch_result_list_set_bounds( gui_sketch_result_list_t *this_, shape_int_rectangle_t bounds )
{
    (*this_).bounds = bounds;
}

static inline bool gui_sketch_result_list_is_visible( gui_sketch_result_list_t *this_ )
{
    return (*this_).visible;
}

static inline void gui_sketch_result_list_set_visible( gui_sketch_result_list_t *this_, bool visible )
{
    (*this_).visible = visible;
}

static inline void gui_sketch_result_list_get_button_at_pos( const gui_sketch_result_list_t *this_,
                                                             int32_t x,
                                                             int32_t y,
                                                             gui_sketch_action_t *out_action_id )
{
    assert ( NULL != out_action_id );

    const shape_int_rectangle_t *const prev_box
        = pos_search_result_page_get_button_prev_box_const( &((*this_).page) );
    const shape_int_rectangle_t *const next_box
       = pos_search_result_page_get_button_next_box_const( &((*this_).page) );

    if ( shape_int_rectangle_contains( prev_box, x, y ) && pos_search_result_page_has_prev_page( &((*this_).page) ) )
    {
        *out_action_id = GUI_SKETCH_ACTION_PREVIOUS_PAGE;
    }
    else if ( shape_int_rectangle_contains( next_box, x, y ) && pos_search_result_page_has_next_page( &((*this_).page) ) )
    {
        *out_action_id = GUI_SKETCH_ACTION_NEXT_PAGE;
    }
    else
    {
        *out_action_id = GUI_SKETCH_ACTION_NONE;
    }
}

static inline void gui_sketch_result_list_get_object_id_at_pos( const gui_sketch_result_list_t *this_,
                                                                int32_t x,
                                                                int32_t y,
                                                                data_id_t* out_selected_id,
                                                                data_id_t* out_diagram_id )
{
    assert( out_selected_id != NULL );
    assert( out_diagram_id != NULL );

    /* default in case no object found */
    data_id_init_void( out_selected_id );
    data_id_init_void( out_diagram_id );

    /* search object */
    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        const uint_fast32_t page_start = pos_search_result_page_get_page_start( &((*this_).page) );
        const uint_fast32_t page_length = pos_search_result_page_get_page_length( &((*this_).page) );
        assert( page_length <= POS_SEARCH_RESULT_PAGE_MAX_PAGE_SIZE );
        for ( uint_fast32_t idx = 0; idx < page_length; idx ++ )
        {
            const pos_search_result_t *const element = pos_search_result_page_get_search_result_layout_const( &((*this_).page), page_start + idx );
            const shape_int_rectangle_t *icon_box = pos_search_result_get_icon_box_const( element );
            const shape_int_rectangle_t *label_box = pos_search_result_get_label_box_const( element );

            if ( shape_int_rectangle_contains( icon_box, x, y ) || shape_int_rectangle_contains( label_box, x, y ) )
            {
                const data_search_result_t *const at_pos = pos_search_result_get_data_const( element );
                data_id_copy( out_selected_id, data_search_result_get_match_id_const( at_pos ) );
                data_id_copy( out_diagram_id, data_search_result_get_diagram_id_const( at_pos ) );
                break;
            }
        }
    }
}


/*
Copyright 2018-2025 Andreas Warnke

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
