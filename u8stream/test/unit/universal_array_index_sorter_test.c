/* File: universal_array_index_sorter_test.c; Copyright and License: see below */

#include "u8list/universal_array_index_sorter.h"
#include "universal_array_index_sorter_test.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <stdio.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_insert_and_retrieve( test_fixture_t *fix );

test_suite_t universal_array_index_sorter_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_array_index_sorter_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_insert_and_retrieve", &test_insert_and_retrieve );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_insert_and_retrieve( test_fixture_t *fix )
{
    int err;
    uint32_t count;
    uint32_t unsorted_index;
    universal_array_index_sorter_t testee;

    /* init */
    universal_array_index_sorter_init( &testee );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_EXPECT_EQUAL_INT( 0, count );

    /* insert first */
    err = universal_array_index_sorter_insert( &testee, 17001, -17 );
    TEST_EXPECT_EQUAL_INT( 0, err );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_EXPECT_EQUAL_INT( 1, count );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 0 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 17001, unsorted_index );

    /* insert second in front */
    err = universal_array_index_sorter_insert( &testee, 23000, -19 );
    TEST_EXPECT_EQUAL_INT( 0, err );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_EXPECT_EQUAL_INT( 2, count );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 0 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 23000, unsorted_index );

    /* insert third at end */
    err = universal_array_index_sorter_insert( &testee, 45022, 11 );
    TEST_EXPECT_EQUAL_INT( 0, err );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_EXPECT_EQUAL_INT( 3, count );

    /* insert fourth in middle */
    err = universal_array_index_sorter_insert( &testee, 99900, -18 );
    TEST_EXPECT_EQUAL_INT( 0, err );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_EXPECT_EQUAL_INT( 4, count );

    /* check full list */
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 0 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 23000, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 1 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 99900, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 2 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 17001, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 3 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 45022, unsorted_index );

    /* done */
    universal_array_index_sorter_destroy( &testee );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2017-2024 Andreas Warnke
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
