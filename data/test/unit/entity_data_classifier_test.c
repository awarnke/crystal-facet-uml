/* File: entity_data_classifier_test.c; Copyright and License: see below */

#include "entity_data_classifier_test.h"
#include "entity/data_classifier.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "utf8stringbuf/utf8string.h"
#include <string.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_initialize( test_fixture_t *test_env );
static test_case_result_t test_set_get( test_fixture_t *test_env );

test_suite_t entity_data_classifier_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "entity_data_classifier_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_initialize", &test_initialize );
    test_suite_add_test_case( &result, "test_set_get", &test_set_get );
    return result;
}

/* initialize a long string by single characters to avoid the maximum string length ‘4095’ ISO C99 compilers are required to support */
#define TOO_LONG_20 't', 'o', 'o', ' ', 'l', 'o', 'n', 'g', ' ', 't', 'e', 'x', 't', ' ', '.', '.', '.', ' ', ' ', ' ',
#define TOO_LONG_100 TOO_LONG_20 TOO_LONG_20 TOO_LONG_20 TOO_LONG_20 TOO_LONG_20
#define TOO_LONG_800 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100
static const char static_too_long[4801] = { TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 '\0' };

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
    data_classifier_t testee;

    /* sub test case 0 */
    data_classifier_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_classifier_is_valid( &testee ) );
    const char* uuid_0 = data_classifier_get_uuid_const( &testee );
    TEST_EXPECT( uuid_0 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_0 ) );
    TEST_EXPECT_EQUAL_STRING( "", data_classifier_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_classifier_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_classifier_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_x_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_y_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_BLOCK, data_classifier_get_main_type( &testee ) );

    /* sub test case 1 */
    const u8_error_t result_1
        = data_classifier_init_new( &testee,
                                    DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE,
                                    "stereotype",
                                    "name",
                                    "description",
                                    1920,
                                    1080,
                                    24
                                  );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_1 );
    TEST_EXPECT_EQUAL_INT( false, data_classifier_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_classifier_get_row_id( &testee ) );
    const char* uuid_1 = data_classifier_get_uuid_const( &testee );
    TEST_EXPECT( uuid_1 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_1 ) );
    TEST_EXPECT_EQUAL_STRING( "stereotype", data_classifier_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "name", data_classifier_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "description", data_classifier_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1920, data_classifier_get_x_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1080, data_classifier_get_y_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 24, data_classifier_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE, data_classifier_get_main_type( &testee ) );

    /* sub test case 2 */
    const u8_error_t result_2
        = data_classifier_init_new( &testee,
                                    DATA_CLASSIFIER_TYPE_DYN_FORK_NODE,
                                    (*test_env).too_long,
                                    (*test_env).too_long,
                                    (*test_env).too_long,
                                    16,
                                    9,
                                    47
                                  );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_2 );
    TEST_EXPECT_EQUAL_INT( false, data_classifier_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_ROW_VOID, data_classifier_get_row_id( &testee ) );
    const char* uuid_2 = data_classifier_get_uuid_const( &testee );
    TEST_EXPECT( uuid_2 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_2 ) );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_name_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_stereotype_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_description_const( &testee ), "too long text" ) );
    TEST_EXPECT_EQUAL_INT( 16, data_classifier_get_x_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 9, data_classifier_get_y_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 47, data_classifier_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_DYN_FORK_NODE, data_classifier_get_main_type( &testee ) );

    /* sub test case 3 */
    data_classifier_reinit_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_classifier_is_valid( &testee ) );
    const char* uuid_3 = data_classifier_get_uuid_const( &testee );
    TEST_EXPECT( uuid_3 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_3 ) );
    TEST_EXPECT_EQUAL_STRING( "", data_classifier_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_classifier_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "", data_classifier_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_x_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_y_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_BLOCK, data_classifier_get_main_type( &testee ) );

    /* sub test case 4 */
    const u8_error_t result_4
        = data_classifier_init( &testee,
                                1234,
                                DATA_CLASSIFIER_TYPE_ARTIFACT,
                                "stereotype",
                                "name",
                                "description",
                                1920,
                                1080,
                                24,
                                "1ff2be8d-c46a-4777-8017-e073a41cc680"
                              );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_4 );
    TEST_EXPECT_EQUAL_INT( true, data_classifier_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1234, data_classifier_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "1ff2be8d-c46a-4777-8017-e073a41cc680", data_classifier_get_uuid_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "stereotype", data_classifier_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "name", data_classifier_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "description", data_classifier_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1920, data_classifier_get_x_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 1080, data_classifier_get_y_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 24, data_classifier_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_ARTIFACT, data_classifier_get_main_type( &testee ) );

    /* sub test case 5 */
    const u8_error_t result_5
        = data_classifier_reinit( &testee,
                                  54,
                                  DATA_CLASSIFIER_TYPE_OBJECT,
                                  (*test_env).too_long,
                                  (*test_env).too_long,
                                  (*test_env).too_long,
                                  16,
                                  9,
                                  47,
                                  "097498ef-e43b-4b79-b26a-df6f23590165"
                                );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_5 );
    TEST_EXPECT_EQUAL_INT( true, data_classifier_is_valid( &testee ) );
    TEST_EXPECT_EQUAL_INT( 54, data_classifier_get_row_id( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "097498ef-e43b-4b79-b26a-df6f23590165", data_classifier_get_uuid_const( &testee ) );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_name_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_stereotype_const( &testee ), "too long text" ) );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_description_const( &testee ), "too long text" ) );
    TEST_EXPECT_EQUAL_INT( 16, data_classifier_get_x_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 9, data_classifier_get_y_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 47, data_classifier_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_OBJECT, data_classifier_get_main_type( &testee ) );

    /* sub test case 6 */
    data_classifier_destroy( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_classifier_is_valid( &testee ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_get( test_fixture_t *test_env )
{
    data_classifier_t testee;

    /* sub test case 0 */
    data_classifier_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_classifier_is_valid( &testee ) );

    /* sub test case 1 */
    data_classifier_trace( &testee );
    /* function call is possible, function returns */

    /* sub test case 2 */
    data_classifier_set_row_id( &testee, 478 );
    const data_row_t row_id = data_classifier_get_row_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, row_id );
    TEST_EXPECT_EQUAL_INT( true, data_classifier_is_valid( &testee ) );
    const data_id_t data_id = data_classifier_get_data_id( &testee );
    TEST_EXPECT_EQUAL_INT( 478, data_id_get_row_id( &data_id ) );
    TEST_EXPECT_EQUAL_INT( DATA_TABLE_CLASSIFIER, data_id_get_table( &data_id ) );

    /* sub test case 3 */
    const data_classifier_type_t m_type_0 = data_classifier_get_main_type( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_classifier_type_is_behavioral( m_type_0 ) );
    data_classifier_set_main_type( &testee, DATA_CLASSIFIER_TYPE_STATE );
    const data_classifier_type_t m_type_1 = data_classifier_get_main_type( &testee );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_STATE, m_type_1 );
    TEST_EXPECT_EQUAL_INT( true, data_classifier_type_is_behavioral( m_type_1 ) );

    /* sub test case 4 */
    TEST_EXPECT_EQUAL_INT( false, data_classifier_has_stereotype( &testee ) );
    const u8_error_t result_4 = data_classifier_set_stereotype( &testee, "2ch" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_4 );
    TEST_EXPECT_EQUAL_STRING( "2ch", data_classifier_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( true, data_classifier_has_stereotype( &testee ) );

    /* sub test case 5 */
    const u8_error_t result_5 = data_classifier_set_stereotype( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_5 );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_stereotype_const( &testee ), "too long text" ) );

    /* sub test case 6 */
    const u8_error_t result_6 = data_classifier_set_name( &testee, "Amplifier" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_6 );
    TEST_EXPECT_EQUAL_STRING( "Amplifier", data_classifier_get_name_const( &testee ) );

    /* sub test case 7 */
    const u8_error_t result_7 = data_classifier_set_name( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_7 );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_stereotype_const( &testee ), "too long text" ) );

    /* sub test case 8 */
    const u8_error_t result_8 = data_classifier_set_description( &testee, "The " );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_8 );
    TEST_EXPECT_EQUAL_STRING( "The ", data_classifier_get_description_const( &testee ) );

    /* sub test case 9 */
    const u8_error_t result_9 = data_classifier_append_description( &testee, "amplifier " );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_9 );
    TEST_EXPECT_EQUAL_STRING( "The amplifier ", data_classifier_get_description_const( &testee ) );

    /* sub test case 10 */
    const u8_error_t result_10 = data_classifier_append_description( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_10 );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_description_const( &testee ), "The amplifier too long" ) );

    /* sub test case 11 */
    const u8_error_t result_11 = data_classifier_set_description( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_11 );
    TEST_EXPECT( utf8string_starts_with_str( data_classifier_get_description_const( &testee ), "too long text" ) );

    /* sub test case 12 */
    data_classifier_set_x_order( &testee, 16 );
    TEST_EXPECT_EQUAL_INT( 16, data_classifier_get_x_order( &testee ) );

    /* sub test case 13 */
    data_classifier_set_y_order( &testee, 9 );
    TEST_EXPECT_EQUAL_INT( 9, data_classifier_get_y_order( &testee ) );

    /* sub test case 14 */
    data_classifier_set_list_order( &testee, 3 );
    TEST_EXPECT_EQUAL_INT( 3, data_classifier_get_list_order( &testee ) );

    /* sub test case 15 */
    u8_error_t result_15 = data_classifier_set_uuid( &testee, "wrong" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_VALUE_OUT_OF_RANGE, result_15 );

    /* sub test case 16 */
    u8_error_t result_16 = data_classifier_set_uuid( &testee, (*test_env).too_long );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_STRING_BUFFER_EXCEEDED, result_16 );

    /* sub test case 17 */
    u8_error_t result_17 = data_classifier_set_uuid( &testee, "1652f338-5011-4775-9b56-8c08caaa2663" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result_17 );
    TEST_EXPECT_EQUAL_STRING( "1652f338-5011-4775-9b56-8c08caaa2663", data_classifier_get_uuid_const( &testee ) );

    data_classifier_destroy( &testee );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2026 Andreas Warnke
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
