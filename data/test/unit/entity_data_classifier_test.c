/* File: entity_data_classifier_test.c; Copyright and License: see below */

#include "entity_data_classifier_test.h"
#include "entity/data_classifier.h"
#include "test_expect.h"
#include "test_environment_assert.h"
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

#define TOO_LONG_100 "too long text ...   too long text ...   too long text ...   too long text ...   too long text ...   "
#define TOO_LONG_800 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100 TOO_LONG_100
const char too_long[4800] = TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800 TOO_LONG_800;

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_initialize( test_fixture_t *test_env )
{
    data_classifier_t testee;

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
    TEST_EXPECT_EQUAL_INT( DATA_ROW_ID_VOID, data_classifier_get_row_id( &testee ) );
    const char* uuid_1 = data_classifier_get_uuid_const( &testee );
    TEST_EXPECT( uuid_1 != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen( uuid_1 ) );
    TEST_EXPECT_EQUAL_STRING( "stereotype", data_classifier_get_stereotype_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "name", data_classifier_get_name_const( &testee ) );
    TEST_EXPECT_EQUAL_STRING( "description", data_classifier_get_description_const( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_x_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_y_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, data_classifier_get_list_order( &testee ) );
    TEST_EXPECT_EQUAL_INT( DATA_CLASSIFIER_TYPE_BLOCK, data_classifier_get_main_type( &testee ) );


    data_classifier_destroy( &testee );

    return TEST_CASE_RESULT_ERR;
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_get( test_fixture_t *test_env )
{
    data_classifier_t testee;
    u8_error_t result;

    data_classifier_init_empty( &testee );

    result = data_classifier_set_stereotype( &testee, "stereo" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, result );
    TEST_EXPECT_EQUAL_STRING( "stereo", data_classifier_get_stereotype_const( &testee ) );

    data_classifier_destroy( &testee );

    return TEST_CASE_RESULT_ERR;
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
