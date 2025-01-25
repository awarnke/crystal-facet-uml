/* File: u8_interval.inl; Copyright and License: see below */

#include <math.h>

static inline u8_interval_t u8_interval_new_void()
{
    return (u8_interval_t){ .low = NAN, .high = NAN };
}

static inline u8_interval_t u8_interval_new_unordered ( double a, double b )
{
    return ( a < b ) ? (u8_interval_t){ .low = a, .high = b } : (u8_interval_t){ .low = b, .high = a };
}

static inline u8_interval_t u8_interval_new ( double low, double high )
{
    assert( low < high );
    return (u8_interval_t){ .low = low, .high = high };
}

static inline u8_interval_t u8_interval_new_intersect ( const u8_interval_t *a, const u8_interval_t *b )
{
    u8_interval_t result;

    if (( (*a).high < (*b).low )||( (*a).low > (*b).high ))
    {
        /* there is no overlap */
        result = (u8_interval_t){ .low = NAN, .high = NAN };
    }
    else
    {
        /* there is an overlap, lower border is the max of the low values, upper border is the min of the high values: */
        result = (u8_interval_t){ .low = fmax( (*a).low, (*b).low ), .high = fmin( (*a).high, (*b).high ) };
    }

    return result;
}

static inline bool u8_interval_is_valid ( const u8_interval_t *this_ )
{
    return (( ! isnan( (*this_).low ) )&&( ! isnan( (*this_).high ) ));
}

static inline double u8_interval_get_low ( const u8_interval_t *this_ )
{
    return (*this_).low;
}

static inline double u8_interval_get_high ( const u8_interval_t *this_ )
{
    return (*this_).high;
}

static inline double u8_interval_get_size ( const u8_interval_t *this_ )
{
    return u8_interval_is_valid( this_ ) ? ( (*this_).high - (*this_).low ) : 0.0;
}


/*
Copyright 2025-2025 Andreas Warnke

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
