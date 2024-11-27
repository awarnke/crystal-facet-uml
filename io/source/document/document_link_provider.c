/* File: document_link_provider.c; Copyright and License: see below */

#include "document/document_link_provider.h"
#include "utf8stringbuf/utf8stringview.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void document_link_provider_init( document_link_provider_t *this_,
                                  data_database_reader_t *db_reader )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );

    (*this_).db_reader = db_reader;

    U8_TRACE_END();
}

void document_link_provider_destroy( document_link_provider_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;

    U8_TRACE_END();
}

u8_error_t document_link_provider_get_occurrences( document_link_provider_t *this_,
                                                   data_id_t classifier_id,
                                                   data_diagram_t (**out_diagram)[],
                                                   uint32_t *out_diagram_count )
{
    U8_TRACE_BEGIN();
    assert ( NULL != (*this_).db_reader );
    assert ( DATA_TABLE_CLASSIFIER == data_id_get_table( &classifier_id ) );
    assert ( NULL != out_diagram );
    assert ( NULL != out_diagram_count );
    u8_error_t db_err = U8_ERROR_NONE;

    data_diagram_iterator_t diagram_iterator;
    db_err |= data_diagram_iterator_init_empty( &diagram_iterator );
    db_err |= data_database_reader_get_diagrams_by_classifier_id( (*this_).db_reader,
                                                                  data_id_get_row_id( &classifier_id ),
                                                                  &diagram_iterator
                                                                );
    uint_fast32_t next_idx = 0;
    for ( ; (next_idx < DOCUMENT_LINK_PROVIDER_MAX_LINKS) && data_diagram_iterator_has_next( &diagram_iterator ); next_idx++ )
    {
        db_err |= data_diagram_iterator_next( &diagram_iterator, &( (*this_).temp_diagrams[next_idx] ) );
    }
    if ( data_diagram_iterator_has_next( &diagram_iterator ) )
    {
        /* This is not an error - rather expected - shuold only be traced...*/
        U8_TRACE_INFO_INT( "document_link_provider_get_occurrences finds too many classifier occurrences", DOCUMENT_LINK_PROVIDER_MAX_LINKS );
    }
    db_err |= data_diagram_iterator_destroy( &diagram_iterator );

    *out_diagram = &((*this_).temp_diagrams);
    *out_diagram_count = next_idx;

    U8_TRACE_END_ERR( db_err );
    return db_err;
}


/*
Copyright 2023-2024 Andreas Warnke

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
