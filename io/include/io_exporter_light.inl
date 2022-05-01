/* File: io_exporter_light.h; Copyright and License: see below */

#include "json/json_element_writer.h"
#include "json/json_writer_pass.h"
#include "u8stream/universal_output_stream.h"
#include "u8stream/universal_memory_output_stream.h"
#include <assert.h>

static inline void io_exporter_light_init ( io_exporter_light_t *this_, data_database_reader_t *db_reader )
{
    assert( NULL != db_reader );
    (*this_).db_reader = db_reader;
}

static inline void io_exporter_light_destroy ( io_exporter_light_t *this_ )
{
    assert( NULL != (*this_).db_reader );
    (*this_).db_reader = NULL;
}

static inline int io_exporter_light_export_set_to_buf( io_exporter_light_t *this_,
                                                       const data_small_set_t *set_to_be_exported,
                                                       data_stat_t *io_export_stat,
                                                       utf8stringbuf_t out_buf )
{
    assert( NULL != set_to_be_exported );
    assert( NULL != io_export_stat );
    int exp_err = 0;
    data_stat_t count_just_once;
    data_stat_init( &count_just_once );

    /* initialize an output stream */
    universal_memory_output_stream_t memout;
    universal_memory_output_stream_init( &memout,
                                         utf8stringbuf_get_string( out_buf ),
                                         utf8stringbuf_get_size( out_buf )
                                       );
    universal_output_stream_t *output;
    output = universal_memory_output_stream_get_output_stream( &memout );

    /* initialize an element writer */
    json_element_writer_t json_writer;
    json_element_writer_init( &json_writer, io_export_stat, output );
    io_element_writer_t *element_writer;
    element_writer = json_element_writer_get_element_writer( &json_writer );

    /* initialize a traversal */
    io_export_set_traversal_init( &((*this_).temp_set_traversal),
                                  (*this_).db_reader,
                                  &count_just_once,
                                  element_writer
                                );

    /* do traversal */
    const char *const document_title = "";
    exp_err |= io_element_writer_write_header( element_writer, document_title );
    json_element_writer_set_mode( &json_writer, JSON_WRITER_PASS_VIEWS );
    exp_err |= io_export_set_traversal_export_set( &((*this_).temp_set_traversal), set_to_be_exported );
    data_stat_add( io_export_stat, &count_just_once );  /* after the first pass, report the error statistics */
    json_element_writer_set_mode( &json_writer, JSON_WRITER_PASS_NODES );
    exp_err |= io_export_set_traversal_export_set( &((*this_).temp_set_traversal), set_to_be_exported );
    json_element_writer_set_mode( &json_writer, JSON_WRITER_PASS_EDGES );
    exp_err |= io_export_set_traversal_export_set( &((*this_).temp_set_traversal), set_to_be_exported );
    exp_err |= io_element_writer_write_footer( element_writer );

    /* de-initialize a traversal */
    io_export_set_traversal_destroy ( &((*this_).temp_set_traversal) );

    /* de-initialize an element writer */
    json_element_writer_destroy( &json_writer );

    /* de-initialize an output stream */
    exp_err |= universal_memory_output_stream_write_0term( &memout );
    universal_memory_output_stream_destroy( &memout );

    data_stat_destroy( &count_just_once );
    return exp_err;
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
