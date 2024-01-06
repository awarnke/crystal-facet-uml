/* File: gui_search_runner.c; Copyright and License: see below */

#include "gui_search_runner.h"
#include "set/data_search_result_list.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

void gui_search_runner_init ( gui_search_runner_t *this_,
                              gui_simple_message_to_user_t *message_to_user,
                              data_database_reader_t *db_reader,
                              data_database_t *database,
                              gui_sketch_area_t *result_consumer )
{
    U8_TRACE_BEGIN();
    assert ( message_to_user != NULL );
    assert ( db_reader != NULL );
    assert ( database != NULL );
    assert ( result_consumer != NULL );

    (*this_).message_to_user = message_to_user;
    (*this_).db_reader = db_reader;
    const u8_error_t d_err = data_database_text_search_init ( &((*this_).db_searcher), database );
    if ( U8_ERROR_NONE != d_err )
    {
        U8_LOG_WARNING_HEX( "data_database_text_search_t could not be constructed.", d_err );
    }
    (*this_).result_consumer = result_consumer;
    DATA_SEARCH_RESULT_LIST_INIT( &((*this_).temp_result_list), (*this_).temp_result_list_buf );

    U8_TRACE_END();
}

void gui_search_runner_destroy ( gui_search_runner_t *this_ )
{
    U8_TRACE_BEGIN();

    (*this_).message_to_user = NULL;
    (*this_).db_reader = NULL;
    const u8_error_t d_err = data_database_text_search_destroy ( &((*this_).db_searcher) );
    if ( U8_ERROR_NONE != d_err )
    {
        U8_LOG_WARNING_HEX( "data_database_text_search_t could not be destructed.", d_err );
    }
    (*this_).result_consumer = NULL;

    U8_TRACE_END();
}

