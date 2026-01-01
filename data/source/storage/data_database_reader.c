/* File: data_database_reader.c; Copyright and License: see below */

#include "storage/data_database_reader.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

u8_error_t data_database_reader_init ( data_database_reader_t *this_, data_database_t *database )
{
    U8_TRACE_BEGIN();
    assert( NULL != database );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = database;
    (*this_).is_open = false;

    data_database_listener_init ( &((*this_).me_as_listener),
                                  this_,
                                  (void (*)(void*,data_database_listener_signal_t)) &data_database_reader_db_change_callback
                                );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    if ( data_database_is_open( database ) )
    {
        /* if the database is open, open also the reader */
        result |= data_database_reader_private_open( this_ );
    }

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_reader_destroy ( data_database_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_reader_private_close( this_ );
    }

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    (*this_).database = NULL;

    U8_TRACE_END_ERR(result);
    return result;
}

void data_database_reader_db_change_callback ( data_database_reader_t *this_, data_database_listener_signal_t signal_id )
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
                result |= data_database_reader_private_close( this_ );
            }
        }
        break;

        case DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED:
        {
            U8_TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED" );
            if ( (*this_).is_open )
            {
                result |= data_database_reader_private_close( this_ );
            }
            result |= data_database_reader_private_open( this_ );
        }
        break;

        default:
        {
            U8_LOG_ERROR( "unexpected data_database_listener_signal_t" );
        }
    }

    U8_TRACE_END();
}

/* ================================ private ================================ */

u8_error_t data_database_reader_private_open ( data_database_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_open )
    {
        result |= data_database_classifier_reader_init( &((*this_).temp_classifier_reader), (*this_).database );
        result |= data_database_diagram_reader_init( &((*this_).temp_diagram_reader), (*this_).database );

        if ( result == U8_ERROR_NONE )
        {
            (*this_).is_open = true;
        }
    }
    else
    {
        result |= U8_ERROR_INVALID_REQUEST;
        U8_LOG_WARNING( "Database is already open." );
    }

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_reader_private_close ( data_database_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_diagram_reader_destroy( &((*this_).temp_diagram_reader) );
        result |= data_database_classifier_reader_destroy( &((*this_).temp_classifier_reader) );

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
Copyright 2016-2026 Andreas Warnke

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
