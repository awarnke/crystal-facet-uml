/* File: consistency_relationship.c; Copyright and License: see below */

#include "consistency/consistency_relationship.h"
#include "ctrl_classifier_controller.h"
#include "ctrl_diagram_controller.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"

void consistency_relationship_init( consistency_relationship_t *this_,
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

void consistency_relationship_destroy( consistency_relationship_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).clfy_ctrl = NULL;
    data_rules_destroy( &((*this_).rules) );

    U8_TRACE_END();
}

u8_error_t consistency_relationship_delete_invisibles_in_diagram( consistency_relationship_t *this_,
                                                                  const data_diagram_t *updated_diagram )
{
    U8_TRACE_BEGIN();
    assert( updated_diagram != NULL );
    u8_error_t result = U8_ERROR_NONE;

    const data_diagram_type_t new_diagram_type = data_diagram_get_diagram_type( updated_diagram );
    const bool new_diagram_shows_rel = data_rules_diagram_shows_uncond_relationships( &((*this_).rules), new_diagram_type );
    /*
    const bool new_diagram_is_scenario = data_rules_diagram_is_scenario( &((*this_).rules), new_diagram_type );
    const bool new_diagram_shows_msg = data_rules_diagram_shows_scenario_relationships( &((*this_).rules), new_diagram_type );
    assert( new_diagram_is_scenario == new_diagram_shows_msg );
    */

    if ( ! new_diagram_shows_rel )
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
                data_diagramelement_t diagelement_buf;
                result |= data_diagramelement_iterator_next( &diagramelement_iterator, &diagelement_buf );
                const data_row_t classifier_id = data_diagramelement_get_classifier_row_id( &diagelement_buf );
                result |= consistency_relationship_delete_invisibles_at_classifier( this_, classifier_id);
            }
        }
        result |= data_diagramelement_iterator_destroy( &diagramelement_iterator );
    }
    else
    {
        U8_TRACE_INFO("All unconditional relationships are visible, no need to check for invisibility.");
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_relationship_delete_invisibles_at_classifier( consistency_relationship_t *this_,
                                                                     data_row_t classifier_id )
{
    U8_TRACE_BEGIN();
    assert( classifier_id != DATA_ROW_VOID );
    u8_error_t result = U8_ERROR_NONE;

    data_small_set_t relations_to_delete;
    data_small_set_init( &relations_to_delete );

    /* load relationships to be checked */
    data_relationship_iterator_t relationship_iterator;
    data_relationship_iterator_init_empty( &relationship_iterator );
    result |= data_database_reader_get_relationships_by_classifier_id( (*this_).db_reader,
                                                                       classifier_id,
                                                                       &relationship_iterator
                                                                     );
    if ( result == U8_ERROR_NONE )
    {
        while( data_relationship_iterator_has_next( &relationship_iterator ) )
        {
            result |= data_relationship_iterator_next( &relationship_iterator, &((*this_).temp_relationship_buf) );

            bool visible = true;
            const u8_error_t vis_err
                = consistency_relationship_private_is_shown_by_a_diagram( this_, &((*this_).temp_relationship_buf), &visible );

            if ( vis_err == U8_ERROR_NONE )
            {
                if ( ! visible )
                {
                    /* invisible relationship found */
                    /* this must be copied into a local data set to make this class re-entrant for recursive calls */
                    const data_id_t relation_to_delete = data_relationship_get_data_id( &((*this_).temp_relationship_buf) );
                    result |= data_small_set_add_obj( &relations_to_delete, relation_to_delete );
                }
            }
            result |= vis_err;
        }
    }
    else
    {
        U8_LOG_WARNING( "Relationships of the deleted classifier cannot be checked for being superfluous now." );
        U8_TRACE_INFO_INT( "classifier has unckecked relationships:", classifier_id );
    }
    result |= data_relationship_iterator_destroy( &relationship_iterator );
    /* note that relationship_iterator cannot be used here any longer due to re-entrancy by recursion */

    /* delete all found relationship */
    const uint32_t relations_count = data_small_set_get_count( &relations_to_delete );
    for ( uint32_t index2 = 0; index2 < relations_count; index2 ++ )
    {
        const data_id_t delete_rel = data_small_set_get_id( &relations_to_delete, index2 );
        assert( data_id_get_table( &delete_rel ) == DATA_TABLE_RELATIONSHIP );
        result |= ctrl_classifier_controller_delete_relationship( (*this_).clfy_ctrl,
                                                                  data_id_get_row_id( &delete_rel ),
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_APPEND
                                                                );
    }

    data_small_set_destroy( &relations_to_delete );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t consistency_relationship_private_is_shown_by_a_diagram( consistency_relationship_t *this_,
                                                                   const data_relationship_t *relation,
                                                                   bool *out_result )
{
    U8_TRACE_BEGIN();
    assert( NULL != relation );
    assert( NULL != out_result );
    u8_error_t result = U8_ERROR_NONE;

    const data_row_t relationship_id = data_relationship_get_row_id( relation );
    bool a_diagram_shows_uncond_relationship = false;  /* tells if a diagram is found where unconditional relationships are visible */
    bool a_diagram_shows_scenario_relationship = false;  /* tells if a diagram is found where scenario relationships are visible */

    {
        data_diagram_iterator_t diagram_iterator;
        data_diagram_iterator_init_empty( &diagram_iterator );

        result |= data_database_reader_get_diagrams_by_relationship_id( (*this_).db_reader,
                                                                        relationship_id,
                                                                        &diagram_iterator
                                                                      );

        if ( result == U8_ERROR_NONE )
        {
            while ( data_diagram_iterator_has_next( &diagram_iterator ) )
            {
                result |= data_diagram_iterator_next( &diagram_iterator, &((*this_).temp_diagram_buf) );
                const data_diagram_type_t diagram_type = data_diagram_get_diagram_type( &((*this_).temp_diagram_buf) );
                a_diagram_shows_uncond_relationship |= data_rules_diagram_shows_uncond_relationships( &((*this_).rules), diagram_type );
                /* Note that ..get_diagrams_by_relationship_id() returns scenario-based diagrams only if the relationship is visible there: */
                a_diagram_shows_scenario_relationship |= data_rules_diagram_shows_scenario_relationships( &((*this_).rules), diagram_type );

                /* Workaround: Box diagrams show containment relationships - this is not yet handled by (*this_).rules */
                /* And anyhow a problem because the use can change the type and bypass existing database triggers... TODO */
                const bool keep_containment_at_box_diagram = ( data_relationship_get_main_type( relation ) == DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT )
                    && ( diagram_type == DATA_DIAGRAM_TYPE_BOX_DIAGRAM );
                a_diagram_shows_uncond_relationship |= keep_containment_at_box_diagram;
            }
        }

        result |= data_diagram_iterator_destroy( &diagram_iterator );
    }

    *out_result = ( a_diagram_shows_uncond_relationship || a_diagram_shows_scenario_relationship );

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
