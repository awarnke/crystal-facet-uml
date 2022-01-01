/* File: ctrl_undo_redo_entry_type.h; Copyright and License: see below */

#ifndef CTRL_UNDO_REDO_ENTRY_TYPE_H
#define CTRL_UNDO_REDO_ENTRY_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Indentifies the type of action of an ctrl_undo_redo_entry_t that can be undone or redone
 */

/*!
 *  \brief list of actions that can be un-done and re-done
 */
enum ctrl_undo_redo_entry_type_enum {
    CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY,  /*!< marks a boundary between sets of undo/redo actions */
    CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM,  /*!< marks a delete diagram action for redo (meaning create for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM,  /*!< marks an update diagram action (meaning content of the record has changed) */
    CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM,  /*!< marks a create diagram action for redo (meaning delete for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT,  /*!< marks a delete diagramelement action for redo (meaning create for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT,  /*!< marks an update diagramelement action (meaning content of the record has changed) */
    CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT,  /*!< marks a create diagramelement action for redo (meaning delete for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER,  /*!< marks a delete classifier action for redo (meaning create for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER,  /*!< marks an update classifier action (meaning content of the record has changed) */
    CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER,  /*!< marks a create classifier action for redo (meaning delete for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE,  /*!< marks a delete feature action for redo (meaning create for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE,  /*!< marks an update feature action (meaning content of the record has changed) */
    CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE,  /*!< marks a create feature action for redo (meaning delete for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP,  /*!< marks a delete relationship action for redo (meaning create for undo) */
    CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP,  /*!< marks an update relationship action (meaning content of the record has changed) */
    CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP,  /*!< marks a create relationship action for redo (meaning delete for undo) */
};

typedef enum ctrl_undo_redo_entry_type_enum ctrl_undo_redo_entry_type_t;

#endif  /* CTRL_UNDO_REDO_ENTRY_TYPE_H */


/*
Copyright 2016-2022 Andreas Warnke

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
