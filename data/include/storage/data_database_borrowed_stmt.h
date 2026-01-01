/* File: data_database_borrowed_stmt.h; Copyright and License: see below */

#ifndef DATA_DATABASE_BORROWED_STMT_H
#define DATA_DATABASE_BORROWED_STMT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Allows an iterator to borrow a statement from a database access class.
 *
 *  It ensures that only one iterator borrows the statement and that this statement is retorned when done.
 */

#include "u8/u8_error.h"
#include <sqlite3.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_borrowed_stmt_struct {
    data_database_t *database;  /*!< pointer to database, allows to check if it is still open */
    sqlite3_stmt *db_statement;  /*!< a pointer to the sql statement object */
    bool *borrow_flag;  /*!< a pointer to the borrow_flag of the sql statement; not thread-safe */
};

typedef struct data_database_borrowed_stmt_struct data_database_borrowed_stmt_t;

/*!
 *  \brief initializes the data_database_borrowed_stmt_t struct to void
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_database_borrowed_stmt_init_void ( data_database_borrowed_stmt_t *this_ );

/*!
 *  \brief initializes the data_database_borrowed_stmt_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database on which the statement operates
 *  \param db_statement sql statement that can be used
 *  \param borrow_flag  borrow_flag of the sql statement; not thread-safe
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_WRONG_STATE if db_statement is already borrowed
 */
static inline u8_error_t data_database_borrowed_stmt_init ( data_database_borrowed_stmt_t *this_,
                                                            data_database_t *database,
                                                            sqlite3_stmt *db_statement,
                                                            bool *borrow_flag
                                                          );

/*!
 *  \brief destroys the data_database_borrowed_stmt_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, U8_ERROR_AT_DB in case of an error - also in case of a previous error
 */
static inline u8_error_t data_database_borrowed_stmt_destroy ( data_database_borrowed_stmt_t *this_ );

/*!
 *  \brief returns if this object was initialized with valid data
 *
 *  \param this_ pointer to own object attributes
 *  \return true if valid
 */
static inline bool data_database_borrowed_stmt_is_valid ( const data_database_borrowed_stmt_t *this_ );

/*!
 *  \brief gets the sql statement
 *
 *  \param this_ pointer to own object attributes
 *  \return sql statement
 */
static inline sqlite3_stmt * data_database_borrowed_stmt_get_statement ( const data_database_borrowed_stmt_t *this_ );

/*!
 *  \brief gets a database object
 *
 *  \param this_ pointer to own object attributes
 *  \return database on which the statement operates
 */
static inline data_database_t * data_database_borrowed_stmt_get_database ( data_database_borrowed_stmt_t *this_ );

#include "storage/data_database_borrowed_stmt.inl"

#endif  /* DATA_DATABASE_BORROWED_STMT_H */


/*
Copyright 2024-2026 Andreas Warnke

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
