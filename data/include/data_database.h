/* File: data_database.h; Copyright and License: see below */

#ifndef DATA_DATABASE_H
#define DATA_DATABASE_H

/* public file for the doxygen documentation: */
/*! \file */

#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_struct {
    sqlite3 *db;
    const char* db_file_name; /* non-const pointer to const string */
    bool is_open;
};

typedef struct data_database_struct data_database_t;

/*!
 *  \brief initializes the data_database_t struct
 */
void data_database_init ( data_database_t *this_ );

/*!
 *  \brief opens a database file
 */
void data_database_open ( data_database_t *this_, const char* db_file_path );

/*!
 *  \brief closes the current database file
 */
void data_database_close ( data_database_t *this_ );

/*!
 *  \brief destroys the data_database_t struct
 */
void data_database_destroy ( data_database_t *this_ );

/*!
 *  \brief returns a pointer to the sqlite database
 */
static inline sqlite3 *data_database_get_database ( data_database_t *this_ );

#include "data_database.inl"

#endif  /* DATA_DATABASE_H */


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
