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

    data_row_t classifier_id = data_diagramelement_get_classifier_row_id( deleted_diagramelement );
    data_small_set_t relations_to_delete;
    data_small_set_init( &relations_to_delete );

    /* load relationships to be checked */
    data_relationship_iterator_t relationship_iterator;
    result |= data_relationship_iterator_init_empty( &relationship_iterator );
    result |= data_database_reader_get_relationships_by_classifier_id( (*this_).db_reader,
                                                                       classifier_id,
                                                                       &relationship_iterator
                                                                     );
    if ( result == U8_ERROR_NONE )
    {
        while( data_relationship_iterator_has_next( &relationship_iterator ) )
        {
            result |= data_relationship_iterator_next( &relationship_iterator, &((*this_).temp_relationship_buf) );

            bool visible = true;
            const u8_error_t vis_err
                = consistency_drop_invisibles_private_has_relationship_a_diagram( this_, &((*this_).temp_relationship_buf), &visible );

            if ( vis_err == U8_ERROR_NONE )
            {
                if ( ! visible )
                {
                    /* invisible relationship found */
                    /* this must be copied into a local data set to make this class re-entrant for recursive calls */
                    const data_id_t relation_to_delete = data_relationship_get_data_id( &((*this_).temp_relationship_buf) );
                    result |= data_small_set_add_obj( &relations_to_delete, relation_to_delete );
                }
            }
            else
            {
                result |= vis_err;
            }
        }
    }
    else
    {
        U8_LOG_WARNING( "Relationships of the deleted classifier cannot be checked for being superfluous now." );
        U8_TRACE_INFO_INT( "classifier has unckecked relationships:", classifier_id );
    }
    result |= data_relationship_iterator_destroy( &relationship_iterator );
    /* note that relationship_iterator cannot be used here any longer due to re-entrancy by recursion */

    /* delete all found relationship */
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

    const data_row_t from_classifier_id = data_relationship_get_from_classifier_row_id( relation );
    const data_row_t to_classifier_id = data_relationship_get_to_classifier_row_id( relation );

    if ( from_classifier_id == to_classifier_id )
    {
        /* relationship is visible in one diagram because source and destination are identical */
        *out_result = true;
    }
    else
    {
        bool same_diag = false;
        data_small_set_t from_diagrams;
        data_small_set_init( &from_diagrams );

        /* load diagramelements that are referenced by the relationships from end */
        {
            data_diagramelement_iterator_t diagramelement_iterator;
            result |= data_diagramelement_iterator_init_empty( &diagramelement_iterator );
            result |= data_database_reader_get_diagramelements_by_classifier_id( (*this_).db_reader,
                                                                                 from_classifier_id,
                                                                                 &diagramelement_iterator
                                                                               );
            if ( result == U8_ERROR_NONE )
            {
                while ( data_diagramelement_iterator_has_next( &diagramelement_iterator ) )
                {
                    result |= data_diagramelement_iterator_next( &diagramelement_iterator, &((*this_).temp_diagelement_buf) );
                    const data_id_t from_diag_id
                        = data_diagramelement_get_diagram_data_id ( &((*this_).temp_diagelement_buf) );
                    const u8_error_t ins_err
                        = data_small_set_add_obj( &from_diagrams, from_diag_id );
                    if ( ins_err == U8_ERROR_DUPLICATE_ID )
                    {
                        /* not an error */
                    }
                    else
                    {
                        result |= ins_err;
                    }
                }
            }
            result |= data_diagramelement_iterator_destroy( &diagramelement_iterator );
        }

        /* load diagramelements that are referenced by the relationships to end */
        data_diagramelement_iterator_t diagramelement_iterator2;
        result |= data_diagramelement_iterator_init_empty( &diagramelement_iterator2 );
        result |= data_database_reader_get_diagramelements_by_classifier_id( (*this_).db_reader,
                                                                             to_classifier_id,
                                                                             &diagramelement_iterator2
                                                                           );

        /* check for same diagram */
        if ( result == U8_ERROR_NONE )
        {
            while ( data_diagramelement_iterator_has_next( &diagramelement_iterator2 ) )
            {
                result |= data_diagramelement_iterator_next( &diagramelement_iterator2, &((*this_).temp_diagelement_buf) );
                const data_id_t to_diag_id
                    = data_diagramelement_get_diagram_data_id ( &((*this_).temp_diagelement_buf) );
                if ( data_small_set_contains( &from_diagrams, to_diag_id ) )
                {
                    same_diag = true;
                }
            }
        }
        result |= data_diagramelement_iterator_destroy( &diagramelement_iterator2 );
        data_small_set_destroy( &from_diagrams );

        *out_result = same_diag;
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2018-2024 Andreas Warnke

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
