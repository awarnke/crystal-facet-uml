/* File: gui_sketch_request.inl; Copyright and License: see below */

#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static inline void gui_sketch_request_init( gui_sketch_request_t *this_ )
{
    (*this_).selected_tool = GUI_TOOL_NAVIGATE;
    data_id_init_void ( &((*this_).focused_diagram) );
    data_id_init_void ( &((*this_).parent_diagram) );
    data_small_set_init( &((*this_).search_result_diagrams) );
}

static inline void gui_sketch_request_reinit( gui_sketch_request_t *this_ )
{
    (*this_).selected_tool = GUI_TOOL_NAVIGATE;
    data_id_reinit_void ( &((*this_).focused_diagram) );
    data_id_reinit_void ( &((*this_).parent_diagram) );
    data_small_set_reinit( &((*this_).search_result_diagrams) );
}

static inline void gui_sketch_request_destroy( gui_sketch_request_t *this_ )
{
    (*this_).selected_tool = GUI_TOOL_NAVIGATE;
    data_id_destroy ( &((*this_).focused_diagram) );
    data_id_destroy ( &((*this_).parent_diagram) );
    data_small_set_destroy( &((*this_).search_result_diagrams) );
}


/*
Copyright 2021-2021 Andreas Warnke

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
