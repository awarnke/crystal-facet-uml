/* File: ctrl_multi_step_changer.c; Copyright and License: see below */

#include "ctrl_multi_step_changer.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void ctrl_multi_step_changer_init ( ctrl_multi_step_changer_t *this_,
                                    ctrl_controller_t *controller,
                                    data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != controller );
    assert( NULL != db_reader );

    /* init member attributes */
    (*this_).controller = controller;
    (*this_).db_reader = db_reader;

    TRACE_END();
}

void ctrl_multi_step_changer_destroy ( ctrl_multi_step_changer_t *this_ )
{
    TRACE_BEGIN();
    assert( NULL != (*this_).controller );
    assert( NULL != (*this_).db_reader );

    /* destroy member attributes */
    (*this_).controller = NULL;
    (*this_).db_reader = NULL;

    TRACE_END();
}

/* ================================ interface for sets of elements ================================ */

u8_error_t ctrl_multi_step_changer_delete_set ( ctrl_multi_step_changer_t *this_,
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

        /* keep track if to add a boundary to the undo redo list before each deletion action */
        ctrl_undo_redo_action_boundary_t append_next = CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW;

        ctrl_classifier_controller_t *classifier_ctrl = ctrl_controller_get_classifier_control_ptr( (*this_).controller);
        ctrl_diagram_controller_t *diagram_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

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
                    result |= ctrl_classifier_controller_delete_feature ( classifier_ctrl,
                                                                          data_id_get_row_id( &current_id ),
                                                                          append_next
                                                                        );
                    if ( result == U8_ERROR_NONE )
                    {
                        append_next = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_TABLE_FEATURE, DATA_STAT_SERIES_ERROR );
                    }
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    result |= ctrl_classifier_controller_delete_relationship ( classifier_ctrl,
                                                                                data_id_get_row_id( &current_id ),
                                                                                append_next
                                                                              );
                    if ( result == U8_ERROR_NONE )
                    {
                        append_next = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR );
                    }
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
                    result |= ctrl_diagram_controller_delete_diagramelement ( diagram_ctrl,
                                                                              data_id_get_row_id( &current_id ),
                                                                              append_next
                                                                            );
                    if ( result == U8_ERROR_NONE )
                    {
                        append_next = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_ERROR );
                    }
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
                    result |= ctrl_classifier_controller_delete_classifier( classifier_ctrl,
                                                                            data_id_get_row_id( &current_id ),
                                                                            append_next
                                                                          );
                    if ( result == U8_ERROR_NONE )
                    {
                        append_next = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
                    }
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
                    result |= ctrl_diagram_controller_delete_diagram ( diagram_ctrl,
                                                                       data_id_get_row_id( &current_id ),
                                                                       append_next
                                                                     );
                    if ( result == U8_ERROR_NONE )
                    {
                        append_next = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_ERROR );
                    }
                }
                break;

                default:
                {
                    /* see step zero */
                }
                break;
            }
        }

        /* update statistics based on undo redo list */
        result |= ctrl_controller_get_statistics( (*this_).controller, io_stat );
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
