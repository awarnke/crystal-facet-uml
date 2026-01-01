/* File: ctrl_multi_step_changer.c; Copyright and License: see below */

#include "ctrl_multi_step_changer.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

void ctrl_multi_step_changer_init ( ctrl_multi_step_changer_t *this_,
                                    ctrl_controller_t *controller,
                                    data_database_reader_t *db_reader )
{
    U8_TRACE_BEGIN();
    assert( NULL != controller );
    assert( NULL != db_reader );

    /* init member attributes */
    (*this_).controller = controller;
    (*this_).db_reader = db_reader;

    (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW;

    U8_TRACE_END();
}

void ctrl_multi_step_changer_destroy ( ctrl_multi_step_changer_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( NULL != (*this_).controller );
    assert( NULL != (*this_).db_reader );

    /* destroy member attributes */
    (*this_).controller = NULL;
    (*this_).db_reader = NULL;

    U8_TRACE_END();
}

/* ================================ delete sets of elements ================================ */

u8_error_t ctrl_multi_step_changer_delete_set ( ctrl_multi_step_changer_t *this_,
                                                const data_small_set_t *objects,
                                                data_stat_t *io_stat )
{
    U8_TRACE_BEGIN();
    assert ( NULL != io_stat );
    u8_error_t result = U8_ERROR_NONE;

    if ( data_small_set_is_empty( objects ) )
    {
        result = U8_ERROR_INPUT_EMPTY;
    }
    else
    {
        int index;

        ctrl_classifier_controller_t *const classifier_ctrl = ctrl_controller_get_classifier_control_ptr( (*this_).controller);
        ctrl_diagram_controller_t *const diagram_ctrl = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

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
                                                                          (*this_).is_first_step
                                                                        );
                    if ( result == U8_ERROR_NONE )
                    {
                        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_ERROR );
                    }
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    result |= ctrl_classifier_controller_delete_relationship ( classifier_ctrl,
                                                                               data_id_get_row_id( &current_id ),
                                                                               (*this_).is_first_step
                                                                             );
                    if ( result == U8_ERROR_NONE )
                    {
                        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_ERROR );
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
                                                                              (*this_).is_first_step
                                                                            );
                    if ( result == U8_ERROR_NONE )
                    {
                        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_ERROR );
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
                                                                            (*this_).is_first_step
                                                                          );
                    if ( result == U8_ERROR_NONE )
                    {
                        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_ERROR );
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
                                                                       (*this_).is_first_step
                                                                     );
                    if ( result == U8_ERROR_NONE )
                    {
                        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    }
                    else
                    {
                        data_stat_inc_count( io_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_ERROR );
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
        ctrl_undo_redo_iterator_t iter;
        ctrl_undo_redo_iterator_init_empty( &iter );
        result |= ctrl_controller_get_undo_iterator( (*this_).controller, &iter );
        ctrl_undo_redo_iterator_collect_statistics( &iter, false /* NOT categorize as undo */, io_stat );
        ctrl_undo_redo_iterator_destroy( &iter );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ create elements without duplicate ids ================================ */

u8_error_t ctrl_multi_step_changer_create_diagram ( ctrl_multi_step_changer_t *this_,
                                                    data_diagram_t *new_diagram,
                                                    u8_error_t* out_info )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_diagram );
    assert( NULL != out_info );
    u8_error_t result = U8_ERROR_NONE;

    /* ensure that a uuid exists */
    if ( 0 == utf8string_get_length( data_diagram_get_uuid_const( new_diagram ) ) )
    {
        data_uuid_t new_uuid;
        data_uuid_init_new( &new_uuid );
        data_diagram_set_uuid( new_diagram, data_uuid_get_string( &new_uuid ) );
        data_uuid_destroy( &new_uuid );
    }

    ctrl_diagram_controller_t *const diagram_ctrl
        = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

    data_row_t new_diagram_id;
    const u8_error_t create_err
        = ctrl_diagram_controller_create_diagram( diagram_ctrl,
                                                  new_diagram,
                                                  (*this_).is_first_step,
                                                  &new_diagram_id
                                                );
    *out_info = create_err;
    if ( create_err == U8_ERROR_NONE )
    {
        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
        data_diagram_set_row_id( new_diagram, new_diagram_id );
    }
    else if ( u8_error_contains( create_err, U8_ERROR_DUPLICATE ) )
    {
        data_diagram_set_row_id( new_diagram, DATA_ROW_VOID );
        const u8_error_t alt_create_err
            = ctrl_diagram_controller_create_diagram( diagram_ctrl,
                                                      new_diagram,
                                                      (*this_).is_first_step,
                                                      &new_diagram_id
                                                    );
        if ( alt_create_err == U8_ERROR_NONE )
        {
            (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
            data_diagram_set_row_id( new_diagram, new_diagram_id );
            *out_info |= U8_ERROR_DUPLICATE_ID;
        }
        else
        {
            result = alt_create_err;
        }
    }
    else
    {
        result = create_err;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_multi_step_changer_create_diagramelement ( ctrl_multi_step_changer_t *this_,
                                                           data_diagramelement_t *new_diagramelement,
                                                           u8_error_t* out_info )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_diagramelement );
    assert( NULL != out_info );
    u8_error_t result = U8_ERROR_NONE;

    /* ensure that a uuid exists */
    if ( 0 == utf8string_get_length( data_diagramelement_get_uuid_const( new_diagramelement ) ) )
    {
        data_uuid_t new_uuid;
        data_uuid_init_new( &new_uuid );
        data_diagramelement_set_uuid( new_diagramelement, data_uuid_get_string( &new_uuid ) );
        data_uuid_destroy( &new_uuid );
    }

    ctrl_diagram_controller_t *const diagram_ctrl
        = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

    data_row_t new_diagramelement_id;
    const u8_error_t create_err
        = ctrl_diagram_controller_create_diagramelement( diagram_ctrl,
                                                         new_diagramelement,
                                                         (*this_).is_first_step,
                                                         &new_diagramelement_id
                                                       );
    *out_info = create_err;
    if ( create_err == U8_ERROR_NONE )
    {
        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
        data_diagramelement_set_row_id( new_diagramelement, new_diagramelement_id );
    }
    else if ( u8_error_contains( create_err, U8_ERROR_DUPLICATE ) )
    {
        data_diagramelement_set_row_id( new_diagramelement, DATA_ROW_VOID );
        const u8_error_t alt_create_err
            = ctrl_diagram_controller_create_diagramelement( diagram_ctrl,
                                                             new_diagramelement,
                                                             (*this_).is_first_step,
                                                             &new_diagramelement_id
                                                           );
        if ( alt_create_err == U8_ERROR_NONE )
        {
            (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
            data_diagramelement_set_row_id( new_diagramelement, new_diagramelement_id );
            *out_info |= U8_ERROR_DUPLICATE_ID;
        }
        else
        {
            result = alt_create_err;
        }
    }
    else
    {
        result = create_err;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_multi_step_changer_create_classifier ( ctrl_multi_step_changer_t *this_,
                                                       data_classifier_t *new_classifier,
                                                       u8_error_t* out_info )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_classifier );
    assert( NULL != out_info );
    u8_error_t result = U8_ERROR_NONE;

    /* ensure that a uuid exists */
    if ( 0 == utf8string_get_length( data_classifier_get_uuid_const( new_classifier ) ) )
    {
        data_uuid_t new_uuid;
        data_uuid_init_new( &new_uuid );
        data_classifier_set_uuid( new_classifier, data_uuid_get_string( &new_uuid ) );
        data_uuid_destroy( &new_uuid );
    }

    ctrl_classifier_controller_t *const classifier_ctrl
        = ctrl_controller_get_classifier_control_ptr( (*this_).controller);

    data_row_t new_classifier_id;
    const u8_error_t create_err
        = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                        new_classifier,
                                                        (*this_).is_first_step,
                                                        &new_classifier_id
                                                      );
    *out_info = create_err;
    if ( create_err == U8_ERROR_NONE )
    {
        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
        data_classifier_set_row_id( new_classifier, new_classifier_id );
    }
    else if ( u8_error_contains( create_err, U8_ERROR_DUPLICATE ) )
    {
        data_classifier_set_row_id( new_classifier, DATA_ROW_VOID );
        const u8_error_t alt_create_err
            = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                            new_classifier,
                                                            (*this_).is_first_step,
                                                            &new_classifier_id
                                                          );
        if ( alt_create_err == U8_ERROR_NONE )
        {
            (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
            data_classifier_set_row_id( new_classifier, new_classifier_id );
            *out_info |= U8_ERROR_DUPLICATE_ID;
        }
        else
        {
            result = alt_create_err;
        }
    }
    else
    {
        result = create_err;
    }

    if ( u8_error_contains( result, U8_ERROR_DUPLICATE ) )
    {
        *out_info |= result;
        result = U8_ERROR_NONE;

        /* find an alternative, unused name */
        char wish_name_buf[DATA_CLASSIFIER_MAX_NAME_SIZE];
        utf8stringbuf_t wish_name = UTF8STRINGBUF( wish_name_buf );
        result |= utf8stringbuf_copy_str( &wish_name, data_classifier_get_name_const( new_classifier ) );  /* error to be reported to caller */
        {
            bool name_ok = false;
            static const uint_fast16_t MAX_SEARCH_STEP = 10000;
            static const uint_fast16_t FIRST_STEP = 2;
            for ( uint_fast16_t search_step = FIRST_STEP; ( search_step < MAX_SEARCH_STEP )&&( ! name_ok )&&( result == U8_ERROR_NONE ); search_step ++ )
            {
                char new_name_buf[DATA_CLASSIFIER_MAX_NAME_SIZE];
                utf8stringbuf_t new_name = UTF8STRINGBUF( new_name_buf );
                const u8_error_t trunc_err
                    = ctrl_multi_step_changer_private_propose_classifier_name( this_,
                                                                               utf8stringbuf_get_string( &wish_name ),
                                                                               search_step,
                                                                               new_name
                                                                             );
                if ( trunc_err != U8_ERROR_NONE )
                {
                    U8_TRACE_INFO_STR("Name truncated at search for alternative:", utf8stringbuf_get_string( &new_name ) );
                }
                data_classifier_set_name( new_classifier, utf8stringbuf_get_string( &new_name ) );
                const u8_error_t retry_err
                    = ctrl_classifier_controller_create_classifier( classifier_ctrl,
                                                                    new_classifier,
                                                                    (*this_).is_first_step,
                                                                    &new_classifier_id
                                                                  );
                if ( u8_error_contains( retry_err, U8_ERROR_DUPLICATE ) )
                {
                    *out_info |= retry_err;
                }
                else if ( retry_err == U8_ERROR_NONE )
                {
                    name_ok = true;  /* name unused */
                    (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
                    data_classifier_set_row_id( new_classifier, new_classifier_id );
                    *out_info |= U8_ERROR_DUPLICATE_NAME;
                }
                else
                {
                    result |= retry_err;
                }
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_multi_step_changer_create_feature ( ctrl_multi_step_changer_t *this_,
                                                    data_feature_t *new_feature,
                                                    u8_error_t* out_info )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_feature );
    assert( NULL != out_info );
    u8_error_t result = U8_ERROR_NONE;

    /* ensure that a uuid exists */
    if ( 0 == utf8string_get_length( data_feature_get_uuid_const( new_feature ) ) )
    {
        data_uuid_t new_uuid;
        data_uuid_init_new( &new_uuid );
        data_feature_set_uuid( new_feature, data_uuid_get_string( &new_uuid ) );
        data_uuid_destroy( &new_uuid );
    }

    ctrl_classifier_controller_t *const classifier_ctrl
        = ctrl_controller_get_classifier_control_ptr( (*this_).controller);

    data_row_t new_feature_id;
    const u8_error_t create_err
        = ctrl_classifier_controller_create_feature( classifier_ctrl,
                                                     new_feature,
                                                     (*this_).is_first_step,
                                                     &new_feature_id
                                                   );
    *out_info = create_err;
    if ( create_err == U8_ERROR_NONE )
    {
        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
        data_feature_set_row_id( new_feature, new_feature_id );
    }
    else if ( u8_error_contains( create_err, U8_ERROR_DUPLICATE) )
    {
        data_feature_set_row_id( new_feature, DATA_ROW_VOID );
        const u8_error_t alt_create_err
            = ctrl_classifier_controller_create_feature( classifier_ctrl,
                                                         new_feature,
                                                         (*this_).is_first_step,
                                                         &new_feature_id
                                                       );
        if ( alt_create_err == U8_ERROR_NONE )
        {
            (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
            data_feature_set_row_id( new_feature, new_feature_id );
            *out_info |= U8_ERROR_DUPLICATE_ID;
        }
        else
        {
            result = alt_create_err;
        }
    }
    else
    {
        result = create_err;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_multi_step_changer_create_relationship ( ctrl_multi_step_changer_t *this_,
                                                         data_relationship_t *new_relationship,
                                                         u8_error_t* out_info )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_relationship );
    assert( NULL != out_info );
    u8_error_t result = U8_ERROR_NONE;

    /* ensure that a uuid exists */
    if ( 0 == utf8string_get_length( data_relationship_get_uuid_const( new_relationship ) ) )
    {
        data_uuid_t new_uuid;
        data_uuid_init_new( &new_uuid );
        data_relationship_set_uuid( new_relationship, data_uuid_get_string( &new_uuid ) );
        data_uuid_destroy( &new_uuid );
    }

    ctrl_classifier_controller_t *const classifier_ctrl
        = ctrl_controller_get_classifier_control_ptr( (*this_).controller);

    data_row_t new_relationship_id;
    const u8_error_t create_err
        = ctrl_classifier_controller_create_relationship( classifier_ctrl,
                                                          new_relationship,
                                                          (*this_).is_first_step,
                                                          &new_relationship_id
                                                        );
    *out_info = create_err;
    if ( create_err == U8_ERROR_NONE )
    {
        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
        data_relationship_set_row_id( new_relationship, new_relationship_id );
    }
    else if ( u8_error_contains( create_err, U8_ERROR_DUPLICATE) )
    {
        data_relationship_set_row_id( new_relationship, DATA_ROW_VOID );
        const u8_error_t alt_create_err
            = ctrl_classifier_controller_create_relationship( classifier_ctrl,
                                                              new_relationship,
                                                              (*this_).is_first_step,
                                                              &new_relationship_id
                                                            );
        if ( alt_create_err == U8_ERROR_NONE )
        {
            (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
            data_relationship_set_row_id( new_relationship, new_relationship_id );
            *out_info |= U8_ERROR_DUPLICATE_ID;
        }
        else
        {
            result = alt_create_err;
        }
    }
    else
    {
        result = create_err;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ update links of existing elements  ================================ */

u8_error_t ctrl_multi_step_changer_update_diagram_parent_id ( ctrl_multi_step_changer_t *this_,
                                                              data_row_t diagram_id,
                                                              data_row_t new_diagram_parent_id )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    ctrl_diagram_controller_t *const diagram_ctrl
        = ctrl_controller_get_diagram_control_ptr( (*this_).controller );

    result = ctrl_diagram_controller_update_diagram_parent_id( diagram_ctrl,
                                                               diagram_id,
                                                               new_diagram_parent_id,
                                                               (*this_).is_first_step
                                                             );
    if ( result == U8_ERROR_NONE )
    {
        (*this_).is_first_step = CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ propose names of classifiers ================================ */

u8_error_t ctrl_multi_step_changer_private_propose_classifier_name ( ctrl_multi_step_changer_t *this_,
                                                                     const char* base_classifier_name,
                                                                     uint_fast16_t iteration,
                                                                     utf8stringbuf_t out_name )
{
    U8_TRACE_BEGIN();
    const size_t RESERVED_FOR_NUMBER = 5;
    assert( NULL != base_classifier_name );
    assert( utf8stringbuf_get_size( &out_name ) > RESERVED_FOR_NUMBER );
    u8_error_t result = U8_ERROR_NONE;

    /* find an alternative, unused name */
    /* copy the base_classifier_name to newname_buf */
    {
        utf8stringbuf_t shortened_new_name
            = utf8stringbuf_new( utf8stringbuf_get_string( &out_name ), utf8stringbuf_get_size( &out_name ) - RESERVED_FOR_NUMBER );
        result |= utf8stringbuf_copy_str( &shortened_new_name, base_classifier_name );
        /* null termination is guaranteed, also this function does not cut an utf8 code point in the middle. */
    }
    /* append a separator and the iteration number */
    result |= utf8stringbuf_append_str( &out_name, "-" );
    result |= utf8stringbuf_append_int( &out_name, iteration );

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2026 Andreas Warnke

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
