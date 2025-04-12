/* File: utf8stringbuf.inl; Copyright and License: see below */

#include "utf8stringbuf/utf8codepoint.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

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
 *  \fn utf8_string_buf_private_make_null_termination( utf8stringbuf_t *this_ )
 *  \private
 *  \return new, truncated length of the string
 */
/* function to make a buffer null terminated while ensuring that all utf8 character sequences are valid */
extern unsigned int utf8_string_buf_private_make_null_termination( utf8stringbuf_t *this_ );

static inline utf8stringbuf_t utf8stringbuf( char *that )
{
    utf8stringbuf_t result;
    if ( that == NULL )
    {
        result.size = 1;
        result.buf = utf8stringbuf_private_empty_buf;
    }
    else
    {
        result.size = strlen( that )+1;
        result.buf = that;
    }
    return result;
}

static inline utf8stringbuf_t utf8stringbuf_new( char *buf, size_t size )
{
    utf8stringbuf_t result;
    if (( buf == NULL )||(size==0))
    {
        result.size = 1;
        result.buf = utf8stringbuf_private_empty_buf;
    }
    else
    {
        result.size = size;
        result.buf = buf;
    }
    return result;
}

static inline void utf8stringbuf_clear( utf8stringbuf_t *this_ )
{
    memset( (*this_).buf, '\0', (*this_).size );
}

static inline char* utf8stringbuf_get_string( const utf8stringbuf_t *this_ )
{
    return (*this_).buf;
}

static inline size_t utf8stringbuf_get_size( const utf8stringbuf_t *this_ )
{
    return (*this_).size;
}

static inline unsigned int utf8stringbuf_get_length( const utf8stringbuf_t *this_ )
{
    unsigned int lenResult;
    lenResult = strlen( (*this_).buf );
    return lenResult;
}

static inline utf8stringview_t utf8stringbuf_get_view( const utf8stringbuf_t *this_ )
{
    utf8stringview_t result;
    assert( NULL != (*this_).buf );
    const utf8error_t err = utf8stringview_init( &result, (*this_).buf, strlen( (*this_).buf ) );
    assert( UTF8ERROR_SUCCESS == err );
    (void) err;  /* do not warn on unused value */
    return result;
}

