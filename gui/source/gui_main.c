/* File: gui_main.c; Copyright and License: see below */

#include "gui_main.h"
#include "gui_main_window.h"
#include "gui_window_manager.h"
#include "storage/data_database_reader.h"
#include "meta/meta_info.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static gui_window_manager_t window_manager;

static void gui_main_activate_callback( GtkApplication* app, gpointer user_data )
{
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    gui_window_manager_t *const win_manager = user_data;
    assert( win_manager != NULL );

    gui_window_manager_open_main_window( win_manager );

    U8_TRACE_END();
}

#if (( GTK_MAJOR_VERSION == 4 ) && ( GTK_MINOR_VERSION <= 6 ))
#define G_APPLICATION_DEFAULT_FLAGS (G_APPLICATION_FLAGS_NONE)
#endif  /* (( GTK_MAJOR_VERSION == 4 ) && ( GTK_MINOR_VERSION <= 6 )) */

void gui_main ( io_data_file_t *data_file, int argc, char **argv ) {
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    U8_TRACE_INFO_INT( "sizeof(gui_window_manager_t):", sizeof(gui_window_manager_t) );
    /* expect that no gtk_get_micro_version needed for analysis: */
    U8_LOG_EVENT_INT( "compiled against gtk:    ", GTK_MAJOR_VERSION * 1000 + GTK_MINOR_VERSION );
    U8_LOG_EVENT_INT( "linked against gtk:    ", gtk_get_major_version() * 1000 + gtk_get_minor_version() );
    U8_TRACE_INFO( "initializing gui thread..." );

    /* init */
    GtkApplication *const gtk_app
        = gtk_application_new( META_INFO_APPLICATION_ID_STR, G_APPLICATION_DEFAULT_FLAGS );

    gui_window_manager_init( &window_manager, data_file, gtk_app );

    g_signal_connect( gtk_app, "activate", G_CALLBACK( gui_main_activate_callback ), &window_manager);

    /* run */
    U8_LOG_EVENT( "Connecting to a display (if display is remote, this may take some seconds) ..." );
    int error_code = g_application_run( G_APPLICATION(gtk_app), argc, argv );
    if ( error_code != 0 )
    {
        U8_LOG_ERROR_INT( "g_application_run:", error_code );
    }

    /* destroy */
    gui_window_manager_destroy( &window_manager );
    g_object_unref( gtk_app );

    U8_TRACE_TIMESTAMP();
    U8_TRACE_END();
}


/*
Copyright 2016-2024 Andreas Warnke

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
