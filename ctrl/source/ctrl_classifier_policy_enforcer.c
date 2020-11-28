/* File: ctrl_classifier_policy_enforcer.c; Copyright and License: see below */

#include "ctrl_classifier_policy_enforcer.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "trace.h"
#include "tslog.h"

void ctrl_classifier_policy_enforcer_init ( ctrl_classifier_policy_enforcer_t *this_,
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

void ctrl_classifier_policy_enforcer_destroy ( ctrl_classifier_policy_enforcer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    (*this_).diag_ctrl = NULL;

    TRACE_END();
}

/* ================================ LIFELINES ================================ */

ctrl_error_t ctrl_classifier_policy_enforcer_private_unlink_lifeline ( ctrl_classifier_policy_enforcer_t *this_,
                                                                       const data_feature_t *deleted_feature )
{
    TRACE_BEGIN();
    assert( NULL != deleted_feature );
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    if ( DATA_FEATURE_TYPE_LIFELINE == data_feature_get_main_type ( deleted_feature ) )
    {
        data_row_id_t classifier_id;
        classifier_id = data_feature_get_classifier_row_id ( deleted_feature );
        data_row_id_t deleted_feature_id;
        deleted_feature_id = data_feature_get_row_id( deleted_feature );

        /* search all diagramelements of the classifier */
        uint32_t diagramelement_count;
        data_result = data_database_reader_get_diagramelements_by_classifier_id ( (*this_).db_reader,
                                                                                  classifier_id,
                                                                                  CTRL_CLASSIFIER_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES,
                                                                                  &((*this_).private_temp_diagele_buf),
                                                                                  &diagramelement_count
                                                                                );
        result |= (ctrl_error_t) data_result;

        if ( DATA_ERROR_NONE == data_result )
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

                    result |= ctrl_diagram_controller_update_diagramelement_focused_feature_id ( (*this_).diag_ctrl,
                                                                                                 diagele_id,
                                                                                                 DATA_ROW_ID_VOID,
                                                                                                 true /* = add_to_latest_undo_set */
                                                                                               );
                }
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2018-2020 Andreas Warnke

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
