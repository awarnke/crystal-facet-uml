/* File: data_database.c; Copyright and License: see below */

#include "storage/data_database.h"
#include "data_id.h"
#include "data_table.h"
#include "trace.h"
#include "tslog.h"
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
static const char *DATA_DATABASE_CREATE_CLASSIFIER_TABLE =
    "CREATE TABLE IF NOT EXISTS classifiers ( "
        "id INTEGER PRIMARY KEY ASC, "
        "main_type INTEGER, "
        "stereotype TEXT, "
        "name TEXT UNIQUE, "
        "description TEXT, "
        "x_order INTEGER, "
        "y_order INTEGER, "
        "list_order INTEGER, "
        "uuid TEXT NOT NULL DEFAULT \'\'"  /* DEFAULT needed in case a new DB is modified by an old program version */
    ");";

/*!
 *  \brief string constant to create an sql database index
 */
/*
static const char *DATA_DATABASE_CREATE_CLASSIFIERORDERING_INDEX =
    "CREATE INDEX IF NOT EXISTS classifier_ordering ON classifiers ( "
        "y_order ASC, "
        "x_order ASC "
    ");";
*/

#if 0
/*!
 *  \brief string constant to update an sql database table
 *
 *  This command extends classifiers by list_order field.
 *  \see http://sqlite.org/lang_altertable.html
 */
static const char *DATA_DATABASE_ALTER_CLASSIFIER_TABLE_1 =
    "ALTER TABLE classifiers "
    "ADD COLUMN list_order INTEGER;";
#endif

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends classifiers by a uuid field.
 */
static const char *DATA_DATABASE_ALTER_CLASSIFIER_TABLE_UUID =
    "ALTER TABLE classifiers "
    "ADD COLUMN uuid TEXT NOT NULL DEFAULT \'\';";

#define DATA_DATABASE_CREATE_UUID \
"lower(hex( randomblob(4)) || '-' || hex( randomblob(2))"\
" || '-' || '4' || substr( hex( randomblob(2)), 2) || '-' || substr('89AB', 1 + (abs(random()) % 4) , 1) ||"\
" substr(hex(randomblob(2)), 2) || '-' || hex(randomblob(6)))"

/*!
 *  \brief string constant to update an sql database table
 *
 *  A uuid is generated and updated wherever missing.
 *  The subselect-WHERE clause is needed to generate for each row a different uuid.
 */
static const char *DATA_DATABASE_UPDATE_CLASSIFIER_UUID =
    "UPDATE classifiers SET uuid=(SELECT " DATA_DATABASE_CREATE_UUID " WHERE classifiers.id!=-1) WHERE uuid=\'\';";

/*!
 *  \brief string constant to create an sql database table
 *
 *  This table contains instances of generalizations, associations (which are relationships)
 *  \see http://www.omg.org/spec/UML/
 */
static const char *DATA_DATABASE_CREATE_RELATIONSHIP_TABLE =
    "CREATE TABLE IF NOT EXISTS relationships ( "
        "id INTEGER PRIMARY KEY ASC, "
        "main_type INTEGER, "
        "from_classifier_id INTEGER, "
        "to_classifier_id INTEGER, "
        "name TEXT, "
        "description TEXT, "
        "list_order INTEGER, "
        "from_feature_id INTEGER DEFAULT NULL, "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "to_feature_id INTEGER DEFAULT NULL, "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "uuid TEXT NOT NULL DEFAULT \'\', "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "FOREIGN KEY(from_classifier_id) REFERENCES classifiers(id), "
        "FOREIGN KEY(to_classifier_id) REFERENCES classifiers(id), "
        "FOREIGN KEY(from_feature_id) REFERENCES features(id), "
        "FOREIGN KEY(to_feature_id) REFERENCES features(id) "
    ");";

/*!
 *  \brief string constant to create an sql database index
 */
/*
static const char *DATA_DATABASE_CREATE_RELATIONSHIPORDERING_INDEX =
    "CREATE INDEX IF NOT EXISTS relationship_ordering ON relationships ( "
        "list_order ASC "
    ");";
*/

#if 0
/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends relationships by from_feature_id field.
 *  \see http://sqlite.org/lang_altertable.html
 */
