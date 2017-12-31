/* File: data_json_serializer.c; Copyright and License: see below */

#include "serial/data_json_serializer.h"
#include "serial/data_json_constants.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

static const char * const DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS[] = {
    "\x09", "\\t",  /* tab */
    "\x0a", "\\n",  /* newline */
    "\x0d", "\\r",  /* return */
    "\x08", "\\b",  /* backspace */
    "\x0c", "\\f",  /* form feed */
    "\"", "\\\"",  /* double quote */
    "\\", "\\\\",  /* backslash*/
    "/", "\\/",  /* slash */
    NULL,  /* for JSON, see rfc7159 */
};

void data_json_serializer_init ( data_json_serializer_t *this_ )
{
    TRACE_BEGIN();

    (*this_).in_array = false;
    (*this_).is_first = false;

    TRACE_END();
}

void data_json_serializer_destroy ( data_json_serializer_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

data_error_t data_json_serializer_begin_set ( data_json_serializer_t *this_, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( ! (*this_).in_array )
    {
        (*this_).in_array = true;

        utf8error_t strerr = UTF8ERROR_SUCCESS;

        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_BEGIN_OBJECT_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_SET
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_BEGIN_ARRAY );

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
        (*this_).is_first = true;
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_serializer_end_set ( data_json_serializer_t *this_, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        (*this_).in_array = false;

        utf8error_t strerr = UTF8ERROR_SUCCESS;

        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_ARRAY
                                            DATA_JSON_CONSTANTS_NL
                                            DATA_JSON_CONSTANTS_END_OBJECT_NL );

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
        (*this_).is_first = false;
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_serializer_append_classifier ( data_json_serializer_t *this_,
                                                      data_classifier_t *object,
                                                      data_feature_t (*features)[],
                                                      uint32_t feature_count,
                                                      utf8stringbuf_t out )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        utf8error_t strerr = UTF8ERROR_SUCCESS;

        /* separate objects if not first */
        if ( (*this_).is_first )
        {
            (*this_).is_first = false;
            strerr |= utf8stringbuf_append_str( out,
                                                DATA_JSON_CONSTANTS_NL );
        }
        else
        {
            strerr |= utf8stringbuf_append_str( out,
                                                DATA_JSON_CONSTANTS_NEXT_NL );
        }

        /* begin classifier */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_BEGIN_OBJECT
                                            DATA_JSON_CONSTANTS_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_BEGIN_OBJECT_NL );

        /* id */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER_ID
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_classifier_get_id( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* main type */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_classifier_get_main_type( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* stereotype */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_QUOTE );
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
        strerr |= utf8stringbuf_append_str( out,
                                            data_classifier_get_stereotype_ptr( object ));
        strerr |= utf8stringbuf_replace_all( out,
                                             DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* name */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER_NAME
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_QUOTE );
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
        strerr |= utf8stringbuf_append_str( out,
                                            data_classifier_get_name_ptr( object ));
        strerr |= utf8stringbuf_replace_all( out,
                                             DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* description */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_QUOTE );
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
        strerr |= utf8stringbuf_append_str( out,
                                            data_classifier_get_description_ptr( object ));
        strerr |= utf8stringbuf_replace_all( out,
                                             DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* x_order */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_classifier_get_x_order( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* y_order */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_classifier_get_y_order( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* array of features */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_BEGIN_ARRAY
                                            DATA_JSON_CONSTANTS_NL
                                          );
        for ( uint32_t index = 0; index < feature_count; index ++ )
        {
            strerr |= data_json_serializer_private_append_feature ( this_, &((*features)[index]), out );
            if ( index+1 == feature_count )
            {
                strerr |= utf8stringbuf_append_str( out,
                                                    DATA_JSON_CONSTANTS_NL );
            }
            else
            {
                strerr |= utf8stringbuf_append_str( out,
                                                    DATA_JSON_CONSTANTS_NEXT_NL );
            }
        }
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_ARRAY
                                            DATA_JSON_CONSTANTS_NL );

        /* end classifier */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_OBJECT_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_OBJECT );

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_serializer_append_diagram ( data_json_serializer_t *this_, data_diagram_t *object, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        utf8error_t strerr = UTF8ERROR_SUCCESS;

        /* separate objects if not first */
        if ( (*this_).is_first )
        {
            (*this_).is_first = false;
            strerr |= utf8stringbuf_append_str( out,
                                                DATA_JSON_CONSTANTS_NL );
        }
        else
        {
            strerr |= utf8stringbuf_append_str( out,
                                                DATA_JSON_CONSTANTS_NEXT_NL );
        }

        /* begin diagram */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_BEGIN_OBJECT
                                            DATA_JSON_CONSTANTS_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_DIAGRAM
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_BEGIN_OBJECT_NL );

        /* id */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_DIAGRAM_ID
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_diagram_get_id( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* diagram type */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_diagram_get_diagram_type( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* name */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_DIAGRAM_NAME
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_QUOTE );
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
        strerr |= utf8stringbuf_append_str( out,
                                            data_diagram_get_name_ptr( object ));
        strerr |= utf8stringbuf_replace_all( out,
                                             DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* description */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_QUOTE );
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
        strerr |= utf8stringbuf_append_str( out,
                                            data_diagram_get_description_ptr( object ));
        strerr |= utf8stringbuf_replace_all( out,
                                             DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* list_order */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_diagram_get_list_order( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NL );

        /* end diagram */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_OBJECT_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_OBJECT );

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_serializer_append_relationship ( data_json_serializer_t *this_, data_relationship_t *object, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        utf8error_t strerr = UTF8ERROR_SUCCESS;

        /* separate objects if not first */
        if ( (*this_).is_first )
        {
            (*this_).is_first = false;
            strerr |= utf8stringbuf_append_str( out,
                                                DATA_JSON_CONSTANTS_NL );
        }
        else
        {
            strerr |= utf8stringbuf_append_str( out,
                                                DATA_JSON_CONSTANTS_NEXT_NL );
        }

        /* begin relationship */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_BEGIN_OBJECT
                                            DATA_JSON_CONSTANTS_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_RELATIONSHIP
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_BEGIN_OBJECT_NL );

        /* id */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_ID
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_relationship_get_id( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* main type */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_relationship_get_main_type( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* from_classifier_id */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_ID
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_relationship_get_from_classifier_id( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* to_classifier_id */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_ID
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_relationship_get_to_classifier_id( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* name */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_NAME
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_QUOTE );
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
        strerr |= utf8stringbuf_append_str( out,
                                            data_relationship_get_name_ptr( object ));
        strerr |= utf8stringbuf_replace_all( out,
                                             DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* description */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF
                                            DATA_JSON_CONSTANTS_QUOTE );
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
        strerr |= utf8stringbuf_append_str( out,
                                            data_relationship_get_description_ptr( object ));
        strerr |= utf8stringbuf_replace_all( out,
                                             DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_NEXT_NL );

        /* list_order */
        out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER
                                            DATA_JSON_CONSTANTS_QUOTE
                                            DATA_JSON_CONSTANTS_DEF );
        strerr |= utf8stringbuf_append_int( out,
                                            data_relationship_get_list_order( object ));
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_NL );

        /* end relationship */
        strerr |= utf8stringbuf_append_str( out,
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_OBJECT_NL
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_TAB
                                            DATA_JSON_CONSTANTS_END_OBJECT );

        if ( strerr != UTF8ERROR_SUCCESS )
        {
            TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
            result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
        }
    }
    else
    {
        result = DATA_ERROR_INVALID_REQUEST;
    }

    TRACE_END_ERR( result );
    return result;
}

utf8error_t data_json_serializer_private_append_feature ( data_json_serializer_t *this_, data_feature_t *object, utf8stringbuf_t out )
{
    TRACE_BEGIN();
    assert ( NULL != object );

    utf8error_t strerr = UTF8ERROR_SUCCESS;

    /* begin feature */
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_BEGIN_OBJECT_NL );

    /* id */
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_KEY_FEATURE_ID
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_DEF );
    strerr |= utf8stringbuf_append_int( out,
                                        data_feature_get_id( object ));
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_NEXT_NL );

    /* main_type */
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_DEF );
    strerr |= utf8stringbuf_append_int( out,
                                        data_feature_get_main_type( object ));
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_NEXT_NL );

    /* key */
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_KEY_FEATURE_KEY
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_DEF
                                        DATA_JSON_CONSTANTS_QUOTE );
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
    strerr |= utf8stringbuf_append_str( out,
                                        data_feature_get_key_ptr( object ));
    strerr |= utf8stringbuf_replace_all( out,
                                            DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_NEXT_NL );

    /* value */
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_KEY_FEATURE_VALUE
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_DEF
                                        DATA_JSON_CONSTANTS_QUOTE );
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
    strerr |= utf8stringbuf_append_str( out,
                                        data_feature_get_value_ptr( object ));
    strerr |= utf8stringbuf_replace_all( out,
                                            DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_NEXT_NL );

    /* description */
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_DEF
                                        DATA_JSON_CONSTANTS_QUOTE );
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, to allow escaping only the string literal */
    strerr |= utf8stringbuf_append_str( out,
                                        data_feature_get_description_ptr( object ));
    strerr |= utf8stringbuf_replace_all( out,
                                            DATA_JSON_SERIALIZER_PRIVATE_ENCODE_JSON_STRINGS );
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_NEXT_NL );

    /* list_order */
    out = utf8stringbuf_get_end( out );  /* goto end of buffer, do not care about already written data */
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER
                                        DATA_JSON_CONSTANTS_QUOTE
                                        DATA_JSON_CONSTANTS_DEF );
    strerr |= utf8stringbuf_append_int( out,
                                        data_feature_get_list_order( object ));
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_NL );

    /* end feature */
    strerr |= utf8stringbuf_append_str( out,
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_TAB
                                        DATA_JSON_CONSTANTS_END_OBJECT );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
    }

    TRACE_END_ERR( strerr );
    return strerr;
}


/*
Copyright 2016-2018 Andreas Warnke

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
