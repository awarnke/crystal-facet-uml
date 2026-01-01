/* File: tvec_setup.h; Copyright and License: see below */

#ifndef TVEC_SETUP_H
#define TVEC_SETUP_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides functions to initialize the database before starting a test case
 */

#include "ctrl_controller.h"
#include "entity/data_row.h"

/*!
 *  \brief attributes of the tvec_setup_t
 */
struct tvec_setup_struct {
    ctrl_controller_t *controller;  /*!< pointer to external controller */
};

typedef struct tvec_setup_struct tvec_setup_t;

/*!
 *  \brief initializes the tvec_setup_t
 *
 *  \param this_ pointer to own object attributes
 *  \param controller the controller pointer
 */
static inline void tvec_setup_init( tvec_setup_t *this_, ctrl_controller_t *controller );

/*!
 *  \brief destroys the tvec_setup_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void tvec_setup_destroy( tvec_setup_t *this_ );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram_id id of the parent diagram or DATA_ROW_VOID to create a root diagram
 *  \param name name of the newly created element
 *  \return id of newly created diagram
 */
static data_row_t tvec_setup_diagram( tvec_setup_t *this_, data_row_t parent_diagram_id, const char* name );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the newly created element
 *  \return id of newly created classifier
 */
static data_row_t tvec_setup_classifier( tvec_setup_t *this_, const char* name );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the parent diagram to which to attach the classifier
 *  \param classifier_id id of the classifier which to attach to the diagram
 *  \param focused_feature_id id of the lifeline if applicable, DATA_ROW_VOID otherwise
 *  \return id of newly created diagramelement
 */
static data_row_t tvec_setup_diagramelement( tvec_setup_t *this_,
                                             data_row_t diagram_id,
                                             data_row_t classifier_id,
                                             data_row_t focused_feature_id
                                           );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_classifier_id id of the parent classifier to which to attach the feature
 *  \param name name of the newly created element
 *  \return id of newly created feature
 */
static data_row_t tvec_setup_feature( tvec_setup_t *this_, data_row_t parent_classifier_id, const char* name );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_id id of the source classifier from which the relation starts
 *  \param from_feature_id id of the source feature from which the relation starts or DATA_ROW_VOID if directly conected to classifier
 *  \param to_classifier_id id of the target classifier to which the relation goes to
 *  \param to_feature_id id of the target feature at which the relation ends or DATA_ROW_VOID if directly conected to classifier
 *  \param name name of the newly created element
 *  \return id of newly created relationship
 */
static data_row_t tvec_setup_relationship( tvec_setup_t *this_,
                                           data_row_t from_classifier_id,
                                           data_row_t from_feature_id,
                                           data_row_t to_classifier_id,
                                           data_row_t to_feature_id,
                                           const char* name
                                         );

#include "tvec_setup.inl"

#endif  /* TVEC_SETUP_H */


/*
Copyright 2020-2026 Andreas Warnke

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
