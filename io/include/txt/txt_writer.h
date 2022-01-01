/* File: txt_writer.h; Copyright and License: see below */

#ifndef TXT_WRITER_H
#define TXT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports a document in plain txt format
 */

#include "io_file_format.h"
#include "xml/xml_writer.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include "set/data_visible_set.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include "stream/universal_output_stream.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the txt writer
 */
struct txt_writer_struct {
    universal_output_stream_t *output;  /*!< output stream where to write the generated document to */
};

typedef struct txt_writer_struct txt_writer_t;

/*!
 *  \brief initializes the txt writer
 *
 *  \param this_ pointer to own object attributes
 *  \param output output stream where to write the generated document to
 */
void txt_writer_init( txt_writer_t *this_,
                      universal_output_stream_t *output
                    );

/*!
 *  \brief destroys the txt writer
 *
 *  \param this_ pointer to own object attributes
 */
void txt_writer_destroy( txt_writer_t *this_ );

/*!
 *  \brief writes a string to a file
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \return 0 in case of success, -1 otherwise
 */
static inline int txt_writer_write_plain ( txt_writer_t *this_, const char *text );

/*!
 *  \brief prints a multiline string with indentation prefix
 *
 *  if the string is empty, no character is written. If the last line is not empty, an additional newline is appended.
 *  newline, return and return-newline are recognized as line breaks.
 *
 *  \param this_ pointer to own object attributes
 *  \param indent pattern, by which each line is indented; must not be NULL
 *  \param multiline_string string to write to out
 *  \return 0 in case of success, -1 otherwise
 */
int txt_writer_write_indent_multiline_string ( txt_writer_t *this_,
                                               const char *indent,
                                               const char *multiline_string
                                             );

/*!
 *  \brief prints an id with indentation prefix and surrounding brackets
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param indent_width number of space-characters, by which each line is indented. Negative values cause a zero-indent.
 *  \param id identifier
 *  \return 0 in case of success, -1 otherwise
 */
int txt_writer_write_indent_id ( txt_writer_t *this_,
                                 int indent_width,
                                 data_id_t id
                               );

/*!
 *  \brief prints an id
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param id identifier
 *  \return 0 in case of success, -1 otherwise
 */
int txt_writer_write_plain_id ( txt_writer_t *this_, data_id_t id );

#include "txt_writer.inl"

#endif  /* TXT_WRITER_H */


/*
Copyright 2019-2022 Andreas Warnke

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
