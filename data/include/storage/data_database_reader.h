/* File: data_database_reader.h; Copyright and License: see below */

#ifndef DATA_DATABASE_READER_H
#define DATA_DATABASE_READER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Searches and reads records from the database
 */

#include "storage/data_database_listener_signal.h"
#include "storage/data_database.h"
#include "data_diagram.h"
#include "data_error.h"
#include "data_classifier.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_reader_struct {
    data_database_t *database;  /*!< pointer to external database */

    pthread_mutex_t private_lock;  /*!< lock to ensure that all private attributes are used by only one thread */
    bool is_open;  /*!< the prepared statements are only initialized if the database is open */
    sqlite3_stmt *private_prepared_query_diagram_by_id;
    sqlite3_stmt *private_prepared_query_diagrams_by_parent_id;
    sqlite3_stmt *private_prepared_query_classifier_by_id;
    sqlite3_stmt *private_prepared_query_classifiers_by_diagram_id;

    data_database_listener_t me_as_listener;  /*!< own instance of data_database_listener_t which wraps data_database_reader_db_change_callback */
};

typedef struct data_database_reader_struct data_database_reader_t;

/*!
 *  \brief initializes the data_database_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this reader uses
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_reader_init ( data_database_reader_t *this_, data_database_t *database );

/*!
 *  \brief initializes the data_database_reader_t struct and allows access to the database after the database is opened
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_reader_private_open ( data_database_reader_t *this_ );

/*!
 *  \brief closes the data_database_reader_t struct (prohibits access to the database) before the database is closed
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_reader_private_close ( data_database_reader_t *this_ );

/*!
 *  \brief destroys the data_database_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_reader_destroy ( data_database_reader_t *this_ );

/*!
 *  \brief reads a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the diagram to be read from the database
 *  \param out_diagram the diagram read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_, int64_t id, data_diagram_t *out_diagram );

/*!
 *  \brief reads all child-diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_diagram array of diagrams read from the database (in case of success)
 *  \param out_diagram_count number of diagram records stored in out_diagram
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_,
                                                              int64_t parent_id,
                                                              uint32_t max_out_array_size,
                                                              data_diagram_t (*out_diagram)[],
                                                              uint32_t *out_diagram_count
                                                            );

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the classifier to be read from the database
 *  \param out_classifier the classifier read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_reader_get_classifier_by_id ( data_database_reader_t *this_, int64_t id, data_classifier_t *out_classifier );

/*!
 *  \brief reads all classifier of a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_classifier array of classifiers read from the database (in case of success)
 *  \param out_classifier_count number of classifier records stored in out_diagram
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_reader_get_classifiers_by_diagram_id ( data_database_reader_t *this_,
                                                                  int64_t diagram_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_classifier_t (*out_classifier)[],
                                                                  uint32_t *out_classifier_count
                                                                );

/*!
 *  \brief gets a lock to protect data in data_database_reader_t from concurrent access.
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
static inline data_error_t data_database_reader_private_lock ( data_database_reader_t *this_ );

/*!
 *  \brief releases the lock.
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
static inline data_error_t data_database_reader_private_unlock ( data_database_reader_t *this_ );

/*!
 *  \brief creates a prepared statement.
 *
 *  \param this_ pointer to own object attributes
 *  \param string_statement statement as string to be prepared
 *  \param string_size size of string_statement in bytes, including the terminating zero
 *  \param out_statement_ptr address of a pointer. The pointer is modifies as to point to a statement object.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
static inline data_error_t data_database_reader_private_prepare_statement ( data_database_reader_t *this_,
                                                                            const char *string_statement,
                                                                            int string_size, s
                                                                            qlite3_stmt **out_statement_ptr
                                                                          );

/*!
 *  \brief finalizes a prepared statement.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
static inline data_error_t data_database_reader_private_finalize_statement ( data_database_reader_t *this_, sqlite3_stmt *statement_ptr );

/*!
 *  \brief binds a single integer to a prepared statement.
 *
 *  The prepared statement shall have only one variable of type integer.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param id integer to bind to the prepared statement.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
static inline data_error_t data_database_reader_private_bind_id_to_statement ( data_database_reader_t *this_, sqlite3_stmt *statement_ptr, int64_t id );

/*!
 *  \brief prepares a database change and re-initializes afterwards
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_id state of database change
 */
void data_database_reader_db_change_callback ( data_database_reader_t *this_, data_database_listener_signal_t signal_id );

#include "storage/data_database_reader.inl"

#endif  /* DATA_DATABASE_READER_H */


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
