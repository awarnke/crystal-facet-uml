/* File: io_export_interaction_traversal.h; Copyright and License: see below */

#ifndef IO_EXPORT_INTERACTION_TRAVERSAL_H
#define IO_EXPORT_INTERACTION_TRAVERSAL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Traverses the model-part shown in an interaction diagram and writes the elements to an xmi_interaction_writer
 *
 *  Source: db_reader(data_database_reader_t);
 *  Task: traverse the source model-part;
 *  Sink: xmi_interaction_writer
 */

#include "xmi/xmi_interaction_writer.h"
#include "set/data_visible_set.h"
#include "set/data_node_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "data_table.h"
#include "data_rules.h"
#include "universal_array_list.h"
#include <stdio.h>

/*!
 *  \brief attributes of the description writer
 */
struct io_export_interaction_traversal_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    data_visible_set_t *input_data;  /*!< pointer to an external buffer for private use as data cache */
    data_rules_t filter_rules;  /*!< own instance of uml and sysml consistency rules */
    universal_array_list_t *written_id_set;  /*!< pointer to external list of already exported element ids */
    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    xmi_interaction_writer_t format_writer;  /*!< pointer to own xmi_interaction_writer which is the output sink */
};

typedef struct io_export_interaction_traversal_struct io_export_interaction_traversal_t;

/*!
 *  \brief initializes the io_export_interaction_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param input_data pointer to an external buffer for private use as data cache
 *  \param io_written_id_set pointer to external list of already exported element ids
 *  \param io_export_stat pointer to statistics object where export statistics are collected
 *  \param out_writer pointer to an xml_writer_t which is the output sink
 */
void io_export_interaction_traversal_init( io_export_interaction_traversal_t *this_,
                                           data_database_reader_t *db_reader,
                                           data_visible_set_t *input_data,
                                           universal_array_list_t *io_written_id_set,
                                           data_stat_t *io_export_stat,
                                           xml_writer_t *out_writer
                                         );

/*!
 *  \brief destroys the io_export_interaction_traversal_t
 *
 *  \param this_ pointer to own object attributes
 */
void io_export_interaction_traversal_destroy( io_export_interaction_traversal_t *this_ );

/*!
 *  \brief iterates all interactions where classifier is involved, exports these interactions
 *
 *  \param this_ pointer to own object attributes
 *  \param nesting_type type of the nesting parent classifier
 *  \param classifier_id id of the classifier which occurrences to process for export
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_interaction_traversal_iterate_classifier_occurrences ( io_export_interaction_traversal_t *this_,
                                                                     data_classifier_type_t nesting_type,
                                                                     data_id_t classifier_id
                                                                   );

/*!
 *  \brief prints the end of a diagram section
 *
 *  \param this_ pointer to own object attributes
 *  \param nesting_type type of the nesting parent classifier
 *  \param diagram_id id of the diagram which to process for export
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_interaction_traversal_private_walk_diagram ( io_export_interaction_traversal_t *this_, 
                                                           data_classifier_type_t nesting_type,
                                                           data_id_t diagram_id 
                                                         );

/*!
 *  \brief prints names and descriptions of the classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data of which the classifiers are written, not NULL
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_interaction_traversal_private_iterate_diagram_classifiers ( io_export_interaction_traversal_t *this_,
                                                                          const data_visible_set_t *diagram_data
                                                                        );

/*!
 *  \brief prints names and descriptions of the classifier to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data that contains the classifier of which the features are written, not NULL
 *  \param classifier_id id of the classifier of which the features are written
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_interaction_traversal_private_iterate_classifier_features ( io_export_interaction_traversal_t *this_,
                                                                          const data_visible_set_t *diagram_data,
                                                                          data_id_t classifier_id
                                                                        );

/*!
 *  \brief prints names and descriptions of the relationships to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data that contains the from-classifier of which the relationships are written, not NULL
 *  \param from_classifier_id id of the classifier of which the relationships are written
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_interaction_traversal_private_iterate_classifier_relationships ( io_export_interaction_traversal_t *this_,
                                                                               const data_visible_set_t *diagram_data,
                                                                               data_id_t from_classifier_id
                                                                             );

#endif  /* IO_EXPORT_INTERACTION_TRAVERSAL_H */


/*
Copyright 2020-2020 Andreas Warnke

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
