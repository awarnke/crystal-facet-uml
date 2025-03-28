/* File: json_token_reader_test.c; Copyright and License: see below */

#include "json_token_reader_test.h"
#include "json/json_token_reader.h"
#include "u8stream/universal_memory_input_stream.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_skip_whitespace( test_fixture_t *fix );
static test_case_result_t test_is_value_end( test_fixture_t *fix );
static test_case_result_t test_get_value_type( test_fixture_t *fix );
static test_case_result_t test_parse_string( test_fixture_t *fix );
static test_case_result_t test_parse_integer( test_fixture_t *fix );
static test_case_result_t test_skip_number( test_fixture_t *fix );
static test_case_result_t test_parse( test_fixture_t *fix );


test_suite_t json_token_reader_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "json_token_reader_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_skip_whitespace", &test_skip_whitespace );
    test_suite_add_test_case( &result, "test_is_value_end", &test_is_value_end );
    test_suite_add_test_case( &result, "test_get_value_type", &test_get_value_type );
    test_suite_add_test_case( &result, "test_parse_string", &test_parse_string );
    test_suite_add_test_case( &result, "test_parse_integer", &test_parse_integer );
    test_suite_add_test_case( &result, "test_skip_number", &test_skip_number );
    test_suite_add_test_case( &result, "test_parse", &test_parse );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static json_token_reader_t tok;

