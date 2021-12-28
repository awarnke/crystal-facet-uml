/* File: ctrl_diagram_policy_enforcer.c; Copyright and License: see below */

#include "ctrl_diagram_policy_enforcer.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "trace.h"
#include "tslog.h"

void ctrl_diagram_policy_enforcer_init( ctrl_diagram_policy_enforcer_t *this_,
                                        data_database_reader_t *db_reader,
                                        struct ctrl_classifier_controller_struct *clfy_ctrl,
                                        struct ctrl_diagram_controller_struct *diag_ctrl )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != clfy_ctrl );
    assert( NULL != diag_ctrl );

    (*this_).db_reader = db_reader;
    (*this_).clfy_ctrl = clfy_ctrl;
    (*this_).diag_ctrl = diag_ctrl;

    TRACE_END();
}

void ctrl_diagram_policy_enforcer_destroy( ctrl_diagram_policy_enforcer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    (*this_).diag_ctrl = NULL;

    TRACE_END();
}

/* ================================ LIFELINES ================================ */

u8_error_t ctrl_diagram_policy_enforcer_private_create_lifelines( ctrl_diagram_policy_enforcer_t *this_,
                                                                  const data_diagram_t *updated_diagram )
{
    TRACE_BEGIN();
    assert( NULL != updated_diagram );
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;

    data_diagram_type_t new_type;
    new_type = data_diagram_get_diagram_type ( updated_diagram );
    if (( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM == new_type )
        || ( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM == new_type )
        || ( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == new_type )
        || ( DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM == new_type ))
    {
        /* this diagram type needs lifelines */

        data_row_id_t diagram_id;
        diagram_id = data_diagram_get_row_id ( updated_diagram );

        /* search all contained diagramelements */
        uint32_t diagramelement_count;
        data_result = data_database_reader_get_diagramelements_by_diagram_id( (*this_).db_reader,
                                                                              diagram_id,
                                                                              CTRL_DIAGRAM_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES,
                                                                              &((*this_).private_temp_diagele_buf),
                                                                              &diagramelement_count
                                                                            );
        result |= (u8_error_t) data_result;

        if ( U8_ERROR_NONE == data_result )
        {
            /* search the diagramelements */
            for ( uint32_t index = 0; index < diagramelement_count; index ++ )
            {
                data_diagramelement_t *current_diagele;
                current_diagele = &((*this_).private_temp_diagele_buf[index]);
                data_row_id_t focused_feature;
                focused_feature = data_diagramelement_get_focused_feature_row_id( current_diagele );

                if ( DATA_ROW_ID_VOID == focused_feature )
                {
                    /* diagramelement without focused feature found */
                    result |= ctrl_diagram_policy_enforcer_private_create_one_lifeline ( this_, current_diagele );
                }
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_policy_enforcer_private_create_a_lifeline( ctrl_diagram_policy_enforcer_t *this_,
                                                                   const data_diagramelement_t *new_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != new_diagramelement );
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;

    /* load the diagram and check the type */
    data_diagram_t the_diag;
    data_result = data_database_reader_get_diagram_by_id( (*this_).db_reader,
                                                          data_diagramelement_get_diagram_row_id( new_diagramelement ),
                                                          &the_diag
                                                        );

    if ( U8_ERROR_NONE == data_result )
    {
        data_diagram_type_t dig_type = data_diagram_get_diagram_type( &the_diag );
        if (( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM == dig_type )
            || ( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM == dig_type )
            || ( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == dig_type )
            || ( DATA_DIAGRAM_TYPE_INTERACTION_OVERVIEW_DIAGRAM == dig_type ))
        {
            result |= ctrl_diagram_policy_enforcer_private_create_one_lifeline ( this_, new_diagramelement );
        }
    }
    else
    {
        result |= (u8_error_t) data_result;
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_policy_enforcer_private_create_one_lifeline( ctrl_diagram_policy_enforcer_t *this_,
                                                                     const data_diagramelement_t *the_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != the_diagramelement );
    u8_error_t result = U8_ERROR_NONE;
    u8_error_t data_result;

    /* define the lifeline to create */
    data_feature_t new_lifeline;
    data_result = data_feature_init_new( &new_lifeline,
                                         DATA_FEATURE_TYPE_LIFELINE,
                                         data_diagramelement_get_classifier_row_id( the_diagramelement ),
                                         "",  /* key */
                                         "",  /* value or type */
                                         "",  /* description */
                                         0  /* list_order */
                                       );
    result |= (u8_error_t) data_result;

    /* create the lifeline */
    data_row_id_t new_feature_id;
    result |= ctrl_classifier_controller_create_feature( (*this_).clfy_ctrl,
                                                         &new_lifeline,
                                                         CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                         &new_feature_id
                                                       );

    /* the newly create lifeline is the focused feature */
    data_row_id_t diagramelement_id;
    diagramelement_id = data_diagramelement_get_row_id( the_diagramelement );
    result |= ctrl_diagram_controller_update_diagramelement_focused_feature_id( (*this_).diag_ctrl,
                                                                                diagramelement_id,
                                                                                new_feature_id,
                                                                                CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                              );

    /* cleanup */
    data_feature_destroy ( &new_lifeline );

    TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_policy_enforcer_private_delete_a_lifeline( ctrl_diagram_policy_enforcer_t *this_,
                                                                   const data_diagramelement_t *deleted_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != deleted_diagramelement );
    u8_error_t result = U8_ERROR_NONE;

    /* delete the lifeline of the already deleted data_diagramelement_t */
    data_row_id_t focused_feature_id;
    focused_feature_id = data_diagramelement_get_focused_feature_row_id( deleted_diagramelement );
    if ( DATA_ROW_ID_VOID != focused_feature_id )
    {
        result |= ctrl_classifier_controller_delete_feature( (*this_).clfy_ctrl,
                                                             focused_feature_id,
                                                             CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                           );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ NO ABANDONED CLASSIFIERS ================================ */

u8_error_t ctrl_diagram_policy_enforcer_private_delete_unreferenced_classifier( ctrl_diagram_policy_enforcer_t *this_,
                                                                                const data_diagramelement_t *deleted_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != deleted_diagramelement );
    u8_error_t result = U8_ERROR_NONE;

    /* try to also delete the classifier, ignore errors because it is ok if the classifier is still referenced */
    u8_error_t my_ctrl_result;

    my_ctrl_result = ctrl_classifier_controller_delete_classifier( (*this_).clfy_ctrl,
                                                                   data_diagramelement_get_classifier_row_id( deleted_diagramelement ),
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                 );

    if ( U8_ERROR_NONE != ( my_ctrl_result & U8_ERROR_OBJECT_STILL_REFERENCED ))
    {
        TSLOG_ANOMALY( "The classifier cannot be deleted because it is still referenced." );
    }
    else
    {
        /* report this unexpected error */
        result |= my_ctrl_result;
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ NO INVISIBLE RELATIONSHIPS ================================ */

u8_error_t ctrl_diagram_policy_enforcer_private_delete_invisible_relationships( ctrl_diagram_policy_enforcer_t *this_,
                                                                                const data_diagramelement_t *deleted_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != deleted_diagramelement );
    u8_error_t result = U8_ERROR_NONE;

    data_row_id_t classifier_id = data_diagramelement_get_classifier_row_id( deleted_diagramelement );

    /* load relationships to be checked */
    uint32_t relationship_count = 0;
    const u8_error_t d_err
        = data_database_reader_get_relationships_by_classifier_id( (*this_).db_reader,
                                                                   classifier_id,
                                                                   CTRL_DIAGRAM_POLICY_ENFORCER_CONST_MAX_TEMP_RELATIONS,
                                                                   &((*this_).private_temp_rel_buf),
                                                                   &relationship_count
                                                                 );
    if ( d_err == U8_ERROR_ARRAY_BUFFER_EXCEEDED )
    {
        TSLOG_ANOMALY( "The dereferenced classifier has more relationships than can be checked for being superfluous now." );
        TRACE_INFO_INT( "classifier has too many relationships:", classifier_id );
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
                = ctrl_diagram_policy_enforcer_private_has_relationship_a_diagram( this_, relation, &visible );

            if ( vis_err == U8_ERROR_ARRAY_BUFFER_EXCEEDED )
            {
                TSLOG_ANOMALY( "A relationship is connected to a classifier that is too often referenced to check for being superfluous now." );
                TRACE_INFO_INT( "classifier or related classifier has too many diagramelements:", classifier_id );
                /* no further error propagation. */
            }
            else if ( vis_err == U8_ERROR_NONE )
            {
                if ( ! visible )
                {
                    const data_row_id_t relation_id = data_relationship_get_row_id( relation );
                    result |= ctrl_classifier_controller_delete_relationship( (*this_).clfy_ctrl,
                                                                              relation_id,
                                                                              CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                            );
                }
            }
            else
            {
                result |= vis_err;
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t ctrl_diagram_policy_enforcer_private_has_relationship_a_diagram( ctrl_diagram_policy_enforcer_t *this_,
                                                                            const data_relationship_t *relation,
                                                                            bool *out_result )
{
    TRACE_BEGIN();
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
                                                                             CTRL_DIAGRAM_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES,
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
                                                                         CTRL_DIAGRAM_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES,
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

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2018-2021 Andreas Warnke

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