static inline int utf8stringbuf_equals_str( const utf8stringbuf_t *this_, const char *that )
{
    int cmpResult = -1;
    if ( that != NULL ) {
        cmpResult = strcmp( (*this_).buf, that );
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_equals_view( const utf8stringbuf_t *this_, const utf8stringview_t *that )
{
    assert( that != NULL );
    int result;
    const size_t len = utf8stringview_get_length( that );
    if ( len == utf8stringbuf_get_length(this_) )
    {
        if ( ( len == 0 )/*&&( (*this_).length == 0 )*/)
        {
            result = 1;
        }
        else
        {
            result = ( 0 == memcmp ( (*this_).buf, utf8stringview_get_start(that), len ) ) ? 1 : 0;
        }
    }
    else
    {
        result = 0;
    }
    return result;
}

static inline int utf8stringbuf_equals_buf( const utf8stringbuf_t *this_, const utf8stringbuf_t *that )
{
    int cmpResult = -1;
    cmpResult = strcmp( (*this_).buf, (*that).buf );
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_find_first_buf( const utf8stringbuf_t *this_, const utf8stringbuf_t pattern )
{
    int result = UTF8STRINGBUF_NOT_FOUND;
    const char *ptrResult = strstr( (*this_).buf, pattern.buf );
    if ( ptrResult != NULL ) {
        result = (int) (ptrResult - (*this_).buf);
    }
    return result;
}

static inline int utf8stringbuf_starts_with_str( const utf8stringbuf_t *this_, const char *that )
{
    int cmpResult = -1;
    if ( that != NULL ) {
        unsigned int thatLen = strlen( that );
        cmpResult = strncmp( (*this_).buf, that, thatLen );
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_starts_with_buf( const utf8stringbuf_t *this_, const utf8stringbuf_t *that ) {
    int cmpResult = -1;
    unsigned int thatLen = strlen( (*that).buf );
    cmpResult = strncmp( (*this_).buf, (*that).buf, thatLen );
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_ends_with_str( const utf8stringbuf_t *this_, const char *that ) {
    int cmpResult = -1;
    if ( that != NULL ) {
        unsigned int thatLen = strlen( that );
        unsigned int thisLen = strlen( (*this_).buf );
        if ( thatLen <= thisLen ) {
            cmpResult = memcmp( &((*this_).buf[thisLen-thatLen]), that, thatLen );
        }
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8stringbuf_ends_with_buf( const utf8stringbuf_t *this_, const utf8stringbuf_t *that ) {
    int cmpResult = -1;
    unsigned int thatLen = strlen( (*that).buf );
    unsigned int thisLen = strlen( (*this_).buf );
    if ( thatLen <= thisLen ) {
        cmpResult = memcmp( &((*this_).buf[thisLen-thatLen]), (*that).buf, thatLen );
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline utf8error_t utf8stringbuf_copy_buf( utf8stringbuf_t *this_, const utf8stringbuf_t *original ) {
    utf8error_t complete = UTF8ERROR_SUCCESS;
#if __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
    strncpy( (*this_).buf, (*original).buf, (*this_).size );
#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif
    if ( (*this_).buf[(*this_).size-1] != '\0' ) {
        utf8_string_buf_private_make_null_termination( this_ );
        complete = UTF8ERROR_TRUNCATED;
    }
    return complete;
}

static inline utf8error_t utf8stringbuf_copy_str( utf8stringbuf_t *this_, const char *original ) {
    utf8error_t complete = UTF8ERROR_SUCCESS;
    if ( original == NULL ) {
        (*this_).buf[0] = '\0';
        complete = UTF8ERROR_NULL_PARAM;
    }
    else {
#if __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
        strncpy( (*this_).buf, original, (*this_).size );
#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif
        if ( (*this_).buf[(*this_).size-1] != '\0' ) {
            utf8_string_buf_private_make_null_termination( this_ );
            complete = UTF8ERROR_TRUNCATED;
        }
    }
    return complete;
}

static inline utf8error_t utf8stringbuf_copy_view( utf8stringbuf_t *this_, const utf8stringview_t *original )
{
    assert( original != NULL );
    utf8error_t result = UTF8ERROR_SUCCESS;

    const size_t origLen = utf8stringview_get_length( original );
    if ( origLen < (*this_).size ) {
        memcpy( &((*this_).buf[0]), utf8stringview_get_start( original ), origLen );
        (*this_).buf[origLen] = '\0';
    }
    else {
        if ( (*this_).size > 0 )
        {
            memcpy( &((*this_).buf[0]), utf8stringview_get_start( original ), ((*this_).size-1) );
        }
        else
        {
            /* buffer non-existant */
        }
        utf8_string_buf_private_make_null_termination( this_ );
        result = UTF8ERROR_TRUNCATED;
    }

    return result;
}

static inline utf8error_t utf8stringbuf_append_str( utf8stringbuf_t *this_, const char *appendix ) {
    utf8error_t result = UTF8ERROR_SUCCESS;
    if ( appendix == NULL ) {
        result = UTF8ERROR_NULL_PARAM;
    }
    else {
        const size_t start = strlen( (*this_).buf );

        const size_t appLen = strlen( appendix );
        if ( start + appLen < (*this_).size ) {
            /* gcc does not reliably evaluate the if condition above */
#if __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
            memcpy( &((*this_).buf[start]), appendix, appLen+1 );
#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif
        }
        else {
            const size_t appPartLen = ((*this_).size-start)-1;  /* cannot be negative */
            if (( appPartLen > 0 )&&( appPartLen <= PTRDIFF_MAX ))  /* check to suppress compiler warning */
            {
                memcpy( &((*this_).buf[start]), appendix, appPartLen );
            }
            else
            {
                /* buffer full */
            }
            utf8_string_buf_private_make_null_termination( this_ );
            result = UTF8ERROR_TRUNCATED;
        }

        /* For the standard use case, strlen and memcpy are faster than strncpy; strncat does not provide an error */
    }
    return result;
}

static inline utf8error_t utf8stringbuf_append_buf( utf8stringbuf_t *this_, const utf8stringbuf_t *appendix ) {
    utf8error_t result = UTF8ERROR_SUCCESS;
    const size_t start = strlen( (*this_).buf );

    const size_t appLen = strlen( (*appendix).buf );
    if ( start + appLen < (*this_).size ) {
        memcpy( &((*this_).buf[start]), (*appendix).buf, appLen+1 );
    }
    else {
        const size_t appPartLen = ((*this_).size-start)-1;
        if (( appPartLen > 0 )&&( appPartLen <= PTRDIFF_MAX ))  /* check to suppress compiler warning */
        {
            memcpy( &((*this_).buf[start]), (*appendix).buf, appPartLen );
        }
        else
        {
            /* buffer full */
        }
        utf8_string_buf_private_make_null_termination( this_ );
        result = UTF8ERROR_TRUNCATED;
    }

    /* For the standard use case, strlen and memcpy are faster than strncpy; strncat does not provide an error */
    return result;
}

static inline utf8error_t utf8stringbuf_append_int( utf8stringbuf_t *this_, const int64_t appendix ) {
    char numberStr[21]; /* this is sufficient for signed 64 bit integers: -9223372036854775806 */
    /* Note: snprintf is not available on every OS */
    sprintf( numberStr, utf8stringbuf_private_format_signed_64_bit_int, appendix );
    return utf8stringbuf_append_str( this_, numberStr );
}

static inline utf8error_t utf8stringbuf_append_hex( utf8stringbuf_t *this_, const uint64_t appendix ) {
    char numberStr[17]; /* this is sufficient for 64 bit integers */
    /* Note: snprintf is not available on every OS */
    sprintf( numberStr, utf8stringbuf_private_format_64_bit_hex, appendix );
    return utf8stringbuf_append_str( this_, numberStr );
}

static inline utf8stringbuf_t utf8stringbuf_get_end( utf8stringbuf_t *this_ ) {
    unsigned int this_Length = utf8stringbuf_get_length( this_ );
    return utf8stringbuf_new( &((*this_).buf[this_Length]), (*this_).size - this_Length );
}

static inline utf8error_t utf8stringbuf_append_view( utf8stringbuf_t *this_, const utf8stringview_t *appendix )
{
    assert( appendix != NULL );
    utf8error_t result = UTF8ERROR_SUCCESS;
    const size_t start = strlen( (*this_).buf );

    const size_t appLen = utf8stringview_get_length( appendix );
    if ( start + appLen < (*this_).size ) {
        memcpy( &((*this_).buf[start]), utf8stringview_get_start( appendix ), appLen );
        (*this_).buf[start+appLen] = '\0';
    }
    else {
        const size_t appPartLen = ((*this_).size-start)-1;
        if (( appPartLen > 0 )&&( appPartLen <= PTRDIFF_MAX ))  /* check to suppress compiler warning */
        {
            memcpy( &((*this_).buf[start]), utf8stringview_get_start( appendix ), appPartLen );
        }
        else
        {
            /* buffer full */
        }
        utf8_string_buf_private_make_null_termination( this_ );
        result = UTF8ERROR_TRUNCATED;
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
