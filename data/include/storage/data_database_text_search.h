/* File: data_database_text_search.h; Copyright and License: see below */

#ifndef DATA_DATABASE_TEXT_SEARCH_H
#define DATA_DATABASE_TEXT_SEARCH_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Free-text searches records from the database
 *
 *  Result data is specialized for use in search result list
 */

#include "storage/data_database_listener.h"
#include "storage/data_database_listener_signal.h"
#include "storage/data_database.h"
#include "data_diagram.h"
#include "data_error.h"
#include "data_classifier.h"
#include "set/data_visible_classifier.h"
#include "set/data_small_set.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "set/data_search_result.h"
#include "data_rules.h"
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_text_search_struct {
    data_database_t *database;  /*!< pointer to external database */

    bool is_open;  /*!< the prepared statements are only initialized if the database is open */
    sqlite3_stmt *private_prepared_query_diagram_ids_by_textfragment;  /*!< retrieves rows containing matches of the fragment in a text field */
    sqlite3_stmt *private_prepared_query_classifier_ids_by_textfragment;  /*!< retrieves rows containing matches of the fragment in a text field */
    sqlite3_stmt *private_prepared_query_feature_ids_by_textfragment;  /*!< retrieves rows containing matches of the fragment in a text field */
    sqlite3_stmt *private_prepared_query_relationship_ids_by_textfragment;  /*!< retrieves rows containing matches of the fragment in a text field */

    data_rules_t data_rules;  /*!< own instance of data rules */

    data_database_listener_t me_as_listener;  /*!< own instance of data_database_listener_t which wraps data_database_text_search_db_change_callback */
};

typedef struct data_database_text_search_struct data_database_text_search_t;

/*!
 *  \brief initializes the data_database_text_search_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this reader uses
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_text_search_init ( data_database_text_search_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_text_search_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_text_search_destroy ( data_database_text_search_t *this_ );

/*!
 *  \brief prepares a database change and re-initializes afterwards
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_id state of database change
 */
void data_database_text_search_db_change_callback ( data_database_text_search_t *this_, data_database_listener_signal_t signal_id );

/*!
 *  \brief checks if the database reader is open
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the database reader is open
 */
static inline bool data_database_text_search_is_open( data_database_text_search_t *this_ );

/*!
 *  \brief reads a set of objects from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param textfragment text pattern for the objects which to search in the database
 *  \param apply_filter_rules true if search results not matching to the rules set shall be filtered
 *  \param max_out_results size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_results the object ids found in the database
 *  \param out_result_count number of objects stored in out_results
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_text_search_get_objects_by_textfragment ( data_database_text_search_t *this_,
                                                                     const char *textfragment,
                                                                     bool apply_filter_rules,
                                                                     unsigned int max_out_results,
                                                                     data_search_result_t *out_results,
                                                                     unsigned int* out_result_count
                                                                   );

/*!
 *  \brief reads a set of diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param textfragment text pattern for the objects which to search in the database
 *  \param apply_filter_rules true if search results not matching to the rules set shall be filtered
 *  \param max_out_results size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_results the object ids found in the database
 *  \param out_result_count number of objects stored in out_results
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_text_search_private_get_diagrams_by_textfragment ( data_database_text_search_t *this_,
                                                                              const char *textfragment,
                                                                              bool apply_filter_rules,
                                                                              unsigned int max_out_results,
                                                                              data_search_result_t *out_results,
                                                                              unsigned int* out_result_count
                                                                            );

/*!
 *  \brief reads a set of classifiers from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param textfragment text pattern for the objects which to search in the database
 *  \param apply_filter_rules true if search results not matching to the rules set shall be filtered
 *  \param max_out_results size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_results the object ids found in the database
 *  \param out_result_count number of objects stored in out_results
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_text_search_private_get_classifiers_by_textfragment ( data_database_text_search_t *this_,
                                                                                 const char *textfragment,
                                                                                 bool apply_filter_rules,
                                                                                 unsigned int max_out_results,
                                                                                 data_search_result_t *out_results,
                                                                                 unsigned int* out_result_count
                                                                               );

/*!
 *  \brief reads a set of features from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param textfragment text pattern for the objects which to search in the database
 *  \param apply_filter_rules true if search results not matching to the rules set shall be filtered
 *  \param max_out_results size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_results the object ids found in the database
 *  \param out_result_count number of objects stored in out_results
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_text_search_private_get_features_by_textfragment ( data_database_text_search_t *this_,
                                                                              const char *textfragment,
                                                                              bool apply_filter_rules,
                                                                              unsigned int max_out_results,
                                                                              data_search_result_t *out_results,
                                                                              unsigned int* out_result_count
                                                                            );

/*!
 *  \brief reads a set of relationships from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param textfragment text pattern for the objects which to search in the database
 *  \param apply_filter_rules true if search results not matching to the rules set shall be filtered
 *  \param max_out_results size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_results the object ids found in the database
 *  \param out_result_count number of objects stored in out_results
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_text_search_private_get_relationships_by_textfragment ( data_database_text_search_t *this_,
                                                                                   const char *textfragment,
                                                                                   bool apply_filter_rules,
                                                                                   unsigned int max_out_results,
                                                                                   data_search_result_t *out_results,
                                                                                   unsigned int* out_result_count
                                                                                 );

/*!
 *  \brief initializes the data_database_text_search_t struct and allows access to the database after the database is opened
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_text_search_private_open ( data_database_text_search_t *this_ );

/*!
 *  \brief closes the data_database_text_search_t struct (prohibits access to the database) before the database is closed
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_text_search_private_close ( data_database_text_search_t *this_ );

/*!
 *  \brief creates a prepared statement.
 *
 *  \param this_ pointer to own object attributes
 *  \param string_statement statement as string to be prepared
 *  \param string_size size of string_statement in bytes, including the terminating zero
 *  \param out_statement_ptr address of a pointer. The pointer is modifies as to point to a statement object.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_text_search_private_prepare_statement ( data_database_text_search_t *this_,
                                                                                 const char *string_statement,
                                                                                 unsigned int string_size,
                                                                                 sqlite3_stmt **out_statement_ptr
                                                                               );

/*!
 *  \brief finalizes a prepared statement.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_text_search_private_finalize_statement ( data_database_text_search_t *this_, sqlite3_stmt *statement_ptr );

/*!
 *  \brief binds two strings to a prepared statement (after reset).
 *
 *  The prepared statement shall have two variables of type string.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param text_1 first char sequence to bind to the prepared statement.
 *  \param text_2 second char sequence to bind to the prepared statement.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_text_search_private_bind_two_texts_to_statement ( data_database_text_search_t *this_,
                                                                                           sqlite3_stmt *statement_ptr,
                                                                                           const char *text_1,
                                                                                           const char *text_2
                                                                                         );

/*!
 *  \brief binds three strings to a prepared statement (after reset).
 *
 *  The prepared statement shall have three variables of type string.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param text_1 first char sequence to bind to the prepared statement.
 *  \param text_2 second char sequence to bind to the prepared statement.
 *  \param text_3 third char sequence to bind to the prepared statement.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_text_search_private_bind_three_texts_to_statement ( data_database_text_search_t *this_,
                                                                                             sqlite3_stmt *statement_ptr,
                                                                                             const char *text_1,
                                                                                             const char *text_2,
                                                                                             const char *text_3
                                                                                           );

#include "storage/data_database_text_search.inl"

#endif  /* DATA_DATABASE_TEXT_SEARCH_H */


/*
Copyright 2020-2020 Andreas Warnke

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
