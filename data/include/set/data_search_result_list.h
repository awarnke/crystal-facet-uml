/* File: data_search_result_list.h; Copyright and License: see below */

#ifndef DATA_SEARCH_RESULT_LIST_H
#define DATA_SEARCH_RESULT_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a list of search result entries.
 */

#include "universal_array_list.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct universal_array_list_t data_search_result_list_t;

#define DATA_SEARCH_RESULT_LIST_INIT( THIS, ARRAY ) universal_array_list_init( (universal_array_list_t*)THIS, \
sizeof(ARRAY)/sizeof(data_search_result_t), &ARRAY, sizeof(data_search_result_t), &(ARRAY[1])-&(ARRAY[0]), data_search_result_copy, NULL );

static inline void data_search_result_list_destroy ( data_search_result_list_t *this_ )
{ universal_array_list_destroy((universal_array_list_t*)this_); }

static inline void data_search_result_list_trace ( const data_search_result_list_t *this_ )
{ universal_array_list_trace((universal_array_list_t*)this_); }

static inline bool data_search_result_list_is_empty ( const data_search_result_list_t *this_ )
{ return universal_array_list_is_empty((universal_array_list_t*)this_); }

static inline int data_search_result_list_add ( data_search_result_list_t *this_, data_search_result_t* element )
{ return universal_array_list_add((universal_array_list_t*)this_,element); }

static inline data_search_result_list_t *data_search_result_list_get_ptr ( data_search_result_list_t *this_, unsigned int index )
{ return (data_search_result_list_t*)universal_array_list_get_ptr((universal_array_list_t*)this_,index); }

static inline void const *data_search_result_list_get_const ( const data_search_result_list_t *this_, unsigned int index )
{ return (data_search_result_list_t const*)universal_array_list_get_const((universal_array_list_t*)this_,index); }

static inline void data_search_result_list_clear ( data_search_result_list_t *this_ )
{ universal_array_list_clear((universal_array_list_t*)this_); }

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
