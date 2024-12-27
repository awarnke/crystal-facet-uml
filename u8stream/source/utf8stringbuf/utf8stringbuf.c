/* File: utf8stringbuf.c; Copyright and License: see below */

#include <inttypes.h>
#include "utf8stringbuf/utf8stringbuf.h"

/* utf8stringbuf_private_empty_buf is constantly 0, but may be overwritten by a 0 - therefore stored in a read-writeable memory page */
char utf8stringbuf_private_empty_buf[1] = "";

const char *utf8stringbuf_private_format_signed_64_bit_int = "%" PRIi64;

const char *utf8stringbuf_private_format_64_bit_hex = "%" PRIx64;

/*!
 *  \fn utf8stringbuf_private_write_char( char *destination, unsigned int max_size, const uint32_t source )
 *  \private
 */
/* function to write a code point as utf8, returns the number of bytes written and an error code */
static inline utf8error_t utf8stringbuf_private_write_char( char *destination, unsigned int max_size, const uint32_t source, int *out_len );

/* utf8 sequences longer or equal 2 bytes start with a byte with 2 highest bits set: 0xc0 */
/* utf8 sequences longer or equal 3 bytes start with a byte with 3 highest bits set: 0xe0 */
/* utf8 sequences           equal 4 bytes start with a byte with 4 highest bits set: 0xf0 */
static const unsigned char utf8stringbuf_private_pattern_to_detect_half_utf8_sequences[5] = { 0, 0, 0xc0, 0xe0, 0xf0 };

/* Note: There is some magic in the design of utf8 which makes the implementation of this function quite short */
unsigned int utf8_string_buf_private_make_null_termination( utf8stringbuf_t *this_ ) {
    unsigned int truncatedLength;
    int clearAtEnd = 1;

    for ( int searchBackwards = 2; searchBackwards <= 4; searchBackwards ++ ) {
        if ( searchBackwards > (*this_).size ) {
            break;
        }
        const char pattern = utf8stringbuf_private_pattern_to_detect_half_utf8_sequences[searchBackwards];
        if ( ( (*this_).buf[(*this_).size-searchBackwards] & pattern ) == pattern ) {
            clearAtEnd = searchBackwards;
            break;
        }
    }

    truncatedLength = (*this_).size - clearAtEnd;
    /* (*this_).buf[truncatedLength] = '\0'; */ /* Note: some functions like splitIn2 require complete zeroed trailings */
    memset( &((*this_).buf[truncatedLength]), '\0', clearAtEnd );
    return truncatedLength;
}

utf8error_t utf8stringbuf_append_char( utf8stringbuf_t *this_, const uint32_t appendix ) {
    utf8error_t result;
    const unsigned int start = utf8stringbuf_get_length( this_ );
    int appendLen;
    result = utf8stringbuf_private_write_char( &((*this_).buf[start]), (*this_).size - start - 1, appendix, &appendLen );
    if ( result == UTF8ERROR_SUCCESS ) {
        (*this_).buf[start+appendLen] = '\0';
    }
    return result;
}

utf8error_t utf8stringbuf_append_wstr( utf8stringbuf_t *this_, const wchar_t *appendix ) {
    utf8error_t result = UTF8ERROR_NULL_PARAM;
    if ( appendix != NULL ) {
        unsigned int start = utf8stringbuf_get_length( this_ );
        result = UTF8ERROR_SUCCESS;
        for( ; appendix[0]!=L'\0'; appendix = &(appendix[1]) ) {
            int appendLen;
            result |= utf8stringbuf_private_write_char( &((*this_).buf[start]), (*this_).size - start - 1, appendix[0], &appendLen );
            if ( result != UTF8ERROR_SUCCESS ) {
                if ( ( result & UTF8ERROR_TRUNCATED ) != 0 ) {
                    break;
                }
            }
            start = start + appendLen;
        }
        (*this_).buf[start] = '\0';
    }
    return result;
}

static inline utf8error_t utf8stringbuf_private_write_char( char *destination, unsigned int max_size, const uint32_t source, int *out_len ) {
    *out_len = 0;
    utf8error_t result = UTF8ERROR_TRUNCATED;
    if ( source <= 0x7ff ) {
        if ( source <= 0x7f ) {
            /* 1 byte character */
            /* check if there is enough space for the character */
            if ( max_size >= 1 ) {
                destination[0] = source;
                *out_len = 1;
                result = UTF8ERROR_SUCCESS;
            }
        }
        else {
            /* 2 byte character */
            if ( max_size >= 2 ) {
                destination[0] = 0xc0 | ( source >> 6 );
                destination[1] = 0x80 | ( source & 0x3f );
                *out_len = 2;
                result = UTF8ERROR_SUCCESS;
            }
        }
    }
    else {
        if ( source <= 0x10ffff ) {
            if ( source <= 0xffff ) {
                /* 3 byte character */
                if ( max_size >= 3 ) {
                    destination[0] = 0xe0 | ( source >> 12 );
                    destination[1] = 0x80 | (( source >> 6 ) & 0x3f );
                    destination[2] = 0x80 | ( source & 0x3f );
                    *out_len = 3;
                    result = UTF8ERROR_SUCCESS;
                }
            }
            else {
                /* 4 byte character */
                if ( max_size >= 4 ) {
                    destination[0] = 0xf0 | ( source >> 18 );
                    destination[1] = 0x80 | (( source >> 12 ) & 0x3f );
                    destination[2] = 0x80 | (( source >> 6 ) & 0x3f );
                    destination[3] = 0x80 | ( source & 0x3f );
                    *out_len = 4;
                    result = UTF8ERROR_SUCCESS;
                }
            }
        }
        else {
            /* note: utf8 can not encode more than 21 bits per character, and even there only 0-0x10ffff is allowed. */
            result = UTF8ERROR_NOT_A_CODEPOINT;
        }
    }
    return result;
}


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
