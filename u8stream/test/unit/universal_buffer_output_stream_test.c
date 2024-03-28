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

test_suite_t universal_buffer_output_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_buffer_output_stream_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_append_regular", &test_append_regular );
    test_suite_add_test_case( &result, "test_append_border_cases", &test_append_border_cases );
    return result;
}

struct test_fixture_struct {
    char out_buffer[20];
    universal_memory_output_stream_t mem_out_stream;
    char buffer[6];
    universal_buffer_output_stream_t buf_out_stream;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    memset( &((*fix).out_buffer), '\0', sizeof((*fix).out_buffer) );
    universal_memory_output_stream_init( &((*fix).mem_out_stream), &((*fix).out_buffer), sizeof((*fix).out_buffer) );
    universal_output_stream_t *mem_out_stream_ptr = universal_memory_output_stream_get_output_stream( &((*fix).mem_out_stream) );
    universal_buffer_output_stream_init( &((*fix).buf_out_stream), &((*fix).buffer), sizeof((*fix).buffer), mem_out_stream_ptr );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    universal_buffer_output_stream_destroy( &((*fix).buf_out_stream) );
    universal_memory_output_stream_destroy( &((*fix).mem_out_stream) );
}

static test_case_result_t test_append_regular( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_buffer_output_stream_get_output_stream( &((*fix).buf_out_stream) );
    TEST_EXPECT( my_out_stream != NULL );

    /* get universal_output_stream_if_t */
    const universal_output_stream_if_t *my_out_if = universal_output_stream_get_interface ( my_out_stream );
    TEST_EXPECT( my_out_if != NULL );

    /* get objectdata */
    void *my_obj_data = universal_output_stream_get_objectdata ( my_out_stream );
    TEST_EXPECT_EQUAL_PTR( &((*fix).buf_out_stream), my_obj_data );

    /* write */
    const char test_1[] = "Hello";
    err = universal_output_stream_write ( my_out_stream, test_1, sizeof(test_1) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( '\0', (*fix).out_buffer[0] );

    /* flush */
    err = universal_output_stream_flush (my_out_stream);
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &((*fix).out_buffer[0]), test_1 ) );

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

static test_case_result_t test_append_border_cases( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_buffer_output_stream_get_output_stream( &((*fix).buf_out_stream) );
    TEST_EXPECT( my_out_stream != NULL );

    /* write more than buf-size (6) --> 7 */
    const char test_1[] = "1234567";
    err = universal_output_stream_write ( my_out_stream, test_1, strlen(test_1) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), &test_1, sizeof((*fix).buffer) /*=6*/ ) );

    /* write more than current-buf remainder (5) plus buf-size (6) --> 12 */
    const char test_2[] = "890abcdefghi";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "1234567890abcdefghi", 19 ) );

    /* write more than target memory location can hold*/
    const char test_3[] = "!!!4567";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "1234567890abcdefghi!", sizeof((*fix).out_buffer) ) );

    /* write nothing after error */
    const char test_4[] = "";
    err = universal_output_stream_write ( my_out_stream, test_4, 0 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "1234567890abcdefghi!", sizeof((*fix).out_buffer) ) );
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
