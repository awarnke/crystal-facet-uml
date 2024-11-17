/* File: data_relationship_iterator.c; Copyright and License: see below */

#include "storage/data_relationship_iterator.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>
#include <stdint.h>

const char DATA_RELATIONSHIP_ITERATOR_SELECT_RELATIONSHIPS_BY_DIAGRAM_ID[] =
    "SELECT relationships.id,relationships.main_type,relationships.from_classifier_id,relationships.to_classifier_id,"
    "relationships.stereotype,relationships.name,relationships.description,relationships.list_order,"
    "relationships.from_feature_id,relationships.to_feature_id,relationships.uuid,"
    "source.id, dest.id " /* source.id, dest.id needed only for debugging */
    "FROM relationships "
    "INNER JOIN diagramelements AS source "
    "ON source.classifier_id=relationships.from_classifier_id "
    "INNER JOIN diagramelements AS dest "
    "ON (dest.classifier_id=relationships.to_classifier_id)AND(dest.diagram_id=source.diagram_id) "
    "WHERE source.diagram_id=? "
    "GROUP BY relationships.id "
    "ORDER BY relationships.list_order ASC;";

/*
 *  Order by id to ensure a defined, non-changeing order of relationships in json export
 */
const char DATA_RELATIONSHIP_ITERATOR_SELECT_RELATIONSHIPS_BY_CLASSIFIER_ID[] =
    "SELECT id,main_type,from_classifier_id,to_classifier_id,stereotype,name,description,list_order,"
    "from_feature_id,to_feature_id,uuid,-1,-1 "
    "FROM relationships "
    "WHERE from_classifier_id=? OR to_classifier_id=? "
    "ORDER BY id ASC;";

const char DATA_RELATIONSHIP_ITERATOR_SELECT_RELATIONSHIPS_BY_FEATURE_ID[] =
    "SELECT id,main_type,from_classifier_id,to_classifier_id,stereotype,name,description,list_order,"
    "from_feature_id,to_feature_id,uuid,-1,-1 "
    "FROM relationships "
    "WHERE from_feature_id=? OR to_feature_id=?;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_RELATIONSHIP_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: from_classifier_id
 */
static const int RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: to_classifier_id
 */
static const int RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_RELATIONSHIP_STEREOTYPE_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_RELATIONSHIP_NAME_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_RELATIONSHIP_DESCRIPTION_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_RELATIONSHIP_LIST_ORDER_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: from_feature_id
 */
static const int RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN = 8;

/*!
 *  \brief the column id of the result where this parameter is stored: to_feature_id
 */
static const int RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN = 9;

/*!
 *  \brief the column id of the result where this parameter is stored: uuid
 */
static const int RESULT_RELATIONSHIP_UUID_COLUMN = 10;

/*!
 *  \brief the column id of the result where this parameter is stored: source diagramelements.id
 */
static const int RESULT_RELATIONSHIP_SOURCE_DIAGRAMELEMENTS_ID_COLUMN = 11;

/*!
 *  \brief the column id of the result where this parameter is stored: dest diagramelements.id
 */
static const int RESULT_RELATIONSHIP_DEST_DIAGRAMELEMENTS_ID_COLUMN = 12;

u8_error_t data_relationship_iterator_init_empty ( data_relationship_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    data_database_borrowed_stmt_init_void( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_relationship_iterator_reinit ( data_relationship_iterator_t *this_,
                                             data_database_borrowed_stmt_t statement )
{
    U8_TRACE_BEGIN();
    assert( data_database_borrowed_stmt_is_valid( &statement ) );
    u8_error_t result = U8_ERROR_NONE;

    /* destroy old state */
    result = data_relationship_iterator_destroy( this_ );

    /* init new state */
    (*this_).statement = statement;
    (*this_).is_at_end = false;
    result |= data_relationship_iterator_private_step_to_next( this_ );

    U8_TRACE_END_ERR(result);
    return result;
}

u8_error_t data_relationship_iterator_destroy ( data_relationship_iterator_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    data_database_borrowed_stmt_destroy( &((*this_).statement) );
    (*this_).is_at_end = true;

    U8_TRACE_END_ERR(result);
    return result;
}

bool data_relationship_iterator_has_next ( const data_relationship_iterator_t *this_ )
{
    return ( ! (*this_).is_at_end );
}

u8_error_t data_relationship_iterator_next ( data_relationship_iterator_t *this_, data_relationship_t *out_relationship )
{
    U8_TRACE_BEGIN();
    assert( NULL != out_relationship );
    assert( data_database_borrowed_stmt_is_valid( &((*this_).statement) ) );
    u8_error_t result = U8_ERROR_NONE;

    if ( ! (*this_).is_at_end )
    {
        sqlite3_stmt *const sql_statement = data_database_borrowed_stmt_get_statement( &((*this_).statement) );

        result |= data_relationship_init( out_relationship,
                                          sqlite3_column_int64( sql_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                          sqlite3_column_int64( sql_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                          sqlite3_column_int64( sql_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ),
                                          sqlite3_column_int64( sql_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                          sqlite3_column_int64( sql_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ),
                                          sqlite3_column_int( sql_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                          (const char*) sqlite3_column_text( sql_statement, RESULT_RELATIONSHIP_STEREOTYPE_COLUMN ),
                                          (const char*) sqlite3_column_text( sql_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                          (const char*) sqlite3_column_text( sql_statement, RESULT_RELATIONSHIP_DESCRIPTION_COLUMN ),
                                          sqlite3_column_int( sql_statement, RESULT_RELATIONSHIP_LIST_ORDER_COLUMN ),
                                          (const char*) sqlite3_column_text( sql_statement, RESULT_RELATIONSHIP_UUID_COLUMN )
                                        );
        if ( SQLITE_NULL == sqlite3_column_type( sql_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ) )
        {
            data_relationship_set_from_feature_row_id ( out_relationship, DATA_ROW_ID_VOID );
        }
        if ( SQLITE_NULL == sqlite3_column_type( sql_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ) )
        {
            data_relationship_set_to_feature_row_id ( out_relationship, DATA_ROW_ID_VOID );
        }
        U8_TRACE_INFO_INT( "(source)diagramelements.id:", sqlite3_column_int64( sql_statement, RESULT_RELATIONSHIP_SOURCE_DIAGRAMELEMENTS_ID_COLUMN ) );
        U8_TRACE_INFO_INT( "(dest)diagramelements.id:", sqlite3_column_int64( sql_statement, RESULT_RELATIONSHIP_DEST_DIAGRAMELEMENTS_ID_COLUMN ) );
        data_relationship_trace( out_relationship );

        /* step to next */
        result |= data_relationship_iterator_private_step_to_next( this_ );
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

u8_error_t data_relationship_iterator_private_step_to_next ( data_relationship_iterator_t *this_ )
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
            result |= data_relationship_iterator_destroy( this_ );
            result |= U8_ERROR_AT_DB;
        }
    }

    U8_TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2024-2024 Andreas Warnke

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
