/* File: utf8stringview.c; Copyright and License: see below */

#include "utf8stringbuf/utf8stringview.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"

#ifdef __cplusplus
extern "C" {
#endif

utf8error_t utf8stringview_parse_int( const utf8stringview_t *this_, int64_t *out_number, utf8stringview_t *out_remainder )
{
    assert( out_number != NULL );
    utf8error_t result = UTF8ERROR_SUCCESS;

    /* the maximum int64 decimal presentation is 20 characters: -9223372036854775808 */
    /* adding some buffer for leading zeros and spaces... */
    char number_arr[40] = "";
    utf8stringbuf_t number_buf = UTF8STRINGBUF( number_arr );
    result |= utf8stringbuf_copy_view( number_buf, this_ );

    unsigned int byte_length = 0;
    result |= utf8string_parse_int( utf8stringbuf_get_string( number_buf ), &byte_length, out_number );
    assert( byte_length <= (*this_).length );

    if ( out_remainder != NULL )
    {
        *out_remainder = (utf8stringview_t){ .start = (*this_).start + byte_length, .length = (*this_).length - byte_length };
    }
    return result;
}

utf8error_t utf8stringview_parse_float( const utf8stringview_t *this_, double *out_number, utf8stringview_t *out_remainder )
{
    assert( out_number != NULL );
    utf8error_t result = UTF8ERROR_SUCCESS;

    /* the maximum double 64 decimal presentation in e-notation is 23 characters: -2.2250738585072014E-308 */
    /* adding some buffer for leading zeros, spaces, additional accuracy and non-e-notation */
    char number_arr[80] = "";
    utf8stringbuf_t number_buf = UTF8STRINGBUF( number_arr );
    result |= utf8stringbuf_copy_view( number_buf, this_ );

    unsigned int byte_length = 0;
    result |= utf8string_parse_float( utf8stringbuf_get_string( number_buf ), &byte_length, out_number );
    assert( byte_length <= (*this_).length );

    if ( out_remainder != NULL )
    {
        *out_remainder = (utf8stringview_t){ .start = (*this_).start + byte_length, .length = (*this_).length - byte_length };
    }
    return result;
}

#ifdef __cplusplus
}
#endif


/*
 * Copyright 2024-2024 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
