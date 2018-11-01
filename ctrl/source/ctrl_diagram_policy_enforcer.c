/* File: ctrl_diagram_policy_enforcer.c; Copyright and License: see below */

#include "ctrl_diagram_policy_enforcer.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "trace.h"
#include "tslog.h"

void ctrl_diagram_policy_enforcer_init ( ctrl_diagram_policy_enforcer_t *this_,
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

void ctrl_diagram_policy_enforcer_destroy ( ctrl_diagram_policy_enforcer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    (*this_).diag_ctrl = NULL;

    TRACE_END();
}

/* ================================ LIFELINES ================================ */

ctrl_error_t ctrl_diagram_policy_enforcer_private_create_lifelines ( ctrl_diagram_policy_enforcer_t *this_,
                                                                     const data_diagram_t *updated_diagram )
{
    TRACE_BEGIN();
    assert( NULL != updated_diagram );
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    data_diagram_type_t new_type;
    new_type = data_diagram_get_diagram_type ( updated_diagram );
    if (( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM == new_type )
        || ( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM == new_type )
        || ( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == new_type ))
    {
        /* this diagram type needs lifelines */

        int64_t diagram_id;
        diagram_id = data_diagram_get_id ( updated_diagram );

        /* search all contained diagramelements */
        uint32_t diagramelement_count;
        data_result = data_database_reader_get_diagramelements_by_diagram_id ( (*this_).db_reader,
                                                                               diagram_id,
                                                                               CTRL_DIAGRAM_POLICY_ENFORCER_CONST_MAX_TEMP_DIAGELES,
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
                int64_t focused_feature;
                focused_feature = data_diagramelement_get_focused_feature_id( current_diagele );

                if ( DATA_ID_VOID_ID == focused_feature )
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

ctrl_error_t ctrl_diagram_policy_enforcer_private_create_a_lifeline ( ctrl_diagram_policy_enforcer_t *this_,
                                                                      const data_diagramelement_t *new_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != new_diagramelement );
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    /* load the diagram and check the type */
    data_diagram_t the_diag;
    data_result = data_database_reader_get_diagram_by_id ( (*this_).db_reader,
                                                           data_diagramelement_get_diagram_id( new_diagramelement ),
                                                           &the_diag
                                                         );

    if ( DATA_ERROR_NONE == data_result )
    {
        data_diagram_type_t dig_type = data_diagram_get_diagram_type( &the_diag );
        if (( DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM == dig_type )
            || ( DATA_DIAGRAM_TYPE_UML_COMMUNICATION_DIAGRAM == dig_type )
            || ( DATA_DIAGRAM_TYPE_UML_TIMING_DIAGRAM == dig_type ))
        {
            result |= ctrl_diagram_policy_enforcer_private_create_one_lifeline ( this_, new_diagramelement );
        }
    }
    else
    {
        result |= (ctrl_error_t) data_result;
    }

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_policy_enforcer_private_create_one_lifeline ( ctrl_diagram_policy_enforcer_t *this_,
                                                                        const data_diagramelement_t *the_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != the_diagramelement );
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    /* define the lifeline to create */
    data_feature_t new_lifeline;
    data_result = data_feature_init ( &new_lifeline,
                                      DATA_ID_VOID_ID,
                                      DATA_FEATURE_TYPE_LIFELINE,
                                      data_diagramelement_get_classifier_id( the_diagramelement ),
                                      "",  /* key */
                                      "",  /* value or type */
                                      "",  /* description */
                                      0  /* list_order */
                                    );
    result |= (ctrl_error_t) data_result;

    /* create the lifeline */
    int64_t new_feature_id;
    result |= ctrl_classifier_controller_create_feature ( (*this_).clfy_ctrl,
                                                          &new_lifeline,
                                                          true, /* add_to_latest_undo_set */
                                                          &new_feature_id
                                                        );

    /* the newly create lifeline is the focused feature */
    int64_t diagramelement_id;
    diagramelement_id = data_diagramelement_get_id( the_diagramelement );
    result |= ctrl_diagram_controller_update_diagramelement_focused_feature_id ( (*this_).diag_ctrl,
                                                                                 diagramelement_id,
                                                                                 new_feature_id,
                                                                                 true /* add_to_latest_undo_set */
                                                                               );

    /* cleanup */
    data_feature_destroy ( &new_lifeline );

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_policy_enforcer_private_delete_a_lifeline ( ctrl_diagram_policy_enforcer_t *this_,
                                                                      const data_diagramelement_t *deleted_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != deleted_diagramelement );
    ctrl_error_t result = CTRL_ERROR_NONE;

    /* delete the lifeline of the already deleted data_diagramelement_t */
    int64_t focused_feature_id;
    focused_feature_id = data_diagramelement_get_focused_feature_id( deleted_diagramelement );
    if ( DATA_ID_VOID_ID != focused_feature_id )
    {
        result |= ctrl_classifier_controller_delete_feature ( (*this_).clfy_ctrl,
                                                              focused_feature_id,
                                                              true /* add_to_latest_undo_set */
                                                            );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ NO ABANDONED CLASSIFIERS ================================ */

ctrl_error_t ctrl_diagram_policy_enforcer_private_delete_unreferenced_classifier ( ctrl_diagram_policy_enforcer_t *this_,
                                                                                   const data_diagramelement_t *deleted_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != deleted_diagramelement );
    ctrl_error_t result = CTRL_ERROR_NONE;

    /* try to also delete the classifier, ignore errors because it is ok if the classifier is still referenced */
    ctrl_error_t my_ctrl_result;

    my_ctrl_result = ctrl_classifier_controller_delete_classifier( (*this_).clfy_ctrl,
                                                                   data_diagramelement_get_classifier_id( deleted_diagramelement ),
                                                                   true /* = add_to_latest_undo_set */
                                                                 );

    if ( 0 != ( CTRL_ERROR_MASK & CTRL_ERROR_OBJECT_STILL_REFERENCED & my_ctrl_result ))
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


/*
Copyright 2018-2018 Andreas Warnke

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
