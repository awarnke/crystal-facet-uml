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
            data_error_t d_err = DATA_ERROR_NONE;

            switch ( data_id_get_table(&search_id) )
            {
                case DATA_TABLE_CLASSIFIER:
                {

                }
                break;

                case DATA_TABLE_FEATURE:
                {

                }
                break;

                case DATA_TABLE_RELATIONSHIP:
                {

                }
                break;

                case DATA_TABLE_DIAGRAMELEMENT:
                {

                }
                break;

                case DATA_TABLE_DIAGRAM:
                {
                    d_err = data_small_set_add_obj ( &((*this_).temp_result_set), search_id );
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
