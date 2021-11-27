/* File: io_export_flat_traversal.h; Copyright and License: see below */

#ifndef IO_EXPORT_FLAT_TRAVERSAL_H
#define IO_EXPORT_FLAT_TRAVERSAL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Traverses the model by iterating over a flat list of all classifiers
 *         and writes the elements to an format writer.
 *
 *  Source: db_reader(data_database_reader_t);
 *  Task: traverse the source model;
 *  Sink: io_element_writer_t
 */

#include "io_export_interaction_traversal.h"
#include "io_element_writer.h"
#include "set/data_node_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "data_table.h"
#include "universal_array_list.h"
#include <stdio.h>

/*!
 *  \brief attributes of the description writer
 */
struct io_export_flat_traversal_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    io_element_writer_t *element_writer;  /*!< pointer to external io_element_writer_t which is the output sink */

    data_classifier_t temp_classifier;  /*!< own buffer for private use as data cache */
    data_node_set_t temp_node_data;  /*!< own buffer for private use as data cache */
    data_classifier_t temp_from_classifier;  /*!< own buffer for private use as data cache in private_iterate_relationships*/
    data_feature_t temp_from_feature;  /*!< own buffer for private use as data cache in private_iterate_relationships*/
    data_classifier_t temp_to_classifier;  /*!< own buffer for private use as data cache in private_iterate_relationships*/
    data_feature_t temp_to_feature;  /*!< own buffer for private use as data cache in private_iterate_relationships*/
};

typedef struct io_export_flat_traversal_struct io_export_flat_traversal_t;

/*!
 *  \brief initializes the io_export_flat_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param io_export_stat pointer to statistics object where export statistics are collected.
 *                        Errors and warnings during traversal are counted. Success shall be counted by io_element_writer_t.
 *                        Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_element_writer pointer to an external io_element_writer_t which is the output sink
 */
void io_export_flat_traversal_init( io_export_flat_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     data_stat_t *io_export_stat,
                                     io_element_writer_t *out_element_writer
                                   );

/*!
 *  \brief destroys the io_export_flat_traversal_t
 *
 *  \param this_ pointer to own object attributes
 */
void io_export_flat_traversal_destroy( io_export_flat_traversal_t *this_ );

/*!
 *  \brief prints all classifiers as flat list to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_flat_traversal_iterate_classifiers ( io_export_flat_traversal_t *this_ );

/*!
 *  \brief writes the classifier and feature and writes its outgoing relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier pointer to the classifier to process, most likely this is &((*this_).temp_classifier)
 *  \return -1 in case of error,
 *          0 in case of success.
 */
int io_export_flat_traversal_private_traverse_classifier ( io_export_flat_traversal_t *this_,
                                                           const data_classifier_t *classifier
                                                         );

/*!
 *  \brief iterates over features of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param node_data node data of the classifier of which the features are written, not NULL
 *  \return -1 in case of error, 0 in case of success.
 */
int io_export_flat_traversal_private_iterate_features ( io_export_flat_traversal_t *this_,
                                                         const data_node_set_t *node_data
                                                      );

/*!
 *  \brief iterates over relationships of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param node_data node data of the from-classifier of which the relationships are written, not NULL
 *  \return -1 in case of error, 0 in case of success.
 */
int io_export_flat_traversal_private_iterate_relationships ( io_export_flat_traversal_t *this_,
                                                             const data_node_set_t *node_data
                                                           );

/*!
 *  \brief gets the the end-objects of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param relation pointer to relationship that shall be analyzed, not NULL
 *  \param node_data the data set of a model-node where to look for already loaded data (otherwise, load from database)
 *  \param out_from_c (a copy of) the classifier at source end
 *  \param out_from_f (a copy of) the feature at source end; !is_valid() if no feature specified
 *  \param out_to_c (a copy of) the classifier at target end
 *  \param out_to_f (a copy of) the feature at target end; !is_valid() if no feature specified
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t io_export_flat_traversal_private_get_relationship_ends( io_export_flat_traversal_t *this_,
                                                                      const data_relationship_t *relation,
                                                                      const data_node_set_t *node_data,
                                                                      data_classifier_t *out_from_c,
                                                                      data_feature_t *out_from_f,
                                                                      data_classifier_t *out_to_c,
                                                                      data_feature_t *out_to_f
                                                                    );


#endif  /* IO_EXPORT_FLAT_TRAVERSAL_H */


/*
Copyright 2020-2021 Andreas Warnke

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
