/* File: consistency_lifeline.c; Copyright and License: see below */

#include "consistency/consistency_lifeline.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "set/data_full_id_list.h"
#include "set/data_visible_set.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

/*!
 *  \brief constants of consistency_lifeline_t
 */
enum consistency_lifeline_const_enum {
    CONSISTENCY_LIFELINE_CONST_MAX_TEMP_DIAGELES = DATA_VISIBLE_SET_MAX_CLASSIFIERS,  /*!< maximum number of diagramelements in a diagram */
};

void consistency_lifeline_init( consistency_lifeline_t *this_,
                                data_database_reader_t *db_reader,
                                struct ctrl_classifier_controller_struct *clfy_ctrl,
                                struct ctrl_diagram_controller_struct *diag_ctrl )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != clfy_ctrl );
    assert( NULL != diag_ctrl );

    (*this_).db_reader = db_reader;
    (*this_).clfy_ctrl = clfy_ctrl;
    (*this_).diag_ctrl = diag_ctrl;
    data_rules_init( &((*this_).rules) );

    U8_TRACE_END();
}

void consistency_lifeline_destroy( consistency_lifeline_t *this_ )
{
    U8_TRACE_BEGIN();

    data_rules_destroy( &((*this_).rules) );
    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    (*this_).diag_ctrl = NULL;

    U8_TRACE_END();
}

