/* File: entity_data_head_test.c; Copyright and License: see below */

#include "entity_data_head_test.h"
#include "entity/data_head.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include <string.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_initialize( test_fixture_t *test_env );
static test_case_result_t test_set_get( test_fixture_t *test_env );

test_suite_t entity_data_head_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "entity_data_head_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_initialize", &test_initialize );
    test_suite_add_test_case( &result, "test_set_get", &test_set_get );
    return result;
}

#define TOO_LONG_100 "too long text ...   too long text ...   too long text ...   too long text ...   too long text ...   "
#define TOO_LONG_800 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100
static const char static_too_long[1601] = TOO_LONG_800 TOO_LONG_800;

struct test_fixture_struct {
    const char *too_long;  /*!< a pointer to a string that is too long */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture.too_long = &(static_too_long[0]);
    return &test_fixture;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_initialize( test_fixture_t *test_env )
{
    data_head_t testee;

    /* sub test case 0 */
    /* n/a */

    /* sub test case 1 */
    const u8_error_t result_1
        = data_head_init_new( &testee, "key", "value" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_1 );
    TEST_EXPECT_EQUAL_INT( false, data_head_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_head_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "key", data_head_get_key_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "value", data_head_get_value_const( &testee ) );

    /* sub test case 2 */
    const u8_error_t result_2
        = data_head_init_new( &testee, (*test_env).too_long, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_2 );
    TEST_EXPECT_EQUAL_INT( false, data_head_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_head_get_row_id( &testee ) );
    TEST_EXPECT( utf8string_starts_with_str( data_head_get_key_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_head_get_value_const( &testee ), "too long text" ) );

    /* sub test case 3 */
    const u8_error_t result_3
        = data_head_init( &testee, 256, "key", "value" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_3 );
    TEST_EXPECT_EQUAL_INT( true, data_head_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 256, data_head_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "key", data_head_get_key_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "value", data_head_get_value_const( &testee ) );

    /* sub test case 4 */
    /* n/a */

    /* sub test case 5 */
    const u8_error_t result_5
        = data_head_init( &testee, 254,(*test_env).too_long, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_5 );
    TEST_EXPECT_EQUAL_INT( true, data_head_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 254, data_head_get_row_id( &testee ) );
    TEST_EXPECT( utf8string_starts_with_str( data_head_get_key_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_head_get_value_const( &testee ), "too long text" ) );

    /* sub test case 6 */
    data_head_destroy( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_head_is_valid( &testee ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_get( test_fixture_t *test_env )
{
    data_head_t testee;
    data_head_t testee_copy;

    /* sub test case 0 */
    data_head_init_new( &testee, "", "" );
    TEST_EXPECT_EQUAL_INT( false, data_head_is_valid( &testee ) );

    /* sub test case 1 */
    data_head_trace( &testee );
    /* function call is possible, function returns */

    /* sub test case 2 */
    data_head_set_row_id( &testee, 478 );
    const data_row_id_t row_id = data_head_get_row_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, row_id );
    TEST_EXPECT_EQUAL_INT( true, data_head_is_valid( &testee ) );

    /* sub test case 3 */
    /* n/a */

    /* sub test case 4 */
    const u8_error_t result_4 = data_head_set_key( &testee, "2ch" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_4 );
    TEST_EXPECT_EQUAL_STRING( "2ch", data_head_get_key_const( &testee ) );

    /* sub test case 5, work on copy, do not modify original */
    data_head_copy( &testee_copy, &testee );
    const u8_error_t result_5 = data_head_set_key( &testee_copy, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_5 );
    TEST_EXPECT( utf8string_starts_with_str( data_head_get_key_const( &testee_copy ), "too long text" ) );
    TEST_EXPECT_EQUAL_STRING( "2ch", data_head_get_key_const( &testee ) );

    /* sub test case 6, work on copy, do not modify original */
    data_head_replace( &testee_copy, &testee );
    const u8_error_t result_6 = data_head_set_value( &testee_copy, "Amplifier" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_6 );
    TEST_EXPECT_EQUAL_STRING( "Amplifier", data_head_get_value_const( &testee_copy ) );
    TEST_EXPECT_EQUAL_STRING( "", data_head_get_value_const( &testee ) );

    /* sub test case 7 */
    const u8_error_t result_7 = data_head_set_value( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_7 );
    TEST_EXPECT( utf8string_starts_with_str( data_head_get_value_const( &testee ), "too long text" ) );

    /* sub test case 8 */
    /* n/a */

    /* sub test case 9 */
    /* n/a */

    /* sub test case 10 */
    /* n/a */

    /* sub test case 11 */
    /* n/a */

    /* sub test case 12 */
    /* n/a */

    /* sub test case 13 */
    /* n/a */

    /* sub test case 14 */
    /* n/a */

    /* sub test case 15 */
    /* n/a */

    /* sub test case 16 */
    /* n/a */

    /* sub test case 17 */
    /* n/a */

    data_head_destroy( &testee );
    data_head_destroy( &testee_copy );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2024 Andreas Warnke
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
