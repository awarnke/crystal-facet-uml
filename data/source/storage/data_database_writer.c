/* File: data_database_writer.c; Copyright and License: see below */

#include "storage/data_database_writer.h"
#include "data_id.h"
#include "trace.h"
#include "tslog.h"
#include <sqlite3.h>
#include <assert.h>

/*!
 *  \brief string constant to start a transaction
 *
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_WRITER_BEGIN_TRANSACTION =
    "BEGIN TRANSACTION;";

/*!
 *  \brief string constant to commit a transaction
 *
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_WRITER_COMMIT_TRANSACTION =
    "COMMIT TRANSACTION;";

void data_database_writer_init ( data_database_writer_t *this_, data_database_reader_t *db_reader, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    int perr;

    (*this_).database = database;
    (*this_).db_reader = db_reader;

    perr = pthread_mutex_init ( &((*this_).private_lock), NULL );
    if ( perr != 0 )
    {
        TSLOG_ERROR_INT( "pthread_mutex_init() failed:", perr );
    }

    data_database_sql_builder_init( &((*this_).sql_builder) );

    data_database_listener_init ( &((*this_).me_as_listener), this_, (void (*)(void*,data_database_listener_signal_t)) &data_database_writer_db_change_callback );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    TRACE_END();
}

void data_database_writer_destroy ( data_database_writer_t *this_ )
{
    TRACE_BEGIN();
    int perr;

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    data_database_sql_builder_destroy( &((*this_).sql_builder) );

    perr = pthread_mutex_destroy ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        TSLOG_ERROR_INT( "pthread_mutex_destroy() failed:", perr );
    }

    (*this_).db_reader = NULL;
    (*this_).database = NULL;

    TRACE_END();
}

data_error_t data_database_writer_private_execute_create_command ( data_database_writer_t *this_, const char* sql_statement, int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != sql_statement );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( (*this_).database );

    if ( data_database_is_open( (*this_).database ) )
    {
        TSLOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_BEGIN_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }

        TSLOG_EVENT_STR( "sqlite3_exec:", sql_statement );
        sqlite_err = sqlite3_exec( db, sql_statement, NULL, NULL, &error_msg );
        if ( SQLITE_CONSTRAINT == (0xff & sqlite_err) )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed due to UNIQUE constraint:", sql_statement );
            result |= DATA_ERROR_DUPLICATE_NAME;
        }
        else if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", sql_statement );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }

        if ( NULL != out_new_id )
        {
            if ( SQLITE_OK == sqlite_err )
            {
                int64_t new_id;
                new_id = sqlite3_last_insert_rowid(db);
                TSLOG_EVENT_INT( "sqlite3_last_insert_rowid():", new_id );
                *out_new_id = new_id;
            }
        }

        TSLOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_COMMIT_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        TSLOG_WARNING_STR( "database not open. cannot execute", sql_statement );
        result = DATA_ERROR_NO_DB;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_private_transaction_begin ( data_database_writer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( (*this_).database );

    if ( data_database_is_open( (*this_).database ) )
    {
        TSLOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_BEGIN_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        TSLOG_WARNING_STR( "database not open. cannot execute", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
        result = DATA_ERROR_NO_DB;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_private_transaction_commit ( data_database_writer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( (*this_).database );

    if ( data_database_is_open( (*this_).database ) )
    {
        TSLOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_COMMIT_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        TSLOG_WARNING_STR( "database not open. cannot execute", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
        result = DATA_ERROR_NO_DB;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_private_transaction_issue_command ( data_database_writer_t *this_, const char* sql_statement )
{
    TRACE_BEGIN();
    assert( NULL != sql_statement );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( (*this_).database );

    if ( data_database_is_open( (*this_).database ) )
    {
        TSLOG_EVENT_STR( "sqlite3_exec:", sql_statement );
        sqlite_err = sqlite3_exec( db, sql_statement, NULL, NULL, &error_msg );
        if ( SQLITE_CONSTRAINT == (0xff & sqlite_err) )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed due to UNIQUE constraint:", sql_statement );
            result |= DATA_ERROR_DUPLICATE_NAME;
        }
        else if ( SQLITE_OK != sqlite_err )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", sql_statement );
            TSLOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            TSLOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        TSLOG_WARNING_STR( "database not open. cannot execute", sql_statement );
        result = DATA_ERROR_NO_DB;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_create_diagram ( data_database_writer_t *this_,
                                                   const data_diagram_t *diagram,
                                                   int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != diagram );
    data_error_t result = DATA_ERROR_NONE;
    int64_t new_id;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_sql_builder_build_create_diagram_command( &((*this_).sql_builder), diagram );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_execute_create_command( this_, sql_cmd, &new_id );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, new_id );

    if ( NULL != out_new_id )
    {
        *out_new_id = new_id;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_description ( data_database_writer_t *this_,
                                                               int64_t diagram_id,
                                                               const char* new_diagram_description,
                                                               data_diagram_t *out_old_diagram )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram_description );
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagram is NULL if old data shall not be returned */
    if ( NULL != out_old_diagram )
    {
        result |= data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_id, out_old_diagram );
    }

    result |= data_database_sql_builder_build_update_diagram_description_cmd( &((*this_).sql_builder), diagram_id, new_diagram_description );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, diagram_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_name ( data_database_writer_t *this_,
                                                        int64_t diagram_id,
                                                        const char* new_diagram_name,
                                                        data_diagram_t *out_old_diagram )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram_name );
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagram is NULL if old data shall not be returned */
    if ( NULL != out_old_diagram )
    {
        result |= data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_id, out_old_diagram );
    }

    result |= data_database_sql_builder_build_update_diagram_name_cmd( &((*this_).sql_builder), diagram_id, new_diagram_name );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, diagram_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_type ( data_database_writer_t *this_,
                                                        int64_t diagram_id,
                                                        data_diagram_type_t new_diagram_type,
                                                        data_diagram_t *out_old_diagram )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagram is NULL if old data shall not be returned */
    if ( NULL != out_old_diagram )
    {
        result |= data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_id, out_old_diagram );
    }

    result |= data_database_sql_builder_build_update_diagram_type_cmd( &((*this_).sql_builder), diagram_id, new_diagram_type );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, diagram_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_list_order ( data_database_writer_t *this_,
                                                              int64_t diagram_id,
                                                              int32_t new_diagram_list_order,
                                                              data_diagram_t *out_old_diagram )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagram is NULL if old data shall not be returned */
    if ( NULL != out_old_diagram )
    {
        result |= data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_id, out_old_diagram );
    }

    result |= data_database_sql_builder_build_update_diagram_list_order_cmd( &((*this_).sql_builder), diagram_id, new_diagram_list_order );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, diagram_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_parent_id ( data_database_writer_t *this_,
                                                             int64_t diagram_id,
                                                             int64_t new_diagram_parent_id,
                                                             data_diagram_t *out_old_diagram )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagram is NULL if old data shall not be returned */
    if ( NULL != out_old_diagram )
    {
        result |= data_database_reader_get_diagram_by_id ( (*this_).db_reader, diagram_id, out_old_diagram );
    }

    result |= data_database_sql_builder_build_update_diagram_parent_id_cmd( &((*this_).sql_builder), diagram_id, new_diagram_parent_id );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, diagram_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_create_classifier( data_database_writer_t *this_,
                                                     const data_classifier_t *classifier,
                                                     int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != classifier );
    data_error_t result = DATA_ERROR_NONE;
    int64_t new_id;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_sql_builder_build_create_classifier_command( &((*this_).sql_builder), classifier );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_execute_create_command( this_, sql_cmd, &new_id );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, new_id );

    if ( NULL != out_new_id )
    {
        *out_new_id = new_id;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_stereotype ( data_database_writer_t *this_,
                                                                 int64_t classifier_id,
                                                                 const char* new_classifier_stereotype,
                                                                 data_classifier_t *out_old_classifier )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_classifier is NULL if old data shall not be returned */
    if ( NULL != out_old_classifier )
    {
        result |= data_database_reader_get_classifier_by_id ( (*this_).db_reader, classifier_id, out_old_classifier );
    }

    result |= data_database_sql_builder_build_update_classifier_stereotype_cmd( &((*this_).sql_builder), classifier_id, new_classifier_stereotype );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_description ( data_database_writer_t *this_,
                                                                  int64_t classifier_id,
                                                                  const char* new_classifier_description,
                                                                  data_classifier_t *out_old_classifier )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_classifier is NULL if old data shall not be returned */
    if ( NULL != out_old_classifier )
    {
        result |= data_database_reader_get_classifier_by_id ( (*this_).db_reader, classifier_id, out_old_classifier );
    }

    result |= data_database_sql_builder_build_update_classifier_description_cmd( &((*this_).sql_builder), classifier_id, new_classifier_description );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_name ( data_database_writer_t *this_,
                                                           int64_t classifier_id,
                                                           const char* new_classifier_name,
                                                           data_classifier_t *out_old_classifier )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_classifier is NULL if old data shall not be returned */
    if ( NULL != out_old_classifier )
    {
        result |= data_database_reader_get_classifier_by_id ( (*this_).db_reader, classifier_id, out_old_classifier );
    }

    result |= data_database_sql_builder_build_update_classifier_name_cmd( &((*this_).sql_builder), classifier_id, new_classifier_name );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_main_type ( data_database_writer_t *this_,
                                                                int64_t classifier_id,
                                                                data_classifier_type_t new_classifier_main_type,
                                                                data_classifier_t *out_old_classifier )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_classifier is NULL if old data shall not be returned */
    if ( NULL != out_old_classifier )
    {
        result |= data_database_reader_get_classifier_by_id ( (*this_).db_reader, classifier_id, out_old_classifier );
    }

    result |= data_database_sql_builder_build_update_classifier_main_type_cmd( &((*this_).sql_builder), classifier_id, new_classifier_main_type );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_x_order ( data_database_writer_t *this_,
                                                              int64_t classifier_id,
                                                              int32_t new_classifier_x_order,
                                                              data_classifier_t *out_old_classifier )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_classifier is NULL if old data shall not be returned */
    if ( NULL != out_old_classifier )
    {
        result |= data_database_reader_get_classifier_by_id ( (*this_).db_reader, classifier_id, out_old_classifier );
    }

    result |= data_database_sql_builder_build_update_classifier_x_order_cmd( &((*this_).sql_builder), classifier_id, new_classifier_x_order );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_y_order ( data_database_writer_t *this_,
                                                              int64_t classifier_id,
                                                              int32_t new_classifier_y_order,
                                                              data_classifier_t *out_old_classifier )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_classifier is NULL if old data shall not be returned */
    if ( NULL != out_old_classifier )
    {
        result |= data_database_reader_get_classifier_by_id ( (*this_).db_reader, classifier_id, out_old_classifier );
    }

    result |= data_database_sql_builder_build_update_classifier_y_order_cmd( &((*this_).sql_builder), classifier_id, new_classifier_y_order );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_create_diagramelement( data_database_writer_t *this_,
                                                         const data_diagramelement_t *diagramelement,
                                                         int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != diagramelement );
    data_error_t result = DATA_ERROR_NONE;
    int64_t new_id;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_sql_builder_build_create_diagramelement_command( &((*this_).sql_builder), diagramelement );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_execute_create_command( this_, sql_cmd, &new_id );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAMELEMENT, new_id );

    if ( NULL != out_new_id )
    {
        *out_new_id = new_id;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagramelement_display_flags ( data_database_writer_t *this_,
                                                                        int64_t diagramelement_id,
                                                                        data_diagramelement_flag_t new_display_flags,
                                                                        data_diagramelement_t *out_old_diagramelement )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagramelement is NULL if old data shall not be returned */
    if ( NULL != out_old_diagramelement )
    {
        result |= data_database_reader_get_diagramelement_by_id ( (*this_).db_reader, diagramelement_id, out_old_diagramelement );
    }

    result |= data_database_sql_builder_build_update_diagramelement_display_flags_cmd( &((*this_).sql_builder), diagramelement_id, new_display_flags );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAMELEMENT, diagramelement_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_delete_diagramelement( data_database_writer_t *this_,
                                                         int64_t obj_id,
                                                         data_diagramelement_t *out_old_diagramelement )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagramelement is NULL if old data shall not be returned */
    if ( NULL != out_old_diagramelement )
    {
        result |= data_database_reader_get_diagramelement_by_id ( (*this_).db_reader, obj_id, out_old_diagramelement );
    }

    result |= data_database_sql_builder_build_delete_diagramelement_command ( &((*this_).sql_builder), obj_id );
    char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );

    result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAMELEMENT, obj_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_delete_classifier( data_database_writer_t *this_,
                                                     int64_t obj_id,
                                                     data_classifier_t *out_old_classifier )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    bool object_still_referenced;
    data_diagram_t referencing_diagram[1];
    uint32_t referencing_diagram_count;
    data_error_t reference_check_err;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_classifier is NULL if old data shall not be returned */
    if ( NULL != out_old_classifier )
    {
        result |= data_database_reader_get_classifier_by_id ( (*this_).db_reader, obj_id, out_old_classifier );
    }

    /* Note: This function fails if the classifier is still referenced. */
    reference_check_err = data_database_reader_get_diagrams_by_classifier_id ( (*this_).db_reader, obj_id, 1, &referencing_diagram, &referencing_diagram_count );
    if ( ( 0 != referencing_diagram_count ) || ( ( reference_check_err & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & DATA_ERROR_MASK ) != 0 ) )
    {
        object_still_referenced = true;
    }
    else
    {
        object_still_referenced = false;
    }

    if ( object_still_referenced )
    {
        result |= DATA_ERROR_OBJECT_STILL_REFERENCED;
    }
    else
    {
        result |= data_database_sql_builder_build_delete_classifier_command ( &((*this_).sql_builder), obj_id );
        char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );
        result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );
    }

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, obj_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_delete_diagram ( data_database_writer_t *this_,
                                                   int64_t obj_id,
                                                   data_diagram_t *out_old_diagram )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    bool object_still_referenced;
    data_diagram_t referencing_diagram[1];
    uint32_t referencing_diagram_count;
    data_visible_classifier_t referencing_classifier[1];
    uint32_t referencing_classifier_count;
    data_error_t reference_check_err;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagram is NULL if old data shall not be returned */
    if ( NULL != out_old_diagram )
    {
        result |= data_database_reader_get_diagram_by_id ( (*this_).db_reader, obj_id, out_old_diagram );
    }

    /* Note: This function fails if the diagram is still referenced. */
    reference_check_err = data_database_reader_get_diagrams_by_parent_id ( (*this_).db_reader, obj_id, 1, &referencing_diagram, &referencing_diagram_count );
    if ( ( 0 != referencing_diagram_count ) || ( ( reference_check_err & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & DATA_ERROR_MASK ) != 0 ) )
    {
        object_still_referenced = true;
    }
    else
    {
        reference_check_err = data_database_reader_get_classifiers_by_diagram_id ( (*this_).db_reader, obj_id, 1, &referencing_classifier, &referencing_classifier_count );
        if ( ( 0 != referencing_classifier_count ) || ( ( reference_check_err & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & DATA_ERROR_MASK ) != 0 ) )
        {
            object_still_referenced = true;
        }
        else
        {
            object_still_referenced = false;
        }
    }

    if ( object_still_referenced )
    {
        result |= DATA_ERROR_OBJECT_STILL_REFERENCED;
    }
    else
    {
        result |= data_database_sql_builder_build_delete_diagram_command ( &((*this_).sql_builder), obj_id );
        char *sql_cmd = data_database_sql_builder_get_string_ptr( &((*this_).sql_builder) );
        result |= data_database_writer_private_transaction_issue_command ( this_, sql_cmd );
    }

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, obj_id );

    TRACE_END_ERR( result );
    return result;
}

void data_database_writer_db_change_callback ( data_database_writer_t *this_, data_database_listener_signal_t signal_id )
{
    TRACE_BEGIN();

    switch ( signal_id )
    {
        case DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE" );
        }
        break;

        case DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED" );
        }
        break;

        default:
        {
            TSLOG_ERROR( "unexpected data_database_listener_signal_t" );
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
