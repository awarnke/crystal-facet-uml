/* File: main_commands.c; Copyright and License: see below */

#include "main_commands.h"
#include "gui_main.h"
#include "io_exporter.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "trace.h"
#include "tslog.h"
#include "util/string/utf8string.h"
#include <gtk/gtk.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

static data_database_t database;
static ctrl_controller_t controller;

int main_commands_init ( main_commands_t *this_, bool start_gui, int argc, char *argv[] )
{
    TRACE_BEGIN();
    int result = 0;

    /* initialize the base libraries: gobject, gio, glib, gdk and gtk */
    if ( start_gui )
    {
        gtk_init(&argc, &argv);
        /* if this program was not terminated, gtk init was successful. */
    }
    else
    {
        gboolean success = gtk_init_check(&argc, &argv);
        if ( ! success )
        {
            TSLOG_WARNING("gtk could not be initialized.");
            result = -1;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

void main_commands_destroy ( main_commands_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
}

int main_commands_upgrade ( main_commands_t *this_, const char *database_path )
{
    TRACE_BEGIN();
    int result = 0;

    assert( database_path != NULL );

    TRACE_INFO("starting DB...");
    data_database_init( &database );

    TRACE_INFO("upgrading DB...");
    const data_error_t up_err
        = data_database_open( &database, database_path );  /* upgrade is implicitely done */
    if ( up_err != DATA_ERROR_NONE )
    {
        fprintf( stdout, "error opening %s\n", database_path );
        result = -1;
    }
    TRACE_INFO( ( 0 == up_err ) ? "success" : "failure" );

    TRACE_INFO("stopping DB...");
    data_database_close( &database );
    data_database_destroy( &database );

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2021-2021 Andreas Warnke

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
