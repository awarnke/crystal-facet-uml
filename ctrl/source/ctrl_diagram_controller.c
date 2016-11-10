/* File: ctrl_diagram_controller.c; Copyright and License: see below */

#include "ctrl_diagram_controller.h"
#include "trace.h"
#include "log.h"

void ctrl_diagram_controller_init ( ctrl_diagram_controller_t *this_,
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

void ctrl_diagram_controller_destroy ( ctrl_diagram_controller_t *this_ )
{
    TRACE_BEGIN();

    (*this_).undo_redo_list = NULL;
    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    TRACE_END();
}

ctrl_error_t ctrl_diagram_controller_create_child_diagram ( ctrl_diagram_controller_t *this_,
                                                            int64_t parent_diagram_id,
                                                            data_diagram_type_t diagram_type,
                                                            const char* diagram_name,
                                                            int64_t* out_new_id )
{
    TRACE_BEGIN();
    data_diagram_t to_be_created;
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    int64_t new_id;

    data_diagram_init_new( &to_be_created, parent_diagram_id, diagram_type, diagram_name, "", 0 );

    data_result = data_database_writer_create_diagram( (*this_).db_writer, &to_be_created, &new_id );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* store new id to diagram object */
        data_diagram_set_id( &to_be_created, new_id );

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

ctrl_error_t ctrl_diagram_controller_create_root_diagram_if_not_exists ( ctrl_diagram_controller_t *this_,
                                                                         data_diagram_type_t diagram_type,
                                                                         const char* diagram_name,
                                                                         int64_t* out_new_id )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_diagram_t root_diag_buf[1];

    /* load all without parent */
    uint32_t count;
    data_result = data_database_reader_get_diagrams_by_parent_id( (*this_).db_reader,
                                                                  DATA_ID_VOID_ID,
                                                                  1,
                                                                  &(root_diag_buf),
                                                                  &count
                                                                );
    if ( DATA_ERROR_NONE != data_result )
    {
        result = (ctrl_error_t) data_result;
    }
    else
    {
        if ( 0 == count )
        {
            /* no root diagram exists */
            result = ctrl_diagram_controller_create_child_diagram( this_, DATA_ID_VOID_ID, diagram_type, diagram_name, out_new_id );
        }
        else
        {
            if ( NULL != out_new_id )
            {
                *out_new_id = DATA_ID_VOID_ID;
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_update_diagram_description ( ctrl_diagram_controller_t *this_,
                                                                  int64_t diagram_id,
                                                                  const char* new_diagram_description )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_description( (*this_).db_writer, diagram_id, new_diagram_description, &old_diagram );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_description( &new_diagram, new_diagram_description );
        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_update_diagram_name ( ctrl_diagram_controller_t *this_,
                                                           int64_t diagram_id,
                                                           const char* new_diagram_name )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_name( (*this_).db_writer, diagram_id, new_diagram_name, &old_diagram );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_name( &new_diagram, new_diagram_name );
        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_update_diagram_type ( ctrl_diagram_controller_t *this_,
                                                           int64_t diagram_id,
                                                           data_diagram_type_t new_diagram_type )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_type( (*this_).db_writer, diagram_id, new_diagram_type, &old_diagram );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_diagram_type( &new_diagram, new_diagram_type );
        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_update_diagram_list_order ( ctrl_diagram_controller_t *this_,
                                                                 int64_t diagram_id,
                                                                 int32_t new_diagram_list_order )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_list_order( (*this_).db_writer, diagram_id, new_diagram_list_order, &old_diagram );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_list_order( &new_diagram, new_diagram_list_order );
        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );
    }
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_create_diagram ( ctrl_diagram_controller_t *this_,
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
                LOG_ERROR_HEX( "unexpected internal error", internal_err );
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

ctrl_error_t ctrl_diagram_controller_create_diagramelement ( ctrl_diagram_controller_t *this_,
                                                             const data_diagramelement_t *new_diagramelement,
                                                             bool add_to_latest_undo_set,
                                                             int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != new_diagramelement );
    data_diagramelement_t to_be_created;
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    int64_t new_id;

    data_diagramelement_copy( &to_be_created, new_diagramelement );
    data_diagramelement_set_id( &to_be_created, DATA_ID_VOID_ID );

    data_result = data_database_writer_create_diagramelement( (*this_).db_writer, &to_be_created, &new_id );
    if ( DATA_ERROR_NONE == data_result )
    {
        /* store new id to data_diagramelement_t object */
        data_diagramelement_set_id( &to_be_created, new_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set )
        {
            ctrl_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( CTRL_ERROR_NONE != internal_err )
            {
                LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the new diagram to the undo redo list */
        ctrl_undo_redo_list_add_create_diagramelement( (*this_).undo_redo_list, &to_be_created );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* copy new id to out parameter */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result = (ctrl_error_t) data_result;

    data_diagramelement_destroy( &to_be_created );

    TRACE_END_ERR( result );
    return result;
}


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
