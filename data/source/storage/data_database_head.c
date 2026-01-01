/* File: data_database_head.c; Copyright and License: see below */

#include "storage/data_database_head.h"
#include "entity/data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include <assert.h>

/*!
 *  \brief value separator string constant to insert a diagram or classifier or other table-row
 */
static const char *const DATA_DATABASE_HEAD_INSERT_VALUE_SEPARATOR = ",";

/*!
 *  \brief string start marker string constant to insert/update a diagram
 */
static const char *const DATA_DATABASE_HEAD_STRING_VALUE_START = "\'";

/*!
 *  \brief string end marker string constant to insert/update a diagram
 */
static const char *const DATA_DATABASE_HEAD_STRING_VALUE_END = "\'";

/*!
 *  \brief translation table to encode strings for usage in string literals
 *
 *  Note: This table is not suitable for searches using the LIKE operator because _ and % are not handled.
 */
const char *const (DATA_DATABASE_HEAD_SQL_ENCODE[][2]) = {
    { "'", "''" },
    { NULL, NULL }
};

void data_database_head_init ( data_database_head_t *this_, data_database_t *database )
{
    U8_TRACE_BEGIN();

    (*this_).database = database;

    /* initialize a memory output stream */
    universal_memory_output_stream_init( &((*this_).plain_out),
                                         &((*this_).private_sql_buffer),
                                         sizeof((*this_).private_sql_buffer),
                                         UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8
                                       );
    universal_output_stream_t *const plain_output
        = universal_memory_output_stream_get_output_stream( &((*this_).plain_out) );

    utf8stream_writer_init( &((*this_).plain), plain_output );

    /* initialize an sql escaped output stream */
    universal_escaping_output_stream_init( &((*this_).escaped_out),
                                           &DATA_DATABASE_HEAD_SQL_ENCODE,
                                           plain_output
                                         );
    universal_output_stream_t *const escaped_output
        = universal_escaping_output_stream_get_output_stream( &((*this_).escaped_out) );

    utf8stream_writer_init( &((*this_).escaped), escaped_output );

    U8_TRACE_END();
}

void data_database_head_destroy ( data_database_head_t *this_ )
{
    U8_TRACE_BEGIN();

    /* de-initialize an sql escaped output stream */
    utf8stream_writer_destroy( &((*this_).escaped) );
    universal_escaping_output_stream_destroy( &((*this_).escaped_out) );

    /* de-initialize an output stream */
    utf8stream_writer_destroy( &((*this_).plain) );
    universal_memory_output_stream_destroy( &((*this_).plain_out) );

    (*this_).database = NULL;

    U8_TRACE_END();
}

/*!
 *  \brief prefix search statement to find a head value by id
 */
static const char *const DATA_DATABASE_HEAD_SELECT_HEAD_BY_ID_PREFIX =
    "SELECT id,key,value FROM head WHERE id=";

/*!
 *  \brief postfix search statement to find a head value by id
 */
static const char *const DATA_DATABASE_HEAD_SELECT_HEAD_BY_ID_POSTFIX = ";";

u8_error_t data_database_head_read_value_by_id ( data_database_head_t *this_, data_row_t obj_id, data_head_t *out_head )
{
    U8_TRACE_BEGIN();
    assert( out_head != NULL );
    u8_error_t result = U8_ERROR_NONE;

    /* create an sql command */
    {
        result |= universal_memory_output_stream_reset( &((*this_).plain_out) );

        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_SELECT_HEAD_BY_ID_PREFIX );
        result |= utf8stream_writer_write_int( &((*this_).plain), obj_id );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_SELECT_HEAD_BY_ID_POSTFIX );

        result |= utf8stream_writer_flush( &((*this_).plain) );  /* enforces 0-termination on (*this_).plain_out */
    }

    if ( result == U8_ERROR_NONE )
    {
        const char *const sql_cmd = &((*this_).private_sql_buffer[0]);
        sqlite3_stmt *prepared_statement;
        int sqlite_err;

        result |= data_database_prepare_statement( (*this_).database,
                                                   sql_cmd,
                                                   utf8string_get_length( sql_cmd ) + sizeof( char ),
                                                   &prepared_statement
                                                 );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO_INT( "sqlite3_step did not find a row for id", obj_id );
            result |= U8_ERROR_NOT_FOUND;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_head_init( out_head,
                                      sqlite3_column_int64( prepared_statement, 0 ),
                                      (const char*) sqlite3_column_text( prepared_statement, 1 ),
                                      (const char*) sqlite3_column_text( prepared_statement, 2 )
                                    );

            data_head_trace( out_head );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step not done yet:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }

        result |= data_database_finalize_statement( (*this_).database, prepared_statement );
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

