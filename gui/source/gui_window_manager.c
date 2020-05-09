/* File: gui_window_manager.c; Copyright and License: see below */

#include "gui_window_manager.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_window_manager_init ( gui_window_manager_t *this_, ctrl_controller_t *controller, data_database_t *database )
{
    TRACE_BEGIN();

    gui_resources_init( &((*this_).gui_resources) );
    data_database_reader_init( &((*this_).db_reader), database );
    (*this_).controller = controller;
    (*this_).database = database;
    observer_init( &((*this_).window_close_observer), this_, (void (*)(void*,void*)) &gui_window_manager_close_main_window, "gui_window_manager_close_main_window()" );
    observer_init( &((*this_).window_open_observer), this_, (void (*)(void*,void*)) &gui_window_manager_open_main_window2, "gui_window_manager_open_main_window2()" );
    for( int index = 0; index < GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS; index ++ )
    {
        (*this_).main_window_active[index] = false;
    }

    TRACE_END();
}

void gui_window_manager_destroy( gui_window_manager_t *this_ )
{
    TRACE_BEGIN();

    for( int index = 0; index < GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS; index ++ )
    {
        if ( (*this_).main_window_active[index] )
        {
            gui_main_window_destroy( &((*this_).main_window[index]) );
            (*this_).main_window_active[index] = false;
        }
    }
    observer_destroy( &((*this_).window_close_observer) );
    data_database_reader_destroy( &((*this_).db_reader) );
    gui_resources_destroy( &((*this_).gui_resources) );

    TRACE_END();
}

gui_main_window_t *gui_window_manager_open_main_window( gui_window_manager_t *this_ )
{
    TRACE_BEGIN();
    gui_main_window_t *result;

    int pos = -1;
    for( int index = 0; index < GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS; index ++ )
    {
        if ( ! (*this_).main_window_active[index] )
        {
            pos = index;
        }
    }

    if ( -1 != pos )
    {
        gui_main_window_init( &((*this_).main_window[pos]),
                              (*this_).controller,
                              (*this_).database,
                              &((*this_).db_reader),
                              &((*this_).gui_resources),
                              &((*this_).window_close_observer),
                              &((*this_).window_open_observer)
                            );
        (*this_).main_window_active[pos] = true;
        TRACE_INFO_INT( "main_window[] index:", pos );
        result = &((*this_).main_window[pos]);
    }
    else
    {
        TSLOG_ERROR_INT( "Maximum number of windows already open.", GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS );
        result = NULL;
    }

    TRACE_END();
    return result;
}

void gui_window_manager_close_main_window( gui_window_manager_t *this_, gui_main_window_t *main_window )
{
    TRACE_BEGIN();
    int count_active = 0;
    int count_closed = 0;

    for( int index = 0; index < GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS; index ++ )
    {
        if ( (*this_).main_window_active[index] )
        {
            if ( main_window == &((*this_).main_window[index]) )
            {
                gui_main_window_destroy( &((*this_).main_window[index]) );
                (*this_).main_window_active[index] = false;
                TRACE_INFO_INT( "main_window[] index:", index );
                count_closed ++;
            }
            else
            {
                count_active ++;
            }
        }
    }

    if ( count_closed == 0 )
    {
        TSLOG_ERROR( "no window of given address found" );
    }
    if ( count_active == 0 )
    {
        gtk_main_quit();
    }

    TRACE_END();
}

void gui_window_manager_open_main_window2( gui_window_manager_t *this_, gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();

    gui_main_window_t *new_win;
    new_win = gui_window_manager_open_main_window( this_ );

    if ( NULL == new_win )
    {
        gui_simple_message_to_user_show_message_with_int( message_to_user,
                                                          GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                          GUI_SIMPLE_MESSAGE_CONTENT_Q_MAX_WINDOWS_ALREADY_OPEN,
                                                          GUI_SIMPLE_MESSAGE_PARAM_NATURE_QUANTITY,
                                                          GUI_WINDOW_MANAGER_MAX_MAIN_WINDOWS
                                                        );
    }

    TRACE_END();
}


/*
Copyright 2016-2020 Andreas Warnke

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
