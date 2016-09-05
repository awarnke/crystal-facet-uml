/* File: data_json_tokenizer_test.c; Copyright and License: see below */

#include "data_json_tokenizer_test.h"
#include "serial/data_json_tokenizer.h"

static void set_up(void);
static void tear_down(void);
static void test_skip_whitespace(void);
static void test_is_token_end(void);
static void test_get_value_type(void);
static void test_find_string_end(void);
static void test_parse_integer(void);
static void test_skip_number(void);
static void test_parse(void);


TestRef data_json_tokenizer_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_skip_whitespace",test_skip_whitespace),
        new_TestFixture("test_is_token_end",test_is_token_end),
        new_TestFixture("test_get_value_type",test_get_value_type),
        new_TestFixture("test_find_string_end",test_find_string_end),
        new_TestFixture("test_parse_integer",test_parse_integer),
        new_TestFixture("test_skip_number",test_skip_number),
        new_TestFixture("test_parse",test_parse),
    };
    EMB_UNIT_TESTCALLER(result,"data_json_tokenizer_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static data_json_tokenizer_t tok;

static void set_up(void)
{
    data_json_tokenizer_init( &tok );
}

static void tear_down(void)
{
    data_json_tokenizer_destroy( &tok );
}

static void test_skip_whitespace(void)
{
    const char test_str[17] = "1234  \t\t\r\r\n\ndef0";
    uint32_t pos;

    /* test skip nothing */
    pos = 3;
    data_json_tokenizer_private_skip_whitespace( &tok, test_str, &pos );
    TEST_ASSERT_EQUAL_INT( 3, pos );

    /* test skip at string end */
    pos = 16;
    data_json_tokenizer_private_skip_whitespace( &tok, test_str, &pos );
    TEST_ASSERT_EQUAL_INT( 16, pos );

    /* test skip all 4 whitespace types */
    pos = 4;
    data_json_tokenizer_private_skip_whitespace( &tok, test_str, &pos );
    TEST_ASSERT_EQUAL_INT( 12, pos );
}

static void test_is_token_end(void)
{
    const char test_str[17] = "+2f\r"  "5,7:"  "9}b "  "\"e\".";
    const char results[18] = "1001"  "1111"  "1111"  "10001";
    uint32_t pos;
    bool is_end;
    assert( sizeof(test_str)+1 == sizeof(results) );

    /* test all positions */
    for ( pos = 0; pos < (sizeof(results)-1); pos ++ )
    {
        is_end = data_json_tokenizer_private_is_token_end( &tok, test_str, &pos );
        TEST_ASSERT_EQUAL_INT( results[pos]=='1', is_end );
    }
}

static void test_get_value_type(void)
{
    data_error_t test_err;
    const char test_str[12*4+1] = "\n{a:"  " [\t\r"  "-12,"  "2.4,"  "\"s\","  "\tnull\r, "  "true   ,"  "false   "  " ] }";
    uint32_t pos;
    data_json_value_type_t value_type;

    /* DATA_JSON_VALUE_TYPE_OBJECT */
    pos = 0;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 1, pos );  /* there is a whitespace in front of the value */
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_OBJECT, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, test_err );

    /* DATA_JSON_VALUE_TYPE_ARRAY */
    pos = 4;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 5, pos );  /* there is a whitespace in front of the value */
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_ARRAY, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, test_err );

    /* DATA_JSON_VALUE_TYPE_INTEGER */
    pos = 8;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 8, pos );
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_INTEGER, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, test_err );

    /* DATA_JSON_VALUE_TYPE_NUMBER */
    pos = 12;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 12, pos );
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_NUMBER, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, test_err );

    /* DATA_JSON_VALUE_TYPE_STRING */
    pos = 16;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 16, pos );
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_STRING, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, test_err );

    /* DATA_JSON_VALUE_TYPE_NULL */
    pos = 20;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 21, pos );  /* there is a whitespace in front of the value */
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_NULL, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, test_err );

    /* DATA_JSON_VALUE_TYPE_BOOLEAN */
    pos = 28;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 28, pos );
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_BOOLEAN, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, test_err );

    /* DATA_JSON_VALUE_TYPE_BOOLEAN */
    pos = 36;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 36, pos );
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_BOOLEAN, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, test_err );

    /* DATA_JSON_VALUE_TYPE_UNDEF */
    pos = 44;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 45, pos );  /* there is a whitespace in front of the not-value */
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_UNDEF, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_PARSER_STRUCTURE, test_err );

    /* EOF */
    pos = 48;
    test_err = data_json_tokenizer_get_value_type ( &tok, test_str, &pos, &value_type );
    TEST_ASSERT_EQUAL_INT( 48, pos );
    TEST_ASSERT_EQUAL_INT( DATA_JSON_VALUE_TYPE_UNDEF, value_type );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_PARSER_STRUCTURE, test_err );
}

