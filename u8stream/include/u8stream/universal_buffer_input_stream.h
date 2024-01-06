/* File: universal_buffer_input_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_BUFFER_INPUT_STREAM_H
#define UNIVERSAL_BUFFER_INPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements an universal_input_stream and buffers data in a fixed-sized memory buffer
 */

#include "u8stream/universal_input_stream.h"

/*!
 *  \brief attributes of the universal_buffer_input_stream
 */
struct universal_buffer_input_stream_struct {
    universal_input_stream_t input_stream;  /*!< instance of implemented interface \c universal_input_stream_t */
    void* mem_buf_start;  /*!< input memory buffer start */
    size_t mem_buf_size;  /*!< input memory buffer size */
    size_t mem_buf_fill;  /*!< input memory buffer: amount of valid, read bytes */
    size_t mem_buf_pos;  /*!< read position in the input memory buffer */
    universal_input_stream_t *source;  /*!< pointer to stream source, an external \c universal_input_stream_t */
    size_t stream_pos_of_buf;  /*!< buffer position in the total stream; read position is (stream_pos_of_buf+mem_buf_pos) */
};

typedef struct universal_buffer_input_stream_struct universal_buffer_input_stream_t;

/*!
 *  \brief initializes the universal_buffer_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mem_buf_start address of memory buffer
 *  \param mem_buf_size size of the memory buffer
 *  \param source stream source where to read bytes from when buffer empty
 */
void universal_buffer_input_stream_init ( universal_buffer_input_stream_t *this_,
                                          void* mem_buf_start,
                                          size_t mem_buf_size,
                                          universal_input_stream_t *source
                                        );

/*!
 *  \brief destroys the universal_buffer_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 */
void universal_buffer_input_stream_destroy ( universal_buffer_input_stream_t *this_ );

/*!
 *  \brief resets the buffer to empty
 *
 *  \param this_ pointer to own object attributes
 */
void universal_buffer_input_stream_reset ( universal_buffer_input_stream_t *this_ );

/*!
 *  \brief reads a buffer from a memory region
 *
 *  \param this_ pointer to own object attributes
 *  \param out_buffer buffer to write read bytes
 *  \param max_size length of the buffer to write
 *  \param out_length number of bytes read
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
u8_error_t universal_buffer_input_stream_read ( universal_buffer_input_stream_t *this_,
                                                void *out_buffer,
                                                size_t max_size,
                                                size_t *out_length
                                              );

/*!
 *  \brief fetches 1 byte from the input buffer without moving the read position.
 *
 *  \param this_ pointer to own object attributes
 *  \return next byte or '\0' if end of stream
 */
static inline char universal_buffer_input_stream_peek_next ( universal_buffer_input_stream_t *this_ );

/*!
 *  \brief reads 1 byte from the input buffer,
 *
 *  \param this_ pointer to own object attributes
 *  \return next byte or '\0' if end of stream
 */
static inline char universal_buffer_input_stream_read_next ( universal_buffer_input_stream_t *this_ );

/*!
 *  \brief returns the current read position
 *
 *  \param this_ pointer to own object attributes
 *  \return read position
 */
static inline size_t universal_buffer_input_stream_read_pos ( universal_buffer_input_stream_t *this_ );

/*!
 *  \brief gets the input stream interface of this universal_buffer_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
  */
universal_input_stream_t* universal_buffer_input_stream_get_input_stream( universal_buffer_input_stream_t *this_ );

#include "universal_buffer_input_stream.inl"

#endif  /* UNIVERSAL_BUFFER_INPUT_STREAM_H */


/*
Copyright 2021-2024 Andreas Warnke

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
