/* File: io_importer.c; Copyright and License: see below */

#include "io_importer.h"
#include "json/json_element_reader.h"
#include "u8stream/universal_file_input_stream.h"
#include "u8stream/universal_memory_input_stream.h"
#include "u8stream/universal_null_output_stream.h"
#include "u8/u8_error.h"
#include "utf8stringbuf/utf8string.h"
#include "u8/u8_trace.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <stdbool.h>

void io_importer_init ( io_importer_t *this_,
                        data_database_reader_t *db_reader,
                        ctrl_controller_t *controller )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != controller );

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;

    U8_TRACE_END();
}

void io_importer_destroy ( io_importer_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( NULL != (*this_).db_reader );
    assert( NULL != (*this_).controller );

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    U8_TRACE_END();
}

u8_error_t io_importer_import_clipboard( io_importer_t *this_,
                                         const char *json_text,
                                         data_row_id_t diagram_id,
                                         data_stat_t *io_stat,
                                         u8_error_info_t *out_err_info )
{
    U8_TRACE_BEGIN();
    assert( NULL != json_text );
    assert( out_err_info != NULL );
    u8_error_info_init_void( out_err_info );
    u8_error_t result = U8_ERROR_NONE;

    universal_null_output_stream_t null_out;
    universal_null_output_stream_init( &null_out );
    universal_utf8_writer_t out_writer;
    universal_utf8_writer_init( &out_writer, universal_null_output_stream_get_output_stream( &null_out ) );

    io_import_elements_init_for_paste( &((*this_).temp_elements_importer),
                                       diagram_id,
                                       (*this_).db_reader,
                                       (*this_).controller,
                                       io_stat,
                                       &out_writer
                                     );
    json_importer_init( &((*this_).temp_json_importer), &((*this_).temp_elements_importer) );

    universal_memory_input_stream_t in_mem_stream;
    universal_memory_input_stream_init( &in_mem_stream, json_text, strlen(json_text) );
    universal_input_stream_t* in_stream = universal_memory_input_stream_get_input_stream( &in_mem_stream );

    result = json_importer_import_stream( &((*this_).temp_json_importer),
                                          in_stream,
                                          out_err_info
                                        );

    universal_memory_input_stream_destroy( &in_mem_stream );

    json_importer_destroy( &((*this_).temp_json_importer) );
    io_import_elements_destroy( &((*this_).temp_elements_importer) );

    universal_utf8_writer_destroy( &out_writer );
    universal_null_output_stream_destroy( &null_out );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t io_importer_import_file( io_importer_t *this_,
                                    io_import_mode_t import_mode,
                                    const char *import_file_path,
                                    data_stat_t *io_stat,
                                    u8_error_info_t *out_err_info,
                                    universal_utf8_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert( import_file_path != NULL );
    assert( io_stat != NULL );
    assert( out_english_report != NULL );
    assert( out_err_info != NULL );
    u8_error_info_init_void( out_err_info );
    u8_error_t parse_error = U8_ERROR_NONE;

    /* open file */
    universal_file_input_stream_t in_file;
    universal_file_input_stream_init( &in_file );
    parse_error |= universal_file_input_stream_open( &in_file, import_file_path );

    /* import from stream */
    if ( parse_error == U8_ERROR_NONE )
    {
        universal_input_stream_t *const in_stream = universal_file_input_stream_get_input_stream( &in_file );
        parse_error = io_importer_import_stream( this_, import_mode, in_stream, io_stat, out_err_info, out_english_report );
    }

    /* close file */
    parse_error |= universal_file_input_stream_close( &in_file );
    parse_error |= universal_file_input_stream_destroy( &in_file );

    U8_TRACE_END_ERR( parse_error );
    return parse_error;
}

u8_error_t io_importer_import_stream( io_importer_t *this_,
                                      io_import_mode_t import_mode,
                                      universal_input_stream_t *in_stream,
                                      data_stat_t *io_stat,
                                      u8_error_info_t *out_err_info,
                                      universal_utf8_writer_t *out_english_report )
{
    U8_TRACE_BEGIN();
    assert( in_stream != NULL );
    assert( io_stat != NULL );
    assert( out_english_report != NULL );
    assert( out_err_info != NULL );
    u8_error_info_init_void( out_err_info );
    u8_error_t parse_error = U8_ERROR_NONE;

    io_import_elements_init( &((*this_).temp_elements_importer),
                             (*this_).db_reader,
                             (*this_).controller,
                             io_stat,
                             out_english_report
                           );
    json_importer_init( &((*this_).temp_json_importer), &((*this_).temp_elements_importer) );

    /* check json structure */
    if ( parse_error == U8_ERROR_NONE )
    {
        U8_TRACE_INFO("scanning file...");
        static const char *const PASS_CHECK_TITLE
            = "PASS: Check that the file structure is valid\n      ";
        universal_utf8_writer_write_str( out_english_report, PASS_CHECK_TITLE );

        io_import_elements_set_mode( &((*this_).temp_elements_importer), IO_IMPORT_MODE_CHECK );
        parse_error = json_importer_import_stream( &((*this_).temp_json_importer),
                                                   in_stream,
                                                   out_err_info
                                                 );

        universal_utf8_writer_write_str( out_english_report, "\n" );
    }

    /* reset file */
    if ( parse_error == U8_ERROR_NONE )
    {
        parse_error |= universal_input_stream_reset( in_stream );
    }

    /* start an outer transaction to speed up the creation of objects */
    parse_error |= ctrl_controller_transaction_begin( (*this_).controller );

    /* import: create elements */
    if (( import_mode != IO_IMPORT_MODE_CHECK )&&( parse_error == U8_ERROR_NONE ))
    {
        U8_TRACE_INFO("importing file...");
        static const char *const PASS_CREATE_TITLE
            = "PASS: Create diagrams, classifiers and features\n      ";
        universal_utf8_writer_write_str( out_english_report, PASS_CREATE_TITLE );

        io_import_elements_set_mode( &((*this_).temp_elements_importer), IO_IMPORT_MODE_CREATE );
        parse_error = json_importer_import_stream( &((*this_).temp_json_importer),
                                                   in_stream,
                                                   out_err_info
                                                 );

        universal_utf8_writer_write_str( out_english_report, "\n" );
    }

    /* reset file once more */
    if ( parse_error == U8_ERROR_NONE )
    {
        parse_error |= universal_input_stream_reset( in_stream );
    }

    /* import: create elements */
    if (( import_mode != IO_IMPORT_MODE_CHECK )&&( parse_error == U8_ERROR_NONE ))
    {
        U8_TRACE_INFO("importing file...");
        static const char *const PASS_LINK_TITLE
            = "PASS: Link diagrams to parents, classifiers to diagrams, create relationships\n      ";
        universal_utf8_writer_write_str( out_english_report, PASS_LINK_TITLE );

        io_import_elements_set_mode( &((*this_).temp_elements_importer), IO_IMPORT_MODE_LINK );
        parse_error = json_importer_import_stream( &((*this_).temp_json_importer),
                                                   in_stream,
                                                   out_err_info
                                                 );

        universal_utf8_writer_write_str( out_english_report, "\n" );
    }

    /* commit the outer transaction */
    parse_error |= ctrl_controller_transaction_commit( (*this_).controller );

    json_importer_destroy( &((*this_).temp_json_importer) );
    io_import_elements_destroy( &((*this_).temp_elements_importer) );

    U8_TRACE_END_ERR( parse_error );
    return parse_error;
}


/*
Copyright 2021-2022 Andreas Warnke

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
