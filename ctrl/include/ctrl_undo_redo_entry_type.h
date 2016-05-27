/* File: ctrl_undo_redo_entry_type.h; Copyright and License: see below */

#ifndef CTRL_UNDO_REDO_ENTRY_TYPE_H
#define CTRL_UNDO_REDO_ENTRY_TYPE_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Indentifies the type of action of an ctrl_undo_redo_entry_t that can be undone or redone
 */

#include "ctrl_error.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "data_diagram_type.h"
#include "data_diagram.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the diagram functions
 */
struct ctrl_undo_redo_entry_type_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t db_writer;  /*!< own instance of a database writer */
    data_database_reader_t db_reader;  /*!< own instance of a database reader */
};

typedef struct ctrl_diagram_controller_struct ctrl_diagram_controller_t;

#endif  /* CTRL_UNDO_REDO_ENTRY_TYPE_H */


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