static const char *DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_1 =
    "ALTER TABLE relationships "
    "ADD COLUMN from_feature_id INTEGER DEFAULT NULL;";

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends relationships by to_feature_id field.
 */
static const char *DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_2 =
    "ALTER TABLE relationships "
    "ADD COLUMN to_feature_id INTEGER DEFAULT NULL;";
#endif

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends relationships by a uuid field.
 */
static const char *DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_UUID =
    "ALTER TABLE relationships "
    "ADD COLUMN uuid TEXT NOT NULL DEFAULT \'\';";

/*!
 *  \brief string constant to update an sql database table
 *
 *  A uuid is generated and updated wherever missing.
 *  The subselect-WHERE clause is needed to generate for each row a different uuid.
 */
static const char *DATA_DATABASE_UPDATE_RELATIONSHIP_UUID =
    "UPDATE relationships SET uuid=(SELECT " DATA_DATABASE_CREATE_UUID " WHERE relationships.id!=-1) WHERE uuid=\'\';";

/*!
 *  \brief string constant to create an sql database table
 *
 *  This table contains instances of attributes (which are properties which are features).
 *  \see http://www.omg.org/spec/UML/
 */
static const char *DATA_DATABASE_CREATE_FEATURE_TABLE =
    "CREATE TABLE IF NOT EXISTS features ( "
        "id INTEGER PRIMARY KEY ASC, "
        "main_type INTEGER, "
        "classifier_id INTEGER, "
        "key TEXT, "
        "value TEXT, "
        "description TEXT, "
        "list_order INTEGER, "
        "uuid TEXT NOT NULL DEFAULT \'\', "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "FOREIGN KEY(classifier_id) REFERENCES classifiers(id) "
    ");";

/*!
 *  \brief string constant to create an sql database index
 */
/*
static const char *DATA_DATABASE_CREATE_FEATUREORDERING_INDEX =
    "CREATE INDEX IF NOT EXISTS feature_ordering ON features ( "
        "list_order ASC "
    ");";
*/

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends featues by a uuid field.
 */
static const char *DATA_DATABASE_ALTER_FEATURE_TABLE_UUID =
    "ALTER TABLE features "
    "ADD COLUMN uuid TEXT NOT NULL DEFAULT \'\';";

/*!
 *  \brief string constant to update an sql database table
 *
 *  A uuid is generated and updated wherever missing.
 *  The subselect-WHERE clause is needed to generate for each row a different uuid.
 */
static const char *DATA_DATABASE_UPDATE_FEATURE_UUID =
    "UPDATE features SET uuid=(SELECT " DATA_DATABASE_CREATE_UUID " WHERE features.id!=-1) WHERE uuid=\'\';";

/*!
 *  \brief string constant to create an sql database table
 */
static const char *DATA_DATABASE_CREATE_DIAGRAM_TABLE =
    "CREATE TABLE IF NOT EXISTS diagrams ( "
        "id INTEGER PRIMARY KEY ASC, "
        "parent_id INTEGER, "  /* is NULL for the root diagram */
        "diagram_type INTEGER, "
        "name TEXT, "
        "description TEXT, "
        "list_order INTEGER, "
        "display_flags INTEGER NOT NULL DEFAULT 0, "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "uuid TEXT NOT NULL DEFAULT \'\', "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "FOREIGN KEY(parent_id) REFERENCES diagrams(id) "
    ");";

/*!
 *  \brief string constant to create an sql database index
 */
/*
static const char *DATA_DATABASE_CREATE_DIAGRAMORDERING_INDEX =
    "CREATE INDEX IF NOT EXISTS diagram_ordering ON diagrams ( "
        "list_order ASC "
    ");";
*/

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends diagrams by display_flags field.
 */
static const char *DATA_DATABASE_ALTER_DIAGRAM_TABLE_1 =
    "ALTER TABLE diagrams "
    "ADD COLUMN display_flags INTEGER NOT NULL DEFAULT 0;";

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends diagrams by a uuid field.
 */
static const char *DATA_DATABASE_ALTER_DIAGRAM_TABLE_UUID =
    "ALTER TABLE diagrams "
    "ADD COLUMN uuid TEXT NOT NULL DEFAULT \'\';";

/*!
 *  \brief string constant to update an sql database table
 *
 *  A uuid is generated and updated wherever missing.
 *  The subselect-WHERE clause is needed to generate for each row a different uuid.
 */
