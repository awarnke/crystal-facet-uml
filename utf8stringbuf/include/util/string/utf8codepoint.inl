/*!
 *  \file utf8codepoint.inl
 * 
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2018 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */ 

/*#ifndef UTF8CODEPOINT_INL_*/
/*#define UTF8CODEPOINT_INL_*/

#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
    
/*!
 *  \enum utf8codepoint_enum
 *  \private
 */
/*  enumeration for invalid code points */
enum utf8codepoint_enum {UTF8CODEPOINT_INVALID_LEN=0,};

/*!
 *  \fn utf8codepoint_private_init_from_multi_byte( const char *that, unsigned int max_size )
 *  \private
 *  \param that Pointer to a multi-byte utf-8 character; must not be NULL. 
 *  \param max_size maximum number of bytes to read.  
 */
extern utf8codepoint_t utf8codepoint_private_init_from_multi_byte( const char *that, unsigned int max_size );

static inline utf8codepoint_t utf8codepoint( uint32_t code_point ) {
    utf8codepoint_t result;
    /* for balanced performance between standard and worst cases, */
    /* this_ is implemented as asymmetric decision tree: */
    /* in the best case, we have 2 comparisons, in the worst case 4 */
    result.byte_length = 
            ( code_point <= 0x7ff ) 
            ? (
                    ( code_point <= 0x7f ) ? 1 : 2
            )
            : (
                    ( code_point <= 0x1fffff ) 
                    ? (
                            ( code_point <= 0xffff ) ? 3 : 4
                    )
                    : (
                            ( code_point <= 0x3ffffff ) 
                            ? 5 
                            : (
                                    ( code_point <= 0x7fffffff ) ? 6 : UTF8CODEPOINT_INVALID_LEN
                            )
                    )
            );
    result.code_point = code_point;
    return result;
}

static inline utf8codepoint_t utf8codepoint_init( const char *that, unsigned int max_size ) {
    utf8codepoint_t result = { UTF8CODEPOINT_INVALID_LEN, 0x0, };
    if ( that != NULL ) {
        if ( max_size > 0 ) {
            unsigned char firstByte = (unsigned char) (that[0]);
            if (( 0x80 & firstByte ) == 0x00 ) {
                /* 7-bit ASCII character */
                result.byte_length = 1;
                result.code_point = firstByte; 
            }
            else {
                /* call a non-inline function for the difficult part */
                result = utf8codepoint_private_init_from_multi_byte( that, max_size );
            }
        }
    }
    return result;
}

static inline uint32_t utf8codepoint_get_char( const utf8codepoint_t this_ ) {
    return this_.code_point;
}

static inline unsigned int utf8codepoint_get_length( const utf8codepoint_t this_ ) {
    return this_.byte_length;
}

static inline int utf8codepoint_is_valid( const utf8codepoint_t this_ ) {
    return ( UTF8CODEPOINT_INVALID_LEN != this_.byte_length ) ? 1 : 0;
}

static inline int utf8codepoint_is_unicode( const utf8codepoint_t this_ ) {
    int result = 0;
    if ( this_.byte_length != UTF8CODEPOINT_INVALID_LEN ) {
        if ( this_.code_point < 0xd800 ) {
            result = 1;
        }
        else if (( this_.code_point > 0xdfff ) && ( this_.code_point < 0xfdd0 )) {
            result = 1;
        }
        else if (( this_.code_point > 0xfdef ) && ( this_.code_point < 0x110000 )) {
            if (( this_.code_point & 0x00fffe ) != 0x00fffe ) {
                result = 1;
            }
        }
    }
    return result;
}

#ifdef __cplusplus
}
#endif

/*#endif*/ /*UTF8CODEPOINT_INL_*/
