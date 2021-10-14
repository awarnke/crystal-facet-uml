/* File: universal_memory_output_stream.c; Copyright and License: see below */

#include "stream/universal_memory_output_stream.h"
#include "stream/universal_output_stream_if.h"
#include "trace.h"
#include "tslog.h"
#include <string.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_memory_output_stream_private_if
    = {
        .open =  (int (*)(universal_output_stream_impl_t*, const char*)) &universal_memory_output_stream_open,
        .write = (int (*)(universal_output_stream_impl_t*, const void*, size_t)) &universal_memory_output_stream_write,
        .flush = (int (*)(universal_output_stream_impl_t*)) &universal_memory_output_stream_flush,
        .close = (int (*)(universal_output_stream_impl_t*)) &universal_memory_output_stream_close
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

int universal_memory_output_stream_destroy( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();
    int err = 0;

    (*this_).mem_buf_start = NULL;
    (*this_).mem_buf_size = 0;
    (*this_).mem_buf_filled = 0;
    universal_output_stream_private_destroy( &((*this_).output_stream) );

    TRACE_END_ERR(err);
    return err;
}

int universal_memory_output_stream_open ( universal_memory_output_stream_t *this_, const char *path )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    const int err = 0;

    (*this_).mem_buf_filled = 0;

    TRACE_END_ERR(err);
    return err;
}

int universal_memory_output_stream_write ( universal_memory_output_stream_t *this_, const void *start, size_t length )
{
    /*TRACE_BEGIN();*/
    assert( start != NULL );
    assert( (*this_).mem_buf_start != NULL );
    int err = 0;

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
        err = -1;
    }

    /*TRACE_END_ERR(err);*/
    return err;
}

int universal_memory_output_stream_flush( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    const int err = 0;

    TRACE_END_ERR(err);
    return err;
}

int universal_memory_output_stream_close( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    const int err = 0;

    TRACE_END_ERR(err);
    return err;
}

int universal_memory_output_stream_write_0term ( universal_memory_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).mem_buf_start != NULL );
    int err = 0;

    if ( (*this_).mem_buf_size == 0 )
    {
        TSLOG_ERROR( "buffer size is 0; buffer is not terminated by zero." );
        err = -1;
    }

    else if ( (*this_).mem_buf_size == (*this_).mem_buf_filled )
    {
        char *const last_char = &(  (*(  (char(*)[])(*this_).mem_buf_start  ))[(*this_).mem_buf_size - 1]  );
        *last_char = '\0';
        TSLOG_WARNING( "last byte overwritten by terminating zero" );
        err = -1;
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
Copyright 2020-2021 Andreas Warnke

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
