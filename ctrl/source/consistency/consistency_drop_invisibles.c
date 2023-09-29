/* File: consistency_drop_invisibles.c; Copyright and License: see below */

#include "consistency/consistency_drop_invisibles.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void consistency_drop_invisibles_init( consistency_drop_invisibles_t *this_,
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

    U8_TRACE_END();
}

void consistency_drop_invisibles_destroy( consistency_drop_invisibles_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    (*this_).diag_ctrl = NULL;

    U8_TRACE_END();
}

/* ================================ NO ABANDONED CLASSIFIERS ================================ */

u8_error_t consistency_drop_invisibles_delete_unreferenced_classifier( consistency_drop_invisibles_t *this_,
                                                                       const data_diagramelement_t *deleted_diagramelement )
{
    U8_TRACE_BEGIN();
    assert( NULL != deleted_diagramelement );
    u8_error_t result = U8_ERROR_NONE;

    /* try to also delete the classifier, ignore errors because it is ok if the classifier is still referenced */
    u8_error_t my_ctrl_result;

    my_ctrl_result = ctrl_classifier_controller_delete_classifier( (*this_).clfy_ctrl,
                                                                   data_diagramelement_get_classifier_row_id( deleted_diagramelement ),
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                 );

    if ( u8_error_contains( my_ctrl_result, U8_ERROR_OBJECT_STILL_REFERENCED ) )
    {
        U8_LOG_EVENT( "The classifier cannot be deleted because it is still referenced." );
    }
    else
    {
        /* report this unexpected error */
        result |= my_ctrl_result;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ NO INVISIBLE RELATIONSHIPS ================================ */

u8_error_t consistency_drop_invisibles_delete_invisible_relationships( consistency_drop_invisibles_t *this_,
                                                                       const data_diagramelement_t *deleted_diagramelement )
{
    U8_TRACE_BEGIN();
    assert( NULL != deleted_diagramelement );
    u8_error_t result = U8_ERROR_NONE;

    data_row_id_t classifier_id = data_diagramelement_get_classifier_row_id( deleted_diagramelement );
    data_small_set_t relations_to_delete;
    data_small_set_init( &relations_to_delete );

    /* load relationships to be checked */
    uint32_t relationship_count = 0;
    const u8_error_t d_err
        = data_database_reader_get_relationships_by_classifier_id( (*this_).db_reader,
                                                                   classifier_id,
                                                                   CONSISTENCY_DROP_INVISIBLES_CONST_MAX_TEMP_RELATIONS,
                                                                   &((*this_).private_temp_rel_buf),
                                                                   &relationship_count
                                                                 );
    if ( d_err == U8_ERROR_ARRAY_BUFFER_EXCEEDED )
    {
        U8_LOG_ANOMALY( "The dereferenced classifier has more relationships than can be checked for being superfluous now." );
        U8_TRACE_INFO_INT( "classifier has too many relationships:", classifier_id );
        /* no further error propagation. */
    }
    else
    {
        result |= d_err;
    }

    if ( result == U8_ERROR_NONE )
    {
        for ( uint_fast32_t rel_idx = 0; rel_idx < relationship_count; rel_idx ++ )
        {
            const data_relationship_t *relation = &((*this_).private_temp_rel_buf[rel_idx]);

            bool visible = true;
            const u8_error_t vis_err
                = consistency_drop_invisibles_private_has_relationship_a_diagram( this_, relation, &visible );

            if ( vis_err == U8_ERROR_ARRAY_BUFFER_EXCEEDED )
            {
                U8_LOG_ANOMALY( "A relationship is connected to a classifier that is too often referenced to check for being superfluous now." );
                U8_TRACE_INFO_INT( "classifier or related classifier has too many diagramelements:", classifier_id );
                /* no further error propagation. */
            }
            else if ( vis_err == U8_ERROR_NONE )
            {
                if ( ! visible )
                {
                    /* invisible relationship found */
                    /* this must be copied into a local data set to make this class re-entrant for recursive calls */
                    result |= data_small_set_add_obj( &relations_to_delete, data_relationship_get_data_id( relation ) );
                }
            }
            else
            {
                result |= vis_err;
            }
        }
    }

    /* delete all found relationship */
    /* note that (*this_).private_temp_rel_buf cannot be used here any longer due to re-entrancy by recursion */
    const uint32_t relations_count = data_small_set_get_count( &relations_to_delete );
    for ( uint32_t index2 = 0; index2 < relations_count; index2 ++ )
    {
        const data_id_t delete_rel = data_small_set_get_id( &relations_to_delete, index2 );
        assert( data_id_get_table( &delete_rel ) == DATA_TABLE_RELATIONSHIP );
        result |= ctrl_classifier_controller_delete_relationship( (*this_).clfy_ctrl,
                                                                  data_id_get_row_id( &delete_rel ),
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                );
    }

    data_small_set_destroy( &relations_to_delete );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_drop_invisibles_private_has_relationship_a_diagram( consistency_drop_invisibles_t *this_,
                                                                           const data_relationship_t *relation,
                                                                           bool *out_result )
{
    U8_TRACE_BEGIN();
    assert( NULL != relation );
    assert( NULL != out_result );
    u8_error_t result = U8_ERROR_NONE;

    const data_row_id_t from_classifier_id = data_relationship_get_from_classifier_row_id( relation );
    const data_row_id_t to_classifier_id = data_relationship_get_to_classifier_row_id( relation );

    if ( from_classifier_id == to_classifier_id )
    {
        /* relationship is visible in one diagram because source and destination are identical */
        *out_result = true;
    }
    else
    {
        data_small_set_t from_diagrams;
        data_small_set_init( &from_diagrams );
        u8_error_t from_err;

        /* load diagramelements that are referenced by the relationships from end */
        {
            uint32_t from_diagramelement_count = 0;
            from_err
                = data_database_reader_get_diagramelements_by_classifier_id( (*this_).db_reader,
                                                                             from_classifier_id,
                                                                             CONSISTENCY_DROP_INVISIBLES_CONST_MAX_TEMP_DIAGELES,
                                                                             &((*this_).private_temp_diagele_buf),
                                                                             &from_diagramelement_count
                                                                           );
            if ( from_err == U8_ERROR_NONE )
            {
                /* copy diagram ids to id-set */
                for ( uint_fast32_t from_idx = 0; from_idx < from_diagramelement_count; from_idx ++ )
                {
                    const data_id_t from_diag_id
                        = data_diagramelement_get_diagram_data_id ( &((*this_).private_temp_diagele_buf[from_idx]) );
                    const u8_error_t ins_err
                        = data_small_set_add_obj( &from_diagrams, from_diag_id );
                    if ( ins_err == U8_ERROR_DUPLICATE_ID )
                    {
                        /* not an error */
                    }
                    else
                    {
                        from_err |= ins_err;
                    }
                }
            }
        }

        /* load diagramelements that are referenced by the relationships to end */
        uint32_t to_diagramelement_count = 0;
        const u8_error_t to_err
            = data_database_reader_get_diagramelements_by_classifier_id( (*this_).db_reader,
                                                                         to_classifier_id,
                                                                         CONSISTENCY_DROP_INVISIBLES_CONST_MAX_TEMP_DIAGELES,
                                                                         &((*this_).private_temp_diagele_buf),
                                                                         &to_diagramelement_count
                                                                       );

        /* check for same diagram */
        if (( from_err == U8_ERROR_NONE )&&( to_err == U8_ERROR_NONE ))
        {
            bool same_diag = false;
            for ( uint_fast32_t to_idx = 0; to_idx < to_diagramelement_count; to_idx ++ )
            {
                const data_id_t to_diag_id
                    = data_diagramelement_get_diagram_data_id ( &((*this_).private_temp_diagele_buf[to_idx]) );
                if ( data_small_set_contains( &from_diagrams, to_diag_id ) )
                {
                    same_diag = true;
                }
            }
            *out_result = same_diag;
        }
        else
        {
            result |= from_err;
            result |= to_err;
        }

        data_small_set_destroy( &from_diagrams );
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2018-2023 Andreas Warnke

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
