/* File: universal_arena_list.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <stdint.h>
#include <assert.h>

static inline void universal_arena_list_init( universal_arena_list_t *this_,
                                              universal_memory_arena_t *allocator )
{
    assert( allocator != NULL );
    (*this_).begin = NULL;
    (*this_).allocator = allocator;
}

static inline void universal_arena_list_destroy( universal_arena_list_t *this_ )
{
    assert( (*this_).allocator != NULL );
    (*this_).allocator = NULL;
    (*this_).begin = NULL;
}

static inline u8_error_t universal_arena_list_append( universal_arena_list_t *this_, void* element )
{
    assert( (*this_).allocator != NULL );
    u8_error_t err = U8_ERROR_NONE;

    universal_arena_list_element_t *new_ele;
    err = universal_memory_arena_get_block( (*this_).allocator, sizeof(universal_arena_list_element_t), (void**)&new_ele );
    if ( err == U8_ERROR_NONE )
    {
        universal_arena_list_element_init( new_ele, element, NULL );

        if ( (*this_).begin == NULL )
        {
            (*this_).begin = new_ele;
        }
        else
        {
            universal_arena_list_element_t *find_last = (*this_).begin;
            for ( bool finished = false; ( ! finished ); )
            {
                if ( universal_arena_list_element_get_next( find_last ) == NULL )
                {
                    universal_arena_list_element_set_next( find_last, new_ele );
                    finished = true;
                }
                else
                {
                    find_last = universal_arena_list_element_get_next( find_last );
                }
            }
        }
    }

    return err;
}

static inline universal_arena_list_element_t* universal_arena_list_get_begin( universal_arena_list_t *this_ )
{
    return (*this_).begin;
}

static inline universal_arena_list_element_t* universal_arena_list_get_end( universal_arena_list_t *this_ )
{
    return NULL;
}


/*
Copyright 2021-2026 Andreas Warnke

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
