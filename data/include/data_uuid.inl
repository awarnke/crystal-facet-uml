/* File: data_uuid.c; Copyright and License: see below */

#include "u8stream/universal_simple_random.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <time.h>
#include <assert.h>

#define data_uuid_unused(variable) ((void)(variable))

static inline u8_error_t data_uuid_init ( data_uuid_t *this_, utf8string_t uuid_string )
{
    assert( NULL != uuid_string );
    utf8error_t strerr;
    u8_error_t result = U8_ERROR_NONE;

    (*this_).uuid_string = utf8stringbuf_init( sizeof((*this_).private_uuid_string_buffer),
                                               (*this_).private_uuid_string_buffer
                                             );
    strerr = utf8stringbuf_copy_str( (*this_).uuid_string, uuid_string );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        U8_LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }
    else if ( utf8stringbuf_get_length( (*this_).uuid_string ) != DATA_UUID_STRING_LENGTH )
    {
        U8_LOG_ERROR_INT( "uuid_string too short:", utf8stringbuf_get_length( (*this_).uuid_string ) );
        result |= U8_ERROR_VALUE_OUT_OF_RANGE;
    }

    return result;
}

static inline u8_error_t data_uuid_reinit ( data_uuid_t *this_, utf8string_t uuid_string )
{
    return data_uuid_init( this_, uuid_string );
}

static inline void data_uuid_init_new ( data_uuid_t *this_ )
{
    (*this_).uuid_string = utf8stringbuf_init( sizeof((*this_).private_uuid_string_buffer),
                                               (*this_).private_uuid_string_buffer
                                             );
    utf8stringbuf_clear( (*this_).uuid_string );

    /* Get current time to enrich the universal_random_t by additional entropy. */
    /* Otherwise the pseudo-random number only depends on the initial seed and number of samples already produced. */
    clock_t now = clock();  /* integer represents clocks, to be divided by CLOCKS_PER_SEC */

    universal_simple_random_t rnd;
    universal_simple_random_init( &rnd );
    {
        utf8error_t strerr = UTF8ERROR_SUCCESS;

        assert( sizeof(int) >= sizeof(uint32_t) );
        const uint32_t rand1 = now ^ universal_simple_random_get_uint32( &rnd );
        const uint16_t rand2 = universal_simple_random_get_uint16( &rnd );
        const uint16_t rand3 = (universal_simple_random_get_uint16( &rnd ) | 0x4000) & 0x4fff;  /* version 4 (4 bits) */
        const uint16_t rand4 = (universal_simple_random_get_uint16( &rnd ) | 0x8000) & 0xbfff;  /* 2 reserved bits */
        const uint16_t rand5 = universal_simple_random_get_uint16( &rnd );
        const uint32_t rand6 = universal_simple_random_get_uint32( &rnd );

        char thirtyseven_bytes[DATA_UUID_STRING_LENGTH+1];
        const int length = sprintf( &(thirtyseven_bytes[0]),
                                    "%08x-%04x-%04x-%04x-%04x%08x",
                                    rand1,
                                    rand2,
                                    rand3,
                                    rand4,
                                    rand5,
                                    rand6
                                  );
        assert( length == sizeof(thirtyseven_bytes) - sizeof((const char)'\0') );
        data_uuid_unused( length );
        strerr |= utf8stringbuf_append_str( (*this_).uuid_string, &(thirtyseven_bytes[0]) );
        assert( strerr == UTF8ERROR_SUCCESS );
    }
    universal_simple_random_destroy( &rnd );
}

static inline void data_uuid_copy ( data_uuid_t *this_, const data_uuid_t *original )
{
    assert( NULL != original );

    (*this_) = (*original);
    /* repair the overwritten pointers */
    (*this_).uuid_string = utf8stringbuf_init( sizeof((*this_).private_uuid_string_buffer),
                                               (*this_).private_uuid_string_buffer
                                             );
}

static inline void data_uuid_replace ( data_uuid_t *this_, const data_uuid_t *that )
{
    assert( NULL != that );

    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).uuid_string = utf8stringbuf_init( sizeof((*this_).private_uuid_string_buffer),
                                               (*this_).private_uuid_string_buffer
                                             );
}

static inline void data_uuid_destroy ( data_uuid_t *this_ )
{
}

static inline utf8string_t data_uuid_get_string ( const data_uuid_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).uuid_string );
}

static inline void data_uuid_trace ( const data_uuid_t *this_ )
{
    U8_TRACE_INFO_STR( "data_uuid_t", utf8stringbuf_get_string( (*this_).uuid_string ) );
}


/*
Copyright 2021-2022 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
