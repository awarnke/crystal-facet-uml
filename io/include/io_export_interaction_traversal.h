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

#include "io_element_writer.h"
#include "xmi/xmi_interaction_writer.h"
#include "set/data_visible_set.h"
#include "set/data_stat.h"
#include "storage/data_database_reader.h"
#include "entity/data_table.h"
#include "data_rules.h"
#include "u8list/universal_array_list.h"
#include "u8/u8_error.h"
#include <stdio.h>

/*!
 *  \brief attributes of the interaction traversal
 *
 *  Lifecycle: A database traversal shall perform a single export operation only.
 *  It may be initialized before one export operation and be destroyed afterwards.
 */
struct io_export_interaction_traversal_struct {
    data_database_reader_t *db_reader;  /* !< pointer to external database reader */
    data_visible_set_t *input_data;  /*!< pointer to an external buffer for private use as data cache */
    data_rules_t filter_rules;  /*!< own instance of uml and sysml consistency rules */
    universal_array_list_t *written_id_set;  /*!< pointer to external list of already exported element ids */
    data_stat_t *export_stat;  /*!< pointer to external statistics object where export statistics are collected */
    io_element_writer_t *element_writer;  /*!< pointer to external io_element_writer_t which is the regular output sink */
    data_classifier_t fake_interaction_classifier;  /*!< a fake classifier of type DATA_CLASSIFIER_TYPE_INTERACTION */
    data_feature_t fake_lifeline_feature;  /*!< a fake feature of type DATA_FEATURE_TYPE_LIFELINE */
};

typedef struct io_export_interaction_traversal_struct io_export_interaction_traversal_t;

/*!
 *  \brief initializes the io_export_interaction_traversal_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param input_data pointer to an external buffer for private use as data cache
 *  \param io_written_id_set pointer to external list of already exported element ids
 *  \param io_export_stat pointer to statistics object where export statistics are collected.
 *                        Errors and warnings during traversal are counted. Success shall be counted by io_element_writer_t.
 *                        Statistics are only added, *io_stat shall be initialized by caller.
 *  \param out_element_writer pointer to an io_element_writer_t which is the output sink
 */
void io_export_interaction_traversal_init( io_export_interaction_traversal_t *this_,
                                           data_database_reader_t *db_reader,
                                           data_visible_set_t *input_data,
                                           universal_array_list_t *io_written_id_set,
                                           data_stat_t *io_export_stat,
                                           io_element_writer_t *out_element_writer
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
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_interaction_traversal_iterate_classifier_occurrences ( io_export_interaction_traversal_t *this_,
                                                                            data_classifier_type_t nesting_type,
                                                                            data_id_t classifier_id
                                                                          );

/*!
 *  \brief prints the end of a diagram section
 *
 *  \param this_ pointer to own object attributes
 *  \param nesting_type type of the nesting parent classifier
 *  \param diagram_id id of the diagram which to process for export
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_interaction_traversal_private_walk_diagram ( io_export_interaction_traversal_t *this_,
                                                                  data_classifier_type_t nesting_type,
                                                                  data_id_t diagram_id
                                                                );

/*!
 *  \brief prints names and descriptions of the classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data of which the classifiers are written, not NULL
 *  \param fake_interaction fake classifier of type DATA_CLASSIFIER_TYPE_INTERACTION derived from the diagram
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_interaction_traversal_private_iterate_diagram_classifiers ( io_export_interaction_traversal_t *this_,
                                                                                 const data_visible_set_t *diagram_data,
                                                                                 const data_classifier_t *fake_interaction
                                                                               );

/*!
 *  \brief prints names and descriptions of the focused feature to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data that contains the classifier of which the features are written, not NULL
 *  \param focused_feature_id id of the  focused feature (lifeline) which shall be written
 *  \param fake_interaction fake classifier of type DATA_CLASSIFIER_TYPE_INTERACTION derived from the diagram
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_interaction_traversal_private_look_for_focused_feature ( io_export_interaction_traversal_t *this_,
                                                                              const data_visible_set_t *diagram_data,
                                                                              data_id_t focused_feature_id,
                                                                              const data_classifier_t *fake_interaction
                                                                            );

/*!
 *  \brief prints names and descriptions of the relationships to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data that contains the from-classifier of which the relationships are written, not NULL
 *  \param from_classifier_id id of the classifier of which the relationships are written
 *  \param focused_feature_id id of the focused feature (lifeline) of which the relationships are written
 *  \param fake_interaction fake classifier of type DATA_CLASSIFIER_TYPE_INTERACTION derived from the diagram
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_interaction_traversal_private_iterate_feature_relationships ( io_export_interaction_traversal_t *this_,
                                                                                   const data_visible_set_t *diagram_data,
                                                                                   data_id_t from_classifier_id,
                                                                                   data_id_t focused_feature_id,
                                                                                   const data_classifier_t *fake_interaction
                                                                                 );

/*!
 *  \brief converts the data fields of an interaction diagram to a fake classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param interaction_diagram diagram data that contains the interaction information
 *  \param[out] out_fake_classifier the classifier to be filled with data from interaction_diagram
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t io_export_interaction_traversal_private_fake_interaction ( io_export_interaction_traversal_t *this_,
                                                                      const data_diagram_t *interaction_diagram,
                                                                      data_classifier_t *out_fake_classifier
                                                                    );

/*!
 *  \brief iterates over features of a classifier.
 *
 *  These are written to (*this_).element_writer except for lifelines.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_data diagram data that contains the parent_classifier of which the features are written, not NULL
 *  \param parent_classifier the classifier of which the features are written, not NULL
 *  \return U8_ERROR_NONE in case of success.
 */
u8_error_t io_export_interaction_traversal_private_iterate_node_features( io_export_interaction_traversal_t *this_,
                                                                          const data_visible_set_t *diagram_data,
                                                                          const data_classifier_t *parent_classifier
                                                                        );


#endif  /* IO_EXPORT_INTERACTION_TRAVERSAL_H */


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
