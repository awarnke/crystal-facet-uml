/* File: ctrl_classifier_policy_enforcer.h; Copyright and License: see below */

#ifndef CTRL_CLASSIFIER_POLICY_ENFORCER_H
#define CTRL_CLASSIFIER_POLICY_ENFORCER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Enforces application-level conditions.
 *
 *  While application level conditions are more a nice to have feature to not irritate the user
 *  and to support in creating consistent diagrams,
 *  the ctrl_consistency_checker checks that the model in the database is valid,
 *  e.g. relations link existing objects instead of invalid ids.
 */

#include "u8/u8_error.h"
#include "storage/data_database_reader.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct ctrl_classifier_controller_struct;
struct ctrl_diagram_controller_struct;

/*!
 *  \brief constants of ctrl_classifier_policy_enforcer_t
 */
enum ctrl_classifier_policy_enforcer_const_enum {
    CTRL_CLASSIFIER_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES = 128,  /*!< maximum number of diagramelements of a classifier */
};

/*!
 *  \brief all data attributes needed for the policy enforcer
 *
 *  The policy enforcer works on a similar abstraction level as the gui module.
 *  Therefore it references the same ctrl and data objects as the gui module.
 */
struct ctrl_classifier_policy_enforcer_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    struct ctrl_classifier_controller_struct *clfy_ctrl;  /*!< pointer to external classifier controller */
    struct ctrl_diagram_controller_struct *diag_ctrl;  /*!< pointer to external diagram controller */

    data_diagramelement_t private_temp_diagele_buf[CTRL_CLASSIFIER_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES];
};

typedef struct ctrl_classifier_policy_enforcer_struct ctrl_classifier_policy_enforcer_t;

/*!
 *  \brief initializes the ctrl_classifier_policy_enforcer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param clfy_ctrl pointer to classifier controller to create and delete features and to delete relationships
 *  \param diag_ctrl pointer to diagram controller to modify diagramelements
 */
void ctrl_classifier_policy_enforcer_init ( ctrl_classifier_policy_enforcer_t *this_,
                                            data_database_reader_t *db_reader,
                                            struct ctrl_classifier_controller_struct *clfy_ctrl,
                                            struct ctrl_diagram_controller_struct *diag_ctrl
                                          );

/*!
 *  \brief destroys the ctrl_classifier_policy_enforcer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_classifier_policy_enforcer_destroy ( ctrl_classifier_policy_enforcer_t *this_ );

/* ================================ ENTRY POINTS ================================ */

/*!
 *  \brief executes policies involved in deleting a feature.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_feature data of the deleted feature.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_classifier_policy_enforcer_post_delete_feature ( ctrl_classifier_policy_enforcer_t *this_,
                                                                               const data_feature_t *deleted_feature
                                                                             );

/* ================================ LIFELINES ================================ */

/*!
 *  \brief executes policies involved in deleting a feature.
 *
 *  Current rules are:
 *  - when deleting a feature
 *    all diagramelements that reference the feature shall be modified to not reference the feature anymore.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_feature data of the deleted feature.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t ctrl_classifier_policy_enforcer_private_unlink_lifeline ( ctrl_classifier_policy_enforcer_t *this_,
                                                                     const data_feature_t *deleted_feature
                                                                   );

#include "ctrl_classifier_policy_enforcer.inl"

#endif  /* CTRL_CLASSIFIER_POLICY_ENFORCER_H */


/*
Copyright 2018-2022 Andreas Warnke

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
