/* File: data_database.c; Copyright and License: see below */

#include "storage/data_database.h"
#include "data_id.h"
#include "data_table.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include <assert.h>

const char DATA_DATABASE_SQLITE3_MAGIC[16]
    = {0x53, 0x51, 0x4c, 0x69, 0x74, 0x65, 0x20, 0x66, 0x6f, 0x72, 0x6d, 0x61, 0x74, 0x20, 0x33, 0x00 };

/*!
 *  \brief string constant to create an sql database table
 *
 *  This table contains head values
 */
static const char *DATA_DATABASE_CREATE_HEAD_TABLE =
    "CREATE TABLE IF NOT EXISTS head ( "
        "id INTEGER NOT NULL PRIMARY KEY ASC, "
        "key TEXT NOT NULL UNIQUE, "
        "value TEXT"
    ");";

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
        "id INTEGER NOT NULL PRIMARY KEY ASC, "
        "main_type INTEGER NOT NULL, "
        "stereotype TEXT, "
        "name TEXT UNIQUE, "
        "description TEXT, "
        "x_order INTEGER, "
        "y_order INTEGER, "
        "list_order INTEGER, "
        "uuid TEXT NOT NULL DEFAULT \'\'"  /* DEFAULT needed in case a new DB is modified by an old program version */
    ");";

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends classifiers by a uuid field.
 *  \see http://sqlite.org/lang_altertable.html
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
        "id INTEGER NOT NULL PRIMARY KEY ASC, "
        "main_type INTEGER NOT NULL, "
        "from_classifier_id INTEGER NOT NULL, "
        "to_classifier_id INTEGER NOT NULL, "
        "stereotype TEXT DEFAULT \'\', "  /* since 1.47.0, DEFAULT needed in case a new DB is modified by an old program version */
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
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends relationships by a stereotype field.
 */
static const char *DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_STEREOTYPE =
    "ALTER TABLE relationships "
    "ADD COLUMN stereotype TEXT DEFAULT \'\';";

/*!
 *  \brief string constant to create an sql database table
 *
 *  This table contains instances of attributes (which are properties which are features).
 *  \see http://www.omg.org/spec/UML/
 */
static const char *DATA_DATABASE_CREATE_FEATURE_TABLE =
    "CREATE TABLE IF NOT EXISTS features ( "
        "id INTEGER NOT NULL PRIMARY KEY ASC, "
        "main_type INTEGER NOT NULL, "
        "classifier_id INTEGER NOT NULL, "
        "key TEXT, "
        "value TEXT, "
        "description TEXT, "
        "list_order INTEGER, "
        "uuid TEXT NOT NULL DEFAULT \'\', "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "FOREIGN KEY(classifier_id) REFERENCES classifiers(id) "
    ");";

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
        "id INTEGER NOT NULL PRIMARY KEY ASC, "
        "parent_id INTEGER, "  /* is NULL for the root diagram */
        "diagram_type INTEGER NOT NULL, "
        "stereotype TEXT DEFAULT \'\', "  /* since 1.47.0, DEFAULT needed in case a new DB is modified by an old program version */
        "name TEXT, "
        "description TEXT, "
        "list_order INTEGER, "
        "display_flags INTEGER NOT NULL DEFAULT 0, "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "uuid TEXT NOT NULL DEFAULT \'\', "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "FOREIGN KEY(parent_id) REFERENCES diagrams(id) "
    ");";

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

/*!
 *  \brief string constant to update an sql database table
 *
 *  The DEFAULT clause is needed to convert the existing records to the new format
 *  and in case a new database is modified by an old program version.
 *
 *  This command extends diagrams by a stereotype field.
 */
static const char *DATA_DATABASE_ALTER_DIAGRAM_TABLE_STEREOTYPE =
    "ALTER TABLE diagrams "
    "ADD COLUMN stereotype TEXT DEFAULT \'\';";

/*!
 *  \brief string constant to create an sql database table
 */
