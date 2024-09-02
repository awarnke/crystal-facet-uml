/* File: data_node_set_test.c; Copyright and License: see below */

#include "data_node_set_test.h"
#include "set/data_node_set.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_small_set_add_and_remove( test_fixture_t *test_env );
static test_case_result_t test_small_set_full( test_fixture_t *test_env );
static test_case_result_t test_small_set_clear( test_fixture_t *test_env );

test_suite_t data_node_set_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_node_set_test",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_small_set_add_and_remove", &test_small_set_add_and_remove );
    test_suite_add_test_case( &result, "test_small_set_full", &test_small_set_full );
    test_suite_add_test_case( &result, "test_small_set_clear", &test_small_set_clear );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_small_set_add_and_remove( test_fixture_t *test_env )
{

    return TEST_CASE_RESULT_ERR;
}

static test_case_result_t test_small_set_full( test_fixture_t *test_env )
{

    return TEST_CASE_RESULT_ERR;
}

static test_case_result_t test_small_set_clear( test_fixture_t *test_env )
{

    return TEST_CASE_RESULT_ERR;
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
