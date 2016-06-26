/* File: data_database_reader.c; Copyright and License: see below */

#include "storage/data_database_reader.h"
#include "trace.h"
#include "log.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

/*!
 *  \brief predefined search statement to find a diagram by id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order FROM diagrams WHERE id=?;";

/*!
 *  \brief predefined search statement to find diagrams by parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order FROM diagrams WHERE parent_id=? ORDER BY list_order;";

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
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_DIAGRAM_NAME_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_DIAGRAM_DESCRIPTION_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_DIAGRAM_LIST_ORDER_COLUMN = 5;

/*!
 *  \brief predefined search statement to find a classifier by id
 */
static const char DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order FROM classifiers WHERE id=?;";

/*!
 *  \brief predefined search statement to find classifier by diagram-id
 */
static const char DATA_DATABASE_READER_SELECT_CLASSIFIERS_BY_DIAGRAM_ID[] =
    "SELECT classifiers.id,main_type,stereotype,name,description,x_order,y_order,"
    "diagramelements.id,diagramelements.display_flags "
    "FROM classifiers INNER JOIN diagramelements ON diagramelements.classifier_id=classifiers.id "
    "WHERE diagramelements.diagram_id=? ORDER BY y_order,x_order;";

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
 *  \brief the column id of the result where this parameter is stored: diagramelements.id
 */
static const int RESULT_CLASSIFIER_DIAGRAMELEMENT_ID_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.display_flags
 */
static const int RESULT_CLASSIFIER_DISPLAY_FLAGS_COLUMN = 8;

