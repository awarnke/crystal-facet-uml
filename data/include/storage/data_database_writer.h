/* File: data_database_writer.h; Copyright and License: see below */

#ifndef DATA_DATABASE_WRITER_H
#define DATA_DATABASE_WRITER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Creates, modifies and deletes records in the database
 */

#include "storage/data_database.h"
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

    pthread_mutex_t private_lock; /*!< lock to ensure that all private attributes are used by only one thread */
    utf8stringbuf_t private_temp_stringbuf;
    utf8stringbuf_t private_sql_stringbuf;
    char private_temp_buffer[8192];
    char private_sql_buffer[8192];
};

typedef struct data_database_writer_struct data_database_writer_t;

/*!
 *  \brief initializes the data_database_writer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_writer_init ( data_database_writer_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_writer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_writer_destroy ( data_database_writer_t *this_ );

/*!
 *  \brief creates a new diagram and returns its id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram data of the new diagram record to be created.
 *  \param out_new_id storage, where the id of the newly created record is stored.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_create_diagram ( data_database_writer_t *this_, const data_diagram_t *diagram, int64_t* out_new_id );

/*!
 *  \brief updates the diagram attribute: description
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_description new description text of the diagram
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagram_description ( data_database_writer_t *this_, int64_t diagram_id, const char* new_diagram_description );

/*!
 *  \brief updates the diagram attribute: name
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_name new name of the diagram
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagram_name ( data_database_writer_t *this_, int64_t diagram_id, const char* new_diagram_name );

/*!
 *  \brief updates the diagram attribute: diagram_type
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_type new diagram_type of the diagram
 *  \return error id in case of an error, DATA_ERROR_NONE otherwise
 */
data_error_t data_database_writer_update_diagram_type ( data_database_writer_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type );

/*!
 *  \brief creates a new classifier and returns its id
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier data of the new classifier record to be created.
 *  \param out_new_id storage, where the id of the newly created record is stored.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_create_classifier( data_database_writer_t *this_, const data_classifier_t *classifier, int64_t* out_new_id );

/*!
 *  \brief creates a new diagramelement and returns its id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement data of the new diagramelement record to be created.
 *  \param out_new_id storage, where the id of the newly created record is stored.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_create_diagramelement( data_database_writer_t *this_, const data_diagramelement_t *diagramelement, int64_t* out_new_id );

/*!
 *  \brief builds the sql command string to create a new diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram diagram to be created.
 */
data_error_t data_database_writer_private_build_create_diagram_command ( data_database_writer_t *this_, const data_diagram_t *diagram );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_name new name of the diagram
 */
data_error_t data_database_writer_private_build_update_diagram_name_cmd ( data_database_writer_t *this_, int64_t diagram_id, const char *new_diagram_name );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_description new description of the diagram
 */
data_error_t data_database_writer_private_build_update_diagram_description_cmd ( data_database_writer_t *this_, int64_t diagram_id, const char *new_diagram_description );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_type new type of the diagram
 */
data_error_t data_database_writer_private_build_update_diagram_type_cmd ( data_database_writer_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type );

/*!
 *  \brief executes a single SQL command
 *
 *  This function does not care about locks. It does not sent notifications.
 *
 *  \param this_ pointer to own object attributes
 *  \param sql_statement statement to be created.
 *  \param fetch_new_id true if the statement creates a new row of which the id shall be returned
 *  \param out_new_id if fetch_new_id, the id of the newly created row is returned
 *  \return DATA_ERROR_NONE in case of success, an error id otherwise
 */
data_error_t data_database_writer_private_execute_single_command ( data_database_writer_t *this_, const char* sql_statement, bool fetch_new_id, int64_t* out_new_id );

/*!
 *  \brief builds the sql command string to create a new classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier classifier to be created.
 */
data_error_t data_database_writer_private_build_create_classifier_command ( data_database_writer_t *this_, const data_classifier_t *classifier );

/*!
 *  \brief builds the sql command string to create a new diagramelement record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement diagramelement to be created.
 */
data_error_t data_database_writer_private_build_create_diagramelement_command ( data_database_writer_t *this_, const data_diagramelement_t *diagramelement );

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
