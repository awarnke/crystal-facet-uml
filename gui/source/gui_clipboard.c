/* File: gui_clipboard.c; Copyright and License: see below */

#include "gui_clipboard.h"
#include "gui_toolbox.h"
#include "gui_tool.h"
#include "u8/u8_trace.h"
#include "u8/u8_error.h"
#include "set/data_stat.h"
#include "utf8stringbuf/utf8string.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void gui_clipboard_init ( gui_clipboard_t *this_,
                          GdkClipboard *clipboard,
                          gui_toolbox_t *tool_switcher,
                          gui_simple_message_to_user_t *message_to_user,
                          data_database_reader_t *db_reader,
                          ctrl_controller_t *controller )
{
    U8_TRACE_BEGIN();
    assert( NULL != clipboard );
    assert( NULL != message_to_user );
    assert( NULL != db_reader );
    assert( NULL != controller );

    (*this_).destination_diagram_id = DATA_ROW_VOID;
    (*this_).message_to_user = message_to_user;
    (*this_).tool_switcher = tool_switcher;
    (*this_).the_clipboard = clipboard;
    (*this_).clipboard_stringbuf = utf8stringbuf_new( (*this_).private_clipboard_buffer,
                                                      sizeof((*this_).private_clipboard_buffer)
                                                    );

    io_exporter_light_init ( &((*this_).exporter), db_reader );
    io_importer_init ( &((*this_).importer), db_reader, controller );

    U8_TRACE_END();
}

void gui_clipboard_destroy ( gui_clipboard_t *this_ )
{
    U8_TRACE_BEGIN();

    io_exporter_light_destroy ( &((*this_).exporter) );
    io_importer_destroy ( &((*this_).importer) );

    (*this_).the_clipboard = NULL;
    (*this_).tool_switcher = NULL;
    (*this_).message_to_user = NULL;

    U8_TRACE_END();
}

int gui_clipboard_copy_set_to_clipboard( gui_clipboard_t *this_, const data_small_set_t *set_to_be_copied, data_stat_t *io_stat )
{
    U8_TRACE_BEGIN();
    assert( NULL != set_to_be_copied );
    assert( NULL != io_stat );
    int serialize_error;

    serialize_error = io_exporter_light_export_set_to_buf( &((*this_).exporter),
                                                           set_to_be_copied,
                                                           io_stat,
                                                           (*this_).clipboard_stringbuf
                                                         );

    if ( serialize_error == 0 )
    {
        gdk_clipboard_set_text( (*this_).the_clipboard, utf8stringbuf_get_string( &((*this_).clipboard_stringbuf) ) );
    }
    else
    {
        U8_LOG_ERROR_HEX( "Exporting selected set to clipboard failed:", serialize_error );
    }
    U8_TRACE_INFO( utf8stringbuf_get_string( &((*this_).clipboard_stringbuf) ) );

    U8_TRACE_END_ERR( serialize_error );
    return serialize_error;
}

void gui_clipboard_request_clipboard_text( gui_clipboard_t *this_, data_row_t destination_diagram_id )
{
    U8_TRACE_BEGIN();

    utf8stringbuf_clear( &((*this_).clipboard_stringbuf) );

    (*this_).destination_diagram_id = destination_diagram_id;
    U8_TRACE_INFO_INT ( "(*this_).destination_diagram_id:", destination_diagram_id );

    gdk_clipboard_read_text_async( (*this_).the_clipboard,
                                   NULL,  /* GCancellable* cancellable */
                                   (GAsyncReadyCallback) gui_clipboard_clipboard_text_received_callback,
                                   this_
                                 );

    U8_TRACE_END();
}

void gui_clipboard_clipboard_text_received_callback( GObject *source_object,
                                                     GAsyncResult* res,
                                                     gpointer user_data )
{
    U8_TRACE_BEGIN();
    assert( NULL != source_object );
    assert( NULL != res );
    assert( NULL != user_data );
    gui_clipboard_t *this_ = user_data;
    assert( GDK_CLIPBOARD(source_object) == (*this_).the_clipboard );

    GError* error = NULL;
    char* clipboard_text = gdk_clipboard_read_text_finish( (*this_).the_clipboard, res, &error );

    if ( error != NULL )
    {
        U8_LOG_ERROR_STR( "Error:", (*error).message );
        g_error_free( error );
    }

    if ( clipboard_text != NULL )
    {
        gui_clipboard_private_copy_clipboard_to_db( this_, clipboard_text );
        /* g_object_unref( clipboard_text ); */
        g_free( clipboard_text );
    }
    else
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_INPUT_DATA
                                               );
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}

void gui_clipboard_private_copy_clipboard_to_db( gui_clipboard_t *this_, const char *json_text )
{
    U8_TRACE_BEGIN();
    assert( NULL != json_text );
    U8_TRACE_INFO_INT ( "(*this_).destination_diagram_id:", (*this_).destination_diagram_id );

    /* import the clipboard */
    u8_error_t parse_error;
    data_stat_t stat;
    data_stat_init( &stat );
    u8_error_info_t err_info;
    parse_error = io_importer_import_clipboard( &((*this_).importer),
                                                json_text,
                                                (*this_).destination_diagram_id,
                                                &stat,
                                                &err_info
                                              );

    /* in case a new diagram was pasted, go to nav mode */
    if ( 0 != data_stat_get_count( &stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_CREATED ) )
    {
        gui_toolbox_set_selected_tool( (*this_).tool_switcher, GUI_TOOL_NAVIGATE );
    }
    else
    {
        gui_toolbox_set_selected_tool( (*this_).tool_switcher, GUI_TOOL_EDIT );
    }

    /* show a message to the user */
    if ( u8_error_info_is_error( &err_info ) )
    {
        gui_simple_message_to_user_show_error_info( (*this_).message_to_user, &err_info );
    }
    else if ( U8_ERROR_NONE != parse_error )
    {
        gui_simple_message_to_user_show_message( (*this_).message_to_user,
                                                 GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_NO_INPUT_DATA
                                               );
    }
    else
    {
        gui_simple_message_to_user_show_message_with_stat( (*this_).message_to_user,
                                                           GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                           GUI_SIMPLE_MESSAGE_CONTENT_PASTE_FROM_CLIPBOARD,
                                                           &stat
                                                         );
    }

    data_stat_destroy( &stat );
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
