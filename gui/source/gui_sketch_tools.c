/* File: gui_sketch_tools.c; Copyright and License: see below */

#include "gui_sketch_tools.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdbool.h>

static bool gui_sketch_tools_glib_signal_initialized = false;
static guint gui_sketch_tools_glib_signal_id = 0;
const char *GUI_SKETCH_TOOLS_GLIB_SIGNAL_NAME = "cfu_tool_changed";

void gui_sketch_tools_init ( gui_sketch_tools_t *this_ )
{
    TRACE_BEGIN();

    (*this_).selected_tool = GUI_SKETCH_TOOLS_NAVIGATE;
    (*this_).listener = NULL;

    /* define a new signal */
    if ( ! gui_sketch_tools_glib_signal_initialized )
    {
        gui_sketch_tools_glib_signal_id = g_signal_new (
            GUI_SKETCH_TOOLS_GLIB_SIGNAL_NAME,
            G_TYPE_OBJECT,
            G_SIGNAL_RUN_FIRST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__INT,
            G_TYPE_NONE,
            1,
            G_TYPE_INT /* gui_sketch_tools_tool_t */
        );
        gui_sketch_tools_glib_signal_initialized = true;
        TRACE_INFO_INT( "g_signal_new(\"cfu_tool_changed\") returned new signal id", gui_sketch_tools_glib_signal_id );
    }

    TRACE_END();
}

void gui_sketch_tools_destroy ( gui_sketch_tools_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
}

void gui_sketch_tools_navigate_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    (*this_).selected_tool = GUI_SKETCH_TOOLS_NAVIGATE;

    gui_sketch_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_edit_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    (*this_).selected_tool = GUI_SKETCH_TOOLS_EDIT;

    gui_sketch_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_create_object_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    (*this_).selected_tool = GUI_SKETCH_TOOLS_CREATE_OBJECT;

    gui_sketch_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_create_diagram_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    (*this_).selected_tool = GUI_SKETCH_TOOLS_CREATE_DIAGRAM;

    gui_sketch_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_private_notify_listener( gui_sketch_tools_t *this_ )
{
    TRACE_BEGIN();

    if ( (*this_).listener != NULL )
    {
        TRACE_INFO( "g_signal_emit to listener" );
        g_signal_emit( (*this_).listener, gui_sketch_tools_glib_signal_id, 0, (*this_).selected_tool );
    }

    TRACE_END();
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
