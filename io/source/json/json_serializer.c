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

    (*this_).in_outer_array = false;
    (*this_).is_outer_first = false;
    (*this_).in_inner_array = false;
    (*this_).is_inner_first = false;

    TRACE_END();
}

void json_serializer_destroy ( json_serializer_t *this_ )
{
    TRACE_BEGIN();

    json_writer_destroy( &((*this_).json_writer) );

    TRACE_END();
}

int json_serializer_write_header ( json_serializer_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).in_outer_array == false );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_BEGIN_OBJECT_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_HEAD
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_BEGIN_OBJECT_NL
                                      );

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        "warning"
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE
                                        "This format is subject to change."
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NL
                                      );

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT
                                        JSON_CONSTANTS_NEXT_NL
                                      );


    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_write_footer ( json_serializer_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).in_outer_array == false );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_END_OBJECT_NL
                                       );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_begin_data ( json_serializer_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).in_outer_array == false );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DATA
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_BEGIN_ARRAY
                                      );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    (*this_).in_outer_array = true;
    (*this_).is_outer_first = true;

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_end_data ( json_serializer_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).in_outer_array == true );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    (*this_).in_outer_array = false;
    (*this_).is_outer_first = false;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_ARRAY
                                        JSON_CONSTANTS_NL
                                       );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_begin_classifier ( json_serializer_t *this_, const data_classifier_t *object )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    assert( (*this_).in_outer_array == true );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    /* separate objects in array if not first */
    if ( (*this_).is_outer_first )
    {
        (*this_).is_outer_first = false;
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NL );
    }
    else
    {
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL );
    }

    /* begin classifier */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_CLASSIFIER_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_classifier_get_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* main type */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_classifier_get_main_type( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* stereotype */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_classifier_get_stereotype_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* name */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_CLASSIFIER_NAME
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_classifier_get_name_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* description */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_stringlist_enc( &((*this_).json_writer),
                                                 data_classifier_get_description_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* x_order */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_classifier_get_x_order( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* y_order */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_classifier_get_y_order( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* list_order */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_CLASSIFIER_LIST_ORDER
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_classifier_get_list_order( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* uuid */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_UUID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_classifier_get_uuid_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* array of features */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
                                      );

    (*this_).in_inner_array = true;
    (*this_).is_inner_first = true;

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_end_classifier ( json_serializer_t *this_, const data_classifier_t *object )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    assert( (*this_).in_outer_array == true );
    assert( (*this_).in_inner_array == true );
    int out_err = 0;

    (*this_).in_inner_array = false;
    (*this_).is_inner_first = false;

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NL );

    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_ARRAY
                                        JSON_CONSTANTS_NL );

    /* end classifier */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_append_feature ( json_serializer_t *this_, const data_feature_t *object )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    assert( (*this_).in_outer_array == true );
    assert( (*this_).in_inner_array == true );
    int out_err = 0;

    /* separate objects in array if not first */
    if ( (*this_).is_inner_first )
    {
        (*this_).is_inner_first = false;
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NL );
    }
    else
    {
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL );
    }

    /* begin feature */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_BEGIN_OBJECT_NL );

    /* id */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_feature_get_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* main_type */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_feature_get_main_type( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* key */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_feature_get_key_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* value */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_feature_get_value_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* description */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_stringlist_enc( &((*this_).json_writer),
                                                 data_feature_get_description_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* list_order */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_feature_get_list_order( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* uuid */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_feature_get_uuid_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NL );  /* LAST, no NEXT */

    /* end feature */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_append_relationship ( json_serializer_t *this_,
                                          const data_relationship_t *object,
                                          const data_classifier_t *from_clas,
                                          const data_feature_t *from_feat,
                                          const data_classifier_t *to_clas,
                                          const data_feature_t *to_feat )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    assert ( NULL != from_clas );
    assert ( NULL != from_feat );
    assert ( NULL != to_clas );
    assert ( NULL != to_feat );
    assert( (*this_).in_outer_array == true );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    /* separate objects if not first */
    if ( (*this_).is_outer_first )
    {
        (*this_).is_outer_first = false;
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NL );
    }
    else
    {
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL );
    }

    /* begin relationship */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_relationship_get_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* main type */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_relationship_get_main_type( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* name */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_NAME
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_relationship_get_name_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* description */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_stringlist_enc( &((*this_).json_writer),
                                                 data_relationship_get_description_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* list_order */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_relationship_get_list_order( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* from_classifier_id */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_relationship_get_from_classifier_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* from_classifier_name */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_NAME
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_classifier_get_name_const( from_clas ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* to_classifier_id */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_relationship_get_to_classifier_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* to_classifier_name */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_NAME
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_classifier_get_name_const( to_clas ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* from_feature_id */
    const bool from_feat_valid = ( data_relationship_get_from_feature_row_id( object ) != DATA_ROW_ID_VOID );
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_relationship_get_from_feature_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* from_feature_key */
    if ( from_feat_valid )
    {
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_KEY
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                            JSON_CONSTANTS_QUOTE );
        out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                                 data_feature_get_key_const( from_feat ));
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_NEXT_NL );
    }

    /* to_feature_id */
    const bool to_feat_valid = ( data_relationship_get_to_feature_row_id( object ) != DATA_ROW_ID_VOID );
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_relationship_get_to_feature_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* to_feature_key */
    if ( to_feat_valid )
    {
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_TAB
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_KEY
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_DEF
                                            JSON_CONSTANTS_QUOTE );
        out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                                 data_feature_get_key_const( to_feat ));
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_QUOTE
                                            JSON_CONSTANTS_NEXT_NL );
    }

    /* uuid */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_UUID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_relationship_get_uuid_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NL );  /* LAST, no NEXT */

    /* end relationship */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_append_diagram ( json_serializer_t *this_, const data_diagram_t *object )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    assert( (*this_).in_outer_array == true );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    /* separate objects if not first */
    if ( (*this_).is_outer_first )
    {
        (*this_).is_outer_first = false;
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NL );
    }
    else
    {
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL );
    }

    /* begin diagram */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
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
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAM_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagram_get_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* parent_id */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAM_PARENT_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagram_get_parent_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* diagram type */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagram_get_diagram_type( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* name */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAM_NAME
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_diagram_get_name_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* description */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_stringlist_enc( &((*this_).json_writer),
                                                 data_diagram_get_description_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NEXT_NL );

    /* list_order */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagram_get_list_order( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* display_flags */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagram_get_display_flags( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* uuid */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_UUID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_diagram_get_uuid_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NL );  /* LAST, no NEXT */

    /* end diagram */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
}

int json_serializer_append_diagramelement ( json_serializer_t *this_, const data_diagramelement_t *object )
{
    TRACE_BEGIN();
    assert ( NULL != object );
    assert( (*this_).in_outer_array == true );
    assert( (*this_).in_inner_array == false );
    int out_err = 0;

    /* separate objects if not first */
    if ( (*this_).is_outer_first )
    {
        (*this_).is_outer_first = false;
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NL );
    }
    else
    {
        out_err |= json_writer_write_plain( &((*this_).json_writer),
                                            JSON_CONSTANTS_NEXT_NL );
    }

    /* begin diagramelement */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_BEGIN_OBJECT
                                        JSON_CONSTANTS_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAMELEMENT
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_BEGIN_OBJECT_NL );

    /* id */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAMELEMENT_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagramelement_get_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* diagram_id */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DIAGRAM_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagramelement_get_diagram_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* classifier_id */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagramelement_get_classifier_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* focused_feature_id */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_ID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagramelement_get_focused_feature_row_id( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* display_flags */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF );
    out_err |= json_writer_write_int( &((*this_).json_writer),
                                      data_diagramelement_get_display_flags( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_NEXT_NL );

    /* uuid */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_KEY_UUID
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_DEF
                                        JSON_CONSTANTS_QUOTE );
    out_err |= json_writer_write_string_enc( &((*this_).json_writer),
                                             data_diagramelement_get_uuid_const( object ));
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_QUOTE
                                        JSON_CONSTANTS_NL );  /* LAST, no NEXT */

    /* end diagramelement */
    out_err |= json_writer_write_plain( &((*this_).json_writer),
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT_NL
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_TAB
                                        JSON_CONSTANTS_END_OBJECT );

    if ( out_err != 0 )
    {
        TSLOG_ERROR( "output buffer exceeded." );
    }

    TRACE_END_ERR( out_err );
    return out_err;
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
