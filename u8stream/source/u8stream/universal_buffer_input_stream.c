/* File: universal_buffer_input_stream.c; Copyright and License: see below */

#include "u8stream/universal_buffer_input_stream.h"
#include "u8stream/universal_input_stream_if.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <string.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_input_stream_if_t universal_buffer_input_stream_private_if
    = {
        .read = (u8_error_t (*)(universal_input_stream_impl_t*, void*, size_t, size_t*)) &universal_buffer_input_stream_read,
        .reset = (u8_error_t (*)(universal_input_stream_impl_t*)) &universal_buffer_input_stream_reset
    };

void universal_buffer_input_stream_init( universal_buffer_input_stream_t *this_,
                                         void* mem_buf_start,
                                         size_t mem_buf_size,
                                         universal_input_stream_t *source )
{
    U8_TRACE_BEGIN();
    assert( mem_buf_start != NULL );
    assert( source != NULL );

    (*this_).source = source;

    (*this_).mem_buf_start = mem_buf_start;
    (*this_).mem_buf_size = mem_buf_size;
    (*this_).mem_buf_fill = 0;
    (*this_).mem_buf_pos = 0;
    universal_input_stream_private_init( &((*this_).input_stream), &universal_buffer_input_stream_private_if, this_ );
    (*this_).stream_pos_of_buf = 0;

    U8_TRACE_END();
}

void universal_buffer_input_stream_destroy( universal_buffer_input_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).source != NULL );

    (*this_).mem_buf_start = NULL;
    (*this_).mem_buf_size = 0;
    (*this_).mem_buf_fill = 0;
    (*this_).mem_buf_pos = 0;
    universal_input_stream_private_destroy( &((*this_).input_stream) );
    (*this_).stream_pos_of_buf = 0;

    (*this_).source = NULL;

    U8_TRACE_END();
}

void universal_buffer_input_stream_reset ( universal_buffer_input_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).source != NULL );

    (*this_).mem_buf_pos = 0;
    (*this_).mem_buf_fill = 0;
    (*this_).stream_pos_of_buf = 0;

    U8_TRACE_END();
}

u8_error_t universal_buffer_input_stream_read ( universal_buffer_input_stream_t *this_, void *out_buffer, size_t max_size, size_t *out_length )
{
    /*U8_TRACE_BEGIN();*/
    assert( out_buffer != NULL );
    assert( max_size != 0 );
    assert( out_length != NULL );
    assert( (*this_).mem_buf_start != NULL );
    assert( (*this_).source != NULL );
    u8_error_t err = U8_ERROR_NONE;

    const size_t buf_available1 = (*this_).mem_buf_fill - (*this_).mem_buf_pos;
    char *const buf_first_read = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_pos]  );
    if ( max_size <= buf_available1 )
    {
        /* read all from buffer */
        memcpy( out_buffer, buf_first_read, max_size );
        (*this_).mem_buf_pos += max_size;
        *out_length = max_size;
    }
    else
    {
        /* read from buffer till buffer is empty */
        memcpy( out_buffer, buf_first_read, buf_available1 );
        (*this_).stream_pos_of_buf += (*this_).mem_buf_fill;
        (*this_).mem_buf_pos = 0;
        (*this_).mem_buf_fill = 0;
        const size_t remaining_len = max_size - buf_available1;
        void *const remaining_start = &(  (*(  (char(*)[])out_buffer  ))[buf_available1]  );

        if ( remaining_len < (*this_).mem_buf_size )
        {
            err |= universal_input_stream_read( (*this_).source, (*this_).mem_buf_start, (*this_).mem_buf_size, &((*this_).mem_buf_fill) );

            const size_t buf_available2 = ( (*this_).mem_buf_fill < remaining_len )?( (*this_).mem_buf_fill ):( remaining_len );

            memcpy( remaining_start, (*this_).mem_buf_start, buf_available2 );
            (*this_).mem_buf_pos = buf_available2;
            *out_length = buf_available1 + buf_available2;
        }
        else
        {
            size_t remaining_actual = 0;
            err |= universal_input_stream_read( (*this_).source, remaining_start, remaining_len, &remaining_actual );
            *out_length = buf_available1 + remaining_actual;
            (*this_).stream_pos_of_buf += remaining_actual;
        }
    }

    /*U8_TRACE_END_ERR(err);*/
    return err;
}

universal_input_stream_t* universal_buffer_input_stream_get_input_stream( universal_buffer_input_stream_t *this_ )
{
    U8_TRACE_BEGIN();

    universal_input_stream_t* result = &((*this_).input_stream);

    U8_TRACE_END();
    return result;
}


/*
Copyright 2021-2026 Andreas Warnke

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
