/* File: u8dir_file_test.c; Copyright and License: see below */

#include "u8dir_file_test.h"
#include "u8/u8_error.h"
#include "u8dir/u8dir_file.h"
#include "u8stream/universal_file_output_stream.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <sys/stat.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_file_remove( test_fixture_t *fix );
static test_case_result_t test_file_stat( test_fixture_t *fix );

static uint64_t create_a_file( u8dir_file_t path );

test_suite_t u8dir_file_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "u8dir_file_test_get_suite",
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_file_remove", &test_file_remove );
    test_suite_add_test_case( &result, "test_file_stat", &test_file_stat );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static uint64_t create_a_file( u8dir_file_t path )
{
    universal_file_output_stream_t create_file;
    universal_file_output_stream_init( &create_file );
    u8_error_t dummy_file_err = U8_ERROR_NONE;
    dummy_file_err |= universal_file_output_stream_open( &create_file, path );
    const char content[] = "123";
    dummy_file_err |= universal_file_output_stream_write( &create_file, &content, sizeof(content) );
    dummy_file_err |= universal_file_output_stream_destroy( &create_file );
    TEST_ENVIRONMENT_ASSERT( dummy_file_err == U8_ERROR_NONE );
    return( sizeof(content) );  // return the filesize
}

static test_case_result_t test_file_remove( test_fixture_t *fix )
{
    u8_error_t err;

    /* case: non_existant */
    const u8dir_file_t non_existant = "non_existant.file";
    err = u8dir_file_remove( non_existant );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_FILE_ALREADY_REMOVED, err );

    /* case: existant */
    const u8dir_file_t existant = "existant.file";
    (void) create_a_file( existant );
    err = u8dir_file_remove( existant );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* case: non_removable */
#ifdef _WIN32
    /* this part of the test case does not work on windows/wine */
#else
    const u8dir_file_t temp_dir = "local_temp.dir";
    const u8dir_file_t non_removable = "local_temp.dir/non_removable.file";
    int mode_err;
    {
        /* _WIN32 command would be: mode_err = mkdir( temp_dir ); */
        mode_err = mkdir( temp_dir, (mode_t)0777 );
        TEST_ENVIRONMENT_ASSERT( mode_err == 0 );
        (void) create_a_file( non_removable );
        mode_err = chmod( temp_dir, (mode_t)0 );
        TEST_ENVIRONMENT_ASSERT( mode_err == 0 );
    }
    err = u8dir_file_remove( non_removable );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    {
        mode_err = chmod( temp_dir, (mode_t)0777 );
        TEST_ENVIRONMENT_ASSERT( mode_err == 0 );
        err = u8dir_file_remove( non_removable );
        TEST_ENVIRONMENT_ASSERT( err == U8_ERROR_NONE );
        err = u8dir_file_remove( temp_dir );
        TEST_ENVIRONMENT_ASSERT( err == U8_ERROR_NONE );
    }
#endif

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_file_stat( test_fixture_t *fix )
{
    u8_error_t err;
    uint64_t out_value = 17;

    /* case: non_existant */
    const u8dir_file_t non_existant = "non_existant.file";

    err = u8dir_file_get_size( non_existant, &out_value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_READ, err );
    TEST_EXPECT_EQUAL_INT( 17, out_value );

    err = u8dir_file_get_modification_time( non_existant, &out_value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_READ, err );
    TEST_EXPECT_EQUAL_INT( 17, out_value );

    err = u8dir_file_get_creation_time( non_existant, &out_value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_READ, err );
    TEST_EXPECT_EQUAL_INT( 17, out_value );

    /* case: existant */
    const u8dir_file_t existant = "existant.file";
    const uint64_t f_size = create_a_file( existant );

    err = u8dir_file_get_size( existant, &out_value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( f_size, out_value );

    err = u8dir_file_get_modification_time( existant, &out_value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT( out_value > 1000000000 );  /* 1000000000 was Sep 09 2001 */

    uint64_t out_create_time = 17;
    err = u8dir_file_get_creation_time( existant, &out_create_time );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( out_value, out_create_time );

    err = u8dir_file_remove( existant );
    TEST_ENVIRONMENT_ASSERT( U8_ERROR_NONE == err );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2023-2026 Andreas Warnke
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
