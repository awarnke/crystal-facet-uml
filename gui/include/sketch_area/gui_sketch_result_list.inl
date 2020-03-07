/* File: gui_sketch_result_list.inl; Copyright and License: see below */

#include "tslog.h"

static inline void gui_sketch_result_list_load_data( gui_sketch_result_list_t *this_,
                                                     const data_search_result_list_t *result_list,
                                                     data_database_reader_t *db_reader )
{
    assert( result_list != NULL );
    assert( db_reader != NULL );
    data_search_result_list_clear( &((*this_).result_list) );
    data_search_result_list_add_all( &((*this_).result_list), result_list );
}

static inline void gui_sketch_result_list_invalidate_data( gui_sketch_result_list_t *this_ )
{
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

static inline void gui_sketch_result_list_get_object_id_at_pos ( gui_sketch_result_list_t *this_,
                                                          int32_t x,
                                                          int32_t y,
                                                          data_id_t* out_selected_id )
{
    TSLOG_ERROR("Not yet implemented: gui_sketch_result_list_get_object_id_at_pos");
}


/*
Copyright 2018-2020 Andreas Warnke

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
