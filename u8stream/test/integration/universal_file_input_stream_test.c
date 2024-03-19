/* File: universal_file_input_stream_test.c; Copyright and License: see below */

#include "universal_file_input_stream_test.h"
#include "u8/u8_error.h"
#include "u8/u8_fault_inject.h"
#include "u8_test_cond.h"
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
static test_case_result_t test_file_read( test_fixture_t *fix );
static test_case_result_t test_wrong_mode( test_fixture_t *fix );
static test_case_result_t test_no_access( test_fixture_t *fix );
#ifndef NDEBUG
static test_case_result_t test_posix_errors_fault_injected( test_fixture_t *fix );
#endif

test_suite_t universal_file_input_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
#ifndef NDEBUG
                     "universal_file_input_stream_test_get_suite (VARIANT: DEBUG)",
#else
                     "universal_file_input_stream_test_get_suite (VARIANT: RELEASE)",
#endif
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_file_read", &test_file_read );
    test_suite_add_test_case( &result, "test_wrong_mode", &test_wrong_mode );
    test_suite_add_test_case( &result, "test_no_access", &test_no_access );
#ifndef NDEBUG
    test_suite_add_test_case( &result, "test_posix_errors_fault_injected", &test_posix_errors_fault_injected );
#endif
    return result;
}

struct test_fixture_struct {
    const char * test_file_name;
    char test_contents[4];
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture.test_file_name = "universal_file_input_stream_test.txt";
    memcpy( test_fixture.test_contents, "123", sizeof(test_fixture.test_contents) );
    /* write a file */
    {
        test_fixture_t *fix = &test_fixture;
        universal_file_output_stream_t create_file;
        u8_error_t file_err = U8_ERROR_NONE;
        universal_file_output_stream_init( &create_file );
        file_err |= universal_file_output_stream_open( &create_file, (*fix).test_file_name );
        file_err |= universal_file_output_stream_write( &create_file, &((*fix).test_contents), sizeof((*fix).test_contents) );
        file_err |= universal_file_output_stream_close( &create_file );
        file_err |= universal_file_output_stream_destroy( &create_file );
        TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == file_err );
    }
    return &test_fixture;
}

static void tear_down( test_fixture_t *fix )
{
    /* cleanup */
    u8_error_t file_err;
    file_err = u8dir_file_remove( (*fix).test_file_name );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == file_err );
}

static test_case_result_t test_file_read( test_fixture_t *fix )
{
    universal_file_input_stream_t in_file;
    u8_error_t file_err = U8_ERROR_NONE;

    /* open an existing and readable file */
    {
        universal_file_input_stream_init( &in_file );
        universal_input_stream_t *base_class = universal_file_input_stream_get_input_stream( &in_file );
        TEST_EXPECT( NULL != base_class );
        file_err = universal_file_input_stream_open( &in_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }
    /* read an existing and readable file */
    {
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
    }
    /* re-read an existing and readable file */
    {
        file_err = universal_file_input_stream_reset( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        char content2[2];
        size_t read_bytes2;
        file_err = universal_file_input_stream_read( &in_file, &content2, sizeof(content2), &read_bytes2 );
        TEST_EXPECT_EQUAL_INT( sizeof(content2), read_bytes2 );
        TEST_EXPECT_EQUAL_INT( 0, memcmp( &content2, "12", sizeof(content2) ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }
    /* close a file */
    {
        file_err = universal_file_input_stream_close( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_input_stream_destroy( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_wrong_mode( test_fixture_t *fix )
{
    universal_file_input_stream_t in_file;
    u8_error_t file_err = U8_ERROR_NONE;

    /* init a file */
    {
        universal_file_input_stream_init( &in_file );
    }
    /* read, reset and close before open */
    {
        char content[2];
        size_t read_bytes;
        file_err = universal_file_input_stream_read( &in_file, &content, sizeof(content), &read_bytes );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
        file_err = universal_file_input_stream_reset( &in_file);
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
        file_err = universal_file_input_stream_close( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
    }
    /* open twice */
    {
        file_err = universal_file_input_stream_open( &in_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_file_input_stream_open( &in_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_WRONG_STATE, file_err );
    }
    /* destroy without close */
    {
        file_err = universal_file_input_stream_destroy( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_no_access( test_fixture_t *fix )
{
    universal_file_input_stream_t in_file;
    u8_error_t file_err = U8_ERROR_NONE;

    /* open a non existing file */
    {
        universal_file_input_stream_init( &in_file );
        const u8dir_file_t non_existant = "non_existant.file";
        file_err = universal_file_input_stream_open( &in_file, non_existant );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_READ, file_err );
    }
    /* open a directory */
    {
        universal_file_input_stream_init( &in_file );
        const u8dir_file_t directory = ".";
        file_err = universal_file_input_stream_open( &in_file, directory );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        char content[4];
        size_t read_bytes;
        file_err = universal_file_input_stream_read( &in_file, &content, sizeof(content), &read_bytes );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_END_OF_STREAM, file_err );
        file_err = universal_file_input_stream_close( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }
    /* open an existing file without read-permissions */
#ifdef _WIN32
    /* this part of the test case does not work on windows/wine */
#else
    {
        int mode_err;
        mode_err = chmod( (*fix).test_file_name, (mode_t)0 );
        TEST_ENVIRONMENT_ASSERT( mode_err == 0 );

        file_err = universal_file_input_stream_open( &in_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_READ, file_err );

        mode_err = chmod( (*fix).test_file_name, (mode_t)0777 );
        TEST_ENVIRONMENT_ASSERT( mode_err == 0 );
    }
#endif
    /* destroy */
    {
        file_err = universal_file_input_stream_destroy( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    return TEST_CASE_RESULT_OK;
}

#ifndef NDEBUG
static test_case_result_t test_posix_errors_fault_injected( test_fixture_t *fix )
{
    universal_file_input_stream_t in_file;
    u8_error_t file_err = U8_ERROR_NONE;

    /* open an existing and readable file */
    {
        universal_file_input_stream_init( &in_file );
        file_err = universal_file_input_stream_open( &in_file, (*fix).test_file_name );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }

    /* reset (seek to 0) an existing and readable file */
    U8_FAULT_INJECT_SETUP( U8_TEST_COND_FSEEK );
    {
        file_err = universal_file_input_stream_reset( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_READ, file_err );
    }
    U8_FAULT_INJECT_RESET();

    /* close a file */
    U8_FAULT_INJECT_SETUP( U8_TEST_COND_FCLOSE );
    {
        file_err = universal_file_input_stream_close( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_READ, file_err );
        file_err = universal_file_input_stream_destroy( &in_file );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
    }
    U8_FAULT_INJECT_RESET();

    return TEST_CASE_RESULT_OK;
}
#endif


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
