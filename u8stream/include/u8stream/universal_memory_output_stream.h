/* File: universal_memory_output_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_MEMORY_OUTPUT_STREAM_H
#define UNIVERSAL_MEMORY_OUTPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements an universal_output_stream and forwards data to a fixed-sized memory buffer
 */

#include "u8stream/universal_output_stream.h"

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
 */
void universal_memory_output_stream_destroy( universal_memory_output_stream_t *this_ );

/*!
 *  \brief resets write position to 0 of this memory output stream,
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_memory_output_stream_reset ( universal_memory_output_stream_t *this_ );

/*!
 *  \brief writes a buffer (e.g. a stringview) to a memory output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param start buffer to write, not 0-terminated
 *  \param length length of the buffer to write
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_memory_output_stream_write ( universal_memory_output_stream_t *this_, const void *start, size_t length );

/*!
 *  \brief flushes buffers
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_memory_output_stream_flush( universal_memory_output_stream_t *this_ );

/*!
 *  \brief writes a terminating zero to a memory output stream
 *
 *  If the memory buffer is full, the last byte is overwritten by zero
 *  to ensure that even in this error case, the buffer is null-terminated.
 *
 *  \param this_ pointer to own object attributes
 *  \param utf8_mode true ensures that no code point is cut in half
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_CONFIG_OUT_OF_RANGE if memory buffer has size 0, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_memory_output_stream_write_0term ( universal_memory_output_stream_t *this_, bool utf8_mode );

/*!
 *  \brief gets the output stream interface of this universal_memory_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
  */
universal_output_stream_t* universal_memory_output_stream_get_output_stream( universal_memory_output_stream_t *this_ );

#endif  /* UNIVERSAL_MEMORY_OUTPUT_STREAM_H */


/*
Copyright 2020-2024 Andreas Warnke

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
