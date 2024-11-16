/* File: data_database_borrowed_stmt.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "u8/u8_trace.h"
#include <assert.h>

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
    assert( *((*this_).borrow_flag) == true ); /* noone interfered with the status */
    *((*this_).borrow_flag) = false;
    u8_error_t result = U8_ERROR_NONE;
    U8_TRACE_END_ERR( result );
    return result;
}

static inline sqlite3_stmt * data_database_borrowed_stmt_get_statement ( const data_database_borrowed_stmt_t *this_ )
{
    return (*this_).db_statement;
}

static inline data_database_t * data_database_borrowed_stmt_get_database ( data_database_borrowed_stmt_t *this_ )
{
    return (*this_).database;
}


/*
Copyright 2024-2024 Andreas Warnke

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
