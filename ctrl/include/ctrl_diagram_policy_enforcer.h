/* File: ctrl_diagram_policy_enforcer.h; Copyright and License: see below */

#ifndef CTRL_DIAGRAM_POLICY_ENFORCER_H
#define CTRL_DIAGRAM_POLICY_ENFORCER_H

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

#include "ctrl_error.h"
#include "storage/data_database_reader.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct ctrl_classifier_controller_struct;
struct ctrl_diagram_controller_struct;

/*!
 *  \brief constants of ctrl_diagram_policy_enforcer_t
 */
enum ctrl_diagram_policy_enforcer_const_enum {
    CTRL_DIAGRAM_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES = 128,  /*!< maximum number of diagramelements in a diagram */
};

/*!
 *  \brief all data attributes needed for the policy enforcer
 *
 *  The policy enforcer works on a similar abstraction level as the gui module.
 *  Therefore it references the same ctrl and data objects as the gui module.
 */
struct ctrl_diagram_policy_enforcer_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    struct ctrl_classifier_controller_struct *clfy_ctrl;  /*!< pointer to external classifier controller */
    struct ctrl_diagram_controller_struct *diag_ctrl;  /*!< pointer to external diagram controller */

    data_diagramelement_t private_temp_diagele_buf[CTRL_DIAGRAM_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES];
};

typedef struct ctrl_diagram_policy_enforcer_struct ctrl_diagram_policy_enforcer_t;

/*!
 *  \brief initializes the ctrl_diagram_policy_enforcer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param clfy_ctrl pointer to classifier controller to create and delete features and to delete relationships
 *  \param diag_ctrl pointer to diagram controller to modify diagramelements
 */
void ctrl_diagram_policy_enforcer_init ( ctrl_diagram_policy_enforcer_t *this_,
                                         data_database_reader_t *db_reader,
                                         struct ctrl_classifier_controller_struct *clfy_ctrl,
                                         struct ctrl_diagram_controller_struct *diag_ctrl
                                       );

/*!
 *  \brief destroys the ctrl_diagram_policy_enforcer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_diagram_policy_enforcer_destroy ( ctrl_diagram_policy_enforcer_t *this_ );

/* ================================ ENTRY POINTS ================================ */

/*!
 *  \brief executes policies involved in changing the diagram type.
 *
 *  \param this_ pointer to own object attributes
 *  \param updated_diagram data of the updated diagram.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
static inline ctrl_error_t ctrl_diagram_policy_enforcer_post_update_diagram_type ( ctrl_diagram_policy_enforcer_t *this_,
                                                                                   const data_diagram_t *updated_diagram
                                                                                 );

/*!
 *  \brief executes policies involved in creating a diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagramelement data of the new diagramelement.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
static inline ctrl_error_t ctrl_diagram_policy_enforcer_post_create_diagramelement ( ctrl_diagram_policy_enforcer_t *this_,
                                                                                     const data_diagramelement_t *new_diagramelement
                                                                                   );

/*!
 *  \brief executes policies involved in deleting a diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
static inline ctrl_error_t ctrl_diagram_policy_enforcer_post_delete_diagramelement ( ctrl_diagram_policy_enforcer_t *this_,
                                                                                     const data_diagramelement_t *deleted_diagramelement
                                                                                   );

/* ================================ LIFELINES ================================ */

/*!
 *  \brief executes policies involved in changing the diagram type.
 *
 *  Current rules are:
 *  - when changing a diagram type to DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *    or to DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM
 *    or to DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
 *    or to DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM,
 *    all contained elements shall get a lifeline
 *    and this lifeline shall be the focused_feature of the diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param updated_diagram data of the updated diagram.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_policy_enforcer_private_create_lifelines ( ctrl_diagram_policy_enforcer_t *this_,
                                                                     const data_diagram_t *updated_diagram
                                                                   );

/*!
 *  \brief executes policies involved in creating a diagramelement.
 *
 *  Current rules are:
 *  - when creating a diagramelement
 *    to a diagram of type DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *    or DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM
 *    or DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
 *    or to DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM,
 *    the new diagramelement shall get a lifeline
 *    and this lifeline shall be the focused_feature of the diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagramelement data of the new diagramelement.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_policy_enforcer_private_create_a_lifeline ( ctrl_diagram_policy_enforcer_t *this_,
                                                                      const data_diagramelement_t *new_diagramelement
                                                                    );

/*!
 *  \brief creates one lifeline, unconditional
 *
 *  \param this_ pointer to own object attributes
 *  \param the_diagramelement data of the diagramelement.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_policy_enforcer_private_create_one_lifeline ( ctrl_diagram_policy_enforcer_t *this_,
                                                                        const data_diagramelement_t *the_diagramelement
                                                                      );

/*!
 *  \brief executes policies involved in deleting a diagramelement.
 *
 *  Current rules are:
 *  - when deleting a diagramelement,
 *    a possibly referenced focused_feature of type lifeline
 *    is also deleted.
 *  - when deleting above referenced lifeline, all relationships
 *    to and from that lifeline are also deleted.
 *
 *  Rows are deleted in an order that enables an always consistent database structure.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_policy_enforcer_private_delete_a_lifeline ( ctrl_diagram_policy_enforcer_t *this_,
                                                                      const data_diagramelement_t *deleted_diagramelement
                                                                    );

/* ================================ NO ABANDONED CLASSIFIERS ================================ */

/*!
 *  \brief executes policies involved in deleting a diagramelement.
 *
 *  Current rules are:
 *  - after deleting a diagramelement,
 *    delete the classifier (if now unreferenced)
 *
 *  Rows are deleted in an order that enables an always consistent database structure.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_policy_enforcer_private_delete_unreferenced_classifier ( ctrl_diagram_policy_enforcer_t *this_,
                                                                                   const data_diagramelement_t *deleted_diagramelement
                                                                                 );

#include "ctrl_diagram_policy_enforcer.inl"

#endif  /* CTRL_DIAGRAM_POLICY_ENFORCER_H */


/*
Copyright 2018-2020 Andreas Warnke

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