static void test_find_string_end(void)
{
    const char test_str[5][17] = {
        "\"\'\'simple    \'\'\"",
        "\"\\\\mixed    \\n\'\"",
        "\"\'\'esc at end\\\\\"",
        "\"\'\'2quote@end\\\"\"",
        "\"\'\'    no_end\\\"",
    };
    uint32_t pos;

    /* test all positions */
    for ( int idx = 0; idx < 5; idx ++ )
    {
        pos = 1;
        data_json_tokenizer_private_find_string_end( &tok, test_str[idx], &pos );
        TEST_ASSERT_EQUAL_INT( 15, pos );
    }
}

static void test_parse_integer(void)
{
    const char test_str[8][18] = {
        "  0",
        "  00",
        "  -33",
        "  -0",
        "  -12345678;other",
        "  -hello",
        "  123",
        "  abc",
    };
    const int64_t int_results[8] = {0,0,-33,0,-12345678,0,123,0};
    const uint32_t pos_results[8] = {3,3,5,2,11,2,5,2};
    uint32_t pos;
    uint64_t int_result;

    /* test all positions */
    for ( int index = 0; index < 8; index ++ )
    {
        pos = 2;
        int_result = data_json_tokenizer_private_parse_integer( &tok, test_str[index], &pos );
        TEST_ASSERT_EQUAL_INT( int_results[index], int_result );
        TEST_ASSERT_EQUAL_INT( pos_results[index], pos );
    }
}

static void test_skip_number(void)
{
    const char test_str[12][18] = {
        "  0.000",
        "  00.0",
        "  -33",
        "  -0.00",
        "  -12.45678;other",
        "  -hello",
        "  123.5e+8",
        "  123.4E88",
        "  123.5E+8",
        "  123.4E",
        "  123.5e0",
        "  123E888",
    };
    const uint32_t pos_results[12] = {7,6,5,7,11,3,10,10,10,8,9,9};
    uint32_t pos;

    /* test all positions */
    for ( int idx = 0; idx < 12; idx ++ )
    {
        pos = 2;
        data_json_tokenizer_private_skip_number( &tok, test_str[idx], &pos );
        TEST_ASSERT_EQUAL_INT( pos_results[idx], pos );
    }
}

static void test_parse(void)
{
    static const char test_json[] =
        "\n{"
        "\n  \"set\": ["
        "\n    {"
        "\n      \"classifier\"\t: {"
        "\n        \"id\"  \r\n:-99,"
        "\n        \"main_type\": 90,"
        "\n        \"stereotype\": \"\","
        "\n        \"name\": \"Master\","
        "\n        \"description\": \"\","
        "\n        \"x_order\": 0,"
        "\n        \"y_order\": 0"
        "\n      }"
        "\n    },"
        "\n    {"
        "\n      \"diagram\": {"
        "\n        \"id\": 1,"
        "\n        \"diagram_type\": 30,"
        "\n        \"name\": \"Overview\","
        "\n        \"description\": \"\","
        "\n        \"list_order\": 0"
        "\n      }"
        "\n    }"
        "\n  ]"
        "\n}"
        "\n";
    data_error_t res;
    uint32_t pos = 0;
    bool cond;
    char my_buf[32];
    utf8stringbuf_t my_string = UTF8STRINGBUF( my_buf );
    int64_t my_int;

    res = data_json_tokenizer_expect_begin_object ( &tok, test_json, &pos );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 2, pos );

    res = data_json_tokenizer_is_end_object ( &tok, test_json, &pos, &cond );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 5, pos );
    TEST_ASSERT_EQUAL_INT( false, cond );

    res = data_json_tokenizer_get_member_name ( &tok, test_json, &pos, my_string );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 10, pos );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "set", utf8stringbuf_get_string(my_string)) );

    res = data_json_tokenizer_expect_name_separator( &tok, test_json, &pos );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 11, pos );

    res = data_json_tokenizer_expect_begin_array ( &tok, test_json, &pos );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 13, pos );

    res = data_json_tokenizer_is_end_array ( &tok, test_json, &pos, &cond );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 18, pos );
    TEST_ASSERT_EQUAL_INT( false, cond );

    res = data_json_tokenizer_expect_begin_object ( &tok, test_json, &pos );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 19, pos );

    res = data_json_tokenizer_is_end_object ( &tok, test_json, &pos, &cond );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 26, pos );
    TEST_ASSERT_EQUAL_INT( false, cond );

    res = data_json_tokenizer_get_member_name ( &tok, test_json, &pos, my_string );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 38, pos );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "classifier", utf8stringbuf_get_string(my_string)) );

    res = data_json_tokenizer_expect_name_separator( &tok, test_json, &pos );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 40, pos );

    res = data_json_tokenizer_expect_begin_object ( &tok, test_json, &pos );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 42, pos );

    /* skip test for end object here */

    res = data_json_tokenizer_get_member_name ( &tok, test_json, &pos, my_string );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 55, pos );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( "id", utf8stringbuf_get_string(my_string)) );

    res = data_json_tokenizer_expect_name_separator( &tok, test_json, &pos );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 60, pos );

    res = data_json_tokenizer_get_int_value ( &tok, test_json, &pos, &my_int );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, res );
    TEST_ASSERT_EQUAL_INT( 63, pos );
    TEST_ASSERT_EQUAL_INT( -99, my_int );

}

/*
 * Copyright 2016-2016 Andreas Warnke
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
