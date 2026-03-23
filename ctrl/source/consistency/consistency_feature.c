/* File: consistency_feature.c; Copyright and License: see below */

#include "consistency/consistency_feature.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void consistency_feature_init( consistency_feature_t *this_,
                               data_database_reader_t *db_reader,
                               struct ctrl_classifier_controller_struct *clfy_ctrl )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != clfy_ctrl );

    (*this_).db_reader = db_reader;
    (*this_).clfy_ctrl = clfy_ctrl;
    data_rules_init( &((*this_).rules) );

    U8_TRACE_END();
}

void consistency_feature_destroy( consistency_feature_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    data_rules_destroy( &((*this_).rules) );

    U8_TRACE_END();
}

u8_error_t consistency_feature_delete_invisibles_in_diagram( consistency_feature_t *this_,
                                                             const data_diagram_t *updated_diagram )
{
    U8_TRACE_BEGIN();

    u8_error_t result = U8_ERROR_NOT_YET_IMPLEMENTED;
    U8_LOG_ERROR("U8_ERROR_NOT_YET_IMPLEMENTED");

    result = U8_ERROR_NONE;

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_feature_delete_invisibles_of_classifier( consistency_feature_t *this_,
                                                                const data_diagramelement_t *deleted_diagramelement )

{
    U8_TRACE_BEGIN();

    u8_error_t result = U8_ERROR_NOT_YET_IMPLEMENTED;
    U8_LOG_ERROR("U8_ERROR_NOT_YET_IMPLEMENTED");

    result = U8_ERROR_NONE;

    U8_TRACE_END_ERR( result );
    return result;
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
