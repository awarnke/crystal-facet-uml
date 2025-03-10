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

/*!
 *  \brief constants for maximum values of io_export_model_traversal_t
 */
enum io_export_model_traversal_max_enum {
    IO_EXPORT_MODEL_TRAVERSAL_MAX_TOTAL_ELEMENTS = 65536,  /*!< maximum number of total classifiers and relationships to be exported */
    IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH = 32,  /*!< maximum number of recursive descend into containment children */
};

/*!
 *  \brief attributes of the model traversal
 *
 *  Lifecycle: A database traversal shall perform a single export operation only.
 *  It may be initialized before one export operation and be destroyed afterwards.
 */
struct io_export_model_traversal_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    io_element_writer_t *element_writer;  /*!< pointer to external io_element_writer_t which is the output sink */

    io_export_interaction_traversal_t interaction_helper;  /* !< instance of own io_export_interaction_traversal to help with interaction exports */

    data_id_t written_id_set_buf[IO_EXPORT_MODEL_TRAVERSAL_MAX_TOTAL_ELEMENTS];  /*!< buffer for list of already exported element ids */
    universal_array_list_t written_id_set;  /*!< list of already exported element ids (extended when starting to export an element), used for classifiers and relationships and lifelines(which are features) */

    data_classifier_t temp_host;  /*!< own buffer for private use as data cache */
    data_classifier_t temp_classifier;  /*!< own buffer for private use as data cache */
    data_relationship_t temp_relationship;  /*!< own buffer for private use as data cache */
    data_classifier_t temp_from_classifier;  /*!< own buffer for private use as data cache */
    data_feature_t temp_from_feature;  /*!< own buffer for private use as data cache */
    data_classifier_t temp_to_classifier;  /*!< own buffer for private use as data cache */
    data_feature_t temp_to_feature;  /*!< own buffer for private use as data cache */
};

typedef struct io_export_model_traversal_struct io_export_model_traversal_t;

/*!
 *  \brief initializes the io_export_model_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param input_data pointer to an external buffer for private use as data cache by interaction_helper
 *  \param io_export_stat pointer to statistics object where export statistics are collected.
 *                        Errors and warnings during traversal are counted. Success shall be counted by io_element_writer_t.
 *                        Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_element_writer pointer to an external io_element_writer_t which is the output sink
 */
void io_export_model_traversal_init( io_export_model_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     data_visible_set_t *input_data,
                                     data_stat_t *io_export_stat,
                                     io_element_writer_t *out_element_writer
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
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_model_traversal_walk_model_nodes ( io_export_model_traversal_t *this_ );

/*!
 *  \brief writes the node if it is not a duplicate. writes its relationships if they are not duplicates.
 *
 *  While traversing, the written_id_set is extended.
 *  Classifiers that are contained in written_id_set or that are beyond max_recursion are not traversed.
 *
 *  \param this_ pointer to own object attributes
 *  \param host_id the hosting parent classifier, needed for xmi export; DATA_ID_VOID on top-level of document
 *  \param containment_relationship_id id of the containment relationship which caused the processing, DATA_ID_VOID if not applicable
 *  \param classifier_id id of the classifier to process
 *  \param recursion_depth current number of tree depth.
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_model_traversal_private_walk_node ( io_export_model_traversal_t *this_,
                                                         data_id_t host_id,
                                                         data_id_t containment_relationship_id,
                                                         data_id_t classifier_id,
                                                         unsigned int recursion_depth
                                                       );

/*!
 *  \brief writes the first half of the node
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export
 *  \param classifier pointer to the classifier which to process
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_model_traversal_private_begin_node ( io_export_model_traversal_t *this_,
                                                          data_classifier_type_t host_type,
                                                          const data_classifier_t *classifier
                                                        );

/*!
 *  \brief retrieves the relationships of type DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier pointer to the classifier which to process
 *  \param io_contained_classifiers set of classifiers, contained classifiers are appended. This set shall be initialized when calling this function.
 *  \param io_containment_relations set of relationships, containment relationships are appended. This set shall be initialized when calling this function.
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_model_traversal_private_get_containments ( io_export_model_traversal_t *this_,
                                                                const data_classifier_t *classifier,
                                                                data_small_set_t *io_contained_classifiers,
                                                                data_small_set_t *io_containment_relations
                                                              );

/*!
 *  \brief recusively descends the containment tree (graph) of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param host_id the hosting parent classifier, needed for xmi export
 *  \param contained_classifiers set of contained classifiers.
 *  \param containment_relations set of containment relationships.
 *  \param recursion_depth current number of tree depth. Used to actively limit the recursive descend to max IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH.
 *  \return U8_ERROR_NONE in case of success.
 *          If IO_EXPORT_MODEL_TRAVERSAL_MAX_TREE_DEPTH limits the descent,
 *          or written_id_set prevents duplicate traversal of a classifier,
 *          U8_ERROR_NONE is returned nonetheless.
 */
u8_error_t io_export_model_traversal_private_walk_containments ( io_export_model_traversal_t *this_,
                                                                 data_id_t host_id,
                                                                 const data_small_set_t *contained_classifiers,
                                                                 const data_small_set_t *containment_relations,
                                                                 unsigned int recursion_depth
                                                               );

/*!
 *  \brief writes the second half of the node
 *
 *  \param this_ pointer to own object attributes
 *  \param host_type type of the hosting parent classifier, needed for xmi export
 *  \param classifier pointer to the classifier which to process
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_model_traversal_private_end_node ( io_export_model_traversal_t *this_,
                                                        data_classifier_type_t host_type,
                                                        const data_classifier_t *classifier
                                                      );

/*!
 *  \brief iterates over features of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier pointer to the classifier of which the features shall be processed
 *  \return U8_ERROR_NONE in case of success.
 */
u8_error_t io_export_model_traversal_private_iterate_node_features ( io_export_model_traversal_t *this_,
                                                                     const data_classifier_t *classifier
                                                                   );

/*!
 *  \brief iterates over relationships of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param nested_to_foreign_node true, if nested to a foreign node, e.g. the outer model,
 *         false is nested to node_data.
 *  \param host the hosting parent classifier, needed for xmi export; is NULL on top-level of document
 *  \param classifier pointer to the classifier of which the relationships shall be processed
 *  \return U8_ERROR_NONE in case of success.
 */
u8_error_t io_export_model_traversal_private_iterate_node_relationships ( io_export_model_traversal_t *this_,
                                                                          bool nested_to_foreign_node,
                                                                          const data_classifier_t *host,
                                                                          const data_classifier_t *classifier
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
u8_error_t io_export_model_traversal_private_get_relationship_ends( io_export_model_traversal_t *this_,
                                                                    const data_classifier_t *classifier,
                                                                    const data_relationship_t *relation,
                                                                    data_classifier_t *out_from_c,
                                                                    data_feature_t *out_from_f,
                                                                    data_classifier_t *out_to_c,
                                                                    data_feature_t *out_to_f
                                                                  );

/*!
 *  \brief checks in which interaction-diagrams the node is used and fakes interaction-model-objects.
 *
 *  \param this_ pointer to own object attributes
 *  \param nesting_type type of the nesting-parent classifier
 *  \param classifier pointer to the classifier of which the interactions are faked, not NULL
 *  \return U8_ERROR_NONE in case of success.
 */
u8_error_t io_export_model_traversal_private_fake_interactions_of_node ( io_export_model_traversal_t *this_,
                                                                         data_classifier_type_t nesting_type,
                                                                         const data_classifier_t *classifier
                                                                       );

#endif  /* IO_EXPORT_MODEL_TRAVERSAL_H */


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
