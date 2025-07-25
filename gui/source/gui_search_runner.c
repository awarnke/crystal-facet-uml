/* File: gui_search_runner.c; Copyright and License: see below */

#include "gui_search_runner.h"
#include "set/data_search_result_list.h"
#include "utf8stream/utf8stream_writer.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "u8/u8_i32.h"
#include <assert.h>

void gui_search_runner_init ( gui_search_runner_t *this_,
                              gui_simple_message_to_user_t *message_to_user,
                              data_database_reader_t *db_reader,
                              data_database_t *database,
                              observer_t result_consumer )
{
    U8_TRACE_BEGIN();
    assert ( message_to_user != NULL );
    assert ( db_reader != NULL );
    assert ( database != NULL );

    /* external entities */
    (*this_).message_to_user = message_to_user;
    (*this_).db_reader = db_reader;
    const u8_error_t d_err = data_database_text_search_init ( &((*this_).db_searcher), database );
    if ( U8_ERROR_NONE != d_err )
    {
        U8_LOG_WARNING_HEX( "data_database_text_search_t could not be constructed.", d_err );
    }
    (*this_).result_consumer = result_consumer;

    /* request data */
    utf8stream_writemem_init( &((*this_).search_string_writer),
                              &((*this_).search_string_buf),
                              sizeof( (*this_).search_string_buf )
                            );
    (*this_).page_request = pos_scroll_page_new( 0, false /* backwards */ );

    /* result data */
    (*this_).result_buffer_start = 0;
    DATA_SEARCH_RESULT_LIST_INIT( &((*this_).result_list), (*this_).result_buffer );

    U8_TRACE_END();
}

void gui_search_runner_destroy ( gui_search_runner_t *this_ )
{
    U8_TRACE_BEGIN();

    /* external entities */
    (*this_).message_to_user = NULL;
    (*this_).db_reader = NULL;
    const u8_error_t d_err = data_database_text_search_destroy ( &((*this_).db_searcher) );
    if ( U8_ERROR_NONE != d_err )
    {
        U8_LOG_WARNING_HEX( "data_database_text_search_t could not be destructed.", d_err );
    }
    observer_destroy( &((*this_).result_consumer) );

    /* request data */
    utf8stream_writemem_destroy( &((*this_).search_string_writer) );

    /* result data */
    (*this_).result_buffer_start = 0;
    data_search_result_list_destroy( &((*this_).result_list) );

    U8_TRACE_END();
}

void gui_search_runner_run ( gui_search_runner_t *this_, const char* search_string, pos_scroll_page_t page )
{
    U8_TRACE_BEGIN();

    if ( search_string != NULL )
    {
        /* store parameters internally */
        utf8stream_writemem_reset( &((*this_).search_string_writer) );
        utf8stream_writer_t *write = utf8stream_writemem_get_writer( &((*this_).search_string_writer) );
        u8_error_t u_err = utf8stream_writer_write_str( write, search_string );
        if ( u_err != U8_ERROR_NONE )
        {
            U8_TRACE_INFO( "search string too long, was truncated." );
        }

        gui_search_runner_rerun( this_, page );
    }
    else
    {
        assert(false);
    }

    U8_TRACE_END();
}

