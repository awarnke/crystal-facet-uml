/*!
 *  \file utf8stringbuf.inl
 *
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2018 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */

/*#ifndef UTF8STRINGBUF_INL_*/
/*#define UTF8STRINGBUF_INL_*/

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "util/string/utf8codepoint.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \enum utf8stringbuf_bool_enum
 *  \private
 */
/*  enumeration for true and false or success and failure */
enum utf8stringbuf_bool_enum {UTF8STRINGBUF_FALSE=0, UTF8STRINGBUF_TRUE=1,};

/*!
 *  \enum utf8stringbuf_search_enum
 *  \private
 */
/*  enumeration for search pattern not found */
enum utf8stringbuf_search_enum {UTF8STRINGBUF_NOT_FOUND=-1,};

/*!
 *  \var utf8stringbuf_private_format_signed_64_bit_int
 *  \private
 */
/*  a character array containing nothing but a single zero */
extern const char *utf8stringbuf_private_format_signed_64_bit_int;

/*!
 *  \var utf8stringbuf_private_format_64_bit_hex
 *  \private
 */
/*  a character array containing nothing but a single zero */
extern const char *utf8stringbuf_private_format_64_bit_hex;

/*!
 *  \var utf8stringbuf_private_empty_buf
 *  \private
 */
/*  a character array containing nothing but a single zero */
extern char utf8stringbuf_private_empty_buf[1];

/*!
 *  \fn utf8_string_buf_private_make_null_termination( utf8stringbuf_t this_ )
 *  \private
 *  \return new, truncated length of the string
 */
/* function to make a buffer null terminated while ensuring that all utf8 character sequences are valid */
extern unsigned int utf8_string_buf_private_make_null_termination( utf8stringbuf_t this_ );

/*!
 *  \fn utf8_string_buf_private_replace_region_by_str( utf8stringbuf_t this_, unsigned int this_Length, int start, int length, const char *replacement )
 *  \private
 */
/* function to replace a region within a string buffer, the original string length must be provided in this_Length */
extern utf8error_t utf8_string_buf_private_replace_region_by_str( utf8stringbuf_t this_, unsigned int this_Length, int start, int length, const char *replacement );

static inline utf8stringbuf_t utf8stringbuf( char *that ) {
    utf8stringbuf_t result;
    if ( that == NULL ) {
        result.size = 1;
        result.buf = utf8stringbuf_private_empty_buf;
    }
    else {
        result.size = strlen( that )+1;
        result.buf = that;
    }
    return result;
}

static inline utf8stringbuf_t utf8stringbuf_init( unsigned int size, char *buf ) {
    utf8stringbuf_t result;
    if (( buf == NULL )||(size==0)) {
        result.size = 1;
        result.buf = utf8stringbuf_private_empty_buf;
    }
    else {
        result.size = size;
        result.buf = buf;
    }
    return result;
}

static inline void utf8stringbuf_clear( utf8stringbuf_t this_ ) {
    memset( this_.buf, '\0', this_.size );
}

static inline char* utf8stringbuf_get_string( const utf8stringbuf_t this_ ) {
    return this_.buf;
}

static inline unsigned int utf8stringbuf_get_size( const utf8stringbuf_t this_ ) {
    return this_.size;
}

static inline unsigned int utf8stringbuf_get_length( const utf8stringbuf_t this_ ) {
    unsigned int lenResult;
    lenResult = strlen( this_.buf );
    return lenResult;
}

