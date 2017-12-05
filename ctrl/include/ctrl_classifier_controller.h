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
#include "data_feature.h"
#include "data_relationship.h"
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

/* ================================ CLASSIFIER ================================ */

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

/*!
 *  \brief deletes a classifier record and associated relationships and features
 *         and stores all actions in the undo redo list.
 *
 *  Note: The classifier is not deleted if still referenced by diagramelements.
 *  Note: No boundary is added to the undo redo list. Therefore this function is private.
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id of the classifier record to be deleted.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
ctrl_error_t ctrl_classifier_controller_private_delete_classifier( ctrl_classifier_controller_t *this_, int64_t obj_id );

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
 *  \brief updates the classifier attribute: x_order
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_x_order new x_order of the classifier
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_classifier_x_order ( ctrl_classifier_controller_t *this_,
                                                                    int64_t classifier_id,
                                                                    int32_t new_classifier_x_order
                                                                  );

/*!
 *  \brief updates the classifier attribute: y_order
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_y_order new y_order of the classifier
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_classifier_y_order ( ctrl_classifier_controller_t *this_,
                                                                    int64_t classifier_id,
                                                                    int32_t new_classifier_y_order
                                                                  );

/*!
 *  \brief updates the classifier attributes: x_order and y_order
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_x_order new x_order of the classifier
 *  \param new_classifier_y_order new y_order of the classifier
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_classifier_x_order_y_order ( ctrl_classifier_controller_t *this_,
                                                                            int64_t classifier_id,
                                                                            int32_t new_classifier_x_order,
                                                                            int32_t new_classifier_y_order
                                                                          );

/* ================================ COMMON ================================ */

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

/* ================================ FEATURE ================================ */

/*!
 *  \brief creates a new feature.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_feature data of the new feature to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created feature, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_create_feature ( ctrl_classifier_controller_t *this_,
                                                         const data_feature_t *new_feature,
                                                         bool add_to_latest_undo_set,
                                                         int64_t* out_new_id
                                                       );

/*!
 *  \brief updates the feature attribute: main_type
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_type new main_type of the feature
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_feature_main_type ( ctrl_classifier_controller_t *this_,
                                                                   int64_t feature_id,
                                                                   data_feature_type_t new_feature_type
                                                                 );

/*!
 *  \brief updates the feature attribute: key
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_key new key of the feature
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_feature_key ( ctrl_classifier_controller_t *this_,
                                                             int64_t feature_id,
                                                             const char* new_feature_key
                                                           );

/*!
 *  \brief updates the feature attribute: value
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_value new value of the feature
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_feature_value ( ctrl_classifier_controller_t *this_,
                                                               int64_t feature_id,
                                                               const char* new_feature_value
                                                             );

/*!
 *  \brief updates the feature attribute: description
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_description new description text of the feature
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_feature_description ( ctrl_classifier_controller_t *this_,
                                                                     int64_t feature_id,
                                                                     const char* new_feature_description
                                                                   );

/*!
 *  \brief updates the feature attribute: list_order
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_list_order new list_order of the feature
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_feature_list_order ( ctrl_classifier_controller_t *this_,
                                                                    int64_t feature_id,
                                                                    int32_t new_feature_list_order
                                                                  );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief creates a new relationship.
 *
 *  \param this_ pointer to own object attributes
 *  \param new_relationship data of the new relationship to be created; the id is ignored.
 *  \param add_to_latest_undo_set true if this add-action shall be merged to the last set of actions in the undo_redo_list_t,
 *                                false if a new boundary shall be created in the undo_redo_list_t.
 *  \param out_new_id id of the newly created relationship, NULL if the new id is not needed.
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_create_relationship ( ctrl_classifier_controller_t *this_,
                                                              const data_relationship_t *new_relationship,
                                                              bool add_to_latest_undo_set,
                                                              int64_t* out_new_id
                                                            );

/*!
 *  \brief updates the relationship attribute: main_type
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the diagram to be updated
 *  \param new_relationship_type new main_type of the diagram
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_relationship_main_type ( ctrl_classifier_controller_t *this_,
                                                                        int64_t relationship_id,
                                                                        data_relationship_type_t new_relationship_type
                                                                      );

/*!
 *  \brief updates the relationship attribute: name
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship to be updated
 *  \param new_relationship_name new name of the relationship
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_relationship_name ( ctrl_classifier_controller_t *this_,
                                                                   int64_t relationship_id,
                                                                   const char* new_relationship_name
                                                                 );

/*!
 *  \brief updates the relationship attribute: description
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship to be updated
 *  \param new_relationship_description new description text of the relationship
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_relationship_description ( ctrl_classifier_controller_t *this_,
                                                                          int64_t relationship_id,
                                                                          const char* new_relationship_description
                                                                        );

/*!
 *  \brief updates the relationship attribute: list_order
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship to be updated
 *  \param new_relationship_list_order new list_order of the relationship
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_update_relationship_list_order ( ctrl_classifier_controller_t *this_,
                                                                         int64_t relationship_id,
                                                                         int32_t new_relationship_list_order
                                                                       );

#endif  /* CTRL_CLASSIFIER_CONTROLLER_H */


/*
Copyright 2016-2017 Andreas Warnke

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
