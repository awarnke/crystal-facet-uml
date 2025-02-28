/* File: data_database_classifier_reader.h; Copyright and License: see below */

#ifndef DATA_DATABASE_CLASSIFIER_READER_H
#define DATA_DATABASE_CLASSIFIER_READER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Searches and reads classifier-related records from the database
 */

#include "storage/data_database_listener.h"
#include "storage/data_database_listener_signal.h"
#include "storage/data_classifier_iterator.h"
#include "storage/data_feature_iterator.h"
#include "storage/data_relationship_iterator.h"
#include "storage/data_database.h"
#include "entity/data_diagram.h"
#include "u8/u8_error.h"
#include "entity/data_classifier.h"
#include "set/data_visible_classifier.h"
#include "set/data_small_set.h"
#include "entity/data_feature.h"
#include "entity/data_relationship.h"
#include "entity/data_row.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the classifier-related database-read functions
 */
struct data_database_classifier_reader_struct {
    data_database_t *database;  /*!< pointer to external database */

    sqlite3_stmt *statement_classifier_by_id;
    sqlite3_stmt *statement_classifier_by_name;
    sqlite3_stmt *statement_classifier_by_uuid;
    sqlite3_stmt *statement_classifiers_all;
    bool statement_classifiers_all_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
    sqlite3_stmt *statement_classifiers_all_hierarchical;
    bool statement_classifiers_all_hierarchical_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */

    sqlite3_stmt *statement_feature_by_id;
    sqlite3_stmt *statement_feature_by_uuid;
    sqlite3_stmt *statement_features_by_classifier_id;
    bool statement_features_by_classifier_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
    sqlite3_stmt *statement_features_by_diagram_id;
    bool statement_features_by_diagram_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */

    sqlite3_stmt *statement_relationship_by_id;
    sqlite3_stmt *statement_relationship_by_uuid;
    sqlite3_stmt *statement_relationships_by_classifier_id;
    bool statement_relationships_by_classifier_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
    sqlite3_stmt *statement_relationships_by_feature_id;
    bool statement_relationships_by_feature_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
    sqlite3_stmt *statement_relationships_by_diagram_id;
    bool statement_relationships_by_diagram_id_borrowed;  /*!< flag that indicates if the statement is borrowed by an iterator */
};

typedef struct data_database_classifier_reader_struct data_database_classifier_reader_t;

/*!
 *  \brief initializes the data_database_classifier_reader_t struct
 *
 *  This object shall be initialized only after opening the database.
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this reader uses
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_classifier_reader_init ( data_database_classifier_reader_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_classifier_reader_t struct
 *
 *  This object shall be destroyed before closing the database.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_classifier_reader_destroy ( data_database_classifier_reader_t *this_ );

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the classifier to be read from the database
 *  \param[out] out_classifier the classifier read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_classifier_by_id ( data_database_classifier_reader_t *this_,
                                                                  data_row_t id,
                                                                  data_classifier_t *out_classifier
                                                                );

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name the classifier to be read from the database
 *  \param[out] out_classifier the classifier read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if name does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_classifier_by_name ( data_database_classifier_reader_t *this_,
                                                                    const char *name,
                                                                    data_classifier_t *out_classifier
                                                                  );

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the classifier to be read from the database
 *  \param[out] out_classifier the classifier read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_classifier_by_uuid ( data_database_classifier_reader_t *this_,
                                                                    const char *uuid,
                                                                    data_classifier_t *out_classifier
                                                                  );

/*!
 *  \brief iterates over all classifiers from the database.
 *
 *  classifiers are sorted by number of containment-parents, ascending.
 *
 *  \param this_ pointer to own object attributes
 *  \param hierarchical true if the iterator shall start with classifiers without parent
 *  \param[in,out] io_classifier_iterator iterator over all classifiers. The caller is responsible
 *                                        for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_all_classifiers ( data_database_classifier_reader_t *this_,
                                                                 bool hierarchical,
                                                                 data_classifier_iterator_t *io_classifier_iterator
                                                               );

/* ================================ FEATURE ================================ */

