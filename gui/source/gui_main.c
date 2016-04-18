/* File: gui_main.c; Copyright and License: see below */

#include "gui_main.h"
#include "gui_main_window.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

void gui_main ( int argc, char *argv[], ctrl_controller_t *controller, data_database_t *database ) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    static gui_main_window_t the_window;

    /* the second window is just for keeping in mind the MVC pattern */
    static gui_main_window_t the_window2;

    static gui_resources_t gui_resources;

    static data_database_reader_t db_reader;

    TRACE_INFO_INT("sizeof(gui_main_window_t):",sizeof(gui_main_window_t));
    TRACE_INFO_INT("sizeof(gui_resources_t):",sizeof(gui_resources_t));
    TRACE_INFO("initializing gui thread...");

    gtk_init(&argc, &argv);

    gui_resources_init( &gui_resources );
    data_database_reader_init( &db_reader, database );

    ctrl_diagram_controller_t *diag_control;
    diag_control = ctrl_controller_get_diagram_control_ptr ( controller );
    ctrl_diagram_controller_create_root_diagram_if_not_exists ( diag_control, DATA_DIAGRAM_TYPE_BLOCK_DIAGRAM, "Overview [root]", NULL );

    gui_main_window_init( &the_window, controller, database, &db_reader, &gui_resources );

    /* the second window is just for keeping in mind the MVC pattern */
    gui_main_window_init( &the_window2, controller, database, &db_reader, &gui_resources );

    TRACE_TIMESTAMP();
    gtk_main();
    gtk_main(); /* once for every window: closing 1 window will end 1 main loop */

    data_database_reader_destroy( &db_reader );
    gui_resources_destroy( &gui_resources );

    TRACE_TIMESTAMP();
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
