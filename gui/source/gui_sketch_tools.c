/* File: gui_sketch_tools.c; Copyright and License: see below */

#include "gui_sketch_tools.h"
#include "trace.h"
#include "serial/data_json_serializer.h"
#include "serial/data_json_deserializer.h"
#include "ctrl_error.h"
#include "util/string/utf8string.h"
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
                             GtkClipboard *clipboard,
                             gui_sketch_marker_t *marker,
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

    (*this_).selected_tool = GUI_SKETCH_TOOLS_NAVIGATE;
    (*this_).listener = NULL;
    (*this_).marker = marker;
    (*this_).message_to_user = message_to_user;
    (*this_).db_reader = db_reader;
    (*this_).controller = controller;
    (*this_).tool_navigate = tool_navigate;
    (*this_).tool_edit = tool_edit;
    (*this_).tool_new_obj = tool_new_obj;
    (*this_).tool_new_view = tool_new_view;
    (*this_).the_clipboard = clipboard;
    (*this_).clipboard_stringbuf = utf8stringbuf_init( sizeof((*this_).private_clipboard_buffer),
                                                       (*this_).private_clipboard_buffer );

    gui_serializer_deserializer_init ( &((*this_).serdes),
                                       clipboard,
                                       marker,
                                       message_to_user,
                                       db_reader,
                                       controller
                                     );

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
    data_small_set_t *set_to_be_cut;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_cut = gui_sketch_marker_get_selected_set_ptr( (*this_).marker );

    /* do not check if set is empty; gui_serializer_deserializer_copy_set_to_clipboard will do this */

    gui_serializer_deserializer_copy_set_to_clipboard( &((*this_).serdes), set_to_be_cut );

    gui_sketch_tools_private_delete_set( this_, set_to_be_cut );

    gui_sketch_marker_clear_selected_set( (*this_).marker );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_copy_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
    data_small_set_t *set_to_be_copied;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_copied = gui_sketch_marker_get_selected_set_ptr( (*this_).marker );

    if ( data_small_set_is_empty( set_to_be_copied ) )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
        );
        /* continue nonetheless, it is possible to copy an empty set to the clipboard */
    }

    gui_serializer_deserializer_copy_set_to_clipboard( &((*this_).serdes), set_to_be_copied );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_paste_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    utf8stringbuf_clear( (*this_).clipboard_stringbuf );

    /* this more complicated call (compared to gtk_clipboard_wait_for_text) avoids recursive calls of the gdk main loop */
    gtk_clipboard_request_text ( (*this_).the_clipboard, (GtkClipboardTextReceivedFunc) gui_sketch_tools_clipboard_text_received_callback, this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}


void gui_sketch_tools_clipboard_text_received_callback ( GtkClipboard *clipboard, const gchar *clipboard_text, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;

    if ( clipboard_text != NULL )
    {
        gui_sketch_tools_private_copy_clipboard_to_db( this_, clipboard_text );
    }
    else
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_INPUT_DATA
        );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_private_copy_clipboard_to_db( gui_sketch_tools_t *this_, const char *json_text )
{
    TRACE_BEGIN();
    int64_t focused_diagram;

    focused_diagram = gui_sketch_marker_get_focused_diagram( (*this_).marker );

    data_error_t diag_check_error;
    static data_diagram_t test_diagram;  /* unsynchronized - but it is just a dummy ... */
    diag_check_error = data_database_reader_get_diagram_by_id ( (*this_).db_reader, focused_diagram, &test_diagram );
    if ( DATA_ERROR_NONE == diag_check_error )
    {
        TRACE_INFO_INT ( "focused_diagram:", focused_diagram );
        gui_sketch_tools_private_copy_clipboard_to_diagram( this_, json_text, focused_diagram );
    }
    else
    {
        TSLOG_WARNING_INT ( "focused_diagram is invalid:", focused_diagram );
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_SELECTION
        );
    }

    TRACE_END();
}

