/* File: data_uuid_test.c; Copyright and License: see below */

#include "data_uuid_test.h"
#include "data_uuid.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_format( test_fixture_t *test_env );
static test_case_result_t test_unique( test_fixture_t *test_env );

test_suite_t data_uuid_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "data_uuid_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_format", &test_format );
    test_suite_add_test_case( &result, "test_unique", &test_unique );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_format( test_fixture_t *test_env )
{
    data_uuid_t testee;
    data_uuid_init_new( &testee );

    utf8string_t uuid_1 = data_uuid_get_string( &testee );
    /*printf("%s\n",uuid_1);*/

    TEST_EXPECT_EQUAL_INT( 36, utf8string_get_length(uuid_1) );

    TEST_EXPECT_EQUAL_INT( 8, utf8string_find_next_str(uuid_1,"-",0) );
    TEST_EXPECT_EQUAL_INT( 8+1+4, utf8string_find_next_str(uuid_1,"-",8+1) );
    TEST_EXPECT_EQUAL_INT( 8+1+4+1, utf8string_find_next_str(uuid_1,"4",8+1+4) );
    TEST_EXPECT_EQUAL_INT( 8+1+4+1+4, utf8string_find_next_str(uuid_1,"-",8+1+4+1) );
    TEST_EXPECT_EQUAL_INT( 8+1+4+1+4+1+4, utf8string_find_next_str(uuid_1,"-",8+1+4+1+4+1) );
    TEST_EXPECT_EQUAL_INT( -1, utf8string_find_next_str(uuid_1,"-",8+1+4+1+4+1+4+1) );

    data_uuid_destroy( &testee );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_unique( test_fixture_t *test_env )
{
    data_uuid_t testee_1;
    data_uuid_init_new( &testee_1 );
    data_uuid_t testee_2;
    data_uuid_init_new( &testee_2 );

    utf8string_t uuid_1 = data_uuid_get_string( &testee_1 );
    /*printf("%s\n",uuid_1);*/

    TEST_EXPECT_EQUAL_INT( 1, utf8string_equals_str(uuid_1,uuid_1) );

    utf8string_t uuid_2 = data_uuid_get_string( &testee_2 );
    /*printf("%s\n",uuid_2);*/

    TEST_EXPECT_EQUAL_INT( 0, utf8string_equals_str(uuid_1,uuid_2) );

    data_uuid_destroy( &testee_1 );
    data_uuid_destroy( &testee_2 );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2021-2023 Andreas Warnke
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
