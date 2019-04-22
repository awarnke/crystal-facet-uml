/* File: io_format_writer.h; Copyright and License: see below */

#ifndef IO_FORMAT_WRITER_H
#define IO_FORMAT_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports a document
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
enum io_format_writer_max_enum {
    IO_DOCUMENT_EXPORTER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

/*!
 *  \brief attributes of the document exporter
 */
struct io_format_writer_struct {
    io_file_format_t export_type;  /*!< format of output document */
    FILE *output;  /*!< output file */
    uint32_t current_tree_depth;  /*!< tree depth in diagram tree, starts at 0, increases with every call to io_format_writer_start_diagram */

    char temp_output_buffer[IO_DOCUMENT_EXPORTER_MAX_STRING_SIZE];  /*!< temporary output buffer to convert strings */
    utf8stringbuf_t temp_output;  /*!< temporary output string buffer to convert strings */
    const char * const * xml_encode_table;  /*!< table for xml encode string replacements */
};

typedef struct io_format_writer_struct io_format_writer_t;

/*!
 *  \brief initializes the document exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param output file object where to write the document to
 */
void io_format_writer_init( io_format_writer_t *this_,
                            io_file_format_t export_type,
                            FILE *output
                          );

/*!
 *  \brief destroys the document exporter
 *
 *  \param this_ pointer to own object attributes
 */
void io_format_writer_destroy( io_format_writer_t *this_ );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param document_title title of the document
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_header( io_format_writer_t *this_, const char *document_title );

/*!
 *  \brief writes a diagram start
 *
 *  This starts a section that contains a diagram and a list of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_start_diagram( io_format_writer_t *this_ );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param diag_ptr pointer to diagram that shall be written
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_diagram( io_format_writer_t *this_,
                                    data_diagram_t *diag_ptr,
                                    const char *diagram_file_base_name
                                  );

/*!
 *  \brief writes a classifier start
 *
 *  This starts a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_start_classifier( io_format_writer_t *this_ );

/*!
 *  \brief writes a classifier of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_ptr pointer to classifier that shall be written
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_classifier( io_format_writer_t *this_, data_classifier_t *classifier_ptr );

/*!
 *  \brief writes a feature of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_ptr pointer to feature that shall be written
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_feature( io_format_writer_t *this_, data_feature_t *feature_ptr );

/*!
 *  \brief writes a relationship of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param relation_ptr pointer to relationship that shall be written
 *  \param dest_classifier_ptr pointer to destination classifier
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_relationship( io_format_writer_t *this_,
                                         data_relationship_t *relation_ptr,
                                         data_classifier_t *dest_classifier_ptr
                                       );

/*!
 *  \brief writes a classifier end
 *
 *  This ends a division that contains a classifier and a list of features and relationships
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_end_classifier( io_format_writer_t *this_ );

/*!
 *  \brief writes a diagram end
 *
 *  This ends a section that contains a diagram and a list of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_end_diagram( io_format_writer_t *this_ );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_write_footer( io_format_writer_t *this_ );

/*!
 *  \brief writes a string to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int io_format_writer_private_write_plain ( io_format_writer_t *this_, const char *text );

/*!
 *  \brief writes a string to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \result 0 in case of success, -1 otherwise
 */
static inline int io_format_writer_private_write_xml_enc ( io_format_writer_t *this_, const char *text );

/*!
 *  \brief prints a multiline string with indentation prefix
 *
 *  if the string is empty, no character is written. If the last line is not empty, an additional newline is appended.
 *  newline, return and return-newline are recognized as line breaks.
 *
 *  \param this_ pointer to own object attributes
 *  \param indent pattern, by which each line is indented; must not be NULL
 *  \param multiline_string string to write to out
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_private_write_indent_multiline_string ( io_format_writer_t *this_,
                                                             const char *indent,
                                                             const char *multiline_string
                                                           );

/*!
 *  \brief prints an id with indentation prefix
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param indent_width number of space-characters, by which each line is indented. Negative values cause a zero-indent.
 *  \param table table identifier
 *  \param row_id row identifier
 *  \result 0 in case of success, -1 otherwise
 */
int io_format_writer_private_write_indent_id ( io_format_writer_t *this_,
                                               int indent_width,
                                               data_table_t table,
                                               int64_t row_id
                                             );

#include "io_format_writer.inl"

#endif  /* IO_FORMAT_WRITER_H */


/*
Copyright 2019-2019 Andreas Warnke

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
