/* File: gui_tools.c; Copyright and License: see below */

#include "gui_tools.h"
#include "trace.h"
#include "ctrl_error.h"
#include "util/string/utf8string.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

static bool gui_tools_glib_signal_initialized = false;
static guint gui_tools_glib_signal_id = 0;
const char *GUI_TOOLS_GLIB_SIGNAL_NAME = "cfu_tool_changed";

void gui_tools_init ( gui_tools_t *this_,
                             GtkToolItem *tool_navigate,
                             GtkToolItem *tool_edit,
                             GtkToolItem *tool_new_obj,
                             GtkToolItem *tool_new_view,
                             GtkClipboard *clipboard,
                             gui_marked_set_t *marker,
                             gui_simple_message_to_user_t *message_to_user,
                             data_database_reader_t *db_reader,
                             ctrl_controller_t *controller )
{
    TRACE_BEGIN();
    assert( NULL != tool_navigate );
    assert( NULL != tool_edit );
    assert( NULL != tool_new_obj );
    assert( NULL != tool_new_view );
    assert( NULL != clipboard );
    assert( NULL != marker );
    assert( NULL != message_to_user );
    assert( NULL != db_reader );
    assert( NULL != controller );

    (*this_).selected_tool = GUI_TOOLS_NAVIGATE;
    (*this_).listener = NULL;
    (*this_).marker = marker;
    (*this_).message_to_user = message_to_user;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).tool_navigate = tool_navigate;
    (*this_).tool_edit = tool_edit;
    (*this_).tool_new_obj = tool_new_obj;
    (*this_).tool_new_view = tool_new_view;

    gui_serializer_deserializer_init ( &((*this_).serdes),
                                       clipboard,
                                       message_to_user,
                                       db_reader,
                                       controller
                                     );

    /* define a new signal */
    if ( ! gui_tools_glib_signal_initialized )
    {
        gui_tools_glib_signal_id = g_signal_new (
            GUI_TOOLS_GLIB_SIGNAL_NAME,
            G_TYPE_OBJECT,
            G_SIGNAL_RUN_FIRST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__INT,
            G_TYPE_NONE,
            1,
            G_TYPE_INT /* gui_tools_tool_t */
        );
        gui_tools_glib_signal_initialized = true;
        TRACE_INFO_INT( "g_signal_new(\"cfu_tool_changed\") returned new signal id", gui_tools_glib_signal_id );
    }

    TRACE_END();
}

void gui_tools_destroy ( gui_tools_t *this_ )
{
    TRACE_BEGIN();

    gui_serializer_deserializer_destroy ( &((*this_).serdes) );

    (*this_).db_reader = NULL;
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

void gui_tools_navigate_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOLS_NAVIGATE;

    gui_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_edit_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOLS_EDIT;

    gui_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_create_object_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOLS_CREATE;

    gui_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_create_diagram_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOLS_SEARCH;

    gui_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_cut_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_tools_cut( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_cut( gui_tools_t *this_ )
{
    TRACE_BEGIN();
    data_small_set_t *set_to_be_cut;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_cut = gui_marked_set_get_selected_set_ptr( (*this_).marker );

    /* do not check if set is empty; gui_serializer_deserializer_copy_set_to_clipboard will do this */

    gui_serializer_deserializer_copy_set_to_clipboard( &((*this_).serdes), set_to_be_cut );

    gui_tools_private_delete_set( this_, set_to_be_cut );

    gui_marked_set_clear_selected_set( (*this_).marker );

    TRACE_END();
}

void gui_tools_copy_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_tools_copy( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_copy( gui_tools_t *this_ )
{
    TRACE_BEGIN();
    data_small_set_t *set_to_be_copied;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_copied = gui_marked_set_get_selected_set_ptr( (*this_).marker );

    if ( data_small_set_is_empty( set_to_be_copied ) )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
        );
        /* continue nonetheless, it is possible to copy an empty set to the clipboard */
    }

    gui_serializer_deserializer_copy_set_to_clipboard( &((*this_).serdes), set_to_be_copied );

    TRACE_END();
}

void gui_tools_paste_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_tools_paste( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_paste( gui_tools_t *this_ )
{
    TRACE_BEGIN();
    gui_simple_message_to_user_hide( (*this_).message_to_user );

    int64_t destination_diagram_id = gui_marked_set_get_focused_diagram( (*this_).marker );
    gui_serializer_deserializer_request_clipboard_text( &((*this_).serdes), destination_diagram_id );

    TRACE_END();
}

void gui_tools_delete_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_tools_delete( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_delete( gui_tools_t *this_ )
{
    TRACE_BEGIN();

    data_small_set_t *set_to_be_deleted;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_deleted = gui_marked_set_get_selected_set_ptr( (*this_).marker );

    /* do not check if set is empty; gui_tools_private_delete_set will do this */

    gui_tools_private_delete_set( this_, set_to_be_deleted );

    gui_marked_set_clear_selected_set( (*this_).marker );

    TRACE_END();
}

void gui_tools_private_delete_set( gui_tools_t *this_, data_small_set_t *set_to_be_deleted )
{
    TRACE_BEGIN();
    ctrl_error_t ctrl_err;

    ctrl_err = ctrl_controller_delete_set ( (*this_).controller, *set_to_be_deleted );
    if ( CTRL_ERROR_INPUT_EMPTY == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
        );
    }
    else if ( 0 != ( CTRL_ERROR_MASK & CTRL_ERROR_OBJECT_STILL_REFERENCED & ctrl_err ))
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DELETING_NOT_POSSIBLE
        );
    }
    else if ( CTRL_ERROR_NONE != ctrl_err )
    {
        TSLOG_ERROR_HEX( "Error in ctrl_classifier_controller_delete_set_from_diagram", ctrl_err );
    }

    TRACE_END();
}

