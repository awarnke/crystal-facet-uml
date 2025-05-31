/* File: io_txt_writer_test.c; Copyright and License: see below */

#include "io_txt_writer_test.h"
#include "format/io_txt_writer.h"
#include "set/data_visible_set.h"
#include "u8stream/universal_memory_output_stream.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_write_indent_multiline_string_null( test_fixture_t *fix );
static test_case_result_t test_write_indent_multiline_string_empty( test_fixture_t *fix );
static test_case_result_t test_write_indent_multiline_string_empty_last( test_fixture_t *fix );
static test_case_result_t test_write_indent_multiline_string_single( test_fixture_t *fix );
static test_case_result_t test_write_indent_multiline_string_dual( test_fixture_t *fix );
static test_case_result_t test_write_indent_multiline_string_crnl( test_fixture_t *fix );
static test_case_result_t test_write_indent_multiline_string_cr( test_fixture_t *fix );

test_suite_t io_txt_writer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "io_txt_writer_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_null", &test_write_indent_multiline_string_null );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_empty", &test_write_indent_multiline_string_empty );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_empty_last", &test_write_indent_multiline_string_empty_last );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_single", &test_write_indent_multiline_string_single );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_dual", &test_write_indent_multiline_string_dual );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_crnl", &test_write_indent_multiline_string_crnl );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_cr", &test_write_indent_multiline_string_cr );
    return result;
}

static const char ENDMARKER[] = "[";
static const int ENDMARKER_LEN = 1;

struct test_fixture_struct {
    /* data_visible_set_t fake_input_data; */
    io_txt_writer_t fake_testee;
    char out_buffer[96];
    universal_memory_output_stream_t out_stream;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    /* data_visible_set_init( &((*fix).fake_input_data) ); */

    universal_memory_output_stream_init( &((*fix).out_stream),
                                         &((*fix).out_buffer),
                                         sizeof( (*fix).out_buffer ),
                                         UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8
                                       );
    io_txt_writer_init( &((*fix).fake_testee), universal_memory_output_stream_get_output_stream( &((*fix).out_stream) ) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    io_txt_writer_destroy( &((*fix).fake_testee) );

    universal_memory_output_stream_destroy( &((*fix).out_stream) );

    /* data_visible_set_destroy( &((*fix).fake_input_data) ); */
}

static test_case_result_t test_write_indent_multiline_string_null( test_fixture_t *fix )
{
    assert( fix != NULL );

    int err = io_txt_writer_write_indent_multiline_string( &((*fix).fake_testee), "123_", NULL );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &((*fix).out_stream), ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &((*fix).out_buffer) );*/
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), "[", strlen("[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_empty( test_fixture_t *fix )
{
    assert( fix != NULL );

    int err = io_txt_writer_write_indent_multiline_string( &((*fix).fake_testee), "123_", "" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &((*fix).out_stream), ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &((*fix).out_buffer) );*/
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), "[", strlen("[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_empty_last( test_fixture_t *fix )
{
    assert( fix != NULL );

    int err = io_txt_writer_write_indent_multiline_string( &((*fix).fake_testee), "123_", "456\n" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &((*fix).out_stream), ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &((*fix).out_buffer) );*/
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), "123_456\n[", strlen("123_456\n[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_single( test_fixture_t *fix )
{
    assert( fix != NULL );

    int err = io_txt_writer_write_indent_multiline_string( &((*fix).fake_testee), "123_", "456" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &((*fix).out_stream), ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &((*fix).out_buffer) );*/
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), "123_456\n[", strlen("123_456\n[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_dual( test_fixture_t *fix )
{
    assert( fix != NULL );

    int err = io_txt_writer_write_indent_multiline_string( &((*fix).fake_testee), "123_", "456\n789" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &((*fix).out_stream), ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &((*fix).out_buffer) );*/
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), "123_456\n123_789\n[", strlen("123_456\n123_456\n[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_crnl( test_fixture_t *fix )
{
    assert( fix != NULL );

    int err = io_txt_writer_write_indent_multiline_string( &((*fix).fake_testee), "123_", "456\r\n789\r\n" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &((*fix).out_stream), ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &((*fix).out_buffer) );*/
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), "123_456\r\n123_789\r\n[", strlen("123_456\r\n123_789\r\n[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_cr( test_fixture_t *fix )
{
    assert( fix != NULL );

    static const char* LONG_LINE
        = "56789\r123456789\r123456789\r123456789\r123456789\r123456789\r123456789\r123456789\r567\r";
    int err = io_txt_writer_write_indent_multiline_string( &((*fix).fake_testee), "123_", LONG_LINE );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &((*fix).out_stream), ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &((*fix).out_buffer) );*/
    printf("%s",(char*)&((*fix).out_buffer));
    static const char* LONG_LINE_RESULT
        = "123_56789\r123456789\r123456789\r123456789\r123456789\r123456789\r123456789\r123456789\r\n123_567\r\n[";
    TEST_EXPECT( 0 == memcmp( &((*fix).out_buffer), LONG_LINE_RESULT, strlen(LONG_LINE_RESULT) ) );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2017-2025 Andreas Warnke
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
