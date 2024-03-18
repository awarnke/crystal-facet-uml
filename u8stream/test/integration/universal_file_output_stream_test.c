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
static test_case_result_t test_file_write( test_fixture_t *fix );
static test_case_result_t test_wrong_mode( test_fixture_t *fix );
static test_case_result_t test_no_access( test_fixture_t *fix );

test_suite_t universal_file_output_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_file_output_stream_test_get_suite",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_file_write", &test_file_write );
    test_suite_add_test_case( &result, "test_wrong_mode", &test_wrong_mode );
    test_suite_add_test_case( &result, "test_no_access", &test_no_access );
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

static test_case_result_t test_file_write( test_fixture_t *fix )
{
    universal_file_output_stream_t create_file;
    u8_error_t file_err = U8_ERROR_NONE;

    /* write a file */
    {
        universal_file_output_stream_init( &create_file );
        universal_output_stream_t *base_class = universal_file_output_stream_get_output_stream( &create_file );
        TEST_EXPECT( NULL != base_class );
        file_err = universal_file_output_stream_open( &create_file, (*fix).test_file_name );
        const char content[] = "123";
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_write( &create_file, &content, sizeof(content) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_flush( &create_file);
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_close( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_destroy( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    /* read and check the file */
    {
        universal_file_input_stream_t in_file;
        universal_file_input_stream_init( &in_file );
        u8_error_t file_err = U8_ERROR_NONE;
        file_err = universal_file_input_stream_open( &in_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        char content[8];
        size_t read_bytes;
        file_err = universal_file_input_stream_read( &in_file, &content, sizeof(content), &read_bytes );
        TEST_EXPECT_EQUAL_INT( 4, read_bytes );
        TEST_EXPECT_EQUAL_INT( 0, memcmp( &content, "123", sizeof("123") ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_input_stream_destroy( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_wrong_mode( test_fixture_t *fix )
{
    universal_file_output_stream_t create_file;
    u8_error_t file_err = U8_ERROR_NONE;

    /* init a file */
    {
        universal_file_output_stream_init( &create_file );
    }
    /* write, flush and close before open */
    {
        const char content[] = "123";
        file_err = universal_file_output_stream_write( &create_file, &content, sizeof(content) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
        file_err = universal_file_output_stream_flush( &create_file);
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
        file_err = universal_file_output_stream_close( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
    }
    /* open twice */
    {
        file_err = universal_file_output_stream_open( &create_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_open( &create_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
    }
    /* destroy without close */
    {
        file_err = universal_file_output_stream_destroy( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_no_access( test_fixture_t *fix )
{
    universal_file_output_stream_t create_file;
    u8_error_t file_err = U8_ERROR_NONE;

    /* write a file */
    {
        universal_file_output_stream_init( &create_file );
        u8_error_t file_err = U8_ERROR_NONE;
        file_err = universal_file_output_stream_open( &create_file, (*fix).test_file_name );
        const char content[] = "123";
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_write( &create_file, &content, sizeof(content) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_close( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_destroy( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }
    /* open the already existing file */
    {
        universal_file_output_stream_init( &create_file );
        file_err = universal_file_output_stream_open( &create_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        const char new_content[] = "ab";
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_write( &create_file, &new_content, sizeof(new_content) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_close( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_output_stream_destroy( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }
    /* open a directory */
    {
        universal_file_output_stream_init( &create_file );
        const u8dir_file_t directory = ".";
        file_err = universal_file_output_stream_open( &create_file, directory );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, file_err );
        char bad_content[] = "oh no";
        file_err = universal_file_output_stream_write( &create_file, &bad_content, sizeof(bad_content) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
        file_err = universal_file_output_stream_close( &create_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
    }
    /* open an existing file without write-permissions */
#ifdef _WIN32
    /* this part of the test case does not work on windows/wine */
#else
    {
        int mode_err;
        mode_err = chmod( (*fix).test_file_name, (mode_t)0 );
        TEST_ENVIRONMENT_ASSERT( mode_err == 0 );

        universal_file_output_stream_init( &create_file );
        file_err = universal_file_output_stream_open( &create_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, file_err );

        mode_err = chmod( (*fix).test_file_name, (mode_t)0777 );
        TEST_ENVIRONMENT_ASSERT( mode_err == 0 );
    }
#endif
    /* destroy */
    {
        file_err = universal_file_output_stream_destroy( &create_file );
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
