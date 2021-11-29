/* File: main_commands.c; Copyright and License: see below */

#include "main_commands.h"
#include "gui_main.h"
#include "io_exporter.h"
#include "io_importer.h"
#include "trace.h"
#include "tslog.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <assert.h>

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
            /* no error here, if no gui requested - test fail otherwise */
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

int main_commands_upgrade ( main_commands_t *this_, const char *database_path, universal_utf8_writer_t *out_english_report )
{
    TRACE_BEGIN();
    int result = 0;

    assert( database_path != NULL );

    TRACE_INFO("starting DB...");
    data_database_init( &((*this_).temp_database) );

    TRACE_INFO("upgrading DB...");
    const data_error_t up_err
        = data_database_open( &((*this_).temp_database), database_path );  /* upgrade is implicitely done */
    if ( up_err != DATA_ERROR_NONE )
    {
        universal_utf8_writer_write_str( out_english_report, "error opening database " );
        universal_utf8_writer_write_str( out_english_report, database_path );
        universal_utf8_writer_write_str( out_english_report, "\n" );
        result = -1;
    }

    TRACE_INFO("stopping DB...");
    data_database_close( &((*this_).temp_database) );
    data_database_destroy( &((*this_).temp_database) );

    TRACE_END_ERR( result );
    return result;
}

int main_commands_repair ( main_commands_t *this_,
                           const char *database_path,
                           bool check_only,
                           universal_utf8_writer_t *out_english_report )
{
    TRACE_BEGIN();
    assert( database_path != NULL );
    const bool do_repair = ( ! check_only );
    static char repair_log_buffer[10000] = "";
    static utf8stringbuf_t repair_log = UTF8STRINGBUF( repair_log_buffer );
    int result = 0;

    TRACE_INFO("starting DB...");
    data_database_init( &((*this_).temp_database) );
    const data_error_t db_err
        = ( check_only )
        ? data_database_open_read_only( &((*this_).temp_database), database_path )
        : data_database_open( &((*this_).temp_database), database_path );
    if ( db_err != DATA_ERROR_NONE )
    {
        universal_utf8_writer_write_str( out_english_report, "error opening database " );
        universal_utf8_writer_write_str( out_english_report, database_path );
        universal_utf8_writer_write_str( out_english_report, "\n" );
    }

    TRACE_INFO("initializing controller...");
    ctrl_controller_init( &((*this_).temp_controller), &((*this_).temp_database) );

    TRACE_INFO("reparing/testing...");
    ctrl_controller_repair_database( &((*this_).temp_controller), do_repair, NULL, NULL, repair_log );
    TRACE_INFO("reparing/testing finished.");

    TRACE_INFO("destroying controller...");
    ctrl_controller_destroy( &((*this_).temp_controller) );

    TRACE_INFO("stopping DB...");
    data_database_close( &((*this_).temp_database) );
    data_database_destroy( &((*this_).temp_database) );

    universal_utf8_writer_write_str( out_english_report, "\n\n" );
    universal_utf8_writer_write_str( out_english_report, utf8stringbuf_get_string(repair_log) );
    universal_utf8_writer_write_str( out_english_report, "\n" );

    TRACE_END_ERR( result );
    return result;
}

int main_commands_start_gui ( main_commands_t *this_, const char *database_path, universal_utf8_writer_t *out_english_report )
{
    TRACE_BEGIN();
    int result = 0;

    TRACE_INFO("starting DB...");
    TRACE_INFO_INT("sizeof(data_database_t)/B:",sizeof(data_database_t));
    data_database_init( &((*this_).temp_database) );
    if ( NULL != database_path )
    {
        const data_error_t db_err
            = data_database_open( &((*this_).temp_database), database_path );
        if ( db_err != DATA_ERROR_NONE )
        {
            universal_utf8_writer_write_str( out_english_report, "error opening database " );
            universal_utf8_writer_write_str( out_english_report, database_path );
            universal_utf8_writer_write_str( out_english_report, "\n" );
        }
    }

    TRACE_TIMESTAMP();
    TRACE_INFO("initializing controller...");
    TRACE_INFO_INT("sizeof(ctrl_controller_t)/B:",sizeof(ctrl_controller_t));
    ctrl_controller_init( &((*this_).temp_controller), &((*this_).temp_database) );

    TRACE_TIMESTAMP();
    TRACE_INFO("running GUI...");
    gui_main( &((*this_).temp_controller), &((*this_).temp_database) );
    TRACE_INFO("GUI stopped.");

    TRACE_TIMESTAMP();
    TRACE_INFO("destroying controller...");
    ctrl_controller_destroy( &((*this_).temp_controller) );

    TRACE_TIMESTAMP();
    TRACE_INFO("stopping DB...");
    data_database_close( &((*this_).temp_database) );
    data_database_destroy( &((*this_).temp_database) );

    TRACE_END_ERR( result );
    return result;
}

