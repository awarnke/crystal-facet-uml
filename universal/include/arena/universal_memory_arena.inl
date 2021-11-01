/* File: universal_memory_arena.inl; Copyright and License: see below */

#include <assert.h>

static inline void universal_memory_arena_init ( universal_memory_arena_t *this_,
                                                 const void* mem_buf_start,
                                                 size_t mem_buf_size )
{
    assert( mem_buf_start != NULL );
    (*this_).mem_buf_start = mem_buf_start;
    (*this_).mem_buf_size = mem_buf_size;
    (*this_).mem_buf_used = 0;
}

static inline void universal_memory_arena_destroy ( universal_memory_arena_t *this_ )
{
    assert( mem_buf_start != NULL );
    (*this_).mem_buf_start = NULL;
}

static inline void universal_memory_arena_reset ( universal_memory_arena_t *this_ )
{
    (*this_).mem_buf_used = 0;
}

static inline int universal_memory_arena_get_block ( universal_memory_arena_t *this_,
                                                     size_t block_size,
                                                     void *out_block )
{
    assert( mem_buf_start != NULL );
    int err = 0;
    static const unsigned int align = sizeof(void*);
    static const unsigned int align_mask = -align;  /* assuming a complement of 2 presentation */
    void *const block_start = ((*this_).mem_buf_start + (*this_).mem_buf_used + align - 1) & align_mask;
    void *const next_start = block_start + block_size;
    if (( (*this_).mem_buf_start + (*this_).mem_buf_size )<( next_start ))
    {
        err = -1;
        *out_block = NULL;
    }
    else
    {
        err = 0;
        *out_block = block_start;
        (*this_).mem_buf_used = next_start - (*this_).mem_buf_start;
    }
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