void gui_sketch_tools_private_copy_clipboard_to_diagram( gui_sketch_tools_t *this_, const char *json_text, int64_t diagram_id )
{
    TRACE_BEGIN();
    data_json_deserializer_t deserializer;
    data_error_t parse_error = DATA_ERROR_NONE;

    TRACE_INFO ( json_text );

    data_json_deserializer_init( &deserializer, json_text );

    parse_error = data_json_deserializer_expect_begin_set( &deserializer );

    if ( DATA_ERROR_NONE == parse_error )
    {
        data_table_t next_object_type;
        bool set_end = false;  /* end of data set reached or error at parsing */
        bool is_first = true;  /* is this the first object in the database that is created? if false, database changes are appended to the last undo_redo_set */
        static const uint32_t MAX_LOOP_COUNTER = (CTRL_UNDO_REDO_LIST_MAX_SIZE/2)-2;  /* no not import more things than can be undone */
        for ( int count = 0; ( ! set_end ) && ( count < MAX_LOOP_COUNTER ); count ++ )
        {
            parse_error = data_json_deserializer_get_type_of_next_element( &deserializer, &next_object_type );
            if ( DATA_ERROR_NONE == parse_error )
            {
                switch ( next_object_type )
                {
                    case DATA_TABLE_VOID:
                    {
                        /* we are finished */
                        set_end = true;
                    }
                    break;

                    case DATA_TABLE_CLASSIFIER:
                    {
                        data_classifier_t new_classifier;
                        parse_error = data_json_deserializer_get_next_classifier ( &deserializer, &new_classifier );
                        if ( DATA_ERROR_NONE != parse_error )
                        {
                            /* parser error, break loop: */
                            set_end = true;
                        }
                        else
                        {
                            /* create classifier if not yet existing */
                            int64_t the_classifier_id;
                            {
                                /* check if the parsed classifier already exists in this database; if not, create it */
                                ctrl_classifier_controller_t *classifier_ctrl;
                                classifier_ctrl = ctrl_controller_get_classifier_control_ptr( (*this_).controller );

                                data_error_t read_error;
                                data_classifier_t existing_classifier;
                                read_error = data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                                        data_classifier_get_id( &new_classifier ),
                                                                                        &existing_classifier
                                );
                                bool classifier_exists = false;
                                if ( DATA_ERROR_NONE == read_error )
                                {
                                    /* if the name is equal, expect the objects to be equal */
                                    if ( utf8string_equals_str( data_classifier_get_name_ptr( &new_classifier ),
                                                                data_classifier_get_name_ptr( &existing_classifier ) ) )
                                    {
                                        classifier_exists = true;
                                    }
                                }
                                if ( ! classifier_exists )
                                {
                                    ctrl_error_t write_error;
                                    write_error = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                                                                &new_classifier,
                                                                                                ! is_first,
                                                                                                &the_classifier_id
                                    );
                                    if ( CTRL_ERROR_DUPLICATE_NAME == write_error )
                                    {
                                        gui_simple_message_to_user_show_message_with_string( (*this_).message_to_user,
                                                                                             GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                                                             GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE,
                                                                                             data_classifier_get_name_ptr( &new_classifier )
                                        );
                                        set_end = true;
                                        parse_error = DATA_ERROR_DUPLICATE_NAME;
                                    }
                                    else if ( CTRL_ERROR_NONE != write_error )
                                    {
                                        TSLOG_ERROR( "unexpected error" );
                                        set_end = true;
                                    }
                                    is_first = false;
                                }
                                else
                                {
                                    the_classifier_id = data_classifier_get_id( &existing_classifier );
                                }
                            }

                            if ( ! set_end )  /* no error */
                            {
                                /* link the classifier to the current diagram */
                                ctrl_diagram_controller_t *diag_ctrl;
                                diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

                                ctrl_error_t write_error2;
                                int64_t new_element_id;
                                data_diagramelement_t diag_ele;
                                data_diagramelement_init_new( &diag_ele, diagram_id, the_classifier_id, DATA_DIAGRAMELEMENT_FLAG_NONE );
                                write_error2 = ctrl_diagram_controller_create_diagramelement( diag_ctrl,
                                                                                            &diag_ele,
                                                                                            ! is_first,
                                                                                            &new_element_id
                                );
                                if ( CTRL_ERROR_NONE != write_error2 )
                                {
                                    TSLOG_ERROR( "unexpected error" );
                                    set_end = true;
                                }
                                is_first = false;
                            }
                        }
                    }
                    break;

                    case DATA_TABLE_DIAGRAM:
                    {
                        data_diagram_t new_diagram;
                        parse_error = data_json_deserializer_get_next_diagram ( &deserializer, &new_diagram );
                        if ( DATA_ERROR_NONE != parse_error )
                        {
                            /* parser error, break loop: */
                            set_end = true;
                        }
                        else
                        {
                            /* create the parsed diagram as child below the current diagram */
                            ctrl_diagram_controller_t *diag_ctrl;
                            diag_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

                            ctrl_error_t write_error3;
                            int64_t new_diag_id;
                            data_diagram_set_parent_id( &new_diagram, diagram_id );
                            write_error3 = ctrl_diagram_controller_create_diagram( diag_ctrl,
                                                                                   &new_diagram,
                                                                                   ! is_first,
                                                                                   &new_diag_id
                            );
                            if ( CTRL_ERROR_NONE != write_error3 )
                            {
                                TSLOG_ERROR( "unexpected error" );
                                set_end = true;
                            }

                            is_first = false;
                        }
                    }
                    break;

                    default:
                    {
                        TSLOG_ERROR( "unexpected error" );
                        set_end = true;
                    }
                }
            }
            else
            {
                /* parser error, break loop: */
                set_end = true;
            }
        }
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = data_json_deserializer_expect_end_set( &deserializer );
    }

    data_json_deserializer_destroy( &deserializer );

    if ( DATA_ERROR_DUPLICATE_NAME == parse_error )
    {
        /* error message is already displayed */
    }
    else if ( DATA_ERROR_NONE != parse_error )
    {
        uint32_t read_pos;
        data_json_deserializer_get_read_pos ( &deserializer, &read_pos );
        gui_simple_message_to_user_show_message_with_int( (*this_).message_to_user,
                                                          GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                          GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA,
                                                          read_pos
        );
    }

    TRACE_END();
}

