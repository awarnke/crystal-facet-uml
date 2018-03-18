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
