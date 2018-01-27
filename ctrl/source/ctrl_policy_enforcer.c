/* File: ctrl_policy_enforcer.c; Copyright and License: see below */

#include "ctrl_policy_enforcer.h"
#include "trace.h"
#include "tslog.h"

void ctrl_policy_enforcer_init ( ctrl_policy_enforcer_t *this_,
                                 ctrl_undo_redo_list_t *undo_redo_list,
                                 data_database_t *database,
                                 data_database_reader_t *db_reader,
                                 data_database_writer_t *db_writer )
{
    TRACE_BEGIN();

    (*this_).undo_redo_list = undo_redo_list;
    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;

    TRACE_END();
}

void ctrl_policy_enforcer_destroy ( ctrl_policy_enforcer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).undo_redo_list = NULL;
    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    TRACE_END();
}

/* ================================ LIFELINES ================================ */

ctrl_error_t ctrl_policy_enforcer_update_diagram_type ( ctrl_policy_enforcer_t *this_,
                                                        const data_diagram_t *new_diagram,
                                                        bool add_to_latest_undo_set,
                                                        int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram );
    data_diagram_t to_be_created;
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    int64_t new_id;

    data_diagram_copy( &to_be_created, new_diagram );
    data_diagram_set_id( &to_be_created, DATA_ID_VOID_ID );

    data_result = data_database_writer_create_diagram( (*this_).db_writer, &to_be_created, &new_id );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* store new id to diagram object */
        data_diagram_set_id( &to_be_created, new_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set )
        {
            ctrl_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( CTRL_ERROR_NONE != internal_err )
            {
                TSLOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the new diagram to the undo redo list */
        ctrl_undo_redo_list_add_create_diagram( (*this_).undo_redo_list, &to_be_created );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* copy new id to out parameter */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result = (ctrl_error_t) data_result;

    data_diagram_destroy( &to_be_created );

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2018-2018 Andreas Warnke

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
