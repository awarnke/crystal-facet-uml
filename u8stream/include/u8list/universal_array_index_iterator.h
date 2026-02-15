/* File: universal_array_index_iterator.h; Copyright and License: see below */

#ifndef UNIVERSAL_ARRAY_INDEX_ITERATOR_H
#define UNIVERSAL_ARRAY_INDEX_ITERATOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a list of array indices
 */

#include "u8/u8_error.h"
#include "u8list/universal_array_index_sorter.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the iterator functions
 *
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 */
struct universal_array_index_iterator_struct {
    uint32_t next_index;  /*!< next index to read from the index list */
    const universal_array_index_sorter_t *index_list;  /*!< pointer to the index list */
};

typedef struct universal_array_index_iterator_struct universal_array_index_iterator_t;

/*!
 *  \brief initializes the universal_array_index_iterator_t struct to an empty set
 *
 *  \param this_ pointer to own object attributes
 *  \param index_list pointer to the index list
 */
static inline void universal_array_index_iterator_init ( universal_array_index_iterator_t *this_,
                                                         const universal_array_index_sorter_t *index_list
                                                       );

/*!
 *  \brief resets the universal_array_index_iterator_t struct to the start of the list
 *  \param this_ pointer to own object attributes
 */
static inline void universal_array_index_iterator_reset ( universal_array_index_iterator_t *this_ );

/*!
 *  \brief destroys the universal_array_index_iterator_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_array_index_iterator_destroy ( universal_array_index_iterator_t *this_ );

/*!
 *  \brief checks if a next element exists in the iterator - does not modify the iterator state
 *
 *  \param this_ pointer to own object attributes
 *  \return true if another index exists, false if not or in case of an error
 */
static inline bool universal_array_index_iterator_has_next ( const universal_array_index_iterator_t *this_ );

/*!
 *  \brief reads the next index from the index list.
 *
 *  \param this_ pointer to own object attributes
 *  \return the next index.
 */
static inline uint32_t universal_array_index_iterator_next ( universal_array_index_iterator_t *this_ );

#include "u8list/universal_array_index_iterator.inl"

#endif  /* UNIVERSAL_ARRAY_INDEX_ITERATOR_H */


/*
Copyright 2026-2026 Andreas Warnke

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
