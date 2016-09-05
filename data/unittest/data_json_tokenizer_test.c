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
static void test_parse_double(void);


TestRef data_json_tokenizer_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_skip_whitespace",test_skip_whitespace),
        new_TestFixture("test_is_token_end",test_is_token_end),
        new_TestFixture("test_get_value_type",test_get_value_type),
        new_TestFixture("test_find_string_end",test_find_string_end),
        new_TestFixture("test_parse_integer",test_parse_integer),
        new_TestFixture("test_parse_double",test_parse_double),
    };
    EMB_UNIT_TESTCALLER(result,"data_json_tokenizer_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_skip_whitespace(void)
{
    const char test_str[17] = "1234  \t\t\r\r\n\ndef0";
    data_json_tokenizer_t tok;
    uint32_t pos;

    /* init */
    data_json_tokenizer_init( &tok );

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

    /* destroy */
    data_json_tokenizer_destroy( &tok );
}

static void test_is_token_end(void)
{
    const char test_str[17] = "+2f\r"  "5,7:"  "9}b "  "\"e\".";
    const char results[18] = "1001"  "1111"  "1111"  "10001";
    data_json_tokenizer_t tok;
    uint32_t pos;
    bool is_end;
    assert( sizeof(test_str)+1 == sizeof(results) );

    /* init */
    data_json_tokenizer_init( &tok );

    /* test all positions */
    for ( pos = 0; pos < (sizeof(results)-1); pos ++ )
    {
        is_end = data_json_tokenizer_private_is_token_end( &tok, test_str, &pos );
        TEST_ASSERT_EQUAL_INT( results[pos]=='1', is_end );
    }

    /* destroy */
    data_json_tokenizer_destroy( &tok );
}

static void test_get_value_type(void)
{
    data_error_t test_err;
    const char test_str[12*4+1] = "\n{a:"  " [\t\r"  "-12,"  "2.4,"  "\"s\","  "\tnull\r, "  "true   ,"  "false   "  " ] }";
    data_json_tokenizer_t tok;
    uint32_t pos;
    data_json_value_type_t value_type;

    /* init */
    data_json_tokenizer_init( &tok );

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

    /* destroy */
    data_json_tokenizer_destroy( &tok );
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
    data_json_tokenizer_t tok;
    uint32_t pos;

    /* init */
    data_json_tokenizer_init( &tok );

    /* test all positions */
    for ( int idx = 0; idx < 5; idx ++ )
    {
        pos = 1;
        data_json_tokenizer_private_find_string_end( &tok, test_str[idx], &pos );
        TEST_ASSERT_EQUAL_INT( 15, pos );
    }

    /* destroy */
    data_json_tokenizer_destroy( &tok );
}

static void test_parse_integer(void)
{
    const char test_str[17] = "+2f\r"  "5,7:"  "9}b "  "\"e\".";
    const char results[18] = "1001"  "1111"  "1111"  "10001";
    data_json_tokenizer_t tok;
    uint32_t pos;
    bool is_end;
    assert( sizeof(test_str)+1 == sizeof(results) );

    /* init */
    data_json_tokenizer_init( &tok );

    /* test all positions */
    for ( pos = 0; pos < (sizeof(results)-1); pos ++ )
    {
        is_end = data_json_tokenizer_private_is_token_end( &tok, test_str, &pos );
        TEST_ASSERT_EQUAL_INT( results[pos]=='1', is_end );
    }

    /* destroy */
    data_json_tokenizer_destroy( &tok );
}

static void test_parse_double(void)
{
    const char test_str[17] = "+2f\r"  "5,7:"  "9}b "  "\"e\".";
    const char results[18] = "1001"  "1111"  "1111"  "10001";
    data_json_tokenizer_t tok;
    uint32_t pos;
    bool is_end;
    assert( sizeof(test_str)+1 == sizeof(results) );

    /* init */
    data_json_tokenizer_init( &tok );

    /* test all positions */
    for ( pos = 0; pos < (sizeof(results)-1); pos ++ )
    {
        is_end = data_json_tokenizer_private_is_token_end( &tok, test_str, &pos );
        TEST_ASSERT_EQUAL_INT( results[pos]=='1', is_end );
    }

    /* destroy */
    data_json_tokenizer_destroy( &tok );
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
