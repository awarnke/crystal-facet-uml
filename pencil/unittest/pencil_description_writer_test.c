/* File: pencil_description_writer.c; Copyright and License: see below */

#include "pencil_description_writer.h"
#include "pencil_description_writer_test.h"
#include "pencil_input_data.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_write_indent_multiline_string_null(void);
static void test_write_indent_multiline_string_empty(void);
static void test_write_indent_multiline_string_empty_last(void);
static void test_write_indent_multiline_string_single(void);
static void test_write_indent_multiline_string_dual(void);
static void test_write_indent_multiline_string_crnl(void);
static void test_write_indent_multiline_string_cr(void);

TestRef pencil_description_writer_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_write_indent_multiline_string_null",test_write_indent_multiline_string_null),
        new_TestFixture("test_write_indent_multiline_string_empty",test_write_indent_multiline_string_empty),
        new_TestFixture("test_write_indent_multiline_string_empty_last",test_write_indent_multiline_string_empty_last),
        new_TestFixture("test_write_indent_multiline_string_single",test_write_indent_multiline_string_single),
        new_TestFixture("test_write_indent_multiline_string_dual",test_write_indent_multiline_string_dual),
        new_TestFixture("test_write_indent_multiline_string_crnl",test_write_indent_multiline_string_crnl),
        new_TestFixture("test_write_indent_multiline_string_cr",test_write_indent_multiline_string_cr),
    };
    EMB_UNIT_TESTCALLER(result,"pencil_description_writer_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static pencil_input_data_t my_fake_input_data;
static pencil_description_writer_t my_fake_testee;
static char my_out_buffer[24];
static FILE *my_out_stream;
static const char ENDMARKER[] = "[";
static const int ENDMARKER_LEN = 1;

static void set_up(void)
{
    pencil_input_data_init( &my_fake_input_data );
    pencil_description_writer_init( &my_fake_testee, &my_fake_input_data );

    my_out_stream = fmemopen( &my_out_buffer, sizeof( my_out_buffer ), "w");
    assert ( NULL != my_out_stream );
}

static void tear_down(void)
{
    fclose( my_out_stream );

    pencil_description_writer_destroy( &my_fake_testee );
    pencil_input_data_destroy( &my_fake_input_data );
}

static void test_write_indent_multiline_string_null(void)
{

    int err = pencil_description_writer_private_write_indent_multiline_string( &my_fake_testee, "123_", NULL, my_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    fwrite( ENDMARKER, 1 /* size of char */,ENDMARKER_LEN, my_out_stream );
    fflush( my_out_stream );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, "[", strlen("[") ) );
}

static void test_write_indent_multiline_string_empty(void)
{

    int err = pencil_description_writer_private_write_indent_multiline_string( &my_fake_testee, "123_", "", my_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    fwrite( ENDMARKER, 1 /* size of char */,ENDMARKER_LEN, my_out_stream );
    fflush( my_out_stream );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, "[", strlen("[") ) );
}

static void test_write_indent_multiline_string_empty_last(void)
{

    int err = pencil_description_writer_private_write_indent_multiline_string( &my_fake_testee, "123_", "456\n", my_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    fwrite( ENDMARKER, 1 /* size of char */,ENDMARKER_LEN, my_out_stream );
    fflush( my_out_stream );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, "123_456\n[", strlen("123_456\n[") ) );
}

static void test_write_indent_multiline_string_single(void)
{

    int err = pencil_description_writer_private_write_indent_multiline_string( &my_fake_testee, "123_", "456", my_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    fwrite( ENDMARKER, 1 /* size of char */,ENDMARKER_LEN, my_out_stream );
    fflush( my_out_stream );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, "123_456\n[", strlen("123_456\n[") ) );
}

static void test_write_indent_multiline_string_dual(void)
{

    int err = pencil_description_writer_private_write_indent_multiline_string( &my_fake_testee, "123_", "456\n789", my_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    fwrite( ENDMARKER, 1 /* size of char */,ENDMARKER_LEN, my_out_stream );
    fflush( my_out_stream );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, "123_456\n123_789\n[", strlen("123_456\n123_456\n[") ) );
}

static void test_write_indent_multiline_string_crnl(void)
{

    int err = pencil_description_writer_private_write_indent_multiline_string( &my_fake_testee, "123_", "456\r\n789\r\n", my_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    fwrite( ENDMARKER, 1 /* size of char */,ENDMARKER_LEN, my_out_stream );
    fflush( my_out_stream );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, "123_456\n123_789\n[", strlen("123_456\n123_789\n[") ) );
}

static void test_write_indent_multiline_string_cr(void)
{

    int err = pencil_description_writer_private_write_indent_multiline_string( &my_fake_testee, "123_", "456\r789\r", my_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    fwrite( ENDMARKER, 1 /* size of char */,ENDMARKER_LEN, my_out_stream );
    fflush( my_out_stream );
    /*fprintf( stdout, "check: \"%s\"\n", &my_out_buffer );*/
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, "123_456\n123_789\n[", strlen("123_456\n123_789\n[") ) );
}


/*
 * Copyright 2017-2017 Andreas Warnke
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
