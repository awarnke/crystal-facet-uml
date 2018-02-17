/* File: ctrl_diagram_controller.h; Copyright and License: see below */

#ifndef CTRL_DIAGRAM_CONTROLLER_H
#define CTRL_DIAGRAM_CONTROLLER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides write access and triggers consistency checks to diagrams in the database
 */

#include "ctrl_error.h"
#include "ctrl_undo_redo_list.h"
#include "ctrl_policy_enforcer.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "data_diagram_type.h"
#include "data_diagram.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the diagram functions
 */
struct ctrl_diagram_controller_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t *db_writer;  /*!< pointer to external database writer */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_undo_redo_list_t *undo_redo_list;  /*!< pointer to external ctrl_undo_redo_list_t */
    ctrl_policy_enforcer_t *policy_enforcer;  /*!< pointer to external ctrl_policy_enforcer_t */
};

typedef struct ctrl_diagram_controller_struct ctrl_diagram_controller_t;

/*!
 *  \brief initializes the ctrl_diagram_controller_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param undo_redo_list pointer to list of undo/redo actions
 *  \param policy_enforcer pointer to policy enforcer that keeps the database in a gui-suiting style
 *  \param database pointer to database object
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param db_writer pointer to database writer object that can be used for changing data
 */
void ctrl_diagram_controller_init ( ctrl_diagram_controller_t *this_,
                                    ctrl_undo_redo_list_t *undo_redo_list,
                                    ctrl_policy_enforcer_t *policy_enforcer,
                                    data_database_t *database,
                                    data_database_reader_t *db_reader,
                                    data_database_writer_t *db_writer
                                  );

/*!
 *  \brief destroys the ctrl_diagram_controller_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_diagram_controller_destroy ( ctrl_diagram_controller_t *this_ );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief creates a new diagram.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagram data of the new diagram to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created diagram, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_create_diagram ( ctrl_diagram_controller_t *this_,
                                                      const data_diagram_t *new_diagram,
                                                      bool add_to_latest_undo_set,
                                                      int64_t* out_new_id
                                                    );

/*!
 *  \brief creates a new diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_diagram_id id of the parent diagram for which a child is created
 *  \param diagram_type type of the new diagram
 *  \param diagram_name name of the new diagram
 *  \param out_new_id id of the newly created diagram, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_create_child_diagram ( ctrl_diagram_controller_t *this_,
                                                            int64_t parent_diagram_id,
                                                            data_diagram_type_t diagram_type,
                                                            const char* diagram_name,
                                                            int64_t* out_new_id
                                                          );

/*!
 *  \brief checks if a root diagram exists and creates one if not
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_type type of the root diagram
 *  \param diagram_name name of the root diagram
 *  \param out_new_id id of the created root diagram or DATA_ID_VOID_ID if a root diagram already existed, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_create_root_diagram_if_not_exists ( ctrl_diagram_controller_t *this_,
                                                                         data_diagram_type_t diagram_type,
                                                                         const char* diagram_name,
                                                                         int64_t* out_new_id
                                                                       );

/*!
 *  \brief deletes a diagram record
 *         TODO: and associated diagramelements (see ctrl_diagram_controller_delete_diagramelement for additional actions)
 *         and stores all actions in the undo redo list.
 *
 *  Note: The diagram is not deleted if still referenced by other diagrams.
 *
 *  \see ctrl_diagram_controller_delete_diagramelement
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id of the diagram record to be deleted.
 *  \param add_to_latest_undo_set true if this delete-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
ctrl_error_t ctrl_diagram_controller_delete_diagram ( ctrl_diagram_controller_t *this_,
                                                      int64_t obj_id,
                                                      bool add_to_latest_undo_set
                                                    );

/*!
 *  \brief updates the diagram attribute: parent_id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_parent_id new parent_id of the diagram
 *  \param add_to_latest_undo_set true if this update-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_update_diagram_parent_id ( ctrl_diagram_controller_t *this_,
                                                                int64_t diagram_id,
                                                                int64_t new_diagram_parent_id,
                                                                bool add_to_latest_undo_set
                                                              );

/*!
 *  \brief updates the diagram attribute: description
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_description new description text of the diagram
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_update_diagram_description ( ctrl_diagram_controller_t *this_,
                                                                  int64_t diagram_id,
                                                                  const char* new_diagram_description
                                                                );

/*!
 *  \brief updates the diagram attribute: name
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_name new name of the diagram
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_update_diagram_name ( ctrl_diagram_controller_t *this_,
                                                           int64_t diagram_id,
                                                           const char* new_diagram_name
                                                         );

/*!
 *  \brief updates the diagram attribute: diagram_type
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_type new diagram_type of the diagram
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_update_diagram_type ( ctrl_diagram_controller_t *this_,
                                                           int64_t diagram_id,
                                                           data_diagram_type_t new_diagram_type
                                                         );

/*!
 *  \brief updates the diagram attribute: list_order
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_list_order new list_order of the diagram
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_update_diagram_list_order ( ctrl_diagram_controller_t *this_,
                                                                 int64_t diagram_id,
                                                                 int32_t new_diagram_list_order
                                                               );

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief creates a new diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagramelement data of the new diagramelement to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created diagramelement, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_create_diagramelement ( ctrl_diagram_controller_t *this_,
                                                             const data_diagramelement_t *new_diagramelement,
                                                             bool add_to_latest_undo_set,
                                                             int64_t* out_new_id
                                                           );

/*!
 *  \brief deletes a diagramelement record
 *         and associated classifiers (if not referenced anymore)
 *         and the possibly associated feature (via policy_enforcer)
 *         and stores all actions in the undo redo list.
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id of the diagramelement record to be deleted.
 *  \param add_to_latest_undo_set true if this delete-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
ctrl_error_t ctrl_diagram_controller_delete_diagramelement ( ctrl_diagram_controller_t *this_,
                                                             int64_t obj_id,
                                                             bool add_to_latest_undo_set
                                                           );

/*!
 *  \brief updates the diagramelement attribute: display_flags
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement to be updated
 *  \param new_diagramelement_display_flags new display_flags of the diagramelement
 *  \param add_to_latest_undo_set true if this update-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_update_diagramelement_display_flags ( ctrl_diagram_controller_t *this_,
                                                                           int64_t diagramelement_id,
                                                                           data_diagramelement_flag_t new_diagramelement_display_flags,
                                                                           bool add_to_latest_undo_set
                                                                         );

/*!
 *  \brief updates the diagramelement attribute: focused_feature_id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement to be updated
 *  \param new_diagramelement_focused_feature_id new focused_feature_id of the diagramelement
 *  \param add_to_latest_undo_set true if this update-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_diagram_controller_update_diagramelement_focused_feature_id ( ctrl_diagram_controller_t *this_,
                                                                                int64_t diagramelement_id,
                                                                                int64_t new_diagramelement_focused_feature_id,
                                                                                bool add_to_latest_undo_set
                                                                              );

#endif  /* CTRL_DIAGRAM_CONTROLLER_H */


/*
Copyright 2016-2018 Andreas Warnke

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
