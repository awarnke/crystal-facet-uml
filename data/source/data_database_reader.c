/* File: data_database_reader.c; Copyright and License: see below */

#include "data_database_reader.h"
#include "trace.h"
#include "log.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>

/*!
 *  \brief predefined search statement find a diagram by id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID[] =
    "SELECT id,parent_id,type,name,description,list_order FROM diagrams WHERE id=?;";

/*!
 *  \brief predefined search statement find diagrams by parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID[] =
    "SELECT id,parent_id,type,name,description,list_order FROM diagrams WHERE parent_id=? ORDER BY list_order;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_COLUMN_ID = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: parent_id
 */
static const int RESULT_COLUMN_PARENT_ID = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: type
 */
static const int RESULT_COLUMN_TYPE = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_COLUMN_NAME = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_COLUMN_DESCRIPTION = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_COLUMN_LIST_ORDER = 5;

/*!
 * \brief callback to trace database results
 */
static int data_database_reader_private_trace_sql_result( void *my_data, int num, char** a, char** b );

void data_database_reader_init ( data_database_reader_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    int sqlite_err;
    const char *unused;
    int perr;
    sqlite3 *db;

    (*this_).database = database;
    db = data_database_get_database ( database );

    perr = pthread_mutex_init ( &((*this_).private_lock), NULL );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_init() failed:", perr );
    }

    TRACE_INFO_STR( "sqlite3_prepare_v2():", DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID );
    sqlite_err = sqlite3_prepare_v2(
        db,
        DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID,
        sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID ),
        &((*this_).private_prepared_query_diagram_by_id),
        &unused
    );
    if (( SQLITE_OK != sqlite_err )
        || ( unused != &(DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID[sizeof(DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID)-1]) ))
    {
        LOG_ERROR_STR( "sqlite3_prepare_v2() failed:", DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID );
        LOG_ERROR_INT( "sqlite3_prepare_v2() failed:", sqlite_err );
        LOG_ERROR_STR( "sqlite3_prepare_v2() failed:", sqlite3_errmsg( db ) );
    }

    TRACE_INFO_STR( "sqlite3_prepare_v2():", DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID );
    sqlite_err = sqlite3_prepare_v2(
        db,
        DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID,
        sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID ),
        &((*this_).private_prepared_query_diagrams_by_parent_id),
        &unused
    );
    if (( SQLITE_OK != sqlite_err )
        || ( unused != &(DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID[sizeof(DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID)-1]) ))
    {
        LOG_ERROR_STR( "sqlite3_prepare_v2() failed:", DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID );
        LOG_ERROR_INT( "sqlite3_prepare_v2() failed:", sqlite_err );
        LOG_ERROR_STR( "sqlite3_prepare_v2() failed:", sqlite3_errmsg( db ) );
    }

    TRACE_END();
}

void data_database_reader_destroy ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    int sqlite_err;
    int perr;

    TRACE_INFO_STR( "sqlite3_finalize():", DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID );
    sqlite_err = sqlite3_finalize( (*this_).private_prepared_query_diagram_by_id );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_finalize() failed:", DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID );
        LOG_ERROR_INT( "sqlite3_finalize() failed:", sqlite_err );
    }

    TRACE_INFO_STR( "sqlite3_finalize():", DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID );
    sqlite_err = sqlite3_finalize( (*this_).private_prepared_query_diagrams_by_parent_id );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_finalize() failed:", DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID );
        LOG_ERROR_INT( "sqlite3_finalize() failed:", sqlite_err );
    }

    perr = pthread_mutex_destroy ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_destroy() failed:", perr );
    }

    TRACE_END();
}

int data_database_reader_get_diagram_by_id ( data_database_reader_t *this_, int64_t id, data_diagram_t *out_diagram )
{
    TRACE_BEGIN();
    int result = -1;
    int sqlite_err;
    static const int FIRST_SQL_BIND_PARAM = 1;
    int perr;

    perr = pthread_mutex_lock ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_lock() failed:", perr );
    }

    sqlite_err = sqlite3_reset( (*this_).private_prepared_query_diagram_by_id );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_INT( "sqlite3_reset() failed:", sqlite_err );
    }

    TRACE_INFO_STR( "sqlite3_bind_int():", DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID );
    TRACE_INFO_INT( "sqlite3_bind_int():", id );
    sqlite_err = sqlite3_bind_int( (*this_).private_prepared_query_diagram_by_id, FIRST_SQL_BIND_PARAM, id );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_INT( "sqlite3_bind_int() failed:", sqlite_err );
    }

    TRACE_INFO( "sqlite3_step()" );
    sqlite_err = sqlite3_step( (*this_).private_prepared_query_diagram_by_id );
    if ( SQLITE_ROW != sqlite_err )
    {
        LOG_ERROR_INT( "sqlite3_step() failed:", sqlite_err );
    }

    if ( SQLITE_ROW == sqlite_err )
    {
        result = 0;
        data_diagram_init_empty(out_diagram);
        (*out_diagram).id = sqlite3_column_int64( (*this_).private_prepared_query_diagram_by_id, RESULT_COLUMN_ID );
        (*out_diagram).parent_id = sqlite3_column_int64( (*this_).private_prepared_query_diagram_by_id, RESULT_COLUMN_PARENT_ID );
        (*out_diagram).diagram_type = sqlite3_column_int( (*this_).private_prepared_query_diagram_by_id, RESULT_COLUMN_TYPE );
        {
            const char* sqlite_stringresult;
            int length;
            utf8error_t strerr = UTF8ERROR_SUCCESS;

            sqlite_stringresult = sqlite3_column_text( (*this_).private_prepared_query_diagram_by_id, RESULT_COLUMN_NAME );
            strerr = utf8stringbuf_copy_str( (*out_diagram).name, sqlite_stringresult );
            if ( strerr != UTF8ERROR_SUCCESS )
            {
                LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
                length = sqlite3_column_bytes( (*this_).private_prepared_query_diagram_by_id, RESULT_COLUMN_NAME );
                LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", length );
            }

            sqlite_stringresult = sqlite3_column_text( (*this_).private_prepared_query_diagram_by_id, RESULT_COLUMN_DESCRIPTION );
            strerr = utf8stringbuf_copy_str( (*out_diagram).description, sqlite_stringresult );
            if ( strerr != UTF8ERROR_SUCCESS )
            {
                LOG_ERROR_HEX( "utf8stringbuf_copy_str() failed:", strerr );
                length = sqlite3_column_bytes( (*this_).private_prepared_query_diagram_by_id, RESULT_COLUMN_DESCRIPTION );
                LOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", length );
            }
        }
        (*out_diagram).list_order = sqlite3_column_int( (*this_).private_prepared_query_diagram_by_id, RESULT_COLUMN_LIST_ORDER );

        data_diagram_trace( out_diagram );
    }

    sqlite_err = sqlite3_step( (*this_).private_prepared_query_diagram_by_id );
    if ( SQLITE_DONE != sqlite_err )
    {
        LOG_ERROR_INT( "sqlite3_step() failed:", sqlite_err );
    }

    perr = pthread_mutex_unlock ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_unlock() failed:", perr );
    }

    TRACE_END_ERR( result );
    return result;
}

int32_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_, int64_t parent_id, int32_t max_out_array_size, data_diagram_t (*out_diagram)[] )
{
    TRACE_BEGIN();
    int32_t result = 0;

    TRACE_END_ERR( result );
    return result;
}

static int data_database_writer_private_trace_sql_result( void *my_data, int num, char** a, char** b )
{
    TRACE_BEGIN();
    int result = 0;

    TRACE_INFO_INT( "num:", num );

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
