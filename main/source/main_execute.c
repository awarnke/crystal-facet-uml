/* File: main_execute.c; Copyright and License: see below */

#include "main_execute.h"
#include "main_commands.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include "utf8stringbuf/utf8string.h"
#include "u8stream/universal_stream_output_stream.h"
#include "utf8stream/utf8stream_writer.h"
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

static const char *const MAIN_HELP
    = "\nUsage:\n"
    "    -h : for help\n"
    "    -v : for version\n"
    "    -e <database_file> <export_format> <export_directory> : to export all diagrams,\n"
    "       export_format: docbook|json|pdf|png|ps|svg|txt|html|xmi\n"
    "    -i <database_file> <import_mode>   <json_input_file>  : to import elements,\n"
    "       import_mode: check|add\n"
    "   [-u] <database_file> : to use/create a database file\n"
    "       extension for compact sqlite format: *.cfu1\n"
    "    -g <database_file> : to upgrade the database tables from version 1.32.1 and older\n"
    "    -t <database_file> : to test the database file\n"
    "    -r <database_file> : to test and repair the database file\n";

int main_execute (int argc, char **argv) {
    U8_TRACE_BEGIN();
    U8_TRACE_TIMESTAMP();
    u8_error_t exit_code = U8_ERROR_NONE;
    U8_LOG_INIT(META_INFO_PROGRAM_ID_STR);
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
    utf8stream_writer_t writer;
    utf8stream_writer_init( &writer, universal_stream_output_stream_get_output_stream( &out_stream ) );

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            utf8stream_writer_write_str( &writer, MAIN_HELP );
            do_not_start = true;
        }
        if ( utf8string_equals_str( argv[1], "-v" ) )
        {
            utf8stream_writer_write_str( &writer, "\n" );
            utf8stream_writer_write_str( &writer, META_VERSION_STR );
            utf8stream_writer_write_str( &writer, "\n" );
            do_not_start = true;
        }
        if ( ! utf8string_starts_with_str( argv[1], "-" ) )
        {
            /* like -u, allows nice drag+drop integration on Windows(TM) */
            database_file = argv[1];
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
            export_format = main_execute_private_get_selected_format(argv[3]);
            export_directory = argv[4];
            do_not_start = true;
            do_export = true;
        }
        if ( utf8string_equals_str( argv[1], "-i" ) )
        {
            database_file = argv[2];
            import_mode = main_execute_private_get_selected_mode(argv[3]);
            import_file = argv[4];
            do_not_start = true;
            do_import = true;
        }
    }

    {
        main_commands_t commands;
        const int argc_remaining = 1;
        exit_code |= main_commands_init( &commands, ( ! do_not_start ), argc_remaining, argv );

        /* upgrade only if explicitly requested, otherwiese read-only mode of -t and -e options does not work: */
        if ( do_upgrade )
        {
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
    utf8stream_writer_destroy( &writer );
    universal_stream_output_stream_destroy( &out_stream );

    U8_LOG_STATS();
    U8_LOG_DESTROY();
    U8_TRACE_TIMESTAMP();
    int exit_byte = ((exit_code >> 24)|(exit_code >> 16)|(exit_code >> 8)|(exit_code))&0xff;
    U8_TRACE_END_ERR(exit_byte);
    return exit_byte;
}

io_file_format_t main_execute_private_get_selected_format( char *arg_fmt )
{
    U8_TRACE_BEGIN();
    assert( arg_fmt != NULL );
    io_file_format_t result = 0;

    if ( utf8string_equals_str( arg_fmt, "docbook" ) )
    {
        result = IO_FILE_FORMAT_DOCBOOK;
    }
    else if ( utf8string_equals_str( arg_fmt, "json" ) )
    {
        result = IO_FILE_FORMAT_JSON;
    }
    else if ( utf8string_equals_str( arg_fmt, "pdf" ) )
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
    else if ( utf8string_equals_str( arg_fmt, "html" ) )
    {
        result = IO_FILE_FORMAT_HTML;
    }
    else if ( utf8string_equals_str( arg_fmt, "xhtml" ) /* legacy option */ )
    {
        result = IO_FILE_FORMAT_HTML;
    }
    else if ( utf8string_equals_str( arg_fmt, "xmi" ) )
    {
        result = IO_FILE_FORMAT_XMI2;
    }

    U8_TRACE_END();
    return result;
}

io_import_mode_t main_execute_private_get_selected_mode( char *arg_fmt )
{
    U8_TRACE_BEGIN();
    assert( arg_fmt != NULL );
    io_import_mode_t result = IO_IMPORT_MODE_CHECK;

    if ( utf8string_equals_str( arg_fmt, "check" ) )
    {
        result = IO_IMPORT_MODE_CHECK;
    }
    else if ( utf8string_equals_str( arg_fmt, "add" ) )
    {
        result = IO_IMPORT_MODE_CREATE|IO_IMPORT_MODE_LINK;
    }
    else if ( utf8string_equals_str( arg_fmt, "update" ) )  /* legacy option */
    {
        result = IO_IMPORT_MODE_CREATE|IO_IMPORT_MODE_LINK;
    }

    U8_TRACE_END();
    return result;
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
