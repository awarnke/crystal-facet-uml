/* File: io_importer.c; Copyright and License: see below */

#include "io_importer.h"
#include "json/json_serializer.h"
#include "json/json_deserializer.h"
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

    (*this_).db_reader = db_reader;
    (*this_).controller = controller;

    TRACE_END();
}

void io_importer_destroy ( io_importer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).controller = NULL;

    TRACE_END();
}

data_error_t io_importer_import_buf_to_db( io_importer_t *this_,
                                                       const char *json_text,
                                                       data_row_id_t diagram_id,
                                                       data_stat_t *io_stat,
                                                       uint32_t *out_read_pos )
{
    TRACE_BEGIN();
    assert( NULL != json_text );
    assert( NULL != io_stat );
    assert( NULL != out_read_pos );

    json_deserializer_t deserializer;
    data_error_t parse_error = DATA_ERROR_NONE;

    TRACE_INFO ( json_text );

    json_deserializer_init( &deserializer, json_text );
    data_row_id_t current_diagram_id = diagram_id;

    /* check if diagram id exists */
    {
        static data_diagram_t dummy_diagram;
        parse_error = data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_id, &dummy_diagram );
        if ( DATA_ERROR_NONE != parse_error )
        {
            TSLOG_ERROR_INT( "diagram id where to import json data does not exist (anymore)", diagram_id );
        }
    }

    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_begin_data( &deserializer );
    }


    if ( DATA_ERROR_NONE == parse_error )
    {
        parse_error = json_deserializer_expect_end_data( &deserializer );
    }

    json_deserializer_get_read_pos ( &deserializer, out_read_pos );
    json_deserializer_destroy( &deserializer );

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
