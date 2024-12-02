/* File: ctrl_multi_step_changer.h; Copyright and License: see below */

#ifndef CTRL_MULTI_STEP_CHANGER_H
#define CTRL_MULTI_STEP_CHANGER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides an entry point for write access and consistency checks to the database
 *
 *  ctrl_classifier_controller_t and ctrl_diagram_controller_t enforce consistency either by calling trigger functions
 *  (which may call the consistency package to perform additional actions) or by failing if operations are not allowed.
 *  ctrl_simple_changer_t and ctrl_multistep_changer_t try to find a solution where the database is always consistent,
 *  e.g. by searching or re-sorting actions or by performing additional steps.
 */

#include "u8/u8_error.h"
#include "ctrl_controller.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "ctrl_undo_redo_action_boundary.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"

/*!
 *  \brief all data attributes needed to perform multiple actions on the controller
 *
 *  Lifecycle: A ctrl_multi_step_changer_t shall perform a single set of operations only.
 *  It may be initialized before one set of actions and be destroyed afterwards.
 *
 *  All operations that are performed using one instance of this class are stored to the same undo/redo action set.
 */
struct ctrl_multi_step_changer_struct {
    ctrl_controller_t* controller;  /*!< pointer to external classifier controller */
    data_database_reader_t* db_reader;  /*!< pointer to external database reader */

    ctrl_undo_redo_action_boundary_t is_first_step;  /*!< keep track if to add a boundary to the undo redo list */
};

typedef struct ctrl_multi_step_changer_struct ctrl_multi_step_changer_t;

/*!
 *  \brief initializes the ctrl_multi_step_changer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to a controller object which can modify the database
 *  \param db_reader pointer to a database reader object
 */
void ctrl_multi_step_changer_init ( ctrl_multi_step_changer_t *this_,
                                    ctrl_controller_t *controller,
                                    data_database_reader_t *db_reader
                                  );

/*!
 *  \brief destroys the ctrl_multi_step_changer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_multi_step_changer_destroy ( ctrl_multi_step_changer_t *this_ );

/* ================================ delete sets of elements ================================ */

/*!
 *  \brief deletes a set of diagrams, diagramelements, classifiers, features, relations
 *
 *  \param this_ pointer to own object attributes
 *  \param objects set of object ids to be deleted
 *  \param[in,out] io_stat Statistics on DATA_STAT_SERIES_DELETED and
 *                         DATA_STAT_SERIES_ERROR (e.g. if a diagram still contains objects).
 *                         *io_stat shall be initialized by caller, statistics are added to initial values.
 *  \return error id in case of an error, e.g. U8_ERROR_INPUT_EMPTY in case of empty set, U8_ERROR_NONE otherwise
 */
u8_error_t ctrl_multi_step_changer_delete_set ( ctrl_multi_step_changer_t *this_,
                                                const data_small_set_t *objects,
                                                data_stat_t *io_stat
                                              );

/* ================================ create elements without duplicate ids ================================ */

/*!
 *  \brief creates a new diagram.
 *
 *  Additionally to ctrl_diagram_controller, this function tries to preserve the proposed id.
 *  If this is duplicate, it generates a new one.
 *
 *  \param this_ pointer to own object attributes
 *  \param[in,out] new_diagram data of the new diagram to be created; the id is taken as proposal.
 *                             The id may be changed when returning from this function.
 *  \param[out] out_info U8_ERROR_DUPLICATE_ID if an alternative id was created, U8_ERROR_NONE in case of success.
 *  \return error id in case of an unhandled error, U8_ERROR_NONE otherwise
 */

u8_error_t ctrl_multi_step_changer_create_diagram ( ctrl_multi_step_changer_t *this_,
                                                    data_diagram_t *new_diagram,
                                                    u8_error_t* out_info
                                                  );

/*!
 *  \brief creates a new diagramelement.
 *
 *  Additionally to ctrl_diagram_controller, this function tries to preserve the proposed id.
 *  If this is duplicate, it generates a new one.
 *
 *  \param this_ pointer to own object attributes
 *  \param[in,out] new_diagramelement data of the new diagramelement to be created; the id is taken as proposal.
 *                                    The id may be changed when returning from this function.
 *  \param[out] out_info U8_ERROR_DUPLICATE_ID if an alternative id was created, U8_ERROR_NONE in case of success.
 *  \return error id in case of an unhandled error, U8_ERROR_NONE otherwise
 */
