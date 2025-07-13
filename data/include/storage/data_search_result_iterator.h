/* File: data_search_result_iterator.h; Copyright and License: see below */

#ifndef DATA_SEARCH_RESULT_ITERATOR_H
#define DATA_SEARCH_RESULT_ITERATOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a search_result set which is a statement result from the database
 */

#include "storage/data_database.h"
#include "storage/data_database_borrowed_stmt.h"
#include "u8/u8_error.h"
#include "set/data_search_result.h"
#include "data_rules.h"
#include <sqlite3.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the search_result iterator functions
 *
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 *
 *  The user of the iterator owns the object, which means that
 *  the function that performs the iteration also provides the memory for the iterator,
 *  initializes the struct before usage and deytroys it afterwards.
 *
 *  Note that the SQL statement lives much longer and is just borrowed from
 *  data_database_text_search.
 */
struct data_search_result_iterator_struct {
    data_database_borrowed_stmt_t diagram_statement;  /*!< borrowed statement to access the sql query results on diagrams */
    data_database_borrowed_stmt_t classifier_statement;  /*!< borrowed statement to access the sql query results on classifiers */
    data_database_borrowed_stmt_t feature_statement;  /*!< borrowed statement to access the sql query results on features */
    data_database_borrowed_stmt_t relationship_statement;  /*!< borrowed statement to access the sql query results on relationships */
    bool is_at_diagram_end;  /*!< true if is_invalid or if at the end of the sql query result */
    bool is_at_classifier_end;  /*!< true if is_invalid or if at the end of the sql query result */
    bool is_at_feature_end;  /*!< true if is_invalid or if at the end of the sql query result */
    bool is_at_relationship_end;  /*!< true if is_invalid or if at the end of the sql query result */

    data_search_result_t *next_search_result_buf;  /*!< a buffer that was passed from data_database_text_search to store the next data_search_result_t */

    data_rules_t data_rules;  /*!< own instance of data rules */
};

typedef struct data_search_result_iterator_struct data_search_result_iterator_t;

/*!
 *  \brief predefined search statement to find search_results by diagram-id
 */
extern const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_DIAGRAM_BY_TEXTFRAGMENT;

/*!
 *  \brief predefined search statement to find search_results by diagram-id
 */
extern const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_CLASSIFIER_BY_TEXTFRAGMENT;

/*!
 *  \brief predefined search statement to find search_results by diagram-id
 */
extern const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_FEATURE_BY_TEXTFRAGMENT;

/*!
 *  \brief predefined search statement to find search_results by diagram-id
 */
extern const char *const DATA_SEARCH_RESULT_ITERATOR_SELECT_RELATIONSHIP_BY_TEXTFRAGMENT;

/*!
 *  \brief initializes the data_search_result_iterator_t struct to an empty set
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_search_result_iterator_init_empty ( data_search_result_iterator_t *this_ );

/*!
 *  \brief re-initializes the data_search_result_iterator_t struct to iterate over a db-result-set of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_statement borrowed statement to access the sql query results on diagrams
 *  \param classifier_statement borrowed statement to access the sql query results on classifiers
 *  \param feature_statement borrowed statement to access the sql query results on features
 *  \param relationship_statement borrowed statement to access the sql query results on relationships
 *  \param next_search_result_buf a buffer that was passed from data_database_text_search to store the next data_search_result_t
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_search_result_iterator_reinit ( data_search_result_iterator_t *this_,
                                                data_database_borrowed_stmt_t diagram_statement,
                                                data_database_borrowed_stmt_t classifier_statement,
                                                data_database_borrowed_stmt_t feature_statement,
                                                data_database_borrowed_stmt_t relationship_statement,
                                                data_search_result_t *next_search_result_buf
                                              );

/*!
 *  \brief destroys the data_search_result_iterator_t struct
 *
 *  This destructor frees the resources occupied by the database.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_search_result_iterator_destroy ( data_search_result_iterator_t *this_ );

/*!
 *  \brief checks if a next element exists in the iterator - does not modify the iterator state
 *
 *  \param this_ pointer to own object attributes
 *  \return true if another search_result exists, false if not or in case of an error
 */
bool data_search_result_iterator_has_next ( const data_search_result_iterator_t *this_ );

/*!
 *  \brief reads the next search_result from the database (and internally advances to the search_result after).
 *
 *  \param this_ pointer to own object attributes
 *  \param out_search_result the next search_result read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_INVALID_REQUEST if there is no next element in the iterator.
 */
u8_error_t data_search_result_iterator_next ( data_search_result_iterator_t *this_, data_search_result_t *out_search_result );

/*!
 *  \brief moves the iterator to the next element, updates (*this_).is_at_end
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_search_result_iterator_private_step_to_next ( data_search_result_iterator_t *this_ );

/*!
 *  \brief reads a set of diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param out_search_result the next search_result read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_search_result_iterator_private_get_diagram ( data_search_result_iterator_t *this_, data_search_result_t *out_search_result );

/*!
 *  \brief reads a set of classifiers from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param out_search_result the next search_result read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_search_result_iterator_private_get_classifier ( data_search_result_iterator_t *this_, data_search_result_t *out_search_result );

/*!
 *  \brief reads a set of features from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param out_search_result the next search_result read from the database (in case of success).
 *                           If the feature is filtered from the diagram by rules, out_search_result is set to void.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_search_result_iterator_private_get_feature ( data_search_result_iterator_t *this_, data_search_result_t *out_search_result );

/*!
 *  \brief reads a set of relationships from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param out_search_result the next search_result read from the database (in case of success).
 *                           If the feature is filtered from the diagram by rules, out_search_result is set to void.
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_NO_DB if the database is not open.
 */
u8_error_t data_search_result_iterator_private_get_relationship ( data_search_result_iterator_t *this_, data_search_result_t *out_search_result );

#endif  /* DATA_SEARCH_RESULT_ITERATOR_H */


/*
Copyright 2025-2025 Andreas Warnke

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
