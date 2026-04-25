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
                                                             const data_diagram_t *updated_diagram,
                                                             consistency_stat_t *io_stat )
{
    U8_TRACE_BEGIN();
    assert( updated_diagram != NULL );
    assert( io_stat != NULL );
    u8_error_t result = U8_ERROR_NONE;

    const data_diagram_type_t new_diagram_type = data_diagram_get_diagram_type( updated_diagram );
    const bool new_diagram_shows_feature = data_rules_diagram_shows_uncond_features( &((*this_).rules), new_diagram_type );
    /*
    const bool new_diagram_is_scenario = data_rules_diagram_is_scenario( &((*this_).rules), new_diagram_type );
    const bool new_diagram_shows_lifeline = data_rules_diagram_shows_scenario_features( &((*this_).rules), new_diagram_type );
    assert( new_diagram_is_scenario == new_diagram_shows_lifeline );
    */

    if ( ! new_diagram_shows_feature )
    {
        const data_row_t diagram_id = data_diagram_get_row_id( updated_diagram );
        data_diagramelement_iterator_t diagramelement_iterator;
        data_diagramelement_iterator_init_empty( &diagramelement_iterator );
        result |= data_database_reader_get_diagramelements_by_diagram_id( (*this_).db_reader,
                                                                          diagram_id,
                                                                          &diagramelement_iterator
                                                                        );
        if ( result == U8_ERROR_NONE )
        {
            while ( data_diagramelement_iterator_has_next( &diagramelement_iterator ) )
            {
                result |= data_diagramelement_iterator_next( &diagramelement_iterator, &((*this_).temp_diagramelement_buf) );
                const data_row_t classifier_id = data_diagramelement_get_classifier_row_id( &((*this_).temp_diagramelement_buf) );
                result |= consistency_feature_delete_invisibles_of_classifier( this_, classifier_id, io_stat );
            }
        }
        result |= data_diagramelement_iterator_destroy( &diagramelement_iterator );
    }
    else
    {
        U8_TRACE_INFO("All unconditional features are visible, no need to check for invisibility.");
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_feature_delete_invisibles_of_classifier( consistency_feature_t *this_,
                                                                data_row_t classifier_id,
                                                                consistency_stat_t *io_stat )
{
    U8_TRACE_BEGIN();
    assert( classifier_id != DATA_ROW_VOID );
    assert( io_stat != NULL );
    u8_error_t result = U8_ERROR_NONE;

    bool a_diagram_shows_uncond_feature = false;  /* tells if a diagram is found where unconditional features of a classifier are visible */
    {
        data_diagram_iterator_t diagram_iterator;
        data_diagram_iterator_init_empty( &diagram_iterator );
        result |= data_database_reader_get_diagrams_by_classifier_id( (*this_).db_reader,
                                                                      classifier_id,
                                                                      &diagram_iterator
                                                                    );
        if ( result == U8_ERROR_NONE )
        {
            while ( data_diagram_iterator_has_next( &diagram_iterator ) )
            {
                result |= data_diagram_iterator_next( &diagram_iterator, &((*this_).temp_diagram_buf) );
                const data_diagram_type_t diagram_type = data_diagram_get_diagram_type( &((*this_).temp_diagram_buf) );
                a_diagram_shows_uncond_feature |= data_rules_diagram_shows_uncond_features( &((*this_).rules), diagram_type );
                /* this function does not care on visibility of scenario based features, this is handled in consistency_lifeline */
            }
        }
        result |= data_diagram_iterator_destroy( &diagram_iterator );
    }

    if ( ! a_diagram_shows_uncond_feature )
    {
        data_feature_iterator_t feature_iterator;
        data_feature_iterator_init_empty( &feature_iterator );
        result |= data_database_reader_get_features_by_classifier_id( (*this_).db_reader,
                                                                      classifier_id,
                                                                      &feature_iterator
                                                                    );
        if ( result == U8_ERROR_NONE )
        {
            while ( data_feature_iterator_has_next( &feature_iterator ) )
            {
                result |= data_feature_iterator_next( &feature_iterator, &((*this_).temp_feature_buf) );
                const data_feature_type_t feature_type = data_feature_get_main_type( &((*this_).temp_feature_buf) );
                const bool feature_is_scenario = data_rules_feature_is_scenario_cond( &((*this_).rules), feature_type );
                if ( ! feature_is_scenario )
                {
                    const data_row_t feature_id = data_feature_get_row_id( &((*this_).temp_feature_buf) );
                    result |= ctrl_classifier_controller_delete_feature( (*this_).clfy_ctrl,
                                                                         feature_id,
                                                                         CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND,
                                                                         io_stat
                                                                       );
                }
            }
        }
        result |= data_feature_iterator_destroy( &feature_iterator );
    }
    else
    {
        U8_TRACE_INFO("Unconditional features are visible, no deletion of unconditional features.");
    }

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
