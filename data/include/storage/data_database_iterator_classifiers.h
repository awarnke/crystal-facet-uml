/* File: data_database_iterator_classifiers.h; Copyright and License: see below */

#ifndef DATA_DATABASE_ITERATOR_CLASSIFIERS_H
#define DATA_DATABASE_ITERATOR_CLASSIFIERS_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a classifier set which is a statement result from the database
 */

#include "storage/data_database.h"
#include "u8/u8_error.h"
#include "data_classifier.h"
#include <sqlite3.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the classifier iterator functions
 * 
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 */
struct data_database_iterator_classifiers_struct {
    bool is_valid;  /*!< database and/or statement_all_classifiers are invalid (NULL) */
    data_database_t *database;  /*!< pointer to external database. Currently unused, but in future, e.g. is_open could be queried */
    sqlite3_stmt *statement_all_classifiers;  /*!< own instance of a pointer to the sql query object */
    bool is_at_end;  /*!< true if is_invalid or if at the end of the sql query result */
};

typedef struct data_database_iterator_classifiers_struct data_database_iterator_classifiers_t;

/*!
 *  \brief initializes the data_database_iterator_classifiers_t struct to an empty set
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_iterator_classifiers_init_empty ( data_database_iterator_classifiers_t *this_ );

/*!
 *  \brief re-initializes the data_database_iterator_classifiers_t struct to iterate over a db-result-set of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this iterator uses
 *  \param statement_all_classifiers sql statement to access the result set
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_iterator_classifiers_reinit ( data_database_iterator_classifiers_t *this_, data_database_t *database, sqlite3_stmt* statement_all_classifiers );

/*!
 *  \brief destroys the data_database_iterator_classifiers_t struct
 *
 *  This destructor frees the resources occupied by the database.
 * 
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_iterator_classifiers_destroy ( data_database_iterator_classifiers_t *this_ );

/*!
 *  \brief checks if a next element exists in the iterator - does not modify the iterator state
 *
 *  \param this_ pointer to own object attributes
 *  \return true if another classifier exists, false if not or in case of an error
 */
bool data_database_iterator_classifiers_has_next ( const data_database_iterator_classifiers_t *this_ );

/*!
 *  \brief reads the next classifier from the database (and internally advances to the classifier after).
 *
 *  \param this_ pointer to own object attributes
 *  \param out_classifier the next classifier read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_INVALID_REQUEST if there is no next element in the iterator.
 */
u8_error_t data_database_iterator_classifiers_next ( data_database_iterator_classifiers_t *this_, data_classifier_t *out_classifier );

/*!
 *  \brief moves the iterator to the next element, updates (*this_).is_at_end
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_database_iterator_private_step_to_next ( data_database_iterator_classifiers_t *this_ );

#endif  /* DATA_DATABASE_ITERATOR_CLASSIFIERS_H */


/*
Copyright 2020-2022 Andreas Warnke

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
