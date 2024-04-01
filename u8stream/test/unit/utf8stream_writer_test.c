/* File: utf8stream_writer_test.c; Copyright and License: see below */

#include "utf8stream_writer_test.h"
#include "utf8stream/utf8stream_writer.h"
#include "u8stream/universal_memory_output_stream.h"
#include "utf8stringbuf/utf8stringview.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_write( test_fixture_t *fix );
static test_case_result_t test_buffer_and_flush( test_fixture_t *fix );

test_suite_t utf8stream_writer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "utf8stream_writer_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_write", &test_write );
    test_suite_add_test_case( &result, "test_buffer_and_flush", &test_buffer_and_flush );
    return result;
}

struct test_fixture_struct {
    char out_buffer[16];
    universal_memory_output_stream_t mem_out_stream;
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    memset( &((*fix).out_buffer), '\0', sizeof( (*fix).out_buffer) );
    universal_memory_output_stream_init( &((*fix).mem_out_stream),
                                         &((*fix).out_buffer),
                                         sizeof( (*fix).out_buffer),
                                         UNIVERSAL_MEMORY_OUTPUT_STREAM_0TERM_UTF8
                                       );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != 0 );
    universal_memory_output_stream_destroy( &((*fix).mem_out_stream) );
}

static test_case_result_t test_write( test_fixture_t *fix )
{
    assert( fix != 0 );
    u8_error_t err;

    utf8stream_writer_t test_me;
    universal_output_stream_t *sink = universal_memory_output_stream_get_output_stream( &((*fix).mem_out_stream) );
    utf8stream_writer_init( &test_me, sink );

    err = utf8stream_writer_write_str( &test_me, "t" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    err = utf8stream_writer_write_char( &test_me, 0x20AC );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    err = utf8stream_writer_write_view( &test_me, &UTF8STRINGVIEW_STR("st: ") );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    err = utf8stream_writer_write_int( &test_me, -177 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    err = utf8stream_writer_write_hex( &test_me, 0x210 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    err = utf8stream_writer_flush( &test_me );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer), "t\xE2\x82\xACst: -177210", sizeof( (*fix).out_buffer ) ) );

    err = utf8stream_writer_write_str( &test_me, "TEST: " );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );  /* this is stored to the internal buffer, memory full error comes later at flush */
    err = utf8stream_writer_write_int( &test_me, 18 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    err = utf8stream_writer_flush( &test_me );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer), "t\xE2\x82\xACst: -177210", sizeof( (*fix).out_buffer ) ) );

    err = utf8stream_writer_destroy( &test_me );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );  /* because the null termination does not fit */

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_buffer_and_flush( test_fixture_t *fix )
{
    assert( fix != 0 );
    assert( UTF8STREAM_WRITER_MAX_BUF == 64 );  /* otherwise test case needs to be adapted */
    u8_error_t err;

    utf8stream_writer_t test_me;
    universal_output_stream_t *sink = universal_memory_output_stream_get_output_stream( &((*fix).mem_out_stream) );
    utf8stream_writer_init( &test_me, sink );

    /* write nothing to empty buffer and empty memory-sink */
    err = utf8stream_writer_write_str( &test_me, "" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );

    /* write small data to empty buffer and empty memory-sink */
    const char sixteen_chars[] = "TEST56789abcdef0";
    err = utf8stream_writer_write_str( &test_me, sixteen_chars );
    err |= utf8stream_writer_write_str( &test_me, sixteen_chars );
    err |= utf8stream_writer_write_str( &test_me, sixteen_chars );
    err |= utf8stream_writer_write_str( &test_me, sixteen_chars );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( '\0', (*fix).out_buffer[0] );  /* buffer full, memory-sink empty */

    /* write small data to full buffer and empty memory-sink */
    err = utf8stream_writer_write_str( &test_me, "1" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );  /* memory-sink over-full */
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &((*fix).out_buffer), sixteen_chars, sizeof( (*fix).out_buffer ) ) );

    /* reset the underlying memory buffer */
    universal_memory_output_stream_reset( &((*fix).mem_out_stream) );

    /* write big data to buffer and empty memory-sink */
    char big[UTF8STREAM_WRITER_MAX_BUF+4];
    memset( big, 'a', sizeof( big ) );
    utf8stringview_t big_view = UTF8STRINGVIEW( (const char*) &big, sizeof( big ) );
    err = utf8stream_writer_write_view( &test_me, &big_view );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );  /* memory-sink over-full */
    TEST_EXPECT_EQUAL_INT( '1', (*fix).out_buffer[0] );
    TEST_EXPECT_EQUAL_INT( 'a', (*fix).out_buffer[1] );

    err = utf8stream_writer_destroy( &test_me );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_AT_FILE_WRITE, err );  /* because the null termination does not fit */

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2023-2024 Andreas Warnke
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
