/* File: data_database_head_reader.c; Copyright and License: see below */

#include "storage/data_database_head_reader.h"
#include "entity/data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include <assert.h>

u8_error_t data_database_head_reader_init( data_database_head_reader_t *this_, data_database_t *database )
{
    U8_TRACE_BEGIN();
    assert( NULL != database );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = database;

    (*this_).statement_head_by_id = NULL;
    (*this_).statement_head_by_key = NULL;

    result |= data_database_head_reader_private_open( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_head_reader_destroy( data_database_head_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_head_reader_private_close( this_ );

    (*this_).database = NULL;

    U8_TRACE_END_ERR(result);
    return result;
}

/* ================================ HEAD ================================ */

/*!
 *  \brief predefined search statement to find a head value by id
 */
static const char DATA_DATABASE_HEAD_SELECT_HEAD_BY_ID[]
    = "SELECT id,key,value FROM head WHERE id=?";

/*!
 *  \brief predefined search statement to find a head value by key
 */
static const char DATA_DATABASE_HEAD_SELECT_HEAD_BY_KEY[]
    = "SELECT id,key,value FROM head WHERE key=?";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_HEAD_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: key
 */
static const int RESULT_HEAD_KEY_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: value
 */
static const int RESULT_HEAD_VALUE_COLUMN = 2;

u8_error_t data_database_head_reader_read_value_by_id ( data_database_head_reader_t *this_, data_row_t id, data_head_t *out_head )
{
    U8_TRACE_BEGIN();
    assert( out_head != NULL );
    u8_error_t result = U8_ERROR_NONE;

    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_head_by_id;

        result |= data_database_head_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO_INT( "sqlite3_step did not find a row for id", id );
            result |= U8_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_head_init( out_head,
                                      sqlite3_column_int64( prepared_statement, RESULT_HEAD_ID_COLUMN ),
                                      (const char*) sqlite3_column_text( prepared_statement, RESULT_HEAD_KEY_COLUMN ),
                                      (const char*) sqlite3_column_text( prepared_statement, RESULT_HEAD_VALUE_COLUMN )
                                    );

            data_head_trace( out_head );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_head_reader_read_value_by_key ( data_database_head_reader_t *this_, const char *key, data_head_t *out_head )
{
    U8_TRACE_BEGIN();
    assert( key != NULL );
    assert( out_head != NULL );
    u8_error_t result = U8_ERROR_NONE;

    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    {
        prepared_statement = (*this_).statement_head_by_key;

        result |= data_database_head_reader_private_bind_text_to_statement( this_, prepared_statement, key );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO_STR( "sqlite3_step did not find a row for key", key );
            result |= U8_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_head_init( out_head,
                                      sqlite3_column_int64( prepared_statement, RESULT_HEAD_ID_COLUMN ),
                                      (const char*) sqlite3_column_text( prepared_statement, RESULT_HEAD_KEY_COLUMN ),
                                      (const char*) sqlite3_column_text( prepared_statement, RESULT_HEAD_VALUE_COLUMN )
                                    );

            data_head_trace( out_head );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

/* ================================ private ================================ */

u8_error_t data_database_head_reader_private_open ( data_database_head_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    {
        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_HEAD_SELECT_HEAD_BY_ID,
                                                   sizeof( DATA_DATABASE_HEAD_SELECT_HEAD_BY_ID ),
                                                   &((*this_).statement_head_by_id)
                                                 );

        result |= data_database_prepare_statement( (*this_).database,
                                                   DATA_DATABASE_HEAD_SELECT_HEAD_BY_KEY,
                                                   sizeof( DATA_DATABASE_HEAD_SELECT_HEAD_BY_KEY ),
                                                   &((*this_).statement_head_by_key)
                                                 );

        if ( result != U8_ERROR_NONE )
        {
            U8_LOG_ERROR( "A prepared statement could not be prepared." );
        }
    }

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_head_reader_private_close ( data_database_head_reader_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    {
        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_head_by_id );
        (*this_).statement_head_by_id = NULL;

        result |= data_database_finalize_statement( (*this_).database, (*this_).statement_head_by_key );
        (*this_).statement_head_by_key = NULL;
    }

    U8_TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2024-2026 Andreas Warnke

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