void gui_tools_highlight_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    data_small_set_t *set_to_be_highlighted;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_highlighted = gui_marked_set_get_selected_set_ptr( (*this_).marker );

    /* do not check if set is empty; gui_tools_private_toggle_display_flag_in_set will do this */

    gui_tools_private_toggle_display_flag_in_set( this_, set_to_be_highlighted, DATA_DIAGRAMELEMENT_FLAG_EMPHASIS | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_instantiate_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    data_small_set_t *set_to_be_instantiated;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_instantiated = gui_marked_set_get_selected_set_ptr( (*this_).marker );

    /* do not check if set is empty; gui_tools_private_toggle_display_flag_in_set will do this */

    gui_tools_private_toggle_display_flag_in_set( this_, set_to_be_instantiated, DATA_DIAGRAMELEMENT_FLAG_INSTANCE );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_reset_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    gui_marked_set_clear_selected_set( (*this_).marker );

    /* trigger redraw */
    gui_tools_private_notify_listener( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_private_toggle_display_flag_in_set( gui_tools_t *this_, data_small_set_t *set_to_be_toggled, data_diagramelement_flag_t flag_bits_to_toggle )
{
    TRACE_BEGIN();
    ctrl_error_t error = CTRL_ERROR_NONE;
    bool new_pattern_initialized = false;
    data_diagramelement_flag_t new_pattern = DATA_DIAGRAMELEMENT_FLAG_NONE;
    bool is_first = true;

    for ( int index = 0; index < data_small_set_get_count( set_to_be_toggled ); index ++ )
    {
        data_id_t current_id;
        current_id = data_small_set_get_id( set_to_be_toggled, index );
        switch ( data_id_get_table( &current_id ) )
        {
            case DATA_TABLE_CLASSIFIER:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_tools_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_tools_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_tools_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
                data_diagramelement_t out_diagramelement;
                int64_t diag_elem_id = data_id_get_row_id( &current_id );
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

                error |= (ctrl_error_t) data_database_reader_get_diagramelement_by_id ( (*this_).db_reader,
                                                                                        diag_elem_id,
                                                                                        &out_diagramelement
                                                                                      );
                data_diagramelement_flag_t current_flags;
                current_flags = data_diagramelement_get_display_flags( &out_diagramelement );

                if ( ! new_pattern_initialized )
                {
                    /* select zero or one bit to set. alg: select the next highest bit */
                    bool last_was_set = true;
                    new_pattern = DATA_DIAGRAMELEMENT_FLAG_NONE;
                    for ( int bit = 0; bit < 8*sizeof(data_diagramelement_flag_t); bit ++ )
                    {
                        data_diagramelement_flag_t probe = (1 << bit);
                        if ( 0 != ( probe & flag_bits_to_toggle ) )
                        {
                            /* this is a relevant bit */
                            if ( 0 != ( probe & current_flags ) )
                            {
                                new_pattern = DATA_DIAGRAMELEMENT_FLAG_NONE;
                                last_was_set = true;
                            }
                            else
                            {
                                if ( last_was_set )
                                {
                                    new_pattern = probe;
                                }
                                last_was_set = false;
                            }
                        }
                    }
                    new_pattern_initialized = true;
                }

                current_flags = (current_flags & (~flag_bits_to_toggle)) | new_pattern;

                error |= ctrl_diagram_controller_update_diagramelement_display_flags( diag_ctrl,
                                                                                      diag_elem_id,
                                                                                      current_flags,
                                                                                      ! is_first
                                                                                    );
                is_first = false;
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_tools_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            default:
            {
                /* program internal error */
                TSLOG_ERROR( "gui_tools_private_toggle_display_flag_in_set fould illegal data_table_t enum value." );
            }
            break;
        }
    }

    if ( error != CTRL_ERROR_NONE )
    {
        /* error to be shown to the user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_SET_PARTLY_UNSUITABLE
        );
    }
    else if ( 0 == data_small_set_get_count( set_to_be_toggled ) )
    {
        /* error to be shown to the user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
        );
    }

    TRACE_END();
}

void gui_tools_undo_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;
    ctrl_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    ctrl_err = ctrl_controller_undo( (*this_).controller );
    if ( CTRL_ERROR_INVALID_REQUEST == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_UNDO
        );
    }
    else if ( CTRL_ERROR_ARRAY_BUFFER_EXCEEDED == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_UNDO_NOT_POSSIBLE
        );
    }
    else
    {
        /* success */
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_redo_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_tools_t *this_ = data;
    ctrl_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    ctrl_err = ctrl_controller_redo( (*this_).controller );
    if ( CTRL_ERROR_INVALID_REQUEST == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_REDO
        );
    }
    else
    {
        /* success */
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_tools_private_notify_listener( gui_tools_t *this_ )
{
    TRACE_BEGIN();

    if ( (*this_).listener != NULL )
    {
        TRACE_INFO( "g_signal_emit to listener" );
        g_signal_emit( (*this_).listener, gui_tools_glib_signal_id, 0, (*this_).selected_tool );
    }

    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke

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
