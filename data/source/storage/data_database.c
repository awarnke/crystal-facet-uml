/* File: data_database.c; Copyright and License: see below */

#include "storage/data_database.h"
#include "data_id.h"
#include "data_table.h"
#include "trace.h"
#include "log.h"
#include <unistd.h>
#include <assert.h>

/*!
 *  \brief string constant to create an sql database table
 *
 *  This table contains instances of classes, states, activities, interfaces (which are classifiers)
 *  and also packages.
 *  It does not contain relationships (even if they are classifiers) like generalizations, associations.
 *  \see http://www.omg.org/spec/UML/
 */
static const char *DATA_DATABASE_CREATE_CLASSIFIERINSTANCE_TABLE =
    "CREATE TABLE IF NOT EXISTS classifiers ( "
        "id INTEGER PRIMARY KEY ASC, "
        "main_type INTEGER, "
        "stereotype TEXT, "
        "name TEXT, "
        "description TEXT, "
        "x_order INTEGER, "
        "y_order INTEGER "
    ");";

/*!
 *  \brief string constant to create an sql database table
 *
 *  This table contains instances of generalizations, associations (which are relationships)
 *  \see http://www.omg.org/spec/UML/
 */
static const char *DATA_DATABASE_CREATE_RELATIONSHIPINSTANCE_TABLE =
    "CREATE TABLE IF NOT EXISTS relationships ( "
        "id INTEGER PRIMARY KEY ASC, "
        "main_type INTEGER, "
        "from_classifier_id INTEGER, "
        "to_classifier_id INTEGER, "
        "name TEXT, "
        "description TEXT, "
        "list_order INTEGER, "
        "FOREIGN KEY(from_classifier_id) REFERENCES classifiers(id), "
        "FOREIGN KEY(to_classifier_id) REFERENCES classifiers(id) "
    ");";

/*!
 *  \brief string constant to create an sql database table
 *
 *  This table contains instances of attributes (which are properties which are features).
 *  \see http://www.omg.org/spec/UML/
 */
static const char *DATA_DATABASE_CREATE_FEATUREINSTANCE_TABLE =
    "CREATE TABLE IF NOT EXISTS features ( "
        "id INTEGER PRIMARY KEY ASC, "
        "main_type INTEGER, "
        "classifier_id INTEGER, "
        "key TEXT, "
        "value TEXT, "
        "description TEXT, "
        "list_order INTEGER, "
        "FOREIGN KEY(classifier_id) REFERENCES classifiers(id) "
    ");";

/*!
 *  \brief string constant to create an sql database table
 */
static const char *DATA_DATABASE_CREATE_DIAGRAM_TABLE =
    "CREATE TABLE IF NOT EXISTS diagrams ( "
        "id INTEGER PRIMARY KEY ASC, "
        "parent_id INTEGER, "
        "diagram_type INTEGER, "
        "name TEXT, "
        "description TEXT, "
        "list_order INTEGER, "
        "FOREIGN KEY(parent_id) REFERENCES diagrams(id) "
    ");";

/*!
 *  \brief string constant to create an sql database table
 */
static const char *DATA_DATABASE_CREATE_DIAGRAM_ELEMENTS_TABLE =
    "CREATE TABLE IF NOT EXISTS diagramelements ( "
        "id INTEGER PRIMARY KEY ASC, "
        "diagram_id INTEGER, "
        "classifier_id INTEGER, "
        "FOREIGN KEY(diagram_id) REFERENCES diagrams(id), "
        "FOREIGN KEY(classifier_id) REFERENCES classifiers(id) "
    ");";

static void data_database_private_initialize_tables( sqlite3 *db )
{
    TRACE_BEGIN();
    int sqlite_err;
    char *error_msg = NULL;

    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_CREATE_CLASSIFIERINSTANCE_TABLE );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_CLASSIFIERINSTANCE_TABLE, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_CLASSIFIERINSTANCE_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_CREATE_RELATIONSHIPINSTANCE_TABLE );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_RELATIONSHIPINSTANCE_TABLE, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_RELATIONSHIPINSTANCE_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_CREATE_FEATUREINSTANCE_TABLE );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_FEATUREINSTANCE_TABLE, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_FEATUREINSTANCE_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_CREATE_DIAGRAM_TABLE );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_DIAGRAM_TABLE, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_DIAGRAM_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_CREATE_DIAGRAM_ELEMENTS_TABLE );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_CREATE_DIAGRAM_ELEMENTS_TABLE, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_CREATE_DIAGRAM_ELEMENTS_TABLE );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    TRACE_END();
}

void data_database_init ( data_database_t *this_ )
{
    TRACE_BEGIN();
    int perr;

    TRACE_INFO_STR( "sqlite3_libversion", sqlite3_libversion() );

    (*this_).db_file_name = utf8stringbuf_init( sizeof((*this_).private_db_file_name_buffer), (*this_).private_db_file_name_buffer );
    utf8stringbuf_clear( (*this_).db_file_name );

    (*this_).is_open = false;

    perr = pthread_mutex_init ( &((*this_).private_lock), NULL );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_init() failed:", perr );
    }

    data_change_notifier_init ( &((*this_).notifier) );
    data_database_private_clear_db_listener_list( this_ );

    TRACE_END();
}

