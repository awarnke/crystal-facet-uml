/* File: data_database_borrowed_stmt.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "u8/u8_trace.h"
#include <assert.h>

static inline void data_database_borrowed_stmt_init_void ( data_database_borrowed_stmt_t *this_ )
{
    (*this_).database = NULL;
    (*this_).db_statement = NULL;
    (*this_).borrow_flag = NULL;
}

static inline u8_error_t data_database_borrowed_stmt_init ( data_database_borrowed_stmt_t *this_,
                                                            data_database_t *database,
                                                            sqlite3_stmt *db_statement,
                                                            bool *borrow_flag )
{
    U8_TRACE_BEGIN();
    assert( database != NULL );
    assert( db_statement != NULL );
    assert( borrow_flag != NULL );
    u8_error_t result = U8_ERROR_NONE;

    (*this_).database = database;
    (*this_).db_statement = db_statement;
    (*this_).borrow_flag = borrow_flag;
    if ( *borrow_flag == true )
    {
        result = U8_ERROR_WRONG_STATE;
    }
    else
    {
        *((*this_).borrow_flag) = true;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline u8_error_t data_database_borrowed_stmt_destroy ( data_database_borrowed_stmt_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).borrow_flag != NULL )
    {
        assert( *((*this_).borrow_flag) == true ); /* noone interfered with the status */
        assert( (*this_).db_statement != NULL );

        const int sqlite_err = sqlite3_reset( (*this_).db_statement );
        if ( sqlite_err != SQLITE_OK )
        {
            U8_LOG_ERROR_INT( "sqlite3_reset() failed:", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }

        *((*this_).borrow_flag) = false;
        (*this_).borrow_flag = NULL;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

static inline bool data_database_borrowed_stmt_is_valid ( const data_database_borrowed_stmt_t *this_ )
{
    return ((*this_).database != NULL ) && ((*this_).db_statement != NULL ) && ((*this_).borrow_flag != NULL );
}

static inline sqlite3_stmt * data_database_borrowed_stmt_get_statement ( const data_database_borrowed_stmt_t *this_ )
{
    assert( (*this_).db_statement != NULL );
    return (*this_).db_statement;
}

static inline data_database_t * data_database_borrowed_stmt_get_database ( data_database_borrowed_stmt_t *this_ )
{
    assert( (*this_).database != NULL );
    return (*this_).database;
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
