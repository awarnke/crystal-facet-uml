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
        struct timespec seed;
        const int err = clock_gettime( CLOCK_REALTIME, &seed );
        if ( err != 0 )
        {
            TSLOG_ERROR_INT( "clock_gettime(CLOCK_MONOTONIC) failed:", errno );
            assert(false);
            /* do not continue if generated uuids are not unique */
            exit(-1);
        }
        uint32_t high_bits_used = seed.tv_nsec<<2;
        srandom( high_bits_used ^ seed.tv_sec );

        universal_random_initialized = true;
    }
    (*this_).dummy=0;
}

static inline void universal_random_destroy ( universal_random_t *this_ )
{
}

static inline long universal_random_get_long ( universal_random_t *this_ )
{
    return random();
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
