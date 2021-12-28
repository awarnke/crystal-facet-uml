/* File: io_importer.c; Copyright and License: see below */

#include "io_importer.h"
#include "json/json_element_reader.h"
#include "stream/universal_file_input_stream.h"
#include "stream/universal_memory_input_stream.h"
#include "u8/u8_error.h"
#include "util/string/utf8string.h"
#include "trace.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void io_importer_init ( io_importer_t *this_,
                        data_database_reader_t *db_reader,
                        ctrl_controller_t *controller )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != controller );

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;

    TRACE_END();
}

void io_importer_destroy ( io_importer_t *this_ )
{
    TRACE_BEGIN();
    assert( NULL != (*this_).db_reader );
    assert( NULL != (*this_).controller );

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    TRACE_END();
}

u8_error_t io_importer_import_clipboard( io_importer_t *this_,
                                         const char *json_text,
                                         data_row_id_t diagram_id,
                                         data_stat_t *io_stat,
                                         uint32_t *out_read_line )
{
    TRACE_BEGIN();
    assert( NULL != json_text );
    u8_error_t result = U8_ERROR_NONE;

    io_import_elements_init_for_paste( &((*this_).temp_elements_importer),
                                       diagram_id,
                                       (*this_).db_reader,
                                       (*this_).controller,
                                       io_stat
                                     );
    json_importer_init( &((*this_).temp_json_importer), &((*this_).temp_elements_importer), io_stat );

    universal_memory_input_stream_t in_mem_stream;
    universal_memory_input_stream_init( &in_mem_stream, json_text, strlen(json_text) );
    universal_input_stream_t* in_stream = universal_memory_input_stream_get_input_stream( &in_mem_stream );

    result = json_importer_import_stream( &((*this_).temp_json_importer),
                                          in_stream,
                                          out_read_line
                                        );

    universal_memory_input_stream_destroy( &in_mem_stream );

    json_importer_destroy( &((*this_).temp_json_importer) );
    io_import_elements_destroy( &((*this_).temp_elements_importer) );

    TRACE_END_ERR( result );
    return result;
}

u8_error_t io_importer_import_file( io_importer_t *this_,
                                    io_import_mode_t import_mode,
                                    const char *import_file_path,
                                    data_stat_t *io_stat,
                                    universal_utf8_writer_t *out_english_report )
{
    TRACE_BEGIN();
    assert( import_file_path != NULL );
    assert( io_stat != NULL );
    assert( out_english_report != NULL );
    u8_error_t parse_error = U8_ERROR_NONE;

    io_import_elements_init( &((*this_).temp_elements_importer), (*this_).db_reader, (*this_).controller, io_stat );
    json_importer_init( &((*this_).temp_json_importer), &((*this_).temp_elements_importer), io_stat );

    /* open file */
    universal_file_input_stream_t in_file;
    universal_file_input_stream_init( &in_file );
    parse_error |= universal_file_input_stream_open( &in_file, import_file_path );
    universal_utf8_writer_write_str( out_english_report, "PASS: CHECK\n" );

    /* check json structure */
    if ( parse_error == U8_ERROR_NONE )
    {
        TRACE_INFO("scanning file...");
        uint32_t error_line;
        io_import_elements_set_mode( &((*this_).temp_elements_importer), IO_IMPORT_MODE_CHECK );
        parse_error = json_importer_import_stream( &((*this_).temp_json_importer),
                                                   universal_file_input_stream_get_input_stream( &in_file ),
                                                   &error_line
                                                 );
        if ( parse_error != U8_ERROR_NONE )
        {
            const u8_error_t log_err
                = universal_utf8_writer_write_str( out_english_report, "error in input file, line: " )
                | universal_utf8_writer_write_int( out_english_report, error_line )
                | universal_utf8_writer_write_str( out_english_report, "\n" );
            parse_error |= log_err;
        }

    }

    /* reset file */
    if ( parse_error == U8_ERROR_NONE )
    {
        parse_error |= universal_file_input_stream_reset( &in_file );
        universal_utf8_writer_write_str( out_english_report, "PASS: CREATE\n" );
    }

    /* import: create elements */
    if (( import_mode != IO_IMPORT_MODE_CHECK )&&( parse_error == U8_ERROR_NONE ))
    {
        TRACE_INFO("importing file...");

        uint32_t error_line;
        io_import_elements_set_mode( &((*this_).temp_elements_importer), IO_IMPORT_MODE_CREATE );
        parse_error = json_importer_import_stream( &((*this_).temp_json_importer),
                                                   universal_file_input_stream_get_input_stream( &in_file ),
                                                   &error_line
                                                 );
        if ( parse_error != U8_ERROR_NONE )
        {
            const u8_error_t log_err
                = universal_utf8_writer_write_str( out_english_report, "error in input file, line: " )
                | universal_utf8_writer_write_int( out_english_report, error_line )
                | universal_utf8_writer_write_str( out_english_report, "\n" );
            parse_error |= log_err;
        }
    }

    /* reset file once more */
    if ( parse_error == U8_ERROR_NONE )
    {
        parse_error |= universal_file_input_stream_reset( &in_file );
        universal_utf8_writer_write_str( out_english_report, "PASS: LINK\n" );
    }

    /* import: create elements */
    if (( import_mode != IO_IMPORT_MODE_CHECK )&&( parse_error == U8_ERROR_NONE ))
    {
        TRACE_INFO("importing file...");

        uint32_t error_line;
        io_import_elements_set_mode( &((*this_).temp_elements_importer), IO_IMPORT_MODE_LINK );
        parse_error = json_importer_import_stream( &((*this_).temp_json_importer),
                                                   universal_file_input_stream_get_input_stream( &in_file ),
                                                   &error_line
                                                 );
        if ( parse_error != U8_ERROR_NONE )
        {
            const u8_error_t log_err
                = universal_utf8_writer_write_str( out_english_report, "error in input file, line: " )
                | universal_utf8_writer_write_int( out_english_report, error_line )
                | universal_utf8_writer_write_str( out_english_report, "\n" );
            parse_error |= log_err;
        }
    }

    /* close file */
    parse_error |= universal_file_input_stream_close( &in_file );
    parse_error |= universal_file_input_stream_destroy( &in_file );

    json_importer_destroy( &((*this_).temp_json_importer) );
    io_import_elements_destroy( &((*this_).temp_elements_importer) );

    TRACE_END_ERR( parse_error );
    return parse_error;
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
