/* File: universal_file_input_stream.c; Copyright and License: see below */

#include "u8stream/universal_file_input_stream.h"
#include "u8stream/universal_input_stream_if.h"
#include "u8_test_cond.h"
#include "u8/u8_fault_inject.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_input_stream_if_t universal_file_input_stream_private_if
    = {
        .read = (u8_error_t (*)(universal_input_stream_impl_t*, void*, size_t, size_t*)) &universal_file_input_stream_read,
        .reset = (u8_error_t (*)(universal_input_stream_impl_t*)) &universal_file_input_stream_reset
    };

void universal_file_input_stream_init ( universal_file_input_stream_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).input = NULL;
    universal_input_stream_private_init( &((*this_).input_stream), &universal_file_input_stream_private_if, this_ );

    U8_TRACE_END();
}

u8_error_t universal_file_input_stream_destroy( universal_file_input_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).input != NULL )
    {
        err = universal_file_input_stream_close( this_ );
    }
    (*this_).input = NULL;
    universal_input_stream_private_destroy( &((*this_).input_stream) );

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_input_stream_open ( universal_file_input_stream_t *this_, const char *path )
{
    U8_TRACE_BEGIN();
    assert( path != NULL );
    u8_error_t err = 0;

    if ( (*this_).input != NULL )
    {
        U8_LOG_ERROR("cannot open a file that is already open.");
        err = U8_ERROR_WRONG_STATE;
        err |= universal_file_input_stream_close( this_ );
    }
    (*this_).input = fopen( path, "r" );
    if ( NULL == (*this_).input )
    {
        /* Note: This need not be an error, could be intentionally to avoid TOCTOU issues. */
        U8_LOG_EVENT_STR("could not open file for reading:", strerror(errno) );
        err |= U8_ERROR_AT_FILE_READ;
    }

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_input_stream_read ( universal_file_input_stream_t *this_,
                                              void *out_buffer,
                                              size_t max_size,
                                              size_t *out_length )
{
    /*U8_TRACE_BEGIN();*/
    assert( out_buffer != NULL );
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).input != NULL )
    {
        size_t read_bytes = fread( out_buffer, 1, max_size, (*this_).input );
        if ( read_bytes != 0 )
        {
            *out_length = read_bytes;
        }
        else
        {
            err = U8_ERROR_END_OF_STREAM;  /* finished, no more bytes to read or other error */
            *out_length = 0;
        }
    }
    else
    {
        U8_LOG_ERROR("cannot read from a file that is not open.");
        err = U8_ERROR_WRONG_STATE;
    }

    /*U8_TRACE_END_ERR(err);*/
    return err;
}

u8_error_t universal_file_input_stream_reset ( universal_file_input_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).input != NULL )
    {
        int seek_err = fseek( (*this_).input, 0, SEEK_SET );
        assert( seek_err == 0 );  /* this should not happen, but do not take this for granted */
        U8_FAULT_INJECT_COND_SET( U8_TEST_COND_FSEEK, seek_err, -1 );
        if ( seek_err != 0 )
        {
            U8_LOG_ERROR("error at resetting the read-cursor in a file.");
            err = U8_ERROR_AT_FILE_READ;
        }
    }
    else
    {
        U8_LOG_ERROR("cannot reset/seek a file that is not open.");
        err = U8_ERROR_WRONG_STATE;
    }

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_file_input_stream_close( universal_file_input_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).input != NULL )
    {
        int close_err = fclose( (*this_).input );
        assert( close_err == 0 );  /* this should not happen, but do not take this for granted */
        U8_FAULT_INJECT_COND_SET( U8_TEST_COND_FCLOSE, close_err, EOF );
        if ( 0 != close_err )
        {
            U8_LOG_ERROR_INT( "error at closing file:", close_err );
            err = U8_ERROR_AT_FILE_READ;
        }
        (*this_).input = NULL;
    }
    else
    {
        U8_LOG_ERROR("cannot close a file that is not open.");
        err = U8_ERROR_WRONG_STATE;
    }

    U8_TRACE_END_ERR(err);
    return err;
}

universal_input_stream_t* universal_file_input_stream_get_input_stream( universal_file_input_stream_t *this_ )
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
