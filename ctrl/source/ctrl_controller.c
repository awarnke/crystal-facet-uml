/* File: ctrl_controller.c; Copyright and License: see below */

#include "ctrl_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void ctrl_controller_init ( ctrl_controller_t *this_, data_database_t *database )
{
    U8_TRACE_BEGIN();

    /* init member attributes */
    (*this_).database = database;
    data_database_reader_init( &((*this_).db_reader), database );
    data_database_writer_init( &((*this_).db_writer), &((*this_).db_reader), database );
    ctrl_undo_redo_list_init ( &((*this_).undo_redo_list), &((*this_).db_reader), &((*this_).db_writer) );
    ctrl_classifier_policy_enforcer_init ( &((*this_).classifier_policy_enforcer), &((*this_).db_reader), &((*this_).classifiers), &((*this_).diagrams) );
    ctrl_classifier_controller_init ( &((*this_).classifiers),
                                      &((*this_).undo_redo_list),
                                      &((*this_).classifier_policy_enforcer),
                                      database,
                                      &((*this_).db_reader),
                                      &((*this_).db_writer)
                                    );
    ctrl_diagram_policy_enforcer_init ( &((*this_).diagram_policy_enforcer), &((*this_).db_reader), &((*this_).classifiers), &((*this_).diagrams) );
    ctrl_diagram_controller_init ( &((*this_).diagrams),
                                   &((*this_).undo_redo_list),
                                   &((*this_).diagram_policy_enforcer),
                                   database,
                                   &((*this_).db_reader),
                                   &((*this_).db_writer)
                                 );
    ctrl_consistency_checker_init ( &((*this_).consistency_checker), database, &((*this_).db_reader), &((*this_).db_writer) );

    /* listen on db open and prepare_close events: */
    data_database_listener_init ( &((*this_).me_as_listener),
                                  this_,
                                  (void (*)(void*,data_database_listener_signal_t)) &ctrl_controller_db_change_callback
                                );
    data_database_add_db_listener( (*this_).database, &((*this_).me_as_listener) );

    U8_TRACE_END();
}

void ctrl_controller_destroy ( ctrl_controller_t *this_ )
{
    U8_TRACE_BEGIN();

    /* stop listening on db open and prepare_close events: */
    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    /* destroy member attributes */
    ctrl_consistency_checker_destroy ( &((*this_).consistency_checker) );
    ctrl_diagram_controller_destroy ( &((*this_).diagrams) );
    ctrl_diagram_policy_enforcer_destroy ( &((*this_).diagram_policy_enforcer) );
    ctrl_classifier_policy_enforcer_destroy ( &((*this_).classifier_policy_enforcer) );
    ctrl_classifier_controller_destroy ( &((*this_).classifiers) );
    ctrl_undo_redo_list_destroy ( &((*this_).undo_redo_list) );
    data_database_writer_destroy( &((*this_).db_writer) );
    data_database_reader_destroy( &((*this_).db_reader) );

    U8_TRACE_END();
}

/* ================================ interface for database file ================================ */

void ctrl_controller_db_change_callback ( ctrl_controller_t *this_, data_database_listener_signal_t signal_id )
{
    U8_TRACE_BEGIN();

    /* for any db event, clear the undo history */
    ctrl_undo_redo_list_clear( &((*this_).undo_redo_list) );

    U8_TRACE_END();
}


/*
Copyright 2016-2022 Andreas Warnke

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
