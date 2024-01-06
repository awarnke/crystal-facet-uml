/* File: ctrl_diagram_trigger.h; Copyright and License: see below */

#ifndef CTRL_DIAGRAM_TRIGGER_H
#define CTRL_DIAGRAM_TRIGGER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief When triggered by database changes, this class calls consistency routines.
 */

#include "u8/u8_error.h"
#include "consistency/consistency_drop_invisibles.h"
#include "consistency/consistency_lifeline.h"

/*!
 *  \brief all data attributes needed for the database trigger
 *
 *  This module is called, triggerd by database changes, to re-establish consistency
 */
struct ctrl_diagram_trigger_struct {
    consistency_drop_invisibles_t *drop_invisibles;  /*!< pointer to external consistency routines */
    consistency_lifeline_t *lifeline;  /*!< pointer to external consistency routines */
};

typedef struct ctrl_diagram_trigger_struct ctrl_diagram_trigger_t;

/*!
 *  \brief initializes the ctrl_diagram_trigger_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param drop_invisibles pointer to external consistency routines for dropping invisible elements
 *  \param lifeline pointer to external consistency routines for creating and destroying lifelines and their references
 */
static inline void ctrl_diagram_trigger_init ( ctrl_diagram_trigger_t *this_,
                                               consistency_drop_invisibles_t *drop_invisibles,
                                               consistency_lifeline_t *lifeline
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
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_diagram_trigger_post_update_diagram_type ( ctrl_diagram_trigger_t *this_,
                                                                         const data_diagram_t *updated_diagram
                                                                       );

/*!
 *  \brief triggers the execution of policies involved in creating a diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_diagramelement data of the new diagramelement.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_diagram_trigger_post_create_diagramelement ( ctrl_diagram_trigger_t *this_,
                                                                           const data_diagramelement_t *new_diagramelement
                                                                         );

/*!
 *  \brief triggers the execution of policies involved in deleting a diagramelement.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_diagramelement data of the deleted diagramelement.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_diagram_trigger_post_delete_diagramelement ( ctrl_diagram_trigger_t *this_,
                                                                           const data_diagramelement_t *deleted_diagramelement
                                                                         );

#include "ctrl_diagram_trigger.inl"

#endif  /* CTRL_DIAGRAM_TRIGGER_H */


/*
Copyright 2018-2024 Andreas Warnke

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
