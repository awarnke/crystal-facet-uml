/* File: gui_sketch_drag_state.inl; Copyright and License: see below */

static inline void gui_sketch_drag_state_init ( gui_sketch_drag_state_t *this_ )
{
    (*this_).dragging = false;
    (*this_).from_x = 0;
    (*this_).from_y = 0;
    (*this_).to_x = 0;
    (*this_).to_y = 0;
    (*this_).start_dragging_when_move = false;
    data_full_id_init_void ( &((*this_).dragged_object) );
}

static inline void gui_sketch_drag_state_reinit ( gui_sketch_drag_state_t *this_ )
{
    gui_sketch_drag_state_init( this_ );
}

static inline void gui_sketch_drag_state_destroy ( gui_sketch_drag_state_t *this_ )
{
    data_full_id_destroy ( &((*this_).dragged_object) );
}

static inline bool gui_sketch_drag_state_is_dragging ( const gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).dragging );
}

static inline void gui_sketch_drag_state_stop_dragging ( gui_sketch_drag_state_t *this_ )
{
    (*this_).dragging = false;
    (*this_).start_dragging_when_move = false;
}

static inline bool gui_sketch_drag_state_is_waiting_for_move ( const gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).start_dragging_when_move );
}

static inline void gui_sketch_drag_state_start_dragging_when_move ( gui_sketch_drag_state_t *this_,
                                                                    data_full_id_t dragged_object )
{
    (*this_).dragged_object = dragged_object;
    (*this_).start_dragging_when_move = true;
}

static inline int32_t gui_sketch_drag_state_get_from_x ( const gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).from_x );
}

static inline int32_t gui_sketch_drag_state_get_from_y ( const gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).from_y );
}

static inline void gui_sketch_drag_state_set_from ( gui_sketch_drag_state_t *this_, int32_t from_x, int32_t from_y )
{
    (*this_).from_x = from_x;
    (*this_).from_y = from_y;
}

static inline int32_t gui_sketch_drag_state_get_to_x ( const gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).to_x );
}

static inline int32_t gui_sketch_drag_state_get_to_y ( const gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).to_y );
}

static inline void gui_sketch_drag_state_set_to ( gui_sketch_drag_state_t *this_, int32_t to_x, int32_t to_y )
{
    (*this_).to_x = to_x;
    (*this_).to_y = to_y;
    if ( (*this_).start_dragging_when_move )
    {
        static const uint32_t ACTIVATION_BORDER = 2;
        int32_t activation_border_left = (*this_).from_x - ACTIVATION_BORDER;
        int32_t activation_border_right = (*this_).from_x + ACTIVATION_BORDER;
        int32_t activation_border_top = (*this_).from_y - ACTIVATION_BORDER;
        int32_t activation_border_bottom = (*this_).from_y + ACTIVATION_BORDER;
        if ( ( to_x > activation_border_right ) || ( to_x < activation_border_left )
            || ( to_y > activation_border_bottom ) || ( to_y < activation_border_top ) )
        {
            (*this_).dragging = true;
            (*this_).start_dragging_when_move = false;
        }
    }
}

static inline data_full_id_t *gui_sketch_drag_state_get_dragged_object_ptr ( gui_sketch_drag_state_t *this_ )
{
    return ( &((*this_).dragged_object) );
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
