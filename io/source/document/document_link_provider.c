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
                                  const char * tag_xref_start,
                                  const char * tag_xref_middle,
                                  const char * tag_xref_end,
                                  const char * tag_xref_next,
                                  const char * tag_xref_more,
                                  xml_writer_t *sink )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != tag_xref_start );
    assert( NULL != tag_xref_middle );
    assert( NULL != tag_xref_end );
    assert( NULL != tag_xref_next );
    assert( NULL != tag_xref_more );
    assert( NULL != sink );

    (*this_).db_reader = db_reader;
    (*this_).tag_xref_start = tag_xref_start;
    (*this_).tag_xref_middle = tag_xref_middle;
    (*this_).tag_xref_end = tag_xref_end;
    (*this_).tag_xref_next = tag_xref_next;
    (*this_).tag_xref_more = tag_xref_more;
    (*this_).sink = sink;

    U8_TRACE_END();
}

void document_link_provider_destroy( document_link_provider_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).sink = NULL;
    (*this_).db_reader = NULL;

    U8_TRACE_END();
}

u8_error_t document_link_provider_write_occurrences( document_link_provider_t *this_, const char *text )
{
    U8_TRACE_BEGIN();
    assert ( NULL != text );
    assert ( NULL != (*this_).db_reader );
    assert ( NULL != (*this_).sink );
    u8_error_t write_err = U8_ERROR_NONE;

    U8_TRACE_END_ERR( write_err );
    return write_err;
}


/*
Copyright 2023-2023 Andreas Warnke

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
