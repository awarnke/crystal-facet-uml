/* File: universal_file_output_stream.c; Copyright and License: see below */

#include "stream/universal_file_output_stream.h"
#include "stream/universal_output_stream_if.h"
#include "trace.h"
#include "tslog.h"
#include <stdbool.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_file_output_stream_private_if
    = {
        /* open:   */ (int (*)(void*, const char*)) &universal_file_output_stream_open,
        /* write:  */ (int (*)(void*, const void*, size_t)) &universal_file_output_stream_write,
        /* flush:  */ (int (*)(void*)) &universal_file_output_stream_flush,
        /* close:  */ (int (*)(void*)) &universal_file_output_stream_close
    };

void universal_file_output_stream_init ( universal_file_output_stream_t *this_ )
{
    TRACE_BEGIN();

    (*this_).output = NULL;
    universal_output_stream_init( &((*this_).output_stream), &universal_file_output_stream_private_if, this_ );

    TRACE_END();
}

int universal_file_output_stream_destroy( universal_file_output_stream_t *this_ )
{
    TRACE_BEGIN();
    int err = 0;

    if ( (*this_).output != NULL )
    {
        err = universal_file_output_stream_close( this_ );
    }
    (*this_).output = NULL;
    universal_output_stream_destroy( &((*this_).output_stream) );

    TRACE_END_ERR(err);
    return err;
}

int universal_file_output_stream_open ( universal_file_output_stream_t *this_, const char *path )
{
    TRACE_BEGIN();
    assert( (*this_).output == NULL );
    assert( path != NULL );
    int err = 0;

    (*this_).output = fopen( path, "w" );
    if ( NULL == (*this_).output )
    {
        TSLOG_ERROR("error at opening file.");
        err = -1;
    }

    TRACE_END_ERR(err);
    return err;
}

int universal_file_output_stream_write ( universal_file_output_stream_t *this_, const void *start, size_t length )
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

int universal_file_output_stream_flush( universal_file_output_stream_t *this_ )
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

int universal_file_output_stream_close( universal_file_output_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).output != NULL );
    int err = 0;

    int close_err;
    close_err = fclose( (*this_).output );
    if ( 0 != close_err )
    {
        TSLOG_ERROR_INT("error at closing file:",close_err);
        err = -1;
    }
    (*this_).output = NULL;

    TRACE_END_ERR(err);
    return err;
}

universal_output_stream_t* universal_file_output_stream_get_output_stream( universal_file_output_stream_t *this_ )
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
