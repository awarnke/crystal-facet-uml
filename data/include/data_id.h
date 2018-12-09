/* File: data_id.h; Copyright and License: see below */

#ifndef DATA_ID_H
#define DATA_ID_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a record by a table-identifier and a row-identifier
 */

#include "data_table.h"
#include <util/string/utf8stringbuf.h>
#include <util/string/utf8error.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constant to define a void data_id_t
 */
enum data_id_void_enum {
    DATA_ID_VOID_ID = -1,  /*!< invalid id */
};

/*!
 *  \brief constants to define maximum length of textual representation
 */
enum data_id_max_enum {
    DATA_ID_MAX_UTF8STRING_LENGTH = 21,  /*!< maximum ID length is strlen("X-9223372036854775807") which is 21 */
    DATA_ID_MAX_UTF8STRING_SIZE = 22,  /*!< maximum ID size is sizeof("X-9223372036854775807") which is 22 */
};

/*!
 *  \brief attributes of a data_id_t
 */
struct data_id_struct {
    data_table_t table;
    int64_t row_id;
};

typedef struct data_id_struct data_id_t;

/*!
 *  \brief initializes the data_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param table table id of the referenced object
 *  \param row_id row id of the referenced object
 */
static inline void data_id_init ( data_id_t *this_, data_table_t table, int64_t row_id );

/*!
 *  \brief re-initializes the data_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param table table id of the referenced object
 *  \param row_id row id of the referenced object
 */
static inline void data_id_reinit ( data_id_t *this_, data_table_t table, int64_t row_id );

/*!
 *  \brief initializes the data_id_t struct to void
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_init_void ( data_id_t *this_ );

/*!
 *  \brief re-initializes the data_id_t struct to void
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_reinit_void ( data_id_t *this_ );

/*!
 *  \brief destroys the data_id_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_destroy ( data_id_t *this_ );

/*!
 *  \brief gets the table of data_id_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_table_t data_id_get_table ( const data_id_t *this_ );

/*!
 *  \brief gets the row_id of data_id_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline int64_t data_id_get_row_id ( const data_id_t *this_ );

/*!
 *  \brief prints the data_id_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_id_trace ( const data_id_t *this_ );

/*!
 *  \brief checks if two data_id_t objects are equal
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return true if both parameters are valid and equal; false if at least one is invalid or they are unequal.
 */
static inline bool data_id_equals ( const data_id_t *this_, const data_id_t *that );

/*!
 *  \brief checks if data_id has a valid table and a valid row
 *
 *  \param this_ pointer to own object attributes
 */
static inline bool data_id_is_valid ( const data_id_t *this_ );

/*!
 *  \brief checks if data_id_t equals a given row_id
 *
 *  \param this_ pointer to own object attributes
 *  \param table table of other object
 *  \param row_id row_id of other object
 *  \return true if both object-ids are valid and equal; false if at least one is invalid or they are unequal.
 */
static inline bool data_id_equals_id ( const data_id_t *this_, data_table_t table, int64_t row_id );

/*!
 *  \brief prints a textual representation of this id to a utf8stringbuf
 *
 *  \param this_ pointer to own object attributes
 *  \param out_str string to which to append the id
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED if the character could not be appended.
 *          (this may happen if the stringbuf is smaller than DATA_ID_MAX_UTF8STRING_LENGTH)
 */
static inline utf8error_t data_id_to_utf8stringbuf ( const data_id_t *this_, utf8stringbuf_t out_str );

#include "data_id.inl"

#endif  /* DATA_ID_H */


/*
Copyright 2016-2018 Andreas Warnke

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
