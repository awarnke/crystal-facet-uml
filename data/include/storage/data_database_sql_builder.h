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
#include "u8/u8_error.h"
#include "data_classifier.h"
#include "data_diagramelement.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_row_id.h"
#include "utf8stringbuf/utf8stringbuf.h"
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

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief builds the sql command string to create a new diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram diagram to be created. The id should be DATA_ROW_ID_VOID unless a diagram with known, unique id shall be created.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_create_diagram_command ( data_database_sql_builder_t *this_, const data_diagram_t *diagram );

/*!
 *  \brief builds the sql command string to delete a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be deleted
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_delete_diagram_command ( data_database_sql_builder_t *this_, data_row_id_t diagram_id );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_name new name of the diagram
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_diagram_name_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, const char *new_diagram_name );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_description new description of the diagram
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_diagram_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, const char *new_diagram_description );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_type new type of the diagram
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_diagram_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, data_diagram_type_t new_diagram_type );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_list_order new list_order of the diagram
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_diagram_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, int32_t new_diagram_list_order );

/*!
 *  \brief builds the sql command string to update a diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram to be updated
 *  \param new_diagram_parent_id new parent_id of the diagram
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_diagram_parent_id_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, data_row_id_t new_diagram_parent_id );

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief builds the sql command string to create a new classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier classifier to be created. The id should be DATA_ROW_ID_VOID unless a classifier with known, unique id shall be created.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_create_classifier_command ( data_database_sql_builder_t *this_, const data_classifier_t *classifier );

/*!
 *  \brief builds the sql command string to delete a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be deleted
 */
u8_error_t data_database_sql_builder_build_delete_classifier_command ( data_database_sql_builder_t *this_, data_row_id_t classifier_id );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_stereotype new stereotype of the classifier
 */
u8_error_t data_database_sql_builder_build_update_classifier_stereotype_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_stereotype );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_name new name of the classifier
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_classifier_name_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_name );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_description new description of the classifier
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_classifier_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_description );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_main_type new description of the classifier
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_classifier_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, data_classifier_type_t new_classifier_main_type );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_x_order new x_order of the classifier
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_classifier_x_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_x_order );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_y_order new y_order of the classifier
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_classifier_y_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_y_order );

/*!
 *  \brief builds the sql command string to update a classifier record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier to be updated
 *  \param new_classifier_list_order new list_order of the classifier
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_classifier_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_list_order );

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief builds the sql command string to create a new diagramelement record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement diagramelement to be created. The id should be DATA_ROW_ID_VOID unless a diagramelement with known, unique id shall be created.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_create_diagramelement_command ( data_database_sql_builder_t *this_, const data_diagramelement_t *diagramelement );

/*!
 *  \brief builds the sql command string to delete a diagramelement record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement to be deleted.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_delete_diagramelement_command ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id );

/*!
 *  \brief builds the sql command string to update a diagramelement record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement to be updated
 *  \param new_display_flags new display_flags of the diagramelement
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_diagramelement_display_flags_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id, data_diagramelement_flag_t new_display_flags );

/*!
 *  \brief builds the sql command string to update a diagramelement record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagramelement_id id of the diagramelement to be updated
 *  \param new_focused_feature_id new focused_feature_id of the diagramelement
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_diagramelement_focused_feature_id_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id, data_row_id_t new_focused_feature_id );

/* ================================ FEATURE ================================ */

/*!
 *  \brief builds the sql command string to create a new feature record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature feature to be created. The id should be DATA_ROW_ID_VOID unless a feature with known, unique id shall be created.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_create_feature_command ( data_database_sql_builder_t *this_, const data_feature_t *feature );

/*!
 *  \brief builds the sql command string to delete a feature record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be deleted
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_delete_feature_command ( data_database_sql_builder_t *this_, data_row_id_t feature_id );

/*!
 *  \brief builds the sql command string to update a feature record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_type new type of the feature
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_feature_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, data_feature_type_t new_feature_type );

/*!
 *  \brief builds the sql command string to update a feature record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_key new key of the feature
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_feature_key_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_key );

/*!
 *  \brief builds the sql command string to update a feature record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_value new value of the feature
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_feature_value_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_value );

/*!
 *  \brief builds the sql command string to update a feature record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_description new description of the feature
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_feature_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_description );

/*!
 *  \brief builds the sql command string to update a feature record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the feature to be updated
 *  \param new_feature_list_order new list_order of the feature
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_feature_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, int32_t new_feature_list_order );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief builds the sql command string to create a new relationship record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship relationship to be created. The id should be DATA_ROW_ID_VOID unless a relationship with known, unique id shall be created.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_create_relationship_command ( data_database_sql_builder_t *this_, const data_relationship_t *relationship );

/*!
 *  \brief builds the sql command string to delete a relationship record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship to be deleted
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_delete_relationship_command ( data_database_sql_builder_t *this_, data_row_id_t relationship_id );

/*!
 *  \brief builds the sql command string to update a relationship record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship to be updated
 *  \param new_relationship_type new type of the relationship
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_relationship_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, data_relationship_type_t new_relationship_type );

/*!
 *  \brief builds the sql command string to update a relationship record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship to be updated
 *  \param new_relationship_name new name of the relationship
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_relationship_name_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, const char *new_relationship_name );

/*!
 *  \brief builds the sql command string to update a relationship record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship to be updated
 *  \param new_relationship_description new description of the relationship
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_relationship_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, const char *new_relationship_description );

/*!
 *  \brief builds the sql command string to update a relationship record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param relationship_id id of the relationship to be updated
 *  \param new_relationship_list_order new list_order of the relationship
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
u8_error_t data_database_sql_builder_build_update_relationship_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, int32_t new_relationship_list_order );

#include "storage/data_database_sql_builder.inl"

#endif  /* DATA_DATABASE_SQL_BUILDER_H */


/*
Copyright 2016-2022 Andreas Warnke

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
