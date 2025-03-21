/* File: main_commands.c; Copyright and License: see below */

#include "main_commands.h"
#include "gui_main.h"
#include "gui_error_info_printer.h"
#include "io_exporter.h"
#include "io_importer.h"
#include "ctrl_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <assert.h>

static io_data_file_t single_big_data_file;  /*!< a data_file struct, placed in the data segment due to its >5MB size */

u8_error_t main_commands_init ( main_commands_t *this_, bool start_gui, int argc, char **argv )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    (*this_).argc = argc;
    (*this_).argv = argv;

    /* initialize the base libraries: gobject, gio, glib, gdk and gtk */
    if ( start_gui )
    {
        gtk_init();
        /* if this program was not terminated, gtk init was successful. */
    }
    else
    {
        const gboolean success = gtk_init_check();
        if ( ! success )
        {
            U8_LOG_WARNING("gtk could not be initialized.");
            /* no error here, if no gui requested - test fail otherwise */
        }
    }

    U8_TRACE_INFO("starting DB and its controller...");
    U8_TRACE_INFO_INT("sizeof(io_data_file_t)/B:",sizeof(io_data_file_t));
    (*this_).data_file = &single_big_data_file;
    io_data_file_init( (*this_).data_file );

    U8_TRACE_END_ERR( result );
    return result;
}

void main_commands_destroy ( main_commands_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_TRACE_INFO("stopping DB and its controller...");
    io_data_file_destroy( (*this_).data_file );

    U8_TRACE_END();
}