void gui_search_runner_rerun ( gui_search_runner_t *this_, pos_scroll_page_t page )
{
    U8_TRACE_BEGIN();

    /* store parameters internally */
    (*this_).page_request = page;
    pos_scroll_page_trace( &page );
    const char *const search_string = utf8stream_writemem_get_string( &((*this_).search_string_writer) );
    uint_fast32_t skip_results
        = ( pos_scroll_page_get_backwards( &page ) )
        ? u8_i32_max2( 0, ((signed)( pos_scroll_page_get_anchor_index( &page ) - GUI_SEARCH_RUNNER_MAX_RESULTS + 1 )) )
        : pos_scroll_page_get_anchor_index( &page );
    U8_TRACE_INFO_INT( "skipping", skip_results );

    /* reset previous errors/warnings/infos */
    gui_simple_message_to_user_hide( (*this_).message_to_user );
    u8_error_t d_err = U8_ERROR_NONE;

    /* prepare search result */
    (*this_).result_buffer_start = 0;
    data_search_result_list_clear( &((*this_).result_list) );

    /* check if an id is being searched */
    data_id_t search_id;
    data_id_init_by_string ( &search_id, search_string );
    data_id_trace ( &search_id );
    const data_row_t search_row_id = data_id_get_row_id(&search_id);

    if ( data_id_is_valid( &search_id ))
    {
        switch ( data_id_get_table(&search_id) )
        {
            case DATA_TABLE_CLASSIFIER:
            {
                d_err = data_database_reader_get_classifier_by_id( (*this_).db_reader,
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
                                                        DATA_ROW_VOID /* diagram_id */
                                                      );
                    gui_search_runner_private_add_diagrams_of_classifier( this_,
                                                                          &half_initialized,
                                                                          &skip_results,
                                                                          &((*this_).result_list)
                                                                        );

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
                d_err = data_database_reader_get_feature_by_id( (*this_).db_reader,
                                                                search_row_id,
                                                                &((*this_).temp_feature)
                                                              );
                if ( d_err == U8_ERROR_NONE )
                {
                    data_row_t classifier_id = data_feature_get_classifier_row_id( &((*this_).temp_feature) );
                    data_search_result_t half_initialized;
                    data_search_result_init_feature( &half_initialized,
                                                     data_feature_get_row_id( &((*this_).temp_feature) ),
                                                     data_feature_get_main_type( &((*this_).temp_feature) ),
                                                     data_feature_get_key_const( &((*this_).temp_feature) ),
                                                     classifier_id,
                                                     DATA_ROW_VOID /* diagram_id */
                                                   );
                    gui_search_runner_private_add_diagrams_of_classifier( this_,
                                                                          &half_initialized,
                                                                          &skip_results,
                                                                          &((*this_).result_list)
                                                                        );

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
                d_err = data_database_reader_get_relationship_by_id( (*this_).db_reader,
                                                                     search_row_id,
                                                                     &((*this_).temp_relationship)
                                                                   );
                if ( d_err == U8_ERROR_NONE )
                {
                    data_row_t classifier_id = data_relationship_get_from_classifier_row_id( &((*this_).temp_relationship) );
                    data_search_result_t half_initialized;
                    data_search_result_init_relationship( &half_initialized,
                                                          data_relationship_get_row_id( &((*this_).temp_relationship) ),
                                                          data_relationship_get_main_type( &((*this_).temp_relationship) ),
                                                          data_relationship_get_name_const( &((*this_).temp_relationship) ),
                                                          classifier_id,
                                                          data_relationship_get_to_classifier_row_id( &((*this_).temp_relationship) ),
                                                          DATA_ROW_VOID /* diagram_id */
                                                        );
                    gui_search_runner_private_add_diagrams_of_classifier( this_,
                                                                          &half_initialized,
                                                                          &skip_results,
                                                                          &((*this_).result_list)
                                                                        );

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
                d_err = data_database_reader_get_diagramelement_by_id( (*this_).db_reader,
                                                                       search_row_id,
                                                                       &((*this_).temp_diagramelement)
                                                                     );
                if ( d_err == U8_ERROR_NONE )
                {
                    if ( skip_results == 0 )
                    {
                        data_search_result_t half_initialized;
                        data_search_result_init_classifier( &half_initialized,
                                                            data_diagramelement_get_classifier_row_id(&((*this_).temp_diagramelement)),
                                                            0 /* match_type is unknown */,
                                                            "" /* match_name */,
                                                            data_diagramelement_get_diagram_row_id(&((*this_).temp_diagramelement))
                                                          );
                        const u8_error_t err = data_search_result_list_add( &((*this_).result_list), &half_initialized );
                        if ( err != U8_ERROR_NONE )
                        {
                            /*d_err = U8_ERROR_ARRAY_BUFFER_EXCEEDED;*/
                            U8_LOG_WARNING( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at inserting search result to list" );
                        }
                        data_search_result_destroy( &half_initialized );
                    }
                    else
                    {
                        /* to advance to the requested search result page, skip this entry */
                        skip_results --;
                        (*this_).result_buffer_start ++;
                    }

                    data_diagramelement_destroy( &((*this_).temp_diagramelement) );
                }
                else
                {
                    U8_TRACE_INFO( "diagramelement does not exist or database not open." );
                }
            }
            break;

            case DATA_TABLE_DIAGRAM:
            {
                d_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader, search_row_id, &((*this_).temp_diagram) );
                if ( d_err == U8_ERROR_NONE )
                {
                    if ( skip_results == 0 )
                    {
                        data_search_result_t half_initialized;
                        data_search_result_init_diagram( &half_initialized,
                                                         search_row_id,
                                                         data_diagram_get_diagram_type( &((*this_).temp_diagram) ),
                                                         data_diagram_get_name_const( &((*this_).temp_diagram) )
                                                       );
                        const u8_error_t err = data_search_result_list_add( &((*this_).result_list), &half_initialized );
                        if ( err != U8_ERROR_NONE )
                        {
                            /*d_err = U8_ERROR_ARRAY_BUFFER_EXCEEDED;*/
                            U8_LOG_WARNING( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at inserting search result to list" );
                        }
                        data_search_result_destroy( &half_initialized );
                    }
                    else
                    {
                        /* to advance to the requested search result page, skip this entry */
                        skip_results --;
                        (*this_).result_buffer_start ++;
                    }

                    data_diagram_destroy( &((*this_).temp_diagram) );
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
    {
        data_search_result_iterator_t data_search_result_iterator;
        d_err |= data_search_result_iterator_init_empty( &data_search_result_iterator );
        d_err = data_database_text_search_get_objects_by_text_fragment( &((*this_).db_searcher),
                                                                        search_string,
                                                                        &data_search_result_iterator
                                                                      );
        while ( data_search_result_iterator_has_next( &data_search_result_iterator ) )
        {
            data_search_result_t current_search_result;
            d_err |= data_search_result_iterator_next( &data_search_result_iterator,
                                                       &current_search_result
                                                     );
            if ( skip_results == 0 )
            {
                d_err |= data_search_result_list_add( &((*this_).result_list), &current_search_result );
                if ( d_err != U8_ERROR_NONE )
                {
                    /*d_err = U8_ERROR_ARRAY_BUFFER_EXCEEDED;*/
                    U8_LOG_WARNING( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at inserting search result to list" );
                }
            }
            else
            {
                /* to advance to the requested search result page, skip this entry */
                skip_results --;
                (*this_).result_buffer_start ++;
            }
        }
        d_err |= data_search_result_iterator_destroy( &data_search_result_iterator );
    }

    if ( U8_ERROR_NONE != d_err )
    {
        U8_LOG_ERROR_HEX( "data_database_text_search_t could not search.", d_err );
    }

    /* present the result */
    observer_notify( &((*this_).result_consumer), this_ );

    /* clear the result */
    data_search_result_list_clear( &((*this_).result_list) );

    U8_TRACE_END();
}

void gui_search_runner_private_add_diagrams_of_classifier ( gui_search_runner_t *this_,
                                                            data_search_result_t *classifier_template,
                                                            uint_fast32_t *io_skip_results,
                                                            data_search_result_list_t *io_list
                                                          )
{
    U8_TRACE_BEGIN();
    assert( classifier_template != NULL );
    assert( io_list != NULL );
    u8_error_t d_err = U8_ERROR_NONE;

    data_row_t classifier_row_id;
    if ( DATA_TABLE_CLASSIFIER == data_id_get_table( data_search_result_get_match_id_const( classifier_template )))
    {
        classifier_row_id = data_id_get_row_id( data_search_result_get_match_id_const( classifier_template ));
    }
    else
    {
        classifier_row_id = data_id_get_row_id( data_search_result_get_src_classifier_id_const( classifier_template ));
    }

    data_diagram_iterator_t diagram_iterator;
    d_err |= data_diagram_iterator_init_empty( &diagram_iterator );
    d_err |= data_database_reader_get_diagrams_by_classifier_id( (*this_).db_reader,
                                                                 classifier_row_id,
                                                                 &diagram_iterator
                                                               );

    if ( d_err == U8_ERROR_NONE )
    {
        while ( data_diagram_iterator_has_next( &diagram_iterator ) )
        {
            d_err |= data_diagram_iterator_next( &diagram_iterator, &((*this_).temp_diagram) );
            const data_row_t diagram_row_id = data_diagram_get_row_id( &((*this_).temp_diagram) );
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
                if ( (*io_skip_results) == 0 )
                {
                    const u8_error_t err = data_search_result_list_add( io_list, classifier_template );
                    if ( err != U8_ERROR_NONE )
                    {
                        /*d_err |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;*/
                        U8_LOG_WARNING( "U8_ERROR_ARRAY_BUFFER_EXCEEDED at inserting search result to list" );
                    }
                }
                else
                {
                    /* to advance to the requested search result page, skip this entry */
                    *io_skip_results = (*io_skip_results) - 1;
                    (*this_).result_buffer_start ++;
                }
            }

            data_diagram_destroy( &((*this_).temp_diagram) );
        }
    }
    else
    {
        U8_TRACE_INFO( "diagram does not exist or database not open." );
    }
    d_err |= data_diagram_iterator_destroy( &diagram_iterator );

    U8_TRACE_END();
}


/*
Copyright 2020-2025 Andreas Warnke

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
