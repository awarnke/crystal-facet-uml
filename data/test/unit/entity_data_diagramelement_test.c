/* File: entity_data_diagramelement_test.c; Copyright and License: see below */

#include "entity_data_diagramelement_test.h"
#include "entity/data_diagramelement.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include <string.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_initialize( test_fixture_t *test_env );
static test_case_result_t test_set_get( test_fixture_t *test_env );

test_suite_t entity_data_diagramelement_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "entity_data_diagramelement_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_initialize", &test_initialize );
    test_suite_add_test_case( &result, "test_set_get", &test_set_get );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_initialize( test_fixture_t *test_env )
{
    data_diagramelement_t testee;

    data_diagramelement_init_empty( &testee );
    TEST_EXPECT_EQUAL_INT( false, data_diagramelement_is_valid( &testee ) );
    const char* uuid = data_diagramelement_get_uuid_const( &testee );
    TEST_EXPECT( uuid != NULL );
    TEST_EXPECT_EQUAL_INT( 36, strlen(uuid) );

    data_diagramelement_destroy( &testee );

    return TEST_CASE_RESULT_ERR;
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_set_get( test_fixture_t *test_env )
{
    data_diagramelement_t testee;

    data_diagramelement_init_empty( &testee );


    data_diagramelement_destroy( &testee );

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