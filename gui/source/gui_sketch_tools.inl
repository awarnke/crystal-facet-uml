/* File: gui_sketch_tools.inl; Copyright and License: see below */

static inline gui_sketch_tools_tool_t gui_sketch_tools_get_selected_tool ( gui_sketch_tools_t *this_ )
{
    return (*this_).selected_tool;
}

static inline void gui_sketch_tools_set_listener ( gui_sketch_tools_t *this_, GObject *listener )
{
    (*this_).listener = listener;
}

static inline void gui_sketch_tools_remove_listener ( gui_sketch_tools_t *this_ )
{
    (*this_).listener = NULL;
}

static inline gui_sketch_tools_tool_t gui_sketch_tools_set_selected_tool ( gui_sketch_tools_t *this_, gui_sketch_tools_tool_t tool )
{
    switch ( tool )
    {
        case GUI_SKETCH_TOOLS_NAVIGATE:
        {
            gtk_toggle_tool_button_set_active ( GTK_TOGGLE_TOOL_BUTTON( (*this_).tool_navigate ), true );
        }
        break;

        case GUI_SKETCH_TOOLS_EDIT:
        {
            gtk_toggle_tool_button_set_active ( GTK_TOGGLE_TOOL_BUTTON( (*this_).tool_edit ), true );
        }
        break;

        case GUI_SKETCH_TOOLS_CREATE_DIAGRAM:
        {
            gtk_toggle_tool_button_set_active ( GTK_TOGGLE_TOOL_BUTTON( (*this_).tool_new_view ), true );
        }
        break;

        case GUI_SKETCH_TOOLS_CREATE_OBJECT:
        {
            gtk_toggle_tool_button_set_active ( GTK_TOGGLE_TOOL_BUTTON( (*this_).tool_new_obj ), true );
        }
        break;

        default:
        {
            TSLOG_ERROR( "invalid enum value" );
        }
        break;
    }
}

/*
Copyright 2016-2017 Andreas Warnke

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
