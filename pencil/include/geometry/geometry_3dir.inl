/* File: geometry_3dir.inl; Copyright and License: see below */

#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline geometry_3dir_t geometry_3dir_new ( geometry_direction_t first,
                                                  geometry_direction_t second,
                                                  geometry_direction_t third )
{
    const geometry_3dir_t result = { .first = first, .second = second, .third = third };
    return result;
}

static inline bool geometry_3dir_equals ( const geometry_3dir_t *this_, const geometry_3dir_t *that )
{
    const bool result
        = (( (*this_).first == (*that).first )&&( (*this_).second == (*that).second )&&( (*this_).third == (*that).third ));
    return result;
}

static inline geometry_direction_t geometry_3dir_get_first ( const geometry_3dir_t *this_ )
{
    return (*this_).first;
}

static inline geometry_direction_t geometry_3dir_get_second ( const geometry_3dir_t *this_ )
{
    return (*this_).second;
}

static inline geometry_direction_t geometry_3dir_get_third ( const geometry_3dir_t *this_ )
{
    return (*this_).third;
}

static inline bool geometry_3dir_is_first_h ( const geometry_3dir_t *this_ )
{
    return ( (*this_).first == GEOMETRY_DIRECTION_LEFT )||( (*this_).first == GEOMETRY_DIRECTION_RIGHT );
}

static inline bool geometry_3dir_is_first_v ( const geometry_3dir_t *this_ )
{
    return ( (*this_).first == GEOMETRY_DIRECTION_UP )||( (*this_).first == GEOMETRY_DIRECTION_DOWN );
}

static inline bool geometry_3dir_is_second_h ( const geometry_3dir_t *this_ )
{
    return ( (*this_).second == GEOMETRY_DIRECTION_LEFT )||( (*this_).second == GEOMETRY_DIRECTION_RIGHT );
}

static inline bool geometry_3dir_is_second_v ( const geometry_3dir_t *this_ )
{
    return ( (*this_).second == GEOMETRY_DIRECTION_UP )||( (*this_).second == GEOMETRY_DIRECTION_DOWN );
}

static inline bool geometry_3dir_is_third_h ( const geometry_3dir_t *this_ )
{
    return ( (*this_).third == GEOMETRY_DIRECTION_LEFT )||( (*this_).third == GEOMETRY_DIRECTION_RIGHT );
}

static inline bool geometry_3dir_is_third_v ( const geometry_3dir_t *this_ )
{
    return ( (*this_).third == GEOMETRY_DIRECTION_UP )||( (*this_).third == GEOMETRY_DIRECTION_DOWN );
}


/*
Copyright 2021-2025 Andreas Warnke

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