data_error_t data_database_reader_init ( data_database_reader_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    int perr;
    data_error_t result = DATA_ERROR_NONE;

    (*this_).database = database;
    (*this_).is_open = false;

    perr = pthread_mutex_init ( &((*this_).private_lock), NULL );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_init() failed:", perr );
        result |= DATA_ERROR_AT_MUTEX;
    }

    data_database_listener_init ( &((*this_).me_as_listener), this_, (void (*)(void*,data_database_listener_signal_t)) &data_database_reader_db_change_callback );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    if ( data_database_is_open( database ) )
    {
        /* if the database is open, open also the reader */
        result |= data_database_reader_private_open( this_ );
    }

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_reader_private_open ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_reader_private_lock( this_ );

    if ( ! (*this_).is_open )
    {
        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID ),
                                                                   &((*this_).private_prepared_query_diagram_by_id)
        );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID ),
                                                                   &((*this_).private_prepared_query_diagrams_by_parent_id)
        );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID ),
                                                                   &((*this_).private_prepared_query_classifier_by_id)
        );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_CLASSIFIERS_BY_DIAGRAM_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_CLASSIFIERS_BY_DIAGRAM_ID ),
                                                                   &((*this_).private_prepared_query_classifiers_by_diagram_id)
        );

        (*this_).is_open = true;
    }
    else
    {
        result |= DATA_ERROR_INVALID_REQUEST;
        LOG_WARNING( "Database is already open." );
    }

    result |= data_database_reader_private_unlock( this_ );

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_reader_private_close ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_reader_private_lock( this_ );

    if ( (*this_).is_open )
    {
        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagram_by_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagrams_by_parent_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_classifier_by_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_classifiers_by_diagram_id );

        (*this_).is_open = false;
    }
    else
    {
        result |= DATA_ERROR_INVALID_REQUEST;
        LOG_WARNING( "Database was not open." );
    }

    result |= data_database_reader_private_unlock( this_ );

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_reader_destroy ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    int perr;
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_reader_private_close( this_ );

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    perr = pthread_mutex_destroy ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_destroy() failed:", perr );
        result |= DATA_ERROR_AT_MUTEX;
    }

    (*this_).database = NULL;

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_, int64_t id, data_diagram_t *out_diagram )
{
    TRACE_BEGIN();
    assert( NULL != out_diagram );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    result |= data_database_reader_private_lock( this_ );

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagram_by_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            LOG_ERROR( "sqlite3_step() did not find a row." );
            result |= DATA_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_diagram_init( out_diagram,
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN )
            );

            data_diagram_trace( out_diagram );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            LOG_ERROR_INT( "sqlite3_step() failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        LOG_WARNING( "Database not open, cannot request data." );
    }

    result |= data_database_reader_private_unlock( this_ );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_,
                                                              int64_t parent_id,
                                                              uint32_t max_out_array_size,
                                                              data_diagram_t (*out_diagram)[],
                                                              uint32_t *out_diagram_count )
{
    TRACE_BEGIN();
    assert( NULL != out_diagram_count );
    assert( NULL != out_diagram );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    result |= data_database_reader_private_lock( this_ );

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagrams_by_parent_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, parent_id );

        *out_diagram_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                LOG_ERROR_INT( "sqlite3_step() failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagram_count = row_index+1;
                data_diagram_t *current_diag = &((*out_diagram)[row_index]);

                result |= data_diagram_init( current_diag,
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN )
                );

                data_diagram_trace( current_diag );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                LOG_ERROR_INT( "out_diagram[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step() finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        LOG_WARNING( "Database not open, cannot request data." );
    }

    result |= data_database_reader_private_unlock( this_ );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_classifier_by_id ( data_database_reader_t *this_, int64_t id, data_classifier_t *out_classifier )
{
    TRACE_BEGIN();
    assert( NULL != out_classifier );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    result |= data_database_reader_private_lock( this_ );

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_classifier_by_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            LOG_ERROR( "sqlite3_step() did not find a row." );
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
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN )
            );

            data_classifier_trace( out_classifier );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            LOG_ERROR_INT( "sqlite3_step() failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        LOG_WARNING( "Database not open, cannot request data." );
    }

    result |= data_database_reader_private_unlock( this_ );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_classifiers_by_diagram_id ( data_database_reader_t *this_,
                                                                  int64_t diagram_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_visible_classifier_t (*out_vis_classifier)[],
                                                                  uint32_t *out_vis_classifier_count )
{
    TRACE_BEGIN();
    assert( NULL != out_vis_classifier_count );
    assert( NULL != out_vis_classifier );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    result |= data_database_reader_private_lock( this_ );

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_classifiers_by_diagram_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, diagram_id );

        *out_vis_classifier_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                LOG_ERROR_INT( "sqlite3_step() failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_vis_classifier_count = row_index+1;
                data_visible_classifier_t *current_vis_classifier;
                current_vis_classifier = &((*out_vis_classifier)[row_index]);
                data_visible_classifier_init_empty( current_vis_classifier );

                data_classifier_t *current_classifier;
                current_classifier = data_visible_classifier_get_classifier_ptr( current_vis_classifier );
                int64_t classifier_id = sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN );
                result |= data_classifier_reinit( current_classifier,
                                                  classifier_id,
                                                  sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN )
                );

                data_diagramelement_t *current_diag_element;
                current_diag_element = data_visible_classifier_get_diagramelement_ptr( current_vis_classifier );
                data_diagramelement_reinit ( current_diag_element,
                                             sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_DIAGRAMELEMENT_ID_COLUMN ),
                                             diagram_id,
                                             classifier_id,
                                             sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_DISPLAY_FLAGS_COLUMN )
                );

                data_classifier_trace( current_classifier );
                data_diagramelement_trace( current_diag_element );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                LOG_ERROR_INT( "out_vis_classifier[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step() finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        LOG_WARNING( "Database not open, cannot request data." );
    }

    result |= data_database_reader_private_unlock( this_ );

    TRACE_END_ERR( result );
    return result;
}

void data_database_reader_db_change_callback ( data_database_reader_t *this_, data_database_listener_signal_t signal_id )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    switch ( signal_id )
    {
        case DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE" );
            result |= data_database_reader_private_close( this_ );
        }
        break;

        case DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED" );
            result |= data_database_reader_private_open( this_ );
        }
        break;

        default:
        {
            LOG_ERROR( "unexpected data_database_listener_signal_t" );
        }
    }

    TRACE_END();
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
