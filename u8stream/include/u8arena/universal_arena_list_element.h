/* File: universal_arena_list_element.h; Copyright and License: see below */

#ifndef UNIVERSAL_ARENA_LIST_ELEMENT_H
#define UNIVERSAL_ARENA_LIST_ELEMENT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief is an element of a single-linked list.
 */

/*!
 *  \brief attributes of the universal_arena_list_element
 */
struct universal_arena_list_element_struct {
    void* data;  /*!< pointer to the data of the element */
    struct universal_arena_list_element_struct* next;  /*!< next element in list or NULL */
};

typedef struct universal_arena_list_element_struct universal_arena_list_element_t;

/*!
 *  \brief initializes the universal_arena_list_element_t
 *
 *  \param this_ pointer to own object attributes
 *  \param data pointer to the data of the element
 *  \param next next element in list or NULL
 */
static inline void universal_arena_list_element_init ( universal_arena_list_element_t *this_,
                                                       void* data,
                                                       universal_arena_list_element_t* next
                                                     );

/*!
 *  \brief destroys the universal_arena_list_element_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_arena_list_element_destroy ( universal_arena_list_element_t *this_ );

/*!
 *  \brief gets the data of universal_arena_list_element_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data
 */
static inline void* universal_arena_list_element_get_data ( universal_arena_list_element_t *this_ );

/*!
 *  \brief gets the next element after this universal_arena_list_element_t
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to next
 */
static inline universal_arena_list_element_t* universal_arena_list_element_get_next ( universal_arena_list_element_t *this_ );

/*!
 *  \brief sets the next element after this universal_arena_list_element_t
 *
 *  \param this_ pointer to own object attributes
 *  \param next next element in list or NULL
 */
static inline void universal_arena_list_element_set_next ( universal_arena_list_element_t *this_,
                                                           universal_arena_list_element_t *next
                                                         );

#include "u8arena/universal_arena_list_element.inl"

#endif  /* UNIVERSAL_ARENA_LIST_ELEMENT_H */


/*
Copyright 2021-2024 Andreas Warnke

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
