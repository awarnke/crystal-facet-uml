/* File: universal_memory_arena_test.c; Copyright and License: see below */

#include "universal_memory_arena_test.h"
#include "u8arena/universal_memory_arena.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_alloc_blocks( test_fixture_t *fix );

test_suite_t universal_memory_arena_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_memory_arena_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_alloc_blocks", &test_alloc_blocks );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_alloc_blocks( test_fixture_t *fix )
{
    char thirty_three[33];

    universal_memory_arena_t test_me;
    universal_memory_arena_init( &test_me, &(thirty_three[1]), sizeof(thirty_three)-1 );

    void *block_1;
    u8_error_t err;
    err = universal_memory_arena_get_block( &test_me, 1, &block_1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT( NULL != block_1 );
    TEST_EXPECT( &(thirty_three[1]) == block_1 );

    void *block_2;
    err = universal_memory_arena_get_block( &test_me, 1, &block_2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT( NULL != block_2 );
    TEST_EXPECT( &(thirty_three[1+sizeof(int)]) == block_2 );

    universal_memory_arena_reset( &test_me );

    void *block_3;
    err = universal_memory_arena_get_block( &test_me, 33-sizeof(int), &block_3 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT( NULL != block_3 );
    TEST_EXPECT( &(thirty_three[1]) == block_3 );

    void *block_4;
    err = universal_memory_arena_get_block( &test_me, sizeof(int), &block_4 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, err );
    TEST_EXPECT( NULL == block_4 );

    universal_memory_arena_destroy( &test_me );
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
