/* File: ctrl_controller.h; Copyright and License: see below */

#ifndef CTRL_CONTROLLER_H
#define CTRL_CONTROLLER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides an entry point for write access and consistency checks to the database
 */

#include "ctrl_error.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "ctrl_undo_redo_list.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "util/string/utf8stringbuf.h"

/*!
 *  \brief all data attributes needed for the controller functions
 */
struct ctrl_controller_struct {
    ctrl_classifier_controller_t classifiers;  /*!< own instance of a classifier controller */
    ctrl_diagram_controller_t diagrams;  /*!< own instance of a diagram controller */
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t db_writer;  /*!< own instance of a database writer */
    data_database_reader_t db_reader;  /*!< own instance of a database reader */
    ctrl_undo_redo_list_t undo_redo_list;  /*!< own instance of a ctrl_undo_redo_list_t */
    ctrl_consistency_checker_t consistency_checker;  /* own instance of a consistency checker */
};

typedef struct ctrl_controller_struct ctrl_controller_t;

/*!
 *  \brief initializes the ctrl_controller_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this controller uses
 */
void ctrl_controller_init ( ctrl_controller_t *this_, data_database_t *database );

/*!
 *  \brief destroys the ctrl_controller_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_controller_destroy ( ctrl_controller_t *this_ );

/*!
 *  \brief gets the classifier controller
 *
 *  \param this_ pointer to own object attributes
 */
static inline ctrl_classifier_controller_t *ctrl_controller_get_classifier_control_ptr ( ctrl_controller_t *this_ );

/*!
 *  \brief gets the diagram controller
 *
 *  \param this_ pointer to own object attributes
 */
static inline ctrl_diagram_controller_t *ctrl_controller_get_diagram_control_ptr ( ctrl_controller_t *this_ );

/*!
 *  \brief un-does a set of actions.
 *
 *  \param this_ pointer to own object attributes
 *  \return CTRL_ERROR_ARRAY_BUFFER_EXCEEDED if there is no more complete set of actions to be un-done due to limits of buffer.
 *          CTRL_ERROR_INVALID_REQUEST if there is no more set of actions to be un-done
 *          CTRL_ERROR_NONE otherwise.
 */
static inline ctrl_error_t ctrl_controller_undo ( ctrl_controller_t *this_ );

/*!
 *  \brief re-does a set of actions.
 *
 *  \param this_ pointer to own object attributes
 *  \return CTRL_ERROR_INVALID_REQUEST if there is no more set of actions to be re-done.
 *          CTRL_ERROR_NONE otherwise.
 */
static inline ctrl_error_t ctrl_controller_redo ( ctrl_controller_t *this_ );

/*!
 *  \brief switches the currently used database file
 *
 *  \param this_ pointer to own object attributes
 *  \param db_file_path file name of the new database to be used
 */
ctrl_error_t ctrl_controller_switch_database ( ctrl_controller_t *this_, const char* db_file_path );

/*!
 *  \brief checks and repairs the database
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param out_report english text stating what was checked and the results and what was reparied and the results
 *  \return CTRL_ERROR_NONE in case of success,
 *          CTRL_ERROR_NO_DB if database not open/loaded,
 *          CTRL_ERROR_DB_STRUCTURE if database was corrupted
 */
static inline ctrl_error_t ctrl_controller_repair_database ( ctrl_controller_t *this_, bool modify_db, utf8stringbuf_t out_report );

#include "ctrl_controller.inl"

#endif  /* CTRL_CONTROLLER_H */


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
