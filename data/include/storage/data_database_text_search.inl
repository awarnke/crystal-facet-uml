/* File: data_database_text_search.inl; Copyright and License: see below */

#include "u8/u8_log.h"
#include "u8/u8_trace.h"
#include <assert.h>

static inline bool data_database_text_search_is_open( data_database_text_search_t *this_ )
{
    bool result;
    result = (*this_).is_open;
    return result;
}

/* ================================ private ================================ */

#if 0
static inline u8_error_t data_database_text_search_private_bind_two_texts_to_statement ( data_database_text_search_t *this_,
                                                                                         sqlite3_stmt *statement_ptr,
                                                                                         const char *text_1,
                                                                                         const char *text_2 )
{
    assert( NULL != statement_ptr );
    assert( NULL != text_1 );
    assert( NULL != text_2 );
    u8_error_t result = U8_ERROR_NONE;
    static const int FIRST_SQL_BIND_PARAM = 1;
    static const int SECOND_SQL_BIND_PARAM = 2;
    int sqlite_err;

    sqlite_err = sqlite3_reset( statement_ptr );
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_INT( "sqlite3_reset() failed:", sqlite_err );
        result |= U8_ERROR_AT_DB;
    }

    U8_TRACE_INFO_STR( "sqlite3_bind_text():", sqlite3_sql(statement_ptr) );
    /* SQLITE_STATIC vs SQLITE_TRANSIENT: This function is used to perform a SELECT statement. */
    /* During the SELECT, the text string is not modified. This is guaranteed by data_database_text_search. */
    U8_TRACE_INFO_STR( "sqlite3_bind_text():", text_1 );
    sqlite_err = sqlite3_bind_text( statement_ptr, FIRST_SQL_BIND_PARAM, text_1, -1, SQLITE_STATIC );
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_INT( "sqlite3_bind_text() failed:", sqlite_err );
        result |= U8_ERROR_AT_DB;
    }
    U8_TRACE_INFO_STR( "sqlite3_bind_text():", text_2 );
    sqlite_err = sqlite3_bind_text( statement_ptr, SECOND_SQL_BIND_PARAM, text_2, -1, SQLITE_STATIC );
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_INT( "sqlite3_bind_text() failed:", sqlite_err );
        result |= U8_ERROR_AT_DB;
    }

    return result;
}
#endif

static inline u8_error_t data_database_text_search_private_bind_three_texts_to_statement ( data_database_text_search_t *this_,
                                                                                           sqlite3_stmt *statement_ptr,
                                                                                           const char *text_1,
                                                                                           const char *text_2,
                                                                                           const char *text_3 )
{
    assert( NULL != statement_ptr );
    assert( NULL != text_1 );
    assert( NULL != text_2 );
    assert( NULL != text_3 );
    u8_error_t result = U8_ERROR_NONE;
    static const int FIRST_SQL_BIND_PARAM = 1;
    static const int SECOND_SQL_BIND_PARAM = 2;
    static const int THIRD_SQL_BIND_PARAM = 3;
    int sqlite_err;

    sqlite_err = sqlite3_reset( statement_ptr );
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_INT( "sqlite3_reset() failed:", sqlite_err );
        result |= U8_ERROR_AT_DB;
    }

    U8_TRACE_INFO_STR( "sqlite3_bind_text():", sqlite3_sql(statement_ptr) );
    /* SQLITE_STATIC vs SQLITE_TRANSIENT: This function is used to perform a SELECT statement. */
    /* During the SELECT, the text string is not modified. This is guaranteed by data_database_text_search. */
    U8_TRACE_INFO_STR( "sqlite3_bind_text():", text_1 );
    sqlite_err = sqlite3_bind_text( statement_ptr, FIRST_SQL_BIND_PARAM, text_1, -1, SQLITE_STATIC );
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_INT( "sqlite3_bind_text() failed:", sqlite_err );
        result |= U8_ERROR_AT_DB;
    }
    U8_TRACE_INFO_STR( "sqlite3_bind_text():", text_2 );
    sqlite_err = sqlite3_bind_text( statement_ptr, SECOND_SQL_BIND_PARAM, text_2, -1, SQLITE_STATIC );
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_INT( "sqlite3_bind_text() failed:", sqlite_err );
        result |= U8_ERROR_AT_DB;
    }
    U8_TRACE_INFO_STR( "sqlite3_bind_text():", text_3 );
    sqlite_err = sqlite3_bind_text( statement_ptr, THIRD_SQL_BIND_PARAM, text_3, -1, SQLITE_STATIC );
    if ( SQLITE_OK != sqlite_err )
    {
        U8_LOG_ERROR_INT( "sqlite3_bind_text() failed:", sqlite_err );
        result |= U8_ERROR_AT_DB;
    }

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
