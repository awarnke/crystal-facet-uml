/* File: data_database_sql_builder.c; Copyright and License: see below */

#include "storage/data_database_sql_builder.h"
#include "data_id.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include "util/string/utf8string.h"
#include <assert.h>

/*!
 *  \brief value separator string constant to insert a diagram or classifier or other table-row
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR = ",";

/*!
 *  \brief string start marker string constant to insert/update a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START = "\'";

/*!
 *  \brief string end marker string constant to insert/update a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END = "\'";

/*!
 *  \brief constant representing the null value
 */
static const char *DATA_DATABASE_SQL_BUILDER_NULL_VALUE = "NULL";

/*!
 *  \brief translation table to encode strings for usage in string literals
 *
 *  Note: This table is not suitable for searches using the LIKE operator because _ and % are not handled.
 */
const char *const DATA_DATABASE_SQL_BUILDER_SQL_ENCODE[][2] = {
    { "'", "''" },
    { NULL, NULL }
};

void data_database_sql_builder_init ( data_database_sql_builder_t *this_ )
{
    TRACE_BEGIN();

    (*this_).temp_stringbuf = utf8stringbuf_init( sizeof((*this_).private_temp_buffer), (*this_).private_temp_buffer );
    (*this_).sql_stringbuf = utf8stringbuf_init( sizeof((*this_).private_sql_buffer), (*this_).private_sql_buffer );

    TRACE_END();
}

void data_database_sql_builder_destroy ( data_database_sql_builder_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
}

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief prefix string constant to insert a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_PREFIX =
    "INSERT INTO diagrams (parent_id,diagram_type,name,description,list_order,display_flags,uuid) VALUES (";

