/* File: json_deserializer.c; Copyright and License: see below */

#include "json/json_deserializer.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void json_deserializer_init ( json_deserializer_t *this_, universal_input_stream_t *in_data )
{
    TRACE_BEGIN();

    (*this_).temp_string = utf8stringbuf_init( sizeof((*this_).temp_string_buffer), (*this_).temp_string_buffer );

    json_token_reader_init( &((*this_).tokenizer), in_data );

    (*this_).after_first_array_entry = false;

    TRACE_END();
}

void json_deserializer_reinit ( json_deserializer_t *this_, universal_input_stream_t *in_data )
{
    TRACE_BEGIN();

    json_deserializer_destroy( this_ );
    json_deserializer_init( this_, in_data );

    TRACE_END();
}

void json_deserializer_destroy ( json_deserializer_t *this_ )
{
    TRACE_BEGIN();

    json_token_reader_destroy( &((*this_).tokenizer) );

    TRACE_END();
}

data_error_t json_deserializer_expect_header ( json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    char member_name_buf[24] = "";
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    if ( DATA_ERROR_NONE == result )
    {
        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
        if ( ! utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_HEAD ) )
        {
            TSLOG_ERROR_INT( "unexpected object contents at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result |= DATA_ERROR_PARSER_STRUCTURE;
        }
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = json_token_reader_expect_name_separator( &((*this_).tokenizer) );
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = json_deserializer_skip_next_object( this_ );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_expect_begin_data ( json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    char member_name_buf[24] = "";
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );

    if ( DATA_ERROR_NONE == result )
    {
        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
        if (( ! utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_VIEWS ) )
            && ( ! utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_NODES ) )
            && ( ! utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_EDGES ) ))
        {
            TSLOG_ERROR_INT( "unexpected object contents at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result |= DATA_ERROR_PARSER_STRUCTURE;
        }
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = json_token_reader_expect_name_separator( &((*this_).tokenizer) );
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = json_token_reader_expect_begin_array( &((*this_).tokenizer) );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_expect_footer ( json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    bool end_ok;
    result = json_token_reader_check_end_object ( &((*this_).tokenizer), &end_ok );
    if ( ! end_ok )
    {
        TSLOG_ERROR_INT( "unexpected object contents at line",
                            json_token_reader_get_input_line( &((*this_).tokenizer) )
                        );
        result |= DATA_ERROR_PARSER_STRUCTURE;
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = json_token_reader_expect_eof ( &((*this_).tokenizer) );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_check_end_data ( json_deserializer_t *this_, bool* out_end )
{
    TRACE_BEGIN();
    assert( NULL != out_end );
    data_error_t result = DATA_ERROR_NONE;

    result = json_token_reader_check_end_array ( &((*this_).tokenizer), out_end );

    if (( DATA_ERROR_NONE == result ) && ( *out_end ))
    {
        (*this_).after_first_array_entry = false;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_expect_begin_type_of_element ( json_deserializer_t *this_, data_table_t *out_type )
{
    TRACE_BEGIN();
    assert ( NULL != out_type );
    data_error_t result = DATA_ERROR_NONE;

    bool array_end;
    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    result = json_token_reader_check_end_array ( &((*this_).tokenizer), &array_end );

    if ( DATA_ERROR_NONE == result )
    {
        if ( array_end )
        {
            (*out_type) = DATA_TABLE_VOID;
        }
        else
        {
            if ( (*this_).after_first_array_entry )
            {
                result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
            }
            else
            {
                (*this_).after_first_array_entry = true;
            }
            if ( DATA_ERROR_NONE == result )
            {
                result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );
            }
            if ( DATA_ERROR_NONE == result )
            {
                result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
            }
            if ( DATA_ERROR_NONE == result )
            {
                if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM ) )
                {
                    (*out_type) = DATA_TABLE_DIAGRAM;
                }
                /*
                else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT ) )
                {
                    (*out_type) = DATA_TABLE_DIAGRAMELEMENT;
                }
                */
                else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER ) )
                {
                    (*out_type) = DATA_TABLE_CLASSIFIER;
                }
                /*
                else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE ) )
                {
                    (*out_type) = DATA_TABLE_FEATURE;
                }
                */
                else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP ) )
                {
                    (*out_type) = DATA_TABLE_RELATIONSHIP;
                }
                else
                {
                    TSLOG_ERROR_INT( "unexpected token at line",
                                     json_token_reader_get_input_line( &((*this_).tokenizer) )
                                   );
                    result = DATA_ERROR_PARSER_STRUCTURE;
                }
            }
            if ( DATA_ERROR_NONE == result )
            {
                result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_expect_end_type_of_element ( json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    bool condition;

    result = json_token_reader_check_end_object ( &((*this_).tokenizer), &condition );
    if ( ! condition )
    {
        TSLOG_ERROR_INT( "unexpected object contents at line",
                         json_token_reader_get_input_line( &((*this_).tokenizer) )
                       );
        result |= DATA_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_get_next_classifier ( json_deserializer_t *this_,
                                                     data_classifier_t *out_object,
                                                     uint32_t max_out_array_size,
                                                     data_feature_t (*out_feature)[],
                                                     uint32_t *out_feature_count )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );
    assert ( NULL != out_feature );
    assert ( NULL != out_feature_count );
    data_error_t result = DATA_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_classifier_init_empty( out_object );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    static const int MAX_MEMBERS = 16;  /* mamimum number of members to parse */
    if ( DATA_ERROR_NONE == result )
    {
        for ( int count = 0; ( ! object_end ) && ( count < MAX_MEMBERS ); count ++ )
        {
            result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
            if ( DATA_ERROR_NONE == result )
            {
                if ( ! object_end )
                {
                    if ( first_member_passed )
                    {
                        result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
                    }
                    else
                    {
                        first_member_passed = true;
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_ID ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_classifier_set_row_id ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_classifier_set_main_type ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_X_ORDER ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_classifier_set_x_order ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_Y_ORDER ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_classifier_set_y_order ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_LIST_ORDER ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_classifier_set_list_order ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_STEREOTYPE ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_classifier_set_stereotype( out_object,
                                                            utf8stringbuf_get_string( (*this_).temp_string )
                                                          );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_NAME ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_classifier_set_name( out_object,
                                                      utf8stringbuf_get_string( (*this_).temp_string )
                                                    );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_DESCRIPTION ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_classifier_set_description( out_object,
                                                             utf8stringbuf_get_string( (*this_).temp_string )
                                                           );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_UUID ) )
                        {
                            /* TODO: do something with the uuid */
                            result = json_deserializer_skip_next_string ( this_ );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES ) )
                        {
                            result = json_deserializer_private_get_next_feature_array ( this_, max_out_array_size, out_feature, out_feature_count );
                        }
                        else
                        {
                            TSLOG_ERROR_INT( "unexpected member name at line",
                                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                                           );
                            result = DATA_ERROR_PARSER_STRUCTURE;
                        }
                    }
                }
            }
            else
            {
                TSLOG_ERROR_INT( "unexpected character at line",
                                 json_token_reader_get_input_line( &((*this_).tokenizer) )
                               );
                result = DATA_ERROR_PARSER_STRUCTURE;
                object_end = true;
            }
        }
    }

    /* footer */

    if ( DATA_ERROR_NONE == result )
    {
        data_classifier_trace( out_object );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_get_next_diagram ( json_deserializer_t *this_, data_diagram_t *out_object )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );
    data_error_t result = DATA_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_diagram_init_empty( out_object );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    static const int MAX_MEMBERS = 16;  /* mamimum number of members to parse */
    if ( DATA_ERROR_NONE == result )
    {
        for ( int count = 0; ( ! object_end ) && ( count < MAX_MEMBERS ); count ++ )
        {
            result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
            if ( DATA_ERROR_NONE == result )
            {
                if ( ! object_end )
                {
                    if ( first_member_passed )
                    {
                        result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
                    }
                    else
                    {
                        first_member_passed = true;
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_ID ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_diagram_set_row_id ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_PARENT_ID ) )
                        {
                            int64_t ignored_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &ignored_integer );
                            (void) ignored_integer;
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_diagram_set_diagram_type ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_diagram_set_list_order ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_diagram_set_description( out_object,
                                                          utf8stringbuf_get_string( (*this_).temp_string )
                                                        );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_NAME ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_diagram_set_name( out_object,
                                                   utf8stringbuf_get_string( (*this_).temp_string )
                                                 );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_diagram_set_display_flags ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_UUID ) )
                        {
                            /* TODO: do something with the uuid */
                            result = json_deserializer_skip_next_string ( this_ );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS ) )
                        {
                            result = json_deserializer_private_skip_next_diagramelement_array ( this_ );
                        }
                        else
                        {
                            TSLOG_ERROR_INT( "unexpected member name at line",
                                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                                           );
                            result = DATA_ERROR_PARSER_STRUCTURE;
                        }
                    }
                }
            }
            else
            {
                TSLOG_ERROR_INT( "unexpected character at line",
                                 json_token_reader_get_input_line( &((*this_).tokenizer) )
                               );
                result = DATA_ERROR_PARSER_STRUCTURE;
                object_end = true;
            }
        }
    }

    /* footer */

    if ( DATA_ERROR_NONE == result )
    {
        data_diagram_trace( out_object );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_get_next_relationship ( json_deserializer_t *this_,
                                                       data_relationship_t *out_object,
                                                       utf8stringbuf_t out_from_classifier_name,
                                                       utf8stringbuf_t out_from_feature_key,
                                                       utf8stringbuf_t out_to_classifier_name,
                                                       utf8stringbuf_t out_to_feature_key
                                                     )
{
    TRACE_BEGIN();
    assert( NULL != out_object );
    data_error_t result = DATA_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_relationship_init_empty( out_object );
    utf8stringbuf_clear( out_from_classifier_name );
    utf8stringbuf_clear( out_from_feature_key );
    utf8stringbuf_clear( out_to_classifier_name );
    utf8stringbuf_clear( out_to_feature_key );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    static const int MAX_MEMBERS = 24;  /* mamimum number of members to parse */
    if ( DATA_ERROR_NONE == result )
    {
        for ( int count = 0; ( ! object_end ) && ( count < MAX_MEMBERS ); count ++ )
        {
            result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
            if ( DATA_ERROR_NONE == result )
            {
                if ( ! object_end )
                {
                    if ( first_member_passed )
                    {
                        result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
                    }
                    else
                    {
                        first_member_passed = true;
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_ID ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_relationship_set_row_id ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_relationship_set_main_type ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_relationship_set_list_order ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_relationship_set_description( out_object,
                                                               utf8stringbuf_get_string( (*this_).temp_string )
                                                             );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_NAME ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_relationship_set_name( out_object,
                                                        utf8stringbuf_get_string( (*this_).temp_string )
                                                      );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_ID ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                            data_relationship_set_from_classifier_row_id ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_CLASSIFIER_NAME ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            utf8stringbuf_copy_buf( out_from_classifier_name, (*this_).temp_string );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_ID ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                            data_relationship_set_from_feature_row_id ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_FEATURE_KEY ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            utf8stringbuf_copy_buf( out_from_feature_key, (*this_).temp_string );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_ID ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                            data_relationship_set_to_classifier_row_id ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_CLASSIFIER_NAME ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            utf8stringbuf_copy_buf( out_to_classifier_name, (*this_).temp_string );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_ID ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                            data_relationship_set_to_feature_row_id ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_FEATURE_KEY ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            utf8stringbuf_copy_buf( out_to_feature_key, (*this_).temp_string );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE ) )
                        {
                            /* TODO: do something with the uuid */
                            result = json_deserializer_skip_next_string ( this_ );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE ) )
                        {
                            /* TODO: do something with the uuid */
                            result = json_deserializer_skip_next_string ( this_ );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_UUID ) )
                        {
                            /* TODO: do something with the uuid */
                            result = json_deserializer_skip_next_string ( this_ );
                        }
                        else
                        {
                            TSLOG_ERROR_INT( "unexpected member name at line",
                                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                                           );
                            result = DATA_ERROR_PARSER_STRUCTURE;
                        }
                    }
                }
            }
            else
            {
                TSLOG_ERROR_INT( "unexpected character at line",
                                 json_token_reader_get_input_line( &((*this_).tokenizer) )
                               );
                result = DATA_ERROR_PARSER_STRUCTURE;
                object_end = true;
            }
        }
    }

    /* footer */

    if ( DATA_ERROR_NONE == result )
    {
        data_relationship_trace( out_object );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_skip_next_object ( json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    static const int MAX_MEMBERS = 16;  /* mamimum number of members to parse */
    if ( DATA_ERROR_NONE == result )
    {
        for ( int count = 0; ( ! object_end ) && ( count < MAX_MEMBERS ); count ++ )
        {
            result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
            if ( DATA_ERROR_NONE == result )
            {
                if ( ! object_end )
                {
                    if ( first_member_passed )
                    {
                        result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
                    }
                    else
                    {
                        first_member_passed = true;
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                    }
                    json_value_type_t v_type;
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_get_value_type ( &((*this_).tokenizer), &v_type );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        switch ( v_type )
                        {
                            case JSON_VALUE_TYPE_OBJECT:
                            {
                                result = DATA_ERROR_PARSER_STRUCTURE; /* this function does not expect objects in objects */
                            }
                            break;

                            case JSON_VALUE_TYPE_ARRAY:
                            {
                                result = DATA_ERROR_PARSER_STRUCTURE; /* this function does not expect arrays in objects */
                            }
                            break;

                            case JSON_VALUE_TYPE_NUMBER:
                            {
                                double parsed_number;
                                result = json_token_reader_read_number_value ( &((*this_).tokenizer), &parsed_number );
                            }
                            break;

                            case JSON_VALUE_TYPE_INTEGER:
                            {
                                int64_t parsed_integer;
                                result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            }
                            break;

                            case JSON_VALUE_TYPE_STRING:
                            {
                                result = json_deserializer_skip_next_string( this_ );
                            }
                            break;

                            case JSON_VALUE_TYPE_BOOLEAN:
                            {
                                bool parsed_bool;
                                result = json_token_reader_read_boolean_value ( &((*this_).tokenizer), &parsed_bool );
                            }
                            break;

                            case JSON_VALUE_TYPE_NULL:
                            {
                                result = json_token_reader_expect_null_value ( &((*this_).tokenizer) );
                            }
                            break;

                            case JSON_VALUE_TYPE_UNDEF:
                            default:
                            {
                                TSLOG_ERROR_INT( "unexpected member name at line",
                                                 json_token_reader_get_input_line( &((*this_).tokenizer) )
                                               );
                                result = DATA_ERROR_PARSER_STRUCTURE; /* this function does not expect objects in objects */
                            }
                            break;
                        }
                    }
                }
            }
            else
            {
                TSLOG_ERROR_INT( "unexpected character at line",
                                 json_token_reader_get_input_line( &((*this_).tokenizer) )
                               );
                result = DATA_ERROR_PARSER_STRUCTURE;
                object_end = true;
            }
        }
    }

    /* footer */

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_skip_next_string ( json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    char dummy_str[4];
    utf8stringbuf_t dummy_strbuf = UTF8STRINGBUF ( dummy_str );

    result = json_token_reader_read_string_value ( &((*this_).tokenizer), dummy_strbuf );
    if ( result == DATA_ERROR_STRING_BUFFER_EXCEEDED )
    {
        /* ignore this. The result string is not needed therefore dummy_str may be too small */
        result = DATA_ERROR_NONE;
    }

    TRACE_END_ERR( result );
    return result;
}

void json_deserializer_get_read_line ( json_deserializer_t *this_, uint32_t *out_read_pos )
{
    TRACE_BEGIN();
    assert ( NULL != out_read_pos );

    (*out_read_pos) = json_token_reader_get_input_line( &((*this_).tokenizer) );

    TRACE_END();
}

data_error_t json_deserializer_private_get_next_feature_array ( json_deserializer_t *this_,
                                                                uint32_t max_out_array_size,
                                                                data_feature_t (*out_feature)[],
                                                                uint32_t *out_feature_count )
{
    TRACE_BEGIN();
    assert ( NULL != out_feature );
    assert ( NULL != out_feature_count );
    data_error_t result = DATA_ERROR_NONE;
    uint32_t feature_count = 0;

    result = json_token_reader_expect_begin_array( &((*this_).tokenizer) );

    if ( DATA_ERROR_NONE == result )
    {
        bool end_array = false;
        bool first_element_passed = false;
        for ( int count = 0; ( ! end_array ) && ( count < max_out_array_size ); count ++ )
        {
            result = json_token_reader_check_end_array( &((*this_).tokenizer), &end_array );
            if ( DATA_ERROR_NONE == result )
            {
                if ( ! end_array )
                {
                    if ( first_element_passed )
                    {
                        result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
                    }
                    else
                    {
                        first_element_passed = true;
                    }
                    result |= json_deserializer_private_get_next_feature( this_, &((*out_feature)[count]) );
                    if ( DATA_ERROR_NONE == result )
                    {
                        feature_count = count+1;
                    }
                    else
                    {
                        /* error, break loop */
                        end_array = true;
                    }
                }
            }
            else
            {
                /* error, break loop */
                TSLOG_ERROR_INT( "unexpected array contents at line",
                                 json_token_reader_get_input_line( &((*this_).tokenizer) )
                               );
                result |= DATA_ERROR_PARSER_STRUCTURE;
                end_array = true;
            }
        }
    }

    if ( DATA_ERROR_NONE == result )
    {
        *out_feature_count = feature_count;
    }
    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_private_get_next_feature ( json_deserializer_t *this_, data_feature_t *out_object )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );

    data_error_t result = DATA_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_feature_init_empty( out_object );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    static const int MAX_MEMBERS = 16;  /* mamimum number of members to parse */
    if ( DATA_ERROR_NONE == result )
    {
        for ( int count = 0; ( ! object_end ) && ( count < MAX_MEMBERS ); count ++ )
        {
            result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
            if ( DATA_ERROR_NONE == result )
            {
                if ( ! object_end )
                {
                    if ( first_member_passed )
                    {
                        result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
                    }
                    else
                    {
                        first_member_passed = true;
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                    }
                    if ( DATA_ERROR_NONE == result )
                    {
                        if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_ID ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_feature_set_row_id ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_feature_set_main_type ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER ) )
                        {
                            int64_t parsed_integer;
                            result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                            data_feature_set_list_order ( out_object, parsed_integer );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_feature_set_description( out_object,
                                                          utf8stringbuf_get_string( (*this_).temp_string )
                                                        );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_KEY ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_feature_set_key( out_object,
                                                  utf8stringbuf_get_string( (*this_).temp_string )
                                                );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_VALUE ) )
                        {
                            result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                            data_feature_set_value( out_object,
                                                    utf8stringbuf_get_string( (*this_).temp_string )
                                                  );
                        }
                        else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_UUID ) )
                        {
                            /* TODO: do something with the uuid */
                            result = json_deserializer_skip_next_string ( this_ );
                        }
                        else
                        {
                            TSLOG_ERROR_INT( "unexpected member name at line",
                                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                                           );
                            result = DATA_ERROR_PARSER_STRUCTURE;
                        }
                    }
                }
            }
            else
            {
                TSLOG_ERROR_INT( "unexpected character at line",
                                 json_token_reader_get_input_line( &((*this_).tokenizer) )
                               );
                result = DATA_ERROR_PARSER_STRUCTURE;
                object_end = true;
            }
        }
    }

    /* footer */

    if ( DATA_ERROR_NONE == result )
    {
        data_feature_trace( out_object );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t json_deserializer_private_skip_next_diagramelement_array ( json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result = json_token_reader_expect_begin_array( &((*this_).tokenizer) );

    bool end_array = false;
    bool first_element_passed = false;
    while (( ! end_array )&&( DATA_ERROR_NONE == result ))
    {
        result = json_token_reader_check_end_array( &((*this_).tokenizer), &end_array );
        if ( DATA_ERROR_NONE == result )
        {
            if ( ! end_array )
            {
                if ( first_element_passed )
                {
                    result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
                }
                else
                {
                    first_element_passed = true;
                }
                result |= json_deserializer_skip_next_object( this_ );
            }
        }
        else
        {
            /* error, break loop */
            TSLOG_ERROR_INT( "unexpected array contents at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
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
