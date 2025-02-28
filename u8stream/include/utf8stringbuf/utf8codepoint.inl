/* File: utf8codepoint.inl; Copyright and License: see below */

#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \enum utf8codepoint_enum
 *  \private
 */
/*  enumeration for invalid code points */
enum utf8codepoint_enum {UTF8CODEPOINT_INVALID_LEN=0,};

static inline utf8codepoint_t utf8codepoint( uint32_t code_point ) {
    utf8codepoint_t result;
    /* for balanced performance between standard and worst cases, */
    /* this_ is implemented as asymmetric decision tree: */
    /* in the best case, we have 2 comparisons, in the worst case 3 */
    result.byte_length =
            ( code_point <= 0x7ff )
            ? (
                    ( code_point <= 0x7f ) ? 1 : 2
            )
            : (
                    ( code_point <= 0x10ffff )
                    ? (
                            ( code_point <= 0xffff ) ? 3 : 4
                    )
                    : UTF8CODEPOINT_INVALID_LEN
            );
    result.code_point = code_point;
    return result;
}

static inline utf8codepoint_t utf8codepoint_new( const char *that, unsigned int max_size ) {
    utf8codepoint_t result = { UTF8CODEPOINT_INVALID_LEN, 0x0, };
    if (( that != NULL )&&( max_size > 0 ))
    {
        const unsigned char firstByte = (const unsigned char) (that[0]);
        if (( 0x80 & firstByte ) == 0x00 )
        {
            /* 7-bit ASCII character */
            result.byte_length = 1;
            result.code_point = firstByte;
        }
        else if ( firstByte < 0xe0 )
        {
            if ( max_size >= 2 )
            {
                const unsigned char secondByte = (const unsigned char) (that[1]);
                if (( ( 0xe0 & firstByte ) == 0xc0 ) && ( ( 0xc0 & secondByte ) == 0x80 ))
                {
                    /* first and second byte are valid */
                    result.byte_length = 2;
                    result.code_point = (((uint32_t)(firstByte & 0x1f))<<6)
                                        |(secondByte & 0x3f);
                }
            }
        }
        else if ( firstByte < 0xf0 )
        {
            if ( max_size >= 3 )
            {
                const unsigned char secondByte = (const unsigned char) (that[1]);
                const unsigned char thirdByte = (const unsigned char) (that[2]);
                if (( ( 0xc0 & secondByte ) == 0x80 ) && ( ( 0xc0 & thirdByte ) == 0x80 ))
                {
                    /* second and third bytes are valid */
                    result.byte_length = 3;
                    result.code_point = (((uint32_t)(firstByte & 0x0f))<<12)
                                        |(((uint32_t)(secondByte & 0x3f))<<6)
                                        |(thirdByte & 0x3f);
                }
            }
        }
        else if ( firstByte < 0xf8 )
        {
            if ( max_size >= 4 )
            {
                const unsigned char secondByte = (const unsigned char) (that[1]);
                const unsigned char thirdByte = (const unsigned char) (that[2]);
                const unsigned char fourthByte = (const unsigned char) (that[3]);
                if (( ( 0xc0 & secondByte ) == 0x80 )
                   && ( ( 0xc0 & thirdByte ) == 0x80 )
                   && ( ( 0xc0 & fourthByte ) == 0x80 ))
                {
                    /* second, third and fourth bytes are valid */
                    result.byte_length = 4;
                    result.code_point = (((uint32_t)(firstByte & 0x07))<<18)
                                        |(((uint32_t)(secondByte & 0x3f))<<12)
                                        |(((uint32_t)(thirdByte & 0x3f))<<6)
                                        |(fourthByte & 0x3f);
                    if ( result.code_point > 0x10ffff )
                    {
                        /* invalid */
                        result.byte_length = UTF8CODEPOINT_INVALID_LEN;
                    }
                }
            }
        }
    }
    return result;
}

static inline uint32_t utf8codepoint_get_char( const utf8codepoint_t *this_ ) {
    return (*this_).code_point;
}

static inline unsigned int utf8codepoint_get_length( const utf8codepoint_t *this_ ) {
    return (*this_).byte_length;
}

static inline utf8codepointseq_t utf8codepoint_get_utf8( const utf8codepoint_t *this_ ) {
    utf8codepointseq_t result;
    const uint32_t code_point = (*this_).code_point;

    if ( code_point <= 0x7ff )
    {
        if ( code_point <= 0x7f )
        {
            result.seq[0] = code_point;
            result.seq[1] = '\0';
            result.seq[2] = '\0';
            result.seq[3] = '\0';
            assert( (*this_).byte_length == 1 );
        }
        else
        {
            result.seq[0] = (0xc0 | (code_point>>6));
            result.seq[1] = (0x80 | (code_point&0x3f));
            result.seq[2] = '\0';
            result.seq[3] = '\0';
            assert( (*this_).byte_length == 2 );
        }
    }
    else
    {
        if ( code_point <= 0x10ffff )
        {
            if ( code_point <= 0xffff )
            {
                result.seq[0] = (0xe0 | (code_point>>12));
                result.seq[1] = (0x80 | ((code_point>>6)&0x3f));
                result.seq[2] = (0x80 | (code_point&0x3f));
                result.seq[3] = '\0';
                assert( (*this_).byte_length == 3 );
            }
            else
            {
                result.seq[0] = (0xf0 | (code_point>>18));
                result.seq[1] = (0x80 | ((code_point>>12)&0x3f));
                result.seq[2] = (0x80 | ((code_point>>6)&0x3f));
                result.seq[3] = (0x80 | (code_point&0x3f));
                assert( (*this_).byte_length == 4 );
            }
        }
        else
        {
            /* UTF8CODEPOINT_INVALID_LEN */
            result.seq[0] = '\0';
            result.seq[1] = '\0';
            result.seq[2] = '\0';
            result.seq[3] = '\0';
            assert( (*this_).byte_length == 0 );
        }
    }

    return result;
}

static inline int utf8codepoint_is_valid( const utf8codepoint_t *this_ ) {
    return ( UTF8CODEPOINT_INVALID_LEN != (*this_).byte_length ) ? 1 : 0;
}

static inline int utf8codepoint_is_unicode( const utf8codepoint_t *this_ ) {
    int result = 0;
    if ( (*this_).byte_length != UTF8CODEPOINT_INVALID_LEN ) {
        if ( (*this_).code_point < 0xd800 ) {
            result = 1;
        }
        else if (( (*this_).code_point > 0xdfff ) && ( (*this_).code_point < 0xfdd0 )) {
            result = 1;
        }
        else if (( (*this_).code_point > 0xfdef ) && ( (*this_).code_point < 0x110000 )) {
            if (( (*this_).code_point & 0x00fffe ) != 0x00fffe ) {
                result = 1;
            }
        }
    }
    return result;
}

#ifdef __cplusplus
}
#endif


/*
 * Copyright 2012-2025 Andreas Warnke
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
