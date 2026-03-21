/* File: consistency_feature.c; Copyright and License: see below */

#include "consistency/consistency_feature.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void consistency_feature_init( consistency_feature_t *this_,
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

void consistency_feature_destroy( consistency_feature_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    (*this_).diag_ctrl = NULL;

    U8_TRACE_END();
}


/*
Copyright 2026-2026 Andreas Warnke

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