/*!
 *  \brief prefix string constant to insert a diagram with predefined id
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_WITH_ID_PREFIX =
    "INSERT INTO diagrams (id,parent_id,diagram_type,name,description,list_order,display_flags,uuid) VALUES (";

/*!
 *  \brief postfix string constant to insert a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_POSTFIX = ");";

/*!
 *  \brief prefix string constant to delete a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAM_PREFIX =
    "DELETE FROM diagrams WHERE (id=";

/*!
 *  \brief postfix string constant to delete a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAM_POSTFIX = ");";

/*!
 *  \brief prefix string constant to update a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX = "UPDATE diagrams SET ";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_PARENT_ID = "parent_id=";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_TYPE = "diagram_type=";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_NAME = "name=";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_LIST_ORDER = "list_order=";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_DESCRIPTION = "description=";

/*!
 *  \brief infix string constant to update a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX = ";";

u8_error_t data_database_sql_builder_build_create_diagram_command ( data_database_sql_builder_t *this_, const data_diagram_t *diagram )
{
    TRACE_BEGIN();
    assert( NULL != diagram );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    if ( DATA_ROW_ID_VOID == data_diagram_get_row_id( diagram )  )
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_PREFIX );
    }
    else
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_WITH_ID_PREFIX );
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagram_get_row_id( diagram ) );
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    if ( DATA_ROW_ID_VOID == data_diagram_get_parent_row_id( diagram ) )
    {
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagram_get_parent_row_id( diagram ) );
    }
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagram_get_diagram_type( diagram ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_diagram_get_name_const( diagram ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_diagram_get_description_const( diagram ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagram_get_list_order( diagram ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagram_get_display_flags( diagram ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_diagram_get_uuid_const( diagram ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_diagram_command ( data_database_sql_builder_t *this_, data_row_id_t diagram_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    utf8stringbuf_clear( (*this_).sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_name_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, const char *new_diagram_name )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram_name );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_NAME );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_diagram_name, 0, DATA_DIAGRAM_MAX_NAME_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_diagram_name ) > DATA_DIAGRAM_MAX_NAME_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, const char *new_diagram_description )
{
    TRACE_BEGIN();
    assert( NULL != new_diagram_description );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_DESCRIPTION );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_diagram_description, 0, DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_diagram_description ) > DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, data_diagram_type_t new_diagram_type )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_TYPE );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_diagram_type );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, int32_t new_diagram_list_order )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_LIST_ORDER );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_diagram_list_order );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_parent_id_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, data_row_id_t new_diagram_parent_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_PARENT_ID );
    if ( DATA_ROW_ID_VOID == new_diagram_parent_id )
    {
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_diagram_parent_id );
    }

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief prefix string constant to insert a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_PREFIX =
    "INSERT INTO classifiers (main_type,stereotype,name,description,x_order,y_order,list_order,uuid) VALUES (";

/*!
 *  \brief prefix string constant to insert a classifier with predefined id
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_WITH_ID_PREFIX =
    "INSERT INTO classifiers (id,main_type,stereotype,name,description,x_order,y_order,list_order,uuid) VALUES (";

/*!
 *  \brief postfix string constant to insert a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_POSTFIX = ");";

/*!
 *  \brief prefix string constant to delete a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_CLASSIFIER_PREFIX =
    "DELETE FROM classifiers WHERE (id=";

/*!
 *  \brief postfix string constant to delete a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_CLASSIFIER_POSTFIX = ");";

/*!
 *  \brief prefix string constant to update a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX = "UPDATE classifiers SET ";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_STEREOTYPE = "stereotype=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_MAIN_TYPE = "main_type=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_NAME = "name=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_DESCRIPTION = "description=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_X_ORDER = "x_order=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_Y_ORDER = "y_order=";

/*!
 *  \brief field name string constant to be used for updating a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_LIST_ORDER = "list_order=";

/*!
 *  \brief infix string constant to update a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a classifier
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX = ";";

u8_error_t data_database_sql_builder_build_create_classifier_command ( data_database_sql_builder_t *this_, const data_classifier_t *classifier )
{
    TRACE_BEGIN();
    assert( NULL != classifier );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    utf8stringbuf_clear( (*this_).sql_stringbuf );

    if ( data_classifier_get_row_id( classifier ) == DATA_ROW_ID_VOID )
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_PREFIX );
    }
    else
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_WITH_ID_PREFIX );
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_classifier_get_row_id( classifier ) );
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_classifier_get_main_type( classifier ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_classifier_get_stereotype_const( classifier ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_classifier_get_name_const( classifier ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_classifier_get_description_const( classifier ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_classifier_get_x_order( classifier ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_classifier_get_y_order( classifier ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_classifier_get_list_order( classifier ) );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_classifier_get_uuid_const( classifier ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_classifier_command ( data_database_sql_builder_t *this_, data_row_id_t classifier_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    utf8stringbuf_clear( (*this_).sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_stereotype_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_stereotype )
{
    TRACE_BEGIN();
    assert( NULL != new_classifier_stereotype );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_STEREOTYPE );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_classifier_stereotype, 0, DATA_CLASSIFIER_MAX_STEREOTYPE_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_classifier_stereotype ) > DATA_CLASSIFIER_MAX_STEREOTYPE_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_name_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_name )
{
    TRACE_BEGIN();
    assert( NULL != new_classifier_name );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_NAME );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_classifier_name, 0, DATA_CLASSIFIER_MAX_NAME_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_classifier_name ) > DATA_CLASSIFIER_MAX_NAME_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_description )
{
    TRACE_BEGIN();
    assert( NULL != new_classifier_description );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_DESCRIPTION );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_classifier_description, 0, DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_classifier_description ) > DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, data_classifier_type_t new_classifier_main_type )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_MAIN_TYPE );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_classifier_main_type );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_x_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_x_order )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_X_ORDER );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_classifier_x_order );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_y_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_y_order )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_Y_ORDER );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_classifier_y_order );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_list_order )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_LIST_ORDER );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_classifier_list_order );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, classifier_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief prefix string constant to insert a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_PREFIX =
    "INSERT INTO diagramelements (diagram_id,classifier_id,display_flags,focused_feature_id,uuid) VALUES (";

/*!
 *  \brief prefix string constant to insert a diagramelement with predefined id
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_WITH_ID_PREFIX =
    "INSERT INTO diagramelements (id,diagram_id,classifier_id,display_flags,focused_feature_id,uuid) VALUES (";

/*!
 *  \brief postfix string constant to insert a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_POSTFIX = ");";

/*!
 *  \brief prefix string constant to delete a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAMELEMENT_PREFIX =
    "DELETE FROM diagramelements WHERE (id=";

/*!
 *  \brief postfix string constant to delete a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAMELEMENT_POSTFIX = ");";

/*!
 *  \brief prefix string constant to update a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_PREFIX = "UPDATE diagramelements SET ";

/*!
 *  \brief field name string constant to be used for updating a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_COL_DISPLAY_FLAGS = "display_flags=";

/*!
 *  \brief field name string constant to be used for updating a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_COL_FOCUSED_FEATURE_ID = "focused_feature_id=";

/*!
 *  \brief infix string constant to update a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a diagramelement
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_POSTFIX = ";";

u8_error_t data_database_sql_builder_build_create_diagramelement_command ( data_database_sql_builder_t *this_, const data_diagramelement_t *diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != diagramelement );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    utf8stringbuf_clear( (*this_).sql_stringbuf );

    if ( DATA_ROW_ID_VOID == data_diagramelement_get_row_id( diagramelement ) )
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_PREFIX );
    }
    else
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_WITH_ID_PREFIX );
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagramelement_get_row_id( diagramelement ) );
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagramelement_get_diagram_row_id( diagramelement ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagramelement_get_classifier_row_id( diagramelement ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagramelement_get_display_flags( diagramelement ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    if ( DATA_ROW_ID_VOID == data_diagramelement_get_focused_feature_row_id( diagramelement ) )
    {
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_diagramelement_get_focused_feature_row_id( diagramelement ) );
    }
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_diagramelement_get_uuid_const( diagramelement ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_diagramelement_command ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    utf8stringbuf_clear( (*this_).sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAMELEMENT_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagramelement_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAMELEMENT_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagramelement_display_flags_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id, data_diagramelement_flag_t new_display_flags )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_COL_DISPLAY_FLAGS );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_display_flags );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagramelement_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagramelement_focused_feature_id_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id, data_row_id_t new_focused_feature_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_COL_FOCUSED_FEATURE_ID );

    if ( DATA_ROW_ID_VOID == new_focused_feature_id )
    {
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_focused_feature_id );
    }

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, diagramelement_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

/* ================================ FEATURE ================================ */

