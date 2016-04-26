/* File: gui_sketch_marker.inl; Copyright and License: see below */

static inline void gui_sketch_marker_init ( gui_sketch_marker_t *this_ )
{
    data_id_init_void( &((*this_).mark_focused) );
    data_id_init_void( &((*this_).mark_highlighted) );
    data_small_set_init( &((*this_).mark_selected) );
}

static inline void gui_sketch_marker_destroy ( gui_sketch_marker_t *this_ )
{
    data_id_destroy( &((*this_).mark_focused) );
    data_id_destroy( &((*this_).mark_highlighted) );
    data_small_set_destroy( &((*this_).mark_selected) );
}


/*
Copyright 2016-2016 Andreas Warnke

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
