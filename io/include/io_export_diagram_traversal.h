/* File: io_export_diagram_traversal.h; Copyright and License: see below */

#ifndef IO_EXPORT_DIAGRAM_TRAVERSAL_H
#define IO_EXPORT_DIAGRAM_TRAVERSAL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Traverses the model-part shown in a diagram and writes the elements to an format writer
 *
 *  Source: db_reader(data_database_reader_t);
 *  Task: traverse the source model-part;
 *  Sink: html_element_writer_t
 */

#include "io_element_writer.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "entity/data_table.h"
#include "data_rules.h"
#include "u8/u8_error.h"
#include <stdio.h>

/*!
 *  \brief attributes of the diagram traversal
 *
 *  Lifecycle: A database traversal shall perform a single export operation only.
 *  It may be initialized before one export operation and be destroyed afterwards.
 */
struct io_export_diagram_traversal_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    data_visible_set_t *input_data;  /*!< pointer to an external buffer for private use as data cache */
    data_rules_t filter_rules;  /*!< own instance of uml and sysml consistency rules */
    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    io_element_writer_t *element_writer;  /*!< pointer to external io_element_writer_t which is the output sink */

    data_diagram_t temp_parent_diag;  /*!< own instance of a data cache for the parent diagram */
};

typedef struct io_export_diagram_traversal_struct io_export_diagram_traversal_t;

/*!
 *  \brief initializes the io_export_diagram_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param input_data pointer to an external buffer for private use as data cache
 *  \param io_export_stat pointer to statistics object where export statistics are collected.
 *                        Errors and warnings during traversal are counted. Success shall be counted by io_element_writer_t.
 *                        Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_element_writer pointer to an external io_element_writer_t which is the output sink
 */
void io_export_diagram_traversal_init( io_export_diagram_traversal_t *this_,
                                       data_database_reader_t *db_reader,
                                       data_visible_set_t *input_data,
                                       data_stat_t *io_export_stat,
                                       io_element_writer_t *out_element_writer
                                     );

/*!
 *  \brief destroys the io_export_diagram_traversal_t
 *
 *  \param this_ pointer to own object attributes
 */
void io_export_diagram_traversal_destroy( io_export_diagram_traversal_t *this_ );

/*!
 *  \brief prints names and descriptions of the diagram and contained classifiers to the output stream
 *
 *  Note: When finished, the caller may recurse to chilren diagrams.
 *  Finally, io_export_diagram_traversal_end_diagram has to be called.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram which to process for export
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_diagram_traversal_begin_and_walk_diagram ( io_export_diagram_traversal_t *this_,
                                                                data_id_t diagram_id,
                                                                const char *diagram_file_base_name
                                                              );

/*!
 *  \brief prints the end of a diagram section
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram which to process for export
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_diagram_traversal_end_diagram ( io_export_diagram_traversal_t *this_,
                                                     data_id_t diagram_id );

/*!
 *  \brief prints names and descriptions of the classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data of which the classifiers are written, not NULL
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_diagram_traversal_private_iterate_diagram_classifiers ( io_export_diagram_traversal_t *this_,
                                                                             const data_visible_set_t *diagram_data
                                                                           );

/*!
 *  \brief prints names and descriptions of the classifier to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data that contains the classifier of which the features are written, not NULL
 *  \param classifier_id id of the classifier of which the features are written
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_diagram_traversal_private_iterate_classifier_features ( io_export_diagram_traversal_t *this_,
                                                                             const data_visible_set_t *diagram_data,
                                                                             data_id_t classifier_id
                                                                           );

/*!
 *  \brief prints names and descriptions of the relationships to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data that contains the from-classifier of which the relationships are written, not NULL
 *  \param from_classifier_id id of the classifier of which the relationships are written
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_diagram_traversal_private_iterate_classifier_relationships ( io_export_diagram_traversal_t *this_,
                                                                                  const data_visible_set_t *diagram_data,
                                                                                  data_id_t from_classifier_id
                                                                                );

#endif  /* IO_EXPORT_DIAGRAM_TRAVERSAL_H */


/*
Copyright 2016-2025 Andreas Warnke

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
