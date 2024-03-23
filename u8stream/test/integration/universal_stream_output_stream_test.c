/* File: universal_stream_output_stream_test.c; Copyright and License: see below */

#include "universal_stream_output_stream_test.h"
#include "u8/u8_error.h"
#include "u8/u8_fault_inject.h"
#include "u8_test_cond.h"
#include "u8stream/universal_stream_output_stream.h"
#include "u8dir/u8dir_file.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <sys/stat.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_stream_write( test_fixture_t *fix );
#ifndef NDEBUG
static test_case_result_t test_posix_errors_fault_injected( test_fixture_t *fix );
#endif

test_suite_t universal_stream_output_stream_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
#ifndef NDEBUG
                     "universal_stream_output_stream_test_get_suite (VARIANT: DEBUG)",
#else
                     "universal_stream_output_stream_test_get_suite (VARIANT: RELEASE)",
#endif
                     TEST_CATEGORY_INTEGRATION | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_stream_write", &test_stream_write );
#ifndef NDEBUG
    test_suite_add_test_case( &result, "test_posix_errors_fault_injected", &test_posix_errors_fault_injected );
#endif
    return result;
}

struct test_fixture_struct {
    char membuf[16];
    FILE *mem_outstream;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture.mem_outstream = fmemopen( &(test_fixture.membuf), sizeof(test_fixture.membuf), "w");
    return &test_fixture;
}

static void tear_down( test_fixture_t *fix )
{
    fclose( (*fix).mem_outstream );
}

static test_case_result_t test_stream_write( test_fixture_t *fix )
{
    universal_stream_output_stream_t out_2_mem;
    u8_error_t file_err = U8_ERROR_NONE;

    /* write a file */
    {
        universal_stream_output_stream_init( &out_2_mem, (*fix).mem_outstream );
        universal_output_stream_t *base_class = universal_stream_output_stream_get_output_stream( &out_2_mem );
        TEST_EXPECT( NULL != base_class );
        const char content[] = "123";
        file_err = universal_stream_output_stream_write( &out_2_mem, &content, sizeof( content ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        file_err = universal_stream_output_stream_flush( &out_2_mem );
        TEST_EXPECT_EQUAL_INT( 0, memcmp( &content, (*fix).membuf, sizeof( content ) ) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, file_err );
        universal_stream_output_stream_destroy( &out_2_mem );
    }

    return TEST_CASE_RESULT_OK;
}

#ifndef NDEBUG
static test_case_result_t test_posix_errors_fault_injected( test_fixture_t *fix )
{
    universal_stream_output_stream_t out_2_mem;
    u8_error_t file_err = U8_ERROR_NONE;
    universal_stream_output_stream_init( &out_2_mem, (*fix).mem_outstream );

    /* write to the file */
    U8_FAULT_INJECT_SETUP( U8_TEST_COND_FWRITE );
    {
        const char content[] = "123";
        file_err = universal_stream_output_stream_write( &out_2_mem, &content, sizeof(content) );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, file_err );
    }
    U8_FAULT_INJECT_RESET();

    /* flush the file */
    U8_FAULT_INJECT_SETUP( U8_TEST_COND_FFLUSH );
    {
        file_err = universal_stream_output_stream_flush( &out_2_mem );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, file_err );
    }
    U8_FAULT_INJECT_RESET();

    universal_stream_output_stream_destroy( &out_2_mem );
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
