/* File: data_enum_iterator.h; Copyright and License: see below */

#ifndef DATA_ENUM_ITERATOR_H
#define DATA_ENUM_ITERATOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a set of data_enum_def_t
 */

#include "entity/data_enum_def.h"
#include <stdint.h>
#include <stdbool.h>
#include "u8/u8_error.h"

/*!
 *  \brief attributes of a data_enum_iterator_t
 */
struct data_enum_iterator_struct {
    const data_enum_def_t* list;
    unsigned int size;
    unsigned int next;
};

typedef struct data_enum_iterator_struct data_enum_iterator_t;

/*!
 *  \brief initializes the data_enum_iterator_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param list table of data_enum_def_t
 *  \param size number of eleents in the list
 *  \param next id of next row to be returned
 */
static inline void data_enum_iterator_init ( data_enum_iterator_t *this_,
                                             const data_enum_def_t* list,
                                             unsigned int size,
                                             unsigned int next
                                           );

/*!
 *  \brief destroys the data_enum_iterator_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_enum_iterator_destroy ( data_enum_iterator_t *this_ );

/*!
 *  \brief checks if a next element exists in the iterator - does not modify the iterator state
 *
 *  \param this_ pointer to own object attributes
 *  \return true if another data_enum_def_t exists, false if not
 */
static inline bool data_enum_iterator_has_next ( const data_enum_iterator_t *this_ );

/*!
 *  \brief reads the next data_enum_def (and internally advances to the data_enum_def after).
 *
 *  \param this_ pointer to own object attributes
 *  \param out_data_enum the next data_enum_def_t (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_INVALID_REQUEST if there is no next element in the iterator.
 */
static inline u8_error_t data_enum_iterator_next ( data_enum_iterator_t *this_, data_enum_def_t *out_data_enum );

/*!
 *  \brief prints the data_enum_iterator_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_enum_iterator_trace ( const data_enum_iterator_t *this_ );

#include "entity/data_enum_iterator.inl"

#endif  /* DATA_ENUM_ITERATOR_H */


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
