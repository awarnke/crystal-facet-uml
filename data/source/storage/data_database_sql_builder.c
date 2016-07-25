/* File: data_database_sql_builder.c; Copyright and License: see below */

#include "storage/data_database_sql_builder.h"
#include "data_id.h"
#include "trace.h"
#include "log.h"
#include <assert.h>

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

void data_database_sql_builder_init ( data_database_sql_builder_t *this_ )
{
    TRACE_BEGIN();

    (*this_).private_temp_stringbuf = utf8stringbuf_init( sizeof((*this_).private_temp_buffer), (*this_).private_temp_buffer );
    (*this_).private_sql_stringbuf = utf8stringbuf_init( sizeof((*this_).private_sql_buffer), (*this_).private_sql_buffer );

    TRACE_END();
}

void data_database_sql_builder_destroy ( data_database_sql_builder_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
}

data_error_t data_database_writer_private_build_create_diagram_command ( data_database_sql_builder_t *this_, const data_diagram_t *diagram )
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

data_error_t data_database_writer_private_build_update_diagram_name_cmd ( data_database_sql_builder_t *this_, int64_t diagram_id, const char *new_diagram_name )
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

data_error_t data_database_writer_private_build_update_diagram_description_cmd ( data_database_sql_builder_t *this_, int64_t diagram_id, const char *new_diagram_description )
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

data_error_t data_database_writer_private_build_update_diagram_type_cmd ( data_database_sql_builder_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type )
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

data_error_t data_database_writer_private_build_delete_diagram_command ( data_database_sql_builder_t *this_, int64_t diagram_id )
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

data_error_t data_database_writer_private_build_create_classifier_command ( data_database_sql_builder_t *this_, const data_classifier_t *classifier )
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

data_error_t data_database_writer_private_build_update_classifier_stereotype_cmd ( data_database_sql_builder_t *this_, int64_t classifier_id, const char *new_classifier_stereotype )
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

data_error_t data_database_writer_private_build_update_classifier_name_cmd ( data_database_sql_builder_t *this_, int64_t classifier_id, const char *new_classifier_name )
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

data_error_t data_database_writer_private_build_update_classifier_description_cmd ( data_database_sql_builder_t *this_, int64_t classifier_id, const char *new_classifier_description )
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

data_error_t data_database_writer_private_build_update_classifier_main_type_cmd ( data_database_sql_builder_t *this_, int64_t classifier_id, data_classifier_type_t new_classifier_main_type )
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

data_error_t data_database_writer_private_build_delete_classifier_command ( data_database_sql_builder_t *this_, int64_t classifier_id )
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

data_error_t data_database_writer_private_build_create_diagramelement_command ( data_database_sql_builder_t *this_, const data_diagramelement_t *diagramelement )
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

data_error_t data_database_writer_private_build_delete_diagramelement_command ( data_database_sql_builder_t *this_, int64_t diagramelement_id )
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
