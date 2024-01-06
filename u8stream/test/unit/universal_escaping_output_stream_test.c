/* File: universal_escaping_output_stream_test.c; Copyright and License: see below */

#include "universal_escaping_output_stream_test.h"
#include "u8stream/universal_escaping_output_stream.h"
#include "u8stream/universal_memory_output_stream.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_write_regular( test_fixture_t *fix );
static test_case_result_t test_write_border_cases( test_fixture_t *fix );

test_suite_t universal_escaping_output_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_escaping_output_stream_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_write_regular", &test_write_regular );
    test_suite_add_test_case( &result, "test_write_border_cases", &test_write_border_cases );
    return result;
}

const char *const ((my_patterns_and_replacements)[][2]) = {{"&","&amp;"},{"--","- - "},{"\n","  \n"},{NULL,NULL}};

struct test_fixture_struct {
    char out_buffer[16];
    universal_memory_output_stream_t mem_out_stream;
    universal_escaping_output_stream_t esc_out_stream;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    memset( &((*fix).out_buffer), '\0', sizeof((*fix).out_buffer) );
    universal_memory_output_stream_init( &((*fix).mem_out_stream), &((*fix).out_buffer), sizeof((*fix).out_buffer) );
    universal_escaping_output_stream_init( &((*fix).esc_out_stream),
                                           &my_patterns_and_replacements,
                                           universal_memory_output_stream_get_output_stream( &((*fix).mem_out_stream) )
                                         );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;
    err = universal_escaping_output_stream_destroy( &((*fix).esc_out_stream) );
    universal_memory_output_stream_destroy( &((*fix).mem_out_stream) );
    TEST_ENVIRONMENT_ASSERT( err == 0 );
}

static test_case_result_t test_write_regular( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_escaping_output_stream_get_output_stream( &((*fix).esc_out_stream) );
    TEST_EXPECT( my_out_stream != NULL );

    /* write */
    const char test_1[] = "&";
    err = universal_output_stream_write ( my_out_stream, test_1, strlen(test_1) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &((*fix).out_buffer[0]), "&amp;" ) );

    /* flush */
    err = universal_output_stream_flush (my_out_stream);
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* write */
    const char test_2[] = "---4\n";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &((*fix).out_buffer[0]), "&amp;- - -4  \n" ) );

    /* write */
    const char test_3[] = "";
    err = universal_output_stream_write ( my_out_stream, test_3, strlen(test_3) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, strcmp( &((*fix).out_buffer[0]), "&amp;- - -4  \n" ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_border_cases( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_escaping_output_stream_get_output_stream( &((*fix).esc_out_stream) );
    TEST_EXPECT( my_out_stream != NULL );

    /* write */
    const char test_1[] = "0123456789abcdef";
    err = universal_output_stream_write ( my_out_stream, test_1, sizeof(test_1) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), test_1, sizeof((*fix).out_buffer) ) );

    /* reset underlying memory output stream */
    err = universal_memory_output_stream_reset( &((*fix).mem_out_stream) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* write */
    const char test_2[] = "&&-----";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "&amp;&amp;- - - ", sizeof((*fix).out_buffer) ) );

    /* write */
    const char test_3[] = "\n";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer[0]), "&amp;&amp;- - - ", sizeof((*fix).out_buffer) ) );
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