/*!
 *  \brief prefix search statement to find a head value by key
 */
static const char *const DATA_DATABASE_HEAD_SELECT_HEAD_BY_KEY_PREFIX =
    "SELECT id,key,value FROM head WHERE key=";

/*!
 *  \brief postfix search statement to find a head value by key
 */
static const char *const DATA_DATABASE_HEAD_SELECT_HEAD_BY_KEY_POSTFIX = ";";

u8_error_t data_database_head_read_value_by_key ( data_database_head_t *this_, const char *key, data_head_t *out_head )
{
    U8_TRACE_BEGIN();
    assert( key != NULL );
    assert( out_head != NULL );
    u8_error_t result = U8_ERROR_NONE;

    /* create an sql command */
    {
        result |= universal_memory_output_stream_reset( &((*this_).plain_out) );

        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_SELECT_HEAD_BY_KEY_PREFIX );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_STRING_VALUE_START );
        result |= utf8stream_writer_flush( &((*this_).plain) );
        result |= utf8stream_writer_write_str( &((*this_).escaped), key );
        result |= utf8stream_writer_flush( &((*this_).escaped) );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_STRING_VALUE_END );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_SELECT_HEAD_BY_KEY_POSTFIX );

        result |= utf8stream_writer_flush( &((*this_).plain) );  /* enforces 0-termination on (*this_).plain_out */
    }

    if ( result == U8_ERROR_NONE )
    {
        const char *const sql_cmd = &((*this_).private_sql_buffer[0]);
        sqlite3_stmt *prepared_statement;
        int sqlite_err;

        result |= data_database_prepare_statement( (*this_).database,
                                                   sql_cmd,
                                                   utf8string_get_length( sql_cmd ) + sizeof( char ),
                                                   &prepared_statement
                                                 );

        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            /* Do not log this incident, the caller may not expect to find a row. */
            U8_TRACE_INFO_STR( "sqlite3_step did not find a row for key", key );
            result |= U8_ERROR_NOT_FOUND;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_head_init( out_head,
                                      sqlite3_column_int64( prepared_statement, 0 ),
                                      (const char*) sqlite3_column_text( prepared_statement, 1 ),
                                      (const char*) sqlite3_column_text( prepared_statement, 2 )
                                    );

            data_head_trace( out_head );

            sqlite_err = sqlite3_step( prepared_statement );
            if ( SQLITE_DONE != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_step not done yet:", sqlite_err );
                result |= U8_ERROR_DB_STRUCTURE;
            }
        }

        result |= data_database_finalize_statement( (*this_).database, prepared_statement );
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

/*!
 *  \brief prefix string constant to insert a head value
 */
static const char *const DATA_DATABASE_HEAD_INSERT_HEAD_PREFIX =
    "INSERT INTO head (key,value) VALUES (";

/*!
 *  \brief postfix string constant to insert a head value
 */
static const char *const DATA_DATABASE_HEAD_INSERT_HEAD_POSTFIX = ");";

