/* File: u8__test.c; Copyright and License: see below */

#include "u8__test.h"
#include "u8/u8_error.h"
#include "u8/u8_error_info.h"
#include "u8/u8_f64.h"
#include "u8/u8_i32.h"
#include "u8/u8_u64.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_error( test_fixture_t *fix );
static test_case_result_t test_error_info( test_fixture_t *fix );
static test_case_result_t test_f64( test_fixture_t *fix );
static test_case_result_t test_i32( test_fixture_t *fix );
static test_case_result_t test_u64( test_fixture_t *fix );

test_suite_t u8__test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "u8__test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_error", &test_error );
    test_suite_add_test_case( &result, "test_error_info", &test_error_info );
    test_suite_add_test_case( &result, "test_f64", &test_f64 );
    test_suite_add_test_case( &result, "test_i32", &test_i32 );
    test_suite_add_test_case( &result, "test_u64", &test_u64 );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_error( test_fixture_t *fix )
{
    bool contains;

    contains = u8_error_contains( U8_ERROR_DUPLICATE | U8_ERROR_ARRAY_BUFFER_EXCEEDED, U8_ERROR_DUPLICATE );
    TEST_EXPECT_EQUAL_INT( true, contains );
    contains = u8_error_contains( U8_ERROR_END_OF_STREAM, U8_ERROR_END_OF_STREAM );
    TEST_EXPECT_EQUAL_INT( true, contains );
    contains = u8_error_contains( U8_ERROR_ARRAY_BUFFER_EXCEEDED, U8_ERROR_DUPLICATE );
    TEST_EXPECT_EQUAL_INT( false, contains );

    bool more;

    more = u8_error_more_than( U8_ERROR_DUPLICATE, U8_ERROR_DUPLICATE );
    TEST_EXPECT_EQUAL_INT( false, more );
    more = u8_error_more_than( U8_ERROR_DUPLICATE | U8_ERROR_ARRAY_BUFFER_EXCEEDED, U8_ERROR_DUPLICATE );
    TEST_EXPECT_EQUAL_INT( true, more );
    more = u8_error_more_than( U8_ERROR_DUPLICATE, U8_ERROR_ARRAY_BUFFER_EXCEEDED );
    TEST_EXPECT_EQUAL_INT( true, more );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_error_info( test_fixture_t *fix )
{
    u8_error_info_t probe;
    bool is_err;
    u8_error_t err;
    u8_error_info_unit_t unit;
    int32_t line;
    const char* name;

    /* test init_void */
    u8_error_info_init_void( &probe );

    is_err = u8_error_info_is_error( &probe );
    TEST_EXPECT_EQUAL_INT( false, is_err );
    err = u8_error_info_get_error( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    unit = u8_error_info_get_unit( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_VOID, unit );
    line = u8_error_info_get_line( &probe );
    TEST_EXPECT_EQUAL_INT( -1, line );
    name = u8_error_info_get_name( &probe );
    TEST_EXPECT_EQUAL_STRING( "", name );

    u8_error_info_destroy( &probe );

    /* test init */
    u8_error_info_init( &probe, U8_ERROR_PARSER_STRUCTURE );

    is_err = u8_error_info_is_error( &probe );
    TEST_EXPECT_EQUAL_INT( true, is_err );
    err = u8_error_info_get_error( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, err );
    unit = u8_error_info_get_unit( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_VOID, unit );
    line = u8_error_info_get_line( &probe );
    TEST_EXPECT_EQUAL_INT( -1, line );
    name = u8_error_info_get_name( &probe );
    TEST_EXPECT_EQUAL_STRING( "", name );

    u8_error_info_destroy( &probe );

    /* test init_line */
    u8_error_info_init_line( &probe, U8_ERROR_PARSER_STRUCTURE, 42 );

    is_err = u8_error_info_is_error( &probe );
    TEST_EXPECT_EQUAL_INT( true, is_err );
    err = u8_error_info_get_error( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, err );
    unit = u8_error_info_get_unit( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE, unit );
    line = u8_error_info_get_line( &probe );
    TEST_EXPECT_EQUAL_INT( 42, line );
    name = u8_error_info_get_name( &probe );
    TEST_EXPECT_EQUAL_STRING( "", name );

    u8_error_info_destroy( &probe );

    /* test init_name */
    u8_error_info_init_name( &probe, U8_ERROR_READ_ONLY_DB, "1 Hello World 15 ... 22 ... 29 ... 36 ... 43 ... 50" );

    is_err = u8_error_info_is_error( &probe );
    TEST_EXPECT_EQUAL_INT( true, is_err );
    err = u8_error_info_get_error( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_READ_ONLY_DB, err );
    unit = u8_error_info_get_unit( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_NAME, unit );
    line = u8_error_info_get_line( &probe );
    TEST_EXPECT_EQUAL_INT( -1, line );
    name = u8_error_info_get_name( &probe );
    TEST_EXPECT_EQUAL_STRING( "1 Hello World 15 ... 22 ... 29 ... 36 ... 43 ..", name );

    u8_error_info_destroy( &probe );

    /* test init_line_name */
    u8_error_info_init_line_name( &probe, U8_ERROR_PARSER_STRUCTURE, 1000, "World" );

    is_err = u8_error_info_is_error( &probe );
    TEST_EXPECT_EQUAL_INT( true, is_err );
    err = u8_error_info_get_error( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_PARSER_STRUCTURE, err );
    unit = u8_error_info_get_unit( &probe );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INFO_UNIT_LINE_NAME, unit );
    line = u8_error_info_get_line( &probe );
    TEST_EXPECT_EQUAL_INT( 1000, line );
    name = u8_error_info_get_name( &probe );
    TEST_EXPECT_EQUAL_STRING( "World", name );

    u8_error_info_destroy( &probe );

    return TEST_CASE_RESULT_OK;
}

double cirumvent_optimization = 0.0;

static test_case_result_t test_f64( test_fixture_t *fix )
{
    double result;

    result = u8_f64_max2( 1.0, -1.0 + cirumvent_optimization );
    TEST_EXPECT_EQUAL_DOUBLE( 1.0, result );

    result = u8_f64_max3( -1.0, 1.0, 10.0 + cirumvent_optimization );
    TEST_EXPECT_EQUAL_DOUBLE( 10.0, result );

    result = u8_f64_min2( 1.0, -1.0 + cirumvent_optimization );
    TEST_EXPECT_EQUAL_DOUBLE( -1.0, result );

    result = u8_f64_min3( -1.0, 1.0, 10.0 + cirumvent_optimization );
    TEST_EXPECT_EQUAL_DOUBLE( -1.0, result );

    return TEST_CASE_RESULT_OK;
}

int prevent_optimization = 0;

static test_case_result_t test_i32( test_fixture_t *fix )
{
    int_fast32_t result;

    result = u8_i32_max2( 1, -1 + prevent_optimization );
    TEST_EXPECT_EQUAL_DOUBLE( 1, result );

    result = u8_i32_max3( -1, 1, 10 + prevent_optimization );
    TEST_EXPECT_EQUAL_DOUBLE( 10, result );

    result = u8_i32_min2( 1, -1 + prevent_optimization );
    TEST_EXPECT_EQUAL_DOUBLE( -1, result );

    result = u8_i32_min3( -1, 1, 10 + prevent_optimization );
    TEST_EXPECT_EQUAL_DOUBLE( -1, result );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_u64( test_fixture_t *fix )
{
    u8_u64_dec_t dec;
    u8_u64_get_dec( 0xffffffffffffffff, &dec );
    TEST_EXPECT_EQUAL_STRING( "18446744073709551615", dec );

    u8_u64_hex_t hex;
    u8_u64_get_hex( 0xffffffffffffffff, &hex );
    TEST_EXPECT_EQUAL_STRING( "ffffffffffffffff", hex );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2023-2026 Andreas Warnke
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
