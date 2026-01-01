/* File: ctrl_undo_redo_entry.h; Copyright and License: see below */

#ifndef CTRL_UNDO_REDO_ENTRY_H
#define CTRL_UNDO_REDO_ENTRY_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores a change in the database that can be undone and redone.
 */

#include "ctrl_undo_redo_entry_type.h"
#include "u8/u8_error.h"
#include "entity/data_classifier.h"
#include "entity/data_diagram.h"
#include "entity/data_diagramelement.h"
#include "entity/data_relationship.h"
#include "entity/data_feature.h"
#include "set/data_stat.h"
#include "storage/data_revision.h"
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
    data_revision_t boundary_revision;
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
 *  \brief destroys the ctrl_undo_redo_entry_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_undo_redo_entry_destroy ( ctrl_undo_redo_entry_t *this_ );

/* ================================ BOUNDARY ================================ */

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_BOUNDARY type
 *
 *  \param this_ pointer to own object attributes
 *  \param boundary_revision database revision at this boundary
 */
static inline void ctrl_undo_redo_entry_init_boundary ( ctrl_undo_redo_entry_t *this_,
                                                        data_revision_t boundary_revision
                                                      );

/*!
 *  \brief gets the revision identifier of database at this boundary action
 *
 *  \param this_ pointer to own object attributes
 *  \return revision id
 */
static inline data_revision_t ctrl_undo_redo_entry_get_boundary_revision ( const ctrl_undo_redo_entry_t *this_ );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAM type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_after pointer to data_diagram_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_create_diagram ( ctrl_undo_redo_entry_t *this_,
                                                              const data_diagram_t *data_after
                                                            );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAM type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_diagram_t, where the old data before the action is stored
 *  \param data_after pointer to data_diagram_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_update_diagram ( ctrl_undo_redo_entry_t *this_,
                                                              const data_diagram_t *data_before,
                                                              const data_diagram_t *data_after
                                                            );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAM type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_diagram_t, where the old data before the action is stored
 */
static inline void ctrl_undo_redo_entry_init_delete_diagram ( ctrl_undo_redo_entry_t *this_,
                                                              const data_diagram_t *data_before
                                                            );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_diagram_t, where the data after the action is stored
 */
