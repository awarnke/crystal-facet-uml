/* File: ctrl_diagram_controller.h; Copyright and License: see below */

#ifndef CTRL_DIAGRAM_CONTROLLER_H
#define CTRL_DIAGRAM_CONTROLLER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "ctrl_error.h"
#include "data_database.h"
#include "data_database_writer.h"
#include "data_database_reader.h"
#include "data_diagram_type.h"
#include "data_diagram.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the diagram functions
 */
struct ctrl_diagram_controller_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t db_writer;  /*!< own instance of a database writer */
    data_database_reader_t db_reader;  /*!< own instance of a database reader */
};

typedef struct ctrl_diagram_controller_struct ctrl_diagram_controller_t;

/*!
 *  \brief initializes the ctrl_diagram_controller_t struct
 */
void ctrl_diagram_controller_init ( ctrl_diagram_controller_t *this_, data_database_t *database );

/*!
 *  \brief destroys the ctrl_diagram_controller_t struct
 */
void ctrl_diagram_controller_destroy ( ctrl_diagram_controller_t *this_ );

/*!
 *  \brief creates a new diagram
 *
 *  \param out_new_id id of the newly created diagram
 */
ctrl_error_t ctrl_diagram_controller_create_diagram ( ctrl_diagram_controller_t *this_, int64_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name, int64_t* out_new_id );

/*!
 *  \brief updates a diagram attribute
 *
 *  \return error id in case of error
 */
ctrl_error_t ctrl_diagram_controller_update_diagram_name ( ctrl_diagram_controller_t *this_, int64_t diagram_id, const char* diagram_name );


#endif  /* CTRL_DIAGRAM_CONTROLLER_H */


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
