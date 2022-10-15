/* File: json_element_reader.c; Copyright and License: see below */

#include "json/json_element_reader.h"
#include "trace/trace.h"
#include "tslog/tslog.h"
#include <assert.h>

void json_element_reader_init ( json_element_reader_t *this_, universal_input_stream_t *in_data )
{
    TRACE_BEGIN();
    assert( NULL != in_data );

    (*this_).temp_string = utf8stringbuf_init( sizeof((*this_).temp_string_buffer), (*this_).temp_string_buffer );

    json_token_reader_init( &((*this_).tokenizer), in_data );

    (*this_).top_array_after_first_entry = false;
    (*this_).sub_array_after_first_entry = false;

    TRACE_END();
}

void json_element_reader_reinit ( json_element_reader_t *this_, universal_input_stream_t *in_data )
{
    TRACE_BEGIN();
    assert( NULL != in_data );

    json_element_reader_destroy( this_ );
    json_element_reader_init( this_, in_data );

    TRACE_END();
}

void json_element_reader_destroy ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();

    json_token_reader_destroy( &((*this_).tokenizer) );

    TRACE_END();
}

u8_error_t json_element_reader_expect_header ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    char member_name_buf[24] = "";
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    if ( U8_ERROR_NONE == result )
    {
        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
        if ( ! utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_HEAD ) )
        {
            TSLOG_ERROR_INT( "unexpected object contents at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result |= U8_ERROR_PARSER_STRUCTURE;
        }
    }

    if ( U8_ERROR_NONE == result )
    {
        result = json_token_reader_expect_name_separator( &((*this_).tokenizer) );
    }

    if ( U8_ERROR_NONE == result )
    {
        result = json_element_reader_skip_next_object( this_ );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_expect_begin_top_array ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    char member_name_buf[24] = "";
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );

    if ( U8_ERROR_NONE == result )
    {
        result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
        if (( ! utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_VIEWS ) )
            && ( ! utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_NODES ) )
            && ( ! utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_EDGES ) ))
        {
            TSLOG_ERROR_INT( "unexpected object contents at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result |= U8_ERROR_PARSER_STRUCTURE;
        }
    }

    if ( U8_ERROR_NONE == result )
    {
        result = json_token_reader_expect_name_separator( &((*this_).tokenizer) );
    }

    if ( U8_ERROR_NONE == result )
    {
        result = json_token_reader_expect_begin_array( &((*this_).tokenizer) );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_expect_footer ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    bool end_ok;
    result = json_token_reader_check_end_object ( &((*this_).tokenizer), &end_ok );
    if ( ! end_ok )
    {
        TSLOG_ERROR_INT( "unexpected object contents at line",
                            json_token_reader_get_input_line( &((*this_).tokenizer) )
                        );
        result |= U8_ERROR_PARSER_STRUCTURE;
    }

    if ( U8_ERROR_NONE == result )
    {
        result = json_token_reader_expect_eof ( &((*this_).tokenizer) );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_check_end_top_array ( json_element_reader_t *this_, bool* out_end )
{
    TRACE_BEGIN();
    assert( NULL != out_end );
    u8_error_t result = U8_ERROR_NONE;

    result = json_token_reader_check_end_array ( &((*this_).tokenizer), out_end );

    if (( U8_ERROR_NONE == result ) && ( *out_end ))
    {
        (*this_).top_array_after_first_entry = false;
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_expect_begin_sub_array ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    result = json_token_reader_expect_begin_array( &((*this_).tokenizer) );
    if ( U8_ERROR_NONE == result )
    {
        (*this_).sub_array_after_first_entry = false;
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_check_end_sub_array ( json_element_reader_t *this_, bool* out_end )
{
    TRACE_BEGIN();
    assert( NULL != out_end );
    u8_error_t result = U8_ERROR_NONE;

    result = json_token_reader_check_end_array ( &((*this_).tokenizer), out_end );

    if ( U8_ERROR_NONE == result )
    {
        if ( *out_end )
        {
            (*this_).sub_array_after_first_entry = false;
        }
        else
        {
            if ( (*this_).sub_array_after_first_entry )
            {
                result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
            }
            else
            {
                (*this_).sub_array_after_first_entry = true;
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_expect_begin_type_of_element ( json_element_reader_t *this_, data_table_t *out_type )
{
    TRACE_BEGIN();
    assert ( NULL != out_type );
    u8_error_t result = U8_ERROR_NONE;

    bool array_end;
    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    result = json_token_reader_check_end_array ( &((*this_).tokenizer), &array_end );

    if ( U8_ERROR_NONE == result )
    {
        if ( array_end )
        {
            (*out_type) = DATA_TABLE_VOID;
        }
        else
        {
            if ( (*this_).top_array_after_first_entry )
            {
                result = json_token_reader_expect_value_separator ( &((*this_).tokenizer) );
            }
            else
            {
                (*this_).top_array_after_first_entry = true;
            }
            if ( U8_ERROR_NONE == result )
            {
                result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );
            }
            if ( U8_ERROR_NONE == result )
            {
                result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
            }
            if ( U8_ERROR_NONE == result )
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
                    result = U8_ERROR_PARSER_STRUCTURE;
                }
            }
            if ( U8_ERROR_NONE == result )
            {
                result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_expect_end_type_of_element ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    bool condition;

    result = json_token_reader_check_end_object ( &((*this_).tokenizer), &condition );
    if ( ! condition )
    {
        TSLOG_ERROR_INT( "unexpected object contents at line",
                         json_token_reader_get_input_line( &((*this_).tokenizer) )
                       );
        result |= U8_ERROR_PARSER_STRUCTURE;
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_get_next_classifier ( json_element_reader_t *this_,
                                                     data_classifier_t *out_object,
                                                     bool* out_has_features_array )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );
    assert ( NULL != out_has_features_array );
    u8_error_t result = U8_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_classifier_init_empty( out_object );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    bool break_at_features = false;
    while (( ! object_end )&&( ! break_at_features )&&( U8_ERROR_NONE == result ))
    {
        result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
        if ( U8_ERROR_NONE == result )
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
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                }
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                }
                if ( U8_ERROR_NONE == result )
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
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_MAIN_TYPE_NAME ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
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
                        result = json_element_reader_private_read_string_array( this_, (*this_).temp_string );
                        data_classifier_set_description( out_object,
                                                         utf8stringbuf_get_string( (*this_).temp_string )
                                                       );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_UUID ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                        result |= data_classifier_set_uuid( out_object,
                                                            utf8stringbuf_get_string( (*this_).temp_string )
                                                          );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_CLASSIFIER_FEATURES ) )
                    {
                        break_at_features = true;  /* end the loop */
                    }
                    else
                    {
                        TSLOG_ERROR_INT( "unexpected member name at line",
                                         json_token_reader_get_input_line( &((*this_).tokenizer) )
                                       );
                        result = U8_ERROR_PARSER_STRUCTURE;
                    }
                }
            }
        }
        else
        {
            TSLOG_ERROR_INT( "unexpected character at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result = U8_ERROR_PARSER_STRUCTURE;
            object_end = true;
        }
    }

    /* footer */

    if ( U8_ERROR_NONE == result )
    {
        data_classifier_trace( out_object );
    }

    *out_has_features_array = break_at_features;
    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_end_unfinished_object ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    bool object_end = false;
    result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
    if ( U8_ERROR_NONE == result )
    {
        if (! object_end)
        {
            result = U8_ERROR_PARSER_STRUCTURE;
        }
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_get_next_diagram ( json_element_reader_t *this_,
                                                  data_diagram_t *out_object,
                                                  utf8stringbuf_t out_parent_uuid,
                                                  bool* out_has_diagramelements_array )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );
    assert ( NULL != out_has_diagramelements_array );
    u8_error_t result = U8_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_diagram_init_empty( out_object );
    utf8stringbuf_clear( out_parent_uuid );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    bool break_at_subelements = false;
    while (( ! object_end )&&( ! break_at_subelements )&&( U8_ERROR_NONE == result ))
    {
        result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
        if ( U8_ERROR_NONE == result )
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
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                }
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                }
                if ( U8_ERROR_NONE == result )
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
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_PARENT_NAME ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                        data_diagram_set_diagram_type ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_DIAGRAM_TYPE_NAME ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_LIST_ORDER ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                        data_diagram_set_list_order ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_DESCRIPTION ) )
                    {
                        result = json_element_reader_private_read_string_array( this_, (*this_).temp_string );
                        data_diagram_set_description( out_object,
                                                      utf8stringbuf_get_string( (*this_).temp_string )
                                                    );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_NAME ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                        result |= data_diagram_set_name( out_object,
                                                         utf8stringbuf_get_string( (*this_).temp_string )
                                                       );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_STEREOTYPE ) )
                    {
                        /* new in 1.43.0: stereotype */
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAGS ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                        data_diagram_set_display_flags ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_DISPLAY_FLAG_NAMES ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_PARENT ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), out_parent_uuid );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_UUID ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                        result |= data_diagram_set_uuid( out_object,
                                                         utf8stringbuf_get_string( (*this_).temp_string )
                                                       );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAM_ELEMENTS ) )
                    {
                        break_at_subelements = true;  /* end the loop */
                    }
                    else
                    {
                        TSLOG_ERROR_INT( "unexpected member name at line",
                                         json_token_reader_get_input_line( &((*this_).tokenizer) )
                                       );
                        result = U8_ERROR_PARSER_STRUCTURE;
                    }
                }
            }
        }
        else
        {
            TSLOG_ERROR_INT( "unexpected character at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result = U8_ERROR_PARSER_STRUCTURE;
            object_end = true;
        }
    }

    /* footer */

    if ( U8_ERROR_NONE == result )
    {
        data_diagram_trace( out_object );
    }

    *out_has_diagramelements_array = break_at_subelements;
    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_get_next_relationship ( json_element_reader_t *this_,
                                                       data_relationship_t *out_object,
                                                       utf8stringbuf_t out_from_node_uuid,
                                                       utf8stringbuf_t out_to_node_uuid
                                                     )
{
    TRACE_BEGIN();
    assert( NULL != out_object );
    u8_error_t result = U8_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_relationship_init_empty( out_object );
    utf8stringbuf_clear( out_from_node_uuid );
    utf8stringbuf_clear( out_to_node_uuid );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    while (( ! object_end )&&( U8_ERROR_NONE == result ))
    {
        result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
        if ( U8_ERROR_NONE == result )
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
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                }
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                }
                if ( U8_ERROR_NONE == result )
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
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_MAIN_TYPE_NAME ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_STEREOTYPE ) )
                    {
                        /* new in 1.43.0: stereotype */
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_LIST_ORDER ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                        data_relationship_set_list_order ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_DESCRIPTION ) )
                    {
                        result = json_element_reader_private_read_string_array( this_, (*this_).temp_string );
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
                        TRACE_INFO_STR( "from-classifier name:", utf8stringbuf_get_string( (*this_).temp_string ) );
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
                        TRACE_INFO_STR( "from-feature key:", utf8stringbuf_get_string( (*this_).temp_string ) );
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
                        TRACE_INFO_STR( "to-classifier name:", utf8stringbuf_get_string( (*this_).temp_string ) );
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
                        TRACE_INFO_STR( "to-feature key:", utf8stringbuf_get_string( (*this_).temp_string ) );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_FROM_NODE ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), out_from_node_uuid );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_RELATIONSHIP_TO_NODE ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), out_to_node_uuid );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_UUID ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                        result |= data_relationship_set_uuid( out_object,
                                                              utf8stringbuf_get_string( (*this_).temp_string )
                                                            );
                    }
                    else
                    {
                        TSLOG_ERROR_INT( "unexpected member name at line",
                                         json_token_reader_get_input_line( &((*this_).tokenizer) )
                                       );
                        result = U8_ERROR_PARSER_STRUCTURE;
                    }
                }
            }
        }
        else
        {
            TSLOG_ERROR_INT( "unexpected character at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result = U8_ERROR_PARSER_STRUCTURE;
            object_end = true;
        }
    }

    /* footer */

    if ( U8_ERROR_NONE == result )
    {
        data_relationship_trace( out_object );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_skip_next_object ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    while (( ! object_end )&&( U8_ERROR_NONE == result ))
    {
        result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
        if ( U8_ERROR_NONE == result )
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
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_read_member_name ( &((*this_).tokenizer), member_name );
                }
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_expect_name_separator ( &((*this_).tokenizer) );
                }
                json_value_type_t v_type;
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_get_value_type ( &((*this_).tokenizer), &v_type );
                }
                if ( U8_ERROR_NONE == result )
                {
                    switch ( v_type )
                    {
                        case JSON_VALUE_TYPE_OBJECT:
                        {
                            result = U8_ERROR_PARSER_STRUCTURE; /* this function does not expect objects in objects */
                        }
                        break;

                        case JSON_VALUE_TYPE_ARRAY:
                        {
                            result = U8_ERROR_PARSER_STRUCTURE; /* this function does not expect arrays in objects */
                        }
                        break;

                        case JSON_VALUE_TYPE_NUMBER:
                        {
                            double parsed_number;
                            /* result = */ json_token_reader_read_number_value ( &((*this_).tokenizer), &parsed_number );
                            /* ignore the U8_ERROR_NOT_YET_IMPLEMENTED here... */
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
                            result = json_element_reader_skip_next_string( this_ );
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
                            result = U8_ERROR_PARSER_STRUCTURE; /* this function does not expect objects in objects */
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
            result = U8_ERROR_PARSER_STRUCTURE;
            object_end = true;
        }
    }

    /* footer */

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_skip_next_string ( json_element_reader_t *this_ )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;
    char dummy_str[4];
    utf8stringbuf_t dummy_strbuf = UTF8STRINGBUF ( dummy_str );

    result = json_token_reader_read_string_value ( &((*this_).tokenizer), dummy_strbuf );
    if ( result == U8_ERROR_STRING_BUFFER_EXCEEDED )
    {
        /* ignore this. The result string is not needed therefore dummy_str may be too small */
        result = U8_ERROR_NONE;
    }

    TRACE_END_ERR( result );
    return result;
}

