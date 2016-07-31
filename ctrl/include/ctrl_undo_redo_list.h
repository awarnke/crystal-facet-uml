/* File: ctrl_undo_redo_list.h; Copyright and License: see below */

#ifndef CTRL_UNDO_REDO_LIST_H
#define CTRL_UNDO_REDO_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief reverts and re-performs changes to the database
 */

#include "ctrl_undo_redo_entry.h"
#include "ctrl_error.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants for max undo redo list size
 */
enum ctrl_undo_redo_list_max_enum {
    CTRL_UNDO_REDO_LIST_MAX_SIZE = 256,  /*!< maximum number of action-steps that can be un-done/re-done */
};

/*!
 *  \brief all data attributes needed for the ctrl_undo_redo_list functions
 */
struct ctrl_undo_redo_list_struct {
    data_database_writer_t *db_writer;  /*!< pointer to external database writer */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */

    uint32_t start;  /*!< start position of the ring buffer (valid absolute index in buffer array: 0 <= start < CTRL_UNDO_REDO_LIST_MAX_SIZE) */
    uint32_t length;  /*!< length of valid entries in the ring buffer (0 <= length <= CTRL_UNDO_REDO_LIST_MAX_SIZE) */
    uint32_t current;  /*!< current position in the ring buffer (relative to start position: 0 <= current <= length). If length == current, there is no redo action left */
    bool buffer_incomplete;  /*!< true if the first entry in the list is already overwritten. buffer_complete influences the error code of the undo function */
    ctrl_undo_redo_entry_t buffer[CTRL_UNDO_REDO_LIST_MAX_SIZE];  /*!< the ring buffer of undo/redo action entries and boundary entries */
};

typedef struct ctrl_undo_redo_list_struct ctrl_undo_redo_list_t;

/*!
 *  \brief initializes the ctrl_undo_redo_list_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database object that can be used for retrieving data
 *  \param db_writer pointer to database object that can be used for changing data
 */
static void ctrl_undo_redo_list_init ( ctrl_undo_redo_list_t *this_, data_database_reader_t *db_reader, data_database_writer_t *db_writer );

/*!
 *  \brief destroys the ctrl_undo_redo_list_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static void ctrl_undo_redo_list_destroy ( ctrl_undo_redo_list_t *this_ );

/*!
 *  \brief clears the ctrl_undo_redo_list_t, e.g. when the current database file has changed
 *
 *  \param this_ pointer to own object attributes
 */
static void ctrl_undo_redo_list_clear ( ctrl_undo_redo_list_t *this_ );

/*!
 *  \brief adds a boundary entry to the list (CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY).
 *
 *  A boundary entry marks a boundary between sets of undo/redo actions.
 *  Typically, a set of undo/redo actions is performed only at a stretch.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \return CTRL_ERROR_ARRAY_BUFFER_EXCEEDED if the last boundary in the list is overwritten and the current set of actions cannot be undone.
 *          CTRL_ERROR_NONE otherwise.
 */
static ctrl_error_t ctrl_undo_redo_list_add_boundary ( ctrl_undo_redo_list_t *this_ );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was deleted
 */
static void ctrl_undo_redo_list_add_delete_diagram ( ctrl_undo_redo_list_t *this_, data_diagram_t *old_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was updated
 *  \param new_value the record contents after the record was updated
 */
static void ctrl_undo_redo_list_add_update_diagram ( ctrl_undo_redo_list_t *this_, data_diagram_t *old_value, data_diagram_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param new_value the record contents after the record was created
 */
static void ctrl_undo_redo_list_add_create_diagram ( ctrl_undo_redo_list_t *this_, data_diagram_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was deleted
 */
static void ctrl_undo_redo_list_add_delete_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *old_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was updated
 *  \param new_value the record contents after the record was updated
 */
static void ctrl_undo_redo_list_add_update_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *old_value, data_diagramelement_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param new_value the record contents after the record was created
 */
static void ctrl_undo_redo_list_add_create_diagramelement ( ctrl_undo_redo_list_t *this_, data_diagramelement_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was deleted
 */
static void ctrl_undo_redo_list_add_delete_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *old_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was updated
 *  \param new_value the record contents after the record was updated
 */
static void ctrl_undo_redo_list_add_update_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *old_value, data_classifier_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param new_value the record contents after the record was created
 */
static void ctrl_undo_redo_list_add_create_classifier ( ctrl_undo_redo_list_t *this_, data_classifier_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was deleted
 */
static void ctrl_undo_redo_list_add_delete_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *old_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was updated
 *  \param new_value the record contents after the record was updated
 */
static void ctrl_undo_redo_list_add_update_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *old_value, data_feature_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param new_value the record contents after the record was created
 */
static void ctrl_undo_redo_list_add_create_feature ( ctrl_undo_redo_list_t *this_, data_feature_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was deleted
 */
static void ctrl_undo_redo_list_add_delete_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *old_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param old_value the record contents before the record was updated
 *  \param new_value the record contents after the record was updated
 */
static void ctrl_undo_redo_list_add_update_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *old_value, data_relationship_t *new_value );

/*!
 *  \brief adds an action entry to the list of type CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \param new_value the record contents after the record was created
 */
static void ctrl_undo_redo_list_add_create_relationship ( ctrl_undo_redo_list_t *this_, data_relationship_t *new_value );

/*!
 *  \brief un-does a set of actions till the last boundary.
 *
 *  \param this_ pointer to own object attributes
 *  \return CTRL_ERROR_ARRAY_BUFFER_EXCEEDED if there is no more complete set of actions to be un-done due to limits of buffer.
 *          CTRL_ERROR_INVALID_REQUEST if there is no more set of actions to be un-done
 *          CTRL_ERROR_NONE otherwise.
 */
ctrl_error_t ctrl_undo_redo_list_undo ( ctrl_undo_redo_list_t *this_ );

/*!
 *  \brief re-does a set of actions till the next boundary.
 *
 *  \param this_ pointer to own object attributes
 *  \return CTRL_ERROR_INVALID_REQUEST if there is no more set of actions to be re-done.
 *          CTRL_ERROR_NONE otherwise.
 */
ctrl_error_t ctrl_undo_redo_list_redo ( ctrl_undo_redo_list_t *this_ );

/*!
 *  \brief adds an entry to the list.
 *
 *  This method may overwrite the oldest entries in the list
 *  (which is implemented as a ring-buffer).
 *  This method overwrites all list-entries newer than the current position
 *  (which happens if the user un-does an action and the current position is
 *  not at the end of the list anymore).
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to an initialized entry
 */
ctrl_undo_redo_entry_t *ctrl_undo_redo_list_private_add_entry_ptr ( ctrl_undo_redo_list_t *this_ );

/*!
 *  \brief counts the boundary entries in a given range
 *
 *   The caller has to check that the search range is within the valid buffer range
 *
 *  \param this_ pointer to own object attributes
 *  \param start_idx index in the buffer where to start the search: 0 <= start_idx < CTRL_UNDO_REDO_LIST_MAX_SIZE
 *  \param search_len length of the area in the buffer where to search: 0 <= search_len <= CTRL_UNDO_REDO_LIST_MAX_SIZE
 *  \return number of boundary entries within the specifies range
 */
static uint32_t ctrl_undo_redo_list_private_count_boundaries ( ctrl_undo_redo_list_t *this_, uint32_t start_idx, uint32_t search_len );

#include "ctrl_undo_redo_list.inl"

#endif  /* CTRL_UNDO_REDO_LIST_H */


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
