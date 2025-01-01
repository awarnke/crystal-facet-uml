/* File: universal_array_list.h; Copyright and License: see below */

#ifndef UNIVERSAL_ARRAY_LIST_H
#define UNIVERSAL_ARRAY_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines basic list functions.
 *
 *  This is an abstract type, to be used by specialized types like data_search_result_list_t.
 */

#include "u8/u8_error.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*!
 *  \brief attributes of a universal_array_list_t
 *
 */
struct universal_array_list_struct {
    /* usage-relevant */
    unsigned int length;
    void *elements;

    /* internal */
    unsigned int max_elements;
    size_t element_size;
    ptrdiff_t step_size; /*!< bytes from one array element to the next */

    /* callbacks to elements */
    void (*copy_ctor)(void* to_instance, const void* from_instance); /* the copy constructor of an element, used by append */
    bool (*equal)(const void* instance_1, const void* instance_2); /* the compare operator, used by get_index_of */
    void (*dtor)(void* instance); /* the destructor of an element, used by clear */
};

typedef struct universal_array_list_struct universal_array_list_t;

/*!
 *  \brief initializes the universal_array_list_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param max_elements maximum number of elements that fit into the array
 *  \param elements pointer to array of elements
 *  \param element_size size in bytes of a single element (the base type, without padding for alignment)
 *  \param step_size number of bytes from one array antry to the next (this may be equal to element_size or bigger in case of padding)
 *  \param copy_ctor a function that copies an element, NULL if memcpy shall be used.
 *  \param equal a compare function, NULL if memcmp shall be used.
 *  \param dtor a function that destroys an element, NULL if no cleanup necessary.
 */
static inline void universal_array_list_init ( universal_array_list_t *this_,
                                               unsigned int max_elements,
                                               void *elements,
                                               size_t element_size,
                                               ptrdiff_t step_size,
                                               void (*copy_ctor)(void* to_instance, const void* from_instance),
                                               bool (*equal)(const void* instance_1, const void* instance_2),
                                               void (*dtor)(void* instance)
                                             );

/*!
 *  \brief destroys the universal_array_list_t struct and all contained elements
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_array_list_destroy ( universal_array_list_t *this_ );

/*!
 *  \brief prints the universal_array_list_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_array_list_trace ( const universal_array_list_t *this_ );

/*!
 *  \brief checks if universal_array_list_t is empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool universal_array_list_is_empty ( const universal_array_list_t *this_ );

/*!
 *  \brief appends an element to universal_array_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param element element to be added. Only a valid object can be added, NULL is not allowed.
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED if list is full, U8_ERROR_NONE on success
 */
static inline u8_error_t universal_array_list_append ( universal_array_list_t *this_, const void* element );

/*!
 *  \brief appends all elements of that to universal_array_list_t
 *
 *  \param this_ pointer to own object attributes
 *  \param that list of elements to be added.
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED if list is full, U8_ERROR_NONE on success
 */
static inline u8_error_t universal_array_list_append_all ( universal_array_list_t *this_, const universal_array_list_t *that );

/*!
 *  \brief returns an element
 *
 *  \param this_ pointer to own object attributes
 *  \param index index to be returned. 0 \<= index \< universal_array_list_get_length(this_)
 *  \return pointer to the element, NULL if index is invalid
 */
static inline void *universal_array_list_get_ptr ( universal_array_list_t *this_, unsigned int index );

/*!
 *  \brief returns a const element
 *
 *  \param this_ pointer to own object attributes
 *  \param index index to be returned. 0 \<= index \< universal_array_list_get_length(this_)
 *  \return pointer to the element, NULL if index is invalid
 */
static inline void const *universal_array_list_get_const ( const universal_array_list_t *this_, unsigned int index );

/*!
 *  \brief returns the index of the given element
 *
 *  \param this_ pointer to own object attributes
 *  \param element element to be searched. Only a valid object can be searched, NULL is not allowed.
 *  \return index of the element, -1 if element is not in the list
 */
static inline int universal_array_list_get_index_of ( const universal_array_list_t *this_, const void* element );

/*!
 *  \brief clears the universal_array_list_t and all contained elements
 *
 *  \param this_ pointer to own object attributes
 */
static inline void universal_array_list_clear ( universal_array_list_t *this_ );

/*!
 *  \brief returns the size of the set
 *
 *  \param this_ pointer to own object attributes
 *  \return number of object-ids in the set, the number is less or equal to universal_array_list_MAX_SET_SIZE
 */
static inline unsigned int universal_array_list_get_length ( const universal_array_list_t *this_ );

#include "u8list/universal_array_list.inl"

#endif  /* UNIVERSAL_ARRAY_LIST_H */


/*
Copyright 2020-2025 Andreas Warnke

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
