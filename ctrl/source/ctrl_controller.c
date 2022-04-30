/* File: ctrl_controller.c; Copyright and License: see below */

#include "ctrl_controller.h"
#include "trace/trace.h"
#include "tslog/tslog.h"

void ctrl_controller_init ( ctrl_controller_t *this_, data_database_t *database )
{
    TRACE_BEGIN();

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

    TRACE_END();
}

void ctrl_controller_destroy ( ctrl_controller_t *this_ )
{
    TRACE_BEGIN();

    /* destroy member attributes */
    ctrl_consistency_checker_destroy ( &((*this_).consistency_checker) );
    ctrl_diagram_controller_destroy ( &((*this_).diagrams) );
    ctrl_diagram_policy_enforcer_destroy ( &((*this_).diagram_policy_enforcer) );
    ctrl_classifier_policy_enforcer_destroy ( &((*this_).classifier_policy_enforcer) );
    ctrl_classifier_controller_destroy ( &((*this_).classifiers) );
    ctrl_undo_redo_list_destroy ( &((*this_).undo_redo_list) );
    data_database_writer_destroy( &((*this_).db_writer) );
    data_database_reader_destroy( &((*this_).db_reader) );

    TRACE_END();
}

/* ================================ interface for database file ================================ */

u8_error_t ctrl_controller_switch_database ( ctrl_controller_t *this_, const char* db_file_path )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    const u8_error_t close_result = data_database_close( (*this_).database );
    ctrl_undo_redo_list_clear( &((*this_).undo_redo_list) );
    if ( U8_ERROR_NONE != close_result )
    {
        /* we do not care about errors at closing, trace and ignore close_result */
        TRACE_INFO_HEX( "Error at data_database_close", close_result );
    }

    const u8_error_t open_result = data_database_open( (*this_).database, db_file_path );
    result = (u8_error_t) open_result;

    TRACE_END_ERR( result );
    return result;
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
