/* File: ctrl_undo_redo_list.h; Copyright and License: see below */

#ifndef CTRL_UNDO_REDO_LIST_H
#define CTRL_UNDO_REDO_LIST_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief reverts and re-performs changes to the database
 */

#include "ctrl_error.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the ctrl_undo_redo_list functions
 */
struct ctrl_undo_redo_list_struct {
    data_database_writer_t *db_writer;  /*!< pointer to external database writer */
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
};

typedef struct ctrl_undo_redo_list_struct ctrl_undo_redo_list_t;

/*!
 *  \brief initializes the ctrl_undo_redo_list_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to database object that can be used for retrieving data
 *  \param db_writer pointer to database object that can be used for changing data
 */
void ctrl_undo_redo_list_init ( ctrl_undo_redo_list_t *this_, data_database_reader_t *db_reader, data_database_writer_t *db_writer );

/*!
 *  \brief destroys the ctrl_undo_redo_list_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_undo_redo_list_destroy ( ctrl_undo_redo_list_t *this_ );

#endif  /* CTRL_UNDO_REDO_LIST_H */


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
