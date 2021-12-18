/* File: ctrl_multi_step_changer.h; Copyright and License: see below */

#ifndef CTRL_MULTI_STEP_CHANGER_H
#define CTRL_MULTI_STEP_CHANGER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides an entry point for write access and consistency checks to the database
 */

#include "u8/u8_error.h"
#include "ctrl_controller.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"

/*!
 *  \brief all data attributes needed to perform multiple actions on the controller
 *
 *  Lifecycle: A ctrl_multi_step_changer_t shall perform a single set of operations only.
 *  It may be initialized before one set of actions and be destroyed afterwards.
 */
struct ctrl_multi_step_changer_struct {
    ctrl_controller_t* controller;  /*!< pointer to external classifier controller */
    data_database_reader_t* db_reader;  /*!< pointer to external database reader */
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

/* ================================ interface for sets of elements ================================ */

/*!
 *  \brief deletes a set of classifiers, diagramelements, features, relations
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

#endif  /* CTRL_MULTI_STEP_CHANGER_H */


/*
Copyright 2016-2021 Andreas Warnke

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
