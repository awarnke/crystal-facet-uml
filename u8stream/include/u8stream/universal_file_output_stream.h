/* File: universal_file_output_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_FILE_OUTPUT_STREAM_H
#define UNIVERSAL_FILE_OUTPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements an universal_output_stream_if and forwards data to a FILE
 */

#include "u8stream/universal_output_stream.h"
#include "u8/u8_error.h"
#include <stdio.h>

/*!
 *  \brief attributes of the universal_file_output_stream
 */
struct universal_file_output_stream_struct {
    universal_output_stream_t output_stream;  /*!< instance of implemented interface \c universal_output_stream_t */
    FILE *output;  /*!< output file, is NULL if file not open for writing */
};

typedef struct universal_file_output_stream_struct universal_file_output_stream_t;

/*!
 *  \brief initializes the universal_file_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 */
void universal_file_output_stream_init ( universal_file_output_stream_t *this_ );

/*!
 *  \brief destroys the universal_file_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_file_output_stream_destroy ( universal_file_output_stream_t *this_ );

/*!
 *  \brief opens a file
 *
 *  It is recommended to use this function even if it is unclear if this program can write to path to ensure that
 *  time-of-check is time-of-use (TOCTOU).
 *
 *  \param this_ pointer to own object attributes
 *  \param path file path identifying the file to open for writing
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_WRONG_STATE if file is already open, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_file_output_stream_open ( universal_file_output_stream_t *this_, const char *path );

/*!
 *  \brief writes a buffer (e.g. a stringview) to a file
 *
 *  Do not write to a file if open was not successful (otherwise an error message is logged).
 *
 *  \param this_ pointer to own object attributes
 *  \param start buffer to write, not 0-terminated
 *  \param length length of the buffer to write
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_WRONG_STATE if file is not open, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_file_output_stream_write ( universal_file_output_stream_t *this_, const void *start, size_t length );

/*!
 *  \brief flushes buffers
 *
 *  Do not flush a file if open was not successful (otherwise an error message is logged).
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_WRONG_STATE if file is not open, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_file_output_stream_flush ( universal_file_output_stream_t *this_ );

/*!
 *  \brief closes the universal_file_output_stream_t
 *
 *  Do not close a file if open was not successful (otherwise an error message is logged).
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_WRONG_STATE if file not open, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_file_output_stream_close ( universal_file_output_stream_t *this_ );

/*!
 *  \brief gets the output stream interface of this universal_file_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
 */
universal_output_stream_t* universal_file_output_stream_get_output_stream ( universal_file_output_stream_t *this_ );

#endif  /* UNIVERSAL_FILE_OUTPUT_STREAM_H */


/*
Copyright 2020-2025 Andreas Warnke

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
