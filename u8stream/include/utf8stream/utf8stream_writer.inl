/* File: utf8stream_writer.c; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <string.h>
#include <inttypes.h>
#include <assert.h>

static inline void utf8stream_writer_init ( utf8stream_writer_t *this_, universal_output_stream_t* out_stream )
{
    assert( out_stream != NULL );
    (*this_).output_stream = out_stream;
    (*this_).buf_fill = 0;
}

static inline u8_error_t utf8stream_writer_destroy ( utf8stream_writer_t *this_ )
{
    assert( (*this_).output_stream != NULL );

    const u8_error_t err = utf8stream_writer_flush( this_ );
    (*this_).output_stream = NULL;

    return err;
}

static inline u8_error_t utf8stream_writer_write_str ( utf8stream_writer_t *this_, const void *utf8_string )
{
    assert( (*this_).output_stream != NULL );
    assert( utf8_string != NULL );

    utf8stringview_t view = UTF8STRINGVIEW_STR( utf8_string );
    const u8_error_t err = utf8stream_writer_write_view( this_, &view );

    return err;
}

static inline u8_error_t utf8stream_writer_write_int ( utf8stream_writer_t *this_, const int64_t number )
{
    assert( (*this_).output_stream != NULL );

    char number_str[21]; /* this is sufficient for signed 64 bit integers: -9223372036854775806 */
    /* Note: snprintf is not available on every OS */
    sprintf( number_str, "%" PRIi64, number );

    utf8stringview_t view = UTF8STRINGVIEW_STR( number_str );
    const u8_error_t err = utf8stream_writer_write_view( this_, &view );

    return err;
}

static inline u8_error_t utf8stream_writer_write_hex ( utf8stream_writer_t *this_, const int64_t number )
{
    assert( (*this_).output_stream != NULL );

    char number_str[17]; /* this is sufficient for 64 bit integers */
    /* Note: snprintf is not available on every OS */
    sprintf( number_str, "%" PRIx64, number );

    utf8stringview_t view = UTF8STRINGVIEW_STR( number_str );
    const u8_error_t err = utf8stream_writer_write_view( this_, &view );

    return err;
}

static inline utf8error_t utf8stream_writer_write_char( utf8stream_writer_t *this_, const uint32_t codepoint )
{
    assert( (*this_).output_stream != NULL );

    const utf8codepoint_t cp = utf8codepoint( codepoint );
    const utf8codepointseq_t mem_buf = utf8codepoint_get_utf8( &cp );
    const unsigned int mem_len = utf8codepoint_get_length( &cp );
    assert( mem_len <= sizeof(utf8codepointseq_t) );

    utf8stringview_t view = UTF8STRINGVIEW( (const char*) &mem_buf, mem_len );
    const u8_error_t err = utf8stream_writer_write_view( this_, &view );

    return err;
}

static inline u8_error_t utf8stream_writer_write_view ( utf8stream_writer_t *this_, const utf8stringview_t *utf8_view )
{
    assert( (*this_).output_stream != NULL );
    assert( (*this_).buf_fill <= sizeof( (*this_).buffer ) );
    assert( UTF8STREAM_WRITER_MAX_BUF == sizeof( (*this_).buffer ) );
    assert( utf8_view != NULL );
    const char *start = utf8stringview_get_start( utf8_view );
    const size_t length = utf8stringview_get_length( utf8_view );
    u8_error_t err = U8_ERROR_NONE;

    /* is there buffer free? */
    if ( length <= ((size_t)UTF8STREAM_WRITER_MAX_BUF) - (*this_).buf_fill )  /* all possible constants are on the right side for compiler optimizations */
    {
        /* store to buffer */
#if __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
        memcpy( &((*this_).buffer[(*this_).buf_fill]), start, length );
#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif
        (*this_).buf_fill += length;
    }
    else
    {
        /* flush the buffer */
        if ( (*this_).buf_fill > 0 )
        {
            err |= universal_output_stream_write( (*this_).output_stream, &((*this_).buffer), (*this_).buf_fill );
            (*this_).buf_fill = 0;
        }

        /* is there enough buffer free now? */
        if ( length < UTF8STREAM_WRITER_MAX_BUF )
        {
            /* store to buffer */
            memcpy( &((*this_).buffer), start, length );
            (*this_).buf_fill = length;
        }
        else
        {
            /* write immediate */
            err |= universal_output_stream_write( (*this_).output_stream, start, length );
        }
    }

    return err;
}

static inline u8_error_t utf8stream_writer_flush ( utf8stream_writer_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( (*this_).output_stream != NULL );
    assert( (*this_).buf_fill <= sizeof( (*this_).buffer ) );
    u8_error_t err = U8_ERROR_NONE;

    if ( (*this_).buf_fill > 0 )
    {
        err |= universal_output_stream_write( (*this_).output_stream, &((*this_).buffer), (*this_).buf_fill );
        (*this_).buf_fill = 0;
    }

    err |= universal_output_stream_flush( (*this_).output_stream );

    U8_TRACE_END_ERR( err );
    return err;
}


/*
Copyright 2021-2025 Andreas Warnke

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
