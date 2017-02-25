/* File: pencil_description_writer.c; Copyright and License: see below */

#include "pencil_description_writer.h"
#include "pencil_description_writer_test.h"
#include "pencil_input_data.h"
#include <stdio.h>
#include <string.h>

static void set_up(void);
static void tear_down(void);
static void test_write_indent_multiline_string_null(void);
static void test_write_indent_multiline_string_empty(void);
static void test_write_indent_multiline_string_empty_last(void);
static void test_write_indent_multiline_string_single(void);
static void test_write_indent_multiline_string_dual(void);
static void test_write_indent_multiline_string_crnl(void);
static void test_write_indent_multiline_string_cr(void);
static void test_write_indent_multiline_string_buf_full(void);

TestRef pencil_description_writer_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_write_indent_multiline_string_empty_last",test_write_indent_multiline_string_empty_last),
    };
    EMB_UNIT_TESTCALLER(result,"pencil_description_writer_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static pencil_input_data_t my_fake_input_data;
static pencil_description_writer_t my_fake_testee;

static void set_up(void)
{
    pencil_input_data_init( &my_fake_input_data );
    pencil_description_writer_init( &my_fake_testee, &my_fake_input_data );
}

static void tear_down(void)
{
    pencil_description_writer_destroy( &my_fake_testee );
    pencil_input_data_destroy( &my_fake_input_data );
}

static void test_write_indent_multiline_string_empty_last(void)
{
    static char my_out_buffer[16];
    memset( &my_out_buffer, '[', sizeof( my_out_buffer ) );
    static FILE *my_out_stream;
    my_out_stream = fmemopen( &my_out_buffer, sizeof(my_out_buffer), "w");
    TEST_ASSERT_NOT_NULL ( my_out_stream );

    int err = pencil_description_writer_private_write_indent_multiline_string( &my_fake_testee, "123_", "456\n", my_out_stream );
    TEST_ASSERT_EQUAL_INT( 0, err );
    fflush( my_out_stream );
    TEST_ASSERT( 0 == memcmp( &my_out_buffer, "123_456\n[[[[[[[[", sizeof( my_out_buffer ) ) );

    fclose( my_out_stream );
}


    /*
     *  \brief prints a multiline string with indentation prefix
     *
     *  if the string is empty, no character is written. If the last line is not empty, an additional newline is appended.
     *  newline, return and return-newline are recognized as line breaks.
     *
     *  \param this_ pointer to own object attributes
     *  \param indent pattern, by which each line is indented; must not be NULL
     *  \param multiline_string string to write to out
     *  \param out a stream where to print the data
     *  \return -1 in case of error, 0 in case of success
     */
    /*
    int pencil_description_writer_private_write_indent_multiline_string ( pencil_description_writer_t *this_,
                                                                          const char *indent,
                                                                          const char *multiline_string,
                                                                          FILE *out
    );
    */



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
