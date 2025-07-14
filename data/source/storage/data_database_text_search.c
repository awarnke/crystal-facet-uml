/* File: data_database_text_search.c; Copyright and License: see below */

#include "storage/data_database_text_search.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "u8stream/universal_output_stream.h"
#include "u8stream/universal_memory_output_stream.h"
#include "u8stream/universal_escaping_output_stream.h"
#include <sqlite3.h>
#include <assert.h>

/*!
 *  \brief translation table to encode strings for usage in LIKE search string literals
 */
static const char *const DATA_DATABASE_TEXT_SEARCH_SQL_ENCODE[][2] = {
    { "%", "\\%" },
    { "_", "\\_" },
    { "\\", "\\\\" },
    { NULL, NULL }
};
static const char DATA_DATABASE_TEXT_SEARCH_SQL_LIKE_DELIMITER = '%';

u8_error_t data_database_text_search_init( data_database_text_search_t *this_, data_database_t *database )
{
    U8_TRACE_BEGIN();
    assert( NULL != database );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = database;
    (*this_).is_open = false;

    data_database_listener_init( &((*this_).me_as_listener), this_, (void (*)(void*,data_database_listener_signal_t)) &data_database_text_search_db_change_callback );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    if ( data_database_is_open( database ) )
    {
        /* if the database is open, open also the reader */
        result |= data_database_text_search_private_open( this_ );
    }

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_text_search_destroy( data_database_text_search_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_text_search_private_close( this_ );
    }

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    (*this_).database = NULL;

    U8_TRACE_END_ERR(result);
    return result;
}

void data_database_text_search_db_change_callback( data_database_text_search_t *this_, data_database_listener_signal_t signal_id )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    switch ( signal_id )
    {
        case DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE:
        {
            U8_TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE" );
            if ( (*this_).is_open )
            {
                result |= data_database_text_search_private_close( this_ );
            }
        }
        break;

        case DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED:
        {
            U8_TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED" );
            if ( (*this_).is_open )
            {
                result |= data_database_text_search_private_close( this_ );
            }
            result |= data_database_text_search_private_open( this_ );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "unexpected data_database_listener_signal_t" );
        }
    }

    U8_TRACE_END();
}

/* ================================ SEARCH_RESULT ================================ */

