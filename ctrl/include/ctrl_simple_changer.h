/* File: ctrl_simple_changer.h; Copyright and License: see below */

#ifndef CTRL_SIMPLE_CHANGER_H
#define CTRL_SIMPLE_CHANGER_H

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
#include "storage/data_database_reader.h"

/*!
 *  \brief all data attributes needed to perform multiple actions on the controller
 *
 *  Lifecycle: A ctrl_simple_changer_t shall perform a single set of operations only.
 *  It may be initialized before one set of actions and be destroyed afterwards.
 *
 *  All operations that are performed using one instance of this class are stored to the same undo/redo action set.
 */
struct ctrl_simple_changer_struct {
    ctrl_controller_t* controller;  /*!< pointer to external classifier controller */
    data_database_reader_t* db_reader;  /*!< pointer to external database reader */
};

typedef struct ctrl_simple_changer_struct ctrl_simple_changer_t;

/*!
 *  \brief initializes the ctrl_simple_changer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param controller pointer to a controller object which can modify the database
 *  \param db_reader pointer to a database reader object
 */
static inline void ctrl_simple_changer_init ( ctrl_simple_changer_t *this_,
                                              ctrl_controller_t *controller,
                                              data_database_reader_t *db_reader
                                            );

/*!
 *  \brief destroys the ctrl_simple_changer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void ctrl_simple_changer_destroy ( ctrl_simple_changer_t *this_ );

/*!
 *  \brief updates the classifier attribute: name
 *
 *  Additionally to ctrl_classifier_controller, this function tries to preserve the proposed name.
 *  If this is duplicate, it generates a new, unique name by appending a number to the name.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_name new name of the classifier
 *  \param[out] out_info U8_ERROR_DUPLICATE_NAME if an alternative name was set, U8_ERROR_NONE in case of success.
 *  \return error id in case of an unhandled error, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_simple_changer_update_classifier_name ( ctrl_simple_changer_t *this_,
                                                                      data_row_t classifier_id,
                                                                      const char* new_classifier_name,
                                                                      u8_error_t* out_info
                                                                    );

/*!
 *  \brief create a name proposal for a classifier based on a base_name and an interation-counter
 *
 *  \param this_ pointer to own object attributes
 *  \param base_classifier_name expected new name of the classifier
 *  \param iteration number of alredy failed tries to find an unused, unique name, range 2..9999
 *  \param[out] out_name proposed new name of the classifier
 *  \return UTF8ERROR_TRUNCATED in case the provided out_name buffer is too small, U8_ERROR_NONE otherwise
 */
static inline u8_error_t ctrl_simple_changer_private_propose_classifier_name ( ctrl_simple_changer_t *this_,
                                                                               const char* base_classifier_name,
                                                                               uint_fast16_t iteration,
                                                                               utf8stringbuf_t out_name
                                                                             );

#include "ctrl_simple_changer.inl"

#endif  /* CTRL_SIMPLE_CHANGER_H */


/*
Copyright 2023-2024 Andreas Warnke

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