void json_element_reader_get_read_line ( json_element_reader_t *this_, uint32_t *out_read_line )
{
    TRACE_BEGIN();
    assert ( NULL != out_read_line );

    (*out_read_line) = json_token_reader_get_input_line( &((*this_).tokenizer) );
    TRACE_INFO_INT( "line", *out_read_line );

    TRACE_END();
}

u8_error_t json_element_reader_get_next_feature ( json_element_reader_t *this_, data_feature_t *out_object )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );

    u8_error_t result = U8_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_feature_init_empty( out_object );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    while (( ! object_end )&&( U8_ERROR_NONE == result ))
    {
        result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
        if ( U8_ERROR_NONE == result )
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
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_read_member_name( &((*this_).tokenizer), member_name );
                }
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_expect_name_separator( &((*this_).tokenizer) );
                }
                if ( U8_ERROR_NONE == result )
                {
                    if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_ID ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                        data_feature_set_row_id ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                        data_feature_set_main_type( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_MAIN_TYPE_NAME ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_LIST_ORDER ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                        data_feature_set_list_order ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_FEATURE_DESCRIPTION ) )
                    {
                        result = json_element_reader_private_read_string_array( this_, (*this_).temp_string );
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
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                        result |= data_feature_set_uuid( out_object,
                                                         utf8stringbuf_get_string( (*this_).temp_string )
                                                       );
                    }
                    else
                    {
                        TSLOG_ERROR_INT( "unexpected member name at line",
                                         json_token_reader_get_input_line( &((*this_).tokenizer) )
                                       );
                        result = U8_ERROR_PARSER_STRUCTURE;
                    }
                }
            }
        }
        else
        {
            TSLOG_ERROR_INT( "unexpected character at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result = U8_ERROR_PARSER_STRUCTURE;
            object_end = true;
        }
    }

    /* footer */

    if ( U8_ERROR_NONE == result )
    {
        data_feature_trace( out_object );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_get_next_diagramelement( json_element_reader_t *this_,
                                                        data_diagramelement_t *out_object,
                                                        utf8stringbuf_t out_node_uuid )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );

    u8_error_t result = U8_ERROR_NONE;

    char member_name_buf[24];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    data_diagramelement_init_empty( out_object );

    /* header */

    result = json_token_reader_expect_begin_object ( &((*this_).tokenizer) );

    /* members */

    bool first_member_passed = false;
    bool object_end = false;
    while (( ! object_end )&&( U8_ERROR_NONE == result ))
    {
        result = json_token_reader_check_end_object ( &((*this_).tokenizer), &object_end );
        if ( U8_ERROR_NONE == result )
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
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_read_member_name( &((*this_).tokenizer), member_name );
                }
                if ( U8_ERROR_NONE == result )
                {
                    result = json_token_reader_expect_name_separator( &((*this_).tokenizer) );
                }
                if ( U8_ERROR_NONE == result )
                {
                    if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_ID ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                        data_diagramelement_set_row_id ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DIAGRAM_ID ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value( &((*this_).tokenizer), &parsed_integer );
                        data_diagramelement_set_diagram_row_id( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_ID ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                        data_diagramelement_set_classifier_row_id ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_CLASSIFIER_NAME ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_ID ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                        data_diagramelement_set_focused_feature_row_id ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE_NAME ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_FOCUSED_FEATURE ) )
                    {
                        /* new in 1.43.0: focused_feature */
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAGS ) )
                    {
                        int64_t parsed_integer;
                        result = json_token_reader_read_int_value ( &((*this_).tokenizer), &parsed_integer );
                        data_diagramelement_set_display_flags ( out_object, parsed_integer );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_DISPLAY_FLAG_NAMES ) )
                    {
                        result = json_element_reader_skip_next_string( this_ );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_DIAGRAMELEMENT_NODE ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), out_node_uuid );
                    }
                    else if ( utf8stringbuf_equals_str( member_name, JSON_CONSTANTS_KEY_UUID ) )
                    {
                        result = json_token_reader_read_string_value( &((*this_).tokenizer), (*this_).temp_string );
                        result |= data_diagramelement_set_uuid( out_object,
                                                                utf8stringbuf_get_string( (*this_).temp_string )
                                                              );
                    }
                    else
                    {
                        TSLOG_ERROR_INT( "unexpected member name at line",
                                         json_token_reader_get_input_line( &((*this_).tokenizer) )
                                       );
                        result = U8_ERROR_PARSER_STRUCTURE;
                    }
                }
            }
        }
        else
        {
            TSLOG_ERROR_INT( "unexpected character at line",
                             json_token_reader_get_input_line( &((*this_).tokenizer) )
                           );
            result = U8_ERROR_PARSER_STRUCTURE;
            object_end = true;
        }
    }

    /* footer */

    if ( U8_ERROR_NONE == result )
    {
        data_diagramelement_trace( out_object );
    }

    TRACE_END_ERR( result );
    return result;
}

u8_error_t json_element_reader_private_read_string_array ( json_element_reader_t *this_,
                                                           utf8stringbuf_t out_joined_string )
{
    TRACE_BEGIN();
    u8_error_t result = U8_ERROR_NONE;

    utf8stringbuf_clear( out_joined_string );

    result = json_token_reader_expect_begin_array( &((*this_).tokenizer) );

    bool end_array = false;
    bool first_element_passed = false;
    while (( ! end_array )&&( U8_ERROR_NONE == result ))
    {
        result = json_token_reader_check_end_array( &((*this_).tokenizer), &end_array );
        if ( U8_ERROR_NONE == result )
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
                utf8stringbuf_t unused_remaining = utf8stringbuf_get_end( out_joined_string );
                result |= json_token_reader_read_string_value( &((*this_).tokenizer), unused_remaining );
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
