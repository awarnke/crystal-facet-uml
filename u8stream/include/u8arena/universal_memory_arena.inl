/* File: universal_memory_arena.inl; Copyright and License: see below */

#include <assert.h>

static inline void universal_memory_arena_init ( universal_memory_arena_t *this_,
                                                 void* mem_buf_start,
                                                 size_t mem_buf_size )
{
    assert( mem_buf_start != NULL );
    (*this_).int_buf_start = mem_buf_start;
    (*this_).int_buf_size = mem_buf_size / sizeof(int);
    (*this_).int_buf_used = 0;
}

static inline void universal_memory_arena_destroy ( universal_memory_arena_t *this_ )
{
    assert( (*this_).int_buf_start != NULL );
    (*this_).int_buf_start = NULL;
}

static inline void universal_memory_arena_reset ( universal_memory_arena_t *this_ )
{
    (*this_).int_buf_used = 0;
}

static inline u8_error_t universal_memory_arena_get_block ( universal_memory_arena_t *this_,
                                                            size_t block_size,
                                                            void **out_block )
{
    assert( out_block != NULL );
    u8_error_t err = U8_ERROR_NONE;

    const size_t requested_ints = ( block_size + sizeof(int) - 1 ) / sizeof(int);

    if (( (*this_).int_buf_used + requested_ints ) > (*this_).int_buf_size )
    {
        err = U8_ERROR_ARRAY_BUFFER_EXCEEDED;
        *out_block = NULL;
    }
    else
    {
        err = U8_ERROR_NONE;
        *out_block = &( (*this_).int_buf_start[(*this_).int_buf_used] );
        (*this_).int_buf_used += requested_ints;
    }

    return err;
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
