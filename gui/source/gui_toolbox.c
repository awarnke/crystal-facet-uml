/* File: gui_toolbox.c; Copyright and License: see below */

#include "gui_toolbox.h"
#include "ctrl_multi_step_changer.h"
#include "u8/u8_trace.h"
#include "u8/u8_error.h"
#include "utf8stringbuf/utf8string.h"
#include <assert.h>
#include "gui_gtk.h"
#include <stdbool.h>

static bool gui_toolbox_glib_signal_initialized = false;
static guint gui_toolbox_glib_signal_id = 0;
const char *GUI_TOOLBOX_GLIB_SIGNAL_NAME = "cfu_tool_changed";

void gui_toolbox_init( gui_toolbox_t *this_,
                       GtkWidget *toolbar,
                       GtkWidget *tool_navigate,
                       GtkWidget *tool_edit,
                       GtkWidget *tool_create,
                       GtkWidget *tool_search,
                       GdkClipboard *gtk_clipboard,
                       gui_marked_set_t *marker,
                       gui_simple_message_to_user_t *message_to_user,
                       data_database_reader_t *db_reader,
                       ctrl_controller_t *controller )
{
    U8_TRACE_BEGIN();
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
    (*this_).marker = marker;
    (*this_).message_to_user = message_to_user;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).toolbar = toolbar;
    (*this_).tool_navigate = tool_navigate;
    (*this_).tool_edit = tool_edit;
    (*this_).tool_create = tool_create;
    (*this_).tool_search = tool_search;

    gui_clipboard_init( &((*this_).clipboard),
                        gtk_clipboard,
                        this_,
                        message_to_user,
                        db_reader,
                        controller
                      );

    /* define a new signal */
    if ( ! gui_toolbox_glib_signal_initialized )
    {
        gui_toolbox_glib_signal_id = g_signal_new(
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
        U8_TRACE_INFO_INT( "g_signal_new(\"cfu_tool_changed\") returned new signal id", gui_toolbox_glib_signal_id );
    }

    U8_TRACE_END();
}

void gui_toolbox_destroy( gui_toolbox_t *this_ )
{
    U8_TRACE_BEGIN();

    gui_clipboard_destroy( &((*this_).clipboard) );

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;
    (*this_).marker = NULL;
    (*this_).message_to_user = NULL;
    (*this_).toolbar = NULL;
    (*this_).tool_navigate = NULL;
    (*this_).tool_edit = NULL;
    (*this_).tool_create = NULL;
    (*this_).tool_search = NULL;

    U8_TRACE_END();
}

void gui_toolbox_set_selected_tool( gui_toolbox_t *this_, gui_tool_t tool )
{
    U8_TRACE_BEGIN();

    switch ( tool )
    {
        case GUI_TOOL_NAVIGATE:
        {
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( (*this_).tool_navigate ), true );
            gui_simple_message_to_user_hide( (*this_).message_to_user );
            (*this_).selected_tool = GUI_TOOL_NAVIGATE;
            gui_toolbox_private_notify_listeners( this_ );
        }
        break;

        case GUI_TOOL_EDIT:
        {
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( (*this_).tool_edit ), true );
            gui_simple_message_to_user_hide( (*this_).message_to_user );
            (*this_).selected_tool = GUI_TOOL_EDIT;
            gui_toolbox_private_notify_listeners( this_ );
        }
        break;

        case GUI_TOOL_SEARCH:
        {
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( (*this_).tool_search ), true );
            gui_simple_message_to_user_hide( (*this_).message_to_user );
            (*this_).selected_tool = GUI_TOOL_SEARCH;
            gui_toolbox_private_notify_listeners( this_ );
        }
        break;

        case GUI_TOOL_CREATE:
        {
            gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( (*this_).tool_create ), true );
            gui_simple_message_to_user_hide( (*this_).message_to_user );
            (*this_).selected_tool = GUI_TOOL_CREATE;
            gui_toolbox_private_notify_listeners( this_ );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "invalid enum value" );
        }
        break;
    }

    U8_TRACE_END();
}

