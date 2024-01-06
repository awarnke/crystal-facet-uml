/* File: utf8stream_writer.h; Copyright and License: see below */

#ifndef UNIVERSAL_UTF8_WRITER_H
#define UNIVERSAL_UTF8_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief implements a utf8 writer and forwards the output to a universal_output_stream
 */

#include "u8stream/universal_output_stream.h"
#include <stdio.h>

/*!
 *  \brief attributes of the utf8stream_writer
 */
struct utf8stream_writer_struct {
    universal_output_stream_t *output_stream;  /*!< pointer to external \c universal_output_stream_t */
};

typedef struct utf8stream_writer_struct utf8stream_writer_t;

/*!
 *  \brief initializes the utf8stream_writer_t
 *
 *  \param this_ pointer to own object attributes
 *  \param out_stream output stream
 */
static inline void utf8stream_writer_init ( utf8stream_writer_t *this_, universal_output_stream_t* out_stream );

/*!
 *  \brief destroys the utf8stream_writer_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void utf8stream_writer_destroy ( utf8stream_writer_t *this_ );

/*!
 *  \brief writes an utf8 string to a stream
 *
 *  Note that the underlying universal_output_stream_t may cut the stream when e.g. the sink location is full.
 *  This may cut an utf8 code point in half.
 *
 *  \param this_ pointer to own object attributes
 *  \param utf8_string 0-terminated string to write
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
static inline u8_error_t utf8stream_writer_write_str ( utf8stream_writer_t *this_, const void *utf8_string );

/*!
 *  \brief writes an integer to a stream
 *
 *  \param this_ pointer to own object attributes
 *  \param number number to write
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
static inline u8_error_t utf8stream_writer_write_int ( utf8stream_writer_t *this_, const int64_t number );

/*!
 *  \brief writes an integer as hexadecimal number to a stream
 *
 *  \param this_ pointer to own object attributes
 *  \param number number to write in hexadecimal
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
static inline u8_error_t utf8stream_writer_write_hex ( utf8stream_writer_t *this_, const int64_t number );

/*!
 *  \brief flushes buffers
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise
 */
static inline u8_error_t utf8stream_writer_flush ( utf8stream_writer_t *this_ );

#include "utf8stream_writer.inl"

#endif  /* UNIVERSAL_UTF8_WRITER_H */


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