data_error_t data_database_open ( data_database_t *this_, const char* db_file_path )
{
    TRACE_BEGIN();
    assert( NULL != db_file_path );
    int sqlite_err;
    data_error_t result = DATA_ERROR_NONE;
    bool notify_listeners = false;

    result |= data_database_private_lock( this_ );

    if ( (*this_).is_open )
    {
        LOG_ERROR("data_database_open called on database that was not closed.");
        result |= DATA_ERROR_INVALID_REQUEST;
    }
    else
    {
        utf8stringbuf_copy_str( (*this_).db_file_name, db_file_path );

        LOG_EVENT_STR( "sqlite3_open:", utf8stringbuf_get_string( (*this_).db_file_name ) );
        sqlite_err = sqlite3_open( utf8stringbuf_get_string( (*this_).db_file_name ), &((*this_).db) );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_INT( "sqlite3_open() failed:", sqlite_err );
            LOG_ERROR_STR( "sqlite3_open() failed:", utf8stringbuf_get_string( (*this_).db_file_name ) );
            (*this_).is_open = false;
        }
        else
        {
            (*this_).is_open = true;
            data_database_private_initialize_tables( (*this_).db );
            notify_listeners = true;
        }
    }

    result |= data_database_private_unlock( this_ );

    if ( notify_listeners )
    {
        /* inform readers and writers on open */
        result |= data_database_private_notify_db_listeners( this_, DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED );

        /* inform listeners on changes */
        data_change_notifier_emit_signal( &((*this_).notifier), DATA_TABLE_VOID, DATA_ID_CONST_VOID_ID );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_close ( data_database_t *this_ )
{
    TRACE_BEGIN();
    int sqlite_err;
    data_error_t result = DATA_ERROR_NONE;
    bool notify_change_listeners = false;

    if ( (*this_).is_open )
    {
        /* prepare close */
        result |= data_database_private_notify_db_listeners( this_, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    }

    result |= data_database_private_lock( this_ );

    if ( (*this_).is_open )
    {
        /* perform close */
        LOG_EVENT_STR( "sqlite3_close:", utf8stringbuf_get_string( (*this_).db_file_name ) );
        sqlite_err = sqlite3_close( (*this_).db );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_INT( "sqlite3_close() failed:", sqlite_err );
        }

        if ( SQLITE_BUSY == sqlite_err )
        {
            /* retry */
            sleep (1);

            LOG_EVENT_STR( "sqlite3_close_v2:", utf8stringbuf_get_string( (*this_).db_file_name ) );
            sqlite_err = sqlite3_close_v2( (*this_).db );
            if ( SQLITE_OK != sqlite_err )
            {
                LOG_ERROR_INT( "sqlite3_close_v2() failed:", sqlite_err );
            }
        }

        utf8stringbuf_clear( (*this_).db_file_name );
        (*this_).is_open = false;

        notify_change_listeners = true;
    }
    else
    {
        LOG_ERROR("data_database_close called on database that was not open.");
        result |= DATA_ERROR_INVALID_REQUEST;
    }

    result |= data_database_private_unlock( this_ );

    if ( notify_change_listeners )
    {
        /* inform listeners on changes */
        data_change_notifier_emit_signal( &((*this_).notifier), DATA_TABLE_VOID, DATA_ID_CONST_VOID_ID );
    }

    TRACE_END_ERR( result );
    return result;
}

void data_database_destroy ( data_database_t *this_ )
{
    TRACE_BEGIN();
    int perr;

    data_database_private_clear_db_listener_list( this_ );
    if ( (*this_).is_open )
    {
        data_database_close( this_ );
    }
    data_change_notifier_destroy( &((*this_).notifier) );

    perr = pthread_mutex_destroy ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_destroy() failed:", perr );
    }

    TRACE_END();
}

data_error_t data_database_add_db_listener( data_database_t *this_, data_database_listener_t *listener )
{
    TRACE_BEGIN();
    assert( NULL != listener );
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_private_lock( this_ );

    int pos = -1;
    for( int index = 0; index < GUI_DATABASE_MAX_LISTENERS; index ++ )
    {
        if ( NULL == (*this_).listener_list[index] )
        {
            pos = index;
        }
    }

    if ( -1 != pos )
    {
        (*this_).listener_list[pos] = listener;
    }
    else
    {
        LOG_ERROR_INT( "Maximum number of listeners reached.", GUI_DATABASE_MAX_LISTENERS );
        result = DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
    }

    result |= data_database_private_unlock( this_ );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_remove_db_listener( data_database_t *this_, data_database_listener_t *listener )
{
    TRACE_BEGIN();
    assert( NULL != listener );
    data_error_t result = DATA_ERROR_NONE;
    int count_closed = 0;

    result |= data_database_private_lock( this_ );

    for( int index = 0; index < GUI_DATABASE_MAX_LISTENERS; index ++ )
    {
        if ( (*this_).listener_list[index] == listener )
        {
            (*this_).listener_list[index] = NULL;
            count_closed ++;
        }
    }

    result |= data_database_private_unlock( this_ );

    if ( count_closed == 0 )
    {
        LOG_ERROR( "listener not found" );
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_private_notify_db_listeners( data_database_t *this_, data_database_listener_signal_t signal_id )
{
    TRACE_BEGIN();
    data_database_listener_t *(listener_list_copy[GUI_DATABASE_MAX_LISTENERS]);
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_private_lock( this_ );

    memcpy( listener_list_copy, (*this_).listener_list, sizeof(listener_list_copy) );

    result |= data_database_private_unlock( this_ );

    for( int index = 0; index < GUI_DATABASE_MAX_LISTENERS; index ++ )
    {
        if ( NULL != listener_list_copy[index] )
        {
            data_database_listener_notify( listener_list_copy[index], signal_id );
        }
    }

    TRACE_END_ERR( result );
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
