/* File: universal_memory_output_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_MEMORY_OUTPUT_STREAM_H
#define UNIVERSAL_MEMORY_OUTPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements an universal_output_stream_if and forwards data to a fixed-sized memory buffer
 */

#include "stream/universal_output_stream.h"

/*!
 *  \brief attributes of the universal_memory_output_stream
 */
struct universal_memory_output_stream_struct {
    universal_output_stream_t output_stream;  /*!< instance of implemented interface \c universal_output_stream_t */
    void* mem_buf_start;  /*!< output memory buffer start */
    size_t mem_buf_size;  /*!< output memory buffer size */
    size_t mem_buf_filled;  /*!< number of bytes written to the output memory buffer */
};

typedef struct universal_memory_output_stream_struct universal_memory_output_stream_t;

/*!
 *  \brief initializes the universal_memory_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mem_buf_start address of memory buffer
 *  \param mem_buf_size size of the memory buffer
 */
void universal_memory_output_stream_init( universal_memory_output_stream_t *this_,
                                          void* mem_buf_start,
                                          size_t mem_buf_size
                                        );

/*!
 *  \brief destroys the universal_memory_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int universal_memory_output_stream_destroy( universal_memory_output_stream_t *this_ );

/*!
 *  \brief opens a file
 *
 *  \param this_ pointer to own object attributes
 *  \param path file path identifying the file to open for writing
 *  \result 0 in case of success, -1 otherwise
 */
int universal_memory_output_stream_open ( universal_memory_output_stream_t *this_, const char *path );

/*!
 *  \brief writes a buffer (e.g. a stringview) to a file
 *
 *  \param this_ pointer to own object attributes
 *  \param start buffer to write, not 0-terminated
 *  \param length length of the buffer to write
 *  \result 0 in case of success, -1 otherwise
 */
int universal_memory_output_stream_write ( universal_memory_output_stream_t *this_, const void *start, size_t length );

/*!
 *  \brief flushes buffers
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int universal_memory_output_stream_flush( universal_memory_output_stream_t *this_ );

/*!
 *  \brief closes the universal_memory_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int universal_memory_output_stream_close( universal_memory_output_stream_t *this_ );

/*!
 *  \brief gets the output stream interface of this universal_memory_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 */
universal_output_stream_t* universal_memory_output_stream_get_output_stream( universal_memory_output_stream_t *this_ );

#endif  /* UNIVERSAL_MEMORY_OUTPUT_STREAM_H */


/*
Copyright 2020-2020 Andreas Warnke

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
