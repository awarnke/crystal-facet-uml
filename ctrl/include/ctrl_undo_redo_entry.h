/* File: ctrl_undo_redo_entry.h; Copyright and License: see below */

#ifndef CTRL_UNDO_REDO_ENTRY_H
#define CTRL_UNDO_REDO_ENTRY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores a change in the database that can be undone and redone.
 */

#include "ctrl_undo_redo_entry_type.h"
#include "ctrl_error.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the diagram functions
 */
struct ctrl_undo_redo_entry_struct {
    ctrl_undo_redo_entry_type_t action_type;
};

typedef struct ctrl_undo_redo_entry_struct ctrl_undo_redo_entry_t;

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to empty
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_undo_redo_entry_init_empty ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param action_type a step of a set of actions that can be undone and redone
 */
void ctrl_undo_redo_entry_init ( ctrl_undo_redo_entry_t *this_, ctrl_undo_redo_entry_type_t action_type );

/*!
 *  \brief re-initializes the ctrl_undo_redo_entry_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param action_type a step of a set of actions that can be undone and redone
 */
void ctrl_undo_redo_entry_reinit ( ctrl_undo_redo_entry_t *this_, ctrl_undo_redo_entry_type_t action_type );

/*!
 *  \brief destroys the ctrl_undo_redo_entry_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_undo_redo_entry_destroy ( ctrl_undo_redo_entry_t *this_ );

#endif  /* CTRL_UNDO_REDO_ENTRY_H */


/*
Copyright 2016-2016 Andreas Warnke

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
