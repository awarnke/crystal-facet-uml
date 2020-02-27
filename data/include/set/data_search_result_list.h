/* File: data_search_result_list.h; Copyright and License: see below */

#ifndef DATA_SEARCH_RESULT_LIST_H
#define DATA_SEARCH_RESULT_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a list of search result entries.
 *
 *  Wraps universal_array_list_t, does some type convertions
 *
 *  If universal_array_list_t would be a template, this type-wrapper file would not be needed.
 */

#include "universal_array_list.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief data_search_result_list_t is a universal_array_list_t
 */
typedef universal_array_list_t data_search_result_list_t;

#define DATA_SEARCH_RESULT_LIST_INIT( THIS, ARRAY ) universal_array_list_init( (universal_array_list_t*)(THIS), \
sizeof(ARRAY)/sizeof(data_search_result_t), &(ARRAY), sizeof(data_search_result_t), ((char*)(&((ARRAY)[1])))-((char*)(&((ARRAY)[0]))), \
(void (*)(void *, const void *))data_search_result_copy, NULL );

/*!
 *  \brief destroys the data_search_result_list_t struct and all contained elements
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_list_destroy ( data_search_result_list_t *this_ )
{ universal_array_list_destroy((universal_array_list_t*)this_); }

/*!
 *  \brief prints the data_search_result_list_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_list_trace ( const data_search_result_list_t *this_ )
{ universal_array_list_trace((universal_array_list_t*)this_); }

/*!
 *  \brief checks if data_search_result_list_t is empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool data_search_result_list_is_empty ( const data_search_result_list_t *this_ )
{ return universal_array_list_is_empty((universal_array_list_t*)this_); }

/*!
 *  \brief adds an element to data_search_result_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param element element to be added. Only a valid object can be added, NULL is not allowed.
 *  \return -1 if list is full, 0 on success
 */
static inline int data_search_result_list_add ( data_search_result_list_t *this_, data_search_result_t* element )
{ return universal_array_list_add((universal_array_list_t*)this_,element); }

/*!
 *  \brief returns an element
 *
 *  \param this_ pointer to own object attributes
 *  \param index index to be returned. 0 \<= index \< universal_array_list_get_length(this_)
 *  \return pointer to the element, NULL if index is invalid
 */
static inline data_search_result_list_t *data_search_result_list_get_ptr ( data_search_result_list_t *this_, unsigned int index )
{ return (data_search_result_list_t*)universal_array_list_get_ptr((universal_array_list_t*)this_,index); }

/*!
 *  \brief returns a const element
 *
 *  \param this_ pointer to own object attributes
 *  \param index index to be returned. 0 \<= index \< universal_array_list_get_length(this_)
 *  \return pointer to the element, NULL if index is invalid
 */
static inline void const *data_search_result_list_get_const ( const data_search_result_list_t *this_, unsigned int index )
{ return (data_search_result_list_t const*)universal_array_list_get_const((universal_array_list_t*)this_,index); }

/*!
 *  \brief clears the data_search_result_list_t and all contained elements
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_search_result_list_clear ( data_search_result_list_t *this_ )
{ universal_array_list_clear((universal_array_list_t*)this_); }

/*!
 *  \brief returns the size of the set
 *
 *  \param this_ pointer to own object attributes
 *  \return number of object-ids in the set, the number is less or equal to universal_array_list_MAX_SET_SIZE
 */
static inline uint32_t data_search_result_list_get_length ( const data_search_result_list_t *this_ )
{ return universal_array_list_get_length((universal_array_list_t*)this_); }

#endif  /* DATA_SEARCH_RESULT_LIST_H */


/*
Copyright 2020-2020 Andreas Warnke

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
