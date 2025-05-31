/* File: io_txt_writer.h; Copyright and License: see below */

#ifndef IO_TXT_WRITER_H
#define IO_TXT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports a document in plain txt format
 */

#include "io_file_format.h"
#include "format/io_xml_writer.h"
#include "entity/data_diagram.h"
#include "entity/data_classifier.h"
#include "set/data_visible_set.h"
#include "geometry/geometry_rectangle.h"
#include "utf8stringbuf/utf8stringview.h"
#include "u8stream/universal_output_stream.h"
#include "u8/u8_error.h"
#include "io_gtk.h"

/*!
 *  \brief attributes of the txt writer
 */
struct io_txt_writer_struct {
    universal_output_stream_t *output;  /*!< output stream where to write the generated document to */
};

typedef struct io_txt_writer_struct io_txt_writer_t;

/*!
 *  \brief initializes the txt writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated document to
 */
void io_txt_writer_init( io_txt_writer_t *this_,
                         universal_output_stream_t *output
                       );

/*!
 *  \brief destroys the txt writer
 *
 *  \param this_ pointer to own object attributes
 */
void io_txt_writer_destroy( io_txt_writer_t *this_ );

/*!
 *  \brief writes a string to a file
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return U8_ERROR_NONE in case of success
 */
static inline u8_error_t io_txt_writer_write_plain ( io_txt_writer_t *this_, const char *text );

/*!
 *  \brief prints a multiline string with indentation prefix
 *
 *  if the string is empty, no character is written. If the last line is not empty, an additional newline is appended.
 *  newline, return and return-newline are recognized as line breaks.
 *
 *  \param this_ pointer to own object attributes
 *  \param indent pattern, by which each line is indented; must not be NULL
 *  \param multiline_string string to write to out
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_txt_writer_write_indent_multiline_string ( io_txt_writer_t *this_,
                                                         const char *indent,
                                                         const char *multiline_string
                                                       );

/*!
 *  \brief prints an id with indentation prefix and surrounding brackets
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param right_align_to right alignment position, by which the number of space-characters to insert is determined.
 *  \param id identifier
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_txt_writer_write_aligned_id ( io_txt_writer_t *this_,
                                            int right_align_to,
                                            data_id_t id
                                          );

/*!
 *  \brief prints an id
 *
 *  In case the id is invalid, nothing is printed.
 *  The id is printed 10 char wide, right-aligned.
 *
 *  \param this_ pointer to own object attributes
 *  \param id identifier
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_txt_writer_write_plain_id ( io_txt_writer_t *this_, data_id_t id );

#include "io_txt_writer.inl"

#endif  /* IO_TXT_WRITER_H */


/*
Copyright 2019-2025 Andreas Warnke

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
