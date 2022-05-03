/* File: universal_memory_output_stream_test.c; Copyright and License: see below */

#include "universal_memory_output_stream_test.h"
#include "u8stream/universal_memory_output_stream.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_insert_regular(void);
static void test_insert_border_cases(void);
static void test_null_termination(void);

static char my_out_buffer[10];
static universal_memory_output_stream_t my_mem_out_stream;

test_suite_t universal_memory_output_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_memory_output_stream_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_insert_regular", &test_insert_regular );
    test_suite_add_test_case( &result, "test_insert_border_cases", &test_insert_border_cases );
    test_suite_add_test_case( &result, "test_null_termination", &test_null_termination );
    return result;
}

static void set_up(void)
{
    memset( &my_out_buffer, '\0', sizeof(my_out_buffer) );
    universal_memory_output_stream_init( &my_mem_out_stream, &my_out_buffer, sizeof(my_out_buffer) );
}

static void tear_down(void)
{
    universal_memory_output_stream_destroy( &my_mem_out_stream );
}

static void test_insert_regular(void)
{
    int err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_memory_output_stream_get_output_stream( &my_mem_out_stream );
    TEST_ASSERT( my_out_stream != NULL );

    /* get universal_output_stream_if_t */
    const universal_output_stream_if_t *my_out_if = universal_output_stream_get_interface ( my_out_stream );
    TEST_ASSERT( my_out_if != NULL );

    /* get objectdata */
    void *my_obj_data = universal_output_stream_get_objectdata ( my_out_stream );
    TEST_ASSERT_EQUAL_PTR( &my_mem_out_stream, my_obj_data );

    /* write */
    const char test_1[] = "Hello";
    err = universal_output_stream_write ( my_out_stream, test_1, sizeof(test_1) );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), test_1 ) );

    /* flush */
    err = universal_output_stream_flush (my_out_stream);
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* reset */
    err = universal_memory_output_stream_reset( &my_mem_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* write */
    const char test_2[] = "Hel";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* write */
    const char test_3[] = "lo!";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), "Hello!" ) );
}

static void test_insert_border_cases(void)
{
    int err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_memory_output_stream_get_output_stream( &my_mem_out_stream );
    TEST_ASSERT( my_out_stream != NULL );

    /* reset */
    err = universal_memory_output_stream_reset( &my_mem_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* write */
    const char test_1[] = "123456";
    err = universal_output_stream_write ( my_out_stream, test_1, strlen(test_1) );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), test_1 ) );

    /* write */
    const char test_2[] = "7890abc";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "1234567890", sizeof(my_out_buffer) ) );

    /* write */
    const char test_3[] = "lo!";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "1234567890", sizeof(my_out_buffer) ) );

    /* write */
    const char test_4[] = "";
    err = universal_output_stream_write ( my_out_stream, test_4, 0 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "1234567890", sizeof(my_out_buffer) ) );
}

static void test_null_termination(void)
{
    int err;

    /* write */
    const char test_1[] = "123456";
    err = universal_memory_output_stream_write ( &my_mem_out_stream, test_1, strlen(test_1) );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), test_1 ) );

    /* write null term*/
    err = universal_memory_output_stream_write_0term( &my_mem_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), test_1, sizeof(test_1) ) );

    /* write */
    const char test_2[] = "7890abc";
    err = universal_memory_output_stream_write ( &my_mem_out_stream, test_2, strlen(test_2) );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "123456" "\0" "789", sizeof(my_out_buffer) ) );

    /* write null term*/
    err = universal_memory_output_stream_write_0term( &my_mem_out_stream );
    TEST_ASSERT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "123456" "\0" "78" "\0", sizeof(my_out_buffer) ) );
}


/*
 * Copyright 2020-2022 Andreas Warnke
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
