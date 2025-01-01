/* File: data_diagramelement_iterator.c; Copyright and License: see below */

#include "storage/data_diagramelement_iterator.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

/*
 *  The "order by id" is important to get reproducable results, e.g. for json export
 */
const char *const DATA_DIAGRAMELEMENT_ITERATOR_SELECT_DIAGRAMELEMENTS_BY_DIAGRAM_ID =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id,uuid FROM diagramelements "
    "WHERE diagram_id=? ORDER BY id ASC;";

/*
 *  The "order by id" is important to get reproducable results, e.g. for json export
 */
const char *const DATA_DIAGRAMELEMENT_ITERATOR_SELECT_DIAGRAMELEMENTS_BY_CLASSIFIER_ID =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id,uuid FROM diagramelements "
    "WHERE classifier_id=? ORDER BY id ASC;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_DIAGRAMELEMENT_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: diagram_id
 */
static const int RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: classifier_id
 */
static const int RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: display_flags
 */
static const int RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: focused_feature_id
 */
static const int RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_DIAGRAMELEMENT_UUID_COLUMN = 5;

u8_error_t data_diagramelement_iterator_init_empty ( data_diagramelement_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    data_database_borrowed_stmt_init_void( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_diagramelement_iterator_reinit ( data_diagramelement_iterator_t *this_,
                                             data_database_borrowed_stmt_t statement )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &statement ) );
    u8_error_t result = U8_ERROR_NONE;

    /* destroy old state */
    result = data_diagramelement_iterator_destroy( this_ );

    /* init new state */
    (*this_).statement = statement;
    (*this_).is_at_end = false;
    result |= data_diagramelement_iterator_private_step_to_next( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_diagramelement_iterator_destroy ( data_diagramelement_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_borrowed_stmt_destroy( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

bool data_diagramelement_iterator_has_next ( const data_diagramelement_iterator_t *this_ )
{
    return ( ! (*this_).is_at_end );
}

u8_error_t data_diagramelement_iterator_next ( data_diagramelement_iterator_t *this_, data_diagramelement_t *out_diagramelement )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagramelement );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).statement) ) );
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_at_end )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).statement) );
        result |= data_diagramelement_init( out_diagramelement,
                                            sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                            sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                            sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN ),
                                            sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                            sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ),
                                            (const char*) sqlite3_column_text( sql_statement, RESULT_DIAGRAMELEMENT_UUID_COLUMN )
                                          );
        if ( SQLITE_NULL == sqlite3_column_type( sql_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
        {
            data_diagramelement_set_focused_feature_row_id ( out_diagramelement, DATA_ROW_VOID );
        }
        data_diagramelement_trace( out_diagramelement );

        /* step to next */
        result |= data_diagramelement_iterator_private_step_to_next( this_ );
    }
    else
    {
        U8_TRACE_INFO( "iterator already at end" );
        (*this_).is_at_end = true;
        result |= U8_ERROR_INVALID_REQUEST;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_diagramelement_iterator_private_step_to_next ( data_diagramelement_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &((*this_).statement) ) );
    u8_error_t result = U8_ERROR_NONE;

    /* do one step, check if is_at_end */
    {
        int sqlite_err;
        U8_TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( data_database_borrowed_stmt_get_statement( &((*this_).statement) ) );
        if ( SQLITE_DONE == sqlite_err )
        {
            U8_TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            (*this_).is_at_end = true;
        }
        else if ( SQLITE_ROW == sqlite_err )
        {
            (*this_).is_at_end = false;
        }
        else
        {
            U8_LOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            (*this_).is_at_end = true;
            result |= data_diagramelement_iterator_destroy( this_ );
            result |= U8_ERROR_AT_DB;
        }
    }

    U8_TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2024-2025 Andreas Warnke

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
