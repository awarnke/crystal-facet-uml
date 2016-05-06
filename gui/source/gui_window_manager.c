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
    observer_init( &((*this_).window_close_observer), this_, (void (*)(void*,void*)) &gui_window_manager_close_main_window );
    gui_main_window_init( &((*this_).the_window), controller, database, &((*this_).db_reader), &((*this_).gui_resources), &((*this_).window_close_observer) );

    /* the second window is just for keeping in mind the MVC pattern */
    gui_main_window_init( &((*this_).the_window2), controller, database, &((*this_).db_reader), &((*this_).gui_resources), &((*this_).window_close_observer) );

    TRACE_END();
}

void gui_window_manager_destroy( gui_window_manager_t *this_ )
{
    TRACE_BEGIN();

    /* destructors of windows are already called */

    observer_destroy( &((*this_).window_close_observer) );
    data_database_reader_destroy( &((*this_).db_reader) );
    gui_resources_destroy( &((*this_).gui_resources) );

    TRACE_END();
}

gui_main_window_t *gui_window_manager_open_main_window( gui_window_manager_t *this_ )
{
    assert (0);
}

void gui_window_manager_close_main_window( gui_window_manager_t *this_, gui_main_window_t *main_window )
{
    TRACE_BEGIN();
    
    gtk_main_quit();

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