/*!
 *  \brief reads a feature from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the feature to be read from the database
 *  \param[out] out_feature the feature read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_feature_by_id ( data_database_classifier_reader_t *this_,
                                                               data_row_t id,
                                                               data_feature_t *out_feature
                                                             );

/*!
 *  \brief reads a feature from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the feature to be read from the database
 *  \param[out] out_feature the feature read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_feature_by_uuid ( data_database_classifier_reader_t *this_,
                                                                 const char *uuid,
                                                                 data_feature_t *out_feature
                                                               );
/*!
 *  \brief reads all features of a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the parent classifier
 *  \param[in,out] io_feature_iterator iterator over features of selected classifier. The caller is responsible
 *                                     for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_features_by_classifier_id ( data_database_classifier_reader_t *this_,
                                                                           data_row_t classifier_id,
                                                                           data_feature_iterator_t *io_feature_iterator
                                                                         );

/*!
 *  \brief reads all features of a diagram from the database
 *
 *  If a classifier is contained multiple times in a diagram, the classifiers associated features are returned only once nonetheless (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param[in,out] io_feature_iterator iterator over features of selected classifier. The caller is responsible
 *                                     for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_features_by_diagram_id ( data_database_classifier_reader_t *this_,
                                                                        data_row_t diagram_id,
                                                                        data_feature_iterator_t *io_feature_iterator
                                                                      );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief reads a relationship from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the relationship to be read from the database
 *  \param[out] out_relationship the relationship read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_DB_STRUCTURE if id does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_relationship_by_id ( data_database_classifier_reader_t *this_,
                                                                    data_row_t id,
                                                                    data_relationship_t *out_relationship
                                                                  );

/*!
 *  \brief reads a relationship from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param uuid the relationship to be read from the database
 *  \param[out] out_relationship the relationship read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NOT_FOUND if uuid does not exist or U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_relationship_by_uuid ( data_database_classifier_reader_t *this_,
                                                                      const char *uuid,
                                                                      data_relationship_t *out_relationship
                                                                    );

/*!
 *  \brief reads all relationships of a classifier from the database
 *
 *  This includes relationships where the classifier is source-only, destination-only or both.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the source(from) or destination(to) classifier
 *  \param[in,out] io_relationship_iterator iterator over relationships of selected classifier. The caller is responsible
 *                                          for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_relationships_by_classifier_id ( data_database_classifier_reader_t *this_,
                                                                                data_row_t classifier_id,
                                                                                data_relationship_iterator_t *io_relationship_iterator
                                                                              );

/*!
 *  \brief reads all relationships of a feature from the database
 *
 *  This includes relationships where the feature is source-only, destination-only or both.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the source(from) or destination(to) feature; must not be DATA_ROW_VOID.
 *  \param[in,out] io_relationship_iterator iterator over relationships of selected feature. The caller is responsible
 *                                          for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_relationships_by_feature_id ( data_database_classifier_reader_t *this_,
                                                                             data_row_t feature_id,
                                                                             data_relationship_iterator_t *io_relationship_iterator
                                                                           );

/*!
 *  \brief reads all relationships of a diagram from the database
 *
 *  If classifiers are contained multiple times in a diagram, each relationship is returned only once nonetheless (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param[in,out] io_relationship_iterator iterator over relationships of selected diagram. The caller is responsible
 *                                          for initializing before and destroying this object afterwards.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_classifier_reader_get_relationships_by_diagram_id ( data_database_classifier_reader_t *this_,
                                                                             data_row_t diagram_id,
                                                                             data_relationship_iterator_t *io_relationship_iterator
                                                                           );

/* ================================ private ================================ */

/*!
 *  \brief initializes the data_database_classifier_reader_t struct and allows access to the database after the database is opened
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_classifier_reader_private_open ( data_database_classifier_reader_t *this_ );

/*!
 *  \brief closes the data_database_classifier_reader_t struct (prohibits access to the database) before the database is closed
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_classifier_reader_private_close ( data_database_classifier_reader_t *this_ );

/*!
 *  \brief binds a single integer to a prepared statement (after reset).
 *
 *  The prepared statement shall have only one variable of type integer.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param id integer to bind to the prepared statement. DATA_ROW_VOID does not work because VOID is mapped to NULL and cannot be selected by the = operator.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_classifier_reader_private_bind_id_to_statement ( data_database_classifier_reader_t *this_,
                                                                                        sqlite3_stmt *statement_ptr,
                                                                                        data_row_t id
                                                                                      );

/*!
 *  \brief binds two integers to a prepared statement (after reset).
 *
 *  The prepared statement shall have exactly two variables of type integer.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param id1 first integer to bind to the prepared statement. DATA_ROW_VOID does not work because VOID is mapped to NULL and cannot be selected by the = operator.
 *  \param id2 second integer to bind to the prepared statement. DATA_ROW_VOID does not work because VOID is mapped to NULL and cannot be selected by the = operator.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_classifier_reader_private_bind_two_ids_to_statement ( data_database_classifier_reader_t *this_,
                                                                                             sqlite3_stmt *statement_ptr,
                                                                                             data_row_t id1,
                                                                                             data_row_t id2
                                                                                           );

/*!
 *  \brief binds a single string to a prepared statement (after reset).
 *
 *  The prepared statement shall have only one variable of type string.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param text char sequence to bind to the prepared statement.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_classifier_reader_private_bind_text_to_statement ( data_database_classifier_reader_t *this_,
                                                                                          sqlite3_stmt *statement_ptr,
                                                                                          const char *text
                                                                                        );

#include "storage/data_database_classifier_reader.inl"

#endif  /* DATA_DATABASE_CLASSIFIER_READER_H */


/*
Copyright 2016-2025 Andreas Warnke

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
