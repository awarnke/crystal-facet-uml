/* File: universal_arena_list.h; Copyright and License: see below */

#ifndef UNIVERSAL_ARENA_LIST_H
#define UNIVERSAL_ARENA_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements a memory region from which memory blocks can be allocated dynamically.
 *
 *  The memory region itself may be statically allocated
 */

#include "arena/universal_arena_list_element.h"

/*!
 *  \brief attributes of the universal_arena_list
 */
struct universal_arena_list_struct {
    const void* mem_buf_start;  /*!< memory buffer start */
    size_t mem_buf_size;  /*!< memory buffer size */
    size_t mem_buf_used;  /*!< part of the memory buffer that is already in use */
};

typedef struct universal_arena_list_struct universal_arena_list_t;

/*!
 *  \brief initializes the universal_arena_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mem_buf_start address of memory buffer
 *  \param mem_buf_size size of the memory buffer
 */
static inline void universal_arena_list_init ( universal_arena_list_t *this_,
                                                 const void* mem_buf_start,
                                                 size_t mem_buf_size
                                               );

/*!
 *  \brief destroys the universal_arena_list_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_arena_list_destroy ( universal_arena_list_t *this_ );

/*!
 *  \brief appends a pointer to an element to the universal_arena_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param element pointer to the element to be added. Only a valid object can be added, NULL is not allowed.
 *  \return -1 if list is full, 0 on success
 */
static inline int universal_arena_list_append ( universal_arena_list_t *this_, const void* element );

#include "arena/universal_arena_list.inl"

#endif  /* UNIVERSAL_ARENA_LIST_H */


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
