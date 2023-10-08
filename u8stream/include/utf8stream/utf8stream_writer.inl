/* File: utf8stream_writer.c; Copyright and License: see below */

#include "u8/u8_trace.h"
#include <string.h>
#include <inttypes.h>
#include <assert.h>

static inline void utf8stream_writer_init ( utf8stream_writer_t *this_, universal_output_stream_t* out_stream )
{
    assert( out_stream != NULL );
    (*this_).output_stream = out_stream;
}

static inline void utf8stream_writer_destroy ( utf8stream_writer_t *this_ )
{
    (*this_).output_stream = NULL;
}

static inline u8_error_t utf8stream_writer_write_str ( utf8stream_writer_t *this_, const void *utf8_string )
{
    assert( (*this_).output_stream != NULL );
    assert( utf8_string != NULL );

    const size_t length = strlen(utf8_string);
    const u8_error_t err = universal_output_stream_write( (*this_).output_stream, utf8_string, length );

    return err;
}

static inline u8_error_t utf8stream_writer_write_int ( utf8stream_writer_t *this_, const int64_t number ) {
    char number_str[21]; /* this is sufficient for signed 64 bit integers: -9223372036854775806 */
    /* Note: snprintf is not available on every OS */
    sprintf( number_str, "%" PRIi64, number );
    const size_t length = strlen(number_str);
    const u8_error_t err = universal_output_stream_write( (*this_).output_stream, number_str, length );

    return err;
}

static inline u8_error_t utf8stream_writer_write_hex ( utf8stream_writer_t *this_, const int64_t number ) {
    char number_str[17]; /* this is sufficient for 64 bit integers */
    /* Note: snprintf is not available on every OS */
    sprintf( number_str, "%" PRIx64, number );
    const size_t length = strlen(number_str);
    const u8_error_t err = universal_output_stream_write( (*this_).output_stream, number_str, length );

    return err;
}

static inline int utf8stream_writer_flush ( utf8stream_writer_t *this_ )
{
    assert( (*this_).output_stream != NULL );

    const u8_error_t err = universal_output_stream_flush( (*this_).output_stream );

    return err;
}


/*
Copyright 2021-2023 Andreas Warnke

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