static inline const data_diagram_t *ctrl_undo_redo_entry_get_diagram_after_action_const ( const ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_diagram_t, where the data before the action is stored
 */
static inline const data_diagram_t *ctrl_undo_redo_entry_get_diagram_before_action_const ( const ctrl_undo_redo_entry_t *this_ );

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_DIAGRAMELEMENT type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_after pointer to data_diagramelement_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_create_diagramelement ( ctrl_undo_redo_entry_t *this_,
                                                                     const data_diagramelement_t *data_after
                                                                   );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_DIAGRAMELEMENT type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_diagramelement_t, where the old data before the action is stored
 *  \param data_after pointer to data_diagramelement_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_update_diagramelement ( ctrl_undo_redo_entry_t *this_,
                                                                     const data_diagramelement_t *data_before,
                                                                     const data_diagramelement_t *data_after
                                                                   );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_DIAGRAMELEMENT type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_diagramelement_t, where the old data before the action is stored
 */
static inline void ctrl_undo_redo_entry_init_delete_diagramelement ( ctrl_undo_redo_entry_t *this_,
                                                                     const data_diagramelement_t *data_before
                                                                   );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_diagramelement_t, where the data after the action is stored
 */
static inline const data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_after_action_const ( const ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_diagramelement_t, where the data before the action is stored
 */
static inline const data_diagramelement_t *ctrl_undo_redo_entry_get_diagramelement_before_action_const ( const ctrl_undo_redo_entry_t *this_ );

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_CLASSIFIER type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_after pointer to data_classifier_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_create_classifier ( ctrl_undo_redo_entry_t *this_ ,
                                                                 const data_classifier_t *data_after
                                                               );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_CLASSIFIER type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_classifier_t, where the old data before the action is stored
 *  \param data_after pointer to data_classifier_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_update_classifier ( ctrl_undo_redo_entry_t *this_ ,
                                                                 const data_classifier_t *data_before,
                                                                 const data_classifier_t *data_after
                                                               );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_CLASSIFIER type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_classifier_t, where the old data before the action is stored
 */
static inline void ctrl_undo_redo_entry_init_delete_classifier ( ctrl_undo_redo_entry_t *this_ ,
                                                                 const data_classifier_t *data_before
                                                               );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_classifier_t, where the data after the action is stored
 */
static inline const data_classifier_t *ctrl_undo_redo_entry_get_classifier_after_action_const ( const ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_classifier_t, where the data before the action is stored
 */
static inline const data_classifier_t *ctrl_undo_redo_entry_get_classifier_before_action_const ( const ctrl_undo_redo_entry_t *this_ );

/* ================================ FEATURE ================================ */

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_FEATURE type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_after pointer to data_feature_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_create_feature ( ctrl_undo_redo_entry_t *this_,
                                                              const data_feature_t *data_after
                                                            );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_FEATURE type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_feature_t, where the old data before the action is stored
 *  \param data_after pointer to data_feature_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_update_feature ( ctrl_undo_redo_entry_t *this_,
                                                              const data_feature_t *data_before,
                                                              const data_feature_t *data_after
                                                            );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_FEATURE type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_feature_t, where the old data before the action is stored
 */
static inline void ctrl_undo_redo_entry_init_delete_feature ( ctrl_undo_redo_entry_t *this_,
                                                              const data_feature_t *data_before
                                                            );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_feature_t, where the data after the action is stored
 */
static inline const data_feature_t *ctrl_undo_redo_entry_get_feature_after_action_const ( const ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_feature_t, where the data before the action is stored
 */
static inline const data_feature_t *ctrl_undo_redo_entry_get_feature_before_action_const ( const ctrl_undo_redo_entry_t *this_ );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_CREATE_RELATIONSHIP type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_after pointer to data_relationship_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_create_relationship ( ctrl_undo_redo_entry_t *this_,
                                                                   const data_relationship_t *data_after
                                                                 );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_UPDATE_RELATIONSHIP type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_relationship_t, where the old data before the action is stored
 *  \param data_after pointer to data_relationship_t, where the new data after the action is stored
 */
static inline void ctrl_undo_redo_entry_init_update_relationship ( ctrl_undo_redo_entry_t *this_,
                                                                   const data_relationship_t *data_before,
                                                                   const data_relationship_t *data_after
                                                                 );

/*!
 *  \brief initializes the ctrl_undo_redo_entry_t struct to a CTRL_UNDO_REDO_ENTRY_TYPE_DELETE_RELATIONSHIP type
 *
 *  \param this_ pointer to own object attributes
 *  \param data_before pointer to data_relationship_t, where the old data before the action is stored
 */
static inline void ctrl_undo_redo_entry_init_delete_relationship ( ctrl_undo_redo_entry_t *this_,
                                                                   const data_relationship_t *data_before
                                                                 );

/*!
 *  \brief gets the new data after the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_relationship_t, where the data after the action is stored
 */
static inline const data_relationship_t *ctrl_undo_redo_entry_get_relationship_after_action_const ( const ctrl_undo_redo_entry_t *this_ );

/*!
 *  \brief gets the old data before the undo redo action
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to data_relationship_t, where the data before the action is stored
 */
static inline const data_relationship_t *ctrl_undo_redo_entry_get_relationship_before_action_const ( const ctrl_undo_redo_entry_t *this_ );

/* ================================ ENTRY_TYPE ================================ */

/*!
 *  \brief gets the action type
 *
 *  \param this_ pointer to own object attributes
 *  \return action type
 */
static inline ctrl_undo_redo_entry_type_t ctrl_undo_redo_entry_get_action_type ( const ctrl_undo_redo_entry_t *this_ );

/* ================================ STATISTICS ================================ */

/*!
 *  \brief adds the action of the ctrl_undo_redo_entry_t to the statistics
 *
 *  \param this_ pointer to own object attributes
 *  \param undo true if the action shall be counted as undone, false if it shall be counted as redone
 *  \param err true if the action shall be counted as error, in this case the undo parameter is ignored.
 *  \param io_stat Statistics on DATA_STAT_SERIES_CREATED, DATA_STAT_SERIES_MODIFIED,
 *                 DATA_STAT_SERIES_DELETED and
 *                 DATA_STAT_SERIES_ERROR (if err).
 *                 *io_stat shall be initialized by caller, statistics are added to initial values.
 */
static inline void ctrl_undo_redo_entry_to_statistics ( const ctrl_undo_redo_entry_t *this_,
                                                        bool undo,
                                                        bool err,
                                                        data_stat_t *io_stat
                                                      );

#include "ctrl_undo_redo_entry.inl"

#endif  /* CTRL_UNDO_REDO_ENTRY_H */


/*
Copyright 2016-2026 Andreas Warnke

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
