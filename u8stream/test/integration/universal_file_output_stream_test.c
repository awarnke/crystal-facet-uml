/* File: universal_file_output_stream_test.c; Copyright and License: see below */

#include "universal_file_output_stream_test.h"
#include "u8/u8_error.h"
#include "u8stream/universal_file_input_stream.h"
#include "u8stream/universal_file_output_stream.h"
#include "u8dir/u8dir_file.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <sys/stat.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_file_write_read( test_fixture_t *fix );

test_suite_t universal_file_output_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_file_output_stream_test_get_suite",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_file_write_read", &test_file_write_read );
    return result;
}

struct test_fixture_struct {
    const char * test_file_name;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture.test_file_name = "universal_file_output_stream_test.txt";
    return &test_fixture;
}

static void tear_down( test_fixture_t *fix )
{
    /* cleanup */
    u8_error_t file_err;
    file_err = u8dir_file_remove( (*fix).test_file_name );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == file_err );
}

static test_case_result_t test_file_write_read( test_fixture_t *fix )
{
    /* write a file */
    {
        universal_file_output_stream_t create_file;
        universal_file_output_stream_init( &create_file );
        u8_error_t file_err = U8_ERROR_NONE;
        file_err = universal_file_output_stream_open( &create_file, (*fix).test_file_name );
        const char content[] = "123";
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_write( &create_file, &content, sizeof(content) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_destroy( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    /* read a file */
    {
        universal_file_input_stream_t in_file;
        universal_file_input_stream_init( &in_file );
        u8_error_t file_err = U8_ERROR_NONE;
        file_err = universal_file_input_stream_open( &in_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        char content[2];
        size_t read_bytes;
        file_err = universal_file_input_stream_read( &in_file, &content, sizeof(content), &read_bytes );
        TEST_EXPECT_EQUAL_INT( sizeof(content), read_bytes );
        TEST_EXPECT_EQUAL_INT( 0, memcmp( &content, "12", sizeof(content) ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_input_stream_read( &in_file, &content, sizeof(content), &read_bytes );
        TEST_EXPECT_EQUAL_INT( sizeof(content), read_bytes );
        TEST_EXPECT_EQUAL_INT( 0, memcmp( &content, "3", sizeof(content) ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_input_stream_read( &in_file, &content, sizeof(content), &read_bytes );
        TEST_EXPECT_EQUAL_INT( 0, read_bytes );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_END_OF_STREAM, file_err );
        file_err = universal_file_input_stream_destroy( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2024 Andreas Warnke
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
