/* File: gui_sketch_tools.c; Copyright and License: see below */

#include "gui_sketch_tools.h"
#include "trace.h"
#include "ctrl_error.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

static bool gui_sketch_tools_glib_signal_initialized = false;
static guint gui_sketch_tools_glib_signal_id = 0;
const char *GUI_SKETCH_TOOLS_GLIB_SIGNAL_NAME = "cfu_tool_changed";

void gui_sketch_tools_init ( gui_sketch_tools_t *this_,
                             GtkToolItem *tool_navigate,
                             GtkToolItem *tool_edit,
                             GtkToolItem *tool_new_obj,
                             GtkToolItem *tool_new_view,
                             gui_sketch_marker_t *marker,
                             gui_simple_message_to_user_t *message_to_user,
                             ctrl_controller_t *controller )
{
    TRACE_BEGIN();
    assert( NULL != message_to_user );

    (*this_).selected_tool = GUI_SKETCH_TOOLS_NAVIGATE;
    (*this_).listener = NULL;
    (*this_).marker = marker;
    (*this_).message_to_user = message_to_user;
    (*this_).controller = controller;
    (*this_).tool_navigate = tool_navigate;
    (*this_).tool_edit = tool_edit;
    (*this_).tool_new_obj = tool_new_obj;
    (*this_).tool_new_view = tool_new_view;

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

    (*this_).controller = NULL;
    (*this_).listener = NULL;
    (*this_).marker = NULL;
    (*this_).message_to_user = NULL;
    (*this_).tool_navigate = NULL;
    (*this_).tool_edit = NULL;
    (*this_).tool_new_obj = NULL;
    (*this_).tool_new_view = NULL;

    TRACE_END();
}

void gui_sketch_tools_navigate_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_SKETCH_TOOLS_NAVIGATE;

    gui_sketch_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_edit_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_SKETCH_TOOLS_EDIT;

    gui_sketch_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_create_object_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_SKETCH_TOOLS_CREATE_OBJECT;

    gui_sketch_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_create_diagram_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_SKETCH_TOOLS_CREATE_DIAGRAM;

    gui_sketch_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_cut_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                         GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                         GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED,
                                                         "Cut"
    );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_copy_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                         GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                         GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED,
                                                         "Copy"
    );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_paste_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                         GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                         GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED,
                                                         "Paste"
    );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_delete_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
    ctrl_error_t ctrl_err;
    data_small_set_t *set_to_be_deleted;
    ctrl_classifier_controller_t *c_controller;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_deleted = gui_sketch_marker_get_selected_set_ptr( (*this_).marker );
    c_controller = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
    ctrl_err = ctrl_classifier_controller_delete_set ( c_controller, *set_to_be_deleted );
    if ( CTRL_ERROR_INPUT_EMPTY == ctrl_err )
    {
        gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION,
                                                             NULL
        );
    }
    else if ( 0 != ( CTRL_ERROR_MASK & CTRL_ERROR_OBJECT_STILL_REFERENCED & ctrl_err ))
    {
        gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                             GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                             GUI_SIMPLE_MESSAGE_CONTENT_DELETING_NOT_POSSIBLE,
                                                             NULL
        );
    }
    else if ( CTRL_ERROR_NONE != ctrl_err )
    {
        LOG_ERROR_HEX( "Error in ctrl_classifier_controller_delete_set_from_diagram", ctrl_err );
    }

    gui_sketch_marker_clear_selected_set( (*this_).marker );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_undo_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
    ctrl_error_t ctrl_err;
    ctrl_undo_redo_list_t *undo_redo_list;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    undo_redo_list = ctrl_controller_get_undo_redo_list_ptr ( (*this_).controller );
    ctrl_err = ctrl_undo_redo_list_undo( undo_redo_list );

    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                         GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                         GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED,
                                                         "Undo"
    );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_redo_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
    ctrl_error_t ctrl_err;
    ctrl_undo_redo_list_t *undo_redo_list;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    undo_redo_list = ctrl_controller_get_undo_redo_list_ptr ( (*this_).controller );
    ctrl_err = ctrl_undo_redo_list_redo( undo_redo_list );

    gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                         GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                         GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED,
                                                         "Redo"
    );

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
