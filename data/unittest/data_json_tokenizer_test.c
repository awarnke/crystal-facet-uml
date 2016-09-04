/* File: data_json_tokenizer_test.c; Copyright and License: see below */

#include "data_json_tokenizer_test.h"
#include "serial/data_json_tokenizer.h"

static void set_up(void);
static void tear_down(void);
static void test_skip_whitespace(void);
static void test_get_value_type(void);

TestRef data_json_tokenizer_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_skip_whitespace",test_skip_whitespace),
        new_TestFixture("test_get_value_type",test_get_value_type),
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
