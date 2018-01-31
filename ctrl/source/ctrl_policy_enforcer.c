/* File: ctrl_policy_enforcer.c; Copyright and License: see below */

#include "ctrl_policy_enforcer.h"
#include "trace.h"
#include "tslog.h"

void ctrl_policy_enforcer_init ( ctrl_policy_enforcer_t *this_,
                                 data_database_reader_t *db_reader,
                                 ctrl_classifier_controller_t *clfy_ctrl )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != clfy_ctrl );

    (*this_).db_reader = db_reader;
    (*this_).clfy_ctrl = clfy_ctrl;

    TRACE_END();
}

void ctrl_policy_enforcer_destroy ( ctrl_policy_enforcer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;

    TRACE_END();
}

/* ================================ LIFELINES ================================ */

ctrl_error_t ctrl_policy_enforcer_post_update_diagram_type ( ctrl_policy_enforcer_t *this_,
                                                             const data_diagram_t *new_diagram,
                                                             bool add_to_latest_undo_set,
                                                             int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram );
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_policy_enforcer_post_create_diagramelement ( ctrl_policy_enforcer_t *this_,
                                                               const data_diagram_t *new_diagram,
                                                               bool add_to_latest_undo_set,
                                                               int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram );
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_policy_enforcer_pre_delete_diagramelement ( ctrl_policy_enforcer_t *this_,
                                                              const data_diagramelement_t *old_diagramelement,
                                                              bool add_to_latest_undo_set,
                                                              int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != old_diagramelement );
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    TRACE_END_ERR( result );
    return result;
}

/* ================================ NO ABANDONED CLASSIFIERS ================================ */

ctrl_error_t ctrl_policy_enforcer_post_delete_diagramelement ( ctrl_policy_enforcer_t *this_,
                                                               const data_diagramelement_t *deleted_diagramelement
                                                             )
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
