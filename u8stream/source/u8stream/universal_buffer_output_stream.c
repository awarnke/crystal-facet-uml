/* File: universal_buffer_output_stream.c; Copyright and License: see below */

#include "u8stream/universal_buffer_output_stream.h"
#include "u8stream/universal_output_stream_if.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <string.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_buffer_output_stream_private_if
    = {
        .write = (u8_error_t (*)(universal_output_stream_impl_t*, const void*, size_t)) &universal_buffer_output_stream_write,
        .flush = (u8_error_t (*)(universal_output_stream_impl_t*)) &universal_buffer_output_stream_flush
    };

void universal_buffer_output_stream_init( universal_buffer_output_stream_t *this_,
                                          void* mem_buf_start,
                                          size_t mem_buf_size,
                                          universal_output_stream_t *sink )
{
    TRACE_BEGIN();
    assert( mem_buf_start != NULL );
    assert( sink != NULL );

    (*this_).sink = sink;

    (*this_).mem_buf_start = mem_buf_start;
    (*this_).mem_buf_size = mem_buf_size;
    (*this_).mem_buf_filled = 0;
    universal_output_stream_private_init( &((*this_).output_stream), &universal_buffer_output_stream_private_if, this_ );

    TRACE_END();
}

u8_error_t universal_buffer_output_stream_destroy( universal_buffer_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).sink != NULL );
    u8_error_t err = U8_ERROR_NONE;

    err = universal_buffer_output_stream_flush( this_ );

    (*this_).mem_buf_start = NULL;
    (*this_).mem_buf_size = 0;
    (*this_).mem_buf_filled = 0;
    universal_output_stream_private_destroy( &((*this_).output_stream) );

    (*this_).sink = NULL;

    TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_buffer_output_stream_write ( universal_buffer_output_stream_t *this_, const void *start, size_t length )
{
    /*TRACE_BEGIN();*/
    assert( start != NULL );
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).sink != NULL );
    u8_error_t err = U8_ERROR_NONE;

    const size_t space_left = (*this_).mem_buf_size - (*this_).mem_buf_filled;
    char *const buf_first_free = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_filled]  );
    if ( length <= space_left )
    {
        /* append all to buffer */
        memcpy( buf_first_free, start, length );
        (*this_).mem_buf_filled += length;
    }
    else
    {
        /* append to buffer till buffer is full */
        memcpy( buf_first_free, start, space_left );
        (*this_).mem_buf_filled += space_left;
        const size_t remaining_len = length - space_left;
        const void *remaining_start = &(  (*(  (char(*)[])start  ))[space_left]  );

        err |= universal_buffer_output_stream_flush( this_ );

        if ( remaining_len <= (*this_).mem_buf_size )
        {
            memcpy( (*this_).mem_buf_start , remaining_start, remaining_len );
            (*this_).mem_buf_filled = remaining_len;
        }
        else
        {
            err |= universal_output_stream_write( (*this_).sink, remaining_start, remaining_len );
        }
    }

    /*TRACE_END_ERR(err);*/
    return err;
}

u8_error_t universal_buffer_output_stream_flush( universal_buffer_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).sink != NULL );
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).mem_buf_filled > 0 )
    {
        err |= universal_output_stream_write( (*this_).sink, (*this_).mem_buf_start, (*this_).mem_buf_filled );
    }
    (*this_).mem_buf_filled = 0;

    TRACE_END_ERR(err);
    return err;
}

universal_output_stream_t* universal_buffer_output_stream_get_output_stream( universal_buffer_output_stream_t *this_ )
{
    TRACE_BEGIN();

    universal_output_stream_t* result = &((*this_).output_stream);

    TRACE_END();
    return result;
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
