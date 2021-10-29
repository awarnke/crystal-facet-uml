/* File: universal_stream_output_stream.c; Copyright and License: see below */

#include "stream/universal_stream_output_stream.h"
#include "stream/universal_output_stream_if.h"
#include "trace.h"
#include "tslog.h"
#include <stdbool.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_stream_output_stream_private_if
    = {
        .write = (int (*)(universal_output_stream_impl_t*, const void*, size_t)) &universal_stream_output_stream_write,
        .flush = (int (*)(universal_output_stream_impl_t*)) &universal_stream_output_stream_flush
    };

void universal_stream_output_stream_init( universal_stream_output_stream_t *this_, FILE* out_stream )
{
    TRACE_BEGIN();
    assert( out_stream != NULL );

    (*this_).output = out_stream;
    universal_output_stream_private_init( &((*this_).output_stream), &universal_stream_output_stream_private_if, this_ );

    TRACE_END();
}

int universal_stream_output_stream_destroy( universal_stream_output_stream_t *this_ )
{
    TRACE_BEGIN();
    int err = 0;

    (*this_).output = NULL;
    universal_output_stream_private_destroy( &((*this_).output_stream) );

    TRACE_END_ERR(err);
    return err;
}

int universal_stream_output_stream_write ( universal_stream_output_stream_t *this_, const void *start, size_t length )
{
    /*TRACE_BEGIN();*/
    assert( (*this_).output != NULL );
    int err = 0;

    size_t written = 0;
    while (( written < length )&&( err == 0 ))
    {
        ssize_t out_count;
        out_count = fwrite( ((const char*)start)+written, 1, length-written, (*this_).output );
        if ( out_count < 0 )
        {
            TSLOG_ERROR_INT( "not all bytes could be written. missing:", length-written );
            err = -1;
        }
        else
        {
            written += out_count;
        }
    }

    /*TRACE_END_ERR(err);*/
    return err;
}

int universal_stream_output_stream_flush( universal_stream_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).output != NULL );
    int err = 0;

    int flush_err;
    flush_err = fflush( (*this_).output );
    if ( 0 != flush_err )
    {
        TSLOG_ERROR_INT("error at flushing file:",flush_err);
        err = -1;
    }

    TRACE_END_ERR(err);
    return err;
}

universal_output_stream_t* universal_stream_output_stream_get_output_stream( universal_stream_output_stream_t *this_ )
{
    TRACE_BEGIN();

    universal_output_stream_t* result = &((*this_).output_stream);

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
