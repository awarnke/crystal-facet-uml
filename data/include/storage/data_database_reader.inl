/* File: data_database_reader.inl; Copyright and License: see below */

#include "tslog.h"
#include "trace.h"
#include <assert.h>

static inline bool data_database_reader_is_open( data_database_reader_t *this_ )
{
    bool result;
    data_error_t locking_error;
    locking_error = data_database_reader_private_lock( this_ );
    result = (*this_).is_open;
    locking_error |= data_database_reader_private_unlock( this_ );
    return result;
}

/* ================================ private ================================ */

static inline data_error_t data_database_reader_private_lock ( data_database_reader_t *this_ )
{
    data_error_t result = DATA_ERROR_NONE;
    int perr;

    perr = pthread_mutex_lock ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        TSLOG_ERROR_INT( "pthread_mutex_lock() failed:", perr );
        result = DATA_ERROR_AT_MUTEX;
    }

    return result;
}

static inline data_error_t data_database_reader_private_unlock ( data_database_reader_t *this_ )
{
    data_error_t result = DATA_ERROR_NONE;
    int perr;

    perr = pthread_mutex_unlock ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        TSLOG_ERROR_INT( "pthread_mutex_unlock() failed:", perr );
        result = DATA_ERROR_AT_MUTEX;
    }

    return result;
}

static inline data_error_t data_database_reader_private_prepare_statement ( data_database_reader_t *this_,
                                                                            const char *string_statement,
                                                                            int string_size,
                                                                            sqlite3_stmt **out_statement_ptr )
{
    assert( NULL != string_statement );
    assert( NULL != out_statement_ptr );
    data_error_t result = DATA_ERROR_NONE;
    const char *first_unused_statement_char;
    int sqlite_err;
    sqlite3 *db;

    db = data_database_get_database_ptr ( (*this_).database );

    TRACE_INFO_STR( "sqlite3_prepare_v2():", string_statement );
    sqlite_err = sqlite3_prepare_v2( db,
                                     string_statement,
                                     string_size,
                                     out_statement_ptr,
                                     &first_unused_statement_char
                                   );
    if (( SQLITE_OK != sqlite_err )
        || ( first_unused_statement_char != &(string_statement[string_size-1]) ))
    {
        TSLOG_ERROR_STR( "sqlite3_prepare_v2() failed:", string_statement );
        TSLOG_ERROR_INT( "sqlite3_prepare_v2() failed:", sqlite_err );
        TSLOG_ERROR_STR( "sqlite3_prepare_v2() failed:", sqlite3_errmsg( db ) );
        result |= DATA_ERROR_AT_DB;
    }

    return result;
}

static inline data_error_t data_database_reader_private_finalize_statement ( data_database_reader_t *this_, sqlite3_stmt *statement_ptr )
{
    assert( NULL != statement_ptr );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;

    TRACE_INFO_STR( "sqlite3_finalize():", sqlite3_sql(statement_ptr) );
    sqlite_err = sqlite3_finalize( statement_ptr );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_STR( "sqlite3_finalize() failed:", sqlite3_sql(statement_ptr) );
        TSLOG_ERROR_INT( "sqlite3_finalize() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }

    return result;
}

static inline data_error_t data_database_reader_private_bind_void_to_statement ( data_database_reader_t *this_,
                                                                                 sqlite3_stmt *statement_ptr )
{
    assert( NULL != statement_ptr );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;

    sqlite_err = sqlite3_reset( statement_ptr );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_INT( "sqlite3_reset() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }

    return result;
}

static inline data_error_t data_database_reader_private_bind_id_to_statement ( data_database_reader_t *this_,
                                                                               sqlite3_stmt *statement_ptr,
                                                                               int64_t id )
{
    assert( NULL != statement_ptr );
    data_error_t result = DATA_ERROR_NONE;
    static const int FIRST_SQL_BIND_PARAM = 1;
    int sqlite_err;

    sqlite_err = sqlite3_reset( statement_ptr );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_INT( "sqlite3_reset() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }

    TRACE_INFO_STR( "sqlite3_bind_int64():", sqlite3_sql(statement_ptr) );
    TRACE_INFO_INT( "sqlite3_bind_int64():", id );
    sqlite_err = sqlite3_bind_int64( statement_ptr, FIRST_SQL_BIND_PARAM, id );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_INT( "sqlite3_bind_int64() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }

    return result;
}

static inline data_error_t data_database_reader_private_bind_two_ids_to_statement ( data_database_reader_t *this_,
                                                                                    sqlite3_stmt *statement_ptr,
                                                                                    int64_t id1,
                                                                                    int64_t id2 )
{
    assert( NULL != statement_ptr );
    data_error_t result = DATA_ERROR_NONE;
    static const int FIRST_SQL_BIND_PARAM = 1;
    static const int SECOND_SQL_BIND_PARAM = 2;
    int sqlite_err;

    sqlite_err = sqlite3_reset( statement_ptr );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_INT( "sqlite3_reset() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }

    TRACE_INFO_STR( "sqlite3_bind_int64():", sqlite3_sql(statement_ptr) );
    TRACE_INFO_INT_INT( "sqlite3_bind_int64():", id1, id2 );
    sqlite_err = sqlite3_bind_int64( statement_ptr, FIRST_SQL_BIND_PARAM, id1 );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_INT( "sqlite3_bind_int64() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }
    sqlite_err = sqlite3_bind_int64( statement_ptr, SECOND_SQL_BIND_PARAM, id2 );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_INT( "sqlite3_bind_int64() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }

    return result;
}

static inline data_error_t data_database_reader_private_bind_text_to_statement ( data_database_reader_t *this_,
                                                                                 sqlite3_stmt *statement_ptr,
                                                                                 const char *text )
{
    assert( NULL != statement_ptr );
    assert( NULL != text );
    data_error_t result = DATA_ERROR_NONE;
    static const int FIRST_SQL_BIND_PARAM = 1;
    int sqlite_err;

    sqlite_err = sqlite3_reset( statement_ptr );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_INT( "sqlite3_reset() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }

    TRACE_INFO_STR( "sqlite3_bind_text():", sqlite3_sql(statement_ptr) );
    TRACE_INFO_STR( "sqlite3_bind_text():", text );
    /* SQLITE_STATIC vs SQLITE_TRANSIENT: This function is used to perform a SELECT statement. */
    /* During the SELECT, the text string is not modified. This is guaranteed by data_database_reader. */
    sqlite_err = sqlite3_bind_text( statement_ptr, FIRST_SQL_BIND_PARAM, text, -1, SQLITE_STATIC );
    if ( SQLITE_OK != sqlite_err )
    {
        TSLOG_ERROR_INT( "sqlite3_bind_text() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }

    return result;
}


/*
Copyright 2016-2019 Andreas Warnke

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
