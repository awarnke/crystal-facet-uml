/* File: universal_memory_arena_test.c; Copyright and License: see below */

#include "universal_memory_arena_test.h"
#include "u8arena/universal_memory_arena.h"
#include "test_expect.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_alloc_blocks(void);

test_suite_t universal_memory_arena_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_memory_arena_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_alloc_blocks", &test_alloc_blocks );
    return result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_alloc_blocks(void)
{
    char thirty_three[33];

    universal_memory_arena_t test_me;
    universal_memory_arena_init( &test_me, &(thirty_three[1]), sizeof(thirty_three)-1 );

    void *block_1;
    int err;
    err = universal_memory_arena_get_block( &test_me, 1, &block_1 );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT( NULL != block_1 );
    TEST_EXPECT( &(thirty_three[1]) == block_1 );

    void *block_2;
    err = universal_memory_arena_get_block( &test_me, 1, &block_2 );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT( NULL != block_2 );
    TEST_EXPECT( &(thirty_three[1+sizeof(int)]) == block_2 );

    universal_memory_arena_reset( &test_me );

    void *block_3;
    err = universal_memory_arena_get_block( &test_me, 33-sizeof(int), &block_3 );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT( NULL != block_3 );
    TEST_EXPECT( &(thirty_three[1]) == block_3 );

    void *block_4;
    err = universal_memory_arena_get_block( &test_me, sizeof(int), &block_4 );
    TEST_EXPECT_EQUAL_INT( -1, err );
    TEST_EXPECT( NULL == block_4 );

    universal_memory_arena_destroy( &test_me );
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
