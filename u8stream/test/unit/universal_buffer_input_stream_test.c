/* File: universal_buffer_input_stream_test.c; Copyright and License: see below */

#include "universal_buffer_input_stream_test.h"
#include "u8stream/universal_buffer_input_stream.h"
#include "u8stream/universal_memory_input_stream.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_read_chunks( test_fixture_t *fix );
static test_case_result_t test_read_all( test_fixture_t *fix );
static test_case_result_t test_peek( test_fixture_t *fix );

test_suite_t universal_buffer_input_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_buffer_input_stream_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_read_chunks", &test_read_chunks );
    test_suite_add_test_case( &result, "test_read_all", &test_read_all );
    test_suite_add_test_case( &result, "test_peek", &test_peek );
    return result;
}

struct test_fixture_struct {
    char in_buffer[10];
    universal_memory_input_stream_t mem_in_stream;
    char buffer[6];
    universal_buffer_input_stream_t buf_in_stream;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    memcpy( &((*fix).in_buffer), "123456789", sizeof((*fix).in_buffer) );
    universal_memory_input_stream_init( &((*fix).mem_in_stream), &((*fix).in_buffer), sizeof((*fix).in_buffer) );
    universal_input_stream_t *mem_in_stream_ptr = universal_memory_input_stream_get_input_stream( &((*fix).mem_in_stream) );
    universal_buffer_input_stream_init( &((*fix).buf_in_stream), &((*fix).buffer), sizeof((*fix).buffer), mem_in_stream_ptr );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    universal_buffer_input_stream_destroy( &((*fix).buf_in_stream) );
    universal_memory_input_stream_destroy( &((*fix).mem_in_stream) );
}

static test_case_result_t test_read_chunks( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* get universal_input_stream_t */
    universal_input_stream_t *my_in_stream;
    my_in_stream = universal_buffer_input_stream_get_input_stream( &((*fix).buf_in_stream) );
    TEST_EXPECT( my_in_stream != NULL );

    /* get universal_input_stream_if_t */
    const universal_input_stream_if_t *my_in_if = universal_input_stream_get_interface ( my_in_stream );
    TEST_EXPECT( my_in_if != NULL );

    /* get objectdata */
    void *my_obj_data = universal_input_stream_get_objectdata ( my_in_stream );
    TEST_EXPECT_EQUAL_PTR( &((*fix).buf_in_stream), my_obj_data );

    /* read first (when buffer was empty) */
    size_t len;
    char buf5[5];
    err = universal_input_stream_read ( my_in_stream, &buf5, 1, &len );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 1, len );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &buf5, "1", 1 ) );

    /* read next 4 (which are already buffered) */
    err = universal_input_stream_read ( my_in_stream, &buf5, 4, &len );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 4, len );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &buf5, "2345", 4 ) );

    /* read last 5 (of which 1 is already in the buffer) */
    err = universal_input_stream_read ( my_in_stream, &buf5, sizeof(buf5), &len );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( sizeof(buf5), len );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &buf5, "6789", sizeof(buf5) ) );

    /* read after end */
    err = universal_input_stream_read ( my_in_stream, &buf5, sizeof(buf5), &len );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_END_OF_STREAM, err );
    TEST_EXPECT_EQUAL_INT( 0, len );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &buf5, "6789", sizeof(buf5)-1 ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_read_all( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* get universal_input_stream_t */
    universal_input_stream_t *my_in_stream;
    my_in_stream = universal_buffer_input_stream_get_input_stream( &((*fix).buf_in_stream) );
    TEST_EXPECT( my_in_stream != NULL );

    /* read first 12 */
    size_t len;
    char buf12[12];
    err = universal_input_stream_read ( my_in_stream, &buf12, sizeof(buf12), &len );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( sizeof((*fix).in_buffer), len );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &buf12, "123456789", sizeof((*fix).in_buffer) ) );

    /* reset */
    err = universal_memory_input_stream_reset ( &((*fix).mem_in_stream) );
    universal_buffer_input_stream_reset ( &((*fix).buf_in_stream) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* read first 12 */
    err = universal_input_stream_read ( my_in_stream, &buf12, sizeof(buf12), &len );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( sizeof((*fix).in_buffer), len );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &buf12, "123456789", sizeof((*fix).in_buffer) ) );

    /* read after end */
    err = universal_input_stream_read ( my_in_stream, &buf12, sizeof(buf12), &len );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_END_OF_STREAM, err );
    TEST_EXPECT_EQUAL_INT( 0, len );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_peek( test_fixture_t *fix )
{
    assert( fix != NULL );
    assert( sizeof((*fix).in_buffer) > 2 );

    /* read the first two without peek when buffer is empty */
    char cur0 = universal_buffer_input_stream_read_next( &((*fix).buf_in_stream) );
    TEST_EXPECT_EQUAL_INT( (*fix).in_buffer[0], cur0 );
    char cur1 = universal_buffer_input_stream_read_next( &((*fix).buf_in_stream) );
    TEST_EXPECT_EQUAL_INT( (*fix).in_buffer[1], cur1 );

    /* peek and read the bytes 2..9 */
    for ( int idx = 2; idx < sizeof((*fix).in_buffer); idx ++ )
    {
        char nxt = universal_buffer_input_stream_peek_next( &((*fix).buf_in_stream) );
        TEST_EXPECT_EQUAL_INT( (*fix).in_buffer[idx], nxt );
        char cur = universal_buffer_input_stream_read_next( &((*fix).buf_in_stream) );
        TEST_EXPECT_EQUAL_INT( (*fix).in_buffer[idx], cur );
    }

    /* peek+read after end */
    char last1 = universal_buffer_input_stream_peek_next( &((*fix).buf_in_stream) );
    TEST_EXPECT_EQUAL_INT( '\0', last1 );
    char last2 = universal_buffer_input_stream_read_next( &((*fix).buf_in_stream) );
    TEST_EXPECT_EQUAL_INT( '\0', last2 );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2021-2025 Andreas Warnke
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
