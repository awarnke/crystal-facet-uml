/* File: universal_buffer_output_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_BUFFER_INPUT_STREAM_H
#define UNIVERSAL_BUFFER_INPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements an universal_output_stream and buffers data in a fixed-sized memory buffer
 */

#include "u8stream/universal_output_stream.h"

/*!
 *  \brief attributes of the universal_buffer_output_stream
 */
struct universal_buffer_output_stream_struct {
    universal_output_stream_t output_stream;  /*!< instance of implemented interface \c universal_output_stream_t */
    void* mem_buf_start;  /*!< output memory buffer start */
    size_t mem_buf_size;  /*!< output memory buffer size */
    size_t mem_buf_filled;  /*!< number of bytes written to the output memory buffer */
    universal_output_stream_t *sink;  /*!< pointer to stream sink, an external \c universal_output_stream_t */
};

typedef struct universal_buffer_output_stream_struct universal_buffer_output_stream_t;

/*!
 *  \brief initializes the universal_buffer_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mem_buf_start address of memory buffer
 *  \param mem_buf_size size of the memory buffer
 *  \param sink stream sink where to forward bytes to at flush and at full buffer
 */
void universal_buffer_output_stream_init( universal_buffer_output_stream_t *this_,
                                          void* mem_buf_start,
                                          size_t mem_buf_size,
                                          universal_output_stream_t *sink
                                        );

/*!
 *  \brief destroys the universal_buffer_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_buffer_output_stream_destroy( universal_buffer_output_stream_t *this_ );

/*!
 *  \brief writes a buffer (e.g. a stringview) to a memory output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param start buffer to write, not 0-terminated
 *  \param length length of the buffer to write
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_buffer_output_stream_write ( universal_buffer_output_stream_t *this_, const void *start, size_t length );

/*!
 *  \brief flushes buffers
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
u8_error_t universal_buffer_output_stream_flush( universal_buffer_output_stream_t *this_ );

/*!
 *  \brief gets the output stream interface of this universal_buffer_output_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
  */
universal_output_stream_t* universal_buffer_output_stream_get_output_stream( universal_buffer_output_stream_t *this_ );

#endif  /* UNIVERSAL_BUFFER_INPUT_STREAM_H */


/*
Copyright 2021-2026 Andreas Warnke

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