void gui_sketch_tools_delete_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
    data_small_set_t *set_to_be_deleted;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_deleted = gui_sketch_marker_get_selected_set_ptr( (*this_).marker );

    /* do not check if set is empty; gui_sketch_tools_private_delete_set will do this */

    gui_sketch_tools_private_delete_set( this_, set_to_be_deleted );

    gui_sketch_marker_clear_selected_set( (*this_).marker );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_private_delete_set( gui_sketch_tools_t *this_, data_small_set_t *set_to_be_deleted )
{
    TRACE_BEGIN();
    ctrl_error_t ctrl_err;
    ctrl_classifier_controller_t *c_controller;

    c_controller = ctrl_controller_get_classifier_control_ptr ( (*this_).controller );
    ctrl_err = ctrl_classifier_controller_delete_set ( c_controller, *set_to_be_deleted );
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

void gui_sketch_tools_highlight_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
    ctrl_error_t ctrl_err;

    data_small_set_t *set_to_be_highlighted;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_highlighted = gui_sketch_marker_get_selected_set_ptr( (*this_).marker );

    /* do not check if set is empty; gui_sketch_tools_private_toggle_display_flag_in_set will do this */

    gui_sketch_tools_private_toggle_display_flag_in_set( this_, set_to_be_highlighted, DATA_DIAGRAMELEMENT_FLAG_EMPHASIS | DATA_DIAGRAMELEMENT_FLAG_GREY_OUT );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_instantiate_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
    ctrl_error_t ctrl_err;

    data_small_set_t *set_to_be_instantiated;

    gui_simple_message_to_user_hide( (*this_).message_to_user );

    set_to_be_instantiated = gui_sketch_marker_get_selected_set_ptr( (*this_).marker );

    /* do not check if set is empty; gui_sketch_tools_private_toggle_display_flag_in_set will do this */

    gui_sketch_tools_private_toggle_display_flag_in_set( this_, set_to_be_instantiated, DATA_DIAGRAMELEMENT_FLAG_INSTANCE );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_sketch_tools_private_toggle_display_flag_in_set( gui_sketch_tools_t *this_, data_small_set_t *set_to_be_toggled, data_diagramelement_flag_t flag_bits_to_toggle )
{
    TRACE_BEGIN();
    ctrl_error_t error = CTRL_ERROR_NONE;
    bool new_pattern_initialized = false;
    data_diagramelement_flag_t new_pattern;
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
                TSLOG_WARNING( "gui_sketch_tools_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_FEATURE:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_sketch_tools_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            case DATA_TABLE_RELATIONSHIP:
            {
                /* program internal error */
                TSLOG_WARNING( "gui_sketch_tools_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
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
                    new_pattern = 0;
                    for ( int bit = 0; bit < 8*sizeof(data_diagramelement_flag_t); bit ++ )
                    {
                        data_diagramelement_flag_t probe = (1 << bit);
                        if ( 0 != ( probe & flag_bits_to_toggle ) )
                        {
                            /* this is a relevant bit */
                            if ( 0 != ( probe & current_flags ) )
                            {
                                new_pattern = 0;
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

                current_flags = current_flags & (~flag_bits_to_toggle) | new_pattern;

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
                TSLOG_WARNING( "gui_sketch_tools_private_toggle_display_flag_in_set cannot toggle display flags in non-diagramelements." );
                error |= CTRL_ERROR_INVALID_REQUEST;
            }
            break;

            default:
            {
                /* program internal error */
                TSLOG_ERROR( "gui_sketch_tools_private_toggle_display_flag_in_set fould illegal data_table_t enum value." );
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

void gui_sketch_tools_undo_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
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

void gui_sketch_tools_redo_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_sketch_tools_t *this_ = data;
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
