/* File: data_database_sql_builder.c; Copyright and License: see below */

#include "storage/data_database_sql_builder.h"
#include "data_id.h"
#include "u8/u8_trace.h"
#include "u8/u8_log.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
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
    U8_TRACE_BEGIN();

    /* initialize a memory output stream */
    universal_memory_output_stream_init( &((*this_).plain_out),
                                         &((*this_).private_sql_buffer),
                                         sizeof((*this_).private_sql_buffer)
                                       );
    universal_output_stream_t *const plain_output
        = universal_memory_output_stream_get_output_stream( &((*this_).plain_out) );

    utf8stream_writer_init( &((*this_).plain), plain_output );

    /* initialize an sql escaped output stream */
    universal_escaping_output_stream_init( &((*this_).escaped_out),
                                           &DATA_DATABASE_SQL_BUILDER_SQL_ENCODE,
                                           plain_output
                                         );
    universal_output_stream_t *const escaped_output
        = universal_escaping_output_stream_get_output_stream( &((*this_).escaped_out) );

    utf8stream_writer_init( &((*this_).escaped), escaped_output );

    U8_TRACE_END();
}

void data_database_sql_builder_destroy ( data_database_sql_builder_t *this_ )
{
    U8_TRACE_BEGIN();

    /* de-initialize an sql escaped output stream */
    utf8stream_writer_destroy( &((*this_).escaped) );
    universal_escaping_output_stream_destroy( &((*this_).escaped_out) );

    /* de-initialize an output stream */
    utf8stream_writer_destroy( &((*this_).plain) );
    universal_memory_output_stream_destroy( &((*this_).plain_out) );

    U8_TRACE_END();
}

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief prefix string constant to insert a diagram
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_PREFIX =
    "INSERT INTO diagrams (parent_id,diagram_type,stereotype,name,description,list_order,display_flags,uuid) VALUES (";

