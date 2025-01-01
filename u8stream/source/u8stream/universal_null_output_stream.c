/* File: universal_null_output_stream.c; Copyright and License: see below */

#include "u8stream/universal_null_output_stream.h"
#include "u8stream/universal_output_stream_if.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <stdbool.h>
#include <assert.h>

/* the vmt implementing the interface */
static const universal_output_stream_if_t universal_null_output_stream_private_if
    = {
        .write = (u8_error_t (*)(universal_output_stream_impl_t*, const void*, size_t)) &universal_null_output_stream_write,
        .flush = (u8_error_t (*)(universal_output_stream_impl_t*)) &universal_null_output_stream_flush
    };

void universal_null_output_stream_init ( universal_null_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();

    universal_output_stream_private_init( &((*this_).output_stream), &universal_null_output_stream_private_if, this_ );

    U8_TRACE_END();
}

u8_error_t universal_null_output_stream_destroy( universal_null_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    universal_output_stream_private_destroy( &((*this_).output_stream) );

    U8_TRACE_END_ERR(err);
    return err;
}

u8_error_t universal_null_output_stream_write ( universal_null_output_stream_t *this_, const void *start, size_t length )
{
    /*U8_TRACE_BEGIN();*/
    u8_error_t err = U8_ERROR_NONE;

    /*U8_TRACE_END_ERR(err);*/
    return err;
}

u8_error_t universal_null_output_stream_flush( universal_null_output_stream_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t err = U8_ERROR_NONE;

    U8_TRACE_END_ERR(err);
    return err;
}

universal_output_stream_t* universal_null_output_stream_get_output_stream( universal_null_output_stream_t *this_ )
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
