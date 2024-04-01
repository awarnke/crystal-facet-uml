/* File: universal_memory_output_stream_test.c; Copyright and License: see below */

#include "universal_memory_output_stream_test.h"
#include "u8stream/universal_memory_output_stream.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_insert_regular( test_fixture_t *fix );
static test_case_result_t test_insert_border_cases( test_fixture_t *fix );
static test_case_result_t test_null_termination( test_fixture_t *fix );

test_suite_t universal_memory_output_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_memory_output_stream_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_insert_regular", &test_insert_regular );
    test_suite_add_test_case( &result, "test_insert_border_cases", &test_insert_border_cases );
    test_suite_add_test_case( &result, "test_null_termination", &test_null_termination );
    return result;
}

struct test_fixture_struct {
    char out_buffer[10];
    universal_memory_output_stream_t mem_out_stream;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    memset( &((*fix).out_buffer), '\0', sizeof((*fix).out_buffer) );
    universal_memory_output_stream_init( &((*fix).mem_out_stream),
                                         &((*fix).out_buffer),
                                         sizeof((*fix).out_buffer),
                                         UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8
                                       );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    universal_memory_output_stream_destroy( &((*fix).mem_out_stream) );
}

static test_case_result_t test_insert_regular( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_memory_output_stream_get_output_stream( &((*fix).mem_out_stream) );
    TEST_EXPECT( my_out_stream != NULL );

    /* get universal_output_stream_if_t */
    const universal_output_stream_if_t *my_out_if = universal_output_stream_get_interface ( my_out_stream );
    TEST_EXPECT( my_out_if != NULL );

    /* get objectdata */
    void *my_obj_data = universal_output_stream_get_objectdata ( my_out_stream );
    TEST_EXPECT_EQUAL_PTR( &((*fix).mem_out_stream), my_obj_data );

    /* write */
    const char test_1[] = "Hello";
    err = universal_output_stream_write ( my_out_stream, test_1, sizeof(test_1) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &((*fix).out_buffer[0]), test_1 ) );

    /* flush */
    err = universal_output_stream_flush (my_out_stream);
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* reset */
    err = universal_memory_output_stream_reset( &((*fix).mem_out_stream) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* write */
    const char test_2[] = "Hel";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* write */
    const char test_3[] = "lo!";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &((*fix).out_buffer[0]), "Hello!" ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_insert_border_cases( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_memory_output_stream_get_output_stream( &((*fix).mem_out_stream) );
    TEST_EXPECT( my_out_stream != NULL );

    /* reset */
    err = universal_memory_output_stream_reset( &((*fix).mem_out_stream) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* write */
    const char test_1[] = "123456";
    err = universal_output_stream_write ( my_out_stream, test_1, strlen(test_1) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &((*fix).out_buffer[0]), test_1 ) );

    /* write */
    const char test_2[] = "7890abc";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "1234567890", sizeof((*fix).out_buffer) ) );

    /* write */
    const char test_3[] = "lo!";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "1234567890", sizeof((*fix).out_buffer) ) );

    /* write */
    const char test_4[] = "";
    err = universal_output_stream_write ( my_out_stream, test_4, 0 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "1234567890", sizeof((*fix).out_buffer) ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_null_termination( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* write */
    const char test_1[] = "123456";
    err = universal_memory_output_stream_write ( &((*fix).mem_out_stream), test_1, strlen(test_1) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &((*fix).out_buffer[0]), test_1 ) );

    /* write null term */
    err = universal_memory_output_stream_flush( &((*fix).mem_out_stream) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), test_1, sizeof(test_1) /* incl 0-term */) );

    /* write */
    const char test_2[] = "7890abc";
    err = universal_memory_output_stream_write ( &((*fix).mem_out_stream), test_2, strlen(test_2) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "1234567890", sizeof((*fix).out_buffer) ) );

    /* write null term, overwrite end automatically by UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8 mode */
    err = universal_memory_output_stream_flush( &((*fix).mem_out_stream) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "123456789" "\0", sizeof((*fix).out_buffer) ) );

    /* reset */
    err = universal_memory_output_stream_reset( &((*fix).mem_out_stream) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* write */
    const char test_3[] = "123456\xf0\x92\x80\x80";
    err = universal_memory_output_stream_write ( &((*fix).mem_out_stream), test_3, strlen(test_3) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), test_3, sizeof((*fix).out_buffer) ) );

    /* write null term, overwrite end so that last code point is not cut */
    err = universal_memory_output_stream_flush( &((*fix).mem_out_stream) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "123456" "\0" "\x92\x80\x80", sizeof((*fix).out_buffer) ) );

    /* try to write null term on zero-size buffer */
    int no_change = -1;
    universal_memory_output_stream_t empty_stream;
    universal_memory_output_stream_init( &empty_stream, &no_change, 0, UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8 );
    err = universal_memory_output_stream_flush( &empty_stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_CONFIG_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_INT( -1, no_change );

    /* null term mode UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_BYTE */
    char ascii[4] = "123";
    universal_memory_output_stream_t ascii_stream;
    universal_memory_output_stream_init( &ascii_stream, &ascii, sizeof(ascii), UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_BYTE );
    err = universal_memory_output_stream_flush( &ascii_stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( '\0', ascii[0] );

    /* null term mode UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_NONE */
    char simple[4] = "123";
    universal_memory_output_stream_t simple_stream;
    universal_memory_output_stream_init( &simple_stream, &simple, sizeof(simple), UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_NONE );
    err = universal_memory_output_stream_flush( &simple_stream );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( '1', ascii[0] );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2020-2024 Andreas Warnke
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
