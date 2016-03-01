/* File: data_database_writer.c; Copyright and License: see below */

#include "data_database_writer.h"
#include "trace.h"
#include "log.h"
#include <sqlite3.h>

/*!
 *  \brief string constant to start a transaction
 * 
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_WRITER_BEGIN_TRANSACTION = 
    "BEGIN TRANSACTION;";
    
/*!
 *  \brief string constant to commit a transaction
 * 
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_WRITER_COMMIT_TRANSACTION = 
    "COMMIT TRANSACTION;";
    
/*!
 *  \brief string constant to insert a diagram
 * 
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_PREFIX = 
    "INSERT INTO diagrams (parent_id,type,name,list_order) VALUES (-1,2,\"Hello\",42);";
    
void data_database_writer_init ( data_database_writer_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    int perr;
    
    (*this_).database = database;
    (*this_).private_temp_stringbuf = utf8stringbuf_init( sizeof((*this_).private_temp_buffer), (*this_).private_temp_buffer );
    (*this_).private_sql_stringbuf = utf8stringbuf_init( sizeof((*this_).private_sql_buffer), (*this_).private_sql_buffer );
    perr = pthread_mutex_init ( &((*this_).private_lock), NULL );
    if ( perr != 0 ) 
    {
        LOG_ERROR_INT( "pthread_mutex_init() failed:", perr );
    }
    
    TRACE_END();
}

void data_database_writer_destroy ( data_database_writer_t *this_ )
{
    TRACE_BEGIN();
    int perr;
    
    perr = pthread_mutex_destroy ( &((*this_).private_lock) );
    if ( perr != 0 ) 
    {
        LOG_ERROR_INT( "pthread_mutex_destroy() failed:", perr );
    }
    
    TRACE_END();
}

int32_t data_database_writer_create_diagram ( data_database_writer_t *this_, const data_diagram_t *diagram )
{
    TRACE_BEGIN();
    int32_t result = 0;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database( (*this_).database );
    int perr;
    utf8error_t strerr;
    
    perr = pthread_mutex_lock ( &((*this_).private_lock) );
    if ( perr != 0 ) 
    {
        LOG_ERROR_INT( "pthread_mutex_lock() failed:", perr );
    }
    
    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_BEGIN_TRANSACTION, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    utf8stringbuf_clear( (*this_).private_temp_stringbuf );
    strerr = UTF8ERROR_SUCCESS;
    if ( strerr != UTF8ERROR_SUCCESS ) 
    {
        LOG_ERROR_INT( "utf8stringbuf_clear() failed:", perr );
    }
    
    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_INSERT_DIAGRAM_PREFIX );
    
    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_COMMIT_TRANSACTION, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err ) 
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }
    
    perr = pthread_mutex_unlock ( &((*this_).private_lock) );
    if ( perr != 0 ) 
    {
        LOG_ERROR_INT( "pthread_mutex_unlock() failed:", perr );
    }
    
    TRACE_END();
    return result;
}


/*
Copyright 2016-2016 Andreas Warnke

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
