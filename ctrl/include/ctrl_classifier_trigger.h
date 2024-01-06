/* File: ctrl_classifier_trigger.h; Copyright and License: see below */

#ifndef CTRL_CLASSIFIER_TRIGGER_H
#define CTRL_CLASSIFIER_TRIGGER_H

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
struct ctrl_classifier_trigger_struct {
    consistency_drop_invisibles_t *drop_invisibles;  /*!< pointer to external consistency routines */
    consistency_lifeline_t *lifeline;  /*!< pointer to external consistency routines */
};

typedef struct ctrl_classifier_trigger_struct ctrl_classifier_trigger_t;

/*!
 *  \brief initializes the ctrl_classifier_trigger_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param drop_invisibles pointer to external consistency routines for dropping invisible elements
 *  \param lifeline pointer to external consistency routines for creating and destroying lifelines and their references
 */
static inline void ctrl_classifier_trigger_init ( ctrl_classifier_trigger_t *this_,
                                                  consistency_drop_invisibles_t *drop_invisibles,
                                                  consistency_lifeline_t *lifeline
                                                );

/*!
 *  \brief destroys the ctrl_classifier_trigger_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_classifier_trigger_destroy ( ctrl_classifier_trigger_t *this_ );

/* ================================ ENTRY POINTS ================================ */

/*!
 *  \brief triggers the execution of policies involved in deleting a feature.
 *
 *  \param this_ pointer to own object attributes
 *  \param deleted_feature data of the deleted feature.
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_classifier_trigger_post_delete_feature ( ctrl_classifier_trigger_t *this_,
                                                                       const data_feature_t *deleted_feature
                                                                     );

#include "ctrl_classifier_trigger.inl"

#endif  /* CTRL_CLASSIFIER_TRIGGER_H */


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
