/* File: test_env_setup_data.h; Copyright and License: see below */

#ifndef TEST_ENV_SETUP_DATA_H
#define TEST_ENV_SETUP_DATA_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides functions to initialize the database before starting a test case
 */

#include "ctrl_controller.h"
#include "data_row_id.h"

/*!
 *  \brief helper function to initialize the database
 * 
 *  \param parent_diagram_id id of the parent diagram or DATA_ROW_ID_VOID to create a root diagram
 *  \param name name of the newly created element
 *  \param controller controller object used to create the new element
 *  \return id of newly created diagram
 */
static data_row_id_t test_env_setup_data_create_diagram( data_row_id_t parent_diagram_id, const char* name, ctrl_controller_t *controller );

/*!
 *  \brief helper function to initialize the database
 * 
 *  \param into_diagram_id id of the parent diagram to which to attach the classifier
 *  \param name name of the newly created element
 *  \param controller controller object used to create the new element
 *  \return id of newly created classifier
 */
static data_row_id_t test_env_setup_data_create_classifier( data_row_id_t into_diagram_id, const char* name, ctrl_controller_t *controller );

/*!
 *  \brief helper function to initialize the database
 * 
 *  \param parent_classifier_id id of the parent classifier to which to attach the feature
 *  \param name name of the newly created element
 *  \param controller controller object used to create the new element
 *  \return id of newly created feature
 */
static data_row_id_t test_env_setup_data_create_feature( data_row_id_t parent_classifier_id, 
                                                         const char* name,
                                                         ctrl_controller_t *controller
                                                       );

/*!
 *  \brief helper function to initialize the database
 * 
 *  \param from_classifier_id id of the source classifier from which the relation starts
 *  \param from_classifier_id id of the source feature from which the relation starts or DATA_ROW_ID_VOID if directly conected to classifier
 *  \param to_classifier_id id of the target classifier to which the relation goes to
 *  \param to_feature_id id of the target feature at which the relation ends or DATA_ROW_ID_VOID if directly conected to classifier
 *  \param name name of the newly created element
 *  \param controller controller object used to create the new element
 *  \return id of newly created relationship
 */
static data_row_id_t test_env_setup_data_create_relationship( data_row_id_t from_classifier_id,  
                                                              data_row_id_t from_feature_id,
                                                              data_row_id_t to_classifier_id,  
                                                              data_row_id_t to_feature_id,
                                                              const char* name, 
                                                              ctrl_controller_t *controller
                                                            );

#include "test_env_setup_data.inl"

#endif  /* TEST_ENV_SETUP_DATA_H */


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
