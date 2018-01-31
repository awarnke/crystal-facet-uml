/* File: ctrl_policy_enforcer.h; Copyright and License: see below */

#ifndef CTRL_POLICY_ENFORCER_H
#define CTRL_POLICY_ENFORCER_H

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
#include "ctrl_classifier_controller.h"
#include "storage/data_database_reader.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the policy enforcer
 *
 *  The policy enforcer works on a similar abstraction level as the gui module.
 *  Therefore it references the same ctrl and data objects as the gui module.
 */
struct ctrl_policy_enforcer_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_classifier_controller_t *clfy_ctrl;  /*!< pointer to external classifier controller */
};

typedef struct ctrl_policy_enforcer_struct ctrl_policy_enforcer_t;

/*!
 *  \brief initializes the ctrl_policy_enforcer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param clfy_ctrl pointer to classifier controller to create and delete features and to delete relationships
 */
void ctrl_policy_enforcer_init ( ctrl_policy_enforcer_t *this_,
                                 data_database_reader_t *db_reader,
                                 ctrl_classifier_controller_t *clfy_ctrl
                               );

/*!
 *  \brief destroys the ctrl_policy_enforcer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_policy_enforcer_destroy ( ctrl_policy_enforcer_t *this_ );

/* ================================ LIFELINES ================================ */

/*!
 *  \brief executes policies involved in changing the diagram type.
 *
 *  Current rules are:
 *  - when changing a diagram type to DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *    or to DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM
 *    or to DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
 *    all contained elements shall get a lifeline
 *    and this lifeline shall be the focused_feature of the diagramelement.
 *
 *  TODO: Is this really a good strategy?
 *
 *  Wouldn't it be better to associate lifelines with "instances":
 *  When making an instance of an element, it gets a lifeline?
 *
 *  Note: in both versions, the model in the database would be the same in the end.
 *  Therefore decision can be changed later...
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagram data of the new diagram to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created diagram, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_policy_enforcer_post_update_diagram_type ( ctrl_policy_enforcer_t *this_,
                                                             const data_diagram_t *new_diagram,
                                                             bool add_to_latest_undo_set,
                                                             int64_t* out_new_id
                                                           );

/*!
 *  \brief executes policies involved in creating a diagramelement.
 *
 *  Current rules are:
 *  - when creating a diagramelement
 *    to a diagram of type DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM
 *    or DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM
 *    or DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM,
 *    the new diagramelement shall get a lifeline
 *    and this lifeline shall be the focused_feature of the diagramelement.
 *
 *  TODO: Is this really a good strategy?
 *
 *  Wouldn't it be better to associate lifelines with "instances":
 *  When making an instance of an element, it gets a lifeline?
 *
 *  Note: in both versions, the model in the database would be the same in the end.
 *  Therefore decision can be changed later...
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagram data of the new diagram to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created diagram, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_policy_enforcer_post_create_diagramelement ( ctrl_policy_enforcer_t *this_,
                                                               const data_diagram_t *new_diagram,
                                                               bool add_to_latest_undo_set,
                                                               int64_t* out_new_id
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
 *  \param old_diagramelement data of the new diagram to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created diagram, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_policy_enforcer_pre_delete_diagramelement ( ctrl_policy_enforcer_t *this_,
                                                              const data_diagramelement_t *old_diagramelement,
                                                              bool add_to_latest_undo_set,
                                                              int64_t* out_new_id
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
ctrl_error_t ctrl_policy_enforcer_post_delete_diagramelement ( ctrl_policy_enforcer_t *this_,
                                                               const data_diagramelement_t *deleted_diagramelement
                                                             );

#endif  /* CTRL_POLICY_ENFORCER_H */


/*
Copyright 2018-2018 Andreas Warnke

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
