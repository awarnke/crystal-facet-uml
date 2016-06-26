/* File: data_database_writer.c; Copyright and License: see below */

#include "storage/data_database_writer.h"
#include "data_id.h"
#include "trace.h"
#include "log.h"
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

/*!
 *  \brief prefix string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_PREFIX =
    "INSERT INTO diagrams (parent_id,diagram_type,name,description,list_order) VALUES (";

/*!
 *  \brief postfix string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_POSTFIX = ");";

/*!
 *  \brief value separator string constant to insert a diagram or classifier or other table-row
 */
static const char *DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR = ",";

/*!
 *  \brief string start marker string constant to insert/update a diagram
 */
static const char *DATA_DATABASE_WRITER_STRING_VALUE_START = "\'";

/*!
 *  \brief string end marker string constant to insert/update a diagram
 */
static const char *DATA_DATABASE_WRITER_STRING_VALUE_END = "\'";

/*!
 *  \brief prefix string constant to insert a classifier
 */
static const char *DATA_DATABASE_WRITER_INSERT_CLASSIFIER_PREFIX =
    "INSERT INTO classifiers (main_type,stereotype,name,description,x_order,y_order) VALUES (";

/*!
 *  \brief postfix string constant to insert a classifier
 */
static const char *DATA_DATABASE_WRITER_INSERT_CLASSIFIER_POSTFIX = ");";

/*!
 *  \brief prefix string constant to insert a diagramelement
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAMELEMENT_PREFIX =
    "INSERT INTO diagramelements (diagram_id,classifier_id,display_flags) VALUES (";

/*!
 *  \brief postfix string constant to insert a diagramelement
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAMELEMENT_POSTFIX = ");";

/*!
 *  \brief prefix string constant to update a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_PREFIX = "UPDATE diagrams SET ";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_TYPE = "diagram_type=";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_NAME = "name=";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_DESCRIPTION = "description=";

/*!
 *  \brief infix string constant to update a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_POSTFIX = ";";

/*!
 *  \brief prefix string constant to update a classifier
 */
static const char *DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_PREFIX = "UPDATE classifiers SET ";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_COL_STEREOTYPE = "stereotype=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_COL_MAIN_TYPE = "main_type=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_COL_NAME = "name=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_COL_DESCRIPTION = "description=";

/*!
 *  \brief infix string constant to update a classifier
 */
static const char *DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a classifier
 */
static const char *DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_POSTFIX = ";";

/*!
 *  \brief prefix string constant to delete a diagram
 */
static const char *DATA_DATABASE_WRITER_DELETE_DIAGRAM_PREFIX =
    "DELETE FROM diagrams WHERE (id=";

/*!
 *  \brief postfix string constant to delete a diagram
 */
static const char *DATA_DATABASE_WRITER_DELETE_DIAGRAM_POSTFIX = ");";

/*!
 *  \brief prefix string constant to delete a classifier
 */
static const char *DATA_DATABASE_WRITER_DELETE_CLASSIFIER_PREFIX =
    "DELETE FROM classifiers WHERE (id=";

/*!
 *  \brief postfix string constant to delete a classifier
 */
static const char *DATA_DATABASE_WRITER_DELETE_CLASSIFIER_POSTFIX = ");";

/*!
 *  \brief prefix string constant to delete a diagramelement
 */
static const char *DATA_DATABASE_WRITER_DELETE_DIAGRAMELEMENT_PREFIX =
    "DELETE FROM diagramelements WHERE (id=";

/*!
 *  \brief postfix string constant to delete a diagramelement
 */
static const char *DATA_DATABASE_WRITER_DELETE_DIAGRAMELEMENT_POSTFIX = ");";

/*!
 *  \brief translation table to encode strings for usage in string literals
 *
 *  Note: This table is not suitable for searches using the LIKE operator because _ and % are not handled.
 */
const char *const DATA_DATABASE_WRITER_SQL_ENCODE[] = {
    "'", "''",
    NULL,
};

void data_database_writer_init ( data_database_writer_t *this_, data_database_reader_t *db_reader, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    int perr;

    (*this_).database = database;
    (*this_).db_reader = db_reader;
    (*this_).private_temp_stringbuf = utf8stringbuf_init( sizeof((*this_).private_temp_buffer), (*this_).private_temp_buffer );
    (*this_).private_sql_stringbuf = utf8stringbuf_init( sizeof((*this_).private_sql_buffer), (*this_).private_sql_buffer );

    perr = pthread_mutex_init ( &((*this_).private_lock), NULL );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_init() failed:", perr );
    }

    data_database_listener_init ( &((*this_).me_as_listener), this_, (void (*)(void*,data_database_listener_signal_t)) &data_database_writer_db_change_callback );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    TRACE_END();
}

