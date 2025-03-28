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
#include "entity/data_diagram.h"
#include "u8/u8_error.h"
#include "entity/data_classifier.h"
#include "set/data_visible_classifier.h"
#include "set/data_small_set.h"
#include "entity/data_feature.h"
#include "entity/data_relationship.h"
#include "set/data_search_result.h"
#include "set/data_search_result_list.h"
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
    sqlite3_stmt *statement_diagram_ids_by_textfragment;  /*!< retrieves rows containing matches of the fragment in a text field */
    sqlite3_stmt *statement_classifier_ids_by_textfragment;  /*!< retrieves rows containing matches of the fragment in a text field */
    sqlite3_stmt *statement_feature_ids_by_textfragment;  /*!< retrieves rows containing matches of the fragment in a text field */
    sqlite3_stmt *statement_relationship_ids_by_textfragment;  /*!< retrieves rows containing matches of the fragment in a text field */

    data_rules_t data_rules;  /*!< own instance of data rules */

    data_database_listener_t me_as_listener;  /*!< own instance of data_database_listener_t which wraps data_database_text_search_db_change_callback */
};

typedef struct data_database_text_search_struct data_database_text_search_t;

/*!
 *  \brief initializes the data_database_text_search_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this reader uses
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_text_search_init ( data_database_text_search_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_text_search_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_text_search_destroy ( data_database_text_search_t *this_ );

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
 *  \param textfragment text pattern for the objects which to search in the database, plain utf8 encoded
 *  \param max_out_results size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_results the object ids found in the database
 *  \param out_result_count number of objects stored in out_results
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
static inline u8_error_t data_database_text_search_get_objects_by_text_fragment ( data_database_text_search_t *this_,
                                                                                  const char *textfragment,
                                                                                  unsigned int max_out_results,
                                                                                  data_search_result_t (*out_results)[],
                                                                                  unsigned int* out_result_count
                                                                                );

/*!
 *  \brief reads a set of objects from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param textfragment text pattern for the objects which to search in the database, plain utf8 encoded
 *  \param io_results the list where to append the object ids found in the database
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_text_search_get_objects_by_textfragment ( data_database_text_search_t *this_,
                                                                   const char *textfragment,
                                                                   data_search_result_list_t *io_results
                                                                 );

/*!
 *  \brief reads a set of diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param stereo_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param descr_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param io_results the list where to append the object ids found in the database
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_text_search_private_get_diagrams_by_textfragment ( data_database_text_search_t *this_,
                                                                            const char *name_fragment,
                                                                            const char *stereo_fragment,
                                                                            const char *descr_fragment,
                                                                            data_search_result_list_t *io_results
                                                                          );

/*!
 *  \brief reads a set of classifiers from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param stereo_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param descr_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param io_results the list where to append the object ids found in the database
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_text_search_private_get_classifiers_by_textfragment ( data_database_text_search_t *this_,
                                                                               const char *name_fragment,
                                                                               const char *stereo_fragment,
                                                                               const char *descr_fragment,
                                                                               data_search_result_list_t *io_results
                                                                             );

/*!
 *  \brief reads a set of features from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param key_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param value_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param descr_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param io_results the list where to append the object ids found in the database
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_text_search_private_get_features_by_textfragment ( data_database_text_search_t *this_,
                                                                            const char *key_fragment,
                                                                            const char *value_fragment,
                                                                            const char *descr_fragment,
                                                                            data_search_result_list_t *io_results
                                                                          );

/*!
 *  \brief reads a set of relationships from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param stereo_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param descr_fragment text pattern for the objects which to search in the database, sql encoded for like statement
 *  \param io_results the list where to append the object ids found in the database
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_database_text_search_private_get_relationships_by_textfragment ( data_database_text_search_t *this_,
                                                                                 const char *name_fragment,
                                                                                 const char *stereo_fragment,
                                                                                 const char *descr_fragment,
                                                                                 data_search_result_list_t *io_results
                                                                               );

/*!
 *  \brief initializes the data_database_text_search_t struct and allows access to the database after the database is opened
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_text_search_private_open ( data_database_text_search_t *this_ );

/*!
 *  \brief closes the data_database_text_search_t struct (prohibits access to the database) before the database is closed
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_text_search_private_close ( data_database_text_search_t *this_ );

#if 0
/*!
 *  \brief binds two strings to a prepared statement (after reset).
 *
 *  The prepared statement shall have two variables of type string.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param text_1 first char sequence to bind to the prepared statement.
 *  \param text_2 second char sequence to bind to the prepared statement.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_text_search_private_bind_two_texts_to_statement ( data_database_text_search_t *this_,
                                                                                         sqlite3_stmt *statement_ptr,
                                                                                         const char *text_1,
                                                                                         const char *text_2
                                                                                       );
#endif

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
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 */
static inline u8_error_t data_database_text_search_private_bind_three_texts_to_statement ( data_database_text_search_t *this_,
                                                                                           sqlite3_stmt *statement_ptr,
                                                                                           const char *text_1,
                                                                                           const char *text_2,
                                                                                           const char *text_3
                                                                                         );

#include "storage/data_database_text_search.inl"

#endif  /* DATA_DATABASE_TEXT_SEARCH_H */


/*
Copyright 2020-2025 Andreas Warnke

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
