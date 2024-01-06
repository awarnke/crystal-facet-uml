/* File: gui_marked_set.inl; Copyright and License: see below */

#include <assert.h>

static inline data_id_t gui_marked_set_get_focused_obj ( const gui_marked_set_t *this_ )
{
    return data_full_id_get_primary_id( &((*this_).focused) );
}

static inline data_full_id_t gui_marked_set_get_focused ( const gui_marked_set_t *this_ )
{
    return (*this_).focused;
}

static inline data_id_t gui_marked_set_get_focused_diagram ( const gui_marked_set_t *this_ )
{
    return( (*this_).focused_diagram );
}

static inline bool gui_marked_set_has_focus ( const gui_marked_set_t *this_ )
{
    return data_full_id_is_valid( &((*this_).focused) );
}

static inline data_id_t gui_marked_set_get_highlighted ( const gui_marked_set_t *this_ )
{
    return( (*this_).highlighted );
}

static inline data_id_t gui_marked_set_get_highlighted_diagram ( const gui_marked_set_t *this_ )
{
    return( (*this_).highlighted_diagram );
}

static inline gui_sketch_action_t gui_marked_set_get_highlighted_button ( const gui_marked_set_t *this_ )
{
    return( (*this_).highlighted_button );
}

static inline const data_small_set_t *gui_marked_set_get_selected_set_const ( const gui_marked_set_t *this_ )
{
    return ( &((*this_).selected_set) );
}

static inline void gui_marked_set_toggle_selected_obj ( gui_marked_set_t *this_, data_id_t obj_id )
{
    data_small_set_toggle_obj( &((*this_).selected_set), obj_id );
}

static inline void gui_marked_set_toggle_obj ( gui_marked_set_t *this_, data_full_id_t obj_id, data_id_t diagram_id )
{
    assert(( data_id_get_table(&diagram_id) == DATA_TABLE_DIAGRAM )||( data_id_get_table(&diagram_id) == DATA_TABLE_VOID ));
    const data_id_t *const vis_id = data_full_id_get_primary_id_ptr( &obj_id );

    data_id_t model_id;
    if ( data_full_id_equals( &obj_id, &((*this_).focused) ) )
    {
        /* clear focus: */
        data_full_id_reinit_void( &((*this_).focused) );
        data_id_replace( &((*this_).focused_diagram), &diagram_id );
        /* remove from selection: */
        data_small_set_delete_obj( &((*this_).selected_set), *vis_id );
        model_id = DATA_ID_VOID;
    }
    else
    {
        /* set focus: */
        data_full_id_replace( &((*this_).focused), &obj_id );
        data_id_replace( &((*this_).focused_diagram), &diagram_id );
        /* add to selection: */
        data_small_set_add_obj( &((*this_).selected_set), *vis_id );
        model_id
            = (DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( vis_id ))
            ? data_full_id_get_secondary_id( &obj_id )
            : *vis_id;
    }

    /* notify new focused element */
    gui_marked_set_private_notify_listeners( this_, model_id );
}

static inline void gui_marked_set_clear_selected_set ( gui_marked_set_t *this_ )
{
    data_small_set_clear( &((*this_).selected_set) );
}

static inline void gui_marked_set_set_focused ( gui_marked_set_t *this_,
                                                data_full_id_t obj_id,
                                                data_id_t diagram_id  )
{
    assert(( data_id_get_table(&diagram_id) == DATA_TABLE_DIAGRAM )||( data_id_get_table(&diagram_id) == DATA_TABLE_VOID ));

    data_full_id_replace( &((*this_).focused ), &obj_id );
    data_id_replace( &((*this_).focused_diagram), &diagram_id );

    /* notify new focused element */
    const data_id_t *const vis_id = data_full_id_get_primary_id_ptr( &obj_id );
    const data_id_t model_id
        = (DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( vis_id ))
        ? data_full_id_get_secondary_id( &obj_id )
        : *vis_id;
    gui_marked_set_private_notify_listeners( this_, model_id );
}

static inline void gui_marked_set_set_highlighted ( gui_marked_set_t *this_,
                                                    data_id_t obj_id,
                                                    data_id_t diagram_id  )
{
    assert(( data_id_get_table(&diagram_id) == DATA_TABLE_DIAGRAM )||( data_id_get_table(&diagram_id) == DATA_TABLE_VOID ));
    data_id_replace( &((*this_).highlighted), &obj_id );
    data_id_replace( &((*this_).highlighted_diagram), &diagram_id );
    (*this_).highlighted_button = GUI_SKETCH_ACTION_NONE;
}

static inline void gui_marked_set_set_highlighted_button ( gui_marked_set_t *this_, gui_sketch_action_t button_id )
{
    data_id_init_void( &((*this_).highlighted) );
    data_id_init_void( &((*this_).highlighted_diagram) );
    (*this_).highlighted_button = button_id;
}

static inline void gui_marked_set_clear_focused ( gui_marked_set_t *this_ )
{
    data_full_id_reinit_void( &((*this_).focused) );

    /* notify new focused element */
    gui_marked_set_private_notify_listeners( this_, DATA_ID_VOID );
}

static inline void gui_marked_set_clear_highlighted ( gui_marked_set_t *this_ )
{
    data_id_reinit_void( &((*this_).highlighted) );
    data_id_reinit_void( &((*this_).highlighted_diagram) );
}


/*
Copyright 2016-2024 Andreas Warnke

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
