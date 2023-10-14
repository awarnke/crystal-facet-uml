/* File: utf8codepoint_test.c; Copyright and License: see below */

#include "utf8codepoint_test.h"
#include "utf8stringbuf/utf8stringbuf.h"
#include "utf8stringbuf/utf8codepoint.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testInit1( test_fixture_t *fix );
static test_case_result_t testInit2( test_fixture_t *fix );
static test_case_result_t testIntToUtf8ToInt( test_fixture_t *fix );
static test_case_result_t testIntToUtf8( test_fixture_t *fix );
static test_case_result_t testUnicode( test_fixture_t *fix );

test_suite_t utf8codepoint_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8CodePointTest", &set_up, &tear_down );
    test_suite_add_test_case( &result, "testInit1", &testInit1 );
    test_suite_add_test_case( &result, "testInit2", &testInit2 );
    test_suite_add_test_case( &result, "testIntToUtf8ToInt", &testIntToUtf8ToInt );
    test_suite_add_test_case( &result, "testIntToUtf8", &testIntToUtf8 );
    test_suite_add_test_case( &result, "testUnicode", &testUnicode );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t testInit1( test_fixture_t *fix )
{
    /* check initialization */
    utf8codepoint_t code_point1 = utf8codepoint( 'a' );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 97, utf8codepoint_get_char( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x00e4 );
    TEST_EXPECT_EQUAL_INT( 2, utf8codepoint_get_length( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0x00e4, utf8codepoint_get_char( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x20ac );
    TEST_EXPECT_EQUAL_INT( 3, utf8codepoint_get_length( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0x20ac, utf8codepoint_get_char( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x1d11e );
    TEST_EXPECT_EQUAL_INT( 4, utf8codepoint_get_length( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0x1d11e, utf8codepoint_get_char( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( '\0' );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_char( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x7fffffff );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0x7fffffff, utf8codepoint_get_char( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x20ffff );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0x20ffff, utf8codepoint_get_char( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0xffffffff );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0xffffffff, utf8codepoint_get_char( code_point1 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point1 ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testInit2( test_fixture_t *fix )
{
    /* check init function */
    utf8codepoint_t code_point2 = utf8codepoint_init( "a", 1 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 97, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\0", 1 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xC3\xA4", 100 );
    TEST_EXPECT_EQUAL_INT( 2, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x00e4, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xC3\xA4", 2 );
    TEST_EXPECT_EQUAL_INT( 2, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x00e4, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT ( 0 != memcmp( &UTF8CODEPOINT_INVAL_CHAR, &code_point2, sizeof(utf8codepoint_t) ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xC3\xA4", 1 );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xC3\xA4", 0 );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT ( 0 == memcmp( &UTF8CODEPOINT_INVAL_CHAR, &code_point2, sizeof(utf8codepoint_t) ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( NULL, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xE2\x82\xAC", 3 );
    TEST_EXPECT_EQUAL_INT( 3, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x20ac, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xF0\x9D\x84\x9E", 4 );
    TEST_EXPECT_EQUAL_INT( 4, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x1d11e, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xF0\x9D\x84\x9E", 3 );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\x9D\x84\x9E", 3 );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xF0\x9D\x84\x00", 4 );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testIntToUtf8ToInt( test_fixture_t *fix )
{
    utf8codepoint_t result;
    char dynTestArr1[7] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check to convert a character to and back from utf8 */
    /* 0 */
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    /* 1 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x01 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x01, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x7f );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x7f, utf8codepoint_get_char(result) );

    /* 2 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x80 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 2, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x80, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x07ff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 2, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x07ff, utf8codepoint_get_char(result) );

    /* 3 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x800 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 3, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x800, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0xffff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 3, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0xffff, utf8codepoint_get_char(result) );

    /* 4 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x10000 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 4, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x10000, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x10ffff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 4, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x10ffff, utf8codepoint_get_char(result) );

    char invalid_1fffff[5] = "\xF7\xBF\xBF\xBF";
    utf8stringbuf_t invalidBuf = UTF8STRINGBUF(invalid_1fffff);

    result = utf8stringbuf_get_char_at( invalidBuf, 0 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid(result) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length(result) );
    TEST_EXPECT_EQUAL_INT( 0x1fffff, utf8codepoint_get_char(result) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testIntToUtf8( test_fixture_t *fix )
{
    utf8codepoint_t probe;
    utf8codepointseq_t expect;
    utf8codepointseq_t result;

    /* check to convert a character to utf8 */
    /* 0 */
    probe = utf8codepoint( 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(probe) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length(probe) );
    result = utf8codepoint_get_utf8( probe );
    expect = (utf8codepointseq_t){.seq={'\0', '\0', '\0', '\0'}};
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &result, &expect, sizeof(utf8codepointseq_t) ) );

    /* 1 byte code points */
    probe = utf8codepoint( 0x24 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(probe) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length(probe) );
    result = utf8codepoint_get_utf8( probe );
    expect = (utf8codepointseq_t){.seq={'\x24', '\0', '\0', '\0'}};
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &result, &expect, sizeof(utf8codepointseq_t) ) );

    /* 2 byte code points */
    probe = utf8codepoint( 0xa2 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(probe) );
    TEST_EXPECT_EQUAL_INT( 2, utf8codepoint_get_length(probe) );
    result = utf8codepoint_get_utf8( probe );
    expect = (utf8codepointseq_t){.seq={'\xc2', '\xa2', '\0', '\0'}};
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &result, &expect, sizeof(utf8codepointseq_t) ) );

    /* 3 byte code points */
    probe = utf8codepoint( 0x20ac );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(probe) );
    TEST_EXPECT_EQUAL_INT( 3, utf8codepoint_get_length(probe) );
    result = utf8codepoint_get_utf8( probe );
    expect = (utf8codepointseq_t){.seq={'\xe2', '\x82', '\xac', '\0'}};
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &result, &expect, sizeof(utf8codepointseq_t) ) );

    /* 4 byte code points */
    probe = utf8codepoint( 0x10348 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid(probe) );
    TEST_EXPECT_EQUAL_INT( 4, utf8codepoint_get_length(probe) );
    result = utf8codepoint_get_utf8( probe );
    expect = (utf8codepointseq_t){.seq={'\xf0', '\x90', '\x8d', '\x88'}};
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &result, &expect, sizeof(utf8codepointseq_t) ) );

    /* invalid */
    probe = utf8codepoint( 0x110000 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid(probe) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length(probe) );
    result = utf8codepoint_get_utf8( probe );
    expect = (utf8codepointseq_t){.seq={'\0', '\0', '\0', '\0'}};
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &result, &expect, sizeof(utf8codepointseq_t) ) );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testUnicode( test_fixture_t *fix )
{
    /* check valid code points */
    utf8codepoint_t code_point1 = utf8codepoint( '\0' );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xd7ff );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xe000 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfdcf );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfdf0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfffc );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x10000 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x1fffc );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x100000 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x10fffc );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    /* check invalid code points */
    code_point1 = utf8codepoint( 0xd800 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xdfff );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfdd0 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfdef );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfffe );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xffff );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x1fffe );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x1ffff );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x10fffe );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x10ffff );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x110000 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xffffffff );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2012-2023 Andreas Warnke
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
