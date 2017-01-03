/* File: gui_main.c; Copyright and License: see below */

#include "gui_main.h"
#include "gui_main_window.h"
#include "gui_window_manager.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static gui_window_manager_t window_manager;

void gui_main ( int argc, char *argv[], ctrl_controller_t *controller, data_database_t *database ) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    TRACE_INFO_INT("sizeof(gui_window_manager_t):",sizeof(gui_window_manager_t));
    TRACE_INFO("initializing gui thread...");

    gtk_init(&argc, &argv);

    gui_window_manager_init( &window_manager, controller, database );

    gui_window_manager_open_main_window( &window_manager );

    TRACE_TIMESTAMP();

    gtk_main();

    gui_window_manager_destroy( &window_manager );

    TRACE_TIMESTAMP();
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