/*!
 *  \brief prefix string constant to insert a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_PREFIX =
    "INSERT INTO features (main_type,classifier_id,key,value,description,list_order,uuid) VALUES (";

/*!
 *  \brief prefix string constant to insert a feature with predefined id
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_WITH_ID_PREFIX =
    "INSERT INTO features (id,main_type,classifier_id,key,value,description,list_order,uuid) VALUES (";

/*!
 *  \brief postfix string constant to insert a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_POSTFIX = ");";

/*!
 *  \brief prefix string constant to delete a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_FEATURE_PREFIX =
    "DELETE FROM features WHERE (id=";

/*!
 *  \brief postfix string constant to delete a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_FEATURE_POSTFIX = ");";

/*!
 *  \brief prefix string constant to update a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX = "UPDATE features SET ";

/*!
 *  \brief field name string constant to be used for updating a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_MAIN_TYPE = "main_type=";

/*!
 *  \brief field name string constant to be used for updating a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_KEY = "key=";

/*!
 *  \brief field name string constant to be used for updating a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_VALUE = "value=";

/*!
 *  \brief field name string constant to be used for updating a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_DESCRIPTION = "description=";

/*!
 *  \brief field name string constant to be used for updating a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_LIST_ORDER = "list_order=";

/*!
 *  \brief infix string constant to update a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a feature
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX = ";";

u8_error_t data_database_sql_builder_build_create_feature_command ( data_database_sql_builder_t *this_, const data_feature_t *feature )
{
    TRACE_BEGIN();
    assert( NULL != feature );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    if ( data_feature_get_row_id( feature ) == DATA_ROW_ID_VOID )
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_PREFIX );
    }
    else
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_WITH_ID_PREFIX );
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_feature_get_row_id( feature ) );
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_feature_get_main_type( feature ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_feature_get_classifier_row_id( feature ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_feature_get_key_const( feature ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_feature_get_value_const( feature ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_feature_get_description_const( feature ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_feature_get_list_order( feature ) );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_feature_get_uuid_const( feature ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_feature_command ( data_database_sql_builder_t *this_, data_row_id_t feature_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    utf8stringbuf_clear( (*this_).sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_FEATURE_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, feature_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_FEATURE_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, data_feature_type_t new_feature_type )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_MAIN_TYPE );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_feature_type );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, feature_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_key_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_key )
{
    TRACE_BEGIN();
    assert( NULL != new_feature_key );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_KEY );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_feature_key, 0, DATA_FEATURE_MAX_KEY_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_feature_key ) > DATA_FEATURE_MAX_KEY_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, feature_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_value_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_value )
{
    TRACE_BEGIN();
    assert( NULL != new_feature_value );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_VALUE );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_feature_value, 0, DATA_FEATURE_MAX_VALUE_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_feature_value ) > DATA_FEATURE_MAX_VALUE_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, feature_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_description )
{
    TRACE_BEGIN();
    assert( NULL != new_feature_description );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_DESCRIPTION );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_feature_description, 0, DATA_FEATURE_MAX_DESCRIPTION_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_feature_description ) > DATA_FEATURE_MAX_DESCRIPTION_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, feature_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, int32_t new_feature_list_order )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_LIST_ORDER );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_feature_list_order );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, feature_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief prefix string constant to insert a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_PREFIX =
    "INSERT INTO relationships "
    "(main_type,from_classifier_id,to_classifier_id,name,description,list_order,from_feature_id,to_feature_id,uuid) "
    "VALUES (";

/*!
 *  \brief prefix string constant to insert a relationship with predefined id
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_WITH_ID_PREFIX =
    "INSERT INTO relationships "
    "(id,main_type,from_classifier_id,to_classifier_id,name,description,list_order,from_feature_id,to_feature_id,uuid) "
    "VALUES (";

/*!
 *  \brief postfix string constant to insert a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_POSTFIX = ");";

/*!
 *  \brief prefix string constant to delete a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_RELATIONSHIP_PREFIX =
    "DELETE FROM relationships WHERE (id=";

/*!
 *  \brief postfix string constant to delete a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_DELETE_RELATIONSHIP_POSTFIX = ");";

/*!
 *  \brief prefix string constant to update a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX = "UPDATE relationships SET ";

/*!
 *  \brief field name string constant to be used for updating a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_MAIN_TYPE = "main_type=";

/*!
 *  \brief field name string constant to be used for updating a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_NAME = "name=";

/*!
 *  \brief field name string constant to be used for updating a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_DESCRIPTION = "description=";

/*!
 *  \brief field name string constant to be used for updating a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_LIST_ORDER = "list_order=";

/*!
 *  \brief infix string constant to update a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX = ";";

u8_error_t data_database_sql_builder_build_create_relationship_command ( data_database_sql_builder_t *this_, const data_relationship_t *relationship )
{
    TRACE_BEGIN();
    assert( NULL != relationship );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    if ( data_relationship_get_row_id( relationship ) == DATA_ROW_ID_VOID )
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_PREFIX );
    }
    else
    {
        strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_WITH_ID_PREFIX );
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_relationship_get_row_id( relationship ) );
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_relationship_get_main_type( relationship ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_relationship_get_from_classifier_row_id( relationship ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_relationship_get_to_classifier_row_id( relationship ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_relationship_get_name_const( relationship ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_relationship_get_description_const( relationship ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_relationship_get_list_order( relationship ) );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    if ( DATA_ROW_ID_VOID == data_relationship_get_from_feature_row_id( relationship ) )
    {
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_relationship_get_from_feature_row_id( relationship ) );
    }
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    if ( DATA_ROW_ID_VOID == data_relationship_get_to_feature_row_id( relationship ) )
    {
        strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, data_relationship_get_to_feature_row_id( relationship ) );
    }

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).temp_stringbuf, data_relationship_get_uuid_const( relationship ) );
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_relationship_command ( data_database_sql_builder_t *this_, data_row_id_t relationship_id )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    utf8stringbuf_clear( (*this_).sql_stringbuf );

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_RELATIONSHIP_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, relationship_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_DELETE_RELATIONSHIP_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, data_relationship_type_t new_relationship_type )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_MAIN_TYPE );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_relationship_type );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, relationship_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_name_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, const char *new_relationship_name )
{
    TRACE_BEGIN();
    assert( NULL != new_relationship_name );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_NAME );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_relationship_name, 0, DATA_RELATIONSHIP_MAX_NAME_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_relationship_name ) > DATA_RELATIONSHIP_MAX_NAME_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, relationship_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, const char *new_relationship_description )
{
    TRACE_BEGIN();
    assert( NULL != new_relationship_description );
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_DESCRIPTION );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        utf8stringbuf_copy_region_from_str( (*this_).temp_stringbuf, new_relationship_description, 0, DATA_RELATIONSHIP_MAX_DESCRIPTION_LENGTH );
        /* do not evaluate the result of above call, it may be UTF8ERROR_OUT_OF_RANGE but this is expected. */
        if ( utf8string_get_length( new_relationship_description ) > DATA_RELATIONSHIP_MAX_DESCRIPTION_LENGTH )
        {
            strerr |= UTF8ERROR_TRUNCATED;
        }
        strerr |= utf8stringbuf_replace_all( (*this_).temp_stringbuf, &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).sql_stringbuf, (*this_).temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, relationship_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, int32_t new_relationship_list_order )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    u8_error_t result = U8_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_LIST_ORDER );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, new_relationship_list_order );

    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).sql_stringbuf, relationship_id );
    strerr |= utf8stringbuf_append_str( (*this_).sql_stringbuf, DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}


/*
Copyright 2016-2022 Andreas Warnke

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
