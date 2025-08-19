/* File: io_txt_writer.inl; Copyright and License: see below */

#include <assert.h>
#include "utf8stringbuf/utf8error.h"

static inline u8_error_t io_txt_writer_write_plain ( io_txt_writer_t *this_, const char *text )
{
    assert ( NULL != text );
    assert ( NULL != (*this_).output );
    u8_error_t write_err;

    const size_t text_len = strlen(text);
    write_err = universal_output_stream_write ( (*this_).output, text, text_len );

    return ( write_err );
}

static inline u8_error_t io_txt_writer_fill ( io_txt_writer_t *this_, char pattern, unsigned int count )
{
    assert ( NULL != (*this_).output );
    u8_error_t write_err = U8_ERROR_NONE;

    /* prepare a buffer for optimized output */
    char buf[16];
    for ( unsigned int index = 0; index < 16; index++ )
    {
        buf[index] = pattern;
    }

    /* do write the buffer  */
    unsigned int buf_count = count / 16;
    for ( unsigned int loop_count = 0; loop_count < buf_count; loop_count++ )
    {
        write_err |= universal_output_stream_write ( (*this_).output, &buf, sizeof(buf) );
    }
    unsigned int buf_remain = count % 16;
    write_err |= universal_output_stream_write ( (*this_).output, &buf, buf_remain );

    return ( write_err );
}


/*
Copyright 2019-2025 Andreas Warnke

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
