/* File: gui_main.c; Copyright and License: see below */

#include "gui_main.h"
#include "gui_main_window.h"
#include "gui_window_manager.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "meta/meta_info.h"
#include "trace.h"
#include "tslog.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static gui_window_manager_t window_manager;

void gui_main ( ctrl_controller_t *controller, data_database_t *database ) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    TRACE_INFO_INT("sizeof(gui_window_manager_t):",sizeof(gui_window_manager_t));
    TRACE_INFO("initializing gui thread...");

    gui_window_manager_init( &window_manager, controller, database );

    gui_window_manager_open_main_window( &window_manager );

    TRACE_TIMESTAMP();

#if ( GTK_MAJOR_VERSION >= 4 )
    GApplication *const g_app
        = g_application_new( META_INFO_PROGRAM_ID_STR,  /* application_id */
                             G_APPLICATION_NON_UNIQUE
                           );
    const int argc = 0;
    char * *const argv = NULL;
    int error_code = g_application_run( g_app, argc, argv );
    if ( error_code != 0 )
    {
        TSLOG_ERROR_INT("g_application_run:",error_code);
    }
#else
    gtk_main();
#endif

    gui_window_manager_destroy( &window_manager );

    TRACE_TIMESTAMP();
    TRACE_END();
}


/*
Copyright 2016-2022 Andreas Warnke

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
