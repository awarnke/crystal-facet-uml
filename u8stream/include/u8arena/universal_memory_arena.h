/* File: universal_memory_arena.h; Copyright and License: see below */

#ifndef UNIVERSAL_MEMORY_ARENA_H
#define UNIVERSAL_MEMORY_ARENA_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements a memory region from which memory blocks can be allocated dynamically.
 *
 *  The memory region itself may be statically allocated.
 *  These functions are not multi-thread safe.
 */

#include "u8/u8_error.h"

/*!
 *  \brief attributes of the universal_memory_arena
 */
struct universal_memory_arena_struct {
    int* int_buf_start;  /*!< memory buffer start */
    size_t int_buf_size;  /*!< memory buffer size in sizeof(int) */
    size_t int_buf_used;  /*!< part of the memory buffer that is already in use, unit: sizeof(int) */
};

typedef struct universal_memory_arena_struct universal_memory_arena_t;

/*!
 *  \brief initializes the universal_memory_arena_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mem_buf_start address of memory buffer.
 *                       To optimize performance, this shall be int-aligned.
 *  \param mem_buf_size size of the memory buffer
 */
static inline void universal_memory_arena_init ( universal_memory_arena_t *this_,
                                                 void* mem_buf_start,
                                                 size_t mem_buf_size
                                               );

/*!
 *  \brief destroys the universal_memory_arena_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_memory_arena_destroy ( universal_memory_arena_t *this_ );

/*!
 *  \brief resets the memory buffer, all memory is treated as unused
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_memory_arena_reset ( universal_memory_arena_t *this_ );

/*!
 *  \brief fetches a memory block from a memory region
 *
 *  \param this_ pointer to own object attributes
 *  \param block_size size of the buffer to allocate
 *  \param out_block start address of the allocated block, NULL if none available
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED in case there is not sufficient memory available, U8_ERROR_NONE in case of success
 */
static inline u8_error_t universal_memory_arena_get_block ( universal_memory_arena_t *this_,
                                                            size_t block_size,
                                                            void **out_block
                                                          );

#include "u8arena/universal_memory_arena.inl"

#endif  /* UNIVERSAL_MEMORY_ARENA_H */


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
