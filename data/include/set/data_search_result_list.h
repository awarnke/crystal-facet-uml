/* File: data_search_result_list.h; Copyright and License: see below */

#ifndef DATA_SEARCH_RESULT_LIST_H
#define DATA_SEARCH_RESULT_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a list of search result entries.
 *
 *  Wraps universal_array_list_t, does some type convertions.
 *  If universal_array_list_t would be a template, this type-wrapper file would not be needed.
 */

#include "set/data_search_result.h"
#include "u8list/universal_array_list.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief data_search_result_list_t is a universal_array_list_t
 */
typedef universal_array_list_t data_search_result_list_t;

#define DATA_SEARCH_RESULT_LIST_INIT( THIS, ARRAY ) universal_array_list_init( (universal_array_list_t*)(THIS), \
sizeof(ARRAY)/sizeof(data_search_result_t), &(ARRAY), sizeof(data_search_result_t), ((char*)(&((ARRAY)[1])))-((char*)(&((ARRAY)[0]))), \
(void (*)(void *, const void *))data_search_result_copy, NULL, NULL );

/*!
 *  \brief initializes the data_search_result_list_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param max_elements maximum number of elements that fit into the array
 *  \param elements pointer to array of elements
 */
static inline void data_search_result_list_init ( data_search_result_list_t *this_,
                                                  unsigned int max_elements,
                                                  data_search_result_t (*elements)[]
                                                );

/*!
 *  \brief destroys the data_search_result_list_t struct and all contained elements
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_list_destroy ( data_search_result_list_t *this_ );

/*!
 *  \brief prints the data_search_result_list_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_list_trace ( const data_search_result_list_t *this_ );

/*!
 *  \brief checks if data_search_result_list_t is empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool data_search_result_list_is_empty ( const data_search_result_list_t *this_ );

/*!
 *  \brief adds an element to data_search_result_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param element element to be added. Only a valid object can be added, NULL is not allowed.
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED if list is full, U8_ERROR_NONE on success
 */
static inline u8_error_t data_search_result_list_add ( data_search_result_list_t *this_, const data_search_result_t* element );

/*!
 *  \brief adds all elements of that to data_search_result_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param that list of element to be added.
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED if list is full, U8_ERROR_NONE on success
 */
static inline u8_error_t data_search_result_list_add_all ( data_search_result_list_t *this_, const data_search_result_list_t *that );

/*!
 *  \brief returns an element
 *
 *  \param this_ pointer to own object attributes
 *  \param index index to be returned. 0 \<= index \< universal_array_list_get_length(this_)
 *  \return pointer to the element, NULL if index is invalid
 */
static inline data_search_result_t *data_search_result_list_get_ptr ( data_search_result_list_t *this_, unsigned int index );

/*!
 *  \brief returns a const element
 *
 *  \param this_ pointer to own object attributes
 *  \param index index to be returned. 0 \<= index \< universal_array_list_get_length(this_)
 *  \return pointer to the element, NULL if index is invalid
 */
static inline data_search_result_t const *data_search_result_list_get_const ( const data_search_result_list_t *this_, unsigned int index );

/*!
 *  \brief clears the data_search_result_list_t and all contained elements
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_list_clear ( data_search_result_list_t *this_ );

/*!
 *  \brief returns the size of the set
 *
 *  \param this_ pointer to own object attributes
 *  \return number of object-ids in the set, the number is less or equal to universal_array_list_MAX_SET_SIZE
 */
static inline unsigned int data_search_result_list_get_length ( const data_search_result_list_t *this_ );

#include "set/data_search_result_list.inl"

#endif  /* DATA_SEARCH_RESULT_LIST_H */


/*
Copyright 2020-2026 Andreas Warnke

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
