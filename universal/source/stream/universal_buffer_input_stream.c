/* File: universal_buffer_input_stream.c; Copyright and License: see below */

#include "stream/universal_buffer_input_stream.h"
#include "stream/universal_input_stream_if.h"
#include "trace.h"
#include "tslog.h"
#include <string.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_input_stream_if_t universal_buffer_input_stream_private_if
    = {
        .read = (int (*)(universal_input_stream_impl_t*, void*, size_t, size_t*)) &universal_buffer_input_stream_read
    };

void universal_buffer_input_stream_init( universal_buffer_input_stream_t *this_,
                                         void* mem_buf_start,
                                         size_t mem_buf_size,
                                         universal_input_stream_t *source )
{
    TRACE_BEGIN();
    assert( mem_buf_start != NULL );
    assert( source != NULL );

    (*this_).source = source;

    (*this_).mem_buf_start = mem_buf_start;
    (*this_).mem_buf_size = mem_buf_size;
    (*this_).mem_buf_fill = 0;
    (*this_).mem_buf_pos = 0;
    universal_input_stream_private_init( &((*this_).input_stream), &universal_buffer_input_stream_private_if, this_ );

    TRACE_END();
}

void universal_buffer_input_stream_destroy( universal_buffer_input_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).source != NULL );

    (*this_).mem_buf_start = NULL;
    (*this_).mem_buf_size = 0;
    (*this_).mem_buf_fill = 0;
    (*this_).mem_buf_pos = 0;
    universal_input_stream_private_destroy( &((*this_).input_stream) );

    (*this_).source = NULL;

    TRACE_END();
}

void universal_buffer_input_stream_reset ( universal_buffer_input_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );

    (*this_).mem_buf_pos = 0;
    (*this_).mem_buf_fill = 0;

    TRACE_END();
}

int universal_buffer_input_stream_read ( universal_buffer_input_stream_t *this_, void *out_buffer, size_t max_size, size_t *out_length )
{
    /*TRACE_BEGIN();*/
    assert( out_buffer != NULL );
    assert( max_size != 0 );
    assert( out_length != NULL );
    assert( (*this_).mem_buf_start != NULL );
    int err = 0;

    const bool buffer_empty = ( (*this_).mem_buf_pos >= (*this_).mem_buf_fill );
    if ( buffer_empty )
    {
        (*this_).mem_buf_pos = 0;
        (*this_).mem_buf_fill = 0;
        err = universal_input_stream_read( (*this_).source, (*this_).mem_buf_start, (*this_).mem_buf_size, &((*this_).mem_buf_fill) );
    }

    const size_t buffer_left = ( (*this_).mem_buf_fill - (*this_).mem_buf_pos );
    if ( buffer_left != 0 )
    {
        const size_t bytes_to_copy = (max_size <= buffer_left) ? max_size : buffer_left;
        char *const buf_first_read = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_pos]  );

        memcpy( out_buffer, buf_first_read, bytes_to_copy );
        (*this_).mem_buf_pos += bytes_to_copy;
        *out_length = bytes_to_copy;
    }
    else
    {
        err = -1;  /* finished, no more bytes to read */
        *out_length = 0;
    }

    /*TRACE_END_ERR(err);*/
    return err;
}

universal_input_stream_t* universal_buffer_input_stream_get_input_stream( universal_buffer_input_stream_t *this_ )
{
    TRACE_BEGIN();

    universal_input_stream_t* result = &((*this_).input_stream);

    TRACE_END();
    return result;
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
