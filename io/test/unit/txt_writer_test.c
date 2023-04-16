/* File: txt_writer_test.c; Copyright and License: see below */

#include "txt/txt_writer.h"
#include "txt_writer_test.h"
#include "set/data_visible_set.h"
#include "u8stream/universal_memory_output_stream.h"
#include "test_expect.h"
#include <string.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_write_indent_multiline_string_null( test_fixture_t *test_env );
static test_case_result_t test_write_indent_multiline_string_empty( test_fixture_t *test_env );
static test_case_result_t test_write_indent_multiline_string_empty_last( test_fixture_t *test_env );
static test_case_result_t test_write_indent_multiline_string_single( test_fixture_t *test_env );
static test_case_result_t test_write_indent_multiline_string_dual( test_fixture_t *test_env );
static test_case_result_t test_write_indent_multiline_string_crnl( test_fixture_t *test_env );
static test_case_result_t test_write_indent_multiline_string_cr( test_fixture_t *test_env );

test_suite_t txt_writer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "txt_writer_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_null", &test_write_indent_multiline_string_null );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_empty", &test_write_indent_multiline_string_empty );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_empty_last", &test_write_indent_multiline_string_empty_last );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_single", &test_write_indent_multiline_string_single );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_dual", &test_write_indent_multiline_string_dual );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_crnl", &test_write_indent_multiline_string_crnl );
    test_suite_add_test_case( &result, "test_write_indent_multiline_string_cr", &test_write_indent_multiline_string_cr );
    return result;
}

static data_visible_set_t my_fake_input_data;
static txt_writer_t my_fake_testee;
static char my_out_buffer[24];
static universal_memory_output_stream_t my_out_stream;
static const char ENDMARKER[] = "[";
static const int ENDMARKER_LEN = 1;

static test_fixture_t * set_up()
{
    data_visible_set_init( &my_fake_input_data );

    universal_memory_output_stream_init( &my_out_stream, &my_out_buffer, sizeof(my_out_buffer) );
    txt_writer_init( &my_fake_testee, universal_memory_output_stream_get_output_stream( &my_out_stream ) );
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
    txt_writer_destroy( &my_fake_testee );

    universal_memory_output_stream_destroy( &my_out_stream );

    data_visible_set_destroy( &my_fake_input_data );
}

static test_case_result_t test_write_indent_multiline_string_null( test_fixture_t *test_env )
{

    int err = txt_writer_write_indent_multiline_string( &my_fake_testee, "123_", NULL );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &my_out_stream, ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_EXPECT( 0 == memcmp( &my_out_buffer, "[", strlen("[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_empty( test_fixture_t *test_env )
{

    int err = txt_writer_write_indent_multiline_string( &my_fake_testee, "123_", "" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &my_out_stream, ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_EXPECT( 0 == memcmp( &my_out_buffer, "[", strlen("[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_empty_last( test_fixture_t *test_env )
{

    int err = txt_writer_write_indent_multiline_string( &my_fake_testee, "123_", "456\n" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &my_out_stream, ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_EXPECT( 0 == memcmp( &my_out_buffer, "123_456\n[", strlen("123_456\n[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_single( test_fixture_t *test_env )
{

    int err = txt_writer_write_indent_multiline_string( &my_fake_testee, "123_", "456" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &my_out_stream, ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_EXPECT( 0 == memcmp( &my_out_buffer, "123_456\n[", strlen("123_456\n[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_dual( test_fixture_t *test_env )
{

    int err = txt_writer_write_indent_multiline_string( &my_fake_testee, "123_", "456\n789" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &my_out_stream, ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_EXPECT( 0 == memcmp( &my_out_buffer, "123_456\n123_789\n[", strlen("123_456\n123_456\n[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_crnl( test_fixture_t *test_env )
{

    int err = txt_writer_write_indent_multiline_string( &my_fake_testee, "123_", "456\r\n789\r\n" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &my_out_stream, ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_EXPECT( 0 == memcmp( &my_out_buffer, "123_456\n123_789\n[", strlen("123_456\n123_789\n[") ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_write_indent_multiline_string_cr( test_fixture_t *test_env )
{

    int err = txt_writer_write_indent_multiline_string( &my_fake_testee, "123_", "456\r789\r" );
    TEST_EXPECT_EQUAL_INT( 0, err );
    universal_memory_output_stream_write( &my_out_stream, ENDMARKER, ENDMARKER_LEN );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_EXPECT( 0 == memcmp( &my_out_buffer, "123_456\n123_789\n[", strlen("123_456\n123_789\n[") ) );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2017-2023 Andreas Warnke
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
