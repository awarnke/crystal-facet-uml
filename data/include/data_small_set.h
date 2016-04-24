/* File: data_small_set.h; Copyright and License: see below */

#ifndef DATA_SMALL_SET_H
#define DATA_SMALL_SET_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a set of records. Maximum size is limited.
 */

#include "data_table.h"
#include "data_id.h"
#include "data_error.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for max set size
 */
enum data_small_set_max_enum {
    DATA_SMALL_SET_MAX_SET_SIZE = 128,  /*!< maximum number of row ids in this set */
};

/*!
 *  \brief attributes of a data_small_set_t
 */
struct data_small_set_struct {
    uint32_t count;
    data_id_t id_set[DATA_SMALL_SET_MAX_SET_SIZE];
};

typedef struct data_small_set_struct data_small_set_t;

/*!
 *  \brief initializes the data_small_set_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_small_set_init ( data_small_set_t *this_ );

/*!
 *  \brief destroys the data_small_set_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_small_set_destroy ( data_small_set_t *this_ );

/*!
 *  \brief prints the data_small_set_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_small_set_trace ( data_small_set_t *this_ );

/*!
 *  \brief checks if data_small_set_t is empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool data_small_set_is_empty ( data_small_set_t *this_ );

/*!
 *  \brief checks if data_small_set_t contains a row id
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id to be seached
 */
static inline bool data_small_set_is_contained ( data_small_set_t *this_, data_id_t row_id );

/*!
 *  \brief checks if data_small_set_t contains a row id
 *
 *  \param this_ pointer to own object attributes
 *  \param table table-id to be seached
 *  \param id row-id to be seached
 */
static inline bool data_small_set_is_row_contained ( data_small_set_t *this_, data_table_t table, int64_t id );

/*!
 *  \brief adds a row_id to data_small_set_t
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id to be added. Only valid rows can be added.
 *  \return DATA_ERROR_DUPLICATE_ID if id is already contained, DATA_ERROR_ARRAY_BUFFER_EXCEEDED if DATA_SMALL_SET_MAX_SET_SIZE exceeded,
 *          DATA_ERROR_INVALID_REQUEST if row id is invalid, DATA_ERROR_NONE else.
 */
static inline data_error_t data_small_set_add_row ( data_small_set_t *this_, data_id_t row_id );

/*!
 *  \brief deletes a row_id from data_small_set_t
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id to be deleted
 *  \return DATA_ERROR_INVALID_REQUEST if id is not contained, DATA_ERROR_NONE else.
 */
static inline data_error_t data_small_set_delete_row ( data_small_set_t *this_, data_id_t row_id );

/*!
 *  \brief toggles (adds if not contained, deletes if contained) a row_id to/from data_small_set_t
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id id to be toggled. Only valid rows can be toggled.
 *  \return DATA_ERROR_ARRAY_BUFFER_EXCEEDED if DATA_SMALL_SET_MAX_SET_SIZE exceeded,
 *          DATA_ERROR_INVALID_REQUEST if row id is invalid, DATA_ERROR_NONE else.
 */
static inline data_error_t data_small_set_toggle_row ( data_small_set_t *this_, data_id_t row_id );


#include "data_small_set.inl"

#endif  /* DATA_SMALL_SET_H */


/*
Copyright 2016-2016 Andreas Warnke

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
