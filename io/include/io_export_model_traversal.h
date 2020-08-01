/* File: io_export_model_traversal.h; Copyright and License: see below */

#ifndef IO_EXPORT_MODEL_TRAVERSAL_H
#define IO_EXPORT_MODEL_TRAVERSAL_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Traverses the model or a model-part(diagram) and writes the elements to an format writer
 *
 *  Source: db_reader(data_database_reader_t);
 *  Task: traverse the source model(-part);
 *  Sink: io_format_writer_t
 */

#include "io_format_writer.h"
#include "io_filter_flag.h"
#include "set/data_visible_set.h"
#include "storage/data_database_reader.h"
#include "data_table.h"
#include "data_rules.h"
#include "universal_array_list.h"
#include <stdio.h>

/*!
 *  \brief constants for maximum values of io_export_model_traversal_t
 */
enum io_export_model_traversal_max_enum {
    IO_EXPORT_MODEL_TRAVERSAL_MAX_TOTAL_CLASSIFIERS = 16384,  /*!< maximum number of total classifiers to be exported */
    IO_EXPORT_MODEL_TRAVERSAL_MAX_FEATURES = 64,  /*!< maximum number of features linked to one classifiers, incl. lifelines */
    IO_EXPORT_MODEL_TRAVERSAL_MAX_RELATIONSHIPS = 256,  /*!< maximum number of relationships linked to one classifiers, incl. scenario-based */
};

/*!
 *  \brief attributes of the description writer
 */
struct io_export_model_traversal_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    data_visible_set_t *input_data;  /*!< pointer to an external buffer for private use as data cache */
    data_rules_t filter_rules;  /*!< own instance of uml and sysml consistency rules */
    io_filter_flag_t filter_flags;  /*!< flags indicating which elements shall be exported */
    io_format_writer_t *format_writer;  /*!< pointer to external io_format_writer_t which is the output sink */

    data_id_t written_id_set_buf[IO_EXPORT_MODEL_TRAVERSAL_MAX_TOTAL_CLASSIFIERS];  /*!< buffer for list of already exported element ids */
    universal_array_list_t written_id_set;  /*!< list of already exported element ids */
    
    data_classifier_t temp_classifier;  /*!< temporary buffer to store one classifier (uninitialized) */
    data_feature_t temp_features[IO_EXPORT_MODEL_TRAVERSAL_MAX_FEATURES];  /*!< temporary buffer to store features of one classifier (uninitialized) */
    data_relationship_t temp_relationships[IO_EXPORT_MODEL_TRAVERSAL_MAX_RELATIONSHIPS];  /*!< temporary buffer to store relationships of one classifier (uninitialized) */
};

typedef struct io_export_model_traversal_struct io_export_model_traversal_t;

/*!
 *  \brief initializes the io_export_model_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param input_data pointer to an external buffer for private use as data cache
 *  \param filter_flags flags indicating which elements shall be exported
 *  \param format_writer pointer to an external io_format_writer_t which is the output sink
 */
void io_export_model_traversal_init( io_export_model_traversal_t *this_,
                                     data_database_reader_t *db_reader,
                                     data_visible_set_t *input_data,
                                     io_filter_flag_t filter_flags,
                                     io_format_writer_t *format_writer
                                   );

/*!
 *  \brief destroys the io_export_model_traversal_t
 *
 *  \param this_ pointer to own object attributes
 */
void io_export_model_traversal_destroy( io_export_model_traversal_t *this_ );

/*!
 *  \brief prints names and descriptions of the diagram and contained classifiers to the output stream
 *
 *  Note: When finished, the caller may recurse to chilren diagrams.
 *  Finally, io_export_model_traversal_end_diagram has to be called.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram which to process for export
 *  \param diagram_file_base_name filename of the diagram without extension
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_begin_and_walk_diagram ( io_export_model_traversal_t *this_,
                                                       data_id_t diagram_id,
                                                       const char *diagram_file_base_name
                                                     );

/*!
 *  \brief prints the end of a diagram section
 *
 *  \param this_ pointer to own object attributes
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_end_diagram ( io_export_model_traversal_t *this_ );

/*!
 *  \brief prints all classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_walk_model ( io_export_model_traversal_t *this_ );

/*!
 *  \brief iterates over features of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier of which the features are written
 *  \return -1 in case of error, 0 in case of success.
 */
int io_export_model_traversal_private_iterate_features ( io_export_model_traversal_t *this_,
                                                         data_id_t classifier_id
                                                       );

/*!
 *  \brief iterates over relationships of a classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier of which the relationships are written
 *  \param io_contained_classifiers data set to which contained classifiers are added
 *  \return -1 in case of error, 0 in case of success.
 */
int io_export_model_traversal_private_iterate_relationships ( io_export_model_traversal_t *this_,
                                                              data_id_t classifier_id,
                                                              data_small_set_t *io_contained_classifiers
                                                            );

/*!
 *  \brief recusively descends the containment tree (graph) of a classifier.
 *
 *  While traversing, the written_id_set is extended.
 *  Classifiers that are contained in written_id_set or that are beyond max_recursion are not traversed.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier of which the containment relations are traversed
 *  \param max_recursion maximum number of tree depth. Use e.g. 16 to actively limit the model, use 64 to catch (nearly) only error cases.
 *  \return -1 in case of error, 0 in case of success.
 *          If max_recursion limits the descent, or written_id_set prevents duplicate traversal of a classifier, 0 is returned nonetheless.
 */
int io_export_model_traversal_private_descend_containments ( io_export_model_traversal_t *this_,
                                                             data_id_t classifier_id,
                                                             unsigned int max_recursion
                                                           );

/*!
 *  \brief prints names and descriptions of the classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_private_write_classifiers ( io_export_model_traversal_t *this_ );

/*!
 *  \brief prints names and descriptions of the classifier to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier of which the features are written
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_private_write_features_of_classifier ( io_export_model_traversal_t *this_,
                                                                     data_id_t classifier_id
                                                                   );

/*!
 *  \brief prints names and descriptions of the relationships to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_id id of the classifier of which the relationships are written
 *  \return -1 in case of error, 0 in case of success
 */
int io_export_model_traversal_private_write_relations_of_classifier ( io_export_model_traversal_t *this_,
                                                                      data_id_t from_classifier_id
                                                                    );

#endif  /* IO_EXPORT_MODEL_TRAVERSAL_H */


/*
Copyright 2016-2020 Andreas Warnke

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