static inline int utf8stringbuf_equals_str( const utf8stringbuf_t this_, const char *that ) {
    int cmpResult = -1;
    if ( that != NULL ) {
        cmpResult = strcmp( this_.buf, that );
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_equals_buf( const utf8stringbuf_t this_, const utf8stringbuf_t that ) {
    int cmpResult = -1;
    cmpResult = strcmp( this_.buf, that.buf );
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_equals_region_str( const utf8stringbuf_t this_, int start, const char *that ) {
    int cmpResult = -1;
    if ( that != NULL ) {
        int thisLen = strlen(this_.buf);
        int thatLen = strlen(that);
        if (( 0 <= start )&&( start+thatLen <= thisLen )) {
            cmpResult = memcmp( &(this_.buf[start]), that, thatLen );
        }
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_equals_region_buf( const utf8stringbuf_t this_, int start, const utf8stringbuf_t that ) {
    int cmpResult = -1;
    int thisLen = strlen(this_.buf);
    int thatLen = strlen(that.buf);
    if (( 0 <= start )&&( start+thatLen <= thisLen )) {
        cmpResult = memcmp( &(this_.buf[start]), that.buf, thatLen );
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_find_first_buf( const utf8stringbuf_t this_, const utf8stringbuf_t pattern ) {
    int result = UTF8STRINGBUF_NOT_FOUND;
    const char *ptrResult = strstr( this_.buf, pattern.buf );
    if ( ptrResult != NULL ) {
        result = (int) (ptrResult - this_.buf);
    }
    return result;
}

static inline int utf8stringbuf_starts_with_str( const utf8stringbuf_t this_, const char *that ) {
    int cmpResult = -1;
    if ( that != NULL ) {
        unsigned int thatLen = strlen( that );
        cmpResult = strncmp( this_.buf, that, thatLen );
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_starts_with_buf( const utf8stringbuf_t this_, const utf8stringbuf_t that ) {
    int cmpResult = -1;
    unsigned int thatLen = strlen( that.buf );
    cmpResult = strncmp( this_.buf, that.buf, thatLen );
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_ends_with_str( const utf8stringbuf_t this_, const char *that ) {
    int cmpResult = -1;
    if ( that != NULL ) {
        unsigned int thatLen = strlen( that );
        unsigned int thisLen = strlen( this_.buf );
        if ( thatLen <= thisLen ) {
            cmpResult = memcmp( &(this_.buf[thisLen-thatLen]), that, thatLen );
        }
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_ends_with_buf( const utf8stringbuf_t this_, const utf8stringbuf_t that ) {
    int cmpResult = -1;
    unsigned int thatLen = strlen( that.buf );
    unsigned int thisLen = strlen( this_.buf );
    if ( thatLen <= thisLen ) {
        cmpResult = memcmp( &(this_.buf[thisLen-thatLen]), that.buf, thatLen );
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_find_first_str( const utf8stringbuf_t this_, const char *pattern ) {
    int result = UTF8STRINGBUF_NOT_FOUND;
    if ( pattern != NULL ) {
        const char *ptrResult = strstr( this_.buf, pattern );
        if ( ptrResult != NULL ) {
            result = (int) (ptrResult - this_.buf);
        }
    }
    return result;
}

static inline int utf8stringbuf_find_last_buf( const utf8stringbuf_t this_, const utf8stringbuf_t pattern ) {
    int result = UTF8STRINGBUF_NOT_FOUND;
    int thisLen = utf8stringbuf_get_length( this_ );
    int patternLen = strlen( pattern.buf );
    if ( patternLen <= thisLen ) {
        for ( int probeIdx = (thisLen-patternLen); probeIdx >= 0; probeIdx --) {
            if ( 0 == memcmp( &(this_.buf[probeIdx]), pattern.buf, patternLen )) {
                /* last occurrence found! */
                result = probeIdx;
                break;
            }
        }
    }
    return result;
}

static inline int utf8stringbuf_find_last_str( const utf8stringbuf_t this_, const char *pattern ) {
    int result = UTF8STRINGBUF_NOT_FOUND;
    if ( pattern != NULL ) {
        int thisLen = utf8stringbuf_get_length( this_ );
        int patternLen = strlen( pattern );
        if ( patternLen <= thisLen ) {
            for ( int probeIdx = (thisLen-patternLen); probeIdx >= 0; probeIdx --) {
                if ( 0 == memcmp( &(this_.buf[probeIdx]), pattern, patternLen )) {
                    /* last occurrence found! */
                    result = probeIdx;
                    break;
                }
            }
        }
    }
    return result;
}

static inline int utf8stringbuf_find_next_buf( const utf8stringbuf_t this_, const utf8stringbuf_t pattern, int start_index ) {
    int result = UTF8STRINGBUF_NOT_FOUND;
    if (( start_index >= 0 ) && ( start_index < this_.size )) {
        const char *ptrResult = strstr( &(this_.buf[start_index]), pattern.buf );
        if ( ptrResult != NULL ) {
            result = (int) (ptrResult - this_.buf);
        }
    }
    return result;
}

static inline int utf8stringbuf_find_next_str( const utf8stringbuf_t this_, const char *pattern, int start_index ) {
    int result = UTF8STRINGBUF_NOT_FOUND;
    if (( pattern != NULL ) && ( start_index >= 0 ) && ( start_index < this_.size )) {
        const char *ptrResult = strstr( &(this_.buf[start_index]), pattern );
        if ( ptrResult != NULL ) {
            result = (int) (ptrResult - this_.buf);
        }
    }
    return result;
}

static inline utf8codepoint_t utf8stringbuf_get_char_at( const utf8stringbuf_t this_, unsigned int byte_index ) {
    utf8codepoint_t result = UTF8CODEPOINT_INVAL_CHAR;
    if ( byte_index < this_.size ) {
        result = utf8codepoint_init( &(this_.buf[byte_index]), this_.size-byte_index );
    }
    return result;
}

static inline utf8error_t utf8stringbuf_copy_buf( utf8stringbuf_t this_, const utf8stringbuf_t original ) {
    utf8error_t complete = UTF8ERROR_SUCCESS;
    strncpy( this_.buf, original.buf, this_.size );
    if ( this_.buf[this_.size-1] != '\0' ) {
        utf8_string_buf_private_make_null_termination( this_ );
        complete = UTF8ERROR_TRUNCATED;
    }
    return complete;
}

static inline utf8error_t utf8stringbuf_copy_str( utf8stringbuf_t this_, const char *original ) {
    utf8error_t complete = UTF8ERROR_SUCCESS;
    if ( original == NULL ) {
        this_.buf[0] = '\0';
        complete = UTF8ERROR_NULL_PARAM;
    }
    else {
        strncpy( this_.buf, original, this_.size );
        if ( this_.buf[this_.size-1] != '\0' ) {
            utf8_string_buf_private_make_null_termination( this_ );
            complete = UTF8ERROR_TRUNCATED;
        }
    }
    return complete;
}

static inline utf8error_t utf8stringbuf_replace_all_str_by_str( const utf8stringbuf_t this_, const char *pattern, const char *replacement ) {
    utf8error_t result = UTF8ERROR_NULL_PARAM;
    if ( pattern != NULL ) {
        const char *const patterns_and_replacements[3] = { pattern, replacement, (char*)NULL, };
        result = utf8stringbuf_replace_all( this_, patterns_and_replacements );
    }
    return result;
}

static inline utf8error_t utf8stringbuf_replace_all_buf_by_buf( const utf8stringbuf_t this_, const utf8stringbuf_t pattern, const utf8stringbuf_t replacement ) {
    const char *const patterns_and_replacements[3] = { pattern.buf, replacement.buf, (char*)NULL, };
    return utf8stringbuf_replace_all( this_, patterns_and_replacements );
}

static inline utf8error_t utf8stringbuf_append_str( utf8stringbuf_t this_, const char *appendix ) {
    utf8error_t result = UTF8ERROR_SUCCESS;
    if ( appendix == NULL ) {
        result = UTF8ERROR_NULL_PARAM;
    }
    else {
        unsigned int start = strlen( this_.buf );

        unsigned int appLen = strlen( appendix );
        if ( start + appLen < this_.size ) {
            memcpy( &(this_.buf[start]), appendix, appLen+1 );
        }
        else {
            memcpy( &(this_.buf[start]), appendix, this_.size-start-1 );
            utf8_string_buf_private_make_null_termination( this_ );
            result = UTF8ERROR_TRUNCATED;
        }

        /* For the standard use case, strlen and memcpy are faster than strncpy; strncat does not provide an error */
    }
    return result;
}

static inline utf8error_t utf8stringbuf_append_buf( utf8stringbuf_t this_, const utf8stringbuf_t appendix ) {
    utf8error_t result = UTF8ERROR_SUCCESS;
    unsigned int start = strlen( this_.buf );

    unsigned int appLen = strlen( appendix.buf );
    if ( start + appLen < this_.size ) {
        memcpy( &(this_.buf[start]), appendix.buf, appLen+1 );
    }
    else {
        memcpy( &(this_.buf[start]), appendix.buf, this_.size-start-1 );
        utf8_string_buf_private_make_null_termination( this_ );
        result = UTF8ERROR_TRUNCATED;
    }

    /* For the standard use case, strlen and memcpy are faster than strncpy; strncat does not provide an error */
    return result;
}

static inline utf8error_t utf8stringbuf_append_int( utf8stringbuf_t this_, const int64_t appendix ) {
    char numberStr[21]; /* this is sufficient for signed 64 bit integers: -9223372036854775806 */
    /* Note: snprintf is not available on every OS */
    sprintf( numberStr, utf8stringbuf_private_format_signed_64_bit_int, appendix );
    return utf8stringbuf_append_str( this_, numberStr );
}

static inline utf8error_t utf8stringbuf_append_hex( utf8stringbuf_t this_, const uint64_t appendix ) {
    char numberStr[17]; /* this is sufficient for 64 bit integers */
    /* Note: snprintf is not available on every OS */
    sprintf( numberStr, utf8stringbuf_private_format_64_bit_hex, appendix );
    return utf8stringbuf_append_str( this_, numberStr );
}

static inline utf8error_t utf8stringbuf_replace_region_by_str( utf8stringbuf_t this_, int start, int length, const char *replacement ) {
    unsigned int this_Length = utf8stringbuf_get_length( this_ );
    return utf8_string_buf_private_replace_region_by_str( this_, this_Length, start, length, replacement );
}

static inline utf8error_t utf8stringbuf_replace_region_by_buf( utf8stringbuf_t this_, int start, int length, const utf8stringbuf_t replacement ) {
    unsigned int this_Length = utf8stringbuf_get_length( this_ );
    return utf8_string_buf_private_replace_region_by_str( this_, this_Length, start, length, replacement.buf );
}

static inline utf8error_t utf8stringbuf_delete( utf8stringbuf_t this_, int start, int length ) {
    unsigned int this_Length = utf8stringbuf_get_length( this_ );
    return utf8_string_buf_private_replace_region_by_str( this_, this_Length, start, length, NULL );
}

static inline utf8error_t utf8stringbuf_delete_from_end( utf8stringbuf_t this_, int length ) {
    int this_Length = utf8stringbuf_get_length( this_ );
    return utf8_string_buf_private_replace_region_by_str( this_, this_Length, this_Length-length, length, NULL );
}

static inline utf8error_t utf8stringbuf_delete_to_end( utf8stringbuf_t this_, int start ) {
    int this_Length = utf8stringbuf_get_length( this_ );
    return utf8_string_buf_private_replace_region_by_str( this_, this_Length, start, this_Length-start, NULL );
}

static inline utf8error_t utf8stringbuf_insert_str( utf8stringbuf_t this_, int start, const char *insert ) {
    unsigned int this_Length = utf8stringbuf_get_length( this_ );
    return utf8_string_buf_private_replace_region_by_str( this_, this_Length, start, 0, insert );
}

static inline utf8error_t utf8stringbuf_insert_buf( utf8stringbuf_t this_, int start, const utf8stringbuf_t insert ) {
    unsigned int this_Length = utf8stringbuf_get_length( this_ );
    return utf8_string_buf_private_replace_region_by_str( this_, this_Length, start, 0, insert.buf );
}

static inline utf8stringbuf_t utf8stringbuf_get_end( utf8stringbuf_t this_ ) {
    unsigned int this_Length = utf8stringbuf_get_length( this_ );
    return utf8stringbuf_init( this_.size-this_Length, &(this_.buf[this_Length]) );
}

#ifdef __cplusplus
}
#endif

/*#endif*/ /*UTF8STRINGBUF_INL_*/
