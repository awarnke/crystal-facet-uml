/* File: universal_file_output_stream.c; Copyright and License: see below */

#include "u8stream/universal_file_output_stream.h"
#include "u8stream/universal_output_stream_if.h"
#include "u8_test_cond.h"
#include "u8/u8_fault_inject.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <errno.h>
#include <string.h>
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
    U8_TRACE_BEGIN();

    (*this_).output = NULL;
    universal_output_stream_private_init( &((*this_).output_stream), &universal_file_output_stream_private_if, this_ );

    U8_TRACE_END();
}

u8_error_t universal_file_output_stream_destroy( universal_file_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        err = universal_file_output_stream_close( this_ );
    }
    (*this_).output = NULL;
    universal_output_stream_private_destroy( &((*this_).output_stream) );

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_output_stream_open ( universal_file_output_stream_t *this_, const char *path )
{
    U8_TRACE_BEGIN();
    assert( path != NULL );
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        U8_LOG_ERROR("cannot open a file that is already open.");
        err = U8_ERROR_WRONG_STATE;
        err |= universal_file_output_stream_close( this_ );
    }
    (*this_).output = fopen( path, "w" );
    if ( NULL == (*this_).output )
    {
        /* Note: This need not be an error, could be intentionally to avoid TOCTOU issues. */
        U8_LOG_EVENT_STR( "could not open file for writing:", strerror(errno) );
        err |= U8_ERROR_AT_FILE_WRITE;
    }

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_output_stream_write ( universal_file_output_stream_t *this_, const void *start, size_t length )
{
    /*U8_TRACE_BEGIN();*/
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        size_t written = 0;
        while (( written < length )&&( err == U8_ERROR_NONE ))
        {
            const size_t remaining = length - written;
            size_t out_count;
            out_count = fwrite( ((const char*)start)+written, 1, remaining, (*this_).output );
            assert( out_count != 0 );  /* this should not happen, but do not take this for granted */
            U8_FAULT_INJECT_COND_SET( U8_TEST_COND_FWRITE, out_count, 0 );
            if ( out_count == 0 )
            {
                U8_LOG_ERROR_INT( "not all bytes could be written. missing:", remaining );
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
        U8_LOG_ERROR("cannot write to a file that is not open.");
        err = U8_ERROR_WRONG_STATE;
    }

    /*U8_TRACE_END_ERR(err);*/
    return err;
}

u8_error_t universal_file_output_stream_flush( universal_file_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        int flush_err = fflush( (*this_).output );
        assert( flush_err == 0 );  /* this should not happen, but do not take this for granted */
        U8_FAULT_INJECT_COND_SET( U8_TEST_COND_FFLUSH, flush_err, EOF );
        if ( 0 != flush_err )
        {
            U8_LOG_ERROR_INT("error at flushing file:",flush_err);
            err = U8_ERROR_AT_FILE_WRITE;
        }
    }
    else
    {
        U8_LOG_ERROR("cannot flush a file that is not open.");
        err = U8_ERROR_WRONG_STATE;
    }

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_output_stream_close( universal_file_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).output != NULL )
    {
        int close_err = fclose( (*this_).output );
        assert( close_err == 0 );  /* this should not happen, but do not take this for granted */
        U8_FAULT_INJECT_COND_SET( U8_TEST_COND_FCLOSE, close_err, EOF );
        if ( 0 != close_err )
        {
            U8_LOG_ERROR_INT("error at closing file:",close_err);
            err = U8_ERROR_AT_FILE_WRITE;
        }
        (*this_).output = NULL;
    }
    else
    {
        U8_LOG_ERROR("cannot close a file that is not open.");
        err = U8_ERROR_WRONG_STATE;
    }

    U8_TRACE_END_ERR(err);
    return err;
}

universal_output_stream_t* universal_file_output_stream_get_output_stream( universal_file_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();

    universal_output_stream_t* result = &((*this_).output_stream);

    U8_TRACE_END();
    return result;
}


/*
Copyright 2020-2025 Andreas Warnke

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
