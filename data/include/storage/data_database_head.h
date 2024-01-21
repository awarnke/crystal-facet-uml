/* File: data_database_head.h; Copyright and License: see below */

#ifndef DATA_DATABASE_HEAD_H
#define DATA_DATABASE_HEAD_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides read and write access to the head table of the database.
 *
 *  There are no undo/redo operations possible. There are no notifications on changes.
 */

#include "data_head.h"
#include "storage/data_database.h"
#include "u8stream/universal_memory_output_stream.h"
#include "u8stream/universal_escaping_output_stream.h"
#include <sqlite3.h>
#include <stdbool.h>
#include <glib.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_head_struct {
    data_database_t *database;  /*!< pointer to external database */
    char private_sql_buffer[1024+1024];  /* sufficient size to encode a data record including DATA_HEAD_MAX_VALUE_LENGTH */
    universal_memory_output_stream_t plain_sql;  /* an output stream to write to private_sql_buffer as plain utf8 */
    utf8stream_writer_t plain;  /* an utf8 writer to write to plain_sql */
    universal_escaping_output_stream_t escaped_sql;  /* an output stream to write to private_sql_buffer sql encoded */
    utf8stream_writer_t escaped;  /* an utf8 writer to write to escaped_sql */
};

typedef struct data_database_head_struct data_database_head_t;

/*!
 *  \brief initializes the data_database_head_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this head data reader and writer uses
 */
void data_database_head_init ( data_database_head_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_head_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_head_destroy ( data_database_head_t *this_ );

/*!
 *  \brief reads a head value from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id the row id of the head value to be read from the database
 *  \param[out] out_head the head value read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if key does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_head_read_value_by_id ( data_database_head_t *this_, data_row_id_t obj_id, data_head_t *out_head );

/*!
 *  \brief reads a head value from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param key the key of the head value to be read from the database
 *  \param[out] out_head the head value read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if key does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_head_read_value_by_key ( data_database_head_t *this_, const char *key, data_head_t *out_head );

/*!
 *  \brief creates a head value and returns its id
 *
 *  \param this_ pointer to own object attributes
 *  \param head data of the new head value record to be created.
 *              The id should be DATA_ROW_ID_VOID to create a new head value
 *              or a valid id to update an existing head value.
 *  \param[out] out_new_id storage, where the id of the newly created record is stored. NULL if the id shall not be returned.
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_DUPLICATE if duplicate id or duplicate key, otherwise an other error code.
 */
u8_error_t data_database_head_create_value ( data_database_head_t *this_, const data_head_t *head, data_row_id_t* out_new_id );

/*!
 *  \brief deletes a head value and returns its old data
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id of the head record to be deleted.
 *  \param[out] out_old_head storage, where the contents of the deleted, old record is stored. NULL if old data shall not be returned.
 *
 *  \return U8_ERROR_NONE in case of success, error id otherwise.
 */
u8_error_t data_database_head_delete_value ( data_database_head_t *this_, data_row_id_t obj_id, data_head_t *out_old_head );

/*!
 *  \brief updates the head attribute: value
 *
 *  \param this_ pointer to own object attributes
 *  \param head_id id of the head record to be updated
 *  \param new_head_value new head value
 *  \param[out] out_old_head storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t data_database_head_update_value ( data_database_head_t *this_, data_row_id_t head_id, const char* new_head_value, data_head_t *out_old_head );

#endif  /* DATA_DATABASE_HEAD_H */


/*
Copyright 2024-2024 Andreas Warnke

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
