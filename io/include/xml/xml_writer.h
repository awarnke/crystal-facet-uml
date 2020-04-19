/* File: xml_writer.h; Copyright and License: see below */

#ifndef XML_WRITER_H
#define XML_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief writes strings to an xml stream
 */

#include "io_file_format.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include "set/data_visible_set.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief constants for max string sizes
 */
enum xml_writer_max_enum {
    XML_WRITER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

/*!
 *  \brief attributes of the xml writer
 */
struct xml_writer_struct {
    FILE *output;  /*!< output file */

    char temp_output_buffer[XML_WRITER_MAX_STRING_SIZE];  /*!< temporary output buffer to convert strings */
    utf8stringbuf_t temp_output;  /*!< temporary output string buffer to convert strings */
    const char * const * xml_encode_table;  /*!< table for xml encode string replacements */
    const char * const * fmt_xhtml_encode_table;  /*!< table for formatted xhtml encode string replacements */
    const char * const * fmt_db_encode_table;  /*!< table for formatted docbook encode string replacements */
};

typedef struct xml_writer_struct xml_writer_t;

/*!
 *  \brief initializes the document exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param output file object where to write the document to
 */
void xml_writer_init( xml_writer_t *this_, FILE *output );

/*!
 *  \brief destroys the document exporter
 *
 *  \param this_ pointer to own object attributes
 */
void xml_writer_destroy( xml_writer_t *this_ );

/*!
 *  \brief writes a string to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_plain ( xml_writer_t *this_, const char *text );

/*!
 *  \brief prints an id
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param table table identifier
 *  \param row_id row identifier
 *  \result 0 in case of success, -1 otherwise
 */
int xml_writer_private_write_plain_id ( xml_writer_t *this_, data_table_t table, int64_t row_id );

/*!
 *  \brief writes a string to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_xml_enc ( xml_writer_t *this_, const char *text );

/*!
 *  \brief writes a string to a file as formatted xhtml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_fmt_xhtml_enc ( xml_writer_t *this_, const char *text );

/*!
 *  \brief writes a string to a file as formatted docbook encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int xml_writer_write_fmt_db_enc ( xml_writer_t *this_, const char *text );

#include "xml_writer.inl"

#endif  /* XML_WRITER_H */


/*
Copyright 2019-2020 Andreas Warnke

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
