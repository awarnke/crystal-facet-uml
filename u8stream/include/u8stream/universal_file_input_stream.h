/* File: universal_file_input_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_FILE_INPUT_STREAM_H
#define UNIVERSAL_FILE_INPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements an universal_input_stream_if and forwards data from a FILE
 */

#include "u8stream/universal_input_stream.h"
#include <stdio.h>

/*!
 *  \brief attributes of the universal_file_input_stream
 */
struct universal_file_input_stream_struct {
    universal_input_stream_t input_stream;  /*!< instance of implemented interface \c universal_input_stream_t */
    FILE *input;  /*!< input file */
};

typedef struct universal_file_input_stream_struct universal_file_input_stream_t;

/*!
 *  \brief initializes the universal_file_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 */
void universal_file_input_stream_init( universal_file_input_stream_t *this_ );

/*!
 *  \brief destroys the universal_file_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
u8_error_t universal_file_input_stream_destroy( universal_file_input_stream_t *this_ );

/*!
 *  \brief resets the read position to 0
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
u8_error_t universal_file_input_stream_reset ( universal_file_input_stream_t *this_ );

/*!
 *  \brief opens a file
 *
 *  \param this_ pointer to own object attributes
 *  \param path file path identifying the file to open for reading
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
u8_error_t universal_file_input_stream_open ( universal_file_input_stream_t *this_, const char *path );

/*!
 *  \brief reads a buffer from a file
 *
 *  \param this_ pointer to own object attributes
 *  \param out_buffer buffer to write read bytes
 *  \param max_size length of the buffer to write
 *  \param out_length number of bytes read
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
u8_error_t universal_file_input_stream_read ( universal_file_input_stream_t *this_, void *out_buffer, size_t max_size, size_t *out_length );

/*!
 *  \brief closes the universal_file_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
u8_error_t universal_file_input_stream_close( universal_file_input_stream_t *this_ );

/*!
 *  \brief gets the input stream interface of this universal_file_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
 */
universal_input_stream_t* universal_file_input_stream_get_input_stream( universal_file_input_stream_t *this_ );

#endif  /* UNIVERSAL_FILE_INPUT_STREAM_H */


/*
Copyright 2021-2022 Andreas Warnke

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
