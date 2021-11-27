/* File: io_importer.c; Copyright and License: see below */

#include "io_importer.h"
#include "json/json_deserializer.h"
#include "stream/universal_file_input_stream.h"
#include "ctrl_error.h"
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

    json_import_to_database_init( &((*this_).json_importer), db_reader, controller );

    TRACE_END();
}

void io_importer_destroy ( io_importer_t *this_ )
{
    TRACE_BEGIN();

    json_import_to_database_destroy( &((*this_).json_importer) );

    TRACE_END();
}

data_error_t io_importer_import_file( io_importer_t *this_,
                                      io_file_format_t import_format,
                                      const char *import_file_path,
                                      data_stat_t *io_stat,
                                      universal_utf8_writer_t *out_english_report )
{
    TRACE_BEGIN();
    assert( import_file_path != NULL );
    assert( io_stat != NULL );
    assert( out_english_report != NULL );
    data_error_t parse_error = DATA_ERROR_NONE;

    universal_utf8_writer_write_str( out_english_report, "importing not yet implemented.\n" );

    /*int id_mapper;*/

    /* open file */
    universal_file_input_stream_t in_file;
    universal_file_input_stream_init( &in_file );
    const int err1 = universal_file_input_stream_open( &in_file, import_file_path );
    if ( err1 != 0 )
    {
        parse_error = DATA_ERROR_INVALID_REQUEST;
    }
    else
    {
        /* id scan */
        TRACE_INFO("scanning file...");

        /* create a list of diagram elements, check if the referenced diagrams and classifiers really exist */

        const data_error_t err3 = json_import_to_database_prescan( &((*this_).json_importer),
                                                                   universal_file_input_stream_get_input_stream( &in_file ),
                                                                   io_stat,
                                                                   out_english_report
                                                                 );
        if ( err3 != 0 )
        {
            parse_error = DATA_ERROR_AT_FILE_READ;
        }

        /* import */
        TRACE_INFO("importing file...");
        const int err5 = universal_file_input_stream_reset( &in_file );
        if ( err5 != 0 )
        {
            parse_error = DATA_ERROR_AT_FILE_READ;
        }
        else
        {
            /* intelligent loop */
            /*     import lightweight diagram without description */
            /* create fake diagrams for orphaned classifiers */
            /* loop */
            /*     update diagram descriptions */
            /*     import classifier and diagramelements */
            /*     import features */
            /* loop */
            /*     import relationship */
        }

        /* close */
        const int err7 = universal_file_input_stream_close( &in_file );
        if ( err7 != 0 )
        {
            parse_error = DATA_ERROR_AT_FILE_READ;
        }
    }
    const int err8 = universal_file_input_stream_destroy( &in_file );
    if ( err8  != 0 )
    {
        parse_error = DATA_ERROR_AT_FILE_READ;
    }

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
