/* File: test_env_setup_data.h; Copyright and License: see below */

#ifndef TEST_ENV_SETUP_DATA_H
#define TEST_ENV_SETUP_DATA_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides functions to initialize the database before starting a test case
 */

#include "ctrl_controller.h"
#include "entity/data_row_id.h"

/*!
 *  \brief attributes of the test_env_setup_t
 */
struct test_env_setup_struct {
    ctrl_controller_t *controller;  /*!< pointer to external controller */
};

typedef struct test_env_setup_struct test_env_setup_t;

/*!
 *  \brief initializes the test_env_setup_t
 *
 *  \param this_ pointer to own object attributes
 *  \param controller the controller pointer
 */
static inline void test_env_setup_init( test_env_setup_t *this_, ctrl_controller_t *controller );

/*!
 *  \brief destroys the test_env_setup_t
 *
 *  \param this_ pointer to own object attributes
 */
static inline void test_env_setup_destroy( test_env_setup_t *this_ );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram_id id of the parent diagram or DATA_ROW_ID_VOID to create a root diagram
 *  \param name name of the newly created element
 *  \return id of newly created diagram
 */
static data_row_id_t test_env_setup_data_create_diagram( test_env_setup_t *this_,
                                                         data_row_id_t parent_diagram_id,
                                                         const char* name
                                                       );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name name of the newly created element
 *  \return id of newly created classifier
 */
static data_row_id_t test_env_setup_data_create_classifier( test_env_setup_t *this_,
                                                            const char* name
                                                          );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the parent diagram to which to attach the classifier
 *  \param classifier_id id of the classifier which to attach to the diagram
 *  \param focused_feature_id id of the lifeline if applicable, DATA_ROW_ID_VOID otherwise
 *  \return id of newly created diagramelement
 */
static data_row_id_t test_env_setup_data_create_diagramelement( test_env_setup_t *this_,
                                                                data_row_id_t diagram_id,
                                                                data_row_id_t classifier_id,
                                                                data_row_id_t focused_feature_id
                                                              );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_classifier_id id of the parent classifier to which to attach the feature
 *  \param name name of the newly created element
 *  \return id of newly created feature
 */
static data_row_id_t test_env_setup_data_create_feature( test_env_setup_t *this_,
                                                         data_row_id_t parent_classifier_id,
                                                         const char* name
                                                       );

/*!
 *  \brief helper function to initialize the database
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_id id of the source classifier from which the relation starts
 *  \param from_feature_id id of the source feature from which the relation starts or DATA_ROW_ID_VOID if directly conected to classifier
 *  \param to_classifier_id id of the target classifier to which the relation goes to
 *  \param to_feature_id id of the target feature at which the relation ends or DATA_ROW_ID_VOID if directly conected to classifier
 *  \param name name of the newly created element
 *  \return id of newly created relationship
 */
static data_row_id_t test_env_setup_data_create_relationship( test_env_setup_t *this_,
                                                              data_row_id_t from_classifier_id,
                                                              data_row_id_t from_feature_id,
                                                              data_row_id_t to_classifier_id,
                                                              data_row_id_t to_feature_id,
                                                              const char* name
                                                            );

#include "test_env_setup_data.inl"

#endif  /* TEST_ENV_SETUP_DATA_H */


/*
Copyright 2020-2024 Andreas Warnke

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
