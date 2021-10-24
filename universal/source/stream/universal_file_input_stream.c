/* File: universal_file_input_stream.c; Copyright and License: see below */

#include "stream/universal_file_input_stream.h"
#include "stream/universal_input_stream_if.h"
#include "trace.h"
#include "tslog.h"
#include <stdbool.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_input_stream_if_t universal_file_input_stream_private_if
    = {
        .read = (int (*)(universal_input_stream_impl_t*, void*, size_t, size_t*)) &universal_file_input_stream_read
    };

void universal_file_input_stream_init ( universal_file_input_stream_t *this_ )
{
    TRACE_BEGIN();

    (*this_).input = NULL;
    universal_input_stream_private_init( &((*this_).input_stream), &universal_file_input_stream_private_if, this_ );

    TRACE_END();
}

int universal_file_input_stream_destroy( universal_file_input_stream_t *this_ )
{
    TRACE_BEGIN();
    int err = 0;

    if ( (*this_).input != NULL )
    {
        err = universal_file_input_stream_close( this_ );
    }
    (*this_).input = NULL;
    universal_input_stream_private_destroy( &((*this_).input_stream) );

    TRACE_END_ERR(err);
    return err;
}

int universal_file_input_stream_open ( universal_file_input_stream_t *this_, const char *path )
{
    TRACE_BEGIN();
    assert( (*this_).input == NULL );
    assert( path != NULL );
    int err = 0;

    (*this_).input = fopen( path, "r" );
    if ( NULL == (*this_).input )
    {
        TSLOG_ERROR("error at opening file for reading.");
        err = -1;
    }

    TRACE_END_ERR(err);
    return err;
}

int universal_file_input_stream_read ( universal_file_input_stream_t *this_, void *out_buffer, size_t max_size, size_t *out_length )
{
    /*TRACE_BEGIN();*/
    assert( (*this_).input != NULL );
    int err = 0;


    /*TRACE_END_ERR(err);*/
    return err;
}

int universal_file_input_stream_close( universal_file_input_stream_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).input != NULL );
    int err = 0;

    int close_err;
    close_err = fclose( (*this_).input );
    if ( 0 != close_err )
    {
        TSLOG_ERROR_INT("error at closing file:",close_err);
        err = -1;
    }
    (*this_).input = NULL;

    TRACE_END_ERR(err);
    return err;
}

universal_input_stream_t* universal_file_input_stream_get_input_stream( universal_file_input_stream_t *this_ )
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
