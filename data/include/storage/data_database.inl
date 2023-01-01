/* File: data_database.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include <assert.h>

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

static inline sqlite3 *data_database_get_database_ptr ( data_database_t *this_ )
{
    return (*this_).db;
}

static inline data_change_notifier_t *data_database_get_notifier_ptr ( data_database_t *this_ )
{
    return &((*this_).notifier);
}

static inline const char *data_database_get_filename_ptr ( data_database_t *this_ )
{
    return ( data_database_is_open( this_ ) ? utf8stringbuf_get_string( (*this_).db_file_name ) : NULL );
}

static inline void data_database_private_clear_db_listener_list( data_database_t *this_ )
{
    for( int index = 0; index < DATA_DATABASE_MAX_LISTENERS; index ++ )
    {
        (*this_).listener_list[index] = NULL;
    }
}

static inline u8_error_t data_database_private_lock ( data_database_t *this_ )
{
    u8_error_t result = U8_ERROR_NONE;

    g_mutex_lock ( &((*this_).private_lock) );

    return result;
}

static inline u8_error_t data_database_private_unlock ( data_database_t *this_ )
{
    u8_error_t result = U8_ERROR_NONE;

    g_mutex_unlock ( &((*this_).private_lock) );

    return result;
}

static inline bool data_database_is_open( data_database_t *this_ )
{
    bool result;
    u8_error_t locking_error;
    locking_error = data_database_private_lock( this_ );
    result = (*this_).db_state != DATA_DATABASE_STATE_CLOSED;
    locking_error |= data_database_private_unlock( this_ );
    return result;
}

static inline u8_error_t data_database_private_exec_sql( data_database_t *this_, const char* sql_command, bool ignore_errors )
{
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
    return result;
}


/*
Copyright 2016-2023 Andreas Warnke

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
