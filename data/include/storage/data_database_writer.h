/* File: data_database_writer.h; Copyright and License: see below */

#ifndef DATA_DATABASE_WRITER_H
#define DATA_DATABASE_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Creates, modifies and deletes records in the database
 *
 *  Note: The database_writer uses a database_reader to execute select (implemented in the reader),
 *  delete and maybe update in a single transaction.
 */

#include "storage/data_database_listener_signal.h"
#include "storage/data_database.h"
#include "storage/data_database_reader.h"
#include "storage/data_database_sql_builder.h"
#include "data_diagram.h"
#include "data_error.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "util/string/utf8stringbuf.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_writer_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader which may be queried within write-transactions */

    pthread_mutex_t private_lock; /*!< lock to ensure that all private attributes are used by only one thread */
    data_database_sql_builder_t sql_builder; /*!< own instance of an object that builds sql strings */

    data_database_listener_t me_as_listener;  /*!< own instance of data_database_listener_t which wraps data_database_writer_db_change_callback */
};

typedef struct data_database_writer_struct data_database_writer_t;

/*!
 *  \brief initializes the data_database_writer_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object which is used for ensuring consistency and reading db-objects before changing
 *  \param database pointer to a database object which is used for writing
 */
void data_database_writer_init ( data_database_writer_t *this_, data_database_reader_t *db_reader, data_database_t *database );

/*!
 *  \brief destroys the data_database_writer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_writer_destroy ( data_database_writer_t *this_ );

/*!
 *  \brief prepares a database change and re-initializes afterwards
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_id state of database change
 */
void data_database_writer_db_change_callback ( data_database_writer_t *this_, data_database_listener_signal_t signal_id );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief creates a new diagram and returns its id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram data of the new diagram record to be created. The id should be DATA_ID_VOID_ID unless a diagram with known, unique id shall be created.
 *  \param out_new_id storage, where the id of the newly created record is stored. NULL if old data shall not be returned.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_create_diagram ( data_database_writer_t *this_, const data_diagram_t *diagram, int64_t* out_new_id );

