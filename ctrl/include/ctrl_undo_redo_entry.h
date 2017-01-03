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
#include "data_classifier.h"
#include "data_diagram.h"
#include "data_diagramelement.h"
#include "data_relationship.h"
#include "data_feature.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief union of all possible data types which are needed for undo and redo
 */
union ctrl_undo_redo_entry_private_data_union {
    data_classifier_t classifier;
    data_diagram_t diagram;
    data_diagramelement_t diagramelement;
    data_relationship_t relationship;
    data_feature_t feature;
};

/*!
 *  \brief all data attributes needed for the diagram functions
 */
struct ctrl_undo_redo_entry_struct {
    ctrl_undo_redo_entry_type_t action_type;
    union ctrl_undo_redo_entry_private_data_union data_before_action;
    union ctrl_undo_redo_entry_private_data_union data_after_action;
};

typedef struct ctrl_undo_redo_entry_struct ctrl_undo_redo_entry_t;

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to empty
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_undo_redo_entry_init_empty ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param action_type a step of a set of actions that can be undone and redone
 */
static inline void ctrl_undo_redo_entry_init ( ctrl_undo_redo_entry_t *this_, ctrl_undo_redo_entry_type_t action_type );

/*!
 *  \brief re-initializes the ctrl_undo_redo_entry_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param action_type a step of a set of actions that can be undone and redone
 */
static inline void ctrl_undo_redo_entry_reinit ( ctrl_undo_redo_entry_t *this_, ctrl_undo_redo_entry_type_t action_type );

/*!
 *  \brief destroys the ctrl_undo_redo_entry_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_undo_redo_entry_destroy ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the action type
 *
 *  \param this_ pointer to own object attributes
 *  \return action type
 */
static inline ctrl_undo_redo_entry_type_t ctrl_undo_redo_entry_get_action_type ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_classifier_t, where the data before the action is stored
 */
static inline data_classifier_t *ctrl_undo_redo_entry_get_classifier_before_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_diagram_t, where the data before the action is stored
 */
static inline data_diagram_t *ctrl_undo_redo_entry_get_diagram_before_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_diagramelement_t, where the data before the action is stored
 */
static inline data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_before_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_relationship_t, where the data before the action is stored
 */
static inline data_relationship_t *ctrl_undo_redo_entry_get_relationship_before_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_feature_t, where the data before the action is stored
 */
static inline data_feature_t *ctrl_undo_redo_entry_get_feature_before_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_classifier_t, where the data after the action is stored
 */
static inline data_classifier_t *ctrl_undo_redo_entry_get_classifier_after_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_diagram_t, where the data after the action is stored
 */
static inline data_diagram_t *ctrl_undo_redo_entry_get_diagram_after_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_diagramelement_t, where the data after the action is stored
 */
static inline data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_after_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_relationship_t, where the data after the action is stored
 */
static inline data_relationship_t *ctrl_undo_redo_entry_get_relationship_after_action_ptr ( ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_feature_t, where the data after the action is stored
 */
static inline data_feature_t *ctrl_undo_redo_entry_get_feature_after_action_ptr ( ctrl_undo_redo_entry_t *this_ );

#include "ctrl_undo_redo_entry.inl"

#endif  /* CTRL_UNDO_REDO_ENTRY_H */


/*
Copyright 2016-2017 Andreas Warnke

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
