/* File: data_database_head_reader.h; Copyright and License: see below */

#ifndef DATA_DATABASE_HEAD_READER_H
#define DATA_DATABASE_HEAD_READER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides read access to the head table of the database.
 */

#include "entity/data_head.h"
#include "storage/data_database.h"
#include "u8stream/universal_memory_output_stream.h"
#include "u8stream/universal_escaping_output_stream.h"
#include "utf8stream/utf8stream_writer.h"
#include <sqlite3.h>
#include <stdbool.h>
#include <glib.h>

/*!
 *  \brief all data attributes needed for reading head structs from the database
 */
struct data_database_head_reader_struct {
    data_database_t *database;  /*!< pointer to external database */

    sqlite3_stmt *statement_head_by_id;
    sqlite3_stmt *statement_head_by_key;
};

typedef struct data_database_head_reader_struct data_database_head_reader_t;

/*!
 *  \brief initializes the data_database_head_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this head data reader and writer uses
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_head_reader_init ( data_database_head_reader_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_head_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_head_reader_destroy ( data_database_head_reader_t *this_ );

/* ================================ HEAD ================================ */

/*!
 *  \brief reads a head value from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the row id of the head value to be read from the database
 *  \param[out] out_head the head value read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if key does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_head_reader_read_value_by_id ( data_database_head_reader_t *this_,
                                                        data_row_t id,
                                                        data_head_t *out_head
                                                      );

/*!
 *  \brief reads a head value from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param key the key of the head value to be read from the database
 *  \param[out] out_head the head value read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if key does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_head_reader_read_value_by_key ( data_database_head_reader_t *this_,
                                                         const char *key,
                                                         data_head_t *out_head
                                                       );

/* ================================ private ================================ */

/*!
 *  \brief initializes the data_database_head_reader_t struct and allows access to the database after the database is opened
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_head_reader_private_open ( data_database_head_reader_t *this_ );

/*!
 *  \brief closes the data_database_head_reader_t struct (prohibits access to the database) before the database is closed
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_head_reader_private_close ( data_database_head_reader_t *this_ );

/*!
 *  \brief binds a single integer to a prepared statement (after reset).
 *
 *  The prepared statement shall have only one variable of type integer.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param id integer to bind to the prepared statement. DATA_ROW_VOID does not work because VOID is mapped to NULL and cannot be selected by the = operator.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_head_reader_private_bind_id_to_statement ( data_database_head_reader_t *this_,
                                                                                  sqlite3_stmt *statement_ptr,
                                                                                  data_row_t id
                                                                                );

/*!
 *  \brief binds a single string to a prepared statement (after reset).
 *
 *  The prepared statement shall have only one variable of type string.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param text char sequence to bind to the prepared statement.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_head_reader_private_bind_text_to_statement ( data_database_head_reader_t *this_,
                                                                                    sqlite3_stmt *statement_ptr,
                                                                                    const char *text
                                                                                  );

#include "data_database_head_reader.inl"

#endif  /* DATA_DATABASE_HEAD_READER_H */


/*
Copyright 2024-2026 Andreas Warnke

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
