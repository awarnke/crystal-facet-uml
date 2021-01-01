/* File: ctrl_undo_redo_action_boundary.h; Copyright and License: see below */

#ifndef CTRL_UNDO_REDO_ACTION_BOUNDARY_H
#define CTRL_UNDO_REDO_ACTION_BOUNDARY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Indentifies the append mode to the ctrl_undo_redo_list_t.
 * 
 *  Either add to last undo/redo action or start a new undo/redo action
 */

#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief list of actions that can be un-done and re-done
 */
enum ctrl_undo_redo_action_boundary_enum {
    CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW = false,  /*!< keeps the boundary and starts a new undo/redo action */
    CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND = true,  /*!< shifts the boundary and adds a new undo/redo entry to the last action-set */
};

typedef enum ctrl_undo_redo_action_boundary_enum ctrl_undo_redo_action_boundary_t;

#endif  /* CTRL_UNDO_REDO_ACTION_BOUNDARY_H */


/*
Copyright 2020-2021 Andreas Warnke

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
