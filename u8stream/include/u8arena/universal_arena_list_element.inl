/* File: universal_arena_list_element.inl; Copyright and License: see below */

#include <assert.h>

static inline void universal_arena_list_element_init( universal_arena_list_element_t *this_,
                                                      void* data,
                                                      universal_arena_list_element_t* next )
{
    assert( data != NULL );
    (*this_).data = data;
    (*this_).next = next;
}

static inline void universal_arena_list_element_destroy( universal_arena_list_element_t *this_ )
{
    assert( (*this_).data != NULL );
    (*this_).data = NULL;
    (*this_).next = NULL;
}

static inline void* universal_arena_list_element_get_data( universal_arena_list_element_t *this_ )
{
    assert( (*this_).data != NULL );
    return (*this_).data;
}

static inline universal_arena_list_element_t* universal_arena_list_element_get_next( universal_arena_list_element_t *this_ )
{
    return (*this_).next;
}

static inline void universal_arena_list_element_set_next( universal_arena_list_element_t *this_,
                                                          universal_arena_list_element_t *next )
{
    (*this_).next = next;
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