void gui_search_runner_run ( gui_search_runner_t *this_, const char* search_string )
{
    U8_TRACE_BEGIN();

    if ( search_string != NULL )
    {
        data_id_t search_id;
        data_id_init_by_string ( &search_id, search_string );
        data_id_trace ( &search_id );

        gui_simple_message_to_user_hide( (*this_).message_to_user );

        data_search_result_list_clear( &((*this_).temp_result_list) );
        const data_row_id_t search_row_id = data_id_get_row_id(&search_id);
        u8_error_t d_err = U8_ERROR_NONE;

        if ( data_id_is_valid( &search_id ))
        {
            switch ( data_id_get_table(&search_id) )
            {
                case DATA_TABLE_CLASSIFIER:
                {
                    d_err = data_database_reader_get_classifier_by_id ( (*this_).db_reader,
                                                                        search_row_id,
                                                                        &((*this_).temp_classifier)
                                                                      );
                    if ( d_err == U8_ERROR_NONE )
                    {
                        data_search_result_t half_initialized;
                        data_search_result_init_classifier( &half_initialized,
                                                            search_row_id,
                                                            data_classifier_get_main_type( &((*this_).temp_classifier) ),
                                                            data_classifier_get_name_const( &((*this_).temp_classifier) ),
                                                            DATA_ROW_ID_VOID /* diagram_id */
                                                        );
                        gui_search_runner_private_add_diagrams_of_classifier( this_, &half_initialized, &((*this_).temp_result_list) );

                        data_classifier_destroy( &((*this_).temp_classifier) );
                        data_search_result_destroy( &half_initialized );
                    }
                    else
                    {
                        U8_TRACE_INFO( "classifier does not exist or database not open." );
                    }
                }
                break;

                case DATA_TABLE_FEATURE:
                {
                    d_err = data_database_reader_get_feature_by_id ( (*this_).db_reader,
                                                                     search_row_id,
                                                                     &((*this_).temp_feature)
                                                                   );
                    if ( d_err == U8_ERROR_NONE )
                    {
                        data_row_id_t classifier_id = data_feature_get_classifier_row_id( &((*this_).temp_feature) );
                        data_search_result_t half_initialized;
                        data_search_result_init_feature( &half_initialized,
                                                         data_feature_get_row_id( &((*this_).temp_feature) ),
                                                         data_feature_get_main_type( &((*this_).temp_feature) ),
                                                         data_feature_get_key_const( &((*this_).temp_feature) ),
                                                         classifier_id,
                                                         DATA_ROW_ID_VOID /* diagram_id */
                                                       );
                        gui_search_runner_private_add_diagrams_of_classifier( this_, &half_initialized, &((*this_).temp_result_list) );

                        data_feature_destroy( &((*this_).temp_feature) );
                        data_search_result_destroy( &half_initialized );
                    }
                    else
                    {
                        U8_TRACE_INFO( "feature does not exist or database not open." );
                    }
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    d_err = data_database_reader_get_relationship_by_id ( (*this_).db_reader,
                                                                          search_row_id,
                                                                          &((*this_).temp_relationship)
                                                                        );
                    if ( d_err == U8_ERROR_NONE )
                    {
                        data_row_id_t classifier_id = data_relationship_get_from_classifier_row_id( &((*this_).temp_relationship) );
                        data_search_result_t half_initialized;
                        data_search_result_init_relationship( &half_initialized,
                                                              data_relationship_get_row_id( &((*this_).temp_relationship) ),
                                                              data_relationship_get_main_type( &((*this_).temp_relationship) ),
                                                              data_relationship_get_name_const( &((*this_).temp_relationship) ),
                                                              classifier_id,
                                                              data_relationship_get_to_classifier_row_id( &((*this_).temp_relationship) ),
                                                              DATA_ROW_ID_VOID /* diagram_id */
                                                            );
                        gui_search_runner_private_add_diagrams_of_classifier( this_, &half_initialized, &((*this_).temp_result_list) );

                        data_relationship_destroy( &((*this_).temp_relationship) );
                        data_search_result_destroy( &half_initialized );
                    }
                    else
                    {
                        U8_TRACE_INFO( "relationship does not exist or database not open." );
                    }
                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {
                    d_err = data_database_reader_get_diagramelement_by_id ( (*this_).db_reader,
                                                                            search_row_id,
                                                                            &((*this_).temp_diagramelement)
                                                                          );
                    if ( d_err == U8_ERROR_NONE )
                    {
                        data_search_result_t half_initialized;
                        data_search_result_init_classifier( &half_initialized,
                                                            data_diagramelement_get_classifier_row_id(&((*this_).temp_diagramelement)),
                                                            0 /* match_type is unknown */,
                                                            "" /* match_name */,
                                                            data_diagramelement_get_diagram_row_id(&((*this_).temp_diagramelement))
                                                          );
                        const u8_error_t err = data_search_result_list_add( &((*this_).temp_result_list), &half_initialized );
                        if ( err != U8_ERROR_NONE )
                        {
                            /*d_err = U8_ERROR_ARRAY_BUFFER_EXCEEDED;*/
                            U8_LOG_WARNING( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at inserting search result to list" );
                        }

                        data_diagramelement_destroy( &((*this_).temp_diagramelement) );
                        data_search_result_destroy( &half_initialized );
                    }
                    else
                    {
                        U8_TRACE_INFO( "diagramelement does not exist or database not open." );
                    }
                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    assert( GUI_SEARCH_RUNNER_MAX_DIAGRAMS > 0 );
                    d_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader, search_row_id, &((*this_).temp_diagrams[0]) );
                    if ( d_err == U8_ERROR_NONE )
                    {
                        data_search_result_t half_initialized;
                        data_search_result_init_diagram( &half_initialized,
                                                         search_row_id,
                                                         data_diagram_get_diagram_type( &((*this_).temp_diagrams[0]) ),
                                                         data_diagram_get_name_const( &((*this_).temp_diagrams[0]) )
                                                       );
                        const u8_error_t err = data_search_result_list_add( &((*this_).temp_result_list), &half_initialized );
                        if ( err != U8_ERROR_NONE )
                        {
                            /*d_err = U8_ERROR_ARRAY_BUFFER_EXCEEDED;*/
                            U8_LOG_WARNING( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at inserting search result to list" );
                        }

                        data_diagram_destroy( &((*this_).temp_diagrams[0]) );
                        data_search_result_destroy( &half_initialized );
                    }
                    else
                    {
                        U8_TRACE_INFO( "diagram does not exist or database not open." );
                    }
                }
                break;

                default:
                {
                    assert(false);  /* data_id_is_valid should have been false already */
                }
                break;
            }
        }
        else
        {
            U8_LOG_EVENT_STR( "User search input is not an id", search_string );
        }

        /* free text search */
        d_err = data_database_text_search_get_objects_by_textfragment ( &((*this_).db_searcher),
                                                                        search_string,
                                                                        &((*this_).temp_result_list)
                                                                      );
        if ( U8_ERROR_NONE != d_err )
        {
            U8_LOG_ERROR_HEX( "data_database_text_search_t could not search.", d_err );
        }

        gui_sketch_area_show_result_list ( (*this_).result_consumer, &((*this_).temp_result_list) );
        data_search_result_list_clear( &((*this_).temp_result_list) );
    }
    else
    {
        assert(false);
    }

    U8_TRACE_END();
}

void gui_search_runner_private_add_diagrams_of_classifier ( gui_search_runner_t *this_,
                                                            data_search_result_t *classifier_template,
                                                            data_search_result_list_t *io_list
                                                          )
{
    U8_TRACE_BEGIN();
    assert( classifier_template != NULL );
    assert( io_list != NULL );
    u8_error_t d_err = U8_ERROR_NONE;

    data_row_id_t classifier_row_id;
    if ( DATA_TABLE_CLASSIFIER == data_id_get_table( data_search_result_get_match_id_const( classifier_template )))
    {
        classifier_row_id = data_id_get_row_id( data_search_result_get_match_id_const( classifier_template ));
    }
    else
    {
        classifier_row_id = data_id_get_row_id( data_search_result_get_src_classifier_id_const( classifier_template ));
    }
    uint32_t diagram_count;
    d_err = data_database_reader_get_diagrams_by_classifier_id ( (*this_).db_reader,
                                                                 classifier_row_id,
                                                                 GUI_SEARCH_RUNNER_MAX_DIAGRAMS,
                                                                 &((*this_).temp_diagrams),
                                                                 &diagram_count
                                                               );
    if ( d_err == U8_ERROR_ARRAY_BUFFER_EXCEEDED )
    {
        U8_LOG_WARNING( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at searching diagrams" );
    }

    if (( d_err == U8_ERROR_NONE )||( d_err == U8_ERROR_ARRAY_BUFFER_EXCEEDED ))
    {
        assert ( diagram_count <= GUI_SEARCH_RUNNER_MAX_DIAGRAMS );
        for ( uint32_t idx = 0; idx < diagram_count; idx ++ )
        {
            const data_row_id_t diagram_row_id = data_diagram_get_row_id ( &((*this_).temp_diagrams[idx]) );
            data_id_reinit( data_search_result_get_diagram_id_ptr( classifier_template ), DATA_TABLE_DIAGRAM, diagram_row_id );

            bool filter = false;
            switch ( data_id_get_table( data_search_result_get_match_id_const( classifier_template ) ) )
            {
                case DATA_TABLE_FEATURE:
                {
                    /* if a user searches explicitly for a feature-id, which feature/classifiers should be filtered? */
                    /* and how? till here, the classifier type is not yet loaded. */
                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {
                    /* if a user searches explicitly for a relationship-id, which ones should be filtered? */
                    /* and how? till here, the classifier type is not yet loaded. */
                }
                break;

                default:
                {
                    /* do not filter classifiers (or other things?) */
                }
                break;
            }

            if ( ! filter )
            {
                const u8_error_t err = data_search_result_list_add( io_list, classifier_template );
                if ( err != U8_ERROR_NONE )
                {
                    /*d_err |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;*/
                    U8_LOG_WARNING( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at inserting search result to list" );
                }
            }

            data_diagram_destroy( &((*this_).temp_diagrams[idx]) );
        }
    }
    else
    {
        U8_TRACE_INFO( "diagram does not exist or database not open." );
    }

    U8_TRACE_END();
}


/*
Copyright 2020-2024 Andreas Warnke

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
