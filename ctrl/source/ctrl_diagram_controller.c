/* File: ctrl_diagram_controller.c; Copyright and License: see below */

#include "ctrl_diagram_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void ctrl_diagram_controller_init( ctrl_diagram_controller_t *this_,
                                   ctrl_undo_redo_list_t *undo_redo_list,
                                   ctrl_diagram_trigger_t *policy_enforcer,
                                   data_database_t *database,
                                   data_database_reader_t *db_reader,
                                   data_database_writer_t *db_writer )
{
    U8_TRACE_BEGIN();

    (*this_).undo_redo_list = undo_redo_list;
    (*this_).policy_enforcer = policy_enforcer;
    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;

    U8_TRACE_END();
}

void ctrl_diagram_controller_destroy( ctrl_diagram_controller_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).undo_redo_list = NULL;
    (*this_).policy_enforcer = NULL;
    (*this_).database = NULL;
    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    U8_TRACE_END();
}

/* ================================ DIAGRAM ================================ */

u8_error_t ctrl_diagram_controller_create_diagram( ctrl_diagram_controller_t *this_,
                                                   const data_diagram_t *new_diagram,
                                                   ctrl_undo_redo_action_boundary_t add_to_latest_undo_set,
                                                   data_row_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_diagram );
    data_diagram_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_t new_id;

    data_diagram_copy( &to_be_created, new_diagram );

    data_result = data_database_writer_create_diagram( (*this_).db_writer, &to_be_created, &new_id );
    if ( U8_ERROR_NONE == data_result )
    {
        /* store new id to diagram object */
        data_diagram_set_row_id( &to_be_created, new_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
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
    result = data_result;

    data_diagram_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_private_create_child_diagram( ctrl_diagram_controller_t *this_,
                                                                 data_row_t parent_diagram_id,
                                                                 data_diagram_type_t diagram_type,
                                                                 const char* diagram_name,
                                                                 data_row_t* out_new_id )
{
    U8_TRACE_BEGIN();
    data_diagram_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_t new_id;

    data_diagram_init_new( &to_be_created, parent_diagram_id, diagram_type, "", diagram_name, "", 0, DATA_DIAGRAM_FLAG_NONE );

    data_result = data_database_writer_create_diagram( (*this_).db_writer, &to_be_created, &new_id );
    if ( U8_ERROR_NONE == data_result )
    {
        /* store new id to diagram object */
        data_diagram_set_row_id( &to_be_created, new_id );

        /* store the new diagram to the undo redo list */
        ctrl_undo_redo_list_add_create_diagram( (*this_).undo_redo_list, &to_be_created );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* copy new id to out parameter */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result = data_result;

    data_diagram_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_create_root_diagram_if_not_exists( ctrl_diagram_controller_t *this_,
                                                                      data_diagram_type_t diagram_type,
                                                                      const char* diagram_name,
                                                                      data_row_t* out_new_id )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    /* load all without parent */
    data_diagram_iterator_t diagram_iterator;
    result |= data_diagram_iterator_init_empty( &diagram_iterator );
    result |= data_database_reader_get_diagrams_by_parent_id( (*this_).db_reader,
                                                              DATA_ROW_VOID,
                                                              &diagram_iterator
                                                            );
    const bool has_root = data_diagram_iterator_has_next( &diagram_iterator );
    result |= data_diagram_iterator_destroy( &diagram_iterator );

    if ( result == U8_ERROR_NONE )
    {
        if ( ! has_root )
        {
            /* no root diagram exists */
            result |= ctrl_diagram_controller_private_create_child_diagram( this_, DATA_ROW_VOID, diagram_type, diagram_name, out_new_id );
        }
        else
        {
            if ( NULL != out_new_id )
            {
                *out_new_id = DATA_ROW_VOID;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_delete_diagram ( ctrl_diagram_controller_t *this_,
                                                    data_row_t obj_id,
                                                    ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    /* delete diagram */
    /* data_database_writer_delete_diagram checks that this diagram is not a parent */
    /* and is not referenced by diagramelements */
    /* fails otherwise: U8_ERROR_OBJECT_STILL_REFERENCED */
    data_diagram_t old_diagram;
    u8_error_t current_result3;
    current_result3 = data_database_writer_delete_diagram ( (*this_).db_writer, obj_id, &old_diagram );

    if ( U8_ERROR_NONE == current_result3 )
    {
        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the deleted diagram to the undo redo list */
        ctrl_undo_redo_list_add_delete_diagram( (*this_).undo_redo_list, &old_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_diagram_destroy( &old_diagram );
    }

    result |= (u8_error_t) current_result3;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_update_diagram_parent_id ( ctrl_diagram_controller_t *this_,
                                                              data_row_t diagram_id,
                                                              data_row_t new_diagram_parent_id,
                                                              ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_parent_id( (*this_).db_writer, diagram_id, new_diagram_parent_id, &old_diagram );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_parent_row_id( &new_diagram, new_diagram_parent_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_diagram_destroy( &new_diagram );
        data_diagram_destroy( &old_diagram );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_update_diagram_type ( ctrl_diagram_controller_t *this_,
                                                         data_row_t diagram_id,
                                                         data_diagram_type_t new_diagram_type,
                                                         data_stat_t *io_stat )
{
    U8_TRACE_BEGIN();
    assert( io_stat != NULL );
    u8_error_t result = U8_ERROR_NONE;
    data_diagram_t old_diagram;

    result |= data_database_writer_update_diagram_type( (*this_).db_writer, diagram_id, new_diagram_type, &old_diagram );
    if ( U8_ERROR_NONE == result )
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_diagram_type( &new_diagram, new_diagram_type );

        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* apply policy rules */
        result |= ctrl_diagram_trigger_post_update_diagram_type( (*this_).policy_enforcer, &new_diagram );

        data_diagram_destroy( &new_diagram );
        data_diagram_destroy( &old_diagram );

        /* report statistics */
        /* changing diagram types may create or delete lifelines and messages, */
        /* therefore ask the undo_redo_list for statistics */
        ctrl_undo_redo_iterator_t iter;
        ctrl_undo_redo_iterator_init_empty( &iter );
        result |= ctrl_undo_redo_list_get_undo_iterator( (*this_).undo_redo_list, &iter );
        ctrl_undo_redo_iterator_collect_statistics( &iter, false /* NOT categorize as undo */, io_stat );
        ctrl_undo_redo_iterator_destroy( &iter );
    }
    else
    {
        data_stat_inc_count ( io_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_ERROR );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_update_diagram_stereotype ( ctrl_diagram_controller_t *this_,
                                                               data_row_t diagram_id,
                                                               const char* new_diagram_stereotype )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_stereotype( (*this_).db_writer, diagram_id, new_diagram_stereotype, &old_diagram );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_stereotype( &new_diagram, new_diagram_stereotype );
        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_diagram_destroy( &new_diagram );
        data_diagram_destroy( &old_diagram );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_update_diagram_name ( ctrl_diagram_controller_t *this_,
                                                         data_row_t diagram_id,
                                                         const char* new_diagram_name )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_name( (*this_).db_writer, diagram_id, new_diagram_name, &old_diagram );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_name( &new_diagram, new_diagram_name );
        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_diagram_destroy( &new_diagram );
        data_diagram_destroy( &old_diagram );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_update_diagram_description ( ctrl_diagram_controller_t *this_,
                                                                data_row_t diagram_id,
                                                                const char* new_diagram_description )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_description( (*this_).db_writer, diagram_id, new_diagram_description, &old_diagram );
    if  (( U8_ERROR_NONE == data_result ) || ( U8_ERROR_STRING_BUFFER_EXCEEDED == data_result ))
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_description( &new_diagram, new_diagram_description );
        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_diagram_destroy( &new_diagram );
        data_diagram_destroy( &old_diagram );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_update_diagram_list_order ( ctrl_diagram_controller_t *this_,
                                                               data_row_t diagram_id,
                                                               int32_t new_diagram_list_order )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_diagram_t old_diagram;

    data_result = data_database_writer_update_diagram_list_order( (*this_).db_writer, diagram_id, new_diagram_list_order, &old_diagram );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new diagram */
        data_diagram_t new_diagram;
        data_diagram_copy( &new_diagram, &old_diagram );
        data_diagram_set_list_order( &new_diagram, new_diagram_list_order );
        /* store the change of the diagram to the undo redo list */
        ctrl_undo_redo_list_add_update_diagram( (*this_).undo_redo_list, &old_diagram, &new_diagram );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_diagram_destroy( &new_diagram );
        data_diagram_destroy( &old_diagram );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAMELEMENT ================================ */

u8_error_t ctrl_diagram_controller_create_diagramelement( ctrl_diagram_controller_t *this_,
                                                          const data_diagramelement_t *new_diagramelement,
                                                          ctrl_undo_redo_action_boundary_t add_to_latest_undo_set,
                                                          data_row_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_diagramelement );
    data_diagramelement_t to_be_created;
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_row_t new_id;

    data_diagramelement_copy( &to_be_created, new_diagramelement );

    data_result = data_database_writer_create_diagramelement( (*this_).db_writer, &to_be_created, &new_id );
    if ( U8_ERROR_NONE == data_result )
    {
        /* store new id to data_diagramelement_t object */
        data_diagramelement_set_row_id( &to_be_created, new_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the new diagram to the undo redo list */
        ctrl_undo_redo_list_add_create_diagramelement( (*this_).undo_redo_list, &to_be_created );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* apply policies */
        result |= ctrl_diagram_trigger_post_create_diagramelement( (*this_).policy_enforcer, &to_be_created );

        /* copy new id to out parameter */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    else
    {
        result = data_result;
    }

    data_diagramelement_destroy( &to_be_created );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_delete_diagramelement( ctrl_diagram_controller_t *this_,
                                                          data_row_t obj_id,
                                                          ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    /* delete diagramelement */
    u8_error_t current_result;
    data_diagramelement_t old_diagramelement;
    current_result = data_database_writer_delete_diagramelement( (*this_).db_writer,
                                                                 obj_id,
                                                                 &old_diagramelement
                                                               );

    if ( U8_ERROR_NONE == current_result )
    {
        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the deleted classifier to the undo redo list */
        ctrl_undo_redo_list_add_delete_diagramelement( (*this_).undo_redo_list, &old_diagramelement );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        /* try to also delete the classifier and focused lifelines */
        result |= ctrl_diagram_trigger_post_delete_diagramelement( (*this_).policy_enforcer, &old_diagramelement );

        data_diagramelement_destroy( &old_diagramelement );
    }
    else
    {
        result |= (u8_error_t) current_result;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_update_diagramelement_display_flags( ctrl_diagram_controller_t *this_,
                                                                        data_row_t diagramelement_id,
                                                                        data_diagramelement_flag_t new_diagramelement_display_flags,
                                                                        ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_diagramelement_t old_diagramelement;

    data_result = data_database_writer_update_diagramelement_display_flags( (*this_).db_writer,
                                                                            diagramelement_id,
                                                                            new_diagramelement_display_flags,
                                                                            &old_diagramelement
                                                                          );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new diagram */
        data_diagramelement_t new_diagramelement;
        data_diagramelement_copy( &new_diagramelement, &old_diagramelement );
        data_diagramelement_set_display_flags( &new_diagramelement, new_diagramelement_display_flags );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the change of the diagramelement to the undo redo list */
        ctrl_undo_redo_list_add_update_diagramelement( (*this_).undo_redo_list, &old_diagramelement, &new_diagramelement );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_diagramelement_destroy( &new_diagramelement );
        data_diagramelement_destroy( &old_diagramelement );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_controller_update_diagramelement_focused_feature_id( ctrl_diagram_controller_t *this_,
                                                                             data_row_t diagramelement_id,
                                                                             data_row_t new_diagramelement_focused_feature_id,
                                                                             ctrl_undo_redo_action_boundary_t add_to_latest_undo_set )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;
    data_diagramelement_t old_diagramelement;

    data_result = data_database_writer_update_diagramelement_focused_feature_id( (*this_).db_writer,
                                                                                 diagramelement_id,
                                                                                 new_diagramelement_focused_feature_id,
                                                                                 &old_diagramelement
                                                                               );
    if ( U8_ERROR_NONE == data_result )
    {
        /* prepare the new diagram */
        data_diagramelement_t new_diagramelement;
        data_diagramelement_copy( &new_diagramelement, &old_diagramelement );
        data_diagramelement_set_focused_feature_row_id( &new_diagramelement, new_diagramelement_focused_feature_id );

        /* if this action shall be stored to the latest set of actions in the undo redo list, remove the boundary: */
        if ( add_to_latest_undo_set == CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND )
        {
            u8_error_t internal_err;
            internal_err = ctrl_undo_redo_list_remove_boundary_from_end( (*this_).undo_redo_list );
            if ( U8_ERROR_NONE != internal_err )
            {
                U8_LOG_ERROR_HEX( "unexpected internal error", internal_err );
            }
        }

        /* store the change of the diagramelement to the undo redo list */
        ctrl_undo_redo_list_add_update_diagramelement( (*this_).undo_redo_list, &old_diagramelement, &new_diagramelement );
        ctrl_undo_redo_list_add_boundary( (*this_).undo_redo_list );

        data_diagramelement_destroy( &new_diagramelement );
        data_diagramelement_destroy( &old_diagramelement );
    }
    result = data_result;

    U8_TRACE_END_ERR( result );
    return result;
}


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
