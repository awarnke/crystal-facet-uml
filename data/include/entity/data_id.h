/* File: data_id.h; Copyright and License: see below */

#ifndef DATA_ID_H
#define DATA_ID_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a record by a table-identifier and a row-identifier
 */

#include "entity/data_table.h"
#include "entity/data_row.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8error.h"
#include "utf8stream/utf8stream_writer.h"
#include "u8/u8_error.h"
#include <stdint.h>
#include <stdbool.h>

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
    data_row_t row_id;
};

typedef struct data_id_struct data_id_t;

#define DATA_ID_VOID ((data_id_t){.table=DATA_TABLE_VOID,.row_id=DATA_ROW_VOID})
#define DATA_ID(tab,row) ((data_id_t){.table=tab,.row_id=row})

/*!
 *  \brief initializes the data_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param table table id of the referenced object
 *  \param row_id row id of the referenced object
 */
static inline void data_id_init ( data_id_t *this_, data_table_t table, data_row_t row_id );

/*!
 *  \brief initializes the data_id_t struct
 *
 *  In case of error, data_id_is_valid() is false afterwards.
 *
 *  \param this_ pointer to own object attributes
 *  \param string_to_parse a pointer to a 0-terminated character sequence, not NULL
 */
static inline void data_id_init_by_string ( data_id_t *this_, const char* string_to_parse );

/*!
 *  \brief initializes the data_id_t struct
 *
 *  In case of error, data_id_is_valid() is false afterwards.
 *
 *  \param this_ pointer to own object attributes
 *  \param string_to_parse a pointer to an utf8stringview_t to parse, not NULL
 *  \param[out] out_remainder remaining utf8stringview_t that follow beyond the data_id, not NULL
 */
static inline void data_id_init_by_stringview ( data_id_t *this_,
                                                const utf8stringview_t* string_to_parse,
                                                utf8stringview_t *out_remainder
                                              );

/*!
 *  \brief re-initializes the data_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param table table id of the referenced object
 *  \param row_id row id of the referenced object
 */
static inline void data_id_reinit ( data_id_t *this_, data_table_t table, data_row_t row_id );

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
 *  \brief copies a data_id_t struct to this uninitialized data_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to original object attributes
 */
static inline void data_id_copy ( data_id_t *this_, const data_id_t *that );

/*!
 *  \brief copies a data_id_t struct to this already initialized data_id_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to original object attributes
 */
static inline void data_id_replace ( data_id_t *this_, const data_id_t *that );

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
static inline data_row_t data_id_get_row_id ( const data_id_t *this_ );

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
 *  \return true if both parameters are valid and equal; false if at least one is void or they are unequal.
 */
static inline bool data_id_equals ( const data_id_t *this_, const data_id_t *that );

/*!
 *  \brief checks if two data_id_t objects are equal or both void
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to other object attributes
 *  \return true if both parameters are void or equal; false if exactly one is void or they are unequal.
 */
static inline bool data_id_equals_or_both_void ( const data_id_t *this_, const data_id_t *that );

/*!
 *  \brief checks if data_id has a valid table and a valid row
 *
 *  \param this_ pointer to own object attributes
 *  \return true if valid, false if void
 */
static inline bool data_id_is_valid ( const data_id_t *this_ );

/*!
 *  \brief checks if data_id_t equals a given row_id
 *
 *  \param this_ pointer to own object attributes
 *  \param table table of other object
 *  \param row_id row_id of other object
 *  \return true if both object-ids are valid and equal; false if at least one is void or they are unequal.
 */
static inline bool data_id_equals_id ( const data_id_t *this_, data_table_t table, data_row_t row_id );

/*!
 *  \brief prints a textual representation of this id to an utf8stringbuf
 *
 *  \param this_ pointer to own object attributes
 *  \param out_str string to which to append the id
 *  \return UTF8ERROR_SUCCESS in case of success: All bytes have been copied.
 *          UTF8ERROR_TRUNCATED if the character could not be appended.
 *          (this may happen if the stringbuf is smaller than DATA_ID_MAX_UTF8STRING_LENGTH)
 */
static inline utf8error_t data_id_to_utf8stringbuf ( const data_id_t *this_, utf8stringbuf_t out_str );

/*!
 *  \brief prints a textual representation of this id to an utf8stream_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param out_writer writer to which to append the id
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
static inline u8_error_t data_id_to_utf8_writer ( const data_id_t *this_, utf8stream_writer_t *out_writer );

#include "entity/data_id.inl"

#endif  /* DATA_ID_H */


/*
Copyright 2016-2025 Andreas Warnke

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
