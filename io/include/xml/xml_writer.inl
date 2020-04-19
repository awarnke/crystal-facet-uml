/* File: xml_writer.inl; Copyright and License: see below */

#include <assert.h>
#include "util/string/utf8error.h"

static inline int xml_writer_write_plain ( xml_writer_t *this_, const char *text )
{
    assert ( NULL != text );
    assert ( NULL != (*this_).output );
    size_t text_len = strlen(text);
    size_t out_count;
    out_count = fwrite( text, 1 /* size of char */, text_len, (*this_).output );
    return ( (out_count != text_len) ? -1 : 0 );
}

static inline int xml_writer_write_xml_enc ( xml_writer_t *this_, const char *text )
{
    assert ( NULL != text );
    assert ( NULL != (*this_).output );
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).temp_output, text );
    strerr |= utf8stringbuf_replace_all( (*this_).temp_output, (*this_).xml_encode_table );
    size_t text_len = utf8stringbuf_get_length( (*this_).temp_output );
    size_t out_count;
    out_count = fwrite( utf8stringbuf_get_string( (*this_).temp_output ), 1 /* size of char */, text_len, (*this_).output );
    return ( ((out_count != text_len)||( UTF8ERROR_SUCCESS != strerr)) ? -1 : 0 );
}

static inline int xml_writer_write_plain_buf ( xml_writer_t *this_, const char *start, size_t length )
{
    assert ( NULL != start );
    assert ( NULL != (*this_).output );
    size_t out_count;
    out_count = fwrite( start, 1 /* size of char */, length, (*this_).output );
    return ( (out_count != length) ? -1 : 0 );
}

static inline int xml_writer_write_xml_enc_buf ( xml_writer_t *this_, const char *start, size_t length )
{
    assert ( NULL != start );
    assert ( NULL != (*this_).output );
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_region_from_str( (*this_).temp_output, start, 0, length );
    strerr |= utf8stringbuf_replace_all( (*this_).temp_output, (*this_).xml_encode_table );
    size_t text_len = utf8stringbuf_get_length( (*this_).temp_output );
    size_t out_count;
    out_count = fwrite( utf8stringbuf_get_string( (*this_).temp_output ), 1 /* size of char */, text_len, (*this_).output );
    return ( ((out_count != text_len)||( UTF8ERROR_SUCCESS != strerr)) ? -1 : 0 );
}


/*
Copyright 2019-2020 Andreas Warnke

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
