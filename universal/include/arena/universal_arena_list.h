/* File: universal_arena_list.h; Copyright and License: see below */

#ifndef UNIVERSAL_ARENA_LIST_H
#define UNIVERSAL_ARENA_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements a single linked list for use in a memory region.
 */

#include "arena/universal_arena_list_element.h"
#include "arena/universal_memory_arena.h"

/*!
 *  \brief attributes of the universal_arena_list
 */
struct universal_arena_list_struct {
    universal_arena_list_element_t *begin;  /*!< first element of the single-linked list or NULL if empty */
    universal_memory_arena_t *allocator;  /*!< allocator where append allocates memory from */
};

typedef struct universal_arena_list_struct universal_arena_list_t;

/*!
 *  \brief initializes the universal_arena_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param allocator allocator where universal_arena_list_append allocates memory from
 */
static inline void universal_arena_list_init ( universal_arena_list_t *this_, universal_memory_arena_t *allocator );

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
 *  \param element pointer to the element to be appended. Only a valid object can be added, NULL is not allowed.
 *  \return -1 if allocator is out of memory, 0 on success
 */
static inline int universal_arena_list_append ( universal_arena_list_t *this_, void* element );

/*!
 *  \brief gets the begin of universal_arena_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to the first element or NULL if empty
 */
static inline universal_arena_list_element_t* universal_arena_list_get_begin ( universal_arena_list_t *this_ );

/*!
 *  \brief gets NULL; for usage in an iterator-like way "for it=begin, it!=end, it=it.next"
 *
 *  \param this_ pointer to own object attributes
 *  \return always NULL
 */
static inline universal_arena_list_element_t* universal_arena_list_get_end ( universal_arena_list_t *this_ );

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
