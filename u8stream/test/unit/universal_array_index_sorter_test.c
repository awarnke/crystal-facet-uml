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
static test_case_result_t test_insert_too_many( test_fixture_t *fix );

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
    test_suite_add_test_case( &result, "test_insert_too_many", &test_insert_too_many );
    return result;
}

struct test_fixture_struct {
    universal_array_index_sorter_t testee;  /* the array index sorter to be tested */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    universal_array_index_sorter_init( &(test_fixture.testee) );
    return &test_fixture;
}

static void tear_down( test_fixture_t *fix )
{
    universal_array_index_sorter_destroy( &((*fix).testee) );
}

static test_case_result_t test_insert_and_retrieve( test_fixture_t *fix )
{
    u8_error_t err;
    uint32_t count;
    uint32_t unsorted_index;

    /* init */
    universal_array_index_sorter_reinit( &((*fix).testee) );
    count = universal_array_index_sorter_get_count( &((*fix).testee) );
    TEST_EXPECT_EQUAL_INT( 0, count );

    /* insert first */
    err = universal_array_index_sorter_insert( &((*fix).testee), 17001, -17 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    count = universal_array_index_sorter_get_count( &((*fix).testee) );
    TEST_EXPECT_EQUAL_INT( 1, count );
    unsorted_index = universal_array_index_sorter_get_array_index( &((*fix).testee), 0 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 17001, unsorted_index );

    /* insert second in front */
    err = universal_array_index_sorter_insert( &((*fix).testee), 23000, -19 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    count = universal_array_index_sorter_get_count( &((*fix).testee) );
    TEST_EXPECT_EQUAL_INT( 2, count );
    unsorted_index = universal_array_index_sorter_get_array_index( &((*fix).testee), 0 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 23000, unsorted_index );

    /* insert third at end */
    err = universal_array_index_sorter_insert( &((*fix).testee), 45022, 11 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    count = universal_array_index_sorter_get_count( &((*fix).testee) );
    TEST_EXPECT_EQUAL_INT( 3, count );

    /* insert fourth in middle */
    err = universal_array_index_sorter_insert( &((*fix).testee), 99900, -18 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    count = universal_array_index_sorter_get_count( &((*fix).testee) );
    TEST_EXPECT_EQUAL_INT( 4, count );

    /* check full list */
    unsorted_index = universal_array_index_sorter_get_array_index( &((*fix).testee), 0 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 23000, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &((*fix).testee), 1 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 99900, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &((*fix).testee), 2 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 17001, unsorted_index );
    unsorted_index = universal_array_index_sorter_get_array_index( &((*fix).testee), 3 /* = sort_index */ );
    TEST_EXPECT_EQUAL_INT( 45022, unsorted_index );

    /* done */
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_insert_too_many( test_fixture_t *fix )
{
    u8_error_t err;
    uint32_t count;
    uint32_t unsorted_index;

    /* insert exact the max size */
    for ( uint_fast32_t idx = 0; idx < UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE; idx ++ )
    {
        err = universal_array_index_sorter_insert( &((*fix).testee), idx*10, UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE-idx );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
        count = universal_array_index_sorter_get_count( &((*fix).testee) );
        TEST_EXPECT_EQUAL_INT( 1+idx, count );
        unsorted_index = universal_array_index_sorter_get_array_index( &((*fix).testee), 0 /* = sort_index */ );
        TEST_EXPECT_EQUAL_INT( idx*10, unsorted_index );
    }

    /* insert one too many */
    err = universal_array_index_sorter_insert( &((*fix).testee), 99900, -18 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, err );
    count = universal_array_index_sorter_get_count( &((*fix).testee) );
    TEST_EXPECT_EQUAL_INT( UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE, count );

    /* done */
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2017-2025 Andreas Warnke
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
