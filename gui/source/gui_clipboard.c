/* File: gui_clipboard.c; Copyright and License: see below */

#include "gui_clipboard.h"
#include "trace.h"
#include "json/json_deserializer.h"
#include "ctrl_error.h"
#include "set/data_stat.h"
#include "util/string/utf8string.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void gui_clipboard_init ( gui_clipboard_t *this_,
                          GtkClipboard *clipboard,
                          gui_simple_message_to_user_t *message_to_user,
                          data_database_reader_t *db_reader,
                          ctrl_controller_t *controller )
{
    TRACE_BEGIN();
    assert( NULL != clipboard );
    assert( NULL != message_to_user );
    assert( NULL != db_reader );
    assert( NULL != controller );

    (*this_).destination_diagram_id = DATA_ROW_ID_VOID;
    (*this_).message_to_user = message_to_user;
    (*this_).the_clipboard = clipboard;
    (*this_).clipboard_stringbuf = utf8stringbuf_init( sizeof((*this_).private_clipboard_buffer),
                                                       (*this_).private_clipboard_buffer
                                                     );

    io_exporter_light_init ( &((*this_).exporter), db_reader );
    io_importer_init ( &((*this_).importer), db_reader, controller );

    TRACE_END();
}

void gui_clipboard_destroy ( gui_clipboard_t *this_ )
{
    TRACE_BEGIN();

    io_exporter_light_destroy ( &((*this_).exporter) );
    io_importer_destroy ( &((*this_).importer) );

    (*this_).the_clipboard = NULL;
    (*this_).message_to_user = NULL;

    TRACE_END();
}

int gui_clipboard_copy_set_to_clipboard( gui_clipboard_t *this_, const data_small_set_t *set_to_be_copied, data_stat_t *io_stat )
{
    TRACE_BEGIN();
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
        gtk_clipboard_set_text ( (*this_).the_clipboard, utf8stringbuf_get_string( (*this_).clipboard_stringbuf ), -1 );
    }
    TRACE_INFO( utf8stringbuf_get_string( (*this_).clipboard_stringbuf ) );

    TRACE_END_ERR( serialize_error );
    return serialize_error;
}

void gui_clipboard_request_clipboard_text( gui_clipboard_t *this_, data_row_id_t destination_diagram_id )
{
    TRACE_BEGIN();

    utf8stringbuf_clear( (*this_).clipboard_stringbuf );

    (*this_).destination_diagram_id = destination_diagram_id;
    TRACE_INFO_INT ( "(*this_).destination_diagram_id:", destination_diagram_id );

    /* this more complicated call (compared to gtk_clipboard_wait_for_text) avoids recursive calls of the gdk main loop */
    gtk_clipboard_request_text ( (*this_).the_clipboard,
                                 (GtkClipboardTextReceivedFunc) gui_clipboard_clipboard_text_received_callback,
                                 this_
                               );

    TRACE_END();
}

void gui_clipboard_clipboard_text_received_callback ( GtkClipboard *clipboard, const gchar *clipboard_text, gpointer data )
{
    TRACE_BEGIN();
    assert( NULL != clipboard );
    assert( NULL != data );
    gui_clipboard_t *this_ = data;

    if ( clipboard_text != NULL )
    {
        gui_clipboard_private_copy_clipboard_to_db( this_, clipboard_text );
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

void gui_clipboard_private_copy_clipboard_to_db( gui_clipboard_t *this_, const char *json_text )
{
    TRACE_BEGIN();
    assert( NULL != json_text );
    TRACE_INFO_INT ( "(*this_).destination_diagram_id:", (*this_).destination_diagram_id );

    data_error_t parse_error;
    data_stat_t stat;
    data_stat_init(&stat);
    uint32_t read_err_pos;
    parse_error = io_importer_import_clipboard( &((*this_).importer),
                                                json_text,
                                                (*this_).destination_diagram_id,
                                                &stat,
                                                &read_err_pos
                                              );

    if ( DATA_ERROR_NONE != parse_error )
    {
        gui_simple_message_to_user_show_message_with_line ( (*this_).message_to_user,
                                                            GUI_SIMPLE_MESSAGE_TYPE_ERROR,
                                                            GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA,
                                                            read_err_pos
                                                          );
    }
    else
    {
        gui_simple_message_to_user_show_message_with_stat ( (*this_).message_to_user,
                                                            GUI_SIMPLE_MESSAGE_TYPE_INFO,
                                                            GUI_SIMPLE_MESSAGE_CONTENT_PASTE_FROM_CLIPBOARD,
                                                            &stat
                                                          );
    }

    data_stat_destroy(&stat);
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
