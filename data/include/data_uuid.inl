/* File: data_uuid.c; Copyright and License: see below */

#include "universal_random.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

static inline data_error_t data_uuid_init ( data_uuid_t *this_, utf8string_t uuid_string )
{
    assert( NULL != uuid_string );
    utf8error_t strerr;
    data_error_t result = DATA_ERROR_NONE;

    (*this_).uuid_string = utf8stringbuf_init( sizeof((*this_).private_uuid_string_buffer),
                                               (*this_).private_uuid_string_buffer
                                             );
    strerr = utf8stringbuf_copy_str( (*this_).uuid_string, uuid_string );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }
    else if ( utf8stringbuf_get_length( (*this_).uuid_string ) != DATA_UUID_STRING_LENGTH )
    {
        TSLOG_ERROR_INT( "uuid_string too short:", utf8stringbuf_get_length( (*this_).uuid_string ) );
        //result |= DATA_ERROR_VALUE_OUT_OF_RANGE;
        // ALLOW THE EMPTY STRING FOR NOW... - TODO
    }

    return result;
}

static inline void data_uuid_init_new ( data_uuid_t *this_ )
{
    (*this_).uuid_string = utf8stringbuf_init( sizeof((*this_).private_uuid_string_buffer),
                                               (*this_).private_uuid_string_buffer
                                             );
    utf8stringbuf_clear( (*this_).uuid_string );

    /* get current time to enrich the universal_random_t by additional emtropy */
    struct timespec now;
    const int err = clock_gettime( CLOCK_MONOTONIC, &now );
    if ( err != 0 )
    {
        TSLOG_ERROR_INT( "clock_gettime(CLOCK_MONOTONIC) failed:", errno );
        assert(false);
    }

    universal_random_t rnd;
    universal_random_init( &rnd );
    {
        utf8error_t strerr = UTF8ERROR_SUCCESS;

        assert( sizeof(long) >= sizeof(uint32_t) );
        const uint32_t rand1 = now.tv_nsec ^ universal_random_get_long( &rnd );
        const uint16_t rand2 = now.tv_sec ^ universal_random_get_long( &rnd );
        const uint16_t rand3 = (universal_random_get_long( &rnd )) & 0x0fff;
        const uint16_t rand4 = universal_random_get_long( &rnd );
        const uint16_t rand5 = universal_random_get_long( &rnd );
        const uint32_t rand6 = universal_random_get_long( &rnd );

        char thirtyseven_bytes[DATA_UUID_STRING_LENGTH+1];
        const int length = sprintf( &(thirtyseven_bytes[0]),
                                    /*         v UUID version 4 */
                                    "%08x-%04x-4%03x-%04x-%04x%08x",
                                    rand1,
                                    rand2,
                                    rand3,
                                    rand4,
                                    rand5,
                                    rand6
                                  );
        assert( length == sizeof(thirtyseven_bytes) - sizeof((const char)'\0') );
        strerr |= utf8stringbuf_append_str( (*this_).uuid_string, &(thirtyseven_bytes[0]) );
        assert( strerr == UTF8ERROR_SUCCESS );
    }
    universal_random_destroy( &rnd );
}

static inline void data_uuid_init_void ( data_uuid_t *this_ )
{
    (*this_).uuid_string = utf8stringbuf_init( sizeof((*this_).private_uuid_string_buffer),
                                               (*this_).private_uuid_string_buffer
                                             );
    utf8stringbuf_clear( (*this_).uuid_string );
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
    TRACE_INFO_STR( "data_uuid_t", utf8stringbuf_get_string( (*this_).uuid_string ) );
}


/*
Copyright 2021-2021 Andreas Warnke

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