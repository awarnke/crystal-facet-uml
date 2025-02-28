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
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "storage/data_feature_iterator.h"
#include "storage/data_relationship_iterator.h"
#include "entity/data_table.h"
#include "u8list/universal_array_list.h"
#include "u8/u8_error.h"
#include <stdio.h>
#include <stdbool.h>

/*!
 *  \brief attributes of the flat-list traversal
 *
 *  Lifecycle: A database traversal shall perform a single export operation only.
 *  It may be initialized before one export operation and be destroyed afterwards.
 */
struct io_export_flat_traversal_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    io_element_writer_t *element_writer;  /*!< pointer to external io_element_writer_t which is the output sink */

    data_classifier_t temp_classifier;  /*!< own buffer for private use as data cache */
    data_relationship_t temp_relationship;  /*!< own buffer for private use as data cache */
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
void io_export_flat_traversal_init ( io_export_flat_traversal_t *this_,
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
 *  \param hierarchical true if the iterator shall start with classifiers without parent
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_flat_traversal_iterate_classifiers ( io_export_flat_traversal_t *this_, bool hierarchical );

/*!
 *  \brief writes the classifier and feature and writes its outgoing relationships
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier pointer to the classifier to process, most likely this is &((*this_).temp_classifier)
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_flat_traversal_private_traverse_classifier ( io_export_flat_traversal_t *this_,
                                                                  const data_classifier_t *classifier
                                                                );

/*!
 *  \brief iterates over features of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier pointer to the classifier of which the features shall be processed
 *  \param feature_iterator iterator over features to be written, not NULL
 *  \return U8_ERROR_NONE in case of success.
 */
u8_error_t io_export_flat_traversal_private_iterate_features ( io_export_flat_traversal_t *this_,
                                                               const data_classifier_t *classifier,
                                                               data_feature_iterator_t *feature_iterator
                                                             );

/*!
 *  \brief iterates over relationships of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier pointer to the classifier of which the relationships shall be processed
 *  \param relationship_iterator iterator over relationships to be written, not NULL
 *  \return U8_ERROR_NONE in case of success.
 */
u8_error_t io_export_flat_traversal_private_iterate_relationships ( io_export_flat_traversal_t *this_,
                                                                    const data_classifier_t *classifier,
                                                                    data_relationship_iterator_t *relationship_iterator
                                                                  );

/*!
 *  \brief gets the the end-objects of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier pointer to the classifier of which the relationship ends shall be loaded
 *  \param relation pointer to relationship that shall be analyzed, not NULL
 *  \param out_from_c (a copy of) the classifier at source end
 *  \param out_from_f (a copy of) the feature at source end; !is_valid() if no feature specified
 *  \param out_to_c (a copy of) the classifier at target end
 *  \param out_to_f (a copy of) the feature at target end; !is_valid() if no feature specified
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_flat_traversal_private_get_relationship_ends ( io_export_flat_traversal_t *this_,
                                                                    const data_classifier_t *classifier,
                                                                    const data_relationship_t *relation,
                                                                    data_classifier_t *out_from_c,
                                                                    data_feature_t *out_from_f,
                                                                    data_classifier_t *out_to_c,
                                                                    data_feature_t *out_to_f
                                                                  );

#endif  /* IO_EXPORT_FLAT_TRAVERSAL_H */


/*
Copyright 2020-2025 Andreas Warnke

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
