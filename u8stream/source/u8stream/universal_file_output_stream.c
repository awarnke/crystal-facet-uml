/* File: universal_file_output_stream.c; Copyright and License: see below */

#include "u8stream/universal_file_output_stream.h"
#include "u8stream/universal_output_stream_if.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <stdbool.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_file_output_stream_private_if
    = {
        .write = (u8_error_t (*)(universal_output_stream_impl_t*, const void*, size_t)) &universal_file_output_stream_write,
        .flush = (u8_error_t (*)(universal_output_stream_impl_t*)) &universal_file_output_stream_flush
    };

void universal_file_output_stream_init ( universal_file_output_stream_t *this_ )
{
    TRACE_BEGIN();

    (*this_).output = NULL;
    universal_output_stream_private_init( &((*this_).output_stream), &universal_file_output_stream_private_if, this_ );

    TRACE_END();
}

u8_error_t universal_file_output_stream_destroy( universal_file_output_stream_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        err = universal_file_output_stream_close( this_ );
    }
    (*this_).output = NULL;
    universal_output_stream_private_destroy( &((*this_).output_stream) );

    TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_output_stream_open ( universal_file_output_stream_t *this_, const char *path )
{
    TRACE_BEGIN();
    assert( path != NULL );
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        TSLOG_ERROR("cannot open a file that is already open.");
        err = U8_ERROR_LOGIC_STATE;
        err |= universal_file_output_stream_close( this_ );
    }
    (*this_).output = fopen( path, "w" );
    if ( NULL == (*this_).output )
    {
        TSLOG_ERROR("error at opening file for writing.");
        err |= U8_ERROR_AT_FILE_WRITE;
    }

    TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_output_stream_write ( universal_file_output_stream_t *this_, const void *start, size_t length )
{
    /*TRACE_BEGIN();*/
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        size_t written = 0;
        while (( written < length )&&( err == U8_ERROR_NONE ))
        {
            ssize_t out_count;
            out_count = fwrite( ((const char*)start)+written, 1, length-written, (*this_).output );
            if ( out_count < 0 )
            {
                TSLOG_ERROR_INT( "not all bytes could be written. missing:", length-written );
                err = U8_ERROR_AT_FILE_WRITE;
            }
            else
            {
                written += out_count;
            }
        }
    }
    else
    {
        TSLOG_ERROR("cannot write to a file that is not open.");
        err = U8_ERROR_LOGIC_STATE;
    }

    /*TRACE_END_ERR(err);*/
    return err;
}

u8_error_t universal_file_output_stream_flush( universal_file_output_stream_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        int flush_err;
        flush_err = fflush( (*this_).output );
        if ( 0 != flush_err )
        {
            TSLOG_ERROR_INT("error at flushing file:",flush_err);
            err = U8_ERROR_AT_FILE_WRITE;
        }
    }
    else
    {
        TSLOG_ERROR("cannot flush a file that is not open.");
        err = U8_ERROR_LOGIC_STATE;
    }

    TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_output_stream_close( universal_file_output_stream_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        const int close_err = fclose( (*this_).output );
        if ( 0 != close_err )
        {
            TSLOG_ERROR_INT("error at closing file:",close_err);
            err = U8_ERROR_AT_FILE_WRITE;
        }
        (*this_).output = NULL;
    }
    else
    {
        TSLOG_ERROR("cannot close a file that is not open.");
        err = U8_ERROR_LOGIC_STATE;
    }

    TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_output_stream_remove( const universal_file_output_stream_t *this_, const char *path )
{
    TRACE_BEGIN();
    assert( path != NULL );
    u8_error_t err = U8_ERROR_NONE;

    const int remove_err = remove( path );
    if ( 0 != remove_err )
    {
        /* This error may have happened on purpose or by an unexpected condition */
        TRACE_INFO_STR( "error at removing file:", path );
        err |= U8_ERROR_AT_FILE_WRITE;
    }

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
