/* File: utf8stringview.inl; Copyright and License: see below */

/*!
 *  \file utf8stringview.inl
 *
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author Copyright 2021-2023 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */

#ifdef __cplusplus
extern "C" {
#endif

static inline utf8stringview_t utf8stringview_init( const char* start, size_t length )
{
    return (utf8stringview_t){.start=start,.length=length};
}

static inline utf8stringview_t utf8stringview_init_str( const char* cstring )
{
    return (utf8stringview_t){.start=cstring,.length=(cstring==NULL)?0:strlen(cstring)};
}

static inline utf8stringview_t utf8stringview_init_region( const char* string, size_t start_idx, size_t length )
{
    return (utf8stringview_t){.start=(string+start_idx),.length=(string==NULL)?0:length};
}

static inline const char* utf8stringview_get_start( const utf8stringview_t this_ ) {
    return this_.start;
}

static inline size_t utf8stringview_get_length( const utf8stringview_t this_ ) {
    return this_.length;
}

static inline size_t utf8stringview_count_codepoints( const utf8stringview_t this_ ) {
    size_t result = 0;
    unsigned int skip = 0;
    if ( this_.start != NULL ) {
        for ( size_t pos = 0; pos < this_.length; pos ++ )
        {
            if ( skip > 0 )
            {
                skip --;
                if ( skip == 0 ) {
                    result ++;
                }
            }
            else
            {
                const unsigned char firstByte = (const unsigned char) (this_.start[pos]);
                if (( 0x80 & firstByte ) == 0x00 )
                {
                    result ++;
                }
                else if ( firstByte < 0xe0 )
                {
                    skip = 1;  /* This is a 2 byte code point */
                }
                else if ( firstByte < 0xf0 )
                {
                    skip = 2;  /* This is a 3 byte code point */
                }
                else if ( firstByte < 0xf8 )
                {
                    skip = 3;  /* This is a 4 byte code point */
                }
            }
        }
    }
    return result;
}

static inline int utf8stringview_equals_str( const utf8stringview_t this_, const char *that )
{
    int result;
    if ( that != NULL )
    {
        size_t len = strlen( that );
        if ( len == this_.length )
        {
            if ( ( len == 0 )/*&&( this_.length == 0 )*/)
            {
                result = 1;
            }
            else
            {
                result = ( 0 == memcmp ( this_.start, that, len ) ) ? 1 : 0;
            }
        }
        else
        {
            result = 0;
        }
    }
    else
    {
        result = 0;
    }
    return result;
}

static inline int utf8stringview_equals_view( const utf8stringview_t this_, const utf8stringview_t that )
{
    int result;
    if ( that.length == this_.length )
    {
        if ( ( that.length == 0 )/*&&( this_.length == 0 )*/)
        {
            result = 1;
        }
        else
        {
            result = ( 0 == memcmp ( this_.start, that.start, that.length ) ) ? 1 : 0;
        }
    }
    else
    {
        result = 0;
    }
    return result;
}

static inline int utf8stringview_find_first_str( const utf8stringview_t this_, const char *pattern ) {
    int result = -1;
    if (( pattern != NULL )&&( this_.start != NULL )) {
        const size_t pattern_len = strlen( pattern );
        if ( pattern_len != 0 )
        {
            const char *const end = this_.start + this_.length;
            for ( const char* pos = this_.start; ( pos + pattern_len <= end )&&( result == -1 ); pos ++ )
            {
                if ( 0 == memcmp( pos, pattern, pattern_len ) )
                {
                    result = ( pos - this_.start );
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
 * Copyright 2021-2023 Andreas Warnke
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
