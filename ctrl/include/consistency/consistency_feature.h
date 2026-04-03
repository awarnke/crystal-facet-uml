/* File: consistency_feature.h; Copyright and License: see below */

#ifndef CONSISTENCY_FEATURE_H
#define CONSISTENCY_FEATURE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Enforces application-level conditions.
 *
 *  While application level conditions are more a nice to have feature to not irritate the user
 *  and to support in creating consistent diagrams,
 *  the ctrl_consistency_checker checks that the model in the database is valid,
 *  e.g. relations link to existing objects instead of invalid ids.
 */

#include "u8/u8_error.h"
#include "storage/data_database_reader.h"
#include "data_rules.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct ctrl_classifier_controller_struct;
struct ctrl_diagram_controller_struct;

/*!
 *  \brief all data attributes needed for the policy enforcer
 *
 *  The policy enforcer works on a similar abstraction level as the gui module.
 *  Therefore it references the same ctrl and data objects as the gui module.
 */
struct consistency_feature_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    struct ctrl_classifier_controller_struct *clfy_ctrl;  /*!< pointer to external classifier controller */
    data_rules_t rules;  /*!< visibility rules for features, depending on diagram type */

    data_diagram_t temp_diagram_buf;  /*!< be aware of reentrancy by recursion! */
    data_feature_t temp_feature_buf;  /*!< be aware of reentrancy by recursion! */
    data_diagramelement_t temp_diagramelement_buf;  /*!< be aware of reentrancy by recursion! */
};

typedef struct consistency_feature_struct consistency_feature_t;

/*!
 *  \brief initializes the consistency_feature_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param clfy_ctrl pointer to classifier controller to create and delete features and to delete relationships
 */
void consistency_feature_init ( consistency_feature_t *this_,
                                data_database_reader_t *db_reader,
                                struct ctrl_classifier_controller_struct *clfy_ctrl
                              );

/*!
 *  \brief destroys the consistency_feature_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void consistency_feature_destroy ( consistency_feature_t *this_ );

/*!
 *  \brief executes policies on features triggered by changing the diagram type.
 *
 *  Note: lifelines are excluded, these are handled by consistency_lifeline_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param updated_diagram the updated diagram.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_feature_delete_invisibles_in_diagram ( consistency_feature_t *this_,
                                                              const data_diagram_t *updated_diagram
                                                            );

/*!
 *  \brief executes policies on features triggered by deleting a diagramelement.
 *
 *  Note: lifelines are excluded, these are handled by consistency_lifeline_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id the classifier to check, e.g. due to a deleted diagramelement.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t consistency_feature_delete_invisibles_of_classifier ( consistency_feature_t *this_,
                                                                 data_row_t classifier_id
                                                               );

#endif  /* CONSISTENCY_FEATURE_H */


/*
Copyright 2026-2026 Andreas Warnke

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
