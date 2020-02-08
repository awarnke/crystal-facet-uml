/* File: data_database_text_search.c; Copyright and License: see below */

#include "storage/data_database_text_search.h"
#include "trace.h"
#include "tslog.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

data_error_t data_database_text_search_init ( data_database_text_search_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    data_error_t result = DATA_ERROR_NONE;

    (*this_).database = database;
    (*this_).is_open = false;

    data_database_listener_init ( &((*this_).me_as_listener), this_, (void (*)(void*,data_database_listener_signal_t)) &data_database_text_search_db_change_callback );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    if ( data_database_is_open( database ) )
    {
        /* if the database is open, open also the reader */
        result |= data_database_text_search_private_open( this_ );
    }

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_text_search_destroy ( data_database_text_search_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_text_search_private_close( this_ );
    }

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    (*this_).database = NULL;

    TRACE_END_ERR(result);
    return result;
}

void data_database_text_search_db_change_callback ( data_database_text_search_t *this_, data_database_listener_signal_t signal_id )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    switch ( signal_id )
    {
        case DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE" );
            if ( (*this_).is_open )
            {
                result |= data_database_text_search_private_close( this_ );
            }
        }
        break;

        case DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED" );
            if ( (*this_).is_open )
            {
                result |= data_database_text_search_private_close( this_ );
            }
            result |= data_database_text_search_private_open( this_ );
        }
        break;

        default:
        {
            TSLOG_ERROR( "unexpected data_database_listener_signal_t" );
        }
    }

    TRACE_END();
}

/* ================================ DIAGRAM ================================ */


/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief predefined search statement to find a classifier by name
 */
static const char data_database_text_search_SELECT_CLASSIFIER_BY_NAME[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order FROM classifiers WHERE name=?;";

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

data_error_t data_database_text_search_get_classifier_by_name ( data_database_text_search_t *this_, const char *name, data_classifier_t *out_classifier )
{
    TRACE_BEGIN();
    assert( NULL != out_classifier );
    assert( NULL != name );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_classifier_by_name;

        result |= data_database_text_search_private_bind_text_to_statement( this_, prepared_statement, name );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            TSLOG_EVENT( "sqlite3_step did not find a row." );
            result |= DATA_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_classifier_init( out_classifier,
                                            sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_LIST_ORDER_COLUMN )
                                          );

            data_classifier_trace( out_classifier );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAMELEMENT ================================ */


/* ================================ FEATURE ================================ */


/* ================================ RELATIONSHIP ================================ */


/* ================================ private ================================ */

data_error_t data_database_text_search_private_open ( data_database_text_search_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( ! (*this_).is_open )
    {
        result |= data_database_text_search_private_prepare_statement ( this_,
                                                                        data_database_text_search_SELECT_CLASSIFIER_BY_NAME,
                                                                        sizeof( data_database_text_search_SELECT_CLASSIFIER_BY_NAME ),
                                                                        &((*this_).private_prepared_query_classifier_by_name)
                                                                      );

        (*this_).is_open = true;
    }
    else
    {
        result |= DATA_ERROR_INVALID_REQUEST;
        TSLOG_WARNING( "Database is already open." );
    }

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_text_search_private_close ( data_database_text_search_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_text_search_private_finalize_statement( this_, (*this_).private_prepared_query_classifier_by_name );

        (*this_).is_open = false;
    }
    else
    {
        result |= DATA_ERROR_INVALID_REQUEST;
        TSLOG_WARNING( "Database was not open." );
    }

    TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2020-2020 Andreas Warnke

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
