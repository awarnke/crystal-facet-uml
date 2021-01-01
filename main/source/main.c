/* File: main.c; Copyright and License: see below */

#include "main.h"
#include "gui_main.h"
#include "io_exporter.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
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

int main (int argc, char *argv[]) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    int exit_code = 0;
    TSLOG_INIT(META_INFO_PROGRAM_ID_STR);
    char *database_file = NULL;
    char *export_directory = NULL;
    bool do_not_start = false;
    bool do_repair = false;
    bool do_check = false;
    bool do_export = false;
    io_file_format_t export_format = 0;

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            fprintf( stdout, "\nUsage:\n" );
            fprintf( stdout, "    %s -h for help\n", argv[0] );
            fprintf( stdout, "    %s -v for version\n", argv[0] );
            fprintf( stdout, "    %s -u <database_file> to use/create a database file\n", argv[0] );
            fprintf( stdout, "    %s -e <database_file> <export_format> <target_directory> to export all diagrams\n", argv[0] );
            fprintf( stdout, "    %s -t <database_file> to test the database file\n", argv[0] );
            fprintf( stdout, "    %s -r <database_file> to test and repair the database file\n", argv[0] );
            do_not_start = true;
        }
        if ( utf8string_equals_str( argv[1], "-v" ) )
        {
            fprintf( stdout, "\n%s\n", META_VERSION_STR );
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
    }
    if ( argc == 5 )
    {
        if ( utf8string_equals_str( argv[1], "-e" ) )
        {
            database_file = argv[2];
            export_format = main_private_get_selected_format(argv[3]);
            export_directory = argv[4];
            do_not_start = true;
            do_export = true;
        }
    }

    /* initialize the base libraries: gobject, gio, glib, gdk and gtk */
    if ( do_not_start )
    {
        gboolean success = gtk_init_check(&argc, &argv);
        if ( ! success )
        {
            TSLOG_WARNING("gtk could not be initialized.");
        }
    }
    else
    {
        gtk_init(&argc, &argv);
    }

    /* repair database */
    if ( do_repair || do_check )
    {
        assert( database_file != NULL );
        static char repair_log_buffer[10000] = "";
        static utf8stringbuf_t repair_log = UTF8STRINGBUF( repair_log_buffer );

        TRACE_INFO("starting DB...");
        data_database_init( &database );
        if ( do_repair )
        {
            data_database_open( &database, database_file );
        }
        else
        {
            data_database_open_read_only( &database, database_file );
        }

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

    /*
    if ( do_upgrade )
    {
        assert( database_file != NULL );

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
    */
    if ( do_export )
    {
        assert( database_file != NULL );
        assert( export_directory != NULL );

        TRACE_INFO("starting DB...");
        data_database_init( &database );
        data_database_open_read_only( &database, database_file );

        TRACE_INFO("exporting DB...");
        int export_err;
        TRACE_INFO_STR( "chosen folder:", export_directory );
        const char *document_filename = data_database_get_filename_ptr ( &database );
        if ( data_database_is_open( &database ) )
        {
            static io_exporter_t exporter;
            static data_database_reader_t db_reader;
            data_database_reader_init( &db_reader, &database );
            io_exporter_init( &exporter, &db_reader );
            {
                data_stat_t export_stat;
                data_stat_init ( &export_stat );
                export_err = io_exporter_export_files( &exporter, export_format, export_directory, document_filename, &export_stat );
                {
                    const unsigned int stat_ok = data_stat_get_series_count (  &export_stat, DATA_STAT_SERIES_EXPORTED );
                    const unsigned int stat_warn = data_stat_get_series_count (  &export_stat, DATA_STAT_SERIES_WARNING );
                    const unsigned int stat_err = data_stat_get_series_count (  &export_stat, DATA_STAT_SERIES_ERROR );
                    fprintf( stdout, "\nexported: %d; warnings: %d; errors: %d\n", stat_ok, stat_warn, stat_err );
                }
                data_stat_trace( &export_stat );
                data_stat_destroy ( &export_stat );
            }
            data_database_reader_destroy( &db_reader );
            io_exporter_destroy( &exporter );
        }
        else
        {
            export_err = -1;
        }
        TRACE_INFO( ( 0 == export_err ) ? "success" : "failure" );

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
        gui_main( &controller, &database );
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

io_file_format_t main_private_get_selected_format( char *arg_fmt )
{
    TRACE_BEGIN();
    assert( arg_fmt != NULL );
    io_file_format_t result = 0;

    if ( utf8string_equals_str( arg_fmt, "pdf" ) )
    {
        result = IO_FILE_FORMAT_PDF;
    }
    else if ( utf8string_equals_str( arg_fmt, "png" ) )
    {
        result = IO_FILE_FORMAT_PNG;
    }
    else if ( utf8string_equals_str( arg_fmt, "ps" ) )
    {
        result = IO_FILE_FORMAT_PS;
    }
    else if ( utf8string_equals_str( arg_fmt, "svg" ) )
    {
        result = IO_FILE_FORMAT_SVG;
    }
    else if ( utf8string_equals_str( arg_fmt, "txt" ) )
    {
        result = IO_FILE_FORMAT_TXT;
    }
    else if ( utf8string_equals_str( arg_fmt, "docbook" ) )
    {
        result = IO_FILE_FORMAT_DOCBOOK;
    }
    else if ( utf8string_equals_str( arg_fmt, "xhtml" ) )
    {
        result = IO_FILE_FORMAT_XHTML;
    }
    else if ( utf8string_equals_str( arg_fmt, "xmi" ) )
    {
        result = IO_FILE_FORMAT_XMI2;
    }

    TRACE_END();
    return result;
}


/*
Copyright 2016-2021 Andreas Warnke

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
