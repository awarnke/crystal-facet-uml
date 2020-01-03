/* File: gui_marked_set.inl; Copyright and License: see below */

static inline void gui_marked_set_init ( gui_marked_set_t *this_ )
{
    data_id_init_void( &((*this_).focused) );
    data_id_init_void( &((*this_).highlighted) );
    data_small_set_init( &((*this_).selected_set) );
    (*this_).focused_diagram = DATA_ID_VOID_ID;
}

static inline void gui_marked_set_destroy ( gui_marked_set_t *this_ )
{
    data_id_destroy( &((*this_).focused) );
    data_id_destroy( &((*this_).highlighted) );
    data_small_set_destroy( &((*this_).selected_set) );
    (*this_).focused_diagram = DATA_ID_VOID_ID;
}

static inline data_id_t gui_marked_set_get_focused ( const gui_marked_set_t *this_ )
{
    return ( (*this_).focused );
}

static inline int64_t gui_marked_set_get_focused_diagram ( const gui_marked_set_t *this_ )
{
    return( (*this_).focused_diagram );
}

static inline data_id_t gui_marked_set_get_highlighted ( const gui_marked_set_t *this_ )
{
    return( (*this_).highlighted );
}

static inline const data_small_set_t *gui_marked_set_get_selected_set_ptr ( gui_marked_set_t *this_ )
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

static inline void gui_marked_set_set_focused ( gui_marked_set_t *this_, data_id_t obj_id )
{
    data_id_destroy( &((*this_).focused) );
    (*this_).focused = obj_id;
}

static inline void gui_marked_set_set_focused_diagram ( gui_marked_set_t *this_, int64_t diag_id )
{
    (*this_).focused_diagram = diag_id;
}

static inline void gui_marked_set_set_highlighted ( gui_marked_set_t *this_, data_id_t obj_id )
{
    data_id_destroy( &((*this_).highlighted) );
    (*this_).highlighted = obj_id;
}

static inline void gui_marked_set_clear_focused ( gui_marked_set_t *this_ )
{
    data_id_reinit_void( &((*this_).focused) );
}

static inline void gui_marked_set_clear_highlighted ( gui_marked_set_t *this_ )
{
    data_id_reinit_void( &((*this_).highlighted) );
}


/*
Copyright 2016-2020 Andreas Warnke

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
