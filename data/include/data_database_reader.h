/* File: data_database_reader.h; Copyright and License: see below */

#ifndef DATA_DATABASE_READER_H
#define DATA_DATABASE_READER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_database.h"
#include "data_diagram.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_reader_struct {
    data_database_t *database;
};

typedef struct data_database_reader_struct data_database_reader_t;

/*!
 *  \brief initializes the data_database_reader_t struct
 */
void data_database_reader_init ( data_database_reader_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_reader_t struct
 */
void data_database_reader_destroy ( data_database_reader_t *this_ );

/*!
 *  \brief reads a diagram from the database
 *
 *  \param out_diagram the diagram read from the database (in case of success)
 *  \return 0 in case of success, a negative value in case of error.
 */
int32_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_, int32_t id, data_diagram_t *out_diagram );

/*!
 *  \brief reads all child-diagrams from the database
 *
 *  \param parent_id id of the parent diagram
 *  \param out_diagram array of diagrams read from the database (in case of success)
 *  \return 0 or a positive number in case of success, a negative value in case of error. in case of success, this is the number of child-diagrams.
 */
int32_t data_database_reader_get_diagrams_of_parent_id ( data_database_reader_t *this_, int32_t parent_id, int32_t max_out_array_size, data_diagram_t (*out_diagram)[] );

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
