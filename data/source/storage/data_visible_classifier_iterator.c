/* File: data_visible_classifier_iterator.c; Copyright and License: see below */

#include "storage/data_visible_classifier_iterator.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

/*!
 *  \brief predefined search statement to find visible classifiers by diagram-id
 */
const char *const DATA_VISIBLE_CLASSIFIER_ITERATOR_SELECT_BY_DIAGRAM_ID =
    "SELECT classifiers.id,classifiers.main_type,classifiers.stereotype,"
    "classifiers.name,classifiers.description,classifiers.x_order,classifiers.y_order,classifiers.list_order,"
    "classifiers.uuid,"
    "diagramelements.id,diagramelements.diagram_id,diagramelements.display_flags,diagramelements.focused_feature_id,"
    "diagramelements.uuid "
    "FROM classifiers "
    "INNER JOIN diagramelements ON diagramelements.classifier_id=classifiers.id "
    "WHERE diagramelements.diagram_id=? "
    "ORDER BY diagramelements.id ASC;";
    /* To ensure reporducible results of json esports, ordering by a unique key is required here. */
    /* Ordering by 2 keys did not produce the expected results with sqlite3 3.34.1 */
    /* "ORDER BY classifiers.list_order ASC,diagramelements.id ASC;"; */
    /* see also https://sqlite.org/forum/forumpost/e1033dab18c262ac4b36cdf7c65bf87a5aaaecab3b3ba100e4588fc30e50f9fb */

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
 *  \brief the column id of the result where this parameter is stored: diagramelements.id
 */
static const int RESULT_DIAGRAMELEMENT_ID_COLUMN = 9;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.diagram_id
 */
static const int RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN = 10;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.display_flags
 */
static const int RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN = 11;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.focused_feature_id
 */
static const int RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN = 12;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.uuid
 */
static const int RESULT_DIAGRAMELEMENT_UUID_COLUMN = 13;

u8_error_t data_visible_classifier_iterator_init_empty ( data_visible_classifier_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    data_database_borrowed_stmt_init_void( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_visible_classifier_iterator_reinit ( data_visible_classifier_iterator_t *this_,
                                             data_database_borrowed_stmt_t statement )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &statement ) );
    u8_error_t result = U8_ERROR_NONE;

    /* destroy old state */
    result = data_visible_classifier_iterator_destroy( this_ );

    /* init new state */
    (*this_).statement = statement;
    (*this_).is_at_end = false;
    result |= data_visible_classifier_iterator_private_step_to_next( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_visible_classifier_iterator_destroy ( data_visible_classifier_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_borrowed_stmt_destroy( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

bool data_visible_classifier_iterator_has_next ( const data_visible_classifier_iterator_t *this_ )
{
    return ( ! (*this_).is_at_end );
}

u8_error_t data_visible_classifier_iterator_next ( data_visible_classifier_iterator_t *this_, data_visible_classifier_t *out_visible_classifier )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_visible_classifier );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).statement) ) );
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_at_end )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).statement) );

        data_visible_classifier_init_empty( out_visible_classifier );

        data_classifier_t *current_classifier;
        current_classifier = data_visible_classifier_get_classifier_ptr( out_visible_classifier );
        data_row_t classifier_id = sqlite3_column_int64( sql_statement, RESULT_CLASSIFIER_ID_COLUMN );
        result |= data_classifier_reinit( current_classifier,
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

        data_diagramelement_t *current_diag_element;
        current_diag_element = data_visible_classifier_get_diagramelement_ptr( out_visible_classifier );
        result |= data_diagramelement_reinit( current_diag_element,
                                              sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                              sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                              classifier_id,
                                              sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                              sqlite3_column_int64( sql_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ),
                                              (const char*) sqlite3_column_text( sql_statement, RESULT_DIAGRAMELEMENT_UUID_COLUMN )
                                            );
        if ( SQLITE_NULL == sqlite3_column_type( sql_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
        {
            data_diagramelement_set_focused_feature_row_id ( current_diag_element, DATA_ROW_VOID );
        }

        data_classifier_trace( current_classifier );
        data_diagramelement_trace( current_diag_element );

        /* step to next */
        result |= data_visible_classifier_iterator_private_step_to_next( this_ );
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

u8_error_t data_visible_classifier_iterator_private_step_to_next ( data_visible_classifier_iterator_t *this_ )
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
            result |= data_visible_classifier_iterator_destroy( this_ );
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
