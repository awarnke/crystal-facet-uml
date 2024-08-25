/* File: utf8stream_writemem.h; Copyright and License: see below */

#ifndef UTF8STREAM_WRITEMEM_H
#define UTF8STREAM_WRITEMEM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief is a pair of an utf8 writer and a universal_memory_output_stream
 */

#include "u8stream/universal_memory_output_stream.h"
#include "u8stream/universal_output_stream.h"
#include "utf8stream/utf8stream_writer.h"
#include "u8/u8_error.h"
#include "utf8stringbuf/utf8stringview.h"
#include "utf8stringbuf/utf8string.h"
#include <stdio.h>

/*!
 *  \brief attributes of the utf8stream_writemem
 */
struct utf8stream_writemem_struct {
    utf8stream_writer_t writer;  /*!< own \c utf8stream_writer_t */
    universal_memory_output_stream_t mem_output;  /*!< own \c universal_memory_output_stream_t */
    u8_error_t lazy_err;  /*!< if a getter function produces an error, this is reported at reset or destroy */
};

typedef struct utf8stream_writemem_struct utf8stream_writemem_t;

/*!
 *  \brief initializes the utf8stream_writemem_t
 *
 *  \param this_ pointer to own object attributes
 *  \param mem_buf_start address of memory buffer
 *  \param mem_buf_size size of the memory buffer
 */
static inline void utf8stream_writemem_init ( utf8stream_writemem_t *this_, void* mem_buf_start, size_t mem_buf_size );

/*!
 *  \brief destroys the utf8stream_writemem_t
 *
 *  This method implicitly performs a utf8stream_writer_flush by calling utf8stream_writer_destroy.
 *  This method universal_memory_output_stream_destroy is no called to avoid duplicate call to flush.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise.
 *          Also a previous error that was not yet reported may be returned now.
 */
static inline u8_error_t utf8stream_writemem_destroy ( utf8stream_writemem_t *this_ );

/*!
 *  \brief resets the utf8stream_writemem_t to an empty buffer
 *
 *  Even if an error is returned, the utf8stream_writemem_t is reset and can be used.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_FILE_WRITE otherwise.
 *          Also a previous error that was not yet reported may be returned now.
 */
static inline u8_error_t utf8stream_writemem_reset ( utf8stream_writemem_t *this_ );

/*!
 *  \brief gets a pointer to the own writer
 *
 *  Consider to call utf8stream_writer_flush when finished writing before using the result.
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to the own writer
 */
static inline utf8stream_writer_t * utf8stream_writemem_get_writer ( utf8stream_writemem_t *this_ );

/*!
 *  \brief gets an utf8stringview_t
 *
 *  Consider to call utf8stream_writer_flush on the writer before retrieving an utf8stringview_t.
 *  This allows to check for error codes before using the result.
 *
 *  \param this_ pointer to own object attributes
 *  \return utf8stringview_t of the written utf8 codepoints
 */
static inline utf8stringview_t utf8stream_writemem_get_view ( utf8stream_writemem_t *this_ );

/*!
 *  \brief gets a const pointer to the c-string
 *
 *  Consider to call utf8stream_writer_flush on the writer before retrieving a c-string.
 *  This allows to check for error codes before using the result.
 *
 *  \param this_ pointer to own object attributes
 *  \return utf8string_t of the written utf8 codepoints
 */
static inline utf8string_t * utf8stream_writemem_get_string ( utf8stream_writemem_t *this_ );

#include "utf8stream_writemem.inl"

#endif  /* UTF8STREAM_WRITEMEM_H */


/*
Copyright 2024-2024 Andreas Warnke

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
