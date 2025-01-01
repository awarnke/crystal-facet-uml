/* File: gui_sketch_result_list.inl; Copyright and License: see below */

#include "u8/u8_log.h"

static inline void gui_sketch_result_list_load_data( gui_sketch_result_list_t *this_,
                                                     const data_search_result_list_t *result_list,
                                                     data_database_reader_t *db_reader )
{
    assert( result_list != NULL );
    assert( db_reader != NULL );
    gui_sketch_result_list_invalidate_data( this_ );
    data_search_result_list_add_all( &((*this_).result_list), result_list );
}

static inline void gui_sketch_result_list_invalidate_data( gui_sketch_result_list_t *this_ )
{
    data_search_result_list_clear( &((*this_).result_list) );

    /* clear layout infos */
    assert( (*this_).element_count <= GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS );
    for ( int ele_index = 0; ele_index < (*this_).element_count; ele_index ++ )
    {
        pos_search_result_destroy( &((*this_).element_pos[ele_index]) );
    }
    (*this_).element_count = 0;
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

static inline void gui_sketch_result_list_get_object_id_at_pos ( const gui_sketch_result_list_t *this_,
                                                                 int32_t x,
                                                                 int32_t y,
                                                                 data_id_t* out_selected_id,
                                                                 data_id_t* out_diagram_id )
{
    assert( out_selected_id != NULL );
    assert( out_diagram_id != NULL );

    /* default in case no object found */
    {
        data_id_init_void( out_selected_id );
        data_id_init_void( out_diagram_id );
    }

    /* search object */
    if ( shape_int_rectangle_contains( &((*this_).bounds), x, y ) )
    {
        const unsigned int count = (*this_).element_count;
        assert( count <= GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS );
        for ( unsigned int idx = 0; idx < count; idx ++ )
        {
            const pos_search_result_t *const element = &((*this_).element_pos[idx]);
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

static inline const pos_search_result_t *gui_sketch_result_list_get_element_pos_const ( const gui_sketch_result_list_t *this_,
                                                                                        uint32_t index )
{
    assert( index < (*this_).element_count );
    return &((*this_).element_pos[index]);
}

static inline uint32_t gui_sketch_result_list_get_element_count ( const gui_sketch_result_list_t *this_ )
{
    assert( (*this_).element_count <= GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS );
    return (*this_).element_count;
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
