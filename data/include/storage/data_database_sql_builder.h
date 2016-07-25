/* File: data_database_sql_builder.h; Copyright and License: see below */

#ifndef DATA_DATABASE_SQL_BUILDER_H
#define DATA_DATABASE_SQL_BUILDER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Builds sql strings to create, modify and delete records in the database
 *
 *  Note: These methods are not thread-safe. Locking is needed by caller.
 */

#include "data_diagram.h"
#include "data_error.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the sql-builder functions
 */
struct data_database_sql_builder_struct {
    utf8stringbuf_t temp_stringbuf;
    utf8stringbuf_t sql_stringbuf;
    char private_temp_buffer[8192];
    char private_sql_buffer[8192];
};

typedef struct data_database_sql_builder_struct data_database_sql_builder_t;

/*!
 *  \brief initializes the data_database_sql_builder_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_sql_builder_init ( data_database_sql_builder_t *this_ );

/*!
 *  \brief destroys the data_database_sql_builder_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_sql_builder_destroy ( data_database_sql_builder_t *this_ );

/*!
 *  \brief returns the sql statement that this object created
 *
 *  \param this_ pointer to own object attributes
 *  \return pointer to an utf8-encoded string
 */
static inline char* data_database_sql_builder_get_string_ptr ( data_database_sql_builder_t *this_ );

/*!
 *  \brief builds the sql command string to create a new diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram diagram to be created.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_create_diagram_command ( data_database_sql_builder_t *this_, const data_diagram_t *diagram );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_name new name of the diagram
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_update_diagram_name_cmd ( data_database_sql_builder_t *this_, int64_t diagram_id, const char *new_diagram_name );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_description new description of the diagram
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_update_diagram_description_cmd ( data_database_sql_builder_t *this_, int64_t diagram_id, const char *new_diagram_description );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_type new type of the diagram
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_update_diagram_type_cmd ( data_database_sql_builder_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type );

/*!
 *  \brief builds the sql command string to delete a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be deleted
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_delete_diagram_command ( data_database_sql_builder_t *this_, int64_t diagram_id );

/*!
 *  \brief builds the sql command string to create a new classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier classifier to be created.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_create_classifier_command ( data_database_sql_builder_t *this_, const data_classifier_t *classifier );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_stereotype new stereotype of the classifier
 */
data_error_t data_database_sql_builder_build_update_classifier_stereotype_cmd ( data_database_sql_builder_t *this_, int64_t classifier_id, const char *new_classifier_stereotype );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_name new name of the classifier
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_update_classifier_name_cmd ( data_database_sql_builder_t *this_, int64_t classifier_id, const char *new_classifier_name );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_description new description of the classifier
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_update_classifier_description_cmd ( data_database_sql_builder_t *this_, int64_t classifier_id, const char *new_classifier_description );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_main_type new description of the classifier
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_update_classifier_main_type_cmd ( data_database_sql_builder_t *this_, int64_t classifier_id, data_classifier_type_t new_classifier_main_type );

/*!
 *  \brief builds the sql command string to delete a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be deleted
 */
data_error_t data_database_sql_builder_build_delete_classifier_command ( data_database_sql_builder_t *this_, int64_t classifier_id );

/*!
 *  \brief builds the sql command string to create a new diagramelement record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement diagramelement to be created.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_create_diagramelement_command ( data_database_sql_builder_t *this_, const data_diagramelement_t *diagramelement );

/*!
 *  \brief builds the sql command string to delete a diagramelement record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement to be deleted.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_sql_builder_build_delete_diagramelement_command ( data_database_sql_builder_t *this_, int64_t diagramelement_id );

#include "storage/data_database_sql_builder.inl"

#endif  /* DATA_DATABASE_SQL_BUILDER_H */


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