/*!
 *  \brief prefix string constant to insert a diagram with predefined id
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_WITH_ID_PREFIX =
    "INSERT INTO diagrams (id,parent_id,diagram_type,stereotype,name,description,list_order,display_flags,uuid) VALUES (";

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
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_STEREOTYPE = "stereotype=";

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
    U8_TRACE_BEGIN();
    assert( NULL != diagram );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    if ( DATA_ROW_ID_VOID == data_diagram_get_row_id( diagram )  )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_PREFIX );
    }
    else
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_WITH_ID_PREFIX );
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagram_get_row_id( diagram ) );
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    if ( DATA_ROW_ID_VOID == data_diagram_get_parent_row_id( diagram ) )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagram_get_parent_row_id( diagram ) );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagram_get_diagram_type( diagram ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    /* stereotype */
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_diagram_get_stereotype_const( diagram ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    /* name */
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_diagram_get_name_const( diagram ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    /* description */
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_diagram_get_description_const( diagram ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagram_get_list_order( diagram ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagram_get_display_flags( diagram ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_diagram_get_uuid_const( diagram ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAM_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_diagram_command ( data_database_sql_builder_t *this_, data_row_id_t diagram_id )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAM_PREFIX );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagram_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAM_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_stereotype_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, const char *new_diagram_stereotype )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_diagram_stereotype );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_STEREOTYPE );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_diagram_stereotype );
        if ( new_length <= DATA_DIAGRAM_MAX_STEREOTYPE_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_diagram_stereotype );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_diagram_stereotype, DATA_DIAGRAM_MAX_STEREOTYPE_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagram_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_name_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, const char *new_diagram_name )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_diagram_name );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_NAME );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_diagram_name );
        if ( new_length <= DATA_DIAGRAM_MAX_NAME_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_diagram_name );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_diagram_name, DATA_DIAGRAM_MAX_NAME_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagram_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, const char *new_diagram_description )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_diagram_description );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_DESCRIPTION );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_diagram_description );
        if ( new_length <= DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_diagram_description );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_diagram_description, DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagram_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, data_diagram_type_t new_diagram_type )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_TYPE );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_diagram_type );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagram_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, int32_t new_diagram_list_order )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_LIST_ORDER );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_diagram_list_order );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagram_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagram_parent_id_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagram_id, data_row_id_t new_diagram_parent_id )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_COL_PARENT_ID );
    if ( DATA_ROW_ID_VOID == new_diagram_parent_id )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        out_err |= utf8stream_writer_write_int( &((*this_).plain), new_diagram_parent_id );
    }

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagram_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAM_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
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
    U8_TRACE_BEGIN();
    assert( NULL != classifier );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    if ( data_classifier_get_row_id( classifier ) == DATA_ROW_ID_VOID )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_PREFIX );
    }
    else
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_WITH_ID_PREFIX );
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_classifier_get_row_id( classifier ) );
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_classifier_get_main_type( classifier ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_classifier_get_stereotype_const( classifier ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_classifier_get_name_const( classifier ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_classifier_get_description_const( classifier ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_classifier_get_x_order( classifier ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_classifier_get_y_order( classifier ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_classifier_get_list_order( classifier ) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_classifier_get_uuid_const( classifier ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_classifier_command ( data_database_sql_builder_t *this_, data_row_id_t classifier_id )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_CLASSIFIER_PREFIX );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), classifier_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_stereotype_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_stereotype )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_classifier_stereotype );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_STEREOTYPE );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_classifier_stereotype );
        if ( new_length <= DATA_CLASSIFIER_MAX_STEREOTYPE_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_classifier_stereotype );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_classifier_stereotype, DATA_CLASSIFIER_MAX_STEREOTYPE_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), classifier_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_name_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_name )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_classifier_name );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_NAME );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_classifier_name );
        if ( new_length <= DATA_CLASSIFIER_MAX_NAME_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_classifier_name );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_classifier_name, DATA_CLASSIFIER_MAX_NAME_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), classifier_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, const char *new_classifier_description )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_classifier_description );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_DESCRIPTION );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_classifier_description );
        if ( new_length <= DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_classifier_description );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_classifier_description, DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), classifier_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, data_classifier_type_t new_classifier_main_type )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_MAIN_TYPE );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_classifier_main_type );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), classifier_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_x_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_x_order )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_X_ORDER );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_classifier_x_order );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), classifier_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_y_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_y_order )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_Y_ORDER );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_classifier_y_order );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), classifier_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_classifier_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t classifier_id, int32_t new_classifier_list_order )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_COL_LIST_ORDER );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_classifier_list_order );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), classifier_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_CLASSIFIER_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
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
    U8_TRACE_BEGIN();
    assert( NULL != diagramelement );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    if ( DATA_ROW_ID_VOID == data_diagramelement_get_row_id( diagramelement ) )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_PREFIX );
    }
    else
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_WITH_ID_PREFIX );
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagramelement_get_row_id( diagramelement ) );
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagramelement_get_diagram_row_id( diagramelement ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagramelement_get_classifier_row_id( diagramelement ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagramelement_get_display_flags( diagramelement ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    if ( DATA_ROW_ID_VOID == data_diagramelement_get_focused_feature_row_id( diagramelement ) )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_diagramelement_get_focused_feature_row_id( diagramelement ) );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_diagramelement_get_uuid_const( diagramelement ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_DIAGRAMELEMENT_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_diagramelement_command ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAMELEMENT_PREFIX );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagramelement_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_DIAGRAMELEMENT_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagramelement_display_flags_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id, data_diagramelement_flag_t new_display_flags )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_COL_DISPLAY_FLAGS );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_display_flags );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagramelement_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_diagramelement_focused_feature_id_cmd ( data_database_sql_builder_t *this_, data_row_id_t diagramelement_id, data_row_id_t new_focused_feature_id )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_COL_FOCUSED_FEATURE_ID );

    if ( DATA_ROW_ID_VOID == new_focused_feature_id )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        out_err |= utf8stream_writer_write_int( &((*this_).plain), new_focused_feature_id );
    }

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), diagramelement_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_DIAGRAMELEMENT_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
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
    U8_TRACE_BEGIN();
    assert( NULL != feature );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    if ( data_feature_get_row_id( feature ) == DATA_ROW_ID_VOID )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_PREFIX );
    }
    else
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_WITH_ID_PREFIX );
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_feature_get_row_id( feature ) );
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_feature_get_main_type( feature ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_feature_get_classifier_row_id( feature ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_feature_get_key_const( feature ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_feature_get_value_const( feature ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_feature_get_description_const( feature ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_feature_get_list_order( feature ) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_feature_get_uuid_const( feature ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_FEATURE_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_feature_command ( data_database_sql_builder_t *this_, data_row_id_t feature_id )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_FEATURE_PREFIX );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), feature_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_FEATURE_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, data_feature_type_t new_feature_type )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_MAIN_TYPE );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_feature_type );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), feature_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_key_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_key )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_feature_key );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_KEY );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_feature_key );
        if ( new_length <= DATA_FEATURE_MAX_KEY_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_feature_key );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_feature_key, DATA_FEATURE_MAX_KEY_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), feature_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_value_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_value )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_feature_value );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_VALUE );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_feature_value );
        if ( new_length <= DATA_FEATURE_MAX_VALUE_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_feature_value );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_feature_value, DATA_FEATURE_MAX_VALUE_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), feature_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, const char *new_feature_description )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_feature_description );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_DESCRIPTION );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_feature_description );
        if ( new_length <= DATA_FEATURE_MAX_DESCRIPTION_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_feature_description );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_feature_description, DATA_FEATURE_MAX_DESCRIPTION_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), feature_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_feature_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t feature_id, int32_t new_feature_list_order )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_COL_LIST_ORDER );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_feature_list_order );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), feature_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_FEATURE_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief prefix string constant to insert a relationship
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_PREFIX =
    "INSERT INTO relationships "
    "(main_type,from_classifier_id,to_classifier_id,stereotype,name,description,list_order,from_feature_id,to_feature_id,uuid) "
    "VALUES (";

/*!
 *  \brief prefix string constant to insert a relationship with predefined id
 */
