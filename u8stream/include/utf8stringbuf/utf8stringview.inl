/* File: utf8stringview.inl; Copyright and License: see below */

#include "u8/u8_i32.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline utf8error_t utf8stringview_init( utf8stringview_t *this_, const char* start, size_t length )
{
    assert( start != NULL );
    utf8error_t result = UTF8ERROR_SUCCESS;
    /* clean type would have been:  char ( *start_arr )[] = (char(*)[]) start; */

    /* check start */
    char start_copy[4] = {'\0','\0','\0','\0'};
    const size_t start_len = ( length >= 4 ) ? 4 : length;
    memcpy( &start_copy, start, start_len );

    if ( ( 0xc0 & (start_copy[0]) ) == 0x80 )
    {
        if ( ( 0xc0 & (start_copy[1]) ) == 0x80 )
        {
            if ( ( 0xc0 & (start_copy[2]) ) == 0x80 )
            {
                start += 3;
                length -= 3;  /* length was greater than 2 - otherwise start_copy[2] would have been 0x0 */
                result = UTF8ERROR_OUT_OF_RANGE;
            }
            else
            {
                start += 2;
                length -= 2;  /* length was greater than 1 - otherwise start_copy[1] would have been 0x0 */
                result = UTF8ERROR_OUT_OF_RANGE;
            }
        }
        else
        {
            start += 1;
            length -= 1;  /* length was greater than 0 - otherwise start_copy[0] would have been 0x0 */
            result = UTF8ERROR_OUT_OF_RANGE;
        }
    }
    else
    {
        /* valid start */
    }

    /* check end */
    char end_copy[4] = {'\0','\0','\0','\0'};
    const size_t end_len = ( length >= 4 ) ? 4 : length;
    memcpy( &(end_copy[4-end_len]), &(start[length-end_len]), end_len );

    if ( ( 0x80 & (end_copy[3]) ) == 0x00 )
    {
        /* valid single-byte end */
    }
    else
    {
        if ( ( 0xe0 & (end_copy[2]) ) == 0xc0 )
        {
            /* valid 2 byte end */
        }
        else if ( ( 0x80 & (end_copy[2]) ) == 0x00 )
        {
            /* 1 byte char at end_copy[2] */
            length -= 1;  /* length was greater than 0 - otherwise end_copy[3] would have been 0x0 */
            result = UTF8ERROR_OUT_OF_RANGE;
        }
        else
        {
            if ( ( 0xf0 & (end_copy[1]) ) == 0xe0 )
            {
                /* valid 3 byte end */
            }
            else if ( ( 0xe0 & (end_copy[1]) ) == 0xc0 )
            {
                /* 2 byte char at end_copy[1] */
                length -= 1;  /* length was greater than 0 - otherwise end_copy[3] would have been 0x0 */
                result = UTF8ERROR_OUT_OF_RANGE;
            }
            else if ( ( 0x80 & (end_copy[1]) ) == 0x00 )
            {
                /* 1 byte char at end_copy[1] */
                length -= 2;  /* length was greater than 1 - otherwise end_copy[2] would have been 0x0 */
                result = UTF8ERROR_OUT_OF_RANGE;
            }
            else
            {
                if ( ( 0xf8 & (end_copy[0]) ) == 0xf0 )
                {
                    /* valid 4 byte end */
                }
                else
                {
                    /* assume 1 byte char at end_copy[0] */
                    length -= 3;  /* length was greater than 2 - otherwise end_copy[1] would have been 0x0 */
                    result = UTF8ERROR_OUT_OF_RANGE;
                }
            }
        }
    }

    *this_ = (utf8stringview_t){.start=start,.length=length};
    return result;
}

static inline void utf8stringview_init_str( utf8stringview_t *this_, const char* cstring )
{
    *this_ = (utf8stringview_t){.start=cstring,.length=(cstring==NULL)?0:strlen(cstring)};
}

