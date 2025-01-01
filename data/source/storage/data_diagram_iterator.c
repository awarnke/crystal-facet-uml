/* File: data_diagram_iterator.c; Copyright and License: see below */

#include "storage/data_diagram_iterator.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

const char *const DATA_DIAGRAM_ITERATOR_SELECT_DIAGRAMS_BY_PARENT_ID =
    "SELECT id,parent_id,diagram_type,stereotype,name,description,list_order,display_flags,uuid "
    "FROM diagrams WHERE parent_id=? ORDER BY list_order ASC;";

const char *const DATA_DIAGRAM_ITERATOR_SELECT_DIAGRAMS_BY_PARENT_ID_NULL =
    "SELECT id,parent_id,diagram_type,stereotype,name,description,list_order,display_flags,uuid "
    "FROM diagrams WHERE parent_id IS NULL ORDER BY list_order ASC;";

const char *const DATA_DIAGRAM_ITERATOR_SELECT_DIAGRAMS_BY_CLASSIFIER_ID =
    "SELECT diagrams.id,diagrams.parent_id,diagrams.diagram_type,diagrams.stereotype,"
    "diagrams.name,diagrams.description,diagrams.list_order,diagrams.display_flags,diagrams.uuid "
    "FROM diagrams "
    "INNER JOIN diagramelements ON diagramelements.diagram_id=diagrams.id "
    "WHERE diagramelements.classifier_id=? "
    "GROUP BY diagrams.id "  /* filter duplicates if a classifier exists twice in a diagram */
    "ORDER BY diagrams.list_order ASC;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_DIAGRAM_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: parent_id
 */
static const int RESULT_DIAGRAM_PARENT_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: type
 */
static const int RESULT_DIAGRAM_TYPE_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: stereotype
 */
static const int RESULT_DIAGRAM_STEREOTYPE_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_DIAGRAM_NAME_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_DIAGRAM_DESCRIPTION_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_DIAGRAM_LIST_ORDER_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: display_flags
 */
static const int RESULT_DIAGRAM_DISPLAY_FLAGS_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_DIAGRAM_UUID_COLUMN = 8;

u8_error_t data_diagram_iterator_init_empty ( data_diagram_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    data_database_borrowed_stmt_init_void( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_diagram_iterator_reinit ( data_diagram_iterator_t *this_,
                                          data_database_borrowed_stmt_t statement )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &statement ) );
    u8_error_t result = U8_ERROR_NONE;

    /* destroy old state */
    result = data_diagram_iterator_destroy( this_ );

    /* init new state */
    (*this_).statement = statement;
    (*this_).is_at_end = false;
    result |= data_diagram_iterator_private_step_to_next( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_diagram_iterator_destroy ( data_diagram_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_borrowed_stmt_destroy( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

bool data_diagram_iterator_has_next ( const data_diagram_iterator_t *this_ )
{
    return ( ! (*this_).is_at_end );
}

u8_error_t data_diagram_iterator_next ( data_diagram_iterator_t *this_, data_diagram_t *out_diagram )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_diagram );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).statement) ) );
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_at_end )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).statement) );
        result |= data_diagram_init( out_diagram,
                                     sqlite3_column_int64( sql_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                     sqlite3_column_int64( sql_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                     sqlite3_column_int( sql_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                     (const char*) sqlite3_column_text( sql_statement, RESULT_DIAGRAM_STEREOTYPE_COLUMN ),
                                     (const char*) sqlite3_column_text( sql_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                     (const char*) sqlite3_column_text( sql_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                     sqlite3_column_int( sql_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN ),
                                     sqlite3_column_int64( sql_statement, RESULT_DIAGRAM_DISPLAY_FLAGS_COLUMN ),
                                     (const char*) sqlite3_column_text( sql_statement, RESULT_DIAGRAM_UUID_COLUMN )
                                   );
        if ( SQLITE_NULL == sqlite3_column_type( sql_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ) )
        {
            data_diagram_set_parent_row_id( out_diagram, DATA_ROW_VOID );
        }
        data_diagram_trace( out_diagram );

        /* step to next */
        result |= data_diagram_iterator_private_step_to_next( this_ );
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

u8_error_t data_diagram_iterator_private_step_to_next ( data_diagram_iterator_t *this_ )
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
            result |= data_diagram_iterator_destroy( this_ );
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
