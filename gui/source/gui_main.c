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

#if ( GTK_MAJOR_VERSION >= 4 )
static void gui_main_activate_callback( GtkApplication* app, gpointer user_data )
{
    TRACE_BEGIN();
    TRACE_TIMESTAMP();

    gui_window_manager_open_main_window( &window_manager );

    TRACE_END();
}
#endif

void gui_main ( ctrl_controller_t *controller, data_database_t *database, int argc, char **argv ) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    TRACE_INFO_INT( "sizeof(gui_window_manager_t):", sizeof(gui_window_manager_t) );
    TRACE_INFO( "initializing gui thread..." );

#if ( GTK_MAJOR_VERSION >= 4 )
    /* init */
    GtkApplication *const gtk_app
        = gtk_application_new( META_INFO_APPLICATION_ID_STR, G_APPLICATION_FLAGS_NONE );
    gui_window_manager_init( &window_manager, controller, database, gtk_app );

    g_signal_connect( gtk_app, "activate", G_CALLBACK( gui_main_activate_callback ), NULL);

    /* run */
    int error_code = g_application_run( G_APPLICATION(gtk_app), argc, argv );
    if ( error_code != 0 )
    {
        TSLOG_ERROR_INT( "g_application_run:", error_code );
    }

    /* destroy */
    gui_window_manager_destroy( &window_manager );
    g_object_unref( gtk_app );
#else
    /* init */
    gui_window_manager_init( &window_manager, controller, database, NULL );
    gui_window_manager_open_main_window( &window_manager );

    /* run */
    gtk_main();

    /* destroy */
    gui_window_manager_destroy( &window_manager );
#endif

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