static const char *DATA_DATABASE_CREATE_DIAGRAMELEMENT_TABLE =
    "CREATE TABLE IF NOT EXISTS diagramelements ( "
        "id INTEGER NOT NULL PRIMARY KEY ASC, "
        "diagram_id INTEGER NOT NULL, "
        "classifier_id INTEGER NOT NULL, "
        "display_flags INTEGER NOT NULL, "
        "focused_feature_id INTEGER DEFAULT NULL, "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "uuid TEXT NOT NULL DEFAULT \'\', "  /* DEFAULT needed in case a new DB is modified by an old program version */
        "FOREIGN KEY(diagram_id) REFERENCES diagrams(id), "
        "FOREIGN KEY(classifier_id) REFERENCES classifiers(id), "
        "FOREIGN KEY(focused_feature_id) REFERENCES features(id) "
    ");";

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

/*!
 *  \brief string constant to start a transaction
 *
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_BEGIN_TRANSACTION =
    "BEGIN TRANSACTION;";

/*!
 *  \brief string constant to commit a transaction
 *
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_COMMIT_TRANSACTION =
    "COMMIT TRANSACTION;";

u8_error_t data_database_private_initialize_tables( data_database_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_HEAD_TABLE, false );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_CLASSIFIER_TABLE, false );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_RELATIONSHIP_TABLE, false );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_FEATURE_TABLE, false );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_DIAGRAM_TABLE, false );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_CREATE_DIAGRAMELEMENT_TABLE, false );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_private_upgrade_tables( data_database_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    /* update table classifiers from version 1.32.1 or earlier to later versions with diagram.display_flags */
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_DIAGRAM_TABLE_1, true );

    /* update all 5 tables from version 1.32.1 (no uuid) to later versions with uuid field */
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_CLASSIFIER_TABLE_UUID, true );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_CLASSIFIER_UUID, true );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_UUID, true );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_RELATIONSHIP_UUID, true );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_FEATURE_TABLE_UUID, true );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_FEATURE_UUID, true );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_DIAGRAM_TABLE_UUID, true );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_DIAGRAM_UUID, true );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_DIAGRAMELEMENT_TABLE_UUID, true );
    result |= data_database_private_exec_sql( this_, DATA_DATABASE_UPDATE_DIAGRAMELEMENT_UUID, true );

    /* update table diagrams and relationships from version 1.46.0 or earlier to later versions with stereotype */
    /* do not care for "already existed" errors: */
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_RELATIONSHIP_TABLE_STEREOTYPE, true );
    data_database_private_exec_sql( this_, DATA_DATABASE_ALTER_DIAGRAM_TABLE_STEREOTYPE, true );

    if ( u8_error_contains( result, U8_ERROR_READ_ONLY_DB ) )
    {
        U8_LOG_EVENT( "sqlite3 database is read only." );
        result = u8_error_more_than( result, U8_ERROR_READ_ONLY_DB ) ? U8_ERROR_AT_DB : U8_ERROR_NONE;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

void data_database_init ( data_database_t *this_ )
{
    U8_TRACE_BEGIN();

    U8_LOG_EVENT_STR( "sqlite3_libversion:", sqlite3_libversion() );

    (*this_).db_file_name = utf8stringbuf_init( sizeof((*this_).private_db_file_name_buffer), (*this_).private_db_file_name_buffer );
    utf8stringbuf_clear( (*this_).db_file_name );

    g_mutex_init ( &((*this_).lock_on_write) );
    (*this_).locked_on_write = false;

    u8_error_t result = data_database_lock_on_write( this_ );
    {
        (*this_).db_state = DATA_DATABASE_STATE_CLOSED;
        (*this_).transaction_recursion = 0;
    }
    result |= data_database_unlock_on_write( this_ );
    if( result != U8_ERROR_NONE )
    {
        assert(false);
    }

    data_change_notifier_init ( &((*this_).notifier) );
    data_database_private_clear_db_listener_list( this_ );

    U8_TRACE_END();
}

u8_error_t data_database_private_open ( data_database_t *this_, const char* db_file_path, int sqlite3_flags )
{
    U8_TRACE_BEGIN();
    assert( NULL != db_file_path );
    /* there should not be pending transactions when calling open */
    assert( (*this_).transaction_recursion == 0 );
    int sqlite_err;
    u8_error_t result = U8_ERROR_NONE;
    bool notify_listeners = false;

    result |= data_database_lock_on_write( this_ );

    if ( (*this_).db_state != DATA_DATABASE_STATE_CLOSED )
    {
        U8_TRACE_INFO("data_database_open called on database that was not closed.");
        result |= U8_ERROR_INVALID_REQUEST;
    }
    else
    {
        utf8stringbuf_copy_str( (*this_).db_file_name, db_file_path );

        U8_LOG_EVENT_STR( "sqlite3_open_v2:", utf8stringbuf_get_string( (*this_).db_file_name ) );
        sqlite_err = sqlite3_open_v2( utf8stringbuf_get_string( (*this_).db_file_name ),
                                      &((*this_).db),
                                      sqlite3_flags,
                                      NULL
                                    );
        if ( SQLITE_OK != sqlite_err )
        {
            U8_LOG_ERROR_INT( "sqlite3_open_v2() failed:", sqlite_err );
            U8_LOG_ERROR_STR( "sqlite3_open_v2() failed:", utf8stringbuf_get_string( (*this_).db_file_name ) );
            (*this_).db_state = DATA_DATABASE_STATE_CLOSED;
            result |= U8_ERROR_NO_DB;  /* no db to use */
        }
        else
        {
            u8_error_t init_err;
            init_err = data_database_private_initialize_tables( this_ );
            if ( init_err == U8_ERROR_NONE )
            {
                init_err = data_database_private_upgrade_tables( this_ );
            }

            if ( init_err == U8_ERROR_NONE )
            {
                (*this_).db_state
                    = ( (sqlite3_flags & SQLITE_OPEN_MEMORY) == 0 )
                    ? DATA_DATABASE_STATE_OPEN
                    : DATA_DATABASE_STATE_IN_MEM;
                notify_listeners = true;
            }
            else
            {
                U8_LOG_EVENT_STR( "sqlite3_close:", utf8stringbuf_get_string( (*this_).db_file_name ) );
                sqlite_err = sqlite3_close( (*this_).db );
                if ( SQLITE_OK != sqlite_err )
                {
                    U8_LOG_ERROR_INT( "sqlite3_close() failed:", sqlite_err );
                }
                utf8stringbuf_clear( (*this_).db_file_name );
                (*this_).db_state = DATA_DATABASE_STATE_CLOSED;
            }
            result |= init_err;
        }
    }

    result |= data_database_unlock_on_write( this_ );

    if ( notify_listeners )
    {
        /* do sent notifications is active by default on a newly opened db */
        data_change_notifier_disable_stealth_mode( &((*this_).notifier) );

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

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_close ( data_database_t *this_ )
{
    U8_TRACE_BEGIN();
    /* there should not be pending transactions when calling cloas */
    assert( (*this_).transaction_recursion == 0 );
    int sqlite_err;
    u8_error_t result = U8_ERROR_NONE;
    bool notify_change_listeners = false;

    /* do sent notifications before closing: */
    if ( data_database_is_open( this_ ) )
    {
        /* do sent notifications when closing: */
        data_change_notifier_disable_stealth_mode( &((*this_).notifier) );

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

    result |= data_database_lock_on_write( this_ );

    if ( (*this_).db_state != DATA_DATABASE_STATE_CLOSED )
    {
        /* perform close */
        U8_LOG_EVENT_STR( "sqlite3_close:", utf8stringbuf_get_string( (*this_).db_file_name ) );
        sqlite_err = sqlite3_close( (*this_).db );
        if ( SQLITE_OK != sqlite_err )
        {
            U8_LOG_ERROR_INT( "sqlite3_close() failed:", sqlite_err );
            result |= U8_ERROR_AT_DB;
        }

        utf8stringbuf_clear( (*this_).db_file_name );
        (*this_).db_state = DATA_DATABASE_STATE_CLOSED;
        (*this_).transaction_recursion = 0;

        notify_change_listeners = true;
    }
    else
    {
        U8_TRACE_INFO("data_database_close called on database that was not open.");
        result |= U8_ERROR_INVALID_REQUEST;
    }

    result |= data_database_unlock_on_write( this_ );

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

    U8_TRACE_END_ERR( result );
    return result;
}

void data_database_destroy ( data_database_t *this_ )
{
    U8_TRACE_BEGIN();
    /* there should not be pending transactions when calling destroy */
    assert( (*this_).transaction_recursion == 0 );

    data_database_private_clear_db_listener_list( this_ );
    if ( data_database_is_open( this_ ) )
    {
        data_database_close( this_ );
    }
    data_change_notifier_destroy( &((*this_).notifier) );

    u8_error_t result = data_database_lock_on_write( this_ );
    {
        (*this_).transaction_recursion = 0;
    }
    result |= data_database_unlock_on_write( this_ );
    if( result != U8_ERROR_NONE )
    {
        assert(false);
    }

    /* g_mutex_clear ( &((*this_).lock_on_write) ); -- must not be called because this GMutex is not on the stack */

    U8_TRACE_END();
}

u8_error_t data_database_flush_caches ( data_database_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( data_database_is_open( this_ ) )
    {
        U8_LOG_EVENT_INT( "sqlite3_libversion_number()", sqlite3_libversion_number() );
        if ( sqlite3_libversion_number() >= 3010000 )
        {
            /* available if sqlite newer than 2016-01-06 (3.10.0) */
#if ( SQLITE_VERSION_NUMBER >= 3010000 )
            int sqlite_err;
            U8_LOG_EVENT( "sqlite3_db_cacheflush" );
            sqlite_err = sqlite3_db_cacheflush( (*this_).db );
            if ( SQLITE_OK != sqlite_err )
            {
                U8_LOG_ERROR_INT( "sqlite3_db_cacheflush() failed:", sqlite_err );
                result = U8_ERROR_AT_DB;
            }
#else
            U8_LOG_WARNING_INT( "The compile-time version of sqlite3 did not provide the sqlite3_db_cacheflush() function.",
                                SQLITE_VERSION_NUMBER
                              );
#endif
        }
        else
        {
            U8_LOG_WARNING_INT( "The runtime-time version of sqlite3 does not provide the sqlite3_db_cacheflush() function.",
                                sqlite3_libversion_number()
                              );
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_trace_stats ( data_database_t *this_ )
{
    U8_TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    if ( data_database_is_open( this_ ) )
    {
        sqlite3_int64 use;
        sqlite3_int64 max;

        use = sqlite3_memory_used();
        max = sqlite3_memory_highwater(false);
        U8_TRACE_INFO_INT_INT( "sqlite3_memory_used/highwater():", use, max );
    }
    else
    {
        U8_TRACE_INFO( "database not open." );
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_add_db_listener( data_database_t *this_, data_database_listener_t *listener )
{
    U8_TRACE_BEGIN();
    assert( NULL != listener );
    u8_error_t result = U8_ERROR_NONE;
    bool already_registered = false;

    result |= data_database_lock_on_write( this_ );

    int pos = -1;
    for( int index = 0; index < DATA_DATABASE_MAX_LISTENERS; index ++ )
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
        U8_LOG_ERROR( "Listener already registered." );
        result |= U8_ERROR_INVALID_REQUEST;
    }
    else if ( -1 != pos )
    {
        (*this_).listener_list[pos] = listener;
    }
    else
    {
        U8_LOG_ERROR_INT( "Maximum number of listeners reached.", DATA_DATABASE_MAX_LISTENERS );
        result |= U8_ERROR_ARRAY_BUFFER_EXCEEDED;
    }

    result |= data_database_unlock_on_write( this_ );

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_remove_db_listener( data_database_t *this_, data_database_listener_t *listener )
{
    U8_TRACE_BEGIN();

    assert( NULL != listener );

    u8_error_t result = U8_ERROR_NONE;
    int count_closed = 0;

    result |= data_database_lock_on_write( this_ );

    for( int index = 0; index < DATA_DATABASE_MAX_LISTENERS; index ++ )
    {
        if ( (*this_).listener_list[index] == listener )
        {
            (*this_).listener_list[index] = NULL;
            count_closed ++;
        }
    }

    result |= data_database_unlock_on_write( this_ );

    if ( count_closed == 0 )
    {
        U8_LOG_ERROR( "listener not found" );
        result |= U8_ERROR_INVALID_REQUEST;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_private_notify_db_listeners( data_database_t *this_, data_database_listener_signal_t signal_id )
{
    U8_TRACE_BEGIN();
    data_database_listener_t *(listener_list_copy[DATA_DATABASE_MAX_LISTENERS]);
    u8_error_t result = U8_ERROR_NONE;

    result |= data_database_lock_on_write( this_ );

    memcpy( listener_list_copy, (*this_).listener_list, sizeof(listener_list_copy) );

    result |= data_database_unlock_on_write( this_ );

    for( int index = 0; index < DATA_DATABASE_MAX_LISTENERS; index ++ )
    {
        if ( NULL != listener_list_copy[index] )
        {
            data_database_listener_notify( listener_list_copy[index], signal_id );
        }
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_transaction_begin ( data_database_t *this_ )
{
    U8_TRACE_BEGIN();
    /* nesting of transactions should not be greater than 2. You may increase this limit if needed. */
    assert( (*this_).transaction_recursion < 2 );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( this_ );

    if ( data_database_is_open( this_ ) )
    {
        if ( (*this_).transaction_recursion == 0 )
        {
            U8_LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_BEGIN_TRANSACTION );
            sqlite_err = sqlite3_exec( db, DATA_DATABASE_BEGIN_TRANSACTION, NULL, NULL, &error_msg );
            if ( SQLITE_OK != sqlite_err )
            {
                U8_LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_BEGIN_TRANSACTION );
                U8_LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
                result |= U8_ERROR_AT_DB;
            }
            if ( error_msg != NULL )
            {
                U8_LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
                sqlite3_free( error_msg );
                error_msg = NULL;
            }
        }
        (*this_).transaction_recursion ++;
    }
    else
    {
        U8_LOG_WARNING_STR( "database not open. cannot execute", DATA_DATABASE_BEGIN_TRANSACTION );
        result = U8_ERROR_NO_DB;
    }

    U8_TRACE_END_ERR( result );
    return result;
}

u8_error_t data_database_transaction_commit ( data_database_t *this_ )
{
    U8_TRACE_BEGIN();
    /* there should be at least 1 pending transaction */
    assert( (*this_).transaction_recursion > 0 );
    u8_error_t result = U8_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( this_ );

    if ( data_database_is_open( this_ ) )
    {
        if ( (*this_).transaction_recursion == 1 )
        {
            U8_LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_COMMIT_TRANSACTION );
            sqlite_err = sqlite3_exec( db, DATA_DATABASE_COMMIT_TRANSACTION, NULL, NULL, &error_msg );
            if ( SQLITE_OK != sqlite_err )
            {
                U8_LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_COMMIT_TRANSACTION );
                U8_LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
                result |= U8_ERROR_AT_DB;
            }
            if ( error_msg != NULL )
            {
                U8_LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
                sqlite3_free( error_msg );
                error_msg = NULL;
            }
        }
        (*this_).transaction_recursion --;
    }
    else
    {
        U8_LOG_WARNING_STR( "database not open. cannot execute", DATA_DATABASE_COMMIT_TRANSACTION );
        result = U8_ERROR_NO_DB;
    }

    U8_TRACE_END_ERR( result );
    return result;
}


/*
Copyright 2016-2023 Andreas Warnke

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
