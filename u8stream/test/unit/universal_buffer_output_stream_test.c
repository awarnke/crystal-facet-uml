/* File: universal_buffer_output_stream_test.c; Copyright and License: see below */

#include "universal_buffer_output_stream_test.h"
#include "u8stream/universal_buffer_output_stream.h"
#include "u8stream/universal_memory_output_stream.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_append_regular( test_fixture_t *fix );
static test_case_result_t test_append_border_cases( test_fixture_t *fix );

static char my_out_buffer[10];
static universal_memory_output_stream_t my_mem_out_stream;
static char my_buffer[6];
static universal_buffer_output_stream_t my_buf_out_stream;

test_suite_t universal_buffer_output_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_buffer_output_stream_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_append_regular", &test_append_regular );
    test_suite_add_test_case( &result, "test_append_border_cases", &test_append_border_cases );
    return result;
}

static test_fixture_t * set_up()
{
    memset( &my_out_buffer, '\0', sizeof(my_out_buffer) );
    universal_memory_output_stream_init( &my_mem_out_stream, &my_out_buffer, sizeof(my_out_buffer) );
    universal_output_stream_t *my_mem_out_stream_ptr = universal_memory_output_stream_get_output_stream( &my_mem_out_stream );
    universal_buffer_output_stream_init( &my_buf_out_stream, &my_buffer, sizeof(my_buffer), my_mem_out_stream_ptr );
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
    universal_buffer_output_stream_destroy( &my_buf_out_stream );
    universal_memory_output_stream_destroy( &my_mem_out_stream );
}

static test_case_result_t test_append_regular( test_fixture_t *fix )
{
    int err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_buffer_output_stream_get_output_stream( &my_buf_out_stream );
    TEST_EXPECT( my_out_stream != NULL );

    /* get universal_output_stream_if_t */
    const universal_output_stream_if_t *my_out_if = universal_output_stream_get_interface ( my_out_stream );
    TEST_EXPECT( my_out_if != NULL );

    /* get objectdata */
    void *my_obj_data = universal_output_stream_get_objectdata ( my_out_stream );
    TEST_EXPECT_EQUAL_PTR( &my_buf_out_stream, my_obj_data );

    /* write */
    const char test_1[] = "Hello";
    err = universal_output_stream_write ( my_out_stream, test_1, sizeof(test_1) );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_INT( '\0', my_out_buffer[0] );

    /* flush */
    err = universal_output_stream_flush (my_out_stream);
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), test_1 ) );

    /* reset */
    err = universal_memory_output_stream_reset( &my_mem_out_stream );
    TEST_EXPECT_EQUAL_INT( 0, err );

    /* write */
    const char test_2[] = "Hel";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_EXPECT_EQUAL_INT( 0, err );

    /* write */
    const char test_3[] = "lo!";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), "Hello!" ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_append_border_cases( test_fixture_t *fix )
{
    int err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_buffer_output_stream_get_output_stream( &my_buf_out_stream );
    TEST_EXPECT( my_out_stream != NULL );

    /* write */
    const char test_1[] = "1234567";
    err = universal_output_stream_write ( my_out_stream, test_1, strlen(test_1) );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), &test_1, sizeof(my_buffer) ) );

    /* write */
    const char test_2[] = "890abc";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "1234567890", sizeof(my_out_buffer) ) );

    /* write */
    const char test_3[] = "lo!!!";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "1234567890", sizeof(my_out_buffer) ) );

    /* write */
    const char test_4[] = "";
    err = universal_output_stream_write ( my_out_stream, test_4, 0 );
    TEST_EXPECT_EQUAL_INT( 0, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "1234567890", sizeof(my_out_buffer) ) );
    return TEST_CASE_RESULT_OK;
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
