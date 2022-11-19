/* File: data_database_iterator_classifiers.c; Copyright and License: see below */

#include "storage/data_database_iterator_classifiers.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

/*!
 *  \brief search statement to iterate over all classifiers sorted by number of parent-containers
 *
 *  The "order by cnt" is important to ensure parent objects are iterated first, e.g. for xmi export
 *  The "order by id" is important to get reproducable results
 */
static const char DATA_DATABASE_ITERATOR_CLASSIFIERS_SELECT_ALL_HIERARCHICAL[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order,uuid,"
        "(SELECT count(*) FROM relationships "
        "WHERE (relationships.to_classifier_id=classifiers.id) AND (relationships.to_feature_id IS NULL) "
        "AND (relationships.main_type=300)) AS cnt "
    "FROM classifiers "
    "ORDER BY cnt ASC,id ASC;";

/*!
 *  \brief search statement to iterate over all classifiers
 *
 *  The "order by id" is important to get reproducable results, e.g. for json export
 */
static const char DATA_DATABASE_ITERATOR_CLASSIFIERS_SELECT_ALL[] =
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

u8_error_t data_database_iterator_classifiers_init_empty ( data_database_iterator_classifiers_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    (*this_).is_valid = false;
    (*this_).database = NULL;
    (*this_).statement_all_classifiers = NULL;
    (*this_).is_at_end = true;

    TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_iterator_classifiers_reinit ( data_database_iterator_classifiers_t *this_,
                                                       data_database_t *database,
                                                       bool hierarchical )
{
    TRACE_BEGIN();
    assert( NULL != database );
    u8_error_t result = U8_ERROR_NONE;

    /* destroy old state */
    result = data_database_iterator_classifiers_destroy( this_ );
    (*this_).statement_all_classifiers = NULL;

    /* init new state */
    (*this_).is_valid = true;
    (*this_).database = database;
    {
        const char *const string_statement
            = hierarchical
            ? DATA_DATABASE_ITERATOR_CLASSIFIERS_SELECT_ALL_HIERARCHICAL
            : DATA_DATABASE_ITERATOR_CLASSIFIERS_SELECT_ALL;
        const size_t string_size
            = hierarchical
            ? sizeof( DATA_DATABASE_ITERATOR_CLASSIFIERS_SELECT_ALL_HIERARCHICAL )
            : sizeof( DATA_DATABASE_ITERATOR_CLASSIFIERS_SELECT_ALL );
        sqlite3 *db = data_database_get_database_ptr ( (*this_).database );
        const char *first_unused_statement_char;
        TRACE_INFO_STR( "sqlite3_prepare_v2():", string_statement );
        const int sqlite_err = sqlite3_prepare_v2( db,
                                                   string_statement,
                                                   string_size,
                                                   &((*this_).statement_all_classifiers),
                                                   &first_unused_statement_char
                                                 );
        if (( SQLITE_OK != sqlite_err )
            || ( first_unused_statement_char != &(string_statement[string_size-1]) ))
        {
            TSLOG_ERROR_STR( "sqlite3_prepare_v2() failed:", string_statement );
            TSLOG_ERROR_INT( "sqlite3_prepare_v2() failed:", sqlite_err );
            TSLOG_ERROR_STR( "sqlite3_prepare_v2() failed:", sqlite3_errmsg( db ) );
            result |= U8_ERROR_AT_DB;
        }
    }
    result |= data_database_iterator_private_step_to_next( this_ );

    TRACE_END_ERR(result);
    return result;
}

u8_error_t data_database_iterator_classifiers_destroy ( data_database_iterator_classifiers_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_valid )
    {
        assert( NULL != (*this_).statement_all_classifiers );
        TRACE_INFO_STR( "sqlite3_finalize():", sqlite3_sql( (*this_).statement_all_classifiers ) );
        const int sqlite_err = sqlite3_finalize( (*this_).statement_all_classifiers );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_finalize() failed:", sqlite3_sql( (*this_).statement_all_classifiers ) );
            TSLOG_ERROR_INT( "sqlite3_finalize() failed:", sqlite_err );
            result = U8_ERROR_AT_DB;
        }
    }

    (*this_).is_valid = false;
    (*this_).database = NULL;
    (*this_).statement_all_classifiers = NULL;
    (*this_).is_at_end = true;

    TRACE_END_ERR(result);
    return result;
}

bool data_database_iterator_classifiers_has_next ( const data_database_iterator_classifiers_t *this_ )
{
    return ( ! (*this_).is_at_end );
}

u8_error_t data_database_iterator_classifiers_next ( data_database_iterator_classifiers_t *this_, data_classifier_t *out_classifier )
{
    TRACE_BEGIN();
    assert( NULL != out_classifier );
    u8_error_t result = U8_ERROR_NONE;

    if ( (*this_).is_valid )
    {
        if ( ! (*this_).is_at_end )
        {
            sqlite3_stmt *const sql_statement = (*this_).statement_all_classifiers;
            data_row_id_t classifier_id = sqlite3_column_int64( sql_statement, RESULT_CLASSIFIER_ID_COLUMN );
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
            TRACE_INFO_INT( "count of containment parents:", sqlite3_column_int( sql_statement, RESULT_CLASSIFIER_CONTAINMENT_PARENTS_COLUMN ));

            /* step to next */
            result |= data_database_iterator_private_step_to_next( this_ );
        }
        else
        {
            TRACE_INFO( "iterator already at end" );
            (*this_).is_at_end = true;
            result |= U8_ERROR_INVALID_REQUEST;
        }
    }
    else
    {
        result |= U8_ERROR_NO_DB;
        TRACE_INFO( "No valid sql statement, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_iterator_private_step_to_next ( data_database_iterator_classifiers_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).is_valid );
    assert( NULL != (*this_).statement_all_classifiers );
    u8_error_t result = U8_ERROR_NONE;

    /* do one step, check if is_at_end */
    {
        int sqlite_err;
        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( (*this_).statement_all_classifiers );
        if ( SQLITE_DONE == sqlite_err )
        {
            TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            (*this_).is_at_end = true;
        }
        else if ( SQLITE_ROW == sqlite_err )
        {
            (*this_).is_at_end = false;
        }
        else
        {
            TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            (*this_).is_at_end = true;
            result |= data_database_iterator_classifiers_destroy( this_ );
            result |= U8_ERROR_AT_DB;
        }
    }

    TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2020-2022 Andreas Warnke

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
