/* File: gui_sketch_request.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static inline void gui_sketch_request_init( gui_sketch_request_t *this_ )
{
    (*this_).tool_mode = GUI_TOOL_NAVIGATE;
    data_id_init_void ( &((*this_).focused_diagram) );
    data_id_init_void ( &((*this_).parent_diagram) );
    data_small_set_init( &((*this_).search_result_diagrams) );
}

static inline void gui_sketch_request_reinit( gui_sketch_request_t *this_ )
{
    (*this_).tool_mode = GUI_TOOL_NAVIGATE;
    data_id_reinit_void ( &((*this_).focused_diagram) );
    data_id_reinit_void ( &((*this_).parent_diagram) );
    data_small_set_reinit( &((*this_).search_result_diagrams) );
}

static inline void gui_sketch_request_destroy( gui_sketch_request_t *this_ )
{
    (*this_).tool_mode = GUI_TOOL_NAVIGATE;
    data_id_destroy ( &((*this_).focused_diagram) );
    data_id_destroy ( &((*this_).parent_diagram) );
    data_small_set_destroy( &((*this_).search_result_diagrams) );
}

static inline gui_tool_t gui_sketch_request_get_tool_mode( const gui_sketch_request_t *this_ )
{
    return (*this_).tool_mode;
}

static inline void gui_sketch_request_set_tool_mode( gui_sketch_request_t *this_, gui_tool_t tool_mode )
{
    switch ( tool_mode )
    {
        case GUI_TOOL_NAVIGATE:
        {
            U8_TRACE_INFO("GUI_TOOL_NAVIGATE");
        }
        break;

        case GUI_TOOL_EDIT:
        {
            U8_TRACE_INFO("GUI_TOOL_EDIT");
        }
        break;

        case GUI_TOOL_SEARCH:
        {
            U8_TRACE_INFO("GUI_TOOL_SEARCH");
        }
        break;

        case GUI_TOOL_CREATE:
        {
            U8_TRACE_INFO("GUI_TOOL_CREATE");
        }
        break;

        default:
        {
            U8_LOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    (*this_).tool_mode = tool_mode;
}

static inline data_id_t gui_sketch_request_get_focused_diagram( const gui_sketch_request_t *this_ )
{
    return (*this_).focused_diagram;
}

static inline data_row_t gui_sketch_request_get_focused_diagram_row_id( const gui_sketch_request_t *this_ )
{
    return data_id_get_row_id( &((*this_).focused_diagram) );
}

static inline void gui_sketch_request_set_focused_diagram( gui_sketch_request_t *this_, data_id_t focused_diagram )
{
    (*this_).focused_diagram = focused_diagram;
    data_id_trace( &((*this_).focused_diagram) );
}

static inline void gui_sketch_request_set_focused_diagram_row_id( gui_sketch_request_t *this_, data_row_t focused_diagram )
{
    data_id_reinit( &((*this_).focused_diagram), DATA_TABLE_DIAGRAM, focused_diagram );
    data_id_trace( &((*this_).focused_diagram) );
}

static inline data_id_t gui_sketch_request_get_parent_diagram( const gui_sketch_request_t *this_ )
{
    return (*this_).parent_diagram;
}

static inline data_row_t gui_sketch_request_get_parent_diagram_row_id( const gui_sketch_request_t *this_ )
{
    return data_id_get_row_id( &((*this_).parent_diagram ) );
}

static inline void gui_sketch_request_set_parent_diagram( gui_sketch_request_t *this_, data_id_t parent_diagram )
{
    (*this_).parent_diagram = parent_diagram;
    data_id_trace( &((*this_).parent_diagram) );
}

static inline void gui_sketch_request_set_parent_diagram_row_id( gui_sketch_request_t *this_, data_row_t parent_diagram )
{
    data_id_reinit( &((*this_).parent_diagram), DATA_TABLE_DIAGRAM, parent_diagram );
    data_id_trace( &((*this_).parent_diagram) );
}

static inline const data_small_set_t * gui_sketch_request_get_search_result_diagrams_const( const gui_sketch_request_t *this_ )
{
    return &((*this_).search_result_diagrams);
}

static inline data_small_set_t * gui_sketch_request_get_search_result_diagrams_ptr( gui_sketch_request_t *this_ )
{
    return &((*this_).search_result_diagrams);
}


/*
Copyright 2021-2024 Andreas Warnke

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