static const char *DATA_DATABASE_UPDATE_DIAGRAM_UUID =
    "UPDATE diagrams SET uuid=(SELECT " DATA_DATABASE_CREATE_UUID " WHERE diagrams.id!=-1) WHERE uuid=\'\';";

#if 0
/*!
 *  \brief string constant to update an sql database table
 *
 *  The root diagram is marked by parent_id = NULL, not by -1 anymore.
 */
static const char *DATA_DATABASE_UPDATE_DIAGRAM_ROOT_PARENT =
    "UPDATE diagrams SET parent_id=NULL WHERE parent_id=-1;";
#endif

/*!
 *  \brief string constant to create an sql database table
 */
static const char *DATA_DATABASE_CREATE_DIAGRAMELEMENT_TABLE =
    "CREATE TABLE IF NOT EXISTS diagramelements ( "
        "id INTEGER PRIMARY KEY ASC, "
        "diagram_id INTEGER, "
        "classifier_id INTEGER, "
        "display_flags INTEGER, "
        "focused_feature_id INTEGER DEFAULT NULL, "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "uuid TEXT NOT NULL DEFAULT \'\', "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "FOREIGN KEY(diagram_id) REFERENCES diagrams(id), "
        "FOREIGN KEY(classifier_id) REFERENCES classifiers(id), "
        "FOREIGN KEY(focused_feature_id) REFERENCES feature(id) "
    ");";

#if 0
/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format.
 *
 *  This command extends diagramelements by to_feature_id field.
 *  \see http://sqlite.org/lang_altertable.html
 */
static const char *DATA_DATABASE_ALTER_DIAGRAMELEMENT_TABLE_1 =
    "ALTER TABLE diagramelements "
    "ADD COLUMN focused_feature_id INTEGER DEFAULT NULL;";
#endif

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format.
 *
 *  This command extends diagramelements by a uuid field.
 *  \see http://sqlite.org/lang_altertable.html
 */
static const char *DATA_DATABASE_ALTER_DIAGRAMELEMENT_TABLE_UUID =
    "ALTER TABLE diagramelements "
    "ADD COLUMN uuid TEXT NOT NULL DEFAULT \'\';";

/*!
 *  \brief string constant to update an sql database table
 *
 *  A uuid is generated and updated wherever missing.
 *  The subselect-WHERE clause is needed to generate for each row a different uuid.
 */
static const char *DATA_DATABASE_UPDATE_DIAGRAMELEMENT_UUID =
    "UPDATE diagramelements SET uuid=(SELECT " DATA_DATABASE_CREATE_UUID " WHERE diagramelements.id!=-1) WHERE uuid=\'\';";

data_error_t data_database_private_initialize_tables( data_database_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_CLASSIFIER_TABLE );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_RELATIONSHIP_TABLE );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_FEATURE_TABLE );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_DIAGRAM_TABLE );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_DIAGRAMELEMENT_TABLE );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_private_initialize_indexes( data_database_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    /*
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_CLASSIFIERORDERING_INDEX );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_RELATIONSHIPORDERING_INDEX );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_FEATUREORDERING_INDEX );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_DIAGRAMORDERING_INDEX );
    */

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_private_upgrade_tables( data_database_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

#if 0
    /* update table relationships from version 1.0.0 to later versions */
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_1 );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_2 );

    /* update table diagramelements from version 1.0.0 to later versions */
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_DIAGRAMELEMENT_TABLE_1 );

    /* update table diagrams from version 1.0.0 to later versions */
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_DIAGRAM_ROOT_PARENT );

    /* update table classifiers from version 1.4.0 or earlier to 1.5.0 or later versions */
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_CLASSIFIER_TABLE_1 );
#endif

    /* update table classifiers from version 1.32.1 or earlier to later versions with diagram.display_flags */
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_DIAGRAM_TABLE_1 );

    /* update all 5 tables from version 1.32.1 (no uuid) to later versions with uuid field */
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_CLASSIFIER_TABLE_UUID );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_CLASSIFIER_UUID );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_UUID );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_RELATIONSHIP_UUID );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_FEATURE_TABLE_UUID );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_FEATURE_UUID );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_DIAGRAM_TABLE_UUID );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_DIAGRAM_UUID );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_DIAGRAMELEMENT_TABLE_UUID );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_DIAGRAMELEMENT_UUID );

    if ( ( result & DATA_ERROR_READ_ONLY_DB ) != DATA_ERROR_NONE )
    {
        TSLOG_EVENT( "sqlite3 database is read only." );
        result = result & ~DATA_ERROR_READ_ONLY_DB;
    }

    TRACE_END_ERR( result );
    return result;
}

