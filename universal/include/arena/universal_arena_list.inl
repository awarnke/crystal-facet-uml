/* File: universal_arena_list.inl; Copyright and License: see below */

#include <assert.h>

static inline void universal_arena_list_init ( universal_arena_list_t *this_,
                                               const void* mem_buf_start,
                                               size_t mem_buf_size )
{
    assert( mem_buf_start != NULL );
    (*this_).mem_buf_start = mem_buf_start;
    (*this_).mem_buf_size = mem_buf_size;
    (*this_).mem_buf_used = 0;
}

static inline void universal_arena_list_destroy ( universal_arena_list_t *this_ )
{
    assert( (*this_).mem_buf_start != NULL );
    (*this_).mem_buf_start = NULL;
}

static inline int universal_arena_list_append ( universal_arena_list_t *this_, const void* element )
{
    int err = 0;
    return err;
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
