/* File: tvec_add.h; Copyright and License: see below */

#ifndef TVEC_ADD_H
#define TVEC_ADD_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides functions to initialize the database before starting a test case
 */

#include "storage/data_database_writer.h"
#include "entity/data_row.h"

/*!
 *  \brief attributes of the tvec_add_t
 */
struct tvec_add_struct {
    data_database_writer_t *db_writer;  /*!< pointer to external data_database_writer_t */
};

typedef struct tvec_add_struct tvec_add_t;

/*!
 *  \brief initializes the tvec_add_t
 *
 *  \param this_ pointer to own object attributes
 *  \param db_writer the db_writer pointer
 */
static inline void tvec_add_init( tvec_add_t *this_, data_database_writer_t *db_writer );

/*!
 *  \brief destroys the tvec_add_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void tvec_add_destroy( tvec_add_t *this_ );

/*!
 *  \brief helper function to initialize the database by adding a diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram_id id of the parent diagram or DATA_ROW_VOID to create a root diagram
 *  \param name name of the newly created element
 *  \param stereotype stereotype name of the new element
 *  \return id of newly created diagram
 */
static inline data_row_t tvec_add_diagram( tvec_add_t *this_,
                                           data_row_t parent_diagram_id,
                                           const char* name,
                                           const char* stereotype
                                         );

/*!
 *  \brief helper function to initialize the database by adding a diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the parent diagram to which to attach the classifier
 *  \param classifier_id id of the classifier which to show in the diagram
 *  \return id of newly created diagramelement
 */
static inline data_row_t tvec_add_diagramelement( tvec_add_t *this_,
                                                  data_row_t diagram_id,
                                                  data_row_t classifier_id
                                                );

/*!
 *  \brief helper function to initialize the database by adding a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the newly created element
 *  \param classifier_type type of the newly created element
 *  \param stereotype stereotype name of the new element
 *  \return id of newly created classifier
 */
static inline data_row_t tvec_add_classifier( tvec_add_t *this_,
                                              const char* name,
                                              data_classifier_type_t classifier_type,
                                              const char* stereotype
                                            );

/*!
 *  \brief helper function to initialize the database by adding a feature
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to which to attach the feature
 *  \param name name of the newly created element
 *  \param stereotype stereotype name of the new element
 *  \return id of newly created feature
 */
static inline data_row_t tvec_add_feature( tvec_add_t *this_,
                                           data_row_t classifier_id,
                                           const char* name,
                                           const char* stereotype
                                         );

/*!
 *  \brief helper function to initialize the database
 *
 *  This function adds 2 objects:
 *  a feature of type DATA_FEATURE_TYPE_LIFELINE
 *  and a diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the parent diagram to which to attach the classifier
 *  \param classifier_id id of the classifier to which to attach the feature
 *  \return id of newly created feature; The id of the diagramelement is not returned.
 */
static inline data_row_t tvec_add_lifeline( tvec_add_t *this_, data_row_t diagram_id, data_row_t classifier_id );

/*!
 *  \brief helper function to initialize the database by adding a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_id id of the classifier from which to start the relationship
 *  \param from_feature_id id of the feature from which to start the relationship
 *  \param to_classifier_id id of the classifier to which to link the relationship
 *  \param to_feature_id id of the feature to which to link the relationship
 *  \param rel_type type of the newly created element
 *  \param name name of the newly created element
 *  \param stereotype stereotype name of the new element
 *  \return id of newly created relationship
 */
static inline data_row_t tvec_add_relationship( tvec_add_t *this_,
                                                data_row_t from_classifier_id,
                                                data_row_t from_feature_id,
                                                data_row_t to_classifier_id,
                                                data_row_t to_feature_id,
                                                data_relationship_type_t rel_type,
                                                const char* name,
                                                const char* stereotype
                                               );

#include "tvec_add.inl"

#endif  /* TVEC_ADD_H */


/*
Copyright 2023-2026 Andreas Warnke

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
