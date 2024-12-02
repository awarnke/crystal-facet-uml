/* File: data_classifier_iterator.c; Copyright and License: see below */

#include "storage/data_classifier_iterator.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

/*
 *  The "order by cnt" is important to ensure parent objects are iterated first, e.g. for xmi export
 *  The "order by id" is important to get reproducable results
 */
const char *const DATA_CLASSIFIER_ITERATOR_SELECT_ALL_HIERARCHICAL =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order,uuid,"
        "(SELECT count(*) FROM relationships "
        "WHERE (relationships.to_classifier_id=classifiers.id) AND (relationships.to_feature_id IS NULL) "
        "AND (relationships.main_type=300)) AS cnt "
    "FROM classifiers "
    "ORDER BY cnt ASC,id ASC;";

/*
 *  The "order by id" is important to get reproducable results, e.g. for json export
 */
const char *const DATA_CLASSIFIER_ITERATOR_SELECT_ALL =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order,uuid,-1 "
    "FROM classifiers "
    "ORDER BY id ASC;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_CLASSIFIER_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_CLASSIFIER_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: stereotype
 */
static const int RESULT_CLASSIFIER_STEREOTYPE_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_CLASSIFIER_NAME_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_CLASSIFIER_DESCRIPTION_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: x_order
 */
static const int RESULT_CLASSIFIER_X_ORDER_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: y_order
 */
static const int RESULT_CLASSIFIER_Y_ORDER_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_CLASSIFIER_LIST_ORDER_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_CLASSIFIER_UUID_COLUMN = 8;

/*!
 *  \brief the column id of the result where this parameter is stored: count of containment parents
 */
static const int RESULT_CLASSIFIER_CONTAINMENT_PARENTS_COLUMN = 9;

u8_error_t data_classifier_iterator_init_empty ( data_classifier_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    data_database_borrowed_stmt_init_void( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_classifier_iterator_reinit ( data_classifier_iterator_t *this_,
                                             data_database_borrowed_stmt_t statement )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &statement ) );
    u8_error_t result = U8_ERROR_NONE;

    /* destroy old state */
    result = data_classifier_iterator_destroy( this_ );

    /* init new state */
    (*this_).statement = statement;
    (*this_).is_at_end = false;
    result |= data_classifier_iterator_private_step_to_next( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_classifier_iterator_destroy ( data_classifier_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_borrowed_stmt_destroy( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

bool data_classifier_iterator_has_next ( const data_classifier_iterator_t *this_ )
{
    return ( ! (*this_).is_at_end );
}

u8_error_t data_classifier_iterator_next ( data_classifier_iterator_t *this_, data_classifier_t *out_classifier )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_classifier );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).statement) ) );
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_at_end )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).statement) );
        data_row_t classifier_id = sqlite3_column_int64( sql_statement, RESULT_CLASSIFIER_ID_COLUMN );
        result |= data_classifier_reinit( out_classifier,
                                          classifier_id,
                                          sqlite3_column_int( sql_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                          (const char*) sqlite3_column_text( sql_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                          (const char*) sqlite3_column_text( sql_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                          (const char*) sqlite3_column_text( sql_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                          sqlite3_column_int( sql_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                          sqlite3_column_int( sql_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN ),
                                          sqlite3_column_int( sql_statement, RESULT_CLASSIFIER_LIST_ORDER_COLUMN ),
                                          (const char*) sqlite3_column_text( sql_statement, RESULT_CLASSIFIER_UUID_COLUMN )
                                        );
        data_classifier_trace( out_classifier );
        U8_TRACE_INFO_INT( "count of containment parents:", sqlite3_column_int( sql_statement, RESULT_CLASSIFIER_CONTAINMENT_PARENTS_COLUMN ));

        /* step to next */
        result |= data_classifier_iterator_private_step_to_next( this_ );
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

u8_error_t data_classifier_iterator_private_step_to_next ( data_classifier_iterator_t *this_ )
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
            result |= data_classifier_iterator_destroy( this_ );
            result |= U8_ERROR_AT_DB;
        }
    }

    U8_TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2020-2024 Andreas Warnke

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
