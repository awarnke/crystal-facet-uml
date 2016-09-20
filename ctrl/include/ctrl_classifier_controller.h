/* File: ctrl_classifier_controller.h; Copyright and License: see below */

#ifndef CTRL_CLASSIFIER_CONTROLLER_H
#define CTRL_CLASSIFIER_CONTROLLER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides write access and triggers consistency checks to classifiers in the database
 */

#include "ctrl_error.h"
#include "ctrl_consistency_checker.h"
#include "ctrl_undo_redo_list.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "data_classifier.h"
#include "data_classifier_type.h"
#include "data_diagram.h"
#include "data_small_set.h"
#include <stdio.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the classifier functions
 */
struct ctrl_classifier_controller_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t *db_writer;  /*!< pointer to external database writer */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    ctrl_consistency_checker_t consistency_checker;  /*!< own instance of a consistency checker */
    ctrl_undo_redo_list_t *undo_redo_list;  /*!< pointer to external ctrl_undo_redo_list_t */
};

typedef struct ctrl_classifier_controller_struct ctrl_classifier_controller_t;

/*!
 *  \brief initializes the ctrl_classifier_controller_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param undo_redo_list pointer to list of undo/redo actions
 *  \param database pointer to database object
 *  \param db_reader pointer to database reader object that can be used for retrieving data
 *  \param db_writer pointer to database writer object that can be used for changing data
 */
void ctrl_classifier_controller_init ( ctrl_classifier_controller_t *this_,
                                       ctrl_undo_redo_list_t *undo_redo_list,
                                       data_database_t *database,
                                       data_database_reader_t *db_reader,
                                       data_database_writer_t *db_writer
                                     );

/*!
 *  \brief destroys the ctrl_classifier_controller_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_classifier_controller_destroy ( ctrl_classifier_controller_t *this_ );

/*!
 *  \brief creates a new classifier or package or other object and attaches it to a diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram, to which to link the newly created classifier
 *  \param classifier_type main_type of the classifier to create
 *  \param classifier_name name of the classifier to create
 *  \param out_new_id id of the newly created classifier. Note: the id of the newly created diagramelement link is not provided.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_create_classifier_in_diagram ( ctrl_classifier_controller_t *this_,
                                                                       int64_t diagram_id,
                                                                       data_classifier_type_t classifier_type,
                                                                       const char* classifier_name,
                                                                       int64_t* out_new_id
                                                                     );

/*!
 *  \brief updates the classifier attribute: stereotype
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_stereotype new description text of the classifier
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_classifier_stereotype ( ctrl_classifier_controller_t *this_,
                                                                       int64_t classifier_id,
                                                                       const char* new_classifier_stereotype
                                                                     );

/*!
 *  \brief updates the classifier attribute: description
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_description new description text of the classifier
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_classifier_description ( ctrl_classifier_controller_t *this_,
                                                                        int64_t classifier_id,
                                                                        const char* new_classifier_description
                                                                      );

/*!
 *  \brief updates the classifier attribute: name
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_name new name of the classifier
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_classifier_name ( ctrl_classifier_controller_t *this_,
                                                                 int64_t classifier_id,
                                                                 const char* new_classifier_name
                                                               );

/*!
 *  \brief updates the classifier attribute: main_type
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_main_type new main_type of the classifier
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_classifier_main_type ( ctrl_classifier_controller_t *this_,
                                                                      int64_t classifier_id,
                                                                      data_classifier_type_t new_classifier_main_type
                                                                    );

/*!
 *  \brief deletes a set of classifiers, diagramelements, features, relations
 *
 *  \param this_ pointer to own object attributes
 *  \param objects set of object ids to be deleted
 *  \return error id in case of an error, e.g. CTRL_ERROR_INPUT_EMPTY in case of empty set, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_delete_set ( ctrl_classifier_controller_t *this_,
                                                     data_small_set_t objects
                                                   );

/*!
 *  \brief creates a new classifier but does not attach it to a diagram - which leaves the classifier unreferenced.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_classifier data of the new classifier to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created classifier.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_create_classifier ( ctrl_classifier_controller_t *this_,
                                                            const data_classifier_t *new_classifier,
                                                            bool add_to_latest_undo_set,
                                                            int64_t* out_new_id
                                                          );

#endif  /* CTRL_CLASSIFIER_CONTROLLER_H */


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
