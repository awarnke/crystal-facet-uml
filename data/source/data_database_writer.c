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
 *  \brief prefix string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_PREFIX =
    "INSERT INTO diagrams (parent_id,type,name,description,list_order) VALUES (";

/*!
 *  \brief value separator string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR = ",";

/*!
 *  \brief string start marker string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_STRING_START = "\'";

/*!
 *  \brief string end marker string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_STRING_END = "\'";

/*!
 *  \brief postfix string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_POSTFIX = ");";

/*!
 *  \brief translation table to encode strings for usage in string literals
 *
 *  Note: This table is not suitable for searches using the LIKE operator because _ and % are not handled.
 */
const char *const DATA_DATABASE_WRITER_SQL_ENCODE[] = {
    "\x09", "\\t",
    "\x0a", "\\n",
    "\x0d", "\\r",
    "\x0e", "\\b",
    "\x1a", "\\z",
    "'", "''",
    "\\", "\\\\",
    NULL,
};

/*!
 *  \brief builds the sql command string to create a new diagram record. The result is stored in (*this_).private_sql_stringbuf.
 *
 *  \param diagram diagram to be created.
 */
static void data_database_writer_private_build_create_diagram_command ( data_database_writer_t *this_, const data_diagram_t *diagram );

/*   !
 * \brief callback to trace database results
 */
/*
static int data_database_writer_private_trace_sql_result( void *my_data, int num, char** a, char** b );
*/

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

static void data_database_writer_private_build_create_diagram_command ( data_database_writer_t *this_, const data_diagram_t *diagram )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).parent_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).diagram_type );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_STRING_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*diagram).name );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_STRING_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_STRING_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*diagram).description );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_STRING_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).parent_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
    }

    TRACE_END();
}

int64_t data_database_writer_create_diagram ( data_database_writer_t *this_, const data_diagram_t *diagram )
{
    TRACE_BEGIN();
    int64_t result = DATA_DIAGRAM_ID_UNINITIALIZED_ID;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database( (*this_).database );
    int perr;

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

    data_database_writer_private_build_create_diagram_command( this_, diagram );
    LOG_EVENT_STR( "sqlite3_exec:", utf8stringbuf_get_string( (*this_).private_sql_stringbuf ) );
    sqlite_err = sqlite3_exec( db, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), NULL, NULL, &error_msg );
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

    if ( SQLITE_OK == sqlite_err )
    {
        result = sqlite3_last_insert_rowid(db);
        LOG_EVENT_INT( "sqlite3_last_insert_rowid():", result );
    }

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
static int data_database_writer_private_trace_sql_result( void *my_data, int num, char** a, char** b )
{
    TRACE_BEGIN();
    int result = 0;

    TRACE_INFO_INT( "num:", num );

    TRACE_END();
    return result;
}
*/

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
