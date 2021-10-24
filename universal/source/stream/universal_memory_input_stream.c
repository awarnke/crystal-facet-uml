/* File: universal_memory_input_stream.c; Copyright and License: see below */

#include "stream/universal_memory_input_stream.h"
#include "stream/universal_input_stream_if.h"
#include "trace.h"
#include "tslog.h"
#include <string.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_input_stream_if_t universal_memory_input_stream_private_if
    = {
        .read = (int (*)(universal_input_stream_impl_t*, void*, size_t, size_t*)) &universal_memory_input_stream_read
    };

void universal_memory_input_stream_init ( universal_memory_input_stream_t *this_,
                                          const void* mem_buf_start,
                                          size_t mem_buf_size )
{
    TRACE_BEGIN();
    assert( mem_buf_start != NULL );

    (*this_).mem_buf_start = mem_buf_start;
    (*this_).mem_buf_size = mem_buf_size;
    (*this_).mem_buf_pos = 0;
    universal_input_stream_private_init( &((*this_).input_stream), &universal_memory_input_stream_private_if, this_ );

    TRACE_END();
}

int universal_memory_input_stream_destroy( universal_memory_input_stream_t *this_ )
{
    TRACE_BEGIN();
    int err = 0;

    (*this_).mem_buf_start = NULL;
    (*this_).mem_buf_size = 0;
    (*this_).mem_buf_pos = 0;
    universal_input_stream_private_destroy( &((*this_).input_stream) );

    TRACE_END_ERR(err);
    return err;
}

int universal_memory_input_stream_reset ( universal_memory_input_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    const int err = 0;

    (*this_).mem_buf_pos = 0;

    TRACE_END_ERR(err);
    return err;
}

int universal_memory_input_stream_read ( universal_memory_input_stream_t *this_, void *out_buffer, size_t max_size, size_t *out_length )
{
    /*TRACE_BEGIN();*/
    assert( out_buffer != NULL );
    assert( out_length != NULL );
    assert( (*this_).mem_buf_start != NULL );
    int err = 0;


    /*TRACE_END_ERR(err);*/
    return err;
}

universal_input_stream_t* universal_memory_input_stream_get_input_stream( universal_memory_input_stream_t *this_ )
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