/*!
 *  \brief deletes a diagram record and returns its old data
 *
 *  Note: This function fails if the diagram is still referenced.
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id of the diagram record to be deleted.
 *  \param out_old_diagram storage, where the contents of the deleted, old record is stored. NULL if old data shall not be returned.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_delete_diagram ( data_database_writer_t *this_, int64_t obj_id, data_diagram_t *out_old_diagram );

/*!
 *  \brief updates the diagram attribute: description
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_description new description text of the diagram
 *  \param out_old_diagram storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagram_description ( data_database_writer_t *this_, int64_t diagram_id, const char* new_diagram_description, data_diagram_t *out_old_diagram );

/*!
 *  \brief updates the diagram attribute: name
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_name new name of the diagram
 *  \param out_old_diagram storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagram_name ( data_database_writer_t *this_, int64_t diagram_id, const char* new_diagram_name, data_diagram_t *out_old_diagram );

/*!
 *  \brief updates the diagram attribute: diagram_type
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_type new diagram_type of the diagram
 *  \param out_old_diagram storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagram_type ( data_database_writer_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type, data_diagram_t *out_old_diagram );

/*!
 *  \brief updates the diagram attribute: list_order
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_list_order new list_order of the diagram
 *  \param out_old_diagram storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagram_list_order ( data_database_writer_t *this_, int64_t diagram_id, int32_t new_diagram_list_order, data_diagram_t *out_old_diagram );

/*!
 *  \brief updates the diagram attribute: parent_id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_parent_id new parent diagram id of the diagram
 *  \param out_old_diagram storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagram_parent_id ( data_database_writer_t *this_, int64_t diagram_id, int64_t new_diagram_parent_id, data_diagram_t *out_old_diagram );

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief creates a new classifier and returns its id
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier data of the new classifier record to be created. The id should be DATA_ID_VOID_ID unless a classifier with known, unique id shall be created.
 *  \param out_new_id storage, where the id of the newly created record is stored. NULL if old data shall not be returned.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_create_classifier( data_database_writer_t *this_, const data_classifier_t *classifier, int64_t* out_new_id );

/*!
 *  \brief deletes a classifier record and returns its old data
 *
 *  Note: This function fails if the classifier is still referenced.
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id of the classifier record to be deleted.
 *  \param out_old_classifier storage, where the contents of the deleted, old record is stored. NULL if old data shall not be returned.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_delete_classifier( data_database_writer_t *this_, int64_t obj_id, data_classifier_t *out_old_classifier );

/*!
 *  \brief updates the classifier attribute: stereotype
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_stereotype new stereotype text of the classifier
 *  \param out_old_classifier storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_classifier_stereotype ( data_database_writer_t *this_, int64_t classifier_id, const char* new_classifier_stereotype, data_classifier_t *out_old_classifier );

/*!
 *  \brief updates the classifier attribute: description
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_description new description text of the classifier
 *  \param out_old_classifier storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_classifier_description ( data_database_writer_t *this_, int64_t classifier_id, const char* new_classifier_description, data_classifier_t *out_old_classifier );

/*!
 *  \brief updates the classifier attribute: name
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_name new name of the classifier
 *  \param out_old_classifier storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_classifier_name ( data_database_writer_t *this_, int64_t classifier_id, const char* new_classifier_name, data_classifier_t *out_old_classifier );

/*!
 *  \brief updates the classifier attribute: main_type
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_main_type new main_type of the classifier
 *  \param out_old_classifier storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_classifier_main_type ( data_database_writer_t *this_, int64_t classifier_id, data_classifier_type_t new_classifier_main_type, data_classifier_t *out_old_classifier );

/*!
 *  \brief updates the classifier attribute: x_order
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_x_order new x_order of the classifier
 *  \param out_old_classifier storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_classifier_x_order ( data_database_writer_t *this_, int64_t classifier_id, int32_t new_classifier_x_order, data_classifier_t *out_old_classifier );

/*!
 *  \brief updates the classifier attribute: y_order
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_y_order new y_order of the classifier
 *  \param out_old_classifier storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_classifier_y_order ( data_database_writer_t *this_, int64_t classifier_id, int32_t new_classifier_y_order, data_classifier_t *out_old_classifier );

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief creates a new diagramelement and returns its id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement data of the new diagramelement record to be created. The id should be DATA_ID_VOID_ID unless a diagramelement with known, unique id shall be created.
 *  \param out_new_id storage, where the id of the newly created record is stored. NULL if old data shall not be returned.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_create_diagramelement( data_database_writer_t *this_, const data_diagramelement_t *diagramelement, int64_t* out_new_id );

/*!
 *  \brief deletes a diagramelement record and returns its old data
 *
 *  \param this_ pointer to own object attributes
 *  \param obj_id id of the diagramelement record to be deleted.
 *  \param out_old_diagramelement storage, where the contents of the deleted, old record is stored. NULL if old data shall not be returned.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_delete_diagramelement( data_database_writer_t *this_, int64_t obj_id, data_diagramelement_t *out_old_diagramelement );

/*!
 *  \brief updates the diagramelement attribute: display_flags
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement to be updated
 *  \param new_display_flags new display flags of the diagramelement
 *  \param out_old_diagramelement storage, where the contents of the old, unmodified record is stored. NULL if old data shall not be returned.
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagramelement_display_flags ( data_database_writer_t *this_, int64_t diagramelement_id, data_diagramelement_flag_t new_display_flags, data_diagramelement_t *out_old_diagramelement );

/* ================================ private ================================ */

/*!
 *  \brief executes a single SQL command
 *
 *  This function does not care about locks. It does not sent notifications.
 *
 *  \param this_ pointer to own object attributes
 *  \param sql_statement statement to be executed.
 *  \param out_new_id if fetch_new_id, the id of the newly created row is returned
 *  \return DATA_ERROR_NONE in case of success, an error id otherwise
 */
data_error_t data_database_writer_private_execute_create_command ( data_database_writer_t *this_, const char* sql_statement, int64_t* out_new_id );

/*!
 *  \brief executes a "BEGIN TRANSACTION" command
 *
 *  This function does not care about locks. It does not sent notifications.
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success, an error id otherwise, e.g. DATA_ERROR_NO_DB in case the database is not open
 */
data_error_t data_database_writer_private_transaction_begin ( data_database_writer_t *this_ );

/*!
 *  \brief executes a "COMMIT TRANSACTION" command
 *
 *  This function does not care about locks. It does not sent notifications.
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success, an error id otherwise
 */
data_error_t data_database_writer_private_transaction_commit ( data_database_writer_t *this_ );

/*!
 *  \brief sends one SQL command to the database within a transaction
 *
 *  This function does not care about locks. It does not sent notifications.
 *
 *  \param this_ pointer to own object attributes
 *  \param sql_statement statement to be executed.
 *  \return DATA_ERROR_NONE in case of success, an error id otherwise
 */
data_error_t data_database_writer_private_transaction_issue_command ( data_database_writer_t *this_, const char* sql_statement );

/*!
 *  \brief gets a lock to protect data in data_database_writer_t from concurrent access.
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
static inline data_error_t data_database_writer_private_lock ( data_database_writer_t *this_ );

/*!
 *  \brief releases the lock.
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
static inline data_error_t data_database_writer_private_unlock ( data_database_writer_t *this_ );

#include "storage/data_database_writer.inl"

#endif  /* DATA_DATABASE_WRITER_H */


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