void data_database_writer_destroy ( data_database_writer_t *this_ )
{
    TRACE_BEGIN();
    int perr;

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    perr = pthread_mutex_destroy ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_destroy() failed:", perr );
    }

    (*this_).db_reader = NULL;
    (*this_).database = NULL;

    TRACE_END();
}

data_error_t data_database_writer_private_build_create_diagram_command ( data_database_writer_t *this_, const data_diagram_t *diagram )
{
    TRACE_BEGIN();
    assert( NULL != diagram );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).parent_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).diagram_type );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*diagram).name );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*diagram).description );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).list_order );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_diagram_name_cmd ( data_database_writer_t *this_, int64_t diagram_id, const char *new_diagram_name )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram_name );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_NAME );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).private_temp_stringbuf, new_diagram_name );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_diagram_description_cmd ( data_database_writer_t *this_, int64_t diagram_id, const char *new_diagram_description )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram_description );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_DESCRIPTION );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).private_temp_stringbuf, new_diagram_description );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_diagram_type_cmd ( data_database_writer_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_TYPE );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, new_diagram_type );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_delete_diagram_command ( data_database_writer_t *this_, int64_t diagram_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    utf8stringbuf_clear( (*this_).private_sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_DELETE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_DELETE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_create_classifier_command ( data_database_writer_t *this_, const data_classifier_t *classifier )
{
    TRACE_BEGIN();
    assert( NULL != classifier );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    utf8stringbuf_clear( (*this_).private_sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*classifier).main_type );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*classifier).stereotype );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*classifier).name );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*classifier).description );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*classifier).x_order );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*classifier).y_order );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_classifier_stereotype_cmd ( data_database_writer_t *this_, int64_t classifier_id, const char *new_classifier_stereotype )
{
    TRACE_BEGIN();
    assert( NULL != new_classifier_stereotype );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_COL_STEREOTYPE );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).private_temp_stringbuf, new_classifier_stereotype );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_classifier_name_cmd ( data_database_writer_t *this_, int64_t classifier_id, const char *new_classifier_name )
{
    TRACE_BEGIN();
    assert( NULL != new_classifier_name );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_COL_NAME );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).private_temp_stringbuf, new_classifier_name );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_classifier_description_cmd ( data_database_writer_t *this_, int64_t classifier_id, const char *new_classifier_description )
{
    TRACE_BEGIN();
    assert( NULL != new_classifier_description );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_COL_DESCRIPTION );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).private_temp_stringbuf, new_classifier_description );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_classifier_main_type_cmd ( data_database_writer_t *this_, int64_t classifier_id, data_classifier_type_t new_classifier_main_type )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_COL_MAIN_TYPE );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, new_classifier_main_type );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_delete_classifier_command ( data_database_writer_t *this_, int64_t classifier_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    utf8stringbuf_clear( (*this_).private_sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_DELETE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_DELETE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_create_diagramelement_command ( data_database_writer_t *this_, const data_diagramelement_t *diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != diagramelement );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    utf8stringbuf_clear( (*this_).private_sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAMELEMENT_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagramelement).diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagramelement).classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagramelement).display_flags );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAMELEMENT_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_delete_diagramelement_command ( data_database_writer_t *this_, int64_t diagramelement_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    utf8stringbuf_clear( (*this_).private_sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_DELETE_DIAGRAMELEMENT_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, diagramelement_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_DELETE_DIAGRAMELEMENT_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_execute_single_command ( data_database_writer_t *this_, const char* sql_statement, bool fetch_new_id, int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != sql_statement );
    assert( !(fetch_new_id) || (NULL != out_new_id) );
    int64_t new_id;
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( (*this_).database );

    if ( data_database_is_open( (*this_).database ) )
    {
        LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_BEGIN_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
            LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }

        LOG_EVENT_STR( "sqlite3_exec:", sql_statement );
        sqlite_err = sqlite3_exec( db, sql_statement, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", sql_statement );
            LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }

        if ( fetch_new_id && ( NULL != out_new_id ))
        {
            if ( SQLITE_OK == sqlite_err )
            {
                new_id = sqlite3_last_insert_rowid(db);
                LOG_EVENT_INT( "sqlite3_last_insert_rowid():", new_id );
                *out_new_id = new_id;
            }
        }

        LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_COMMIT_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
            LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        LOG_WARNING_STR( "database not open. cannot execute", sql_statement );
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
        LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_BEGIN_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
            LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        LOG_WARNING_STR( "database not open. cannot execute", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
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
        LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
        sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_COMMIT_TRANSACTION, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
            LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        LOG_WARNING_STR( "database not open. cannot execute", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
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
        LOG_EVENT_STR( "sqlite3_exec:", sql_statement );
        sqlite_err = sqlite3_exec( db, sql_statement, NULL, NULL, &error_msg );
        if ( SQLITE_OK != sqlite_err )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", sql_statement );
            LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
            result |= DATA_ERROR_AT_DB;
        }
        if ( error_msg != NULL )
        {
            LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
            sqlite3_free( error_msg );
            error_msg = NULL;
        }
    }
    else
    {
        LOG_WARNING_STR( "database not open. cannot execute", sql_statement );
        result = DATA_ERROR_NO_DB;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_create_diagram ( data_database_writer_t *this_, const data_diagram_t *diagram, int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != diagram );
    assert( NULL != out_new_id );
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_create_diagram_command( this_, diagram );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), true, out_new_id );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, *out_new_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_description ( data_database_writer_t *this_, int64_t diagram_id, const char* new_diagram_description )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram_description );
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_update_diagram_description_cmd( this_, diagram_id, new_diagram_description );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, diagram_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_name ( data_database_writer_t *this_, int64_t diagram_id, const char* new_diagram_name )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram_name );
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_update_diagram_name_cmd( this_, diagram_id, new_diagram_name );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, diagram_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_type ( data_database_writer_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_update_diagram_type_cmd( this_, diagram_id, new_diagram_type );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, diagram_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_create_classifier( data_database_writer_t *this_, const data_classifier_t *classifier, int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != classifier );
    assert( NULL != out_new_id );
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_create_classifier_command( this_, classifier );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), true, out_new_id );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, *out_new_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_stereotype ( data_database_writer_t *this_, int64_t classifier_id, const char* new_classifier_stereotype )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_update_classifier_stereotype_cmd( this_, classifier_id, new_classifier_stereotype );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_description ( data_database_writer_t *this_, int64_t classifier_id, const char* new_classifier_description )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_update_classifier_description_cmd( this_, classifier_id, new_classifier_description );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_name ( data_database_writer_t *this_, int64_t classifier_id, const char* new_classifier_name )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_update_classifier_name_cmd( this_, classifier_id, new_classifier_name );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_classifier_main_type ( data_database_writer_t *this_, int64_t classifier_id, data_classifier_type_t new_classifier_main_type )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_update_classifier_main_type_cmd( this_, classifier_id, new_classifier_main_type );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, classifier_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_create_diagramelement( data_database_writer_t *this_, const data_diagramelement_t *diagramelement, int64_t* out_new_id )
{
    TRACE_BEGIN();
    assert( NULL != diagramelement );
    assert( NULL != out_new_id );
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_build_create_diagramelement_command( this_, diagramelement );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), true, out_new_id );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAMELEMENT, *out_new_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_delete_diagramelement( data_database_writer_t *this_, int64_t obj_id, data_diagramelement_t *out_old_diagramelement )
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

    result |= data_database_writer_private_build_delete_diagramelement_command ( this_, obj_id );

    result |= data_database_writer_private_transaction_issue_command ( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ) );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAMELEMENT, obj_id );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_delete_classifier( data_database_writer_t *this_, int64_t obj_id, data_classifier_t *out_old_classifier )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    /* Note: This function fails if the classifier is still referenced. */

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_classifier is NULL if old data shall not be returned */
    if ( NULL != out_old_classifier )
    {
        result |= data_database_reader_get_classifier_by_id ( (*this_).db_reader, obj_id, out_old_classifier );
    }

    result |= data_database_writer_private_build_delete_classifier_command ( this_, obj_id );

    result |= data_database_writer_private_transaction_issue_command ( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ) );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_CLASSIFIER, obj_id );

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID; /* out_old_classifier is not yet read out! */
    if ( NULL != out_old_classifier )
    {
        data_classifier_init_empty( out_old_classifier );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_delete_diagram ( data_database_writer_t *this_, int64_t obj_id, data_diagram_t *out_old_diagram )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    /* Note: This function fails if the diagram is still referenced. */

    result |= data_database_writer_private_lock( this_ );

    result |= data_database_writer_private_transaction_begin ( this_ );

    /* Note: out_old_diagram is NULL if old data shall not be returned */
    if ( NULL != out_old_diagram )
    {
        result |= data_database_reader_get_diagram_by_id ( (*this_).db_reader, obj_id, out_old_diagram );
    }

    result |= data_database_writer_private_build_delete_diagram_command ( this_, obj_id );

    result |= data_database_writer_private_transaction_issue_command ( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ) );

    result |= data_database_writer_private_transaction_commit ( this_ );

    result |= data_database_writer_private_unlock( this_ );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ), DATA_TABLE_DIAGRAM, obj_id );

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID; /* out_old_diagram is not yet read out! */
    if ( NULL != out_old_diagram )
    {
        data_diagram_init_empty( out_old_diagram );
    }

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
