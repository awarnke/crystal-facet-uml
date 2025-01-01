/* File: data_head.h; Copyright and License: see below */

#ifndef DATA_HEAD_H
#define DATA_HEAD_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a head value that describes the operation environment
 *         like a file checksum, a path or a process id
 */

#include "entity/data_id.h"
#include "entity/data_row.h"
#include "entity/data_uuid.h"
#include "u8/u8_error.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for max string sizes
 */
enum data_head_max_enum {
    DATA_HEAD_MAX_KEY_SIZE = 48,  /* name */
    DATA_HEAD_MAX_KEY_LENGTH = 47,
    DATA_HEAD_MAX_VALUE_SIZE = 1024,
    DATA_HEAD_MAX_VALUE_LENGTH = 1023,
};

/*!
 *  \brief all data attributes needed for the head value
 *
 *  A data_head_struct has no uuid because these values are not intended to be
 *  stored to a version control system; there is no need for unique identification
 *  of objects created on different branches and merged later.
 */
struct data_head_struct {
    data_row_t id;
    utf8stringbuf_t key;  /*!< name of the head value */
    char private_key_buffer[DATA_HEAD_MAX_KEY_SIZE];
    utf8stringbuf_t value;
    char private_value_buffer[DATA_HEAD_MAX_VALUE_SIZE];
};

typedef struct data_head_struct data_head_t;

/*!
 *  \brief initializes the data_head_t struct with id DATA_ROW_VOID and a fresh uuid
 *
 *  \param this_ pointer to own object attributes
 *  \param head_key key/name of the head value. head_key must not be NULL.
 *  \param head_value value of the head value. head_value must not be NULL.
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long, U8_ERROR_NONE otherwise.
 */
static inline u8_error_t data_head_init_new ( data_head_t *this_,
                                              const char* head_key,
                                              const char* head_value
                                            );

/*!
 *  \brief initializes the data_head_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param head_id id of the head
 *  \param head_key key/name of the head value. head_key must not be NULL.
 *  \param head_value value of the head value. head_value must not be NULL.
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if string parameters too long,
 *          U8_ERROR_VALUE_OUT_OF_RANGE if uuid malformed, U8_ERROR_NONE otherwise.
 */
static inline u8_error_t data_head_init ( data_head_t *this_,
                                          data_row_t head_id,
                                          const char* head_key,
                                          const char* head_value
                                        );

/*!
 *  \brief initializes the data_head_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void data_head_copy ( data_head_t *this_, const data_head_t *original );

/*!
 *  \brief overwrites the data_head_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to object attributes to copy
 */
static inline void data_head_replace ( data_head_t *this_, const data_head_t *that );

/*!
 *  \brief destroys the data_head_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_head_destroy ( data_head_t *this_ );

/*!
 *  \brief gets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline data_row_t data_head_get_row_id ( const data_head_t *this_ );

/*!
 *  \brief sets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \param id new id of this object
 */
static inline void data_head_set_row_id ( data_head_t *this_, data_row_t id );

/*!
 *  \brief gets the attribute key
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_head_get_key_const ( const data_head_t *this_ );

/*!
 *  \brief sets the attribute key
 *
 *  \param this_ pointer to own object attributes
 *  \param key new key of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_head_set_key ( data_head_t *this_, const char *key );

/*!
 *  \brief gets the attribute value
 *
 *  \param this_ pointer to own object attributes
 *  \return requested attribute of this object
 */
static inline const char *data_head_get_value_const ( const data_head_t *this_ );

/*!
 *  \brief checks if the value attribute is empty
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the value is not empty
 */
static inline bool data_head_has_value ( const data_head_t *this_ );

/*!
 *  \brief sets the attribute value
 *
 *  \param this_ pointer to own object attributes
 *  \param value new value of this object
 *  \return U8_ERROR_STRING_BUFFER_EXCEEDED if new string too long
 */
static inline u8_error_t data_head_set_value ( data_head_t *this_, const char *value );

/*!
 *  \brief checks if attribute id is not DATA_ROW_VOID
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the id of the head value is not DATA_ROW_VOID
 */
static inline bool data_head_is_valid ( const data_head_t *this_ );

/*!
 *  \brief prints the data_head_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_head_trace ( const data_head_t *this_ );

#include "entity/data_head.inl"

#endif  /* DATA_HEAD_H */


/*
Copyright 2023-2025 Andreas Warnke

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
