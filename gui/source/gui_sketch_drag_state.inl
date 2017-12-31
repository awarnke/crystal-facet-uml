/* File: gui_sketch_drag_state.inl; Copyright and License: see below */

static inline void gui_sketch_drag_state_init ( gui_sketch_drag_state_t *this_ )
{
    (*this_).dragging = false;
    (*this_).from_x = 0;
    (*this_).from_y = 0;
    (*this_).to_x = 0;
    (*this_).to_y = 0;
    (*this_).start_dragging_when_move = false;
}

static inline void gui_sketch_drag_state_destroy ( gui_sketch_drag_state_t *this_ )
{
}

static inline bool gui_sketch_drag_state_is_dragging ( gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).dragging );
}

static inline void gui_sketch_drag_state_set_dragging ( gui_sketch_drag_state_t *this_, bool dragging )
{
    (*this_).dragging = dragging;
    (*this_).start_dragging_when_move = false;
}

static inline bool gui_sketch_drag_state_is_waiting_for_move ( gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).start_dragging_when_move );
}

static inline void gui_sketch_drag_state_start_dragging_when_move ( gui_sketch_drag_state_t *this_ )
{
    (*this_).start_dragging_when_move = true;
}

static inline int32_t gui_sketch_drag_state_get_from_x ( gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).from_x );
}

static inline int32_t gui_sketch_drag_state_get_from_y ( gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).from_y );
}

static inline void gui_sketch_drag_state_set_from ( gui_sketch_drag_state_t *this_, int32_t from_x, int32_t from_y )
{
    (*this_).from_x = from_x;
    (*this_).from_y = from_y;
}

static inline int32_t gui_sketch_drag_state_get_to_x ( gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).to_x );
}

static inline int32_t gui_sketch_drag_state_get_to_y ( gui_sketch_drag_state_t *this_ )
{
    return ( (*this_).to_y );
}

static inline void gui_sketch_drag_state_set_to ( gui_sketch_drag_state_t *this_, int32_t to_x, int32_t to_y )
{
    (*this_).to_x = to_x;
    (*this_).to_y = to_y;
    if ( (*this_).start_dragging_when_move )
    {
        static const int ACTIVATION_BORDER = 2;
        if ( ( (*this_).from_x + ACTIVATION_BORDER < to_x ) || ( to_x < (*this_).from_x - ACTIVATION_BORDER )
            || ( (*this_).from_y + ACTIVATION_BORDER < to_y ) || ( to_y < (*this_).from_y - ACTIVATION_BORDER ) )
        {
            (*this_).dragging = true;
            (*this_).start_dragging_when_move = false;
        }
    }
}


/*
Copyright 2016-2018 Andreas Warnke

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
