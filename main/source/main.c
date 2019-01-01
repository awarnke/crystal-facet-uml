/* File: main.c; Copyright and License: see below */

#include "gui_main.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include "util/string/utf8string.h"
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static data_database_t database;
static ctrl_controller_t controller;

/*!
 *  \brief main starts the gui
 */
int main (int argc, char *argv[]) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    int exit_code = 0;
    TSLOG_INIT(META_INFO_PROGRAM_ID_STR);
    char *database_file = NULL;
    bool do_not_start = false;
    bool do_repair = false;
    bool do_check = false;
    bool do_upgrade = false;

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            fprintf( stdout, "\nUsage:\n" );
            fprintf( stdout, "    %s -h for help\n", argv[0] );
            fprintf( stdout, "    %s -v for version\n", argv[0] );
            fprintf( stdout, "    %s -u <database_file> to use/create a database file\n", argv[0] );
            fprintf( stdout, "    %s -f <database_file> to convert a database file to format " META_INFO_VERSION_STR "\n", argv[0] );
            fprintf( stdout, "    %s -t <database_file> to test the database file\n", argv[0] );
            fprintf( stdout, "    %s -r <database_file> to test and repair the database file\n", argv[0] );
            do_not_start = true;
        }
        if ( utf8string_equals_str( argv[1], "-v" ) )
        {
            fprintf( stdout, "\n%s\n", META_INFO_VERSION_STR );
            do_not_start = true;
        }
    }
    if ( argc == 3 )
    {
        if ( utf8string_equals_str( argv[1], "-r" ) )
        {
            database_file = argv[2];
            do_not_start = true;
            do_repair = true;
        }
        if ( utf8string_equals_str( argv[1], "-t" ) )
        {
            database_file = argv[2];
            do_not_start = true;
            do_check = true;
        }
        if ( utf8string_equals_str( argv[1], "-u" ) )
        {
            database_file = argv[2];
        }
        if ( utf8string_equals_str( argv[1], "-f" ) )
        {
            database_file = argv[2];
            /* do_not_start = true; */
            do_upgrade = true;
        }
    }

    /* repair database */
    if ( do_repair || do_check )
    {
        char repair_log_buffer[10000] = "";
        utf8stringbuf_t repair_log = UTF8STRINGBUF( repair_log_buffer );

        TRACE_INFO("starting DB...");
        data_database_init( &database );
        data_database_open( &database, database_file );

        TRACE_INFO("initializing controller...");
        ctrl_controller_init( &controller, &database );

        TRACE_INFO("reparing/testing...");
        ctrl_controller_repair_database( &controller, do_repair, NULL, NULL, repair_log );
        TRACE_INFO("reparing/testing finished.");

        TRACE_INFO("destroying controller...");
        ctrl_controller_destroy( &controller );

        TRACE_INFO("stopping DB...");
        data_database_close( &database );
        data_database_destroy( &database );

        fprintf( stdout, "\n\n%s\n", utf8stringbuf_get_string(repair_log) );
    }

    if ( do_upgrade )
    {
        TRACE_INFO("starting DB...");
        data_database_init( &database );
        data_database_open( &database, database_file );

        TRACE_INFO("upgrading DB...");
        data_error_t up_err;
        up_err = data_database_upgrade_tables( &database );
        TRACE_INFO( ( DATA_ERROR_NONE == up_err ) ? "success" : "failure" );

        TRACE_INFO("stopping DB...");
        data_database_close( &database );
        data_database_destroy( &database );
    }

    /* run program */
    if ( ! do_not_start )
    {
        TRACE_INFO("starting DB...");
        TRACE_INFO_INT("sizeof(data_database_t)/B:",sizeof(data_database_t));
        data_database_init( &database );
        if ( NULL != database_file )
        {
            data_database_open( &database, database_file );
        }

        TRACE_TIMESTAMP();
        TRACE_INFO("initializing controller...");
        TRACE_INFO_INT("sizeof(ctrl_controller_t)/B:",sizeof(ctrl_controller_t));
        ctrl_controller_init( &controller, &database );

        TRACE_TIMESTAMP();
        TRACE_INFO("running GUI...");
        gui_main( argc, argv, &controller, &database );
        TRACE_INFO("GUI stopped.");

        TRACE_TIMESTAMP();
        TRACE_INFO("destroying controller...");
        ctrl_controller_destroy( &controller );

        TRACE_TIMESTAMP();
        TRACE_INFO("stopping DB...");
        data_database_close( &database );
        data_database_destroy( &database );
    }

    TSLOG_DESTROY();
    TRACE_TIMESTAMP();
    TRACE_END_ERR(exit_code);
    return exit_code;
}


/*
Copyright 2016-2019 Andreas Warnke

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
