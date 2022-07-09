/* File: io_data_file.h; Copyright and License: see below */

#ifndef IO_DATA_FILE_H
#define IO_DATA_FILE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Opens and closes a file that is either a database or a json file that needs to be imported and exported.
 */

#include "storage/data_database.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct io_data_file_struct {
    data_database_t database;

    utf8stringbuf_t data_file_name;
    char private_data_file_name_buffer[DATA_DATABASE_MAX_FILEPATH];

    bool auto_writeback_to_json;
};

typedef struct io_data_file_struct io_data_file_t;

/*!
 *  \brief initializes the io_data_file_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void io_data_file_init ( io_data_file_t *this_ );

/*!
 *  \brief destroys the io_data_file_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void io_data_file_destroy ( io_data_file_t *this_ );

/*!
 *  \brief opens a database file
 *
 *  It is not allowed to open an already opened database.
 *
 *  Note that this function opens a database writeable or in read-only mode if the file in read-only.
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path a relative or absolute file path
 *  \return U8_ERROR_NO_DB or U8_ERROR_AT_DB if file cannot be opened,
 *          U8_ERROR_NONE in case of success
 */
u8_error_t io_data_file_open ( io_data_file_t *this_, const char* db_file_path );

/*!
 *  \brief opens a database file in read only mode
 *
 *  Useful for exporting data.
 *
 *  Note that this function opens even a possibly writeable file in read-only mode.
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path a relative or absolute file path
 *  \return U8_ERROR_NO_DB or U8_ERROR_AT_DB if file cannot be opened,
 *          U8_ERROR_NONE in case of success
 */
u8_error_t io_data_file_open_read_only ( io_data_file_t *this_, const char* db_file_path );

/*!
 *  \brief closes the current database file
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_data_file_close ( io_data_file_t *this_ );

/*!
 *  \brief returns a pointer to the database
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to the database
 */
static inline data_database_t *io_data_file_get_database_ptr ( io_data_file_t *this_ );

/*!
 *  \brief flushes caches of the current database
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_DB in case of an error
 */
u8_error_t io_data_file_flush_caches ( io_data_file_t *this_ );

/*!
 *  \brief returns the database filename
 *
 *  \param this_ pointer to own object attributes
 *  \return NULL if no database is open, the filename otherwise
 */
static inline const char *io_data_file_get_filename_ptr ( io_data_file_t *this_ );

/*!
 *  \brief checks if the database file is open
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the database file is open
 */
static inline bool io_data_file_is_open( io_data_file_t *this_ );

#include "io_data_file.inl"

#endif  /* IO_DATA_FILE_H */


/*
Copyright 2022-2022 Andreas Warnke

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
