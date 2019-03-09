/* File: io_diagram_text_exporter.h; Copyright and License: see below */

#ifndef IO_DIAGRAM_TEXT_EXPORTER_H
#define IO_DIAGRAM_TEXT_EXPORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders diagram and classifier descriptions to an output stream
 */

#include "set/data_visible_set.h"
#include "data_table.h"
#include <stdio.h>

/*!
 *  \brief attributes of the description writer
 */
struct io_diagram_text_exporter_struct {
    data_visible_set_t *input_data;  /*!< pointer to an external data cache */
};

typedef struct io_diagram_text_exporter_struct io_diagram_text_exporter_t;

/*!
 *  \brief initializes the io_diagram_text_exporter_t
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be rendered
 */
void io_diagram_text_exporter_init( io_diagram_text_exporter_t *this_, data_visible_set_t *input_data );

/*!
 *  \brief destroys the io_diagram_text_exporter_t
 *
 *  \param this_ pointer to own object attributes
 */
void io_diagram_text_exporter_destroy( io_diagram_text_exporter_t *this_ );

/*!
 *  \brief prints names and descriptions of the diagram and contained classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param out a stream where to print the data
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_draw ( io_diagram_text_exporter_t *this_, FILE *out );

/*!
 *  \brief prints names and descriptions of the diagram to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param out a stream where to print the data
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_private_write_diagram ( io_diagram_text_exporter_t *this_, FILE *out );

/*!
 *  \brief prints names and descriptions of the diagram to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param out a stream where to print the data
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_private_write_classifiers ( io_diagram_text_exporter_t *this_, FILE *out );

/*!
 *  \brief prints names and descriptions of the diagram to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier of which the features are written
 *  \param out a stream where to print the data
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_private_write_features_of_classifier ( io_diagram_text_exporter_t *this_, int64_t classifier_id, FILE *out );

/*!
 *  \brief prints names and descriptions of the diagram to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier of which the relations are written
 *  \param out a stream where to print the data
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_private_write_relations_of_classifier ( io_diagram_text_exporter_t *this_, int64_t classifier_id, FILE *out );

/*!
 *  \brief prints a multiline string with indentation prefix
 *
 *  if the string is empty, no character is written. If the last line is not empty, an additional newline is appended.
 *  newline, return and return-newline are recognized as line breaks.
 *
 *  \param this_ pointer to own object attributes
 *  \param indent pattern, by which each line is indented; must not be NULL
 *  \param multiline_string string to write to out
 *  \param out a stream where to print the data
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_private_write_indent_multiline_string ( io_diagram_text_exporter_t *this_,
                                                                      const char *indent,
                                                                      const char *multiline_string,
                                                                      FILE *out
                                                                    );

/*!
 *  \brief prints an id
 *
 *  if the id is invalid, nothing is printed.
 *
 *  \param this_ pointer to own object attributes
 *  \param indent_width number of space-characters, by which each line is indented. Negative values cause a zero-indent.
 *  \param table table identifier
 *  \param row_id row identifier
 *  \param out a stream where to print the data
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_private_write_id ( io_diagram_text_exporter_t *this_,
                                                 int indent_width,
                                                 data_table_t table,
                                                 int64_t row_id,
                                                 FILE *out
                                               );

#endif  /* IO_DIAGRAM_TEXT_EXPORTER_H */


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
