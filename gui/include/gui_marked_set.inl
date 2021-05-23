/* File: gui_marked_set.inl; Copyright and License: see below */

#include <assert.h>

static inline void gui_marked_set_init ( gui_marked_set_t *this_ )
{
    data_id_init_void( &((*this_).focused) );
    data_id_init_void( &((*this_).focused_diagram) );
    data_id_init_void( &((*this_).highlighted) );
    data_id_init_void( &((*this_).highlighted_diagram) );
    data_small_set_init( &((*this_).selected_set) );
}

static inline void gui_marked_set_reinit ( gui_marked_set_t *this_ )
{
    data_id_init_void( &((*this_).focused) );
    data_id_init_void( &((*this_).focused_diagram) );
    data_id_init_void( &((*this_).highlighted) );
    data_id_init_void( &((*this_).highlighted_diagram) );
    data_small_set_reinit( &((*this_).selected_set) );
}

static inline void gui_marked_set_destroy ( gui_marked_set_t *this_ )
{
    data_id_destroy( &((*this_).focused) );
    data_id_destroy( &((*this_).focused_diagram) );
    data_id_destroy( &((*this_).highlighted) );
    data_id_destroy( &((*this_).highlighted_diagram) );
    data_small_set_destroy( &((*this_).selected_set) );
}

static inline data_id_t gui_marked_set_get_focused ( const gui_marked_set_t *this_ )
{
    return ( (*this_).focused );
}

static inline data_id_t gui_marked_set_get_focused_diagram ( const gui_marked_set_t *this_ )
{
    return( (*this_).focused_diagram );
}

static inline data_id_t gui_marked_set_get_highlighted ( const gui_marked_set_t *this_ )
{
    return( (*this_).highlighted );
}

static inline data_id_t gui_marked_set_get_highlighted_diagram ( const gui_marked_set_t *this_ )
{
    return( (*this_).highlighted_diagram );
}

static inline const data_small_set_t *gui_marked_set_get_selected_set_const ( const gui_marked_set_t *this_ )
{
    return ( &((*this_).selected_set) );
}

static inline void gui_marked_set_toggle_selected_obj ( gui_marked_set_t *this_, data_id_t obj_id )
{
    data_small_set_toggle_obj( &((*this_).selected_set), obj_id );
}

static inline void gui_marked_set_clear_selected_set ( gui_marked_set_t *this_ )
{
    data_small_set_clear( &((*this_).selected_set) );
}

static inline void gui_marked_set_set_focused ( gui_marked_set_t *this_,
                                                data_id_t obj_id,
                                                data_id_t diagram_id  )
{
    assert(( data_id_get_table(&diagram_id) == DATA_TABLE_DIAGRAM )||( data_id_get_table(&diagram_id) == DATA_TABLE_VOID ));
    data_id_replace( &((*this_).focused), &obj_id );
    data_id_replace( &((*this_).focused_diagram), &diagram_id );
}

static inline void gui_marked_set_set_focused_diagram ( gui_marked_set_t *this_, data_id_t diag_id )
{
    assert( data_id_get_table(&diag_id) == DATA_TABLE_DIAGRAM );
    data_id_replace( &((*this_).focused), &diag_id );
    data_id_replace( &((*this_).focused_diagram), &diag_id );
}

static inline void gui_marked_set_set_highlighted ( gui_marked_set_t *this_,
                                                    data_id_t obj_id,
                                                    data_id_t diagram_id  )
{
    assert(( data_id_get_table(&diagram_id) == DATA_TABLE_DIAGRAM )||( data_id_get_table(&diagram_id) == DATA_TABLE_VOID ));
    data_id_replace( &((*this_).highlighted), &obj_id );
    data_id_replace( &((*this_).highlighted_diagram), &diagram_id );
}

static inline void gui_marked_set_set_highlighted_diagram ( gui_marked_set_t *this_, data_id_t diag_id )
{
    assert(( data_id_get_table(&diag_id) == DATA_TABLE_DIAGRAM )||( data_id_get_table(&diag_id) == DATA_TABLE_VOID ));
    data_id_replace( &((*this_).highlighted), &diag_id );
    data_id_replace( &((*this_).highlighted_diagram), &diag_id );
}

static inline void gui_marked_set_clear_focused ( gui_marked_set_t *this_ )
{
    data_id_reinit_void( &((*this_).focused) );
    data_id_reinit_void( &((*this_).focused_diagram) );
}

static inline void gui_marked_set_clear_highlighted ( gui_marked_set_t *this_ )
{
    data_id_reinit_void( &((*this_).highlighted) );
    data_id_reinit_void( &((*this_).highlighted_diagram) );
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
