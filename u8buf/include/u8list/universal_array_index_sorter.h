/* File: universal_array_index_sorter.h; Copyright and License: see below */

#ifndef UNIVERSAL_ARRAY_INDEX_SORTER_H
#define UNIVERSAL_ARRAY_INDEX_SORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Sorts array indexes by a sorting criteria (weight) ascending
 */

#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of universal_array_index_sorter_t
 */
enum universal_array_index_sorter_max_enum {
    UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE = 2048,  /*!< maximum number of indices to sort */
};

/*!
 *  \brief attributes of the universal_array_index_sorter_t
 */
struct universal_array_index_sorter_struct {
    uint32_t entries_count;  /*!< number of all contained array indices */
    uint32_t entries[UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE];  /*!< all contained array indices */
    int64_t weights[UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE];  /*!< weights of entries */
};

typedef struct universal_array_index_sorter_struct universal_array_index_sorter_t;

/*!
 *  \brief initializes the universal_array_index_sorter_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_array_index_sorter_init( universal_array_index_sorter_t *this_ );

/*!
 *  \brief re-initializes the universal_array_index_sorter_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_array_index_sorter_reinit( universal_array_index_sorter_t *this_ );

/*!
 *  \brief destroys the universal_array_index_sorter_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_array_index_sorter_destroy( universal_array_index_sorter_t *this_ );

/*!
 *  \brief adds an entry to the index-list
 *
 *  \param this_ pointer to own object attributes
 *  \param array_index index of data within an external, unknown array
 *  \param weight weight of the array-entry by which to sort
 *  \return -1 in case the list is full, 0 in case of success
 */
static inline int universal_array_index_sorter_insert( universal_array_index_sorter_t *this_, uint32_t array_index, int64_t weight );

/*!
 *  \brief gets the current list length
 *
 *  \param this_ pointer to own object attributes
 *  \return number of entries in the sorted index array
 */
static inline uint32_t universal_array_index_sorter_get_count( const universal_array_index_sorter_t *this_ );

/*!
 *  \brief adds an entry to the index-list
 *
 *  \param this_ pointer to own object attributes
 *  \param sort_index index of the sorted index-array
 *  \return array index in the external, unsorted data array
 */
static inline uint32_t universal_array_index_sorter_get_array_index( const universal_array_index_sorter_t *this_, uint32_t sort_index );

#include "u8list/universal_array_index_sorter.inl"

#endif  /* UNIVERSAL_ARRAY_INDEX_SORTER_H */


/*
Copyright 2017-2023 Andreas Warnke

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