void gui_toolbox_set_selected_tool_and_focus ( gui_toolbox_t *this_, gui_tool_t tool, data_id_t focused_diagram )
{
    U8_TRACE_BEGIN();

    /* switch tool */
    gui_toolbox_set_selected_tool( this_, tool );

    const data_id_t current_focus = gui_marked_set_get_focused_diagram( (*this_).marker );
    if ( ! data_id_equals_or_both_void( &focused_diagram, &current_focus ) )
    {
        /* clear selected set (no notification) */
        gui_marked_set_clear_selected_set( (*this_).marker );

        /* request to load the diagram to focus on */
        gui_marked_set_request_focused_diagram( (*this_).marker, focused_diagram );
    }

    U8_TRACE_END();
}

void gui_toolbox_navigate_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOL_NAVIGATE;

    gui_toolbox_private_notify_listeners( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_edit_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOL_EDIT;

    gui_toolbox_private_notify_listeners( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_create_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOL_CREATE;

    gui_toolbox_private_notify_listeners( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_search_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    (*this_).selected_tool = GUI_TOOL_SEARCH;

    gui_toolbox_private_notify_listeners( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_search_id_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_toolbox_set_selected_tool( this_, GUI_TOOL_SEARCH );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_cut_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_toolbox_cut( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_cut( gui_toolbox_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init(&stat);

    const data_small_set_t *const set_to_be_cut = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* do not check if set is empty; gui_clipboard_copy_set_to_clipboard will do this */

    gui_clipboard_copy_set_to_clipboard( &((*this_).clipboard), set_to_be_cut, &stat );

    ctrl_err = gui_toolbox_private_delete_set( this_, set_to_be_cut, &stat );

    gui_marked_set_clear_selected_set( (*this_).marker );

    if ( U8_ERROR_INPUT_EMPTY == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
                                               );
    }
    else if ( u8_error_contains( ctrl_err, U8_ERROR_OBJECT_STILL_REFERENCED ) )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DELETING_NOT_POSSIBLE
                                               );
    }
    else if ( U8_ERROR_NONE != ctrl_err )
    {
        U8_LOG_ERROR_HEX( "Error in ctrl_classifier_controller_delete_set_from_diagram", ctrl_err );
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

    U8_TRACE_END();
}

void gui_toolbox_copy_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_toolbox_copy( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_copy( gui_toolbox_t *this_ )
{
    U8_TRACE_BEGIN();
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
            gui_simple_message_to_user_show_message_with_stat( (*this_).message_to_user,
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

    U8_TRACE_END();
}

void gui_toolbox_paste_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_toolbox_paste( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_paste( gui_toolbox_t *this_ )
{
    U8_TRACE_BEGIN();
    gui_simple_message_to_user_hide( (*this_).message_to_user );

    const data_id_t destination_diagram_id = gui_marked_set_get_focused_diagram( (*this_).marker );
    if ( data_id_is_valid( &destination_diagram_id ) )
    {
        const data_row_t dest_diagram_row_id = data_id_get_row_id( &destination_diagram_id );
        gui_clipboard_request_clipboard_text( &((*this_).clipboard), dest_diagram_row_id );

        /* this call triggers a callback later to gui_clipboard_clipboard_text_received_callback */

        /* Note: (*this_).message_to_user is updated by (*this_).clipboard already - nothing to do here */
    }
    else
    {
        /* error to be shown to the user */
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_FOCUS
                                               );
    }

    U8_TRACE_END();
}

void gui_toolbox_delete_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_toolbox_delete( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_delete( gui_toolbox_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init(&stat);

    const data_small_set_t *const set_to_be_deleted = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* do not check if set is empty; gui_toolbox_private_delete_set will do this */

    ctrl_err = gui_toolbox_private_delete_set( this_, set_to_be_deleted, &stat );

    gui_marked_set_clear_selected_set( (*this_).marker );

    if ( U8_ERROR_INPUT_EMPTY == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
                                               );
    }
    else if ( u8_error_contains( ctrl_err, U8_ERROR_OBJECT_STILL_REFERENCED ) )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_DELETING_NOT_POSSIBLE
                                               );
    }
    else if ( U8_ERROR_NONE != ctrl_err )
    {
        U8_LOG_ERROR_HEX( "Error in ctrl_classifier_controller_delete_set_from_diagram", ctrl_err );
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

    U8_TRACE_END();
}

u8_error_t gui_toolbox_private_delete_set( gui_toolbox_t *this_,
                                           const data_small_set_t *set_to_be_deleted,
                                           data_stat_t *io_stat )
{
    U8_TRACE_BEGIN();
    assert( NULL != set_to_be_deleted );
    assert( NULL != io_stat );
    u8_error_t ctrl_err;

    ctrl_multi_step_changer_t multi_stepper;
    ctrl_multi_step_changer_init( &multi_stepper, (*this_).controller, (*this_).db_reader );

    ctrl_err = ctrl_multi_step_changer_delete_set ( &multi_stepper, set_to_be_deleted, io_stat );

    ctrl_multi_step_changer_destroy( &multi_stepper );

    U8_TRACE_END_ERR( ctrl_err );
    return ctrl_err;
}

void gui_toolbox_highlight_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    const data_small_set_t *const set_to_be_highlighted = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* do not check if set is empty; gui_toolbox_private_toggle_display_flag_in_set will do this */

    gui_toolbox_private_toggle_display_flag_in_set( this_,
                                                    set_to_be_highlighted,
                                                    DATA_DIAGRAMELEMENT_FLAG_EMPHASIS | DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT
                                                  );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_instantiate_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    const data_small_set_t *const set_to_be_instantiated = gui_marked_set_get_selected_set_const( (*this_).marker );

    /* do not check if set is empty; gui_toolbox_private_toggle_display_flag_in_set will do this */

    gui_toolbox_private_toggle_display_flag_in_set( this_,
                                                    set_to_be_instantiated,
                                                    DATA_DIAGRAMELEMENT_FLAG_NAMED_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE
                                                  );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_reset_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( NULL != this_ );

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    gui_marked_set_clear_selected_set( (*this_).marker );
    //gui_marked_set_clear_focused( (*this_).marker );

    /* trigger redraw */
    gui_toolbox_private_notify_listeners( this_ );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_toolbox_private_toggle_display_flag_in_set( gui_toolbox_t *this_,
                                                     const data_small_set_t *set_to_be_toggled,
                                                     data_diagramelement_flag_t flag_bits_to_toggle )
{
    U8_TRACE_BEGIN();
    u8_error_t error = U8_ERROR_NONE;
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
                U8_LOG_WARNING( "gui_toolbox_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= U8_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                /* program internal error */
                U8_LOG_WARNING( "gui_toolbox_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= U8_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                /* program internal error */
                U8_LOG_WARNING( "gui_toolbox_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= U8_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_DIAGRAMELEMENT:
            {
                data_diagramelement_t out_diagramelement;
                data_row_t diag_elem_id = data_id_get_row_id( &current_id );
                ctrl_diagram_controller_t *diag_ctrl;
                diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

                error |= (u8_error_t) data_database_reader_get_diagramelement_by_id ( (*this_).db_reader,
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
                U8_LOG_WARNING( "gui_toolbox_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= U8_ERROR_INVALID_REQUEST;
            }
            break;

            default:
            {
                /* program internal error */
                U8_LOG_ERROR( "gui_toolbox_private_toggle_display_flag_in_set fould illegal data_table_t enum value." );
            }
            break;
        }
    }

    if ( error != U8_ERROR_NONE )
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

    U8_TRACE_END();
}

void gui_toolbox_undo_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( this_ != NULL );
    u8_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init( &stat );

    ctrl_err = ctrl_controller_undo( (*this_).controller, &stat );

    /* find a diagram that can show the changes */
    if ( U8_ERROR_NONE == ctrl_err )
    {
        ctrl_undo_redo_iterator_t iter;
        ctrl_undo_redo_iterator_init_empty( &iter );
        ctrl_err |= ctrl_controller_get_redo_iterator( (*this_).controller, &iter );
        gui_toolbox_private_show_changes( this_, &iter );
        ctrl_undo_redo_iterator_destroy( &iter );
    }

    /* show error/success message to the user */
    if ( U8_ERROR_INVALID_REQUEST == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_UNDO
                                               );
    }
    else if ( U8_ERROR_ARRAY_BUFFER_EXCEEDED == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_UNDO_NOT_POSSIBLE
                                               );
    }
    else
    {
        /* success */
        gui_simple_message_to_user_show_message_with_stat( (*this_).message_to_user,
                                                           GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                           GUI_SIMPLE_MESSAGE_CONTENT_UNDO,
                                                           &stat
                                                         );
    }

    data_stat_destroy( &stat );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

gboolean gui_toolbox_undo_shortcut_callback( GtkWidget* widget, GVariant* args, gpointer user_data )
{
    gui_toolbox_undo_btn_callback( widget, user_data );
    return TRUE;
}

void gui_toolbox_redo_btn_callback( GtkWidget* button, gpointer data )
{
    U8_TRACE_BEGIN();
    gui_toolbox_t *this_ = data;
    assert( this_ != NULL );
    u8_error_t ctrl_err;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    data_stat_t stat;
    data_stat_init( &stat );

    ctrl_err = ctrl_controller_redo( (*this_).controller, &stat );

    /* find a diagram that can show the changes */
    if ( U8_ERROR_NONE == ctrl_err )
    {
        ctrl_undo_redo_iterator_t iter;
        ctrl_undo_redo_iterator_init_empty( &iter );
        ctrl_err |= ctrl_controller_get_undo_iterator( (*this_).controller, &iter );
        gui_toolbox_private_show_changes( this_, &iter );
        ctrl_undo_redo_iterator_destroy( &iter );
    }

    /* show error/success message to the user */
    if ( U8_ERROR_INVALID_REQUEST == ctrl_err )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_MORE_REDO
                                               );
    }
    else
    {
        /* success */
        gui_simple_message_to_user_show_message_with_stat( (*this_).message_to_user,
                                                           GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                           GUI_SIMPLE_MESSAGE_CONTENT_REDO,
                                                           &stat
                                                         );
    }

    data_stat_destroy( &stat );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

/*!
 *  \brief callback that informs that the redo shortcut was activated
 */
gboolean gui_toolbox_redo_shortcut_callback( GtkWidget* widget, GVariant* args, gpointer user_data )
{
    gui_toolbox_redo_btn_callback( widget, user_data );
    return TRUE;
}

void gui_toolbox_private_show_changes( gui_toolbox_t *this_,
                                       ctrl_undo_redo_iterator_t *action_iterator )
{
    U8_TRACE_BEGIN();
    data_id_t display_diag_nav = DATA_ID_VOID;  /* display this diagram in navigation mode, prio 1 */
    data_id_t display_diag_edit = DATA_ID_VOID;  /* display this diagram in edit mode, prio 2 */
    data_id_t display_classifier = DATA_ID_VOID;  /* display this classifier somehow, prio 3 */

    while( ctrl_undo_redo_iterator_has_next( action_iterator ) )
    {
        const ctrl_undo_redo_entry_t *const probe = ctrl_undo_redo_iterator_next( action_iterator );

        switch( ctrl_undo_redo_entry_get_action_type( probe ) )
        {
            case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM:
            {
                const data_diagram_t *const diag
                    = ctrl_undo_redo_entry_get_diagram_before_action_const( probe );
                assert( data_diagram_is_valid( diag ) );
                display_diag_nav = data_diagram_get_parent_data_id( diag );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM:
            {
                const data_diagram_t *const diag
                    = ctrl_undo_redo_entry_get_diagram_after_action_const( probe );
                assert( data_diagram_is_valid( diag ) );
                display_diag_nav = data_diagram_get_parent_data_id( diag );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM:
            {
                const data_diagram_t *const diag
                    = ctrl_undo_redo_entry_get_diagram_after_action_const( probe );
                assert( data_diagram_is_valid( diag ) );
                display_diag_edit = data_diagram_get_data_id( diag );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT:
            {
                const data_diagramelement_t *const diagele
                    = ctrl_undo_redo_entry_get_diagramelement_before_action_const( probe );
                assert( data_diagramelement_is_valid( diagele ) );
                display_diag_edit = data_diagramelement_get_diagram_data_id( diagele );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT:  /* ... or ... */
            case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT:
            {
                const data_diagramelement_t *const diagele
                    = ctrl_undo_redo_entry_get_diagramelement_after_action_const( probe );
                assert( data_diagramelement_is_valid( diagele ) );
                display_diag_edit = data_diagramelement_get_diagram_data_id( diagele );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER:  /* ... or ... */
            case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER:
            {
                /* Nothing to do: When a classifier is created or deleted, also */
                /* a diagramelement is created or deleted. */
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER:
            {
                const data_classifier_t *const classifier
                    = ctrl_undo_redo_entry_get_classifier_after_action_const( probe );
                assert( data_classifier_is_valid( classifier ) );
                display_classifier = data_classifier_get_data_id( classifier );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE:
            {
                const data_feature_t *const feature
                    = ctrl_undo_redo_entry_get_feature_before_action_const( probe );
                assert( data_feature_is_valid( feature ) );
                display_classifier = data_feature_get_classifier_data_id( feature );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE:  /* ... or ... */
            case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE:
            {
                const data_feature_t *const feature
                    = ctrl_undo_redo_entry_get_feature_after_action_const( probe );
                assert( data_feature_is_valid( feature ) );
                display_classifier = data_feature_get_classifier_data_id( feature );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP:
            {
                const data_relationship_t *const relationship
                    = ctrl_undo_redo_entry_get_relationship_before_action_const( probe );
                assert( data_relationship_is_valid( relationship ) );
                display_classifier = data_relationship_get_from_classifier_data_id( relationship );
            }
            break;

            case CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP:  /* ... or ... */
            case CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP:
            {
                const data_relationship_t *const relationship
                    = ctrl_undo_redo_entry_get_relationship_after_action_const( probe );
                assert( data_relationship_is_valid( relationship ) );
                display_classifier = data_relationship_get_from_classifier_data_id( relationship );
            }
            break;

            default:  /* ... or ... */
            case CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY:
            {
                U8_LOG_WARNING( "gui_toolbox_private_show_changes runs into unexpected switch-case." );
                assert( false );
            }
            break;
        }
    }

    if ( data_id_is_valid( &display_diag_nav ) )
    {
        gui_toolbox_set_selected_tool_and_focus( this_, GUI_TOOL_NAVIGATE, display_diag_nav );
    }
    else if ( data_id_is_valid( &display_diag_edit ) )
    {
        gui_toolbox_set_selected_tool_and_focus( this_, GUI_TOOL_EDIT, display_diag_edit );
    }
    else if ( data_id_is_valid( &display_classifier ) )
    {
        /* try to find ANY diagram that MAY show the modified object: */
        assert( data_id_get_table( &display_classifier ) == DATA_TABLE_CLASSIFIER );
        data_small_set_t diagram_ids;
        data_small_set_init( &diagram_ids );
        const u8_error_t d_err
            =  data_database_reader_get_diagram_ids_by_classifier_id( (*this_).db_reader,
                                                                      data_id_get_row_id( &display_classifier ),
                                                                      &diagram_ids
                                                                    );
        if (( d_err == U8_ERROR_NONE )||( d_err == U8_ERROR_ARRAY_BUFFER_EXCEEDED ))
        {
            if ( data_small_set_contains( &diagram_ids, gui_marked_set_get_focused_diagram( (*this_).marker ) ) )
            {
                /* the right diagram is already shown, just update the tool */
                gui_toolbox_set_selected_tool( this_, GUI_TOOL_EDIT );
            }
            else if ( ! data_small_set_is_empty( &diagram_ids ) )
            {
                gui_toolbox_set_selected_tool_and_focus( this_, GUI_TOOL_EDIT, data_small_set_get_id( &diagram_ids, 0 ) );
            }
            else
            {
                U8_LOG_ANOMALY( "gui_toolbox_private_show_changes found a classifier without a diagram." );
            }
        }
        data_small_set_destroy( &diagram_ids );
    }
    else
    {
        U8_LOG_ANOMALY( "gui_toolbox_private_show_changes did not find a diagram to show the changes." );
    }

    U8_TRACE_END();
}

void gui_toolbox_private_notify_listeners( gui_toolbox_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_INFO( "g_signal_emit to listeners" );
    g_signal_emit( (*this_).toolbar, gui_toolbox_glib_signal_id, 0, (*this_).selected_tool );

    U8_TRACE_END();
}


/*
Copyright 2016-2025 Andreas Warnke

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