u8_error_t consistency_lifeline_delete_lifelines ( consistency_lifeline_t *this_,
                                                   const data_diagram_t *updated_diagram
                                                 )
{
    U8_TRACE_BEGIN();
    assert( NULL != updated_diagram );
    u8_error_t result = U8_ERROR_NONE;

    const data_diagram_type_t new_type = data_diagram_get_diagram_type ( updated_diagram );
    if ( ! data_rules_diagram_is_scenario( &((*this_).rules), new_type ) )
    {
        /* this diagram type must not have any lifelines */
        data_small_set_t lifelines_to_delete;
        data_small_set_init( &lifelines_to_delete );

        /* search all contained diagramelements */
        const data_row_t diagram_id = data_diagram_get_row_id ( updated_diagram );
        data_diagramelement_iterator_t diagramelement_iterator;
        result |= data_diagramelement_iterator_init_empty( &diagramelement_iterator );
        result |= data_database_reader_get_diagramelements_by_diagram_id( (*this_).db_reader,
                                                                          diagram_id,
                                                                          &diagramelement_iterator
                                                                        );
        if ( U8_ERROR_NONE == result )
        {
            /* search the diagramelements */
            while ( data_diagramelement_iterator_has_next( &diagramelement_iterator ) )
            {
                result |= data_diagramelement_iterator_next( &diagramelement_iterator, &((*this_).temp_diagelement_buf) );
                const data_id_t feat_to_delete
                    = data_diagramelement_get_focused_feature_data_id( &((*this_).temp_diagelement_buf) );
                if ( data_id_is_valid( &feat_to_delete ) )
                {
                    /* diagramelement with a focused feature found */
                    /* this must be copied into a local data set to make this class re-entrant for recursive calls */
                    result |= data_small_set_add_obj( &lifelines_to_delete, feat_to_delete );
                }
            }
        }
        else
        {
            U8_LOG_ANOMALY( "consistency_lifeline_delete_lifelines could not load all diagram_elements of a diagram." );
        }
        result |= data_diagramelement_iterator_destroy( &diagramelement_iterator );

        /* delete all found lifelines */
        /* note that (*this_).temp_diagelement_buf cannot be used here any longer due to re-entrancy by recursion */
        const uint32_t lifelines_count = data_small_set_get_count( &lifelines_to_delete );
        for ( uint32_t index2 = 0; index2 < lifelines_count; index2 ++ )
        {
            const data_id_t delete_feat = data_small_set_get_id( &lifelines_to_delete, index2 );
            assert( data_id_get_table( &delete_feat ) == DATA_TABLE_FEATURE );
            result |= ctrl_classifier_controller_delete_feature( (*this_).clfy_ctrl,
                                                                 data_id_get_row_id( &delete_feat ),
                                                                 CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                               );
            /* the current_diagele is already updated by another (recursive) consistency check. */
        }

        data_small_set_destroy( &lifelines_to_delete );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_lifeline_create_lifelines( consistency_lifeline_t *this_,
                                                  const data_diagram_t *updated_diagram )
{
    U8_TRACE_BEGIN();
    assert( NULL != updated_diagram );
    u8_error_t result = U8_ERROR_NONE;

    const data_diagram_type_t new_type = data_diagram_get_diagram_type ( updated_diagram );
    if ( data_rules_diagram_is_scenario( &((*this_).rules), new_type ) )
    {
        /* this diagram type needs lifelines */
        data_full_id_t lifelines_to_create_buf[ CONSISTENCY_LIFELINE_CONST_MAX_TEMP_DIAGELES ];
        data_full_id_list_t lifelines_to_create;
        data_full_id_list_init( &lifelines_to_create,
                                CONSISTENCY_LIFELINE_CONST_MAX_TEMP_DIAGELES,
                                &lifelines_to_create_buf
                              );

        /* search all contained diagramelements */
        const data_row_t diagram_id = data_diagram_get_row_id ( updated_diagram );
        data_diagramelement_iterator_t diagramelement_iterator;
        result |= data_diagramelement_iterator_init_empty( &diagramelement_iterator );
        result |= data_database_reader_get_diagramelements_by_diagram_id( (*this_).db_reader,
                                                                          diagram_id,
                                                                          &diagramelement_iterator
                                                                        );

        if ( U8_ERROR_NONE == result )
        {
            /* search the diagramelements */
            while ( data_diagramelement_iterator_has_next( &diagramelement_iterator ) )
            {
                result |= data_diagramelement_iterator_next( &diagramelement_iterator,
                                                             &((*this_).temp_diagelement_buf)
                                                           );
                data_diagramelement_t *const current_diagele
                    = &((*this_).temp_diagelement_buf);
                const data_row_t focused_feature
                    = data_diagramelement_get_focused_feature_row_id( current_diagele );

                if ( DATA_ROW_VOID == focused_feature )
                {
                    /* diagramelement without focused feature found */
                    /* this must be copied into a local data set to make this class re-entrant for recursive calls */
                    const data_full_id_t diagramelement_ids = {
                        .primary_id = data_diagramelement_get_data_id( current_diagele ),
                        .secondary_id = data_diagramelement_get_classifier_data_id( current_diagele )
                    };
                    result |= data_full_id_list_add( &lifelines_to_create, &diagramelement_ids );
                }
            }
        }
        else
        {
            U8_LOG_ANOMALY( "consistency_lifeline_create_lifelines could not load all diagram_elements of a diagram." );
        }
        result |= data_diagramelement_iterator_destroy( &diagramelement_iterator );


        /* create all missing lifelines */
        /* note that (*this_).private_temp_diagele_buf cannot be used here any longer due to re-entrancy by recursion */
        const uint32_t lifelines_count = data_full_id_list_get_length( &lifelines_to_create );
        for ( uint32_t index2 = 0; index2 < lifelines_count; index2 ++ )
        {
            const data_full_id_t *const diagramelement_ids = data_full_id_list_get_const( &lifelines_to_create, index2 );
            result |= consistency_lifeline_private_create_one_lifeline ( this_, diagramelement_ids );
        }

        data_full_id_list_destroy( &lifelines_to_create );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_lifeline_create_a_lifeline( consistency_lifeline_t *this_,
                                                   const data_diagramelement_t *new_diagramelement )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_diagramelement );
    u8_error_t result = U8_ERROR_NONE;

    /* load the diagram and check the type */
    data_diagram_t the_diag;
    result |= data_database_reader_get_diagram_by_id( (*this_).db_reader,
                                                      data_diagramelement_get_diagram_row_id( new_diagramelement ),
                                                      &the_diag
                                                    );

    if ( U8_ERROR_NONE == result )
    {
        const data_diagram_type_t dig_type = data_diagram_get_diagram_type( &the_diag );
        if ( data_rules_diagram_is_scenario( &((*this_).rules), dig_type ) )
        {
            const data_full_id_t diagramelement_ids = {
                .primary_id = data_diagramelement_get_data_id( new_diagramelement ),
                .secondary_id = data_diagramelement_get_classifier_data_id( new_diagramelement )
            };
            result |= consistency_lifeline_private_create_one_lifeline ( this_, &diagramelement_ids );
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_lifeline_private_create_one_lifeline( consistency_lifeline_t *this_,
                                                             const data_full_id_t *diagramelement_ids )
{
    U8_TRACE_BEGIN();
    assert( NULL != diagramelement_ids );
    u8_error_t result = U8_ERROR_NONE;
    const data_id_t diagramelement_id = data_full_id_get_primary_id( diagramelement_ids );
    assert( DATA_TABLE_DIAGRAMELEMENT == data_id_get_table( &diagramelement_id ) );
    const data_id_t classifier_id = data_full_id_get_secondary_id( diagramelement_ids );
    assert( DATA_TABLE_CLASSIFIER == data_id_get_table( &classifier_id ) );

    /* define the lifeline to create */
    data_feature_t new_lifeline;
    result |= data_feature_init_new( &new_lifeline,
                                     DATA_FEATURE_TYPE_LIFELINE,
                                     data_id_get_row_id( &classifier_id ),
                                     "",  /* key */
                                     "",  /* value or type */
                                     "",  /* description */
                                     0  /* list_order */
                                   );

    /* create the lifeline */
    data_row_t new_feature_id = DATA_ROW_VOID;
    result |= ctrl_classifier_controller_create_feature( (*this_).clfy_ctrl,
                                                         &new_lifeline,
                                                         CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                         &new_feature_id
                                                       );

    /* the newly created lifeline is the focused feature */
    result |= ctrl_diagram_controller_update_diagramelement_focused_feature_id( (*this_).diag_ctrl,
                                                                                data_id_get_row_id( &diagramelement_id ),
                                                                                new_feature_id,
                                                                                CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                              );

    /* cleanup */
    data_feature_destroy ( &new_lifeline );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_lifeline_delete_a_lifeline( consistency_lifeline_t *this_,
                                                   const data_diagramelement_t *deleted_diagramelement )
{
    U8_TRACE_BEGIN();
    assert( NULL != deleted_diagramelement );
    u8_error_t result = U8_ERROR_NONE;

    /* delete the lifeline of the already deleted data_diagramelement_t */
    const data_row_t focused_feature_id
        = data_diagramelement_get_focused_feature_row_id( deleted_diagramelement );
    if ( DATA_ROW_VOID != focused_feature_id )
    {
        result |= ctrl_classifier_controller_delete_feature( (*this_).clfy_ctrl,
                                                             focused_feature_id,
                                                             CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                           );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_lifeline_unlink_lifeline( consistency_lifeline_t *this_,
                                                 const data_feature_t *deleted_feature )
{
    U8_TRACE_BEGIN();
    assert( NULL != deleted_feature );
    u8_error_t result = U8_ERROR_NONE;

    if ( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type ( deleted_feature ) )
    {
        const data_row_t classifier_id = data_feature_get_classifier_row_id ( deleted_feature );
        const data_row_t deleted_feature_id = data_feature_get_row_id( deleted_feature );
        data_small_set_t diag_ele_to_unlink;
        data_small_set_init( &diag_ele_to_unlink );

        /* search all diagramelements of the classifier */
        data_diagramelement_iterator_t diagramelement_iterator;
        result |= data_diagramelement_iterator_init_empty( &diagramelement_iterator );
        result |= data_database_reader_get_diagramelements_by_classifier_id( (*this_).db_reader,
                                                                             classifier_id,
                                                                             &diagramelement_iterator
                                                                           );

        if ( U8_ERROR_NONE == result )
        {
            /* search the diagramelements */
            while ( data_diagramelement_iterator_has_next( &diagramelement_iterator ) )
            {
                result |= data_diagramelement_iterator_next( &diagramelement_iterator, &((*this_).temp_diagelement_buf) );
                data_diagramelement_t *const current_diagele
                    = &((*this_).temp_diagelement_buf);
                const data_row_t focused_feature
                    = data_diagramelement_get_focused_feature_row_id( current_diagele );

                if ( focused_feature == deleted_feature_id )
                {
                    /* diagramelement with the just deleted focused feature found */
                    /* this must be copied into a local data set to make this class re-entrant for recursive calls */
                    const data_id_t diagele_id = data_diagramelement_get_data_id( current_diagele );
                    result |= data_small_set_add_obj( &diag_ele_to_unlink, diagele_id );
                }
            }
        }
        else
        {
            U8_LOG_ANOMALY( "consistency_lifeline_unlink_lifeline could not load all lifelines of a classifier." );
        }
        result |= data_diagramelement_iterator_destroy( &diagramelement_iterator );

        /* unlink all found diagram elements (there should be exactly one) */
        /* note that (*this_).private_temp_diagele_buf cannot be used here any longer due to re-entrancy by recursion */
        const uint32_t diag_ele_count = data_small_set_get_count( &diag_ele_to_unlink );
        if ( diag_ele_count != 1 )
        {
            U8_LOG_ANOMALY_INT( "Unlinking a just deleted lifeline that had not exactly one occurrence:",
                                diag_ele_count
                              );
        }
        for ( uint32_t index2 = 0; index2 < diag_ele_count; index2 ++ )
        {
            const data_id_t diagele_to_unlink = data_small_set_get_id( &diag_ele_to_unlink, index2 );
            assert( data_id_get_table( &diagele_to_unlink ) == DATA_TABLE_DIAGRAMELEMENT );
            result |= ctrl_diagram_controller_update_diagramelement_focused_feature_id( (*this_).diag_ctrl,
                                                                                        data_id_get_row_id( &diagele_to_unlink ),
                                                                                        DATA_ROW_VOID,
                                                                                        CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                                      );
        }

        data_small_set_destroy( &diag_ele_to_unlink );
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2018-2025 Andreas Warnke

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