static inline utf8error_t utf8stringview_init_region( utf8stringview_t *this_, const char* cstring, size_t start_idx, size_t length )
{
    assert( cstring != NULL );
    utf8error_t result = UTF8ERROR_SUCCESS;
    const size_t cstring_len = strlen( cstring );
    if ( start_idx > cstring_len )
    {
        *this_ = (utf8stringview_t){.start=cstring+start_idx,.length=0};
        result |= UTF8ERROR_OUT_OF_RANGE;
    }
    else
    {
        const size_t max_len = cstring_len - start_idx;
        result |= utf8stringview_init( this_, cstring+start_idx, u8_i32_min2( length, max_len ) );
        if ( length > max_len )
        {
            /* notify that stringview cannot exceed the cstring */
            result |= UTF8ERROR_OUT_OF_RANGE;
        }
    }
    return result;
}

static inline void utf8stringview_destroy( utf8stringview_t *this_ )
{
    *this_ = (utf8stringview_t){.start=NULL,.length=0};
}

static inline const char* utf8stringview_get_start( const utf8stringview_t *this_ ) {
    return (*this_).start;
}

static inline size_t utf8stringview_get_length( const utf8stringview_t *this_ ) {
    return (*this_).length;
}

static inline size_t utf8stringview_count_codepoints( const utf8stringview_t *this_ ) {
    size_t result = 0;
    unsigned int skip = 0;
    if ( (*this_).start != NULL ) {
        for ( size_t pos = 0; pos < (*this_).length; pos ++ )
        {
            if ( skip > 0 )
            {
                skip --;
                if ( skip == 0 ) {
                    result ++;  /* This is the last byte of a multi byte code point */
                }
            }
            else
            {
                const unsigned char firstByte = (const unsigned char) ((*this_).start[pos]);
                if (( 0x80 & firstByte ) == 0x00 )
                {
                    result ++;  /* This is a 1 byte code point */
                }
                else if (( 0xc0 & firstByte ) == 0x80 )
                {
                    /* This is not a valid first byte, skipping to the next byte... */
                }
                else if (( 0xe0 & firstByte ) == 0xc0 )
                {
                    skip = 1;  /* This is the start of a 2 byte code point */
                }
                else if (( 0xf0 & firstByte ) == 0xe0 )
                {
                    skip = 2;  /* This is the start of a 3 byte code point */
                }
                else if (( 0xf8 & firstByte ) == 0xf0 )
                {
                    skip = 3;  /* This is the start of a 4 byte code point */
                }
                else
                {
                    /* This is not a valid first byte, skipping to the next byte... */
                }
            }
        }
    }
    return result;
}

