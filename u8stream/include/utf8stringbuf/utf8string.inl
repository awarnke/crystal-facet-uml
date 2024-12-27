/* File: utf8string.inl; Copyright and License: see below */

#include "utf8stringbuf/utf8codepoint.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \enum utf8string_bool_enum
 *  \private
 */
/*  enumeration for true and false or success and failure */
enum utf8string_bool_enum {UTF8STRING_FALSE=0, UTF8STRING_TRUE=1,};

static inline size_t utf8string_get_size( utf8string_t *this_ ) {
    size_t sizeResult = 0;
    if ( this_ != NULL ) {
        sizeResult = strlen( this_ ) + 1; /* 1 for size of terminating 0 character */
    }
    return sizeResult;
}

static inline unsigned int utf8string_get_length( utf8string_t *this_ ) {
    unsigned int lenResult = 0;
    if ( this_ != NULL ) {
        lenResult = strlen( this_ );
    }
    return lenResult;
}

static inline int utf8string_equals_str( utf8string_t *this_, utf8string_t *that ) {
    int cmpResult = -1;
    if (( this_ != NULL ) && ( that != NULL )) {
        cmpResult = strcmp( this_, that );
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline int utf8string_starts_with_str( utf8string_t *this_, utf8string_t *that ) {
    int cmpResult = -1;
    if (( this_ != NULL )&&( that != NULL )) {
        unsigned int thatLen = strlen( that );
        cmpResult = strncmp( this_, that, thatLen );
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline int utf8string_ends_with_str( utf8string_t *this_, utf8string_t *that ) {
    int cmpResult = -1;
    if (( this_ != NULL )&&( that != NULL )) {
        unsigned int thatLen = strlen( that );
        unsigned int thisLen = strlen( this_ );
        if ( thatLen <= thisLen ) {
            cmpResult = memcmp( &(this_[thisLen-thatLen]), that, thatLen );
        }
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline bool utf8string_contains_str( utf8string_t *this_, utf8string_t *pattern ) {
    bool result = false;
    if (( pattern != NULL )&&( this_ != NULL )) {
        const char *ptrResult = strstr( this_, pattern );
        if ( ptrResult != NULL ) {
            result = true;
        }
    }
    return result;
}

static inline utf8codepoint_t utf8string_get_char_at( utf8string_t *this_, unsigned int byte_index ) {
    utf8codepoint_t result = UTF8CODEPOINT_INVAL_CHAR;
    unsigned int thisSize = utf8string_get_size( this_ );
    if ( byte_index < thisSize ) {
        result = utf8codepoint_init( &(this_[byte_index]), thisSize-byte_index );
    }
    return result;
}

#ifdef __cplusplus
}
#endif


/*
 * Copyright 2012-2024 Andreas Warnke
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
