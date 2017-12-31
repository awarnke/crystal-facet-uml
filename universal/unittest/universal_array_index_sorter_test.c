/* File: universal_array_index_sorter_test.c; Copyright and License: see below */

#include "universal_array_index_sorter.h"
#include "universal_array_index_sorter_test.h"
#include <stdio.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_insert_and_retrieve(void);

TestRef universal_array_index_sorter_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_insert_and_retrieve",test_insert_and_retrieve),
    };
    EMB_UNIT_TESTCALLER(result,"universal_array_index_sorter_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_insert_and_retrieve(void)
{
    int err;
    uint32_t count;
    uint32_t unsorted_index;
    universal_array_index_sorter_t testee;

    /* init */
    universal_array_index_sorter_init( &testee );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_ASSERT_EQUAL_INT( 0, count );

    /* insert first */
    err = universal_array_index_sorter_insert( &testee, 17001, -17 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_ASSERT_EQUAL_INT( 1, count );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 0 /* = sort_index */ );
    TEST_ASSERT_EQUAL_INT( 17001, unsorted_index );

    /* insert second in front */
    err = universal_array_index_sorter_insert( &testee, 23000, -19 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_ASSERT_EQUAL_INT( 2, count );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 0 /* = sort_index */ );
    TEST_ASSERT_EQUAL_INT( 23000, unsorted_index );

    /* insert third at end */
    err = universal_array_index_sorter_insert( &testee, 45022, 11 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_ASSERT_EQUAL_INT( 3, count );

    /* insert fourth in middle */
    err = universal_array_index_sorter_insert( &testee, 99900, -18 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    count = universal_array_index_sorter_get_count( &testee );
    TEST_ASSERT_EQUAL_INT( 4, count );

    /* check full list */
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 0 /* = sort_index */ );
    TEST_ASSERT_EQUAL_INT( 23000, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 1 /* = sort_index */ );
    TEST_ASSERT_EQUAL_INT( 99900, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 2 /* = sort_index */ );
    TEST_ASSERT_EQUAL_INT( 17001, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &testee, 3 /* = sort_index */ );
    TEST_ASSERT_EQUAL_INT( 45022, unsorted_index );

    /* done */
    universal_array_index_sorter_destroy( &testee );
}


/*
 * Copyright 2017-2018 Andreas Warnke
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
