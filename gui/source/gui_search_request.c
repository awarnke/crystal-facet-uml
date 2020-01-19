/* File: gui_search_request.c; Copyright and License: see below */

#include "gui_search_request.h"
#include "util/string/utf8string.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void gui_search_request_init ( gui_search_request_t *this_,
                               GtkWidget *search_label,
                               GtkWidget *search_entry,
                               GtkWidget *search_button,
                               gui_search_runner_t *search_runner )
{
    TRACE_BEGIN();
    assert ( search_label != NULL );
    assert ( search_entry != NULL );
    assert ( search_button != NULL );
    assert ( search_runner != NULL );

    (*this_).search_label = search_label;
    (*this_).search_entry = search_entry;
    (*this_).search_button = search_button;
    (*this_).search_runner = search_runner;

    TRACE_END();
}

void gui_search_request_destroy ( gui_search_request_t *this_ )
{
    TRACE_BEGIN();

    (*this_).search_label = NULL;
    (*this_).search_entry = NULL;
    (*this_).search_button = NULL;
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

    switch ( tool )
    {
        case GUI_TOOLBOX_NAVIGATE:
        {
            TRACE_INFO("GUI_TOOLBOX_NAVIGATE");
            gui_search_request_hide( this_ );
        }
        break;

        case GUI_TOOLBOX_EDIT:
        {
            TRACE_INFO("GUI_TOOLBOX_EDIT");
            gui_search_request_hide( this_ );
        }
        break;

        case GUI_TOOLBOX_SEARCH:
        {
            TRACE_INFO("GUI_TOOLBOX_SEARCH");
            gui_search_request_show( this_ );

            /* re-trigger the last search - otherwise the screen stays empty */
            /* would the sketch area store the result list, old search results would be displayed */
            const char *text = gtk_entry_get_text( GTK_ENTRY( (*this_).search_entry ) );
            if ( text != NULL )
            {
                if ( 0 != utf8string_get_length( text ) )
                {
                    gui_search_runner_run ( (*this_).search_runner, text );
                }
            }
        }
        break;

        case GUI_TOOLBOX_CREATE:
        {
            TRACE_INFO("GUI_TOOLBOX_CREATE");
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
    text = gtk_entry_get_text( GTK_ENTRY( (*this_).search_entry ) );

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

void gui_search_request_data_changed_callback( GtkWidget *widget, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    TRACE_INFO("callback not yet implemented.");
    TRACE_END();
}


/*
Copyright 2019-2020 Andreas Warnke

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
