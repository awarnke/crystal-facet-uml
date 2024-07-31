/* File: ctrl_undo_redo_iterator_test.c; Copyright and License: see below */

#include "ctrl_undo_redo_iterator_test.h"
#include "ctrl_undo_redo_iterator.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testIterateForward( test_fixture_t *fix );
static test_case_result_t testIterateReverse( test_fixture_t *fix );
static test_case_result_t testErrors( test_fixture_t *fix );

test_suite_t ctrl_undo_redo_iterator_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "ctrl_undo_redo_iterator",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "testIterateForward", &testIterateForward );
    test_suite_add_test_case( &result, "testIterateReverse", &testIterateReverse );
    test_suite_add_test_case( &result, "testErrors", &testErrors );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t testIterateForward( test_fixture_t *fix )
{
    TEST_EXPECT_EQUAL_INT( true, true );

    return TEST_CASE_RESULT_ERR;
}

static test_case_result_t testIterateReverse( test_fixture_t *fix )
{
    return TEST_CASE_RESULT_ERR;
}

static test_case_result_t testErrors( test_fixture_t *fix )
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
