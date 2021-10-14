/* File: universal_random.inl; Copyright and License: see below */

#include "tslog.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

extern bool universal_random_initialized;

static inline void universal_random_init ( universal_random_t *this_ )
{
    if ( ! universal_random_initialized )
    {
        clock_t now = clock();  /* integer represents clocks, to be divided by CLOCKS_PER_SEC */
        srand( now );
        universal_random_initialized = true;
    }
    (*this_).dummy=0;
}

static inline void universal_random_destroy ( universal_random_t *this_ )
{
}

static inline uint16_t universal_random_get_uint16 ( universal_random_t *this_ )
{
    uint16_t result;

    if ( RAND_MAX >= (uint16_t)(-1) )
    {
        result = rand();
    }
    else
    {
        result = (((uint_fast16_t)rand()) << 8) ^ rand();
    }

    /* Note: random() is possibly not available on win32, preprocessor-if could distinguish __linux__ and _WIN32 */
    return result;
}

static inline uint32_t universal_random_get_uint32 ( universal_random_t *this_ )
{
    uint32_t result;

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
        result = (((uint_fast32_t)rand()) << 24) ^ (((uint_fast32_t)rand()) << 16) ^ (((uint_fast32_t)rand()) << 8) ^ rand();
    }

    /* Note: random() is possibly not available on win32, preprocessor-if could distinguish __linux__ and _WIN32 */
    return result;
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
