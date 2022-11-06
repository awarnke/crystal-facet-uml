/* File: gui_search_request.c; Copyright and License: see below */

#include "gui_search_request.h"
#include "set/data_full_id.h"
#include "data_id.h"
#include "utf8stringbuf/utf8string.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <assert.h>

void gui_search_request_init ( gui_search_request_t *this_,
                               GtkWidget *search_label,
                               GtkWidget *search_entry,
                               GtkWidget *search_button,
                               gui_marked_set_t *marked_set,
                               gui_search_runner_t *search_runner )
{
    TRACE_BEGIN();
    assert ( search_label != NULL );
    assert ( search_entry != NULL );
    assert ( search_button != NULL );
    assert ( marked_set != NULL );
    assert ( search_runner != NULL );

    (*this_).search_label = search_label;
    (*this_).search_entry = search_entry;
    (*this_).search_button = search_button;
    (*this_).marked_set = marked_set;
    (*this_).search_runner = search_runner;

    TRACE_END();
}

void gui_search_request_destroy ( gui_search_request_t *this_ )
{
    TRACE_BEGIN();

    (*this_).search_label = NULL;
    (*this_).search_entry = NULL;
    (*this_).search_button = NULL;
    (*this_).marked_set = NULL;
    (*this_).search_runner = NULL;

    TRACE_END();
}

void gui_search_request_show ( gui_search_request_t *this_ )
{
    TRACE_BEGIN();

    gtk_widget_show( GTK_WIDGET ( (*this_).search_label ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).search_entry ) );
    gtk_widget_show( GTK_WIDGET ( (*this_).search_button ) );

    TRACE_END();
}

void gui_search_request_hide ( gui_search_request_t *this_ )
{
    TRACE_BEGIN();

    gtk_widget_hide( GTK_WIDGET ( (*this_).search_label ) );
    gtk_widget_hide( GTK_WIDGET ( (*this_).search_entry ) );
    gtk_widget_hide( GTK_WIDGET ( (*this_).search_button ) );

    TRACE_END();
}

void gui_search_request_tool_changed_callback( GtkWidget *widget, gui_tool_t tool, gpointer data )
{
    TRACE_BEGIN();
    gui_search_request_t *this_ = data;
    assert( NULL != this_ );

    /* info: This function is called once for activating a tool and once for deactiaving it! */

    switch ( tool )
    {
        case GUI_TOOL_NAVIGATE:
        {
            TRACE_INFO("GUI_TOOL_NAVIGATE");
            gui_search_request_hide( this_ );
        }
        break;

        case GUI_TOOL_EDIT:
        {
            TRACE_INFO("GUI_TOOL_EDIT");
            gui_search_request_hide( this_ );
        }
        break;

        case GUI_TOOL_SEARCH:
        {
            TRACE_INFO("GUI_TOOL_SEARCH");
            gui_search_request_show( this_ );
        }
        break;

        case GUI_TOOL_CREATE:
        {
            TRACE_INFO("GUI_TOOL_CREATE");
            gui_search_request_hide( this_ );
        }
        break;

        default:
        {
            TSLOG_ERROR("selected_tool is out of range");
        }
        break;
    }

    TRACE_END();
}

void gui_search_request_search_start_callback( GtkWidget* trigger_widget, gpointer data )
{
    TRACE_BEGIN();
    gui_search_request_t *this_ = data;
    assert( NULL != this_ );
    /* note: button may bei either the text entry widget or the search button widget */

    const char* text;
#if ( GTK_MAJOR_VERSION >= 4 )
    GtkEntryBuffer *const search_buf = gtk_entry_get_buffer( GTK_ENTRY( (*this_).search_entry ) );
    text = gtk_entry_buffer_get_text( search_buf );
#else
    text = gtk_entry_get_text( GTK_ENTRY( (*this_).search_entry ) );
#endif

    if ( text != NULL )
    {
        gui_search_runner_run ( (*this_).search_runner, text );
    }
    else
    {
        assert(false);
    }

    TRACE_END();
}

void gui_search_request_id_search_callback ( GtkWidget *widget, gpointer user_data )
{
    TRACE_BEGIN();
    gui_search_request_t *this_;
    this_ = (gui_search_request_t*) user_data;
    assert ( NULL != this_ );

    data_full_id_t focused_id = gui_marked_set_get_focused ( (*this_).marked_set );
    if ( data_full_id_is_valid( &focused_id ) )
    {
        /* get the primary id unless it is a DIAGRAMELEMENT, then take the secondary id */
        const data_id_t *const vis_id = data_full_id_get_primary_id_ptr( &focused_id );
        const data_id_t *const model_id
            = (DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( vis_id ))
            ? data_full_id_get_secondary_id_ptr( &focused_id )
            : vis_id;

        char focused_id_buf[DATA_ID_MAX_UTF8STRING_LENGTH] = "";
        utf8stringbuf_t focused_id_str = UTF8STRINGBUF(focused_id_buf);
        const utf8error_t id_err = data_id_to_utf8stringbuf( model_id, focused_id_str );
        if ( id_err == UTF8ERROR_SUCCESS )
        {
#if ( GTK_MAJOR_VERSION >= 4 )
            GtkEntryBuffer *const name_buf = gtk_entry_get_buffer( GTK_ENTRY( (*this_).search_entry ) );
            gtk_entry_buffer_set_text( name_buf, utf8stringbuf_get_string( focused_id_str ), -1 /* = n_chars */ );
#else
            gtk_entry_set_text( GTK_ENTRY ( (*this_).search_entry ), utf8stringbuf_get_string( focused_id_str ) );
#endif
            gui_search_runner_run ( (*this_).search_runner, utf8stringbuf_get_string( focused_id_str ) );
        }
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_search_request_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    assert( NULL != msg );
    gui_search_request_t *this_ = user_data;
    assert( NULL != this_ );
    assert ( NULL != widget );

    data_change_event_type_t evt_type;
    evt_type = data_change_message_get_event ( msg );

    if ( evt_type == DATA_CHANGE_EVENT_TYPE_DB_OPENED )
    {
#if ( GTK_MAJOR_VERSION >= 4 )
        GtkEntryBuffer *const search_buf = gtk_entry_get_buffer( GTK_ENTRY( (*this_).search_entry ) );
        gtk_entry_buffer_set_text ( search_buf, "", 0 /* = n_chars */ );
#else
        gtk_entry_set_text ( GTK_ENTRY( (*this_).search_entry ), "" );
#endif
    }

    TRACE_END();
}


/*
Copyright 2019-2022 Andreas Warnke

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
