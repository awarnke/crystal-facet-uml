/* File: ctrl_controller.c; Copyright and License: see below */

#include "ctrl_controller.h"
#include "trace.h"
#include "tslog.h"

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

/* ================================ interface for sets of elements ================================ */

u8_error_t ctrl_controller_delete_set ( ctrl_controller_t *this_,
                                          const data_small_set_t *objects,
                                          data_stat_t *io_stat )
{
    TRACE_BEGIN();
    assert ( NULL != io_stat );
    u8_error_t result = U8_ERROR_NONE;

    if ( data_small_set_is_empty( objects ) )
    {
        result = U8_ERROR_INPUT_EMPTY;
    }
    else
    {
        int index;

        /* add a boundary to the undo redo list before all deletions */
        /* every delete will move this boundary, because add_to_latest_undo_set is CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND */
        ctrl_undo_redo_list_add_boundary( &((*this_).undo_redo_list) );

        /* STEP ZERO: Delete all objects that can be immediately deleted */

        for ( index = 0; index < data_small_set_get_count( objects ); index ++ )
        {
            data_id_t current_id;
            current_id = data_small_set_get_id( objects, index );
            switch ( data_id_get_table( &current_id ) )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    /* see step two */
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    result |= ctrl_classifier_controller_delete_feature ( &((*this_).classifiers),
                                                                          data_id_get_row_id( &current_id ),
                                                                          CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                        );
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    result |= ctrl_classifier_controller_delete_relationship ( &((*this_).classifiers),
                                                                                data_id_get_row_id( &current_id ),
                                                                                CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                              );
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    /* see step one */
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* see step two */
                }
                break;

                default:
                {
                    result |= U8_ERROR_VALUE_OUT_OF_RANGE;
                }
                break;
            }
        }

        /* STEP ONE: Delete all objects that can be deleted after relationships and features are gone */

        for ( index = 0; index < data_small_set_get_count( objects ); index ++ )
        {
            data_id_t current_id;
            current_id = data_small_set_get_id( objects, index );
            switch ( data_id_get_table( &current_id ) )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    /* see step two */
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    /* see step zero */
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    /* see step zero */
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    result |= ctrl_diagram_controller_delete_diagramelement ( &((*this_).diagrams),
                                                                              data_id_get_row_id( &current_id ),
                                                                              CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                            );
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    /* see step two */
                }
                break;

                default:
                {
                    /* see step zero */
                }
                break;
            }
        }

        /* STEP TWO: Delete all objects that can be deleted after step one */

        for ( index = 0; index < data_small_set_get_count( objects ); index ++ )
        {
            data_id_t current_id;
            current_id = data_small_set_get_id( objects, index );
            switch ( data_id_get_table( &current_id ) )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    result |= ctrl_classifier_controller_delete_classifier( &((*this_).classifiers),
                                                                            data_id_get_row_id( &current_id ),
                                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                          );
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    /* see step zero */
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    /* see step zero */
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    /* see step one */
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    result |= ctrl_diagram_controller_delete_diagram ( &((*this_).diagrams),
                                                                       data_id_get_row_id( &current_id ),
                                                                       CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                     );
                }
                break;

                default:
                {
                    /* see step zero */
                }
                break;
            }
        }

        /* update statistics based on undo redo list (which provides reliable information) */
        result |= ctrl_undo_redo_list_get_last_statistics ( &((*this_).undo_redo_list), io_stat );
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2021 Andreas Warnke

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
