/* File: gui_toolbox.c; Copyright and License: see below */

#include "gui_toolbox.h"
#include "trace.h"
#include "ctrl_error.h"
#include "util/string/utf8string.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

static bool gui_toolbox_glib_signal_initialized = false;
static guint gui_toolbox_glib_signal_id = 0;
const char *GUI_TOOLBOX_GLIB_SIGNAL_NAME = "cfu_tool_changed";

void gui_toolbox_init ( gui_toolbox_t *this_,
                        GtkWidget *toolbar,
                        GtkToolItem *tool_navigate,
                        GtkToolItem *tool_edit,
                        GtkToolItem *tool_create,
                        GtkToolItem *tool_search,
                        GtkClipboard *gtk_clipboard,
                        gui_marked_set_t *marker,
                        gui_simple_message_to_user_t *message_to_user,
                        data_database_reader_t *db_reader,
                        ctrl_controller_t *controller )
{
    TRACE_BEGIN();
    assert( NULL != tool_navigate );
    assert( NULL != tool_edit );
    assert( NULL != tool_create );
    assert( NULL != tool_search );
    assert( NULL != gtk_clipboard );
    assert( NULL != marker );
    assert( NULL != message_to_user );
    assert( NULL != db_reader );
    assert( NULL != controller );

    (*this_).selected_tool = GUI_TOOL_NAVIGATE;
#if 0
    (*this_).listener = NULL;
#endif
    (*this_).marker = marker;
    (*this_).message_to_user = message_to_user;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).toolbar = toolbar;
    (*this_).tool_navigate = tool_navigate;
    (*this_).tool_edit = tool_edit;
    (*this_).tool_create = tool_create;
    (*this_).tool_search = tool_search;

    gui_clipboard_init ( &((*this_).clipboard),
                         gtk_clipboard,
                         message_to_user,
                         db_reader,
                         controller
                       );

    /* define a new signal */
    if ( ! gui_toolbox_glib_signal_initialized )
    {
        gui_toolbox_glib_signal_id = g_signal_new (
            GUI_TOOLBOX_GLIB_SIGNAL_NAME,
            G_TYPE_OBJECT,
            G_SIGNAL_RUN_FIRST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__INT,
            G_TYPE_NONE,
            1,
            G_TYPE_INT /* gui_tool_t */
        );
        gui_toolbox_glib_signal_initialized = true;
        TRACE_INFO_INT( "g_signal_new(\"cfu_tool_changed\") returned new signal id", gui_toolbox_glib_signal_id );
    }

    TRACE_END();
}

void gui_toolbox_destroy ( gui_toolbox_t *this_ )
{
    TRACE_BEGIN();

    gui_clipboard_destroy ( &((*this_).clipboard) );

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
#if 0
    (*this_).listener = NULL;
#endif
    (*this_).marker = NULL;
    (*this_).message_to_user = NULL;
    (*this_).toolbar = NULL;
    (*this_).tool_navigate = NULL;
    (*this_).tool_edit = NULL;
    (*this_).tool_create = NULL;
    (*this_).tool_search = NULL;

    TRACE_END();
}

