/* File: main.c; Copyright and License: see below */

#include "main.h"
#include "main_commands.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include "util/string/utf8string.h"
#include "stream/universal_stream_output_stream.h"
#include "universal_utf8_writer.h"
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

static const char *const MAIN_HELP
    = "\nUsage:\n"
    "    -h for help\n"
    "    -v for version\n"
    "    -e <database_file> <export_format> <target_directory> to export all diagrams\n"
    "    -i <database_file> <import_mode>   <input_file>       to import elements\n"
    "    -u <database_file> to use/create a database file\n"
    "    -g <database_file> to upgrade the database tables from version 1.32.1 and older\n"
    "    -t <database_file> to test the database file\n"
    "    -r <database_file> to test and repair the database file\n";

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
    io_import_mode_t import_mode = 0;
    universal_stream_output_stream_t out_stream;
    universal_stream_output_stream_init( &out_stream, stdout );
    universal_utf8_writer_t writer;
    universal_utf8_writer_init( &writer, universal_stream_output_stream_get_output_stream( &out_stream ) );

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            universal_utf8_writer_write_str( &writer, MAIN_HELP );
            do_not_start = true;
        }
        if ( utf8string_equals_str( argv[1], "-v" ) )
        {
            universal_utf8_writer_write_str( &writer, "\n" );
            universal_utf8_writer_write_str( &writer, META_VERSION_STR );
            universal_utf8_writer_write_str( &writer, "\n" );
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
            import_mode = main_private_get_selected_mode(argv[3]);
            import_file = argv[4];
            do_not_start = true;
            do_import = true;
        }
    }

    {
        static main_commands_t commands;
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
            exit_code |= main_commands_export( &commands, database_file, export_format, export_directory, &writer );
        }

        if ( do_import )
        {
            exit_code |= main_commands_import( &commands, database_file, import_mode, import_file, &writer );
        }

        /* run program */
        if ( ! do_not_start )
        {
            exit_code |= main_commands_start_gui( &commands, database_file, &writer );
        }

        main_commands_destroy( &commands );
    }
    universal_utf8_writer_destroy( &writer );
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

io_import_mode_t main_private_get_selected_mode( char *arg_fmt )
{
    TRACE_BEGIN();
    assert( arg_fmt != NULL );
    io_import_mode_t result = IO_IMPORT_MODE_CHECK;

    if ( utf8string_equals_str( arg_fmt, "check" ) )
    {
        result = IO_IMPORT_MODE_CHECK;
    }
    else if ( utf8string_equals_str( arg_fmt, "update" ) )
    {
        result = IO_IMPORT_MODE_CREATE|IO_IMPORT_MODE_LINK;
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
