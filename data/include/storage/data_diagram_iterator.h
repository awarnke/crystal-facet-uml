/* File: data_diagram_iterator.h; Copyright and License: see below */

#ifndef DATA_DIAGRAM_ITERATOR_H
#define DATA_DIAGRAM_ITERATOR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Iterates over a diagram set which is a statement result from the database
 */

#include "storage/data_database.h"
#include "storage/data_database_borrowed_stmt.h"
#include "u8/u8_error.h"
#include "entity/data_diagram.h"
#include <sqlite3.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the diagram iterator functions
 *
 *  The iterator works similar to the J2SE-ListIterator, hibernate-query-Iterator and QT-QListIterator:
 *  while ( hasNext() ) { element = next() };
 *
 *  The user of the iterator owns the object, which means that
 *  the function that performs the iteration also provides the memory for the iterator,
 *  initializes the struct before usage and deytroys it afterwards.
 *
 *  Note that the SQL statement lives much longer and is just borrowed from
 *  data_database_classifier_reader.
 */
struct data_diagram_iterator_struct {
    data_database_borrowed_stmt_t statement;  /*!< borrowed statement to access the sql query results */
    bool is_at_end;  /*!< true if is_invalid or if at the end of the sql query result */
};

typedef struct data_diagram_iterator_struct data_diagram_iterator_t;

/*!
 *  \brief predefined search statement to find diagrams by parent-id
 */
extern const char DATA_DIAGRAM_ITERATOR_SELECT_DIAGRAMS_BY_PARENT_ID[];

/*!
 *  \brief predefined search statement to find diagrams by NULL parent-id
 */
extern const char DATA_DIAGRAM_ITERATOR_SELECT_DIAGRAMS_BY_PARENT_ID_NULL[];

/*!
 *  \brief predefined search statement to find diagrams by classifier-id
 */
extern const char DATA_DIAGRAM_ITERATOR_SELECT_DIAGRAMS_BY_CLASSIFIER_ID[];

/*!
 *  \brief initializes the data_diagram_iterator_t struct to an empty set
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_diagram_iterator_init_empty ( data_diagram_iterator_t *this_ );

/*!
 *  \brief re-initializes the data_diagram_iterator_t struct to iterate over a db-result-set of classifiers
 *
 *  \param this_ pointer to own object attributes
 *  \param statement borrowed statement to access the sql query results
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_diagram_iterator_reinit ( data_diagram_iterator_t *this_,
                                             data_database_borrowed_stmt_t statement
                                           );

/*!
 *  \brief destroys the data_diagram_iterator_t struct
 *
 *  This destructor frees the resources occupied by the database.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_diagram_iterator_destroy ( data_diagram_iterator_t *this_ );

/*!
 *  \brief checks if a next element exists in the iterator - does not modify the iterator state
 *
 *  \param this_ pointer to own object attributes
 *  \return true if another diagram exists, false if not or in case of an error
 */
bool data_diagram_iterator_has_next ( const data_diagram_iterator_t *this_ );

/*!
 *  \brief reads the next diagram from the database (and internally advances to the diagram after).
 *
 *  \param this_ pointer to own object attributes
 *  \param out_diagram the next diagram read from the database (in case of success)
 *  \return U8_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. U8_ERROR_INVALID_REQUEST if there is no next element in the iterator.
 */
u8_error_t data_diagram_iterator_next ( data_diagram_iterator_t *this_, data_diagram_t *out_diagram );

/*!
 *  \brief moves the iterator to the next element, updates (*this_).is_at_end
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success
 */
u8_error_t data_diagram_iterator_private_step_to_next ( data_diagram_iterator_t *this_ );

#endif  /* DATA_DIAGRAM_ITERATOR_H */


/*
Copyright 2024-2024 Andreas Warnke

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