/* File: io_export_model_traversal.h; Copyright and License: see below */

#ifndef IO_EXPORT_MODEL_TRAVERSAL_H
#define IO_EXPORT_MODEL_TRAVERSAL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Traverses the model and writes the elements to an format writer
 *
 *  Source: db_reader(data_database_reader_t);
 *  Task: traverse the source model;
 *  Sink: io_format_writer_t
 */

#include "io_format_writer.h"
#include "set/data_node_set.h"
#include "storage/data_database_reader.h"
#include "data_table.h"
#include "universal_array_list.h"
#include <stdio.h>

/*!
 *  \brief constants for maximum values of io_export_model_traversal_t
 */
enum io_export_model_traversal_max_enum {
    IO_EXPORT_MODEL_TRAVERSAL_MAX_TOTAL_ELEMENTS = 16384,  /*!< maximum number of total classifiers and relationships to be exported */
    IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH = 0,  /*!< maximum number of recursive descend into containment children */
    IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH_FUTURE = 24,  /*!< maximum number of recursive descend into containment children */
};

/*!
 *  \brief attributes of the description writer
 */
struct io_export_model_traversal_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    io_format_writer_t *format_writer;  /*!< pointer to external io_format_writer_t which is the output sink */

    data_id_t written_id_set_buf[IO_EXPORT_MODEL_TRAVERSAL_MAX_TOTAL_ELEMENTS];  /*!< buffer for list of already exported element ids */
    universal_array_list_t written_id_set;  /*!< list of already exported element ids (extended when starting to export an element), used for classifiers and relationships */

    data_classifier_t temp_classifier;  /*!< own buffer for private use as data cache */
    data_node_set_t temp_node_data;  /*!< own buffer for private use as data cache */
};

typedef struct io_export_model_traversal_struct io_export_model_traversal_t;

/*!
 *  \brief initializes the io_export_model_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param format_writer pointer to an external io_format_writer_t which is the output sink
 */
void io_export_model_traversal_init( io_export_model_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     io_format_writer_t *format_writer
                                   );

/*!
 *  \brief destroys the io_export_model_traversal_t
 *
 *  \param this_ pointer to own object attributes
 */
void io_export_model_traversal_destroy( io_export_model_traversal_t *this_ );

/*!
 *  \brief prints all classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_walk_model_nodes ( io_export_model_traversal_t *this_ );

/*!
 *  \brief writes the node if it is not a duplicate.
 *
 *  While traversing, the written_id_set is extended.
 *  Classifiers that are contained in written_id_set or that are beyond max_recursion are not traversed.
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classiier, needed for xmi export
 *  \param classifier_id id of the classifier to process
 *  \param containment_relationship_id id of the containment relationship which caused the processing, DATA_ID_VOID if not applicable
 *  \param recursion_depth current number of tree depth.
 *  \return -1 in case of error,
 *          0 in case of success.
 */
int io_export_model_traversal_private_walk_unique_node ( io_export_model_traversal_t *this_,
                                                         data_classifier_type_t parent_type,
                                                         data_id_t classifier_id,
                                                         data_id_t containment_relationship_id,
                                                         unsigned int recursion_depth
                                                       );

/*!
 *  \brief writes the node, features and relationships and recusively descends the containment tree (graph) of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_type type of the parent classiier, needed for xmi export
 *  \param classifier_id id of the classifier to process
 *  \param recursion_depth current number of tree depth. Used to actively limit the recursive descend to max IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH.
 *  \return -1 in case of error,
 *          0 in case of success.
 *          If IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH limits the descent,
 *          or written_id_set prevents duplicate traversal of a classifier,
 *          0 is returned nonetheless.
 */
int io_export_model_traversal_private_walk_node ( io_export_model_traversal_t *this_,
                                                  data_classifier_type_t parent_type,
                                                  data_id_t classifier_id,
                                                  unsigned int recursion_depth
                                                );

/*!
 *  \brief iterates over features of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param node_data node data of the classifier of which the features are written, not NULL
 *  \return -1 in case of error, 0 in case of success.
 */
int io_export_model_traversal_private_iterate_node_features ( io_export_model_traversal_t *this_,
                                                              const data_node_set_t *node_data
                                                            );

/*!
 *  \brief iterates over relationships of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param node_data node data of the from-classifier of which the relationships are written, not NULL
 *  \return -1 in case of error, 0 in case of success.
 */
int io_export_model_traversal_private_iterate_node_relationships ( io_export_model_traversal_t *this_,
                                                                   const data_node_set_t *node_data
                                                                 );

#endif  /* IO_EXPORT_MODEL_TRAVERSAL_H */


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
