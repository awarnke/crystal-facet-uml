/* File: data_database.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

/* ================================ Lifecycle ================================ */

static inline u8_error_t data_database_open ( data_database_t *this_, const char* db_file_path )
{
    assert( db_file_path != NULL );
    /* there should not be pending transactions when calling open */
    assert( (*this_).transaction_recursion == 0 );

    const u8_error_t err
        = data_database_private_open( this_, db_file_path, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE );
    return err;
}

static inline u8_error_t data_database_open_read_only ( data_database_t *this_, const char* db_file_path )
{
    assert( db_file_path != NULL );
    /* there should not be pending transactions when calling open */
    assert( (*this_).transaction_recursion == 0 );

    const u8_error_t err
        = data_database_private_open( this_, db_file_path, SQLITE_OPEN_READONLY );
    return err;
}

static inline u8_error_t data_database_open_in_memory ( data_database_t *this_ )
{
    /* there should not be pending transactions when calling open */
    assert( (*this_).transaction_recursion == 0 );

    const char* const IN_MEMORY_FILENAME = ":memory:"; /* magic filename, see https://www.sqlite.org/c3ref/open.html */
    const u8_error_t err
        = data_database_private_open( this_,
                                      IN_MEMORY_FILENAME,
                                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY
                                    );
    return err;
}

static inline bool data_database_is_open( data_database_t *this_ )
{
    bool result;
    u8_error_t locking_error;
    locking_error = data_database_lock_on_write( this_ );
    result = (*this_).db_state != DATA_DATABASE_STATE_CLOSED;
    locking_error |= data_database_unlock_on_write( this_ );
    assert( locking_error == U8_ERROR_NONE );
    (void) locking_error;  /* this should not happen in RELEASE mode */
    return result;
}

/* ================================ Actions on DB ================================ */

static inline sqlite3 *data_database_get_database_ptr ( data_database_t *this_ )
{
    return (*this_).db;
}

static inline u8_error_t data_database_private_exec_sql( data_database_t *this_, const char* sql_command, bool ignore_errors )
{
    U8_TRACE_BEGIN();
    assert( sql_command != NULL );
    int sqlite_err;
    char *error_msg = NULL;
    u8_error_t result = U8_ERROR_NONE;
    sqlite3 *const db = (*this_).db;

    U8_LOG_EVENT_STR( "sqlite3_exec:", sql_command );
    sqlite_err = sqlite3_exec( db, sql_command, NULL, NULL, &error_msg );

    if ( SQLITE_READONLY == sqlite_err )
    {
        if ( ! ignore_errors )
        {
            U8_LOG_WARNING_STR( "sqlite3_exec() failed:", sql_command );
        }
        result |=  U8_ERROR_READ_ONLY_DB;
    }
    else if ( SQLITE_OK != sqlite_err )
    {
        if ( ! ignore_errors )
        {
            U8_LOG_ERROR_STR( "sqlite3_exec() failed:", sql_command );
            U8_LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
        }
        result |= U8_ERROR_AT_DB;
    }
    if ( error_msg != NULL )
    {
        U8_TRACE_INFO_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_prepare_statement ( data_database_t *this_,
                                                           const char *string_statement,
                                                           int string_size,
                                                           sqlite3_stmt **out_statement_ptr )
{
    U8_TRACE_BEGIN();
    assert( NULL != string_statement );
    assert( NULL != out_statement_ptr );
    u8_error_t result = U8_ERROR_NONE;
    const char *first_unused_statement_char;
    int sqlite_err;
    sqlite3 *const db = (*this_).db;

    U8_TRACE_INFO_STR( "sqlite3_prepare_v2():", string_statement );
    sqlite_err = sqlite3_prepare_v2( db,
                                     string_statement,
                                     string_size,
                                     out_statement_ptr,
                                     &first_unused_statement_char
                                   );
    if (( string_size != DATA_DATABASE_SQL_LENGTH_AUTO_DETECT )
        && ( first_unused_statement_char != &(string_statement[string_size-1]) ))
    {
        U8_LOG_ERROR_INT( "sqlite3_prepare_v2() consumed wrong amount of characters, expected:", string_size-1 );
        result |= U8_ERROR_AT_DB;
    }
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_STR( "sqlite3_prepare_v2() failed:", string_statement );
        U8_LOG_ERROR_INT( "sqlite3_prepare_v2() failed:", sqlite_err );
        U8_LOG_ERROR_STR( "sqlite3_prepare_v2() failed:", sqlite3_errmsg( db ) );
        result |= U8_ERROR_AT_DB;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_finalize_statement ( data_database_t *this_, sqlite3_stmt *statement_ptr )
{
    U8_TRACE_BEGIN();
    assert( NULL != statement_ptr );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;

    U8_TRACE_INFO_STR( "sqlite3_finalize():", sqlite3_sql(statement_ptr) );
    sqlite_err = sqlite3_finalize( statement_ptr );
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_STR( "sqlite3_finalize() failed:", sqlite3_sql(statement_ptr) );
        U8_LOG_ERROR_INT( "sqlite3_finalize() failed:", sqlite_err );
        result |= U8_ERROR_AT_DB;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

/* ================================ Information ================================ */

static inline data_revision_t data_database_get_revision ( data_database_t *this_ )
{
    data_revision_t result;
    u8_error_t locking_error;
    locking_error = data_database_lock_on_write( this_ );
    result = (*this_).revision;
    locking_error |= data_database_unlock_on_write( this_ );
    assert( locking_error == U8_ERROR_NONE );
    (void) locking_error;  /* ignore errors at lock */
    return result;
}

static inline void data_database_set_revision ( data_database_t *this_, data_revision_t revision )
{
    U8_TRACE_BEGIN();
    u8_error_t locking_error;
    locking_error = data_database_lock_on_write( this_ );
    (*this_).revision = revision;
    locking_error |= data_database_unlock_on_write( this_ );
    assert( locking_error == U8_ERROR_NONE );
    (void) locking_error;  /* ignore errors at lock */

    U8_LOG_EVENT_INT( "revision:", revision );

    /* notify listeners */
    data_change_notifier_emit_signal_without_parent( &((*this_).notifier),
                                                     DATA_CHANGE_EVENT_TYPE_REVISION_CHANGED,
                                                     DATA_TABLE_VOID,
                                                     DATA_ROW_VOID
                                                   );
    U8_TRACE_END();
}

static inline const char *data_database_get_filename_ptr ( data_database_t *this_ )
{
    return ( data_database_is_open( this_ ) ? utf8stringbuf_get_string( &((*this_).db_file_name) ) : NULL );
}

/* ================================ Change Listener ================================ */

static inline data_change_notifier_t *data_database_get_notifier_ptr ( data_database_t *this_ )
{
    return &((*this_).notifier);
}

static inline void data_database_private_clear_db_listener_list( data_database_t *this_ )
{
    for( int index = 0; index < DATA_DATABASE_MAX_LISTENERS; index ++ )
    {
        (*this_).listener_list[index] = NULL;
    }
}

/* ================================ Lifecycle Lock ================================ */

static inline u8_error_t data_database_lock_on_write ( data_database_t *this_ )
{
    assert( (*this_).locked_on_write == false );
    u8_error_t result = U8_ERROR_NONE;

    g_mutex_lock ( &((*this_).lock_on_write) );
    (*this_).locked_on_write = true;

    return result;
}

static inline u8_error_t data_database_unlock_on_write ( data_database_t *this_ )
{
    assert( (*this_).locked_on_write == true );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).locked_on_write = false;
    g_mutex_unlock ( &((*this_).lock_on_write) );

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
