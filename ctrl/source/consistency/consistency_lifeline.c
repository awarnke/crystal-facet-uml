/* File: consistency_lifeline.c; Copyright and License: see below */

#include "consistency/consistency_lifeline.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

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

    U8_TRACE_END();
}

void consistency_lifeline_destroy( consistency_lifeline_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    (*this_).diag_ctrl = NULL;

    U8_TRACE_END();
}

u8_error_t consistency_lifeline_create_lifelines( consistency_lifeline_t *this_,
                                                  const data_diagram_t *updated_diagram )
{
    U8_TRACE_BEGIN();
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
                                                                              CONSISTENCY_LIFELINE_CONST_MAX_TEMP_DIAGELES,
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
                    result |= consistency_lifeline_private_create_one_lifeline ( this_, current_diagele );
                }
            }
        }
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
            result |= consistency_lifeline_private_create_one_lifeline ( this_, new_diagramelement );
        }
    }
    else
    {
        result |= (u8_error_t) data_result;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_lifeline_private_create_one_lifeline( consistency_lifeline_t *this_,
                                                             const data_diagramelement_t *the_diagramelement )
{
    U8_TRACE_BEGIN();
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
    data_row_id_t focused_feature_id;
    focused_feature_id = data_diagramelement_get_focused_feature_row_id( deleted_diagramelement );
    if ( DATA_ROW_ID_VOID != focused_feature_id )
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
    u8_error_t data_result;

    if ( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type ( deleted_feature ) )
    {
        data_row_id_t classifier_id;
        classifier_id = data_feature_get_classifier_row_id ( deleted_feature );
        data_row_id_t deleted_feature_id;
        deleted_feature_id = data_feature_get_row_id( deleted_feature );

        /* search all diagramelements of the classifier */
        uint32_t diagramelement_count;
        data_result = data_database_reader_get_diagramelements_by_classifier_id( (*this_).db_reader,
                                                                                 classifier_id,
                                                                                 CONSISTENCY_LIFELINE_CONST_MAX_TEMP_DIAGELES,
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

                if ( focused_feature == deleted_feature_id )
                {
                    data_row_id_t diagele_id;
                    diagele_id = data_diagramelement_get_row_id( current_diagele );

                    result |= ctrl_diagram_controller_update_diagramelement_focused_feature_id( (*this_).diag_ctrl,
                                                                                                diagele_id,
                                                                                                DATA_ROW_ID_VOID,
                                                                                                CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                                              );
                }
            }
        }
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
