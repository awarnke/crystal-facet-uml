/* File: io_document_exporter.h; Copyright and License: see below */

#ifndef IO_DOCUMENT_EXPORTER_H
#define IO_DOCUMENT_EXPORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Exports a document
 */

#include "io_file_format.h"
#include "data_diagram.h"
#include "data_classifier.h"
#include "set/data_visible_set.h"
#include "pencil_description_writer.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief constants for max string sizes
 */
enum io_document_exporter_max_enum {
    IO_DOCUMENT_EXPORTER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

/*!
 *  \brief attributes of the document exporter
 */
struct io_document_exporter_struct {
    data_visible_set_t *painter_input_data;  /*!< pointer to external cache of diagram data */
    pencil_description_writer_t description_writer;  /*!< own instance of a description writer */

    char temp_output_buffer[IO_DOCUMENT_EXPORTER_MAX_STRING_SIZE];  /*!< temporary output buffer to convert strings */
    utf8stringbuf_t temp_output;  /*!< temporary output string buffer to convert strings */
    const char * const * xml_encode_table;  /*!< table for xml encode string replacements */
};

typedef struct io_document_exporter_struct io_document_exporter_t;

/*!
 *  \brief initializes the document exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param painter_input_data pointer to cache of diagram data
 */
void io_document_exporter_init( io_document_exporter_t *this_,
                                data_visible_set_t *painter_input_data
                              );

/*!
 *  \brief destroys the document exporter
 *
 *  \param this_ pointer to own object attributes
 */
void io_document_exporter_destroy( io_document_exporter_t *this_ );

/*!
 *  \brief writes the header of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param document_title title of the document
 *  \param output file object where to write the document to
 *  \result 0 in case of success, -1 otherwise
 */
int io_document_exporter_write_header( io_document_exporter_t *this_,
                                       io_file_format_t export_type,
                                       const char *document_title,
                                       FILE *output
                                     );

/*!
 *  \brief writes a diagram start
 *
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param output file object where to write the document to
 *  \result 0 in case of success, -1 otherwise
 */
int io_document_exporter_start_diagram( io_document_exporter_t *this_,
                                        io_file_format_t export_type,
                                        FILE *output
                                      );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \param output file object where to write the document to
 *  \result 0 in case of success, -1 otherwise
 */
int io_document_exporter_write_diagram( io_document_exporter_t *this_,
                                        io_file_format_t export_type,
                                        const char *diagram_file_base_name,
                                        FILE *output
                                      );

/*!
 *  \brief writes a diagram end
 *
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param output file object where to write the document to
 *  \result 0 in case of success, -1 otherwise
 */
int io_document_exporter_end_diagram( io_document_exporter_t *this_,
                                      io_file_format_t export_type,
                                      FILE *output
                                    );

/*!
 *  \brief writes a diagram of the document
 *
 *  \param this_ pointer to own object attributes
 *  \param export_type image file format
 *  \param output file object where to write the document to
 *  \result 0 in case of success, -1 otherwise
 */
int io_document_exporter_write_footer( io_document_exporter_t *this_,
                                       io_file_format_t export_type,
                                       FILE *output
                                     );

/*!
 *  \brief writes a string to a file, unencoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \param output file
 *  \result 0 in case of success, -1 otherwise
 */
static inline int io_document_exporter_private_write_plain ( io_document_exporter_t *this_,
                                                             const char *text,
                                                             FILE *output
                                                           );

/*!
 *  \brief writes a string to a file, xml encoded
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to write
 *  \param output file
 *  \result 0 in case of success, -1 otherwise
 */
static inline int io_document_exporter_private_write_xml_enc ( io_document_exporter_t *this_,
                                                               const char *text,
                                                               FILE *output
                                                             );

#include "io_document_exporter.inl"

#endif  /* IO_DOCUMENT_EXPORTER_H */


/*
Copyright 2016-2019 Andreas Warnke

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
