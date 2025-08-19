/* File: document_link_provider.c; Copyright and License: see below */

#include "document/document_link_provider.h"
#include "utf8stringbuf/utf8stringview.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void document_link_provider_init( document_link_provider_t *this_,
                                  data_database_reader_t *db_reader,
                                  const char * tag_xref_separator,
                                  const char * tag_xref_start,
                                  const char * tag_xref_middle,
                                  const char * tag_xref_end,
                                  io_xml_writer_t *xml_writer )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != tag_xref_separator );
    assert( NULL != tag_xref_start );
    assert( NULL != tag_xref_middle );
    assert( NULL != tag_xref_end );
    assert( NULL != xml_writer );

    (*this_).db_reader = db_reader;
    (*this_).tag_xref_separator = tag_xref_separator;
    (*this_).tag_xref_start = tag_xref_start;
    (*this_).tag_xref_middle = tag_xref_middle;
    (*this_).tag_xref_end = tag_xref_end;
    (*this_).xml_writer = xml_writer;

    U8_TRACE_END();
}

void document_link_provider_destroy( document_link_provider_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).xml_writer = NULL;

    U8_TRACE_END();
}

u8_error_t document_link_provider_write_occurrences( document_link_provider_t *this_,
                                                     data_id_t classifier_id )
{
    U8_TRACE_BEGIN();
    assert ( NULL != (*this_).db_reader );
    assert ( DATA_TABLE_CLASSIFIER == data_id_get_table( &classifier_id ) );
    u8_error_t db_err = U8_ERROR_NONE;
    u8_error_t export_err = U8_ERROR_NONE;

    /* read from database */
    data_diagram_iterator_t diagram_iterator;
    db_err |= data_diagram_iterator_init_empty( &diagram_iterator );
    db_err |= data_database_reader_get_diagrams_by_classifier_id( (*this_).db_reader,
                                                                  data_id_get_row_id( &classifier_id ),
                                                                  &diagram_iterator
                                                                );
    bool is_first = true;
    while( data_diagram_iterator_has_next( &diagram_iterator ) )
    {
        db_err |= data_diagram_iterator_next( &diagram_iterator, &((*this_).temp_diagram) );

        /* write to sink */
        if ( is_first )
        {
            is_first = false;
        }
        else
        {
            export_err |= io_xml_writer_write_plain ( (*this_).xml_writer, (*this_).tag_xref_separator );
        }
        const data_id_t diag_ref_id = data_diagram_get_data_id( &((*this_).temp_diagram) );
        const char *const diag_ref_name = data_diagram_get_name_const( &((*this_).temp_diagram) );
        export_err |= io_xml_writer_write_plain ( (*this_).xml_writer, (*this_).tag_xref_start );
        export_err |= io_xml_writer_write_plain_id( (*this_).xml_writer, diag_ref_id );
        export_err |= io_xml_writer_write_plain ( (*this_).xml_writer, (*this_).tag_xref_middle );
        export_err |= io_xml_writer_write_xml_enc( (*this_).xml_writer, diag_ref_name );
        export_err |= io_xml_writer_write_plain ( (*this_).xml_writer, (*this_).tag_xref_end );
    }
    db_err |= data_diagram_iterator_destroy( &diagram_iterator );

    U8_TRACE_END_ERR( db_err || export_err );
    return ( db_err || export_err );
}


/*
Copyright 2023-2025 Andreas Warnke

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