u8_error_t main_commands_repair ( main_commands_t *this_,
                                  const char *data_file_path,
                                  bool check_only,
                                  utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert( data_file_path != NULL );
    const bool do_repair = ( ! check_only );
    u8_error_t result = U8_ERROR_NONE;

    U8_TRACE_INFO("opening DB...");
    u8_error_info_t err_info;
    result |= io_data_file_open( (*this_).data_file, data_file_path, check_only, &err_info );
    if ( result != U8_ERROR_NONE )
    {
        utf8stream_writer_write_str( out_english_report, "error opening database_file " );
        utf8stream_writer_write_str( out_english_report, data_file_path );
        utf8stream_writer_write_str( out_english_report, "\n" );
    }
    result |= main_commands_private_report_error_info( this_, &err_info, out_english_report );

    U8_TRACE_INFO("reparing/testing...");
    utf8stream_writer_write_str( out_english_report, "\n\n" );
    result |= ctrl_controller_repair_database( io_data_file_get_controller_ptr( (*this_).data_file ),
                                               do_repair,
                                               NULL,
                                               NULL,
                                               out_english_report
                                             );
    utf8stream_writer_write_str( out_english_report, "\n" );
    U8_TRACE_INFO("reparing/testing finished.");

    U8_TRACE_INFO("closing DB...");
    io_data_file_close( (*this_).data_file );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t main_commands_start_gui ( main_commands_t *this_,
                                     const char *data_file_path,
                                     utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    U8_TRACE_TIMESTAMP();
    U8_TRACE_INFO("opening DB...");
    if ( NULL != data_file_path )
    {
        u8_error_info_t err_info;
        result |= io_data_file_open_writeable( (*this_).data_file, data_file_path, &err_info );
        if ( result != U8_ERROR_NONE )
        {
            utf8stream_writer_write_str( out_english_report, "error opening database_file " );
            utf8stream_writer_write_str( out_english_report, data_file_path );
            utf8stream_writer_write_str( out_english_report, "\n" );
        }
        result |= main_commands_private_report_error_info( this_, &err_info, out_english_report );
    }

    U8_TRACE_TIMESTAMP();
    U8_TRACE_INFO("running GUI...");
    gui_main( (*this_).data_file, (*this_).argc, (*this_).argv );
    U8_TRACE_INFO("GUI stopped.");

    U8_TRACE_TIMESTAMP();
    U8_TRACE_INFO("closing DB...");
    io_data_file_close( (*this_).data_file );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t main_commands_export ( main_commands_t *this_,
                                  const char *data_file_path,
                                  io_file_format_t export_format,
                                  const char *export_directory,
                                  utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert( data_file_path != NULL );
    assert( export_directory != NULL );
    u8_error_t export_err = U8_ERROR_NONE;

    U8_TRACE_INFO("opening DB...");
    u8_error_info_t err_info;
    export_err |= io_data_file_open_read_only( (*this_).data_file, data_file_path, &err_info );
    if ( export_err != U8_ERROR_NONE )
    {
        utf8stream_writer_write_str( out_english_report, "error opening database_file " );
        utf8stream_writer_write_str( out_english_report, data_file_path );
        utf8stream_writer_write_str( out_english_report, "\n" );
    }
    export_err |= main_commands_private_report_error_info( this_, &err_info, out_english_report );

    U8_TRACE_INFO("exporting DB...");
    U8_TRACE_INFO_STR( "chosen folder:", export_directory );
    const char *document_filename = io_data_file_get_filename_const( (*this_).data_file );
    if ( io_data_file_is_open( (*this_).data_file ) )
    {
        static data_database_reader_t db_reader;
        data_database_reader_init( &db_reader, io_data_file_get_database_ptr( (*this_).data_file ) );
        static io_exporter_t exporter;
        io_exporter_init( &exporter, &db_reader );
        {
            data_stat_t export_stat;
            data_stat_init ( &export_stat );
            export_err = io_exporter_export_files( &exporter, export_format, export_directory, document_filename, &export_stat );
            export_err |= main_commands_private_report_stat( this_, &export_stat, "exported", out_english_report );
            data_stat_trace( &export_stat );
            data_stat_destroy ( &export_stat );
        }
        io_exporter_destroy( &exporter );
        data_database_reader_destroy( &db_reader );
    }
    else
    {
        export_err = U8_ERROR_NO_DB;
    }

    U8_TRACE_INFO("closing DB...");
    io_data_file_close( (*this_).data_file );

    U8_TRACE_END_ERR( export_err );
    return export_err;
}

u8_error_t main_commands_import ( main_commands_t *this_,
                                  const char *data_file_path,
                                  io_import_mode_t import_mode,
                                  const char *import_file_path,
                                  utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert( data_file_path != NULL );
    assert( import_file_path != NULL );
    u8_error_t import_err = U8_ERROR_NONE;

    U8_TRACE_INFO("opening DB...");
    u8_error_info_t err_info;
    import_err |= io_data_file_open_writeable( (*this_).data_file, data_file_path, &err_info );
    if ( import_err != U8_ERROR_NONE )
    {
        utf8stream_writer_write_str( out_english_report, "error opening database_file " );
        utf8stream_writer_write_str( out_english_report, data_file_path );
        utf8stream_writer_write_str( out_english_report, "\n" );
    }
    import_err |= main_commands_private_report_error_info( this_, &err_info, out_english_report );

    U8_TRACE_INFO("importing data...");
    U8_TRACE_INFO_STR( "chosen data:", import_file_path );
    if ( io_data_file_is_open( (*this_).data_file ) )
    {
        static data_database_reader_t db_reader;
        data_database_reader_init( &db_reader, io_data_file_get_database_ptr( (*this_).data_file ) );
        static io_importer_t importer;
        io_importer_init( &importer, &db_reader, io_data_file_get_controller_ptr( (*this_).data_file ));
        {
            data_stat_t import_stat;
            data_stat_init ( &import_stat );
            u8_error_info_t err_info;
            import_err = io_importer_import_file( &importer, import_mode, import_file_path, &import_stat, &err_info, out_english_report );
            import_err |= main_commands_private_report_stat( this_, &import_stat, "imported", out_english_report );
            import_err |= main_commands_private_report_error_info( this_, &err_info, out_english_report );
            data_stat_trace( &import_stat );
            data_stat_destroy ( &import_stat );
        }
        io_importer_destroy( &importer );
        data_database_reader_destroy( &db_reader );
    }
    else
    {
        import_err = U8_ERROR_NO_DB;
    }

    U8_TRACE_INFO("closing DB...");
    io_data_file_close( (*this_).data_file );

    {
        utf8stream_writer_write_str( out_english_report, "\nplease test the integrity of the database_file:\n" );
        utf8stream_writer_write_str( out_english_report, "crystal-facet-uml -t " );
        utf8stream_writer_write_str( out_english_report, data_file_path );
        utf8stream_writer_write_str( out_english_report, "\n\n" );
    }

    U8_TRACE_END_ERR( import_err );
    return import_err;
}

static const char *const series[DATA_STAT_SERIES_MAX] = {
    [DATA_STAT_SERIES_CREATED]  = " new/exp",
    [DATA_STAT_SERIES_MODIFIED] = "modified",
    [DATA_STAT_SERIES_DELETED]  = " deleted",
    [DATA_STAT_SERIES_IGNORED]  = " ignored",
    [DATA_STAT_SERIES_WARNING]  = "warnings",
    [DATA_STAT_SERIES_ERROR]    = "  errors",
};

static const char *const tables[DATA_STAT_TABLE_MAX] = {
    [DATA_TABLE_VOID] = "life",
    [DATA_TABLE_CLASSIFIER] = "clas",
    [DATA_TABLE_FEATURE] = "feat",
    [DATA_TABLE_RELATIONSHIP] = "rel",
    [DATA_TABLE_DIAGRAMELEMENT] = "d_ele",
    [DATA_TABLE_DIAGRAM] = "diag",
};

u8_error_t main_commands_private_report_stat ( main_commands_t *this_,
                                               const data_stat_t *stat,
                                               const char* mode_name,
                                               utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert( stat != NULL );
    assert( mode_name != NULL );
    assert( out_english_report != NULL );
    u8_error_t write_err = U8_ERROR_NONE;

    /* HEADLINE */
    write_err |= utf8stream_writer_write_str( out_english_report, "\n\t" );

    for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLE_MAX; tables_idx ++ )
    {
        write_err |= utf8stream_writer_write_str( out_english_report, "\t" );
        write_err |= utf8stream_writer_write_str( out_english_report, tables[tables_idx] );
    }
    write_err |= utf8stream_writer_write_str( out_english_report, "\n" );

    /* TABLE */
    for ( int series_idx = 0; series_idx < DATA_STAT_SERIES_MAX; series_idx ++ )
    {
        const uint_fast32_t s_cnt = data_stat_get_series_count( stat, series_idx );

        if ( series_idx == DATA_STAT_SERIES_CREATED )
        {
            write_err |= utf8stream_writer_write_str( out_english_report, mode_name );
        }
        else
        {
            write_err |= utf8stream_writer_write_str( out_english_report, series[series_idx] );
        }
        write_err |= utf8stream_writer_write_str( out_english_report, ": " );

        for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLE_MAX; tables_idx ++ )
        {
            write_err |= utf8stream_writer_write_str( out_english_report, "\t" );
            const uint_fast32_t cnt = data_stat_get_count( stat, tables_idx, series_idx );
            if ( cnt != 0 )
            {
                write_err |= utf8stream_writer_write_int( out_english_report, cnt );
            }
        }

        write_err |= utf8stream_writer_write_str( out_english_report, "\t: " );
        write_err |= utf8stream_writer_write_int( out_english_report, s_cnt );
        write_err |= utf8stream_writer_write_str( out_english_report, "\n" );
    }

    /* ROW OF SUMS */
    write_err |= utf8stream_writer_write_str( out_english_report, "     SUM:" );
    for ( int tables_idx = 0; tables_idx < DATA_STAT_TABLE_MAX; tables_idx ++ )
    {
        const uint_fast32_t t_cnt = data_stat_get_table_count( stat, tables_idx );
        write_err |= utf8stream_writer_write_str( out_english_report, "\t" );
        write_err |= utf8stream_writer_write_int( out_english_report, t_cnt );
    }
    const uint_fast32_t total = data_stat_get_total_count( stat );
    write_err |= utf8stream_writer_write_str( out_english_report, "\t: " );
    write_err |= utf8stream_writer_write_int( out_english_report, total );
    write_err |= utf8stream_writer_write_str( out_english_report, "\n" );

    U8_TRACE_END_ERR( write_err );
    return write_err;
}

u8_error_t main_commands_private_report_error_info ( main_commands_t *this_,
                                                     const u8_error_info_t *error_info,
                                                     utf8stream_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert( error_info != NULL );
    assert( out_english_report != NULL );
    u8_error_t write_err = U8_ERROR_NONE;

    if ( u8_error_info_is_error( error_info ) )
    {
        char info_string_buf[80];
        utf8stringbuf_t info_string = UTF8STRINGBUF(info_string_buf);
        utf8stringbuf_clear( &info_string );
        {
            gui_error_info_printer_t my_err_info_printer;
            gui_error_info_printer_init( &my_err_info_printer );
            write_err |= gui_error_info_printer_show_error_info( &my_err_info_printer, error_info, info_string );
            gui_error_info_printer_destroy( &my_err_info_printer );
        }
        write_err |= utf8stream_writer_write_str( out_english_report, utf8stringbuf_get_string( &info_string ) );
        write_err |= utf8stream_writer_write_str( out_english_report, "\n" );
    }

    U8_TRACE_END_ERR( write_err );
    return write_err;
}


/*
Copyright 2016-2025 Andreas Warnke

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
