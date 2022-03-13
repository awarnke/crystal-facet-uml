/* File: gui_toolbox.inl; Copyright and License: see below */

static inline gui_tool_t gui_toolbox_get_selected_tool( gui_toolbox_t *this_ )
{
    return (*this_).selected_tool;
}

static inline void gui_toolbox_set_selected_tool( gui_toolbox_t *this_, gui_tool_t tool )
{
    switch ( tool )
    {
        case GUI_TOOL_NAVIGATE:
        {
#if ( GTK_MAJOR_VERSION >= 4 )
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( (*this_).tool_navigate ), true );
#else
            gtk_toggle_tool_button_set_active( GTK_TOGGLE_TOOL_BUTTON( (*this_).tool_navigate ), true );
#endif
        }
        break;

        case GUI_TOOL_EDIT:
        {
#if ( GTK_MAJOR_VERSION >= 4 )
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( (*this_).tool_edit ), true );
#else
            gtk_toggle_tool_button_set_active( GTK_TOGGLE_TOOL_BUTTON( (*this_).tool_edit ), true );
#endif
        }
        break;

        case GUI_TOOL_SEARCH:
        {
#if ( GTK_MAJOR_VERSION >= 4 )
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( (*this_).tool_search ), true );
#else
            gtk_toggle_tool_button_set_active( GTK_TOGGLE_TOOL_BUTTON( (*this_).tool_search ), true );
#endif
        }
        break;

        case GUI_TOOL_CREATE:
        {
#if ( GTK_MAJOR_VERSION >= 4 )
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( (*this_).tool_create ), true );
#else
            gtk_toggle_tool_button_set_active( GTK_TOGGLE_TOOL_BUTTON( (*this_).tool_create ), true );
#endif
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
Copyright 2016-2022 Andreas Warnke

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
