/* File: data_small_set.h; Copyright and License: see below */

#ifndef DATA_SMALL_SET_H
#define DATA_SMALL_SET_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a set of object-ids. Maximum size is limited.
 *
 *  The size of this data type is appropriate for usage on stack
 */

#include "data_table.h"
#include "data_id.h"
#include "data_row_id.h"
#include "u8/u8_error.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for max set size
 */
enum data_small_set_max_enum {
    DATA_SMALL_SET_MAX_SET_SIZE = 128,  /*!< maximum number of object ids in this set */
};

/*!
 *  \brief attributes of a data_small_set_t
 *
 *  The size of data_small_set_t is suitable for passing this object as value-parameter to functions
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
 *  \brief reinitializes the data_small_set_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_small_set_reinit ( data_small_set_t *this_ );

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
static inline void data_small_set_trace ( const data_small_set_t *this_ );

/*!
 *  \brief checks if data_small_set_t is empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool data_small_set_is_empty ( const data_small_set_t *this_ );

/*!
 *  \brief checks if data_small_set_t contains an object id
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id to be seached
 */
static inline bool data_small_set_contains ( const data_small_set_t *this_, data_id_t obj_id );

/*!
 *  \brief checks if data_small_set_t contains an object id
 *
 *  \param this_ pointer to own object attributes
 *  \param table table-id to be seached
 *  \param row_id row-id to be seached
 */
static inline bool data_small_set_contains_row_id ( const data_small_set_t *this_, data_table_t table, data_row_id_t row_id );

/*!
 *  \brief adds a obj_id to data_small_set_t
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id to be added. Only valid object ids can be added.
 *  \return U8_ERROR_DUPLICATE_ID if id is already contained, U8_ERROR_ARRAY_BUFFER_EXCEEDED if DATA_SMALL_SET_MAX_SET_SIZE exceeded,
 *          U8_ERROR_INVALID_REQUEST if object id is invalid, U8_ERROR_NONE else.
 */
static inline u8_error_t data_small_set_add_obj ( data_small_set_t *this_, data_id_t obj_id );

/*!
 *  \brief adds a obj_id to data_small_set_t
 *
 *  \param this_ pointer to own object attributes
 *  \param table table-id to be seached
 *  \param row_id row-id to be seached
 *  \return U8_ERROR_DUPLICATE_ID if id is already contained, U8_ERROR_ARRAY_BUFFER_EXCEEDED if DATA_SMALL_SET_MAX_SET_SIZE exceeded,
 *          U8_ERROR_INVALID_REQUEST if object id is invalid, U8_ERROR_NONE else.
 */
static inline u8_error_t data_small_set_add_row_id ( data_small_set_t *this_, data_table_t table, data_row_id_t row_id );

/*!
 *  \brief deletes an obj_id from data_small_set_t
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id to be deleted
 *  \return U8_ERROR_INVALID_REQUEST if id is not contained, U8_ERROR_NONE else.
 */
static inline u8_error_t data_small_set_delete_obj ( data_small_set_t *this_, data_id_t obj_id );

/*!
 *  \brief toggles (adds if not contained, deletes if contained) an obj_id to/from data_small_set_t
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id to be toggled. Only valid object ids can be toggled.
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED if DATA_SMALL_SET_MAX_SET_SIZE exceeded,
 *          U8_ERROR_INVALID_REQUEST if the object id is invalid, U8_ERROR_NONE else.
 */
static inline u8_error_t data_small_set_toggle_obj ( data_small_set_t *this_, data_id_t obj_id );

/*!
 *  \brief clears the data_small_set_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_small_set_clear ( data_small_set_t *this_ );

/*!
 *  \brief returns the size of the set
 *
 *  \param this_ pointer to own object attributes
 *  \return number of object-ids in the set, the number is less or equal to DATA_SMALL_SET_MAX_SET_SIZE
 */
static inline uint32_t data_small_set_get_count ( const data_small_set_t *this_ );

/*!
 *  \brief returns an object-id of the set
 *
 *  \param this_ pointer to own object attributes
 *  \param index index to be returned. index shall be greater or equal to zero and less than data_small_get_count().
 *  \return object id. data_id_is_valid() returns false on the result if index is out of range.
 */
static inline data_id_t data_small_set_get_id ( const data_small_set_t *this_, uint32_t index );

#include "data_small_set.inl"

#endif  /* DATA_SMALL_SET_H */


/*
Copyright 2016-2021 Andreas Warnke

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
