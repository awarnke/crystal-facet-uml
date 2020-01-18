/* File: gui_search_runner.c; Copyright and License: see below */

#include "gui_search_runner.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void gui_search_runner_init ( gui_search_runner_t *this_,
                              gui_simple_message_to_user_t *message_to_user,
                              data_database_reader_t *db_reader,
                              gui_sketch_area_t *result_consumer )
{
    TRACE_BEGIN();
    assert ( message_to_user != NULL );
    assert ( db_reader != NULL );
    assert ( result_consumer != NULL );

    (*this_).message_to_user = message_to_user;
    (*this_).db_reader = db_reader;
    (*this_).result_consumer = result_consumer;

    TRACE_END();
}

void gui_search_runner_destroy ( gui_search_runner_t *this_ )
{
    TRACE_BEGIN();

    (*this_).message_to_user = NULL;
    (*this_).db_reader = NULL;
    (*this_).result_consumer = NULL;

    TRACE_END();
}

void gui_search_runner_run ( gui_search_runner_t *this_, const char* search_string )
{
    TRACE_BEGIN();

    if ( search_string != NULL )
    {
        data_id_t search_id;
        data_id_init_by_string ( &search_id, search_string );
        data_id_trace ( &search_id );

        if ( data_id_is_valid( &search_id ))
        {
            gui_simple_message_to_user_hide( (*this_).message_to_user );

            data_small_set_init( &((*this_).temp_result_set) );
            const int64_t search_row_id = data_id_get_row_id(&search_id);
            data_error_t d_err = DATA_ERROR_NONE;

            switch ( data_id_get_table(&search_id) )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    gui_search_runner_private_add_diagrams_of_classifier( this_, search_row_id, &((*this_).temp_result_set) );
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    d_err = data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                     search_row_id,
                                                                     &((*this_).temp_feature)
                                                                   );
                    if ( d_err == DATA_ERROR_NONE )
                    {
                        int64_t classifier_id = data_feature_get_classifier_id( &((*this_).temp_feature) );
                        gui_search_runner_private_add_diagrams_of_classifier( this_, classifier_id, &((*this_).temp_result_set) );

                        data_feature_destroy( &((*this_).temp_feature) );
                    }
                    else
                    {
                        TRACE_INFO( "feature does not exist or database not open." );
                    }
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    d_err = data_database_reader_get_relationship_by_id ( (*this_).db_reader,
                                                                          search_row_id,
                                                                          &((*this_).temp_relationship)
                                                                        );
                    if ( d_err == DATA_ERROR_NONE )
                    {
                        int64_t classifier_id = data_relationship_get_from_classifier_id( &((*this_).temp_relationship) );
                        gui_search_runner_private_add_diagrams_of_classifier( this_, classifier_id, &((*this_).temp_result_set) );

                        data_relationship_destroy( &((*this_).temp_relationship) );
                    }
                    else
                    {
                        TRACE_INFO( "relationship does not exist or database not open." );
                    }
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    d_err = data_database_reader_get_diagramelement_by_id ( (*this_).db_reader,
                                                                            search_row_id,
                                                                            &((*this_).temp_diagramelement)
                                                                          );
                    if ( d_err == DATA_ERROR_NONE )
                    {
                        d_err = data_small_set_add_row_id ( &((*this_).temp_result_set),
                                                            DATA_TABLE_DIAGRAM,
                                                            data_diagramelement_get_diagram_id(&((*this_).temp_diagramelement))
                                                          );

                        data_diagramelement_destroy( &((*this_).temp_diagramelement) );
                    }
                    else
                    {
                        TRACE_INFO( "diagramelement does not exist or database not open." );
                    }
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    assert( GUI_SEARCH_RUNNER_MAX_DIAGRAMS > 0 );
                    d_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader, search_row_id, &((*this_).temp_diagrams[0]) );
                    if ( d_err == DATA_ERROR_NONE )
                    {
                        d_err = data_small_set_add_obj ( &((*this_).temp_result_set), search_id );

                        data_diagram_destroy( &((*this_).temp_diagrams[0]) );
                    }
                    else
                    {
                        TRACE_INFO( "diagram does not exist or database not open." );
                    }
                }
                break;

                default:
                {
                    assert(false);  /* data_id_is_valid should have been false already */
                }
                break;
            }

            if ( d_err == DATA_ERROR_NONE )
            {
                gui_sketch_area_show_result_list ( (*this_).result_consumer, &((*this_).temp_result_set) );
            }

            data_small_set_destroy( &((*this_).temp_result_set) );
        }
        else
        {
            gui_simple_message_to_user_show_message_with_int( (*this_).message_to_user,
                                                              GUI_SIMPLE_MESSAGE_TYPE_WARNING,
                                                              GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA,
                                                              0 /* pos of error */
                                                            );
        }
    }
    else
    {
        assert(false);
    }

    TRACE_END();
}

void gui_search_runner_private_add_diagrams_of_classifier ( gui_search_runner_t *this_, int64_t classifier_id, data_small_set_t *io_set )
{
    TRACE_BEGIN();
    data_error_t d_err = DATA_ERROR_NONE;

    uint32_t diagram_count;
    d_err = data_database_reader_get_diagrams_by_classifier_id ( (*this_).db_reader,
                                                                 classifier_id,
                                                                 GUI_SEARCH_RUNNER_MAX_DIAGRAMS,
                                                                 &((*this_).temp_diagrams),
                                                                 &diagram_count
                                                               );
    if ( d_err == DATA_ERROR_ARRAY_BUFFER_EXCEEDED )
    {
        TSLOG_WARNING( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at searching diagrams" );
    }

    if (( d_err == DATA_ERROR_NONE )||( d_err == DATA_ERROR_ARRAY_BUFFER_EXCEEDED ))
    {
        assert ( diagram_count <= GUI_SEARCH_RUNNER_MAX_DIAGRAMS );
        for ( uint32_t idx = 0; idx < diagram_count; idx ++ )
        {
            data_id_t current_id;
            data_id_init( &current_id, DATA_TABLE_DIAGRAM, data_diagram_get_id ( &((*this_).temp_diagrams[idx]) ));
            d_err = data_small_set_add_obj ( io_set, current_id );
            if ( d_err == DATA_ERROR_DUPLICATE_ID )
            {
                /* not a bug: a duplicate diagram is caused by a classifier that exists twice in a diagram */
                d_err = DATA_ERROR_NONE;
            }
            else if ( d_err == DATA_ERROR_ARRAY_BUFFER_EXCEEDED )
            {
                TSLOG_WARNING( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at inserting diagram ids to a set" );
            }
            else if ( d_err != DATA_ERROR_NONE )
            {
                TSLOG_WARNING_HEX( "other error at inserting diagram ids to a set", d_err );
            }

            data_diagram_destroy( &((*this_).temp_diagrams[idx]) );
        }
    }
    else
    {
        TRACE_INFO( "diagram does not exist or database not open." );
    }

    TRACE_END();
}


/*
Copyright 2020-2020 Andreas Warnke

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
