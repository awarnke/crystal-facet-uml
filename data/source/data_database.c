/* File: data_database.c; Copyright and License: see below */

#include "data_database.h"
#include "trace.h"
#include "log.h"
#include <unistd.h>

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
    "CREATE TABLE IF NOT EXISTS diagram_elements ( "
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

    TRACE_INFO_STR( "sqlite3_libversion", sqlite3_libversion() );

    (*this_).db_file_name = "";
    (*this_).is_open = false;
    data_change_notifier_init ( &((*this_).notifier) );

    TRACE_END();
}

void data_database_open ( data_database_t *this_, const char* db_file_path )
{
    TRACE_BEGIN();
    int sqlite_err;

    if ( (*this_).is_open )
    {
        LOG_WARNING("data_database_open called on database that was not closed.");
        data_database_close( this_ );
    }

    (*this_).db_file_name = db_file_path;

    LOG_EVENT_STR( "sqlite3_open:", (*this_).db_file_name );
    sqlite_err = sqlite3_open( (*this_).db_file_name, &((*this_).db) );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_INT( "sqlite3_open() failed:", sqlite_err );
        LOG_ERROR_STR( "sqlite3_open() failed:", (*this_).db_file_name );
        (*this_).is_open = false;
    }
    else
    {
        (*this_).is_open = true;
        data_database_private_initialize_tables( (*this_).db );
    }

    TRACE_END();
}

void data_database_close ( data_database_t *this_ )
{
    TRACE_BEGIN();
    int sqlite_err;

    if ( (*this_).is_open )
    {
        LOG_EVENT_STR( "sqlite3_close:", (*this_).db_file_name );
        sqlite_err = sqlite3_close( (*this_).db );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_INT( "sqlite3_close() failed:", sqlite_err );
        }

        if ( SQLITE_BUSY == sqlite_err )
        {
            /* retry */
            sleep (1);

            LOG_EVENT_STR( "sqlite3_close_v2:", (*this_).db_file_name );
            sqlite_err = sqlite3_close_v2( (*this_).db );
            if ( SQLITE_OK != sqlite_err )
            {
                LOG_ERROR_INT( "sqlite3_close_v2() failed:", sqlite_err );
            }
        }

        (*this_).db_file_name = "";
        (*this_).is_open = false;
    }
    else
    {
        LOG_WARNING("data_database_close called on database that was not open.");
    }

    TRACE_END();
}

void data_database_destroy ( data_database_t *this_ )
{
    TRACE_BEGIN();

    if ( (*this_).is_open )
    {
        data_database_close( this_ );
    }
    data_change_notifier_destroy( &((*this_).notifier) );

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