static inline bool utf8stringview_equals_str( const utf8stringview_t *this_, const char *that )
{
    bool result;
    if ( that != NULL )
    {
        size_t len = strlen( that );
        if ( len == (*this_).length )
        {
            if ( ( len == 0 )/*&&( this_.length == 0 )*/)
            {
                result = true;
            }
            else
            {
                result = ( 0 == memcmp ( (*this_).start, that, len ) );
            }
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = false;
    }
    return result;
}

static inline bool utf8stringview_equals_view( const utf8stringview_t *this_, const utf8stringview_t *that )
{
    assert( that != NULL );
    bool result;
    if ( (*that).length == (*this_).length )
    {
        if ( ( (*that).length == 0 )/*&&( this_.length == 0 )*/)
        {
            result = true;
        }
        else
        {
            result = ( 0 == memcmp ( (*this_).start, (*that).start, (*that).length ) );
        }
    }
    else
    {
        result = false;
    }
    return result;
}

static inline bool utf8stringview_starts_with_str( const utf8stringview_t *this_, utf8string_t *that )
{
    bool result = false;
    if (( this_ != NULL )&&( that != NULL ))
    {
        const size_t that_len = strlen( that );
        if ( that_len <= (*this_).length )
        {
            result = ( 0 == memcmp( (*this_).start, that, that_len ) );
        }
        else
        {
            result = false;
        }
    }
    return result;
}

static inline bool utf8stringview_starts_with_view( const utf8stringview_t *this_, const utf8stringview_t *that )
{
    assert( that != NULL );
    bool result = false;
    if (( this_ != NULL )&&( that != NULL ))
    {
        if ( (*that).length <= (*this_).length )
        {
            result = ( 0 == memcmp( (*this_).start, (*that).start, (*that).length ) );
        }
        else
        {
            result = false;
        }
    }
    return result;
}

static inline bool utf8stringview_ends_with_str( const utf8stringview_t *this_, utf8string_t *that )
{
    bool result = false;
    if (( this_ != NULL )&&( that != NULL ))
    {
        const size_t that_len = strlen( that );
        if ( that_len <= (*this_).length )
        {
            result = ( 0 == memcmp( (*this_).start + (*this_).length - that_len, that, that_len ) );
        }
        else
        {
            result = false;
        }
    }
    return result;
}

static inline bool utf8stringview_ends_with_view( const utf8stringview_t *this_, const utf8stringview_t *that )
{
    assert( that != NULL );
    bool result = false;
    if (( this_ != NULL )&&( that != NULL ))
    {
        if ( (*that).length <= (*this_).length )
        {
            result = ( 0 == memcmp( (*this_).start + (*this_).length - (*that).length, (*that).start, (*that).length ) );
        }
        else
        {
            result = false;
        }
    }
    return result;
}

static inline bool utf8stringview_contains_str( const utf8stringview_t *this_, utf8string_t *that )
{
    bool result = false;
    if (( this_ != NULL )&&( that != NULL ))
    {
        const size_t that_len = strlen( that );
        if ( that_len <= (*this_).length )
        {
            const char *const end = (*this_).start + (*this_).length - that_len;
            for ( const char* pos = (*this_).start; ( pos <= end )&&( result == false ); pos ++ )
            {
                if ( 0 == memcmp( pos, that, that_len ) )
                {
                    result = true;
                }
            }
        }
    }
    return result;
}

static inline bool utf8stringview_contains_view( const utf8stringview_t *this_, const utf8stringview_t *that )
{
    assert( that != NULL );
    bool result = false;
    if (( this_ != NULL )&&( that != NULL ))
    {
        if ( (*that).length <= (*this_).length )
        {
            const char *const end = (*this_).start + (*this_).length - (*that).length;
            for ( const char* pos = (*this_).start; ( pos <= end )&&( result == false ); pos ++ )
            {
                if ( 0 == memcmp( pos, (*that).start, (*that).length ) )
                {
                    result = true;
                }
            }
        }
    }
    return result;
}

static inline utf8error_t utf8stringview_split_at_first_str( const utf8stringview_t *this_,
                                                             utf8string_t *pattern,
                                                             utf8stringview_t *out_before,
                                                             utf8stringview_t *out_after )
{
    utf8error_t result = UTF8ERROR_NOT_FOUND;

    if (( pattern != NULL )&&( this_ != NULL ))
    {
        const size_t pattern_len = strlen( pattern );
        if ( pattern_len <= (*this_).length )
        {
            const char *const end = (*this_).start + (*this_).length - pattern_len;
            for ( const char* pos = (*this_).start; ( pos <= end )&&( result == UTF8ERROR_NOT_FOUND ); pos ++ )
            {
                if ( 0 == memcmp( pos, pattern, pattern_len ) )
                {
                    result = UTF8ERROR_SUCCESS;
                    if ( out_before != NULL )
                    {
                        *out_before = (utf8stringview_t){ .start = (*this_).start, .length = ( pos - (*this_).start ) };
                    }
                    if ( out_after != NULL )
                    {
                        *out_after = (utf8stringview_t){ .start = ( pos + pattern_len ), .length = ( end - pos ) };
                    }
                }
            }
        }
    }
    else
    {
        result = UTF8ERROR_NULL_PARAM;
    }

    return result;
}

static inline utf8error_t utf8stringview_split_at_first_view( const utf8stringview_t *this_,
                                                              const utf8stringview_t *pattern,
                                                              utf8stringview_t *out_before,
                                                              utf8stringview_t *out_after )
{
    assert( pattern != NULL );
    utf8error_t result = UTF8ERROR_NOT_FOUND;

    if ( (*pattern).length <= (*this_).length )
    {
        const char *const end = (*this_).start + (*this_).length - (*pattern).length;
        for ( const char* pos = (*this_).start; ( pos <= end )&&( result == UTF8ERROR_NOT_FOUND ); pos ++ )
        {
            if ( 0 == memcmp( pos, (*pattern).start, (*pattern).length ) )
            {
                result = UTF8ERROR_SUCCESS;
                if ( out_before != NULL )
                {
                    *out_before = (utf8stringview_t){ .start = (*this_).start, .length = ( pos - (*this_).start ) };
                }
                if ( out_after != NULL )
                {
                    *out_after = (utf8stringview_t){ .start = ( pos + (*pattern).length ), .length = ( end - pos ) };
                }
            }
        }
    }

    return result;
}

static inline utf8error_t utf8stringview_split_at_last_str( const utf8stringview_t *this_,
                                                            utf8string_t *pattern,
                                                            utf8stringview_t *out_before,
                                                            utf8stringview_t *out_after )
{
    utf8error_t result = UTF8ERROR_NOT_FOUND;

    if (( pattern != NULL )&&( this_ != NULL ))
    {
        const size_t pattern_len = strlen( pattern );
        if ( pattern_len <= (*this_).length )
        {
            for ( ptrdiff_t pos = (*this_).length - pattern_len; ( pos >= 0 )&&( result == UTF8ERROR_NOT_FOUND ); pos -- )
            {
                if ( 0 == memcmp( (*this_).start + pos, pattern, pattern_len ) )
                {
                    result = UTF8ERROR_SUCCESS;
                    if ( out_before != NULL )
                    {
                        *out_before = (utf8stringview_t){ .start = (*this_).start, .length = pos };
                    }
                    if ( out_after != NULL )
                    {
                        *out_after = (utf8stringview_t){ .start = ( (*this_).start + pos + pattern_len ), .length = ( (*this_).length - pattern_len - pos ) };
                    }
                }
            }
        }
    }
    else
    {
        result = UTF8ERROR_NULL_PARAM;
    }

    return result;
}

static inline utf8error_t utf8stringview_split_at_last_view( const utf8stringview_t *this_,
                                                             const utf8stringview_t *pattern,
                                                             utf8stringview_t *out_before,
                                                             utf8stringview_t *out_after )
{
    assert( pattern != NULL );
    utf8error_t result = UTF8ERROR_NOT_FOUND;

    if ( (*pattern).length <= (*this_).length )
    {
        for ( ptrdiff_t pos = (*this_).length - (*pattern).length; ( pos >= 0 )&&( result == UTF8ERROR_NOT_FOUND ); pos -- )
        {
            if ( 0 == memcmp( (*this_).start + pos, (*pattern).start, (*pattern).length ) )
            {
                result = UTF8ERROR_SUCCESS;
                if ( out_before != NULL )
                {
                    *out_before = (utf8stringview_t){ .start = (*this_).start, .length = pos };
                }
                if ( out_after != NULL )
                {
                    *out_after = (utf8stringview_t){ .start = ( (*this_).start + pos + (*pattern).length ), .length = ( (*this_).length - (*pattern).length - pos ) };
                }
            }
        }
    }

    return result;
}

#ifdef __cplusplus
}
#endif


/*
 * Copyright 2021-2025 Andreas Warnke
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