void data_database_init ( data_database_t *this_ )
{
    TRACE_BEGIN();

    TSLOG_EVENT_STR( "sqlite3_libversion:", sqlite3_libversion() );

    (*this_).db_file_name = utf8stringbuf_init( sizeof((*this_).private_db_file_name_buffer), (*this_).private_db_file_name_buffer );
    utf8stringbuf_clear( (*this_).db_file_name );

    (*this_).is_open = false;

    g_mutex_init ( &((*this_).private_lock) );

    data_change_notifier_init ( &((*this_).notifier) );
    data_database_private_clear_db_listener_list( this_ );

    TRACE_END();
}

data_error_t data_database_private_open ( data_database_t *this_, const char* db_file_path, int sqlite3_flags )
{
    TRACE_BEGIN();
    assert( NULL != db_file_path );
    int sqlite_err;
    data_error_t result = DATA_ERROR_NONE;
    bool notify_listeners = false;

    result |= data_database_private_lock( this_ );

    if ( (*this_).is_open )
    {
        TRACE_INFO("data_database_open called on database that was not closed.");
        result |= DATA_ERROR_INVALID_REQUEST;
    }
    else
    {
        utf8stringbuf_copy_str( (*this_).db_file_name, db_file_path );

        TSLOG_EVENT_STR( "sqlite3_open_v2:", utf8stringbuf_get_string( (*this_).db_file_name ) );
        sqlite_err = sqlite3_open_v2( utf8stringbuf_get_string( (*this_).db_file_name ),
                                      &((*this_).db),
                                      sqlite3_flags,
                                      NULL
                                    );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_open_v2() failed:", sqlite_err );
            TSLOG_ERROR_STR( "sqlite3_open_v2() failed:", utf8stringbuf_get_string( (*this_).db_file_name ) );
            (*this_).is_open = false;
            result |= DATA_ERROR_NO_DB;  /* no db to use */
        }
        else
        {
            data_error_t init_err;
            init_err = data_database_private_initialize_tables( this_ );
            if ( init_err == DATA_ERROR_NONE )
            {
                init_err = data_database_private_initialize_indexes( this_ );
            }
            if ( init_err == DATA_ERROR_NONE )
            {
                init_err = data_database_private_upgrade_tables( this_ );
            }

            if ( init_err == DATA_ERROR_NONE )
            {
                (*this_).is_open = true;
                notify_listeners = true;
            }
            else
            {
                TSLOG_EVENT_STR( "sqlite3_close:", utf8stringbuf_get_string( (*this_).db_file_name ) );
                sqlite_err = sqlite3_close( (*this_).db );
                if ( SQLITE_OK != sqlite_err )
                {
                    TSLOG_ERROR_INT( "sqlite3_close() failed:", sqlite_err );
                }
                utf8stringbuf_clear( (*this_).db_file_name );
                (*this_).is_open = false;
            }
            result |= init_err;
        }
    }

    result |= data_database_private_unlock( this_ );

    if ( notify_listeners )
    {
        /* inform readers and writers on open */
        result |= data_database_private_notify_db_listeners( this_, DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED );

        /* inform listeners on changes */
        data_change_notifier_emit_signal( &((*this_).notifier),
                                          DATA_CHANGE_EVENT_TYPE_DB_OPENED,
                                          DATA_TABLE_VOID,
                                          DATA_ROW_ID_VOID,
                                          DATA_TABLE_VOID,
                                          DATA_ROW_ID_VOID
                                        );
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
        data_change_notifier_emit_signal( &((*this_).notifier),
                                          DATA_CHANGE_EVENT_TYPE_DB_PREPARE_CLOSE,
                                          DATA_TABLE_VOID,
                                          DATA_ROW_ID_VOID,
                                          DATA_TABLE_VOID,
                                          DATA_ROW_ID_VOID
                                        );

        /* inform readers and writers on close */
        result |= data_database_private_notify_db_listeners( this_, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    }

    result |= data_database_private_lock( this_ );

    if ( (*this_).is_open )
    {
        /* perform close */
        TSLOG_EVENT_STR( "sqlite3_close:", utf8stringbuf_get_string( (*this_).db_file_name ) );
        sqlite_err = sqlite3_close( (*this_).db );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_close() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }

        utf8stringbuf_clear( (*this_).db_file_name );
        (*this_).is_open = false;

        notify_change_listeners = true;
    }
    else
    {
        TRACE_INFO("data_database_close called on database that was not open.");
        result |= DATA_ERROR_INVALID_REQUEST;
    }

    result |= data_database_private_unlock( this_ );

    if ( notify_change_listeners )
    {
        /* inform listeners on changes */
        data_change_notifier_emit_signal( &((*this_).notifier),
                                          DATA_CHANGE_EVENT_TYPE_DB_CLOSED,
                                          DATA_TABLE_VOID,
                                          DATA_ROW_ID_VOID,
                                          DATA_TABLE_VOID,
                                          DATA_ROW_ID_VOID
        );
    }

    TRACE_END_ERR( result );
    return result;
}

