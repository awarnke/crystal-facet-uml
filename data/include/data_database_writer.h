/* File: data_database_writer.h; Copyright and License: see below */

#ifndef DATA_DATABASE_WRITER_H
#define DATA_DATABASE_WRITER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_database.h"
#include "data_diagram.h"
#include "data_error.h"
#include "util/string/utf8stringbuf.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_writer_struct {
    data_database_t *database;  /*!< pointer to external database */

    pthread_mutex_t private_lock; /*!< lock to ensure that all private attributes are used by only one thread */
    utf8stringbuf_t private_temp_stringbuf;
    utf8stringbuf_t private_sql_stringbuf;
    char private_temp_buffer[8192];
    char private_sql_buffer[8192];
};

typedef struct data_database_writer_struct data_database_writer_t;

/*!
 *  \brief initializes the data_database_writer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_writer_init ( data_database_writer_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_writer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_writer_destroy ( data_database_writer_t *this_ );

/*!
 *  \brief creates a new view and returns its id
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram data of the new diagram record to be created.
 *  \param out_new_id storage, where the id of the newly created record is stored.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_writer_create_diagram ( data_database_writer_t *this_, const data_diagram_t *diagram, int64_t* out_new_id );

/*!
 *  \brief builds the sql command string to create a new diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram diagram to be created.
 */
static data_error_t data_database_writer_private_build_create_diagram_command ( data_database_writer_t *this_, const data_diagram_t *diagram );

#endif  /* DATA_DATABASE_WRITER_H */


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
