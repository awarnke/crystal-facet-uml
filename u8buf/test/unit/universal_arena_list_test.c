/* File: universal_arena_list_test.c; Copyright and License: see below */

#include "universal_arena_list_test.h"
#include "u8arena/universal_arena_list.h"
#include "u8arena/universal_arena_list_element.h"
#include "u8arena/universal_memory_arena.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_append(void);

test_suite_t universal_arena_list_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_arena_list_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_append", &test_append );
    return result;
}

static char one_and_half_element[ sizeof(universal_arena_list_element_t) + 2*sizeof(double) + sizeof(int) ];
universal_memory_arena_t small_arena;

static void set_up(void)
{
    universal_memory_arena_init( &small_arena, &one_and_half_element, sizeof(one_and_half_element) );
}

static void tear_down(void)
{
    universal_memory_arena_destroy( &small_arena );
}

static void test_append(void)
{
    universal_arena_list_t test_me;
    universal_arena_list_init( &test_me, &small_arena );

    /* check empty iterator */
    universal_arena_list_element_t *it1 = universal_arena_list_get_begin( &test_me );
    TEST_ASSERT( NULL == it1 );

    /* get memory from arena */
    double *ele_1;
    int err1 = universal_memory_arena_get_block( &small_arena, sizeof(double), (void**)&ele_1 );
    TEST_ASSERT_EQUAL_INT( 0, err1 );
    *ele_1 = 34.5;

    /* append by getting memory from arena */
    int err2 = universal_arena_list_append( &test_me, ele_1 );
    TEST_ASSERT_EQUAL_INT( 0, err2 );

    /* get memory from arena */
    double *ele_2;
    int err3 = universal_memory_arena_get_block( &small_arena, sizeof(double), (void**)&ele_2 );
    TEST_ASSERT_EQUAL_INT( 0, err3 );
    *ele_1 = 17.25;

    /* fail to append by getting memory from arena */
    int err4 = universal_arena_list_append( &test_me, ele_2 );
    TEST_ASSERT_EQUAL_INT( -1, err4 );

    /* check non-empty iterator */
    universal_arena_list_element_t *it2 = universal_arena_list_get_begin( &test_me );
    TEST_ASSERT( NULL != it2 );
    TEST_ASSERT( ele_1 == universal_arena_list_element_get_data( it2 ) );

    /* check iterate */
    it2 = universal_arena_list_element_get_next( it2 );
    TEST_ASSERT( NULL == it2 );
    TEST_ASSERT( NULL == universal_arena_list_get_end( &test_me ) );

    universal_arena_list_destroy( &test_me );
}


/*
 * Copyright 2021-2022 Andreas Warnke
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
