/* File: ctrl_diagram_trigger.h; Copyright and License: see below */

#ifndef CTRL_DIAGRAM_TRIGGER_H
#define CTRL_DIAGRAM_TRIGGER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief When triggered by database changes, this class calls consistency routines.
 */

#include "entity/data_id.h"
#include "consistency/consistency_classifier.h"
#include "consistency/consistency_feature.h"
#include "consistency/consistency_lifeline.h"
#include "consistency/consistency_relationship.h"
#include "u8/u8_error.h"

/*!
 *  \brief all data attributes needed for the database trigger
 *
 *  This module is called, triggerd by database changes, to re-establish consistency
 */
struct ctrl_diagram_trigger_struct {
    consistency_classifier_t *classifier;  /*!< pointer to external consistency routines */
    consistency_feature_t *feature;  /*!< pointer to external consistency routines */
    consistency_lifeline_t *lifeline;  /*!< pointer to external consistency routines */
    consistency_relationship_t *relationship;  /*!< pointer to external consistency routines */
};

typedef struct ctrl_diagram_trigger_struct ctrl_diagram_trigger_t;

/*!
 *  \brief initializes the ctrl_diagram_trigger_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier pointer to external consistency routines for dropping invisible classifiers
 *  \param feature pointer to external consistency routines for dropping invisible features
 *  \param lifeline pointer to external consistency routines for creating and destroying lifelines and their references
 *  \param relationship pointer to external consistency routines for dropping invisible relationships
 */
static inline void ctrl_diagram_trigger_init ( ctrl_diagram_trigger_t *this_,
                                               consistency_classifier_t *classifier,
                                               consistency_feature_t *feature,
                                               consistency_lifeline_t *lifeline,
                                               consistency_relationship_t *relationship
                                             );

/*!
 *  \brief destroys the ctrl_diagram_trigger_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_diagram_trigger_destroy ( ctrl_diagram_trigger_t *this_ );

/* ================================ ENTRY POINTS ================================ */

/*!
 *  \brief triggers the execution of policies involved in changing the diagram type.
 *
 *  \param this_ pointer to own object attributes
 *  \param updated_diagram data of the updated diagram.
 *  \param[in,out] io_stat Statistics on created and deleted objects.
 *                         This method is expected to only delete elements, therefore all statistics should be zero or negative.
 *                         *io_stat shall be initialized by caller, statistics are added to initial values.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_diagram_trigger_post_update_diagram_type ( ctrl_diagram_trigger_t *this_,
                                                                         const data_diagram_t *updated_diagram,
                                                                         consistency_stat_t *io_stat
                                                                       );

/*!
 *  \brief triggers the execution of policies involved in creating a diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagramelement data of the new diagramelement.
 *  \param[out] out_created_lifeline a valid id if a lifeline was created, invalid if no lifeline was created.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_diagram_trigger_post_create_diagramelement ( ctrl_diagram_trigger_t *this_,
                                                                           const data_diagramelement_t *new_diagramelement,
                                                                           data_id_t *out_created_lifeline
                                                                         );

/*!
 *  \brief triggers the execution of policies involved in deleting a diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \param[in,out] io_stat Statistics on created and deleted objects.
 *                         This method is expected to only delete elements, therefore all statistics should be zero or negative.
 *                         *io_stat shall be initialized by caller, statistics are added to initial values.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_diagram_trigger_post_delete_diagramelement ( ctrl_diagram_trigger_t *this_,
                                                                           const data_diagramelement_t *deleted_diagramelement,
                                                                           consistency_stat_t *io_stat
                                                                         );

#include "ctrl_diagram_trigger.inl"

#endif  /* CTRL_DIAGRAM_TRIGGER_H */


/*
Copyright 2018-2026 Andreas Warnke

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