void data_database_destroy ( data_database_t *this_ )
{
    TRACE_BEGIN();

    data_database_private_clear_db_listener_list( this_ );
    if ( (*this_).is_open )
    {
        data_database_close( this_ );
    }
    data_change_notifier_destroy( &((*this_).notifier) );

    /* g_mutex_clear ( &((*this_).private_lock) ); -- must not be called because this GMutex is not on the stack */

    TRACE_END();
}

data_error_t data_database_flush_caches ( data_database_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).is_open )
    {
        TSLOG_EVENT_INT( "sqlite3_libversion_number()", sqlite3_libversion_number() );
        if ( sqlite3_libversion_number() >= 3010000 )
        {
            /* available if sqlite newer than 2016-01-06 (3.10.0) */
#if ( SQLITE_VERSION_NUMBER >= 3010000 )
            int sqlite_err;
            TSLOG_EVENT( "sqlite3_db_cacheflush" );
            sqlite_err = sqlite3_db_cacheflush( (*this_).db );
            if ( SQLITE_OK != sqlite_err )
            {
                TSLOG_ERROR_INT( "sqlite3_db_cacheflush() failed:", sqlite_err );
                result = DATA_ERROR_AT_DB;
            }
#else
            TSLOG_WARNING_INT( "The compile-time version of sqlite3 did not provide the sqlite3_db_cacheflush() function.", SQLITE_VERSION_NUMBER );
#endif
        }
        else
        {
            TSLOG_WARNING_INT( "The runtime-time version of sqlite3 does not provide the sqlite3_db_cacheflush() function.", sqlite3_libversion_number() );
        }
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_trace_stats ( data_database_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).is_open )
    {
        sqlite3_int64 use;
        sqlite3_int64 max;

        use = sqlite3_memory_used();
        max = sqlite3_memory_highwater(false);
        TRACE_INFO_INT_INT( "sqlite3_memory_used/highwater():", use, max );
    }
    else
    {
        TRACE_INFO( "database not open." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_add_db_listener( data_database_t *this_, data_database_listener_t *listener )
{
    TRACE_BEGIN();
    assert( NULL != listener );
    data_error_t result = DATA_ERROR_NONE;
    bool already_registered = false;

    result |= data_database_private_lock( this_ );

    int pos = -1;
    for( int index = 0; index < GUI_DATABASE_MAX_LISTENERS; index ++ )
    {
        if ( NULL == (*this_).listener_list[index] )
        {
            pos = index;
        }
        if ( listener == (*this_).listener_list[index] )
        {
            already_registered = true;
        }
    }

    if ( already_registered )
    {
        TSLOG_ERROR( "Listener already registered." );
        result |= DATA_ERROR_INVALID_REQUEST;
    }
    else if ( -1 != pos )
    {
        (*this_).listener_list[pos] = listener;
    }
    else
    {
        TSLOG_ERROR_INT( "Maximum number of listeners reached.", GUI_DATABASE_MAX_LISTENERS );
        result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
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
        TSLOG_ERROR( "listener not found" );
        result |= DATA_ERROR_INVALID_REQUEST;
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
Copyright 2016-2021 Andreas Warnke

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