void gui_toolbox_navigate_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOL_NAVIGATE;

    gui_toolbox_private_notify_listeners( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_edit_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOL_EDIT;

    gui_toolbox_private_notify_listeners( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_create_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOL_CREATE;

    gui_toolbox_private_notify_listeners( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_search_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOL_SEARCH;

    gui_toolbox_private_notify_listeners( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_cut_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_toolbox_cut( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_cut( gui_toolbox_t *this_ )
{
    TRACE_BEGIN();
    ctrl_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init(&stat);

    const data_small_set_t *const set_to_be_cut = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* do not check if set is empty; gui_clipboard_copy_set_to_clipboard will do this */

    gui_clipboard_copy_set_to_clipboard( &((*this_).clipboard), set_to_be_cut, &stat );

    ctrl_err = gui_toolbox_private_delete_set( this_, set_to_be_cut, &stat );

    gui_marked_set_clear_selected_set( (*this_).marker );

    if ( CTRL_ERROR_INPUT_EMPTY == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
                                               );
    }
    else if ( CTRL_ERROR_NONE != ( ctrl_err & CTRL_ERROR_OBJECT_STILL_REFERENCED ))
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
    else
    {
        gui_simple_message_to_user_show_message_with_stat ( (*this_).message_to_user,
                                                            GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                            GUI_SIMPLE_MESSAGE_CONTENT_CUT_TO_CLIPBOARD,
                                                            &stat
                                                          );
    }

    data_stat_destroy(&stat);

    TRACE_END();
}

void gui_toolbox_copy_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_toolbox_copy( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_copy( gui_toolbox_t *this_ )
{
    TRACE_BEGIN();
    int out_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init(&stat);

    const data_small_set_t *const set_to_be_copied = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* even in case data_small_set_is_empty( set_to_be_copied ),
     * it is possible to copy an empty set to the clipboard
     * --> therefore simply continue... */
    out_err = gui_clipboard_copy_set_to_clipboard( &((*this_).clipboard), set_to_be_copied, &stat );

    if ( out_err == 0 )
    {
        if ( 0 == data_stat_get_total_count( &stat ) )
        {
            gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                    GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                    GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
                                                );
        }
        else
        {
            gui_simple_message_to_user_show_message_with_stat ( (*this_).message_to_user,
                                                                GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                                GUI_SIMPLE_MESSAGE_CONTENT_COPY_TO_CLIPBOARD,
                                                                &stat
                                                              );
        }
    }
    else
    {
        /* error to be shown to the user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_STRING_TRUNCATED
                                               );
    }

    data_stat_destroy(&stat);

    TRACE_END();
}

void gui_toolbox_paste_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_toolbox_paste( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_paste( gui_toolbox_t *this_ )
{
    TRACE_BEGIN();
    gui_simple_message_to_user_hide( (*this_).message_to_user );

    const data_id_t destination_diagram_id = gui_marked_set_get_focused_diagram( (*this_).marker );
    const data_row_id_t dest_diagram_row_id = data_id_get_row_id( &destination_diagram_id );
    gui_clipboard_request_clipboard_text( &((*this_).clipboard), dest_diagram_row_id );

    /* Note: (*this_).message_to_user is updated by (*this_).clipboard already - nothing to do here */

    TRACE_END();
}

void gui_toolbox_delete_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_toolbox_delete( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_delete( gui_toolbox_t *this_ )
{
    TRACE_BEGIN();
    ctrl_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init(&stat);

    const data_small_set_t *const set_to_be_deleted = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* do not check if set is empty; gui_toolbox_private_delete_set will do this */

    ctrl_err = gui_toolbox_private_delete_set( this_, set_to_be_deleted, &stat );

    gui_marked_set_clear_selected_set( (*this_).marker );

    if ( CTRL_ERROR_INPUT_EMPTY == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
                                               );
    }
    else if ( CTRL_ERROR_NONE != ( ctrl_err & CTRL_ERROR_OBJECT_STILL_REFERENCED ))
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
    else
    {
        gui_simple_message_to_user_show_message_with_stat ( (*this_).message_to_user,
                                                            GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                            GUI_SIMPLE_MESSAGE_CONTENT_DELETE,
                                                            &stat
                                                          );
    }

    data_stat_destroy(&stat);

    TRACE_END();
}

ctrl_error_t gui_toolbox_private_delete_set( gui_toolbox_t *this_,
                                             const data_small_set_t *set_to_be_deleted,
                                             data_stat_t *io_stat )
{
    TRACE_BEGIN();
    assert( NULL != set_to_be_deleted );
    assert( NULL != io_stat );
    ctrl_error_t ctrl_err;

    ctrl_err = ctrl_controller_delete_set ( (*this_).controller, set_to_be_deleted, io_stat );

    TRACE_END_ERR( ctrl_err );
    return ctrl_err;
}

void gui_toolbox_highlight_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    const data_small_set_t *const set_to_be_highlighted = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* do not check if set is empty; gui_toolbox_private_toggle_display_flag_in_set will do this */

    gui_toolbox_private_toggle_display_flag_in_set( this_,
                                                    set_to_be_highlighted,
                                                    DATA_DIAGRAMELEMENT_FLAG_EMPHASIS | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT
                                                  );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_instantiate_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    const data_small_set_t *const set_to_be_instantiated = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* do not check if set is empty; gui_toolbox_private_toggle_display_flag_in_set will do this */

    gui_toolbox_private_toggle_display_flag_in_set( this_,
                                                    set_to_be_instantiated,
                                                    DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE
                                                  );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_reset_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    gui_marked_set_clear_selected_set( (*this_).marker );

    /* trigger redraw */
    gui_toolbox_private_notify_listeners( this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_private_toggle_display_flag_in_set( gui_toolbox_t *this_, const data_small_set_t *set_to_be_toggled, data_diagramelement_flag_t flag_bits_to_toggle )
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
                TSLOG_WARNING( "gui_toolbox_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_toolbox_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_toolbox_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
                data_diagramelement_t out_diagramelement;
                data_row_id_t diag_elem_id = data_id_get_row_id( &current_id );
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
                                                                                      ( is_first
                                                                                      ? CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW
                                                                                      : CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
                                                                                    );
                is_first = false;
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_toolbox_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            default:
            {
                /* program internal error */
                TSLOG_ERROR( "gui_toolbox_private_toggle_display_flag_in_set fould illegal data_table_t enum value." );
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

void gui_toolbox_undo_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    ctrl_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init(&stat);

    ctrl_err = ctrl_controller_undo( (*this_).controller, &stat );
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
        gui_simple_message_to_user_show_message_with_stat ( (*this_).message_to_user,
                                                            GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                            GUI_SIMPLE_MESSAGE_CONTENT_UNDO,
                                                            &stat
                                                          );
    }

    data_stat_destroy(&stat);

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_redo_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    ctrl_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init(&stat);

    ctrl_err = ctrl_controller_redo( (*this_).controller, &stat );
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
        gui_simple_message_to_user_show_message_with_stat ( (*this_).message_to_user,
                                                            GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                            GUI_SIMPLE_MESSAGE_CONTENT_REDO,
                                                            &stat
                                                          );
    }

    data_stat_destroy(&stat);

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_toolbox_private_notify_listeners( gui_toolbox_t *this_ )
{
    TRACE_BEGIN();

    TRACE_INFO( "g_signal_emit to listeners" );
    g_signal_emit( (*this_).toolbar, gui_toolbox_glib_signal_id, 0, (*this_).selected_tool );

    TRACE_END();
}


/*
Copyright 2016-2021 Andreas Warnke

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
