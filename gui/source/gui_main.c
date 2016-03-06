/* File: gui_main.c; Copyright and License: see below */

#include "gui_main.h"
#include "gui_main_window.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

void gui_main ( int argc, char *argv[], ctrl_controller_t *controller, data_database_t *database ) {
    TRACE_BEGIN();
    gui_main_window_t the_window;

    /* the second window is just for keeping in mind the MVC pattern */
    gui_main_window_t the_window2;

    TRACE_INFO("initializing gui thread...");

    gtk_init(&argc, &argv);

    gui_main_window_init( &the_window, controller, database );

    /* the second window is just for keeping in mind the MVC pattern */
    gui_main_window_init( &the_window2, controller, database );

    gtk_main();
    gtk_main(); /* once for every window: closing 1 window will end 1 main loop */

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
