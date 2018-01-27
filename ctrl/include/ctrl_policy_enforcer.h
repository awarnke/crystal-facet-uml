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
#include "ctrl_undo_redo_list.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "data_diagram_type.h"
#include "data_diagram.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the policy enforcer
 */
struct ctrl_policy_enforcer_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t *db_writer;  /*!< pointer to external database writer */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_undo_redo_list_t *undo_redo_list;  /*!< pointer to external ctrl_undo_redo_list_t */
};

typedef struct ctrl_policy_enforcer_struct ctrl_policy_enforcer_t;

/*!
 *  \brief initializes the ctrl_policy_enforcer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param undo_redo_list pointer to list of undo/redo actions
 *  \param database pointer to database object
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param db_writer pointer to database writer object that can be used for changing data
 */
void ctrl_policy_enforcer_init ( ctrl_policy_enforcer_t *this_,
                                    ctrl_undo_redo_list_t *undo_redo_list,
                                    data_database_t *database,
                                    data_database_reader_t *db_reader,
                                    data_database_writer_t *db_writer
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
 *  \param this_ pointer to own object attributes
 *  \param new_diagram data of the new diagram to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created diagram, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_policy_enforcer_update_diagram_type ( ctrl_policy_enforcer_t *this_,
                                                        const data_diagram_t *new_diagram,
                                                        bool add_to_latest_undo_set,
                                                        int64_t* out_new_id
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
