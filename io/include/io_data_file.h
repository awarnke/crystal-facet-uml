/* File: io_data_file.h; Copyright and License: see below */

#ifndef IO_DATA_FILE_H
#define IO_DATA_FILE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Opens and closes a file that is either a database or a json file that needs to be imported and exported.
 */

#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8/u8_error_info.h"
#include "u8/u8_error.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct io_data_file_struct {
    data_database_t database;  /*!< a database struct to read/write the database, handle tlocks and transactions */
                               /*!< and notifications to registered listeners */
    ctrl_controller_t controller;  /*!< a controller struct to modify the database */

    utf8stringbuf_t data_file_name;  /*!< the file name of the data file that is either json or sqlite */
    char private_data_file_name_buffer[DATA_DATABASE_MAX_FILEPATH];

    utf8stringbuf_t db_file_name;  /*!< the file name of the sqlite database */
    char private_db_file_name_buffer[DATA_DATABASE_MAX_FILEPATH];

    bool auto_writeback_to_json;  /*!< true if the current database shall automatically be exported to json */
                                  /*!< when flushing (sync to disk) and when closing. */
    bool delete_db_when_finished;  /*!< true if the current database (db_file_name) shall automatically */
                                   /*!< be deleted when closing. */
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
 *  Note that this function opens a database either writeable or in read-only mode if the file is read-only.
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path a relative or absolute file path
 *  \param out_err_info pointer to an error_info_t data struct that may provide an error description when returning
 *  \return U8_ERROR_NO_DB or U8_ERROR_AT_DB if file cannot be opened,
 *          U8_ERROR_LEXiCAL_STRUCTURE or U8_ERROR_PARSER_STRUCTURE if file is no valid json format,
 *          U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_data_file_open_writeable ( io_data_file_t *this_,
                                                       const char* db_file_path,
                                                       u8_error_info_t *out_err_info
                                                     );

/*!
 *  \brief opens a database file in read only mode
 *
 *  Useful for exporting data.
 *
 *  Note that this function opens even a possibly writeable file in read-only mode.
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path a relative or absolute file path
 *  \param out_err_info pointer to an error_info_t data struct that may provide an error description when returning
 *  \return U8_ERROR_NO_DB or U8_ERROR_AT_DB if file cannot be opened,
 *          U8_ERROR_LEXiCAL_STRUCTURE or U8_ERROR_PARSER_STRUCTURE if file is no valid json format,
 *          U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_data_file_open_read_only ( io_data_file_t *this_,
                                                       const char* db_file_path,
                                                       u8_error_info_t *out_err_info
                                                     );

/*!
 *  \brief opens a database file
 *
 *  It is not allowed to open an already opened database.
 *
 *  Note that this function opens a database either writeable or in read-only mode (if requested or the file is read-only)
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path a relative or absolute file path
 *  \param read_only if true, the data file is not modified. Otherwise it depends on the write permissions of the file.
 *  \param out_err_info pointer to an error_info_t data struct that may provide an error description when returning
 *  \return U8_ERROR_NO_DB or U8_ERROR_AT_DB if file cannot be opened,
 *          U8_ERROR_LEXiCAL_STRUCTURE or U8_ERROR_PARSER_STRUCTURE if file is no valid json format,
 *          U8_ERROR_NONE in case of success
 */
u8_error_t io_data_file_open ( io_data_file_t *this_,
                               const char* db_file_path,
                               bool read_only,
                               u8_error_info_t *out_err_info
                             );

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
 *  \brief returns a pointer to the controller
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to the controller
 */
static inline ctrl_controller_t *io_data_file_get_controller_ptr ( io_data_file_t *this_ );

/*!
 *  \brief flushes caches of the current database and optionally exports this to the json file
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_DB in case of an error
 */
u8_error_t io_data_file_sync_to_disk ( io_data_file_t *this_ );

/*!
 *  \brief prints statistics of the current data file to the trace output
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_data_file_trace_stats ( io_data_file_t *this_ );

/*!
 *  \brief returns the database filename
 *
 *  \param this_ pointer to own object attributes
 *  \return NULL if no database is open, the filename otherwise
 */
static inline const char *io_data_file_get_filename_const ( const io_data_file_t *this_ );

/*!
 *  \brief checks if the database file is open
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the database file is open
 */
static inline bool io_data_file_is_open ( io_data_file_t *this_ );

/*!
 *  \brief guess the filetype.
 *
 *  In case the file exists, the type is guessed from scanning the header,
 *  otherwise the file name extension is checked.
 *
 *  \param this_ pointer to own object attributes
 *  \param filename filename of the file to determine the type of, must not be NULL
 *  \param[out] out_json true if filename refers to a json file, false otherwise.
 *  \return U8_ERROR_NONE in case of success; other values if file does not exist or cannot be read
 */
u8_error_t io_data_file_private_guess_db_type ( const io_data_file_t *this_, const char *filename, bool *out_json );

/*!
 *  \brief splits a file path into parent directory, basename and extension
 *
 *  \param this_ pointer to own object attributes
 *  \param path an absolute or relative file path
 *  \param[out] out_parent parent directory including the final path separator.
 *  \param[out] out_basename basename of the file.
 *              If the extension is empty, basename includes the final dot. (e.g. . or .. or end_on_dot. )
 *              The basename cannot be empty - a leading dot does not start the extention.
 *  \param[out] out_parent the file extension excluding the leading dot.
 */
static inline void io_data_file_private_split_path( const io_data_file_t *this_,
                                                    utf8string_t *path,
                                                    utf8stringview_t *out_parent,
                                                    utf8stringview_t *out_basename,
                                                    utf8stringview_t *out_extension
                                                  );

/*!
 *  \brief imports the data from src_file to the currently open database.
 *
 *  \param this_ pointer to own object attributes
 *  \param src_file filename of the source json file to import, must not be NULL
 *  \param out_err_info pointer to an error_info_t data struct that may provide an error description when returning
 *  \return U8_ERROR_NONE in case of success; other values in case of an error
 */
u8_error_t io_data_file_private_import ( io_data_file_t *this_, const char *src_file, u8_error_info_t *out_err_info );

/*!
 *  \brief exports the data from the currently open database to dst_file.
 *
 *  \param this_ pointer to own object attributes
 *  \param dst_file filename of the destination json file to export, must not be NULL
 *  \return U8_ERROR_NONE in case of success; other values in case of an error
 */
u8_error_t io_data_file_private_export ( io_data_file_t *this_, const char *dst_file );

#include "io_data_file.inl"

#endif  /* IO_DATA_FILE_H */


/*
Copyright 2022-2024 Andreas Warnke

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
