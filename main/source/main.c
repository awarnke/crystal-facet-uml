/* File: main.c; Copyright and License: see below */

#include "main.h"
#include "main_commands.h"
#include "gui_main.h"
#include "io_exporter.h"
#include "storage/data_database.h"
#include "ctrl_controller.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include "util/string/utf8string.h"
#include "stream/universal_stream_output_stream.h"
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
    char *import_file = NULL;
    bool do_not_start = false;
    bool do_repair = false;
    bool do_check = false;
    bool do_export = false;
    bool do_upgrade = false;
    bool do_import = false;
    io_file_format_t export_format = 0;
    io_file_format_t import_format = 0;
    universal_stream_output_stream_t out_stream;
    universal_stream_output_stream_init( &out_stream, stdout );
    universal_output_stream_t *out;
    out = universal_stream_output_stream_get_output_stream( &out_stream );
    universal_utf8_writer_t writer;
    universal_utf8_writer_init( &writer, out );

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            fprintf( stdout, "\nUsage:\n" );
            fprintf( stdout, "    -h for help\n" );
            fprintf( stdout, "    -v for version\n" );
            fprintf( stdout, "    -e <database_file> <export_format> <target_directory> to export all diagrams\n" );
            fprintf( stdout, "    -i <database_file> json            <input_file>       to import elements\n" );
            fprintf( stdout, "    -u <database_file> to use/create a database file\n" );
            fprintf( stdout, "    -g <database_file> to upgrade the database tables from version 1.32.1 and older\n" );
            fprintf( stdout, "    -t <database_file> to test the database file\n" );
            fprintf( stdout, "    -r <database_file> to test and repair the database file\n" );
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
        if ( utf8string_equals_str( argv[1], "-g" ) )
        {
            database_file = argv[2];
            do_not_start = true;
            do_upgrade = true;
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
        if ( utf8string_equals_str( argv[1], "-i" ) )
        {
            database_file = argv[2];
            import_format = main_private_get_selected_format(argv[3]);
            import_file = argv[4];
            do_not_start = true;
            do_import = true;
        }
    }

    main_commands_t commands;
    exit_code |= main_commands_init( &commands, ( ! do_not_start ), argc, argv );

    if ( do_upgrade || do_check || do_export )
    {
        /* if upgrade or starting in read-only mode, upgrade db first: */
        exit_code |= main_commands_upgrade( &commands, database_file, &writer );
    }

    /* repair database */
    if ( do_repair || do_check )
    {
        exit_code |= main_commands_repair( &commands, database_file, do_check, &writer );
    }

    if ( do_export )
    {
        assert( database_file != NULL );
        assert( export_directory != NULL );

        TRACE_INFO("starting DB...");
        data_database_init( &database );
        const data_error_t db_err
            = data_database_open_read_only( &database, database_file );
        if ( db_err != DATA_ERROR_NONE )
        {
            fprintf( stdout, "error opening %s\n", database_file );
        }

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
                    const unsigned int stat_ok = data_stat_get_series_count( &export_stat, DATA_STAT_SERIES_EXPORTED );
                    const unsigned int stat_warn = data_stat_get_series_count( &export_stat, DATA_STAT_SERIES_WARNING );
                    const unsigned int stat_err = data_stat_get_series_count( &export_stat, DATA_STAT_SERIES_ERROR );
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

    if ( do_import )
    {
        assert( database_file != NULL );
        assert( import_file != NULL );

        fprintf( stdout, "\nimporting %s not yet implemented.\n", import_file );
    }

    /* run program */
    if ( ! do_not_start )
    {
        TRACE_INFO("starting DB...");
        TRACE_INFO_INT("sizeof(data_database_t)/B:",sizeof(data_database_t));
        data_database_init( &database );
        if ( NULL != database_file )
        {
            const data_error_t db_err
                = data_database_open( &database, database_file );
            if ( db_err != DATA_ERROR_NONE )
            {
                fprintf( stdout, "error opening %s\n", database_file );
            }
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

    main_commands_destroy( &commands );
    universal_stream_output_stream_destroy( &out_stream );

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
    else if ( utf8string_equals_str( arg_fmt, "json" ) )
    {
        result = IO_FILE_FORMAT_JSON;
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