u8_error_t ctrl_multi_step_changer_create_diagramelement ( ctrl_multi_step_changer_t *this_,
                                                           data_diagramelement_t *new_diagramelement,
                                                           u8_error_t* out_info
                                                         );


/*!
 *  \brief creates a new classifier but does not attach it to a diagram - which leaves the classifier unreferenced.
 *
 *  Additionally to ctrl_classifier_controller, this function tries to preserve the proposed id and name.
 *  If these are duplicate, it generates new ids and/or names.
 *
 *  \param this_ pointer to own object attributes
 *  \param[in,out] new_classifier data of the new classifier to be created; id and name are taken as proposal.
 *                                The id and the name may be changed when returning from this function.
 *  \param[out] out_info U8_ERROR_DUPLICATE_ID and/or U8_ERROR_DUPLICATE_NAME
 *                       if an alternative id and/or name were created, U8_ERROR_NONE in case of success.
 *  \return error id in case of an unhandled error, U8_ERROR_NONE otherwise
 */
u8_error_t ctrl_multi_step_changer_create_classifier ( ctrl_multi_step_changer_t *this_,
                                                       data_classifier_t *new_classifier,
                                                       u8_error_t* out_info
                                                     );

/*!
 *  \brief creates a new feature.
 *
 *  Additionally to ctrl_classifier_controller, this function tries to preserve the proposed id.
 *  If this is duplicate, it generates a new one.
 *
 *  \param this_ pointer to own object attributes
 *  \param[in,out] new_feature data of the new feature to be created; the id is taken as proposal.
 *                             The id may be changed when returning from this function.
 *  \param[out] out_info U8_ERROR_DUPLICATE_ID if an alternative id was created, U8_ERROR_NONE in case of success.
 *  \return error id in case of an unhandled error, U8_ERROR_NONE otherwise
 */
u8_error_t ctrl_multi_step_changer_create_feature ( ctrl_multi_step_changer_t *this_,
                                                    data_feature_t *new_feature,
                                                    u8_error_t* out_info
                                                  );

/*!
 *  \brief creates a new relationship.
 *
 *  Additionally to ctrl_classifier_controller, this function tries to preserve the proposed id.
 *  If this is duplicate, it generates a new one.
 *
 *  \param this_ pointer to own object attributes
 *  \param[in,out] new_relationship data of the new relationship to be created; the id is taken as proposal.
 *                                  The id may be changed when returning from this function.
 *  \param[out] out_info U8_ERROR_DUPLICATE_ID if an alternative id was created, U8_ERROR_NONE in case of success.
 *  \return error id in case of an unhandled error, U8_ERROR_NONE otherwise
 */
u8_error_t ctrl_multi_step_changer_create_relationship ( ctrl_multi_step_changer_t *this_,
                                                         data_relationship_t *new_relationship,
                                                         u8_error_t* out_info
                                                       );

/* ================================ update links of existing elements ================================ */

/*!
 *  \brief updates the diagram attribute: parent_id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_parent_id new parent_id of the diagram
 *  \return error id in case of an error, U8_ERROR_NONE otherwise
 */
u8_error_t ctrl_multi_step_changer_update_diagram_parent_id ( ctrl_multi_step_changer_t *this_,
                                                              data_row_t diagram_id,
                                                              data_row_t new_diagram_parent_id
                                                            );

/* ================================ propose names of classifiers ================================ */

/*!
 *  \brief create a name proposal for a classifier based on a base_name and an iteration-counter
 *
 *  \param this_ pointer to own object attributes
 *  \param base_classifier_name expected new name of the classifier
 *  \param iteration number of alredy failed tries to find an unused, unique name, range 2..9999
 *  \param[out] out_name proposed new name of the classifier
 *  \return UTF8ERROR_TRUNCATED in case the provided out_name buffer is too small, U8_ERROR_NONE otherwise
 */
u8_error_t ctrl_multi_step_changer_private_propose_classifier_name ( ctrl_multi_step_changer_t *this_,
                                                                     const char* base_classifier_name,
                                                                     uint_fast16_t iteration,
                                                                     utf8stringbuf_t out_name
                                                                   );

#endif  /* CTRL_MULTI_STEP_CHANGER_H */


/*
Copyright 2016-2024 Andreas Warnke

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
