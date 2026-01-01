/* File: utf8stream_writemem.c; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <string.h>
#include <inttypes.h>
#include <assert.h>

static inline void utf8stream_writemem_init( utf8stream_writemem_t *this_, void* mem_buf_start, size_t mem_buf_size )
{
    assert( mem_buf_start != NULL );
    assert( mem_buf_size != 0 );
    universal_memory_output_stream_init( &((*this_).mem_output),
                                         mem_buf_start,
                                         mem_buf_size,
                                         UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8
                                       );
    utf8stream_writer_init( &((*this_).writer),
                            universal_memory_output_stream_get_output_stream( &((*this_).mem_output) )
                          );
    (*this_).lazy_err = U8_ERROR_NONE;
}

static inline u8_error_t utf8stream_writemem_destroy( utf8stream_writemem_t *this_ )
{
    u8_error_t err = (*this_).lazy_err;
    err |= utf8stream_writer_destroy( &((*this_).writer) );
    /* avoid duplicate flush; therefore do not call */
    /* err |= universal_memory_output_stream_destroy( &((*this_).mem_output) ); */
    return err;
}

static inline u8_error_t utf8stream_writemem_reset ( utf8stream_writemem_t *this_ )
{
    u8_error_t err = (*this_).lazy_err;
    (*this_).lazy_err = U8_ERROR_NONE;
    err |= utf8stream_writer_flush( &((*this_).writer) );
    err |= universal_memory_output_stream_reset( &((*this_).mem_output) );
    return err;
}

static inline utf8stream_writer_t * utf8stream_writemem_get_writer( utf8stream_writemem_t *this_ )
{
    return &((*this_).writer);
}

static inline utf8stringview_t utf8stream_writemem_get_view( utf8stream_writemem_t *this_ )
{
    (*this_).lazy_err |= utf8stream_writer_flush( &((*this_).writer) );

    const void* start = universal_memory_output_stream_get_start( &((*this_).mem_output) );
    const size_t length = universal_memory_output_stream_get_fill( &((*this_).mem_output) );

    return UTF8STRINGVIEW( start, length );
}

static inline utf8string_t * utf8stream_writemem_get_string( utf8stream_writemem_t *this_ )
{
    (*this_).lazy_err |= utf8stream_writer_flush( &((*this_).writer) );

    const void* start = universal_memory_output_stream_get_start( &((*this_).mem_output) );

    return start;
}


/*
Copyright 2024-2026 Andreas Warnke

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
