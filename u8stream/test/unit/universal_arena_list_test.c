/* File: universal_arena_list_test.c; Copyright and License: see below */

#include "universal_arena_list_test.h"
#include "u8arena/universal_arena_list.h"
#include "u8arena/universal_arena_list_element.h"
#include "u8arena/universal_memory_arena.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_append( test_fixture_t *fix );

test_suite_t universal_arena_list_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_arena_list_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_append", &test_append );
    return result;
}

struct test_fixture_struct {
    char two_and_half_element[ 2*sizeof(universal_arena_list_element_t) + 3*sizeof(double) + sizeof(int) ];
    universal_memory_arena_t small_arena;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    universal_memory_arena_init( &((*fix).small_arena),
                                 &((*fix).two_and_half_element),
                                 sizeof( (*fix).two_and_half_element )
                               );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    universal_memory_arena_destroy( &((*fix).small_arena) );
}

static test_case_result_t test_append( test_fixture_t *fix )
{
    assert( fix != NULL );
    universal_arena_list_t test_me;
    universal_arena_list_init( &test_me, &((*fix).small_arena) );

    /* check empty iterator */
    universal_arena_list_element_t *it1 = universal_arena_list_get_begin( &test_me );
    TEST_EXPECT( NULL == it1 );

    /* get memory from arena */
    double *ele_1;
    u8_error_t err1 = universal_memory_arena_get_block( &((*fix).small_arena), sizeof(double), (void**)&ele_1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err1 );
    *ele_1 = 34.5;

    /* append by getting memory from arena */
    err1 = universal_arena_list_append( &test_me, ele_1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err1 );

    /* get memory from arena */
    double *ele_1b;
    u8_error_t err1b = universal_memory_arena_get_block( &((*fix).small_arena), sizeof(double), (void**)&ele_1b );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err1b );
    *ele_1b = 35.5;

    /* append by getting memory from arena */
    err1b = universal_arena_list_append( &test_me, ele_1b );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err1b );

    /* get memory from arena */
    double *ele_2;
    u8_error_t err3 = universal_memory_arena_get_block( &((*fix).small_arena), sizeof(double), (void**)&ele_2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err3 );
    *ele_2 = 17.25;

    /* fail to append by getting memory from arena */
    u8_error_t err4 = universal_arena_list_append( &test_me, ele_2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, err4 );

    /* check non-empty iterator */
    universal_arena_list_element_t *it2 = universal_arena_list_get_begin( &test_me );
    TEST_EXPECT( NULL != it2 );
    TEST_EXPECT( ele_1 == universal_arena_list_element_get_data( it2 ) );

    /* check iterate 1 */
    it2 = universal_arena_list_element_get_next( it2 );
    TEST_EXPECT( NULL != it2 );
    TEST_EXPECT( ele_1b == universal_arena_list_element_get_data( it2 ) );

    /* check iterate 2 */
    it2 = universal_arena_list_element_get_next( it2 );
    TEST_EXPECT( NULL == it2 );
    TEST_EXPECT( NULL == universal_arena_list_get_end( &test_me ) );

    universal_arena_list_destroy( &test_me );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2021-2024 Andreas Warnke
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
