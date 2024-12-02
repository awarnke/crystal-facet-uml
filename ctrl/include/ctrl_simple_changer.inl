/* File: ctrl_simple_changer.c; Copyright and License: see below */

#include "ctrl_simple_changer.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

static inline void ctrl_simple_changer_init ( ctrl_simple_changer_t *this_,
                                              ctrl_controller_t *controller,
                                              data_database_reader_t *db_reader )
{
    U8_TRACE_BEGIN();
    assert( NULL != controller );
    assert( NULL != db_reader );

    /* init member attributes */
    (*this_).controller = controller;
    (*this_).db_reader = db_reader;

    U8_TRACE_END();
}

static inline void ctrl_simple_changer_destroy ( ctrl_simple_changer_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( NULL != (*this_).controller );
    assert( NULL != (*this_).db_reader );

    /* destroy member attributes */
    (*this_).controller = NULL;
    (*this_).db_reader = NULL;

    U8_TRACE_END();
}

static inline u8_error_t ctrl_simple_changer_update_classifier_name ( ctrl_simple_changer_t *this_,
                                                                      data_row_t classifier_id,
                                                                      const char* new_classifier_name,
                                                                      u8_error_t* out_info )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_classifier_name );
    assert( NULL != out_info );
    u8_error_t result = U8_ERROR_NONE;

    ctrl_classifier_controller_t *const classifier_ctrl = ctrl_controller_get_classifier_control_ptr( (*this_).controller);

    /* To not run into TOCTOU problems, we try to change the name immediately and then check for success afterwards: */
    const u8_error_t initial_err
        = ctrl_classifier_controller_update_classifier_name( classifier_ctrl,
                                                             classifier_id,
                                                             new_classifier_name
                                                           );
    if ( u8_error_contains( initial_err, U8_ERROR_DUPLICATE_NAME ) )
    {
        *out_info = initial_err;

        /* find an alternative, unused name */
        char wish_name_buf[DATA_CLASSIFIER_MAX_NAME_SIZE];
        utf8stringbuf_t wish_name = UTF8STRINGBUF( wish_name_buf );
        result |= utf8stringbuf_copy_str( wish_name, new_classifier_name );  /* error to be reported to caller */
        {
            bool name_ok = false;
            static const uint_fast16_t MAX_SEARCH_STEP = 10000;
            static const uint_fast16_t FIRST_STEP = 2;
            for ( uint_fast16_t search_step = FIRST_STEP; ( search_step < MAX_SEARCH_STEP )&&( ! name_ok )&&( result == U8_ERROR_NONE ); search_step ++ )
            {
                char new_name_buf[DATA_CLASSIFIER_MAX_NAME_SIZE];
                utf8stringbuf_t new_name = UTF8STRINGBUF( new_name_buf );
                const u8_error_t trunc_err
                    = ctrl_simple_changer_private_propose_classifier_name( this_,
                                                                           utf8stringbuf_get_string( wish_name ),
                                                                           search_step,
                                                                           new_name
                                                                         );
                if ( trunc_err != U8_ERROR_NONE )
                {
                    U8_TRACE_INFO_STR("Name truncated at search for alternative:", utf8stringbuf_get_string( new_name ) );
                }
                const u8_error_t retry_err
                    = ctrl_classifier_controller_update_classifier_name( classifier_ctrl,
                                                                         classifier_id,
                                                                         utf8stringbuf_get_string( new_name )
                                                                       );
                if ( u8_error_contains( retry_err, U8_ERROR_DUPLICATE_NAME ) )
                {
                    *out_info |= retry_err;
                }
                else
                {
                    name_ok = ( retry_err == U8_ERROR_NONE );
                    result |= retry_err;
                }
            }
        }
    }
    else
    {
        result = initial_err;
        *out_info = U8_ERROR_NONE;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t ctrl_simple_changer_private_propose_classifier_name ( ctrl_simple_changer_t *this_,
                                                                               const char* base_classifier_name,
                                                                               uint_fast16_t iteration,
                                                                               utf8stringbuf_t out_name )
{
    U8_TRACE_BEGIN();
    const size_t RESERVED_FOR_NUMBER = 5;
    assert( NULL != base_classifier_name );
    assert( utf8stringbuf_get_size(out_name) > RESERVED_FOR_NUMBER );
    u8_error_t result = U8_ERROR_NONE;

    /* find an alternative, unused name */
    /* copy the base_classifier_name to newname_buf */
    {
        utf8stringbuf_t shortened_new_name
            = utf8stringbuf_init( utf8stringbuf_get_size(out_name)-RESERVED_FOR_NUMBER, utf8stringbuf_get_string(out_name) );
        result |= utf8stringbuf_copy_str( shortened_new_name, base_classifier_name );
        /* null termination is guaranteed, also this function does not cut an utf8 code point in the middle. */
    }
    /* append a separator and the iteration number */
    result |= utf8stringbuf_append_str( out_name, "-" );
    result |= utf8stringbuf_append_int( out_name, iteration );

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2023-2024 Andreas Warnke

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
