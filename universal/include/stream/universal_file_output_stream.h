/* File: universal_file_output_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_FILE_OUTPUT_STREAM_H
#define UNIVERSAL_FILE_OUTPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements an universal_output_stream_if and forwards data to a FILE
 */

#include "stream/universal_output_stream.h"
#include <stdio.h>

/*!
 *  \brief attributes of the universal_file_output_stream
 */
struct universal_file_output_stream_struct {
    universal_output_stream_t output_stream;  /*!< instance of implemented interface \c universal_output_stream_t */
    FILE *output;  /*!< output file */
};

typedef struct universal_file_output_stream_struct universal_file_output_stream_t;

/*!
 *  \brief initializes the universal_file_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 */
void universal_file_output_stream_init( universal_file_output_stream_t *this_ );

/*!
 *  \brief destroys the universal_file_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int universal_file_output_stream_destroy( universal_file_output_stream_t *this_ );

/*!
 *  \brief opens a file
 *
 *  \param this_ pointer to own object attributes
 *  \param path file path identifying the file to open for writing
 *  \result 0 in case of success, -1 otherwise
 */
int universal_file_output_stream_open ( universal_file_output_stream_t *this_, const char *path );

/*!
 *  \brief writes a buffer (e.g. a stringview) to a file
 *
 *  \param this_ pointer to own object attributes
 *  \param start buffer to write, not 0-terminated
 *  \param length length of the buffer to write
 *  \result 0 in case of success, -1 otherwise
 */
int universal_file_output_stream_write ( universal_file_output_stream_t *this_, const void *start, size_t length );

/*!
 *  \brief flushes buffers
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int universal_file_output_stream_flush( universal_file_output_stream_t *this_ );

/*!
 *  \brief closes the universal_file_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int universal_file_output_stream_close( universal_file_output_stream_t *this_ );

/*!
 *  \brief gets the output stream interface of this universal_file_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 */
universal_output_stream_t* universal_file_output_stream_get_output_stream( universal_file_output_stream_t *this_ );

#endif  /* UNIVERSAL_FILE_OUTPUT_STREAM_H */


/*
Copyright 2020-2021 Andreas Warnke

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
