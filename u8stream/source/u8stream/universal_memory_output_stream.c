/* File: universal_memory_output_stream.c; Copyright and License: see below */

#include "u8stream/universal_memory_output_stream.h"
#include "u8stream/universal_output_stream_if.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <string.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_memory_output_stream_private_if
    = {
        .write = (u8_error_t (*)(universal_output_stream_impl_t*, const void*, size_t)) &universal_memory_output_stream_write,
        .flush = (u8_error_t (*)(universal_output_stream_impl_t*)) &universal_memory_output_stream_flush
    };

void universal_memory_output_stream_init ( universal_memory_output_stream_t *this_,
                                           void* mem_buf_start,
                                           size_t mem_buf_size )
{
    TRACE_BEGIN();
    assert( mem_buf_start != NULL );

    (*this_).mem_buf_start = mem_buf_start;
    (*this_).mem_buf_size = mem_buf_size;
    (*this_).mem_buf_filled = 0;
    universal_output_stream_private_init( &((*this_).output_stream), &universal_memory_output_stream_private_if, this_ );

    TRACE_END();
}

void universal_memory_output_stream_destroy( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();

    (*this_).mem_buf_start = NULL;
    (*this_).mem_buf_size = 0;
    (*this_).mem_buf_filled = 0;
    universal_output_stream_private_destroy( &((*this_).output_stream) );

    TRACE_END();
}

u8_error_t universal_memory_output_stream_reset ( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    const u8_error_t err = U8_ERROR_NONE;

    (*this_).mem_buf_filled = 0;

    TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_memory_output_stream_write ( universal_memory_output_stream_t *this_, const void *start, size_t length )
{
    /*TRACE_BEGIN();*/
    assert( start != NULL );
    assert( (*this_).mem_buf_start != NULL );
    u8_error_t err = U8_ERROR_NONE;

    const size_t space_left = ( (*this_).mem_buf_size - (*this_).mem_buf_filled );
    char *const buf_first_free = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_filled]  );
    if ( length <= space_left )
    {
        memcpy( buf_first_free, start, length );
        (*this_).mem_buf_filled += length;
    }
    else
    {
        memcpy( buf_first_free, start, space_left );
        (*this_).mem_buf_filled += space_left;
        TSLOG_WARNING_INT( "not all bytes could be written. missing:", length-space_left );
        err = U8_ERROR_AT_FILE_WRITE;
    }

    /*TRACE_END_ERR(err);*/
    return err;
}

u8_error_t universal_memory_output_stream_flush( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    const u8_error_t err = U8_ERROR_NONE;

    TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_memory_output_stream_write_0term ( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).mem_buf_size == 0 )
    {
        TSLOG_ERROR( "buffer size is 0; buffer is not terminated by zero." );
        err = U8_ERROR_LOGIC_STATE;
    }

    else if ( (*this_).mem_buf_size == (*this_).mem_buf_filled )
    {
        char *const last_char = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_size - 1]  );
        *last_char = '\0';
        TSLOG_WARNING( "last byte overwritten by terminating zero" );
        err = U8_ERROR_AT_FILE_WRITE;
    }
    else
    {
        char *const term_char = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_filled]  );
        *term_char = '\0';
        (*this_).mem_buf_filled += sizeof(char);
    }

    TRACE_END_ERR(err);
    return err;
}

universal_output_stream_t* universal_memory_output_stream_get_output_stream( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();

    universal_output_stream_t* result = &((*this_).output_stream);

    TRACE_END();
    return result;
}


/*
Copyright 2020-2022 Andreas Warnke

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