/* File: data_database_consistency_checker.h; Copyright and License: see below */

#ifndef DATA_DATABASE_CONSISTENCY_CHECKER_H
#define DATA_DATABASE_CONSISTENCY_CHECKER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief checks a database and provides utility functinos to repair a database
 */

#include "storage/data_database.h"
#include "data_diagram.h"
#include "data_error.h"
#include "data_classifier.h"
#include "set/data_id_pair.h"
#include "set/data_visible_classifier.h"
#include "set/data_small_set.h"
#include "data_row_id.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief constants of data_database_consistency_checker_t
 */
enum data_database_consistency_checker_const_enum {
    DATA_DATABASE_CONSISTENCY_CHECKER_MAX_TEMP_DIAG_IDS = 8192,  /*!< maximum size of the diagram id buffer */
};

/*!
 *  \brief all data attributes needed for the database consistency checker functions
 */
struct data_database_consistency_checker_struct {
    data_database_t *database;  /*!< pointer to external database */

    data_row_id_t private_temp_diagram_ids_buf[DATA_DATABASE_CONSISTENCY_CHECKER_MAX_TEMP_DIAG_IDS][2];  /*!< buffer to store a temporary diag ids list */
};

typedef struct data_database_consistency_checker_struct data_database_consistency_checker_t;

/*!
 *  \brief initializes the data_database_consistency_checker_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this checker uses
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_consistency_checker_init ( data_database_consistency_checker_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_consistency_checker_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_consistency_checker_destroy ( data_database_consistency_checker_t *this_ );

/*!
 *  \brief determines the set of diagrams from the database that reference each other as parent,
 *         also invalid parent-references are found.
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of circular-referencing diagrams (invalid diagrams.parent_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_circular_diagram_parents ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads all diagram ids and all their parent-ids from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_diagram_id_pair array of diagram-id-pairs read from the database (in case of success).
 *                             The second id of the pair is the parent id - without check if this parent record exists.
 *  \param out_diagram_id_pair_count number of diagram records stored in out_diagram
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_consistency_checker_private_get_diagram_ids ( data_database_consistency_checker_t *this_,
                                                                         uint32_t max_out_array_size,
                                                                         data_row_id_t (*out_diagram_id_pair)[][2],
                                                                         uint32_t *out_diagram_id_pair_count
                                                                       );

/*!
 *  \brief reads the set of nonreferencing diagramelements from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of unreferenced diagramelements (invalid diagramelements.diagram_id or diagramelements.classifier_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_nonreferencing_diagramelements ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads the set of diagramelements where the focused_feature_id points to invalid features
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of diagramelements where the focused_feature_id is not NULL but points to invalid feature.
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_invalid_focused_features ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads the set of unreferenced classifiers from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of unreferenced classifiers (no references from diagramelements.classifier_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_unreferenced_classifiers ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads the set of unreferenced features from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of unreferenced features (invalid features.classifier_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_unreferenced_features ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads the set of unreferenced relationships from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of unreferenced relationships (invalid relationships.from_classifier_id or relationships.to_classifier_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_unreferenced_relationships ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads the set of relationships and features, reports relationships where the to/from feature ids are not NULL but invalid
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of invalid feature relationships, where to/from_feature_id is not existing or not in line with to/from_classifier_id.
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_invalid_relationship_features ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief deletes a classifier record even if it is still referenced
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id of the classifier record to be deleted.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error; DATA_ERROR_READ_ONLY_DB if read only.
 */
data_error_t data_database_consistency_checker_kill_classifier( data_database_consistency_checker_t *this_, data_row_id_t obj_id );

#endif  /* DATA_DATABASE_CONSISTENCY_CHECKER_H */


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
