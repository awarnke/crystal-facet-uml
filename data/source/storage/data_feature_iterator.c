/* File: data_feature_iterator.c; Copyright and License: see below */

#include "storage/data_feature_iterator.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

/*!
 *  \brief predefined search statement to find features by diagram-id
 */
const char *const DATA_FEATURE_ITERATOR_SELECT_FEATURES_BY_DIAGRAM_ID =
    "SELECT features.id,features.main_type,features.classifier_id,"
    "features.key,features.value,features.description,features.list_order,features.uuid,"
    "diagramelements.id " /* diagramelements.id needed only for debugging */
    "FROM features INNER JOIN diagramelements ON diagramelements.classifier_id=features.classifier_id "
    /* "WHERE diagramelements.diagram_id=? " */
    "WHERE diagramelements.diagram_id=? AND ( diagramelements.focused_feature_id=features.id OR features.main_type<>3 ) "
    "GROUP BY features.id "
    "ORDER BY features.list_order ASC,features.id ASC;";   /* ensure always the same order */
    /* Note: 3 == DATA_FEATURE_TYPE_LIFELINE */

    /* if you ignore the type, "non-scenario features" of "classifiers that have a lifeline" would be skipped: */
    /* "WHERE diagramelements.diagram_id=? AND ( (diagramelements.focused_feature_id=features.id) OR (diagramelements.focused_feature_id ISNULL) ) " */
    /* but we want to skip only lifelines and only if these belong to foreign diagrams */

/*!
 *  \brief predefined search statement to find features by classifier-id
 *
 *  Order by id to ensure a defined, non-changeing order of relationships in json export
 */
const char *const DATA_FEATURE_ITERATOR_SELECT_FEATURES_BY_CLASSIFIER_ID =
    "SELECT id,main_type,classifier_id,key,value,description,list_order,uuid,-1 "
    "FROM features "
    "WHERE classifier_id=? ORDER BY id ASC;";   /* ensure always the same order */

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_FEATURE_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_FEATURE_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: classifier_id
 */
static const int RESULT_FEATURE_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: key
 */
static const int RESULT_FEATURE_KEY_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: value
 */
static const int RESULT_FEATURE_VALUE_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_FEATURE_DESCRIPTION_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_FEATURE_LIST_ORDER_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_FEATURE_LIST_UUID_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.id
 */
static const int RESULT_FEATURE_DIAGRAMELEMENTS_ID_COLUMN = 8;

u8_error_t data_feature_iterator_init_empty ( data_feature_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    data_database_borrowed_stmt_init_void( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_feature_iterator_reinit ( data_feature_iterator_t *this_,
                                             data_database_borrowed_stmt_t statement )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &statement ) );
    u8_error_t result = U8_ERROR_NONE;

    /* destroy old state */
    result = data_feature_iterator_destroy( this_ );

    /* init new state */
    (*this_).statement = statement;
    (*this_).is_at_end = false;
    result |= data_feature_iterator_private_step_to_next( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_feature_iterator_destroy ( data_feature_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_borrowed_stmt_destroy( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

bool data_feature_iterator_has_next ( const data_feature_iterator_t *this_ )
{
    return ( ! (*this_).is_at_end );
}

u8_error_t data_feature_iterator_next ( data_feature_iterator_t *this_, data_feature_t *out_feature )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_feature );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).statement) ) );
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_at_end )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).statement) );

        result |= data_feature_init( out_feature,
                                     sqlite3_column_int64( sql_statement, RESULT_FEATURE_ID_COLUMN ),
                                     sqlite3_column_int( sql_statement, RESULT_FEATURE_MAIN_TYPE_COLUMN ),
                                     sqlite3_column_int64( sql_statement, RESULT_FEATURE_CLASSIFIER_ID_COLUMN ),
                                     (const char*) sqlite3_column_text( sql_statement, RESULT_FEATURE_KEY_COLUMN ),
                                     (const char*) sqlite3_column_text( sql_statement, RESULT_FEATURE_VALUE_COLUMN ),
                                     (const char*) sqlite3_column_text( sql_statement, RESULT_FEATURE_DESCRIPTION_COLUMN ),
                                     sqlite3_column_int( sql_statement, RESULT_FEATURE_LIST_ORDER_COLUMN ),
                                     (const char*) sqlite3_column_text( sql_statement, RESULT_FEATURE_LIST_UUID_COLUMN )
                                   );
        U8_TRACE_INFO_INT( "diagramelements.id:", sqlite3_column_int64( sql_statement, RESULT_FEATURE_DIAGRAMELEMENTS_ID_COLUMN ) );
        data_feature_trace( out_feature );

        /* step to next */
        result |= data_feature_iterator_private_step_to_next( this_ );
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

u8_error_t data_feature_iterator_private_step_to_next ( data_feature_iterator_t *this_ )
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
            result |= data_feature_iterator_destroy( this_ );
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