int main_commands_export ( main_commands_t *this_,
                           const char *database_path,
                           io_file_format_t export_format,
                           const char *export_directory,
                           universal_utf8_writer_t *out_english_report )
{
    TRACE_BEGIN();
    assert( database_path != NULL );
    assert( export_directory != NULL );
    int export_err = 0;

    TRACE_INFO("starting DB...");
    data_database_init( &((*this_).temp_database) );
    const data_error_t db_err
        = data_database_open_read_only( &((*this_).temp_database), database_path );
    if ( db_err != DATA_ERROR_NONE )
    {
            universal_utf8_writer_write_str( out_english_report, "error opening database " );
            universal_utf8_writer_write_str( out_english_report, database_path );
            universal_utf8_writer_write_str( out_english_report, "\n" );
    }

    TRACE_INFO("exporting DB...");
    TRACE_INFO_STR( "chosen folder:", export_directory );
    const char *document_filename = data_database_get_filename_ptr ( &((*this_).temp_database) );
    if ( data_database_is_open( &((*this_).temp_database) ) )
    {
        static data_database_reader_t db_reader;
        data_database_reader_init( &db_reader, &((*this_).temp_database) );
        static io_exporter_t exporter;
        io_exporter_init( &exporter, &db_reader );
        {
            data_stat_t export_stat;
            data_stat_init ( &export_stat );
            export_err = io_exporter_export_files( &exporter, export_format, export_directory, document_filename, &export_stat );
            {
                const unsigned int stat_ok = data_stat_get_series_count( &export_stat, DATA_STAT_SERIES_EXPORTED );
                const unsigned int stat_warn = data_stat_get_series_count( &export_stat, DATA_STAT_SERIES_WARNING );
                const unsigned int stat_err = data_stat_get_series_count( &export_stat, DATA_STAT_SERIES_ERROR );
                universal_utf8_writer_write_str( out_english_report, "\nexported: " );
                universal_utf8_writer_write_int( out_english_report, stat_ok );
                universal_utf8_writer_write_str( out_english_report, "; warnings: " );
                universal_utf8_writer_write_int( out_english_report, stat_warn );
                universal_utf8_writer_write_str( out_english_report, "; errors: " );
                universal_utf8_writer_write_int( out_english_report, stat_err );
                universal_utf8_writer_write_str( out_english_report, "\n" );
            }
            data_stat_trace( &export_stat );
            data_stat_destroy ( &export_stat );
        }
        io_exporter_destroy( &exporter );
        data_database_reader_destroy( &db_reader );
    }
    else
    {
        export_err = -1;
    }

    TRACE_INFO("stopping DB...");
    data_database_close( &((*this_).temp_database) );
    data_database_destroy( &((*this_).temp_database) );

    TRACE_END_ERR( export_err );
    return export_err;
}

int main_commands_import ( main_commands_t *this_,
                           const char *database_path,
                           io_file_format_t import_format,
                           const char *import_file_path,
                           universal_utf8_writer_t *out_english_report )
{
    TRACE_BEGIN();
    assert( database_path != NULL );
    assert( import_file_path != NULL );
    int import_err = 0;

    TRACE_INFO("starting DB...");
    data_database_init( &((*this_).temp_database) );
    const data_error_t db_err
        = data_database_open( &((*this_).temp_database), database_path );
    if ( db_err != DATA_ERROR_NONE )
    {
        universal_utf8_writer_write_str( out_english_report, "error opening database " );
        universal_utf8_writer_write_str( out_english_report, database_path );
        universal_utf8_writer_write_str( out_english_report, "\n" );
    }

    TRACE_INFO("initializing controller...");
    ctrl_controller_init( &((*this_).temp_controller), &((*this_).temp_database) );

    TRACE_INFO("importing data...");
    TRACE_INFO_STR( "chosen data:", import_file_path );
    if ( data_database_is_open( &((*this_).temp_database) ) )
    {
        static data_database_reader_t db_reader;
        data_database_reader_init( &db_reader, &((*this_).temp_database) );
        static io_importer_t importer;
        io_importer_init( &importer, &db_reader, &((*this_).temp_controller) );
        {
            data_stat_t import_stat;
            data_stat_init ( &import_stat );
            import_err = io_importer_import_file( &importer, import_format, import_file_path, &import_stat, out_english_report );
            {
                const unsigned int stat_ok = data_stat_get_series_count( &import_stat, DATA_STAT_SERIES_EXPORTED );
                const unsigned int stat_warn = data_stat_get_series_count( &import_stat, DATA_STAT_SERIES_WARNING );
                const unsigned int stat_err = data_stat_get_series_count( &import_stat, DATA_STAT_SERIES_ERROR );
                universal_utf8_writer_write_str( out_english_report, "\nimported: " );
                universal_utf8_writer_write_int( out_english_report, stat_ok );
                universal_utf8_writer_write_str( out_english_report, "; warnings: " );
                universal_utf8_writer_write_int( out_english_report, stat_warn );
                universal_utf8_writer_write_str( out_english_report, "; errors: " );
                universal_utf8_writer_write_int( out_english_report, stat_err );
                universal_utf8_writer_write_str( out_english_report, "\n" );
            }
            data_stat_trace( &import_stat );
            data_stat_destroy ( &import_stat );
        }
        io_importer_destroy( &importer );
        data_database_reader_destroy( &db_reader );
    }
    else
    {
        import_err = -1;
    }

    TRACE_INFO("destroying controller...");
    ctrl_controller_destroy( &((*this_).temp_controller) );

    TRACE_INFO("stopping DB...");
    data_database_close( &((*this_).temp_database) );
    data_database_destroy( &((*this_).temp_database) );

    TRACE_END_ERR( import_err );
    return import_err;
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
