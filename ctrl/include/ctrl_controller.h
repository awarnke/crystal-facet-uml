/* File: ctrl_controller.h; Copyright and License: see below */

#ifndef CTRL_CONTROLLER_H
#define CTRL_CONTROLLER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Provides an entry point for write access and consistency checks to the database
 */

#include "u8/u8_error.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_classifier_trigger.h"
#include "ctrl_diagram_controller.h"
#include "ctrl_diagram_trigger.h"
#include "ctrl_undo_redo_list.h"
#include "ctrl_undo_redo_iterator.h"
#include "consistency/consistency_drop_invisibles.h"
#include "consistency/consistency_lifeline.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "set/data_stat.h"
#include "utf8stream/utf8stream_writer.h"

/*!
 *  \brief all data attributes needed for the controller functions
 */
struct ctrl_controller_struct {
    ctrl_classifier_controller_t classifiers;  /*!< own instance of a classifier controller */
    ctrl_classifier_trigger_t classifier_trigger;  /*!< own instance of a classifier trigger methods */
    ctrl_diagram_controller_t diagrams;  /*!< own instance of a diagram controller */
    ctrl_diagram_trigger_t diagram_trigger;  /*!< own instance of a diagram trigger methods */
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t db_writer;  /*!< own instance of a database writer */
    data_database_reader_t db_reader;  /*!< own instance of a database reader */
    ctrl_undo_redo_list_t undo_redo_list;  /*!< own instance of a ctrl_undo_redo_list_t */
    ctrl_consistency_checker_t consistency_checker;  /* own instance of a consistency checker */
    consistency_drop_invisibles_t consistency_drop_invisibles;  /*!< own instance of consistency_drop_invisibles_t */
    consistency_lifeline_t consistency_lifeline;  /*!< own instance of consistency_lifeline_t */

    data_database_listener_t me_as_listener;  /*!< own instance of data_database_listener_t */
                                              /*!< which wraps ctrl_controller_db_change_callback */
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
 *  \brief Begins a transaction
 *
 *  This function may be called recursively.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, an error id otherwise, e.g. U8_ERROR_NO_DB in case the database is not open
 */
static inline u8_error_t ctrl_controller_transaction_begin ( ctrl_controller_t *this_ );

/*!
 *  \brief Commits a transaction
 *
 *  This function may be called recursively.
 *  The commit is only executed when this function is called once for each preceding data_database_transaction_begin call.
 *
 *  \param this_ pointer to own object attributes
 *  \return U8_ERROR_NONE in case of success, an error id otherwise
 */
static inline u8_error_t ctrl_controller_transaction_commit ( ctrl_controller_t *this_ );

/* ================================ interface for undo redo ================================ */

/*!
 *  \brief un-does a set of actions.
 *
 *  \param this_ pointer to own object attributes
 *  \param[in,out] io_stat Statistics on DATA_STAT_SERIES_CREATED, DATA_STAT_SERIES_MODIFIED,
 *                         DATA_STAT_SERIES_DELETED and
 *                         DATA_STAT_SERIES_ERROR (e.g. if name not unique due to parallel working on same db).
 *                         *io_stat shall be initialized by caller, statistics are added to initial values.
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED if there is no more complete set of actions to be un-done due to limits of buffer.
 *          U8_ERROR_INVALID_REQUEST if there is no more set of actions to be un-done
 *          U8_ERROR_NONE otherwise.
 */
static inline u8_error_t ctrl_controller_undo ( ctrl_controller_t *this_, data_stat_t *io_stat );

/*!
 *  \brief re-does a set of actions.
 *
 *  \param this_ pointer to own object attributes
 *  \param[in,out] io_stat Statistics on DATA_STAT_SERIES_CREATED, DATA_STAT_SERIES_MODIFIED,
 *                         DATA_STAT_SERIES_DELETED and
 *                         DATA_STAT_SERIES_ERROR (e.g. if name not unique due to parallel working on same db).
 *                         *io_stat shall be initialized by caller, statistics are added to initial values.
 *  \return U8_ERROR_INVALID_REQUEST if there is no more set of actions to be re-done.
 *          U8_ERROR_NONE otherwise.
 */
static inline u8_error_t ctrl_controller_redo ( ctrl_controller_t *this_, data_stat_t *io_stat );

/*!
 *  \brief gets an iterator on the last (=undo) set of actions
 *
 *  Before performing an undo, this function returns the expected changes.
 *  After performing an successful redo, this function returns the changes.
 *
 *  The iterator is valid only till the next change action on the data.
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_undo_iterator a valid iterator if U8_ERROR_NONE (providing NULL is not allowed)
 *  \return U8_ERROR_ARRAY_BUFFER_EXCEEDED if there is no more complete set of actions to be un-done due to limits of buffer.
 *          U8_ERROR_INVALID_REQUEST if there is no more set of actions to be un-done
 *          U8_ERROR_NONE otherwise.
 */
static inline u8_error_t ctrl_controller_get_undo_iterator ( const ctrl_controller_t *this_,
                                                             ctrl_undo_redo_iterator_t *out_undo_iterator
                                                           );

/*!
 *  \brief gets an iterator on the next (=redo) set of actions
 *
 *  Before performing an redo, this function returns the expected changes.
 *  After performing an successful undo, this function returns the changes.
 *
 *  The iterator is valid only till the next change action on the data.
 *
 *  \param this_ pointer to own object attributes
 *  \param[out] out_redo_iterator a valid iterator if U8_ERROR_NONE (providing NULL is not allowed)
 *  \return U8_ERROR_INVALID_REQUEST if there is no more set of actions to be re-done
 *          U8_ERROR_NONE otherwise.
 */
static inline u8_error_t ctrl_controller_get_redo_iterator ( const ctrl_controller_t *this_,
                                                             ctrl_undo_redo_iterator_t *out_redo_iterator
                                                           );

/* ================================ interface for database file ================================ */

/*!
 *  \brief switches the currently used database file
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_id state of database change
 */
void ctrl_controller_db_change_callback ( ctrl_controller_t *this_, data_database_listener_signal_t signal_id );

/*!
 *  \brief checks and repairs the database
 *
 *  \param this_ pointer to own object attributes
 *  \param modify_db true if the database shall be repaired and modified
 *  \param[out] out_err number of errors detected (NULL if not requested)
 *  \param[out] out_fix number of errors fixed (NULL if not requested)
 *  \param[out] out_english_report english text stating what was checked and the results and what was reparied and the results
 *  \return U8_ERROR_NONE in case of success,
 *          U8_ERROR_NO_DB if database not open/loaded,
 *          U8_ERROR_DB_STRUCTURE if database was corrupted and is not fully fixed yet
 */
static inline u8_error_t ctrl_controller_repair_database ( ctrl_controller_t *this_,
                                                           bool modify_db,
                                                           uint32_t *out_err,
                                                           uint32_t *out_fix,
                                                           utf8stream_writer_t *out_english_report
                                                         );

#include "ctrl_controller.inl"

#endif  /* CTRL_CONTROLLER_H */


/*
Copyright 2016-2025 Andreas Warnke

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
