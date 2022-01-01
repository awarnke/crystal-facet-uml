/* File: universal_memory_input_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_MEMORY_INPUT_STREAM_H
#define UNIVERSAL_MEMORY_INPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief reads data from a fixed-sized memory buffer and implements an universal_input_stream
 */

#include "stream/universal_input_stream.h"

/*!
 *  \brief attributes of the universal_memory_input_stream
 */
struct universal_memory_input_stream_struct {
    universal_input_stream_t input_stream;  /*!< instance of implemented interface \c universal_input_stream_t */
    const void* mem_buf_start;  /*!< input memory buffer start */
    size_t mem_buf_size;  /*!< input memory buffer size */
    size_t mem_buf_pos;  /*!< read position in the input memory buffer */
};

typedef struct universal_memory_input_stream_struct universal_memory_input_stream_t;

/*!
 *  \brief initializes the universal_memory_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mem_buf_start address of memory buffer
 *  \param mem_buf_size size of the memory buffer
 */
void universal_memory_input_stream_init ( universal_memory_input_stream_t *this_,
                                          const void* mem_buf_start,
                                          size_t mem_buf_size
                                        );

/*!
 *  \brief re-initializes the universal_memory_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mem_buf_start address of memory buffer
 *  \param mem_buf_size size of the memory buffer
 */
void universal_memory_input_stream_reinit ( universal_memory_input_stream_t *this_,
                                            const void* mem_buf_start,
                                            size_t mem_buf_size
                                          );

/*!
 *  \brief destroys the universal_memory_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 */
void universal_memory_input_stream_destroy ( universal_memory_input_stream_t *this_ );

/*!
 *  \brief resets the read position to 0
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
u8_error_t universal_memory_input_stream_reset ( universal_memory_input_stream_t *this_ );

/*!
 *  \brief reads a buffer from a memory region
 *
 *  \param this_ pointer to own object attributes
 *  \param out_buffer buffer to write read bytes
 *  \param max_size length of the buffer to write
 *  \param out_length number of bytes read
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_READ otherwise
 */
u8_error_t universal_memory_input_stream_read ( universal_memory_input_stream_t *this_,
                                                void *out_buffer,
                                                size_t max_size,
                                                size_t *out_length
                                              );

/*!
 *  \brief gets the input stream interface of this universal_memory_input_stream_t
 *
 *  \param this_ pointer to own object attributes
 *  \return the abstract base class of this_
  */
universal_input_stream_t* universal_memory_input_stream_get_input_stream( universal_memory_input_stream_t *this_ );

#endif  /* UNIVERSAL_MEMORY_INPUT_STREAM_H */


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