u8_error_t data_database_head_create_value ( data_database_head_t *this_, const data_head_t *head, data_row_t* out_new_id )
{
    U8_TRACE_BEGIN();
    assert( head != NULL );
    u8_error_t result = U8_ERROR_NONE;

    /* create an sql command */
    {
        result |= universal_memory_output_stream_reset( &((*this_).plain_out) );

        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_INSERT_HEAD_PREFIX );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_STRING_VALUE_START );
        result |= utf8stream_writer_flush( &((*this_).plain) );
        utf8string_t *const key = data_head_get_key_const( head );
        result |= utf8stream_writer_write_str( &((*this_).escaped), key );
        result |= utf8stream_writer_flush( &((*this_).escaped) );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_STRING_VALUE_END );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_INSERT_VALUE_SEPARATOR );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_STRING_VALUE_START );
        result |= utf8stream_writer_flush( &((*this_).plain) );
        utf8string_t *const value = data_head_get_value_const( head );
        result |= utf8stream_writer_write_str( &((*this_).escaped), value );
        result |= utf8stream_writer_flush( &((*this_).escaped) );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_STRING_VALUE_END );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_INSERT_HEAD_POSTFIX );

        result |= utf8stream_writer_flush( &((*this_).plain) );  /* enforces 0-termination on (*this_).plain_out */
    }

    if ( result == U8_ERROR_NONE )
    {
        const char *const sql_cmd = &((*this_).private_sql_buffer[0]);
        data_row_t new_id = DATA_ROW_VOID;

        result |= data_database_transaction_begin ( (*this_).database );
        result |= data_database_in_transaction_create( (*this_).database, sql_cmd, &new_id );
        result |= data_database_transaction_commit ( (*this_).database );

        U8_LOG_EVENT_INT( "sqlite3_exec: INSERT INTO head ... ->", new_id );  /* do not log confidential information, only id */
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

/*!
 *  \brief prefix string constant to delete a head value
 */
static const char *const ATA_DATABASE_HEAD_DELETE_HEAD_PREFIX =
    "DELETE FROM head WHERE (id=";

/*!
 *  \brief postfix string constant to delete a head value
 */
static const char *const DATA_DATABASE_HEAD_DELETE_HEAD_POSTFIX = ");";

u8_error_t data_database_head_delete_value ( data_database_head_t *this_, data_row_t obj_id, data_head_t *out_old_head )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_transaction_begin ( (*this_).database );
    /* Note: out_old_head is NULL if old data shall not be returned */
    if ( NULL != out_old_head )
    {
        result |= data_database_head_read_value_by_id ( this_, obj_id, out_old_head );
    }

    /* create an sql command AFTER(!) reading the old value, same stringbuffer is used. */
    {
        result |= universal_memory_output_stream_reset( &((*this_).plain_out) );

        result |= utf8stream_writer_write_str( &((*this_).plain), ATA_DATABASE_HEAD_DELETE_HEAD_PREFIX );
        result |= utf8stream_writer_write_int( &((*this_).plain), obj_id );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_DELETE_HEAD_POSTFIX );

        result |= utf8stream_writer_flush( &((*this_).plain) );  /* enforces 0-termination on (*this_).plain_out */
    }

    if ( result == U8_ERROR_NONE )
    {
        const char *const sql_cmd = &((*this_).private_sql_buffer[0]);

        result |= data_database_in_transaction_execute( (*this_).database, sql_cmd );
        U8_LOG_EVENT_INT( "sqlite3_exec: DELETE FROM head ... ->", obj_id );  /* do not log confidential information, only id */
    }

    result |= data_database_transaction_commit ( (*this_).database );

    U8_TRACE_END_ERR( result );
    return( result );
}

/*!
 *  \brief prefix string constant to update a head value
 */
static const char *DATA_DATABASE_HEAD_UPDATE_HEAD_PREFIX = "UPDATE head SET value=";

/*!
 *  \brief infix string constant to update a head value
 */
static const char *DATA_DATABASE_HEAD_UPDATE_HEAD_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a head value
 */
static const char *DATA_DATABASE_HEAD_UPDATE_HEAD_POSTFIX = ";";

u8_error_t data_database_head_update_value ( data_database_head_t *this_, data_row_t head_id, const char* new_head_value, data_head_t *out_old_head )
{
    U8_TRACE_BEGIN();
    assert( new_head_value != NULL );
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_transaction_begin ( (*this_).database );
    /* Note: out_old_head is NULL if old data shall not be returned */
    if ( NULL != out_old_head )
    {
        result |= data_database_head_read_value_by_id ( this_, head_id, out_old_head );
    }

    /* create an sql command AFTER(!) reading the old value, same stringbuffer is used. */
    {
        result |= universal_memory_output_stream_reset( &((*this_).plain_out) );

        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_UPDATE_HEAD_PREFIX );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_STRING_VALUE_START );
        result |= utf8stream_writer_flush( &((*this_).plain) );
        result |= utf8stream_writer_write_str( &((*this_).escaped), new_head_value );
        result |= utf8stream_writer_flush( &((*this_).escaped) );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_STRING_VALUE_END );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_UPDATE_HEAD_INFIX );
        result |= utf8stream_writer_write_int( &((*this_).plain), head_id );
        result |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_HEAD_UPDATE_HEAD_POSTFIX );

        result |= utf8stream_writer_flush( &((*this_).plain) );  /* enforces 0-termination on (*this_).plain_out */
    }

    if ( result == U8_ERROR_NONE )
    {
        const char *const sql_cmd = &((*this_).private_sql_buffer[0]);

        result |= data_database_in_transaction_execute( (*this_).database, sql_cmd );
        U8_LOG_EVENT_INT( "sqlite3_exec: UPDATE head ... ->", head_id );  /* do not log confidential information, only id */
    }

    result |= data_database_transaction_commit ( (*this_).database );

    U8_TRACE_END_ERR( result );
    return( result );
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