static const char *DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_WITH_ID_PREFIX =
    "INSERT INTO relationships "
    "(id,main_type,from_classifier_id,to_classifier_id,stereotype,name,description,list_order,from_feature_id,to_feature_id,uuid) "
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
static const char *DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_STEREOTYPE = "stereotype=";

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
    U8_TRACE_BEGIN();
    assert( NULL != relationship );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    if ( data_relationship_get_row_id( relationship ) == DATA_ROW_ID_VOID )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_PREFIX );
    }
    else
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_WITH_ID_PREFIX );
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_relationship_get_row_id( relationship ) );
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    }
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_relationship_get_main_type( relationship ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_relationship_get_from_classifier_row_id( relationship ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_relationship_get_to_classifier_row_id( relationship ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    /* stereotype */
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_relationship_get_stereotype_const( relationship ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    /* name */
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_relationship_get_name_const( relationship ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    /* description*/
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_relationship_get_description_const( relationship ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), data_relationship_get_list_order( relationship ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    if ( DATA_ROW_ID_VOID == data_relationship_get_from_feature_row_id( relationship ) )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_relationship_get_from_feature_row_id( relationship ) );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );
    if ( DATA_ROW_ID_VOID == data_relationship_get_to_feature_row_id( relationship ) )
    {
        out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_NULL_VALUE );
    }
    else
    {
        out_err |= utf8stream_writer_write_int( &((*this_).plain), data_relationship_get_to_feature_row_id( relationship ) );
    }

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_VALUE_SEPARATOR );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    out_err |= utf8stream_writer_write_str( &((*this_).escaped), data_relationship_get_uuid_const( relationship ) );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_INSERT_RELATIONSHIP_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_delete_relationship_command ( data_database_sql_builder_t *this_, data_row_id_t relationship_id )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_RELATIONSHIP_PREFIX );
    out_err |= utf8stream_writer_write_int( &((*this_).plain), relationship_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_DELETE_RELATIONSHIP_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_main_type_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, data_relationship_type_t new_relationship_type )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_MAIN_TYPE );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_relationship_type );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), relationship_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_stereotype_cmd ( data_database_sql_builder_t *this_,
                                                                                data_row_id_t relationship_id,
                                                                                const char *new_relationship_stereotype )
{

    U8_TRACE_BEGIN();
    assert( NULL != new_relationship_stereotype );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_STEREOTYPE );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_relationship_stereotype );
        if ( new_length <= DATA_RELATIONSHIP_MAX_STEREOTYPE_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_relationship_stereotype );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_relationship_stereotype, DATA_RELATIONSHIP_MAX_STEREOTYPE_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), relationship_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_name_cmd ( data_database_sql_builder_t *this_,
                                                                          data_row_id_t relationship_id,
                                                                          const char *new_relationship_name )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_relationship_name );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_NAME );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_relationship_name );
        if ( new_length <= DATA_RELATIONSHIP_MAX_NAME_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_relationship_name );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_relationship_name, DATA_RELATIONSHIP_MAX_NAME_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), relationship_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_description_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, const char *new_relationship_description )
{
    U8_TRACE_BEGIN();
    assert( NULL != new_relationship_description );
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_DESCRIPTION );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        /* copy the string but not more than the maximum allowed bytes: */
        utf8stringview_t shortened_text;
        const size_t new_length = utf8string_get_length( new_relationship_description );
        if ( new_length <= DATA_RELATIONSHIP_MAX_DESCRIPTION_LENGTH )
        {
            utf8stringview_init_str( &shortened_text, new_relationship_description );
        }
        else
        {
            out_err |= UTF8ERROR_TRUNCATED;
            utf8stringview_init( &shortened_text, new_relationship_description, DATA_RELATIONSHIP_MAX_DESCRIPTION_LENGTH );
            /* ignore a possible UTF8ERROR_OUT_OF_RANGE result */
        }
        utf8stream_writer_write_view( &((*this_).escaped), &shortened_text );
        utf8stringview_destroy( &shortened_text );
    }
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_STRING_VALUE_END );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), relationship_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}

u8_error_t data_database_sql_builder_build_update_relationship_list_order_cmd ( data_database_sql_builder_t *this_, data_row_id_t relationship_id, int32_t new_relationship_list_order )
{
    U8_TRACE_BEGIN();
    u8_error_t out_err = U8_ERROR_NONE;
    u8_error_t result = U8_ERROR_NONE;

    out_err |= universal_memory_output_stream_reset( &((*this_).plain_out) );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_PREFIX );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_COL_LIST_ORDER );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), new_relationship_list_order );

    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_INFIX );

    out_err |= utf8stream_writer_write_int( &((*this_).plain), relationship_id );
    out_err |= utf8stream_writer_write_str( &((*this_).plain), DATA_DATABASE_SQL_BUILDER_UPDATE_RELATIONSHIP_POSTFIX );

    out_err |= universal_memory_output_stream_write_0term( &((*this_).plain_out), true );

    if ( out_err != U8_ERROR_NONE )
    {
        U8_LOG_ERROR_HEX( "utf8stream_writer_write_...() failed:", out_err );
        result |= U8_ERROR_STRING_BUFFER_EXCEEDED;
    }

    U8_TRACE_END_ERR( result );
    return( result );
}


/*
Copyright 2016-2024 Andreas Warnke

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