u8_error_t data_database_text_search_get_objects_by_text_fragment ( data_database_text_search_t *this_,
                                                                    const char *textfragment,
                                                                    data_search_result_iterator_t *io_search_result_iterator )
{
    U8_TRACE_BEGIN();
    assert( NULL != io_search_result_iterator );
    assert( NULL != textfragment );
    const unsigned int text_len = utf8string_get_length( textfragment );
    u8_error_t result = U8_ERROR_NONE;

    /* escape-encode textfragment */
    universal_memory_output_stream_t mem_out;
    universal_memory_output_stream_init( &mem_out,
                                         (*this_).temp_like_search_buf,
                                         sizeof( (*this_).temp_like_search_buf ),
                                         UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8
                                       );
    const bool search_empty = ( 0 == text_len );
    if ( search_empty )
    {
        /* no wildcards and no escaping if search string is empty */
    }
    else
    {
        result |= universal_memory_output_stream_write( &mem_out,
                                                        &DATA_DATABASE_TEXT_SEARCH_SQL_LIKE_DELIMITER,
                                                        sizeof(DATA_DATABASE_TEXT_SEARCH_SQL_LIKE_DELIMITER)
                                                      );
        {
            universal_output_stream_t* stream_out = universal_memory_output_stream_get_output_stream( &mem_out );
            universal_escaping_output_stream_t esc_out;
            universal_escaping_output_stream_init( &esc_out, &DATA_DATABASE_TEXT_SEARCH_SQL_ENCODE, stream_out );
            result |= universal_escaping_output_stream_write( &esc_out, textfragment, text_len );
            result |= universal_escaping_output_stream_flush( &esc_out );
            result |= universal_escaping_output_stream_destroy( &esc_out );
        }
        result |= universal_memory_output_stream_write( &mem_out,
                                                        &DATA_DATABASE_TEXT_SEARCH_SQL_LIKE_DELIMITER,
                                                        sizeof(DATA_DATABASE_TEXT_SEARCH_SQL_LIKE_DELIMITER)
                                                      );
    }
    result |= universal_memory_output_stream_destroy( &mem_out );

    U8_TRACE_INFO_STR( "LIKE SEARCH:", (*this_).temp_like_search_buf );
    if ( result != U8_ERROR_NONE )
    {
        U8_LOG_WARNING_STR( "error at escaping the search string", textfragment );
    }
    else
    {
        /* search for the prepared pattern. In case of empty, search for a non-existing pattern in the type fields */
        const char *const search_name = search_empty ? "" : (*this_).temp_like_search_buf;
        const char *const search_type = search_empty ? "\n" : (*this_).temp_like_search_buf;
        const char *const search_descr = search_empty ? "" : (*this_).temp_like_search_buf;

        if ( (*this_).is_open )
        {
            sqlite3_stmt *const prepared_statement_diag = (*this_).statement_diagram_ids_by_textfragment;
            result |= data_database_text_search_private_bind_three_texts_to_statement( this_,
                                                                                       prepared_statement_diag,
                                                                                       search_name,
                                                                                       search_type,
                                                                                       search_descr
                                                                                     );

            sqlite3_stmt *const prepared_statement_class = (*this_).statement_classifier_ids_by_textfragment;
            result |= data_database_text_search_private_bind_three_texts_to_statement( this_,
                                                                                       prepared_statement_class,
                                                                                       search_name,
                                                                                       search_type,
                                                                                       search_descr
                                                                                     );

            sqlite3_stmt *const prepared_statement_feat = (*this_).statement_feature_ids_by_textfragment;
            result |= data_database_text_search_private_bind_three_texts_to_statement( this_,
                                                                                       prepared_statement_feat,
                                                                                       search_name, /* key */
                                                                                       search_type, /* value */
                                                                                       search_descr
                                                                                     );

            sqlite3_stmt *const prepared_statement_rel = (*this_).statement_relationship_ids_by_textfragment;
            result |= data_database_text_search_private_bind_three_texts_to_statement( this_,
                                                                                       prepared_statement_rel,
                                                                                       search_name,
                                                                                       search_type,
                                                                                       search_descr
                                                                                     );

            data_database_borrowed_stmt_t sql_statement_diag;
            data_database_borrowed_stmt_init( &sql_statement_diag,
                                              (*this_).database,
                                              prepared_statement_diag,
                                              &((*this_).statement_diagram_borrowed)
                                            );
            data_database_borrowed_stmt_t sql_statement_class;
            data_database_borrowed_stmt_init( &sql_statement_class,
                                              (*this_).database,
                                              prepared_statement_class,
                                              &((*this_).statement_classifier_borrowed)
                                            );
            data_database_borrowed_stmt_t sql_statement_feat;
            data_database_borrowed_stmt_init( &sql_statement_feat,
                                              (*this_).database,
                                              prepared_statement_feat,
                                              &((*this_).statement_feature_borrowed)
                                            );
            data_database_borrowed_stmt_t sql_statement_rel;
            data_database_borrowed_stmt_init( &sql_statement_rel,
                                              (*this_).database,
                                              prepared_statement_rel,
                                              &((*this_).statement_relationship_borrowed)
                                            );
            result |= data_search_result_iterator_reinit( io_search_result_iterator,
                                                          sql_statement_diag,
                                                          sql_statement_class,
                                                          sql_statement_feat,
                                                          sql_statement_rel,
                                                          &((*this_).temp_search_result_buf)
                                                        );
            /* do not destroy sql_statement_xxx; the object is transferred to the iterator and consumed there. */
        }
        else
        {
            result |= U8_ERROR_NO_DB;
            U8_TRACE_INFO( "Database not open, cannot request data." );
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ private ================================ */

u8_error_t data_database_text_search_private_open( data_database_text_search_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_open )
    {
        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_SEARCH_RESULT_ITERATOR_SELECT_DIAGRAM_BY_TEXTFRAGMENT,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_diagram_ids_by_textfragment)
                                                 );
        (*this_).statement_diagram_borrowed = false;
        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_SEARCH_RESULT_ITERATOR_SELECT_CLASSIFIER_BY_TEXTFRAGMENT,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_classifier_ids_by_textfragment)
                                                 );
        (*this_).statement_classifier_borrowed = false;
        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_SEARCH_RESULT_ITERATOR_SELECT_FEATURE_BY_TEXTFRAGMENT,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_feature_ids_by_textfragment)
                                                 );
        (*this_).statement_feature_borrowed = false;
        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_SEARCH_RESULT_ITERATOR_SELECT_RELATIONSHIP_BY_TEXTFRAGMENT,
                                                   DATA_DATABASE_SQL_LENGTH_AUTO_DETECT,
                                                   &((*this_).statement_relationship_ids_by_textfragment)
                                                 );
        (*this_).statement_relationship_borrowed = false;

        (*this_).is_open = true;
    }
    else
    {
        result |= U8_ERROR_INVALID_REQUEST;
        U8_LOG_WARNING( "Database is already open." );
    }

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_text_search_private_close( data_database_text_search_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        assert( (*this_).statement_relationship_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_relationship_ids_by_textfragment );
        assert( (*this_).statement_feature_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_feature_ids_by_textfragment );
        assert( (*this_).statement_classifier_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_classifier_ids_by_textfragment );
        assert( (*this_).statement_diagram_borrowed == false );
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_diagram_ids_by_textfragment );

        (*this_).is_open = false;
    }
    else
    {
        result |= U8_ERROR_INVALID_REQUEST;
        U8_LOG_WARNING( "Database was not open." );
    }

    U8_TRACE_END_ERR(result);
    return result;
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