static test_case_result_t test_skip_whitespace( test_fixture_t *fix )
{
    universal_memory_input_stream_t test_input;
    const char test_str1[3] = "4 ";
    universal_memory_input_stream_init( &test_input, &test_str1, sizeof(test_str1) );
    json_token_reader_init( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    /* test skip nothing */
    json_token_reader_private_skip_whitespace( &tok );
    TEST_EXPECT_EQUAL_INT( 0, json_token_reader_get_input_pos( &tok ) );

    /* reset to test_str2 */
    const char test_str2[10] = "  \t\t\r\r\n\nd";
    universal_memory_input_stream_reinit( &test_input, &test_str2, sizeof(test_str2) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    /* test skip at string end */
    json_token_reader_private_skip_whitespace( &tok );
    TEST_EXPECT_EQUAL_INT( 8, json_token_reader_get_input_pos( &tok ) );

    /* reset to test_str3 */
    const char test_str3[1] = "";
    universal_memory_input_stream_reinit( &test_input, &test_str3, sizeof(test_str3) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    /* test skip all 4 whitespace types */
    json_token_reader_private_skip_whitespace( &tok );
    TEST_EXPECT_EQUAL_INT( 0, json_token_reader_get_input_pos( &tok ) );

    json_token_reader_destroy( &tok );
    universal_memory_input_stream_destroy( &test_input );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_is_value_end( test_fixture_t *fix )
{
    const char test_str[17] = "+2f\r"  "5,7:"  "9}b "  "\"e\".";
    const char results[18] = "0001"  "0101"  "0101"  "00001";
    assert( sizeof(test_str)+1 == sizeof(results) );
    universal_memory_input_stream_t test_input;
    universal_memory_input_stream_init( &test_input, &test_str, sizeof(test_str) );
    json_token_reader_init( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    /* test all positions */
    for ( int pos = 0; pos < (sizeof(results)-1); pos ++ )
    {
        /* reset to test_str[pos] */
        universal_memory_input_stream_reinit( &test_input, &(test_str[pos]), sizeof(char) );
        json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

        const bool is_end = json_token_reader_private_is_value_end( &tok );
        TEST_EXPECT_EQUAL_INT( ( results[pos] == '1' ), is_end );
    }

    json_token_reader_destroy( &tok );
    universal_memory_input_stream_destroy( &test_input );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_get_value_type( test_fixture_t *fix )
{
    u8_error_t test_err;
    universal_memory_input_stream_t test_input;
    json_value_type_t value_type;

    /* JSON_VALUE_TYPE_OBJECT */
    const char test_str_1[4] = "\n{a";
    universal_memory_input_stream_init( &test_input, &test_str_1, sizeof(test_str_1) );
    json_token_reader_init( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 1, json_token_reader_get_input_pos( &tok ) );  /* there is a whitespace in front of the value */
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_OBJECT, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, test_err );

    /* JSON_VALUE_TYPE_ARRAY */
    const char test_str_2[4] = " [\t";
    universal_memory_input_stream_reinit( &test_input, &test_str_2, sizeof(test_str_2) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 1, json_token_reader_get_input_pos( &tok ) );  /* there is a whitespace in front of the value */
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_ARRAY, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, test_err );

    /* JSON_VALUE_TYPE_NUMBER */
    const char test_str_3[4] = "-12";
    universal_memory_input_stream_reinit( &test_input, &test_str_3, sizeof(test_str_3) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 0, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_NUMBER, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, test_err );

    /* JSON_VALUE_TYPE_STRING */
    const char test_str_4[4] = "\"s\"";
    universal_memory_input_stream_reinit( &test_input, &test_str_4, sizeof(test_str_4) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 0, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_STRING, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, test_err );

    /* JSON_VALUE_TYPE_NULL */
    const char test_str_5[4] = "nul";
    universal_memory_input_stream_reinit( &test_input, &test_str_5, sizeof(test_str_5) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 0, json_token_reader_get_input_pos( &tok ) );  /* there is a whitespace in front of the value */
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_NULL, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, test_err );

    /* JSON_VALUE_TYPE_BOOLEAN */
    const char test_str_6[4] = "fal";
    universal_memory_input_stream_reinit( &test_input, &test_str_6, sizeof(test_str_6) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 0, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_BOOLEAN, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, test_err );

    /* JSON_VALUE_TYPE_BOOLEAN */
    const char test_str_7[4] = "\n\tt";
    universal_memory_input_stream_reinit( &test_input, &test_str_7, sizeof(test_str_7) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 2, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_BOOLEAN, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, test_err );

    /* JSON_VALUE_TYPE_UNDEF */
    const char test_str_8[4] = " ]}";
    universal_memory_input_stream_reinit( &test_input, &test_str_8, sizeof(test_str_8) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 1, json_token_reader_get_input_pos( &tok ) );  /* there is a whitespace in front of the not-value */
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_UNDEF, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, test_err );

    /* EOF */
    const char test_str_9[4] = "  \r";
    universal_memory_input_stream_reinit( &test_input, &test_str_9, sizeof(test_str_9) );
    json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    test_err = json_token_reader_get_value_type ( &tok, &value_type );
    TEST_EXPECT_EQUAL_INT( 3, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( JSON_VALUE_TYPE_UNDEF, value_type );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, test_err );

    json_token_reader_destroy( &tok );
    universal_memory_input_stream_destroy( &test_input );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_parse_string( test_fixture_t *fix )
{
   u8_error_t test_err;
   const char test_str[6][17] = {
        "\"\'\'simple    \'\'\"",
        "\"\\\\mixed    \\n\'\"",
        "\"\'\'esc at end\\\\\"",
        "\"\'\'2quote@end\\\"\"",
        "\"\'\'trailing\"trail",
        "\"\'\'    no_end\\\"",
    };
    char parsed_buf[20];
    utf8stringbuf_t parsed_str = UTF8STRINGBUF( parsed_buf );
    const char expect_str[6][15] = {
        "\'\'simple    \'\'",
        "\\mixed    \n\'",
        "\'\'esc at end\\",
        "\'\'2quote@end\"",
        "\'\'trailing",
        "\'\'    no_end\"",
    };
    universal_memory_input_stream_t test_input;
    universal_memory_input_stream_init( &test_input, &(test_str[0]), sizeof(test_str[0]) );
    json_token_reader_init( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    /* test all positions */
    for ( int index = 0; index < 6; index ++ )
    {
        universal_memory_input_stream_reinit( &test_input, &(test_str[index]), sizeof(test_str[index]) );
        json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

        test_err = json_token_reader_read_string_value( &tok, parsed_str );
        TEST_EXPECT_EQUAL_INT( (index>=4)?U8_ERROR_LEXICAL_STRUCTURE:U8_ERROR_NONE, test_err );
        TEST_EXPECT_EQUAL_INT( 1, utf8stringbuf_equals_str( &parsed_str, expect_str[index] ) );
    }

    json_token_reader_destroy( &tok );
    universal_memory_input_stream_destroy( &test_input );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_parse_integer( test_fixture_t *fix )
{
    u8_error_t test_err;
    const char test_str[8][16] = {
        "  0",
        "  00",
        "  -33",
        "  -0",
        "-12345678,other",
        "  -hello",
        "  123]",
        "  abc",
    };
    const int64_t int_results[8] = {0,0,-33,0,-12345678,0,123,0};
    const int err_results[8] =     {0,1,  0,1,        0,1,  0,1};
    int64_t int_result;
    universal_memory_input_stream_t test_input;
    universal_memory_input_stream_init( &test_input, &(test_str[0]), sizeof(test_str[0]) );
    json_token_reader_init( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    /* test all positions */
    for ( int index = 0; index < 8; index ++ )
    {
        universal_memory_input_stream_reinit( &test_input, &(test_str[index]), sizeof(test_str[index]) );
        json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

        test_err = json_token_reader_read_int_value( &tok, &int_result );
        TEST_EXPECT_EQUAL_INT( (err_results[index]!=0)?U8_ERROR_LEXICAL_STRUCTURE:U8_ERROR_NONE, test_err );
        TEST_EXPECT_EQUAL_INT( int_results[index], int_result );
        /* printf("parsed: %" PRId64 "\n",int_result); */
    }

    json_token_reader_destroy( &tok );
    universal_memory_input_stream_destroy( &test_input );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_skip_number( test_fixture_t *fix )
{
    u8_error_t test_err;
    const char test_str[8][16] = {
        "0.0000 ",
        "000000 ",
        "-33e+0 ",
        "-1.456;",
        "1.5e+8,",
        "1.4E88}",
        "1.5E-8]",
        "+123E8\n",
    };
    universal_memory_input_stream_t test_input;
    universal_memory_input_stream_init( &test_input, &(test_str[0]), sizeof(test_str[0]) );
    json_token_reader_init( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    /* test all positions */
    for ( int index = 0; index < 8; index ++ )
    {
        universal_memory_input_stream_reinit( &test_input, &(test_str[index]), sizeof(test_str[index]) );
        json_token_reader_reinit( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

        test_err = json_token_reader_private_skip_number( &tok );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, test_err );
        TEST_EXPECT_EQUAL_INT( 6, json_token_reader_get_input_pos( &tok ) );  /* all numbers have 6 digits */
    }

    json_token_reader_destroy( &tok );
    universal_memory_input_stream_destroy( &test_input );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_parse( test_fixture_t *fix )
{
    static const char test_str[] =
        "\n{"
        "\n  \"data\": ["
        "\n    {"
        "\n      \"classifier\"\t: {"
        "\n        \"id\"  \r\n:-99,"
        "\n        \"main_type\" \n\t\r : 90.0e+0 "
        "\n        \"stereotype\\r\\/\\\"\\\\\": \"\\f\\n\\t\\b\\r\\/\\\"\\\\\""
        "\n      }"
        "\n    },"
        "\n    null,"
        "\n    true,"
        "\n    false"
        "\n  ]"
        "\n}"
        "\n";
    u8_error_t res;
    bool cond;
    char my_buf[32];
    utf8stringbuf_t my_string = UTF8STRINGBUF( my_buf );
    int64_t my_int;
    double my_double;
    bool my_bool;
    universal_memory_input_stream_t test_input;
    universal_memory_input_stream_init( &test_input, &test_str, sizeof(test_str) );
    json_token_reader_init( &tok, universal_memory_input_stream_get_input_stream( &test_input ) );

    res = json_token_reader_expect_begin_object ( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 2, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_check_end_object ( &tok, &cond );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 5, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( false, cond );

    res = json_token_reader_read_member_name ( &tok, my_string );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 11, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( "data", utf8stringbuf_get_string( &my_string )) );

    res = json_token_reader_expect_name_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 12, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_expect_begin_array ( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 14, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_check_end_array ( &tok, &cond );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 19, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( false, cond );

    res = json_token_reader_expect_begin_object ( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 20, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_check_end_object ( &tok, &cond );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 27, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( false, cond );

    res = json_token_reader_read_member_name ( &tok, my_string );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 39, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( "classifier", utf8stringbuf_get_string( &my_string )) );

    res = json_token_reader_expect_name_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 41, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_expect_begin_object ( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 43, json_token_reader_get_input_pos( &tok ) );

    /* skip test for end object here */

    res = json_token_reader_read_member_name ( &tok, my_string );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 56, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( "id", utf8stringbuf_get_string( &my_string )) );

    res = json_token_reader_expect_name_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 61, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_read_int_value ( &tok, &my_int );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 64, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( -99, my_int );

    /* skip test for end object here */

    res = json_token_reader_expect_value_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 65, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_read_member_name ( &tok, my_string );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 85, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( "main_type", utf8stringbuf_get_string( &my_string )) );

    res = json_token_reader_expect_name_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 91, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_read_number_value ( &tok, &my_double );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NOT_YET_IMPLEMENTED, res );
    TEST_EXPECT_EQUAL_INT( 99, json_token_reader_get_input_pos( &tok ) );

    /* skip test for end object here */

    res = json_token_reader_read_member_name ( &tok, my_string );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 129, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( "stereotype\r/\"\\", utf8stringbuf_get_string( &my_string )) );

    res = json_token_reader_expect_name_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 130, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_read_string_value ( &tok, my_string );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 149, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( "\f\n\t\b\r/\"\\", utf8stringbuf_get_string( &my_string )) );

    res = json_token_reader_check_end_object ( &tok, &cond );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 157, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( true, cond );

    res = json_token_reader_check_end_object ( &tok, &cond );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 163, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( true, cond );

    /* skip test for end array here */

    res = json_token_reader_expect_value_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 164, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_expect_null_value ( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 173, json_token_reader_get_input_pos( &tok ) );

    /* skip test for end array here */

    res = json_token_reader_expect_value_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 174, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_read_boolean_value ( &tok, &my_bool );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 183, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( true, my_bool );

    /* skip test for end array here */

    res = json_token_reader_expect_value_separator( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 184, json_token_reader_get_input_pos( &tok ) );

    res = json_token_reader_read_boolean_value ( &tok, &my_bool );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 194, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( false, my_bool );

    res = json_token_reader_check_end_array ( &tok, &cond );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 198, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( true, cond );

    res = json_token_reader_check_end_object ( &tok, &cond );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 200, json_token_reader_get_input_pos( &tok ) );
    TEST_EXPECT_EQUAL_INT( true, cond );

    res = json_token_reader_expect_eof ( &tok );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, res );
    TEST_EXPECT_EQUAL_INT( 201, json_token_reader_get_input_pos( &tok ) );

    json_token_reader_destroy( &tok );
    universal_memory_input_stream_destroy( &test_input );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2016-2025 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
