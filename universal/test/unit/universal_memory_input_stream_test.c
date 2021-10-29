/* File: universal_memory_input_stream_test.c; Copyright and License: see below */

#include "universal_memory_input_stream_test.h"
#include "stream/universal_memory_input_stream.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_read_chunks(void);
static void test_read_all(void);

static char my_in_buffer[10];
static universal_memory_input_stream_t my_mem_in_stream;

test_suite_t universal_memory_input_stream_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_memory_input_stream_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_read_chunks", &test_read_chunks );
    test_suite_add_test_case( &result, "test_read_all", &test_read_all );
    return result;
}

static void set_up(void)
{
    memcpy( &my_in_buffer, "123456789", sizeof(my_in_buffer) );
    universal_memory_input_stream_init( &my_mem_in_stream, &my_in_buffer, sizeof(my_in_buffer) );
}

static void tear_down(void)
{
    int err;
    universal_memory_input_stream_destroy( &my_mem_in_stream );
}

static void test_read_chunks(void)
{
    int err;

    /* get universal_input_stream_t */
    universal_input_stream_t *my_in_stream;
    my_in_stream = universal_memory_input_stream_get_input_stream( &my_mem_in_stream );
    TEST_ASSERT( my_in_stream != NULL );

    /* get universal_input_stream_if_t */
    const universal_input_stream_if_t *my_in_if = universal_input_stream_get_interface ( my_in_stream );
    TEST_ASSERT( my_in_if != NULL );

    /* get objectdata */
    void *my_obj_data = universal_input_stream_get_objectdata ( my_in_stream );
    TEST_ASSERT_EQUAL_PTR( &my_mem_in_stream, my_obj_data );

    /* read first 5 */
    size_t len;
    char buf5[5];
    err = universal_input_stream_read ( my_in_stream, &buf5, sizeof(buf5), &len );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( sizeof(buf5), len );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &buf5, "12345", sizeof(buf5) ) );

    /* read last 5 */
    err = universal_input_stream_read ( my_in_stream, &buf5, sizeof(buf5), &len );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( sizeof(buf5), len );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &buf5, "6789", sizeof(buf5) ) );

    /* read after end */
    err = universal_input_stream_read ( my_in_stream, &buf5, sizeof(buf5), &len );
    TEST_ASSERT_EQUAL_INT( -1, err );
    TEST_ASSERT_EQUAL_INT( 0, len );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &buf5, "6789", sizeof(buf5) ) );
}

static void test_read_all(void)
{
    int err;

    /* get universal_input_stream_t */
    universal_input_stream_t *my_in_stream;
    my_in_stream = universal_memory_input_stream_get_input_stream( &my_mem_in_stream );
    TEST_ASSERT( my_in_stream != NULL );

    /* read first 12 */
    size_t len;
    char buf12[12];
    err = universal_input_stream_read ( my_in_stream, &buf12, sizeof(buf12), &len );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( sizeof(my_in_buffer), len );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &buf12, "123456789", sizeof(my_in_buffer) ) );

    /* reset */
    err = universal_memory_input_stream_reset ( &my_mem_in_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* read first 12 */
    err = universal_input_stream_read ( my_in_stream, &buf12, sizeof(buf12), &len );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( sizeof(my_in_buffer), len );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &buf12, "123456789", sizeof(my_in_buffer) ) );

    /* read after end */
    err = universal_input_stream_read ( my_in_stream, &buf12, sizeof(buf12), &len );
    TEST_ASSERT_EQUAL_INT( -1, err );
    TEST_ASSERT_EQUAL_INT( 0, len );
}


/*
 * Copyright 2021-2021 Andreas Warnke
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
