/* File: json_serializer.c; Copyright and License: see below */

#include "json/json_serializer.h"
#include "json/json_constants.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void json_serializer_init ( json_serializer_t *this_, universal_output_stream_t * output )
{
    TRACE_BEGIN();
    assert( output != NULL );

    json_writer_init( &((*this_).json_writer), output );

    (*this_).in_array = false;
    (*this_).is_first = false;

    TRACE_END();
}

void json_serializer_destroy ( json_serializer_t *this_ )
{
    TRACE_BEGIN();

    json_writer_destroy( &((*this_).json_writer) );

    TRACE_END();
}

data_error_t json_serializer_begin_data ( json_serializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( ! (*this_).in_array )
    {
        (*this_).in_array = true;

        utf8error_t strerr = UTF8ERROR_SUCCESS;

        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_BEGIN_OBJECT_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_DATA
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_BEGIN_ARRAY );

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

data_error_t json_serializer_end_data ( json_serializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        (*this_).in_array = false;

        utf8error_t strerr = UTF8ERROR_SUCCESS;

        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_END_ARRAY
                                           JSON_CONSTANTS_NL
                                           JSON_CONSTANTS_END_OBJECT_NL );

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

data_error_t json_serializer_append_classifier ( json_serializer_t *this_,
                                                 data_classifier_t *object,
                                                 data_feature_t (*features)[],
                                                 uint32_t feature_count )
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
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                              JSON_CONSTANTS_NL );
        }
        else
        {
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                              JSON_CONSTANTS_NEXT_NL );
        }

        /* begin classifier */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_BEGIN_OBJECT
                                           JSON_CONSTANTS_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_BEGIN_OBJECT_NL );

        /* id */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_ID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_classifier_get_row_id( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* main type */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_classifier_get_main_type( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* stereotype */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_classifier_get_stereotype_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* name */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_NAME
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_classifier_get_name_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* description */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_stringlist_enc( &((*this_).json_writer),
                                                    data_classifier_get_description_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* x_order */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_classifier_get_x_order( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* y_order */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_classifier_get_y_order( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* list_order */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_LIST_ORDER
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_classifier_get_list_order( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* uuid */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_UUID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_classifier_get_uuid_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* array of features */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_BEGIN_ARRAY
                                           JSON_CONSTANTS_NL
                                          );
        for ( uint32_t index = 0; index < feature_count; index ++ )
        {
            strerr |= json_serializer_private_append_feature ( this_, &((*features)[index]) );
            if ( index+1 == feature_count )
            {
                strerr |= json_writer_write_plain( &((*this_).json_writer),
                                                   JSON_CONSTANTS_NL );
            }
            else
            {
                strerr |= json_writer_write_plain( &((*this_).json_writer),
                                                   JSON_CONSTANTS_NEXT_NL );
            }
        }
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_END_ARRAY
                                           JSON_CONSTANTS_NL );

        /* end classifier */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_END_OBJECT_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_END_OBJECT );

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

data_error_t json_serializer_append_diagram ( json_serializer_t *this_, data_diagram_t *object )
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
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                               JSON_CONSTANTS_NL );
        }
        else
        {
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                               JSON_CONSTANTS_NEXT_NL );
        }

        /* begin diagram */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_BEGIN_OBJECT
                                           JSON_CONSTANTS_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_DIAGRAM
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_BEGIN_OBJECT_NL );

        /* id */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_DIAGRAM_ID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_diagram_get_row_id( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* diagram type */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_diagram_get_diagram_type( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* name */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_DIAGRAM_NAME
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_diagram_get_name_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* description */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_stringlist_enc( &((*this_).json_writer),
                                            data_diagram_get_description_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* list_order */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_diagram_get_list_order( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* display_flags */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_diagram_get_display_flags( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* uuid */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_UUID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_diagram_get_uuid_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NL );  /* LAST, no NEXT */

        /* end diagram */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_END_OBJECT_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_END_OBJECT );

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

data_error_t json_serializer_append_relationship ( json_serializer_t *this_,
                                                   data_relationship_t *object,
                                                   data_classifier_t *from_clas,
                                                   data_feature_t *from_feat,
                                                   data_classifier_t *to_clas,
                                                   data_feature_t *to_feat )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    assert ( NULL != from_clas );
    assert ( NULL != from_feat );
    assert ( NULL != to_clas );
    assert ( NULL != to_feat );
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).in_array )
    {
        utf8error_t strerr = UTF8ERROR_SUCCESS;

        /* separate objects if not first */
        if ( (*this_).is_first )
        {
            (*this_).is_first = false;
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                               JSON_CONSTANTS_NL );
        }
        else
        {
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                               JSON_CONSTANTS_NEXT_NL );
        }

        /* begin relationship */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_BEGIN_OBJECT
                                           JSON_CONSTANTS_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_BEGIN_OBJECT_NL );

        /* id */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_ID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_relationship_get_row_id( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* main type */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_relationship_get_main_type( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* name */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_NAME
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_relationship_get_name_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* description */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_stringlist_enc( &((*this_).json_writer),
                                            data_relationship_get_description_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* list_order */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_relationship_get_list_order( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* from_classifier_id */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_ID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_relationship_get_from_classifier_row_id( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* from_classifier_name */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_NAME
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_classifier_get_name_const( from_clas ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* to_classifier_id */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_ID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_relationship_get_to_classifier_row_id( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* to_classifier_name */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_NAME
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_classifier_get_name_const( to_clas ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NEXT_NL );

        /* from_feature_id */
        const bool from_feat_valid = ( data_relationship_get_from_feature_row_id( object ) != DATA_ROW_ID_VOID );
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_ID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_relationship_get_from_feature_row_id( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* from_feature_key */
        if ( from_feat_valid )
        {
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                               JSON_CONSTANTS_TAB
                                               JSON_CONSTANTS_TAB
                                               JSON_CONSTANTS_TAB
                                               JSON_CONSTANTS_TAB
                                               JSON_CONSTANTS_QUOTE
                                               JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_KEY
                                               JSON_CONSTANTS_QUOTE
                                               JSON_CONSTANTS_DEF
                                               JSON_CONSTANTS_QUOTE );
            strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                                data_feature_get_key_const( from_feat ));
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                               JSON_CONSTANTS_QUOTE
                                               JSON_CONSTANTS_NEXT_NL );
        }

        /* to_feature_id */
        const bool to_feat_valid = ( data_relationship_get_to_feature_row_id( object ) != DATA_ROW_ID_VOID );
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_ID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF );
        strerr |= json_writer_write_int( &((*this_).json_writer),
                                         data_relationship_get_to_feature_row_id( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_NEXT_NL );

        /* to_feature_key */
        if ( to_feat_valid )
        {
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                               JSON_CONSTANTS_TAB
                                               JSON_CONSTANTS_TAB
                                               JSON_CONSTANTS_TAB
                                               JSON_CONSTANTS_TAB
                                               JSON_CONSTANTS_QUOTE
                                               JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_KEY
                                               JSON_CONSTANTS_QUOTE
                                               JSON_CONSTANTS_DEF
                                               JSON_CONSTANTS_QUOTE );
            strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                                data_feature_get_key_const( to_feat ));
            strerr |= json_writer_write_plain( &((*this_).json_writer),
                                               JSON_CONSTANTS_QUOTE
                                               JSON_CONSTANTS_NEXT_NL );
        }

        /* uuid */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_UUID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_relationship_get_uuid_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NL );  /* LAST, no NEXT */

        /* end relationship */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_END_OBJECT_NL
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_END_OBJECT );

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

utf8error_t json_serializer_private_append_feature ( json_serializer_t *this_, data_feature_t *object )
{
    TRACE_BEGIN();
    assert ( NULL != object );

    utf8error_t strerr = UTF8ERROR_SUCCESS;

    /* begin feature */
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_BEGIN_OBJECT_NL );

    /* id */
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_FEATURE_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    strerr |= json_writer_write_int( &((*this_).json_writer),
                                     data_feature_get_row_id( object ));
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* main_type */
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    strerr |= json_writer_write_int( &((*this_).json_writer),
                                     data_feature_get_main_type( object ));
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* key */
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_FEATURE_KEY
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                        data_feature_get_key_const( object ));
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* value */
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_FEATURE_VALUE
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                        data_feature_get_value_const( object ));
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* description */
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    strerr |= json_writer_write_stringlist_enc( &((*this_).json_writer),
                                        data_feature_get_description_const( object ));
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* list_order */
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    strerr |= json_writer_write_int( &((*this_).json_writer),
                                        data_feature_get_list_order( object ));
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

        /* uuid */
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_TAB
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_KEY_UUID
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_DEF
                                           JSON_CONSTANTS_QUOTE );
        strerr |= json_writer_write_string_enc( &((*this_).json_writer),
                                            data_feature_get_uuid_const( object ));
        strerr |= json_writer_write_plain( &((*this_).json_writer),
                                           JSON_CONSTANTS_QUOTE
                                           JSON_CONSTANTS_NL );  /* LAST, no NEXT */

    /* end feature */
    strerr |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
    }

    TRACE_END_ERR( strerr );
    return strerr;
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
