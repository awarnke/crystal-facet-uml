/* File: data_uuid.h; Copyright and License: see below */

#ifndef DATA_UUID_H
#define DATA_UUID_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief creates snd stores a string according to rfc4122, UUID version 4
 */

#include "u8/u8_error.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8error.h"

/*!
 *  \brief constants for string sizes
 */
enum data_uuid_string_enum {
    DATA_UUID_STRING_SIZE = 32+4+1,  /*!< 32 hex digits, 4 minus, 1 terminating zero */
    DATA_UUID_STRING_LENGTH = 32+4,  /*!< 32 hex digits, 4 minus */
};

/*!
 *  \brief attributes of a data_uuid_t
 *
 *  The uuid is stored in string format, e.g. f81d4fae-7dec-11d0-a765-00a0c91e6bf6
 */
struct data_uuid_struct {
    utf8stringbuf_t uuid_string;
    char private_uuid_string_buffer[DATA_UUID_STRING_SIZE];
};

typedef struct data_uuid_struct data_uuid_t;

/*!
 *  \brief initializes the data_uuid_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid_string a uuid string formatted according to rfc4122
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long,
 *          U8_ERROR_VALUE_OUT_OF_RANGE if string invalid, U8_ERROR_NONE otherwise.
 */
static inline u8_error_t data_uuid_init ( data_uuid_t *this_, utf8string_t *uuid_string );

/*!
 *  \brief re-initializes the data_uuid_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid_string a uuid string formatted according to rfc4122
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long,
 *          U8_ERROR_VALUE_OUT_OF_RANGE if string invalid, U8_ERROR_NONE otherwise.
 */
static inline u8_error_t data_uuid_reinit ( data_uuid_t *this_, utf8string_t *uuid_string );

/*!
 *  \brief generates a uuid version 4
 *         based on the systems pseudo random number generator
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_uuid_init_new ( data_uuid_t *this_ );

/*!
 *  \brief initializes the data_uuid_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void data_uuid_copy ( data_uuid_t *this_, const data_uuid_t *original );

/*!
 *  \brief overwrites the data_uuid_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to object attributes to copy
 */
static inline void data_uuid_replace ( data_uuid_t *this_, const data_uuid_t *that );

/*!
 *  \brief destroys the data_uuid_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_uuid_destroy ( data_uuid_t *this_ );

/*!
 *  \brief gets the uuid as string representation
 *
 *  \param this_ pointer to own object attributes
 */
static inline utf8string_t * data_uuid_get_string ( const data_uuid_t *this_ );

/*!
 *  \brief prints the data_uuid_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_uuid_trace ( const data_uuid_t *this_ );

#include "entity/data_uuid.inl"

#endif  /* DATA_UUID_H */


/*
Copyright 2021-2026 Andreas Warnke

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
