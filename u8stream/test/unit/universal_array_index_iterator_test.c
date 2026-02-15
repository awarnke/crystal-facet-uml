/* File: universal_array_index_iterator_test.c; Copyright and License: see below */

#include "u8list/universal_array_index_iterator.h"
#include "u8list/universal_array_index_sorter.h"
#include "universal_array_index_iterator_test.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <stdio.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_iterate_on_empty_list( test_fixture_t *fix );
static test_case_result_t test_iterate_on_list( test_fixture_t *fix );

test_suite_t universal_array_index_iterator_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_array_index_iterator_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_iterate_on_list", &test_iterate_on_list );
    test_suite_add_test_case( &result, "test_iterate_on_empty_list", &test_iterate_on_empty_list );
    return result;
}

struct test_fixture_struct {
    universal_array_index_sorter_t index_list;  /* the array index sorter to be iterated */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    universal_array_index_sorter_init( &(test_fixture.index_list) );
    return &test_fixture;
}

static void tear_down( test_fixture_t *fix )
{
    universal_array_index_sorter_destroy( &((*fix).index_list) );
}

static test_case_result_t test_iterate_on_list( test_fixture_t *fix )
{
    /* prepare list */
    const u8_error_t err1 = universal_array_index_sorter_insert( &((*fix).index_list), 17001, -17 );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == err1 );
    const u8_error_t err2 = universal_array_index_sorter_insert( &((*fix).index_list), 23000, -19 );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == err2 );

    /* init iterator */
    universal_array_index_iterator_t testee;
    universal_array_index_iterator_init( &testee, &((*fix).index_list) );

    /* first iteration */
    const bool has_next_1 = universal_array_index_iterator_has_next( &testee );
    TEST_EXPECT_EQUAL_INT( true, has_next_1 );
    const uint32_t index_1 = universal_array_index_iterator_next( &testee );
    TEST_EXPECT_EQUAL_INT( 23000, index_1 );
    const bool has_next_2 = universal_array_index_iterator_has_next( &testee );
    TEST_EXPECT_EQUAL_INT( true, has_next_2 );
    const uint32_t index_2 = universal_array_index_iterator_next( &testee );
    TEST_EXPECT_EQUAL_INT( 17001, index_2 );
    const bool has_next_3 = universal_array_index_iterator_has_next( &testee );
    TEST_EXPECT_EQUAL_INT( false, has_next_3 );

    /* second iteration */
    universal_array_index_iterator_reset( &testee );
    const bool has_next_4 = universal_array_index_iterator_has_next( &testee );
    TEST_EXPECT_EQUAL_INT( true, has_next_4 );
    const uint32_t index_4 = universal_array_index_iterator_next( &testee );
    TEST_EXPECT_EQUAL_INT( 23000, index_4 );

    /* destroy iterator */
    universal_array_index_iterator_destroy( &testee );

    /* done */
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_iterate_on_empty_list( test_fixture_t *fix )
{
    /* init iterator */
    universal_array_index_iterator_t testee;
    universal_array_index_iterator_init( &testee, &((*fix).index_list) );

    /* first iteration */
    const bool has_next_1 = universal_array_index_iterator_has_next( &testee );
    TEST_EXPECT_EQUAL_INT( false, has_next_1 );
    const uint32_t index_1 = universal_array_index_iterator_next( &testee );
    (void) index_1;  /* ignore whatever result we get */
    const bool has_next_2 = universal_array_index_iterator_has_next( &testee );
    TEST_EXPECT_EQUAL_INT( false, has_next_2 );

    /* second iteration */
    universal_array_index_iterator_reset( &testee );
    const bool has_next_4 = universal_array_index_iterator_has_next( &testee );
    TEST_EXPECT_EQUAL_INT( false, has_next_4 );

    /* destroy iterator */
    universal_array_index_iterator_destroy( &testee );

    /* done */
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2017-2026 Andreas Warnke
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
