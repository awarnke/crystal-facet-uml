/* File: utf8string.c; Copyright and License: see below */

#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8codepoint.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8_test_cond.h"
#include "u8/u8_fault_inject.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

utf8error_t utf8string_parse_int( utf8string_t *this_, unsigned int *out_byte_length, int64_t *out_number )
{
    utf8error_t result = UTF8ERROR_SUCCESS;
    if (( this_ != NULL )&&( out_number != NULL )) {
        char *endptr = NULL;
        errno = 0;
        long long parseResult = strtoll( this_, &endptr, 10 /* base */);
        if ((parseResult==0)||(parseResult==LLONG_MIN)||(parseResult==LLONG_MAX))
        {
            if (( errno == ERANGE )||( errno == EINVAL )) {
                result = UTF8ERROR_OUT_OF_RANGE;
            }
        }
        assert( endptr != NULL );  /* this should not happen, but do not take this for granted */
        U8_FAULT_INJECT_COND_SET( U8_TEST_COND_STRTOLL, endptr, NULL );
        unsigned int length;
        if ( endptr == NULL ) {
            length = utf8string_get_length( this_ );
        }
        else {
            length = (int)(endptr-this_);
        }
        if ( out_byte_length != NULL ) {
            *out_byte_length = length;
        }
        if ( length == 0 )
        {
            result = UTF8ERROR_NOT_FOUND;
        }
        *out_number = parseResult;
    }
    else {
        result = UTF8ERROR_NULL_PARAM;
    }
    return result;
}

utf8error_t utf8string_parse_float( utf8string_t *this_, unsigned int *out_byte_length, double *out_number )
{
    utf8error_t result = UTF8ERROR_SUCCESS;
    if (( this_ != NULL )&&( out_number != NULL )) {
        char *endptr = NULL;
        errno = 0;
        const char *const default_locale_temp = setlocale( LC_NUMERIC, NULL );  /* get the current locale */
        char default_locale_buf[20];  /* expecting max length = 19: sr_YU.utf8@cyrillic */
        utf8stringbuf_t default_locale = UTF8STRINGBUF( default_locale_buf );
        utf8stringbuf_copy_str( &default_locale, default_locale_temp );
        const char *const c_locale = setlocale( LC_NUMERIC, "C" );  /* set separator to point (by C locale) */
        assert ( c_locale != NULL );
        (void) c_locale;
        double parseResult = strtod( this_, &endptr );
        const char *const restored_locale = setlocale( LC_NUMERIC, utf8stringbuf_get_string( &default_locale ) );  /* set separator back to previous character */
        assert ( restored_locale != NULL );
        (void) restored_locale;
        if ((parseResult==INFINITY)||(parseResult==-INFINITY)||(parseResult==0.0)||(parseResult==-0.0))
        {
            if (( errno == ERANGE )||( errno == EINVAL )) {
                result = UTF8ERROR_OUT_OF_RANGE;
            }
        }
        assert( endptr != NULL );  /* this should not happen, but do not take this for granted */
        U8_FAULT_INJECT_COND_SET( U8_TEST_COND_STRTOD, endptr, NULL );
        unsigned int length;
        if ( endptr == NULL ) {
            length = utf8string_get_length( this_ );
        }
        else {
            length = (int)(endptr-this_);
        }
        if ( out_byte_length != NULL ) {
            *out_byte_length = length;
        }
        if ( length == 0 )
        {
            result = UTF8ERROR_NOT_FOUND;
        }
        *out_number = parseResult;
    }
    else {
        result = UTF8ERROR_NULL_PARAM;
    }
    return result;
}

#ifdef __cplusplus
}
#endif


/*
 * Copyright 2023-2026 Andreas Warnke
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
