/* File: consistency_classifier.c; Copyright and License: see below */

#include "consistency/consistency_classifier.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void consistency_classifier_init( consistency_classifier_t *this_,
                                  data_database_reader_t *db_reader,
                                  struct ctrl_classifier_controller_struct *clfy_ctrl )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != clfy_ctrl );

    (*this_).db_reader = db_reader;
    (*this_).clfy_ctrl = clfy_ctrl;

    U8_TRACE_END();
}

void consistency_classifier_destroy( consistency_classifier_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;

    U8_TRACE_END();
}

u8_error_t consistency_classifier_delete_unreferenced_classifier( consistency_classifier_t *this_,
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


/*
Copyright 2018-2026 Andreas Warnke

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
