/* File: universal_simple_random.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

extern bool universal_simple_random_initialized;

static inline void universal_simple_random_init ( universal_simple_random_t *this_ )
{
    if ( ! universal_simple_random_initialized )
    {
        clock_t now = clock();  /* integer represents clocks, to be divided by CLOCKS_PER_SEC */
#ifdef _WIN32
        srand( now );
#else  /* POSIX.1-2001 */
        srandom( now );
#endif
        universal_simple_random_initialized = true;
    }

    (*this_).dummy = 0;  /* prevent warnings on uninitialized usage */
}

static inline void universal_simple_random_destroy ( universal_simple_random_t *this_ )
{
}

static inline uint16_t universal_simple_random_get_uint16 ( universal_simple_random_t *this_ )
{
    uint16_t result;

#ifdef _WIN32
    if ( RAND_MAX >= (uint16_t)(-1) )
    {
        result = rand();
    }
    else
    {
        result = (((uint_fast16_t)rand()) << 8) ^ rand();
    }
#else  /* POSIX.1-2001 */
    if ( RAND_MAX >= (uint16_t)(-1) )
    {
        result = random();
    }
    else
    {
        result = (((uint_fast16_t)random()) << 8) ^ random();
    }
#endif

    /* Note: random() is possibly not available on win32?, preprocessor-if could distinguish __linux__ and _WIN32 */
    return result;
}

static inline uint32_t universal_simple_random_get_uint32 ( universal_simple_random_t *this_ )
{
    uint32_t result;

#ifdef _WIN32
    if ( RAND_MAX >= (uint32_t)(-1) )
    {
        result = rand();
    }
    else if ( RAND_MAX >= (uint16_t)(-1) )
    {
        result = (((uint_fast32_t)rand()) << 16) ^ rand();
    }
    else
    {
        result = (((uint_fast32_t)rand()) << 24) ^ (((uint_fast32_t)rand()) << 16)
                 ^ (((uint_fast32_t)rand()) << 8) ^ rand();
    }
#else  /* POSIX.1-2001 */
    if ( RAND_MAX >= (uint32_t)(-1) )
    {
        result = random();
    }
    else if ( RAND_MAX >= (uint16_t)(-1) )
    {
        result = (((uint_fast32_t)random()) << 16) ^ random();
    }
    else
    {
        result = (((uint_fast32_t)random()) << 24) ^ (((uint_fast32_t)random()) << 16)
                 ^ (((uint_fast32_t)random()) << 8) ^ random();
    }
#endif

    /* Note: random() is possibly not available on win32?, preprocessor-if could distinguish __linux__ and _WIN32 */
    return result;
}


/*
Copyright 2021-2023 Andreas Warnke

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
