/* File: utf8codepoint_test.c; Copyright and License: see below */

#include "utf8codepoint_test.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8codepoint.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void setUp(void);
static void tearDown(void);
static void testInit1(void);
static void testInit2(void);
static void testIntToUtf8ToInt(void);
static void testUnicode(void);

test_suite_t utf8codepoint_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8CodePointTest", &setUp, &tearDown );
    test_suite_add_test_case( &result, "testInit1", &testInit1 );
    test_suite_add_test_case( &result, "testInit2", &testInit2 );
    test_suite_add_test_case( &result, "testIntToUtf8ToInt", &testIntToUtf8ToInt );
    test_suite_add_test_case( &result, "testUnicode", &testUnicode );
    return result;
}

static void setUp(void)
{
}

static void tearDown(void)
{
}

static void testInit1(void)
{
    /* check initialization */
    utf8codepoint_t code_point1 = utf8codepoint( 'a' );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 97, utf8codepoint_get_char( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x00e4 );
    TEST_ASSERT_EQUAL_INT( 2, utf8codepoint_get_length( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 0x00e4, utf8codepoint_get_char( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x20ac );
    TEST_ASSERT_EQUAL_INT( 3, utf8codepoint_get_length( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 0x20ac, utf8codepoint_get_char( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x1d11e );
    TEST_ASSERT_EQUAL_INT( 4, utf8codepoint_get_length( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 0x1d11e, utf8codepoint_get_char( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( '\0' );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_get_char( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x7fffffff );
    TEST_ASSERT_EQUAL_INT( 6, utf8codepoint_get_length( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 0x7fffffff, utf8codepoint_get_char( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0x20ffff );
    TEST_ASSERT_EQUAL_INT( 5, utf8codepoint_get_length( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 0x20ffff, utf8codepoint_get_char( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point1 ) );

    code_point1 = utf8codepoint( 0xffffffff );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_get_length( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 0xffffffff, utf8codepoint_get_char( code_point1 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point1 ) );
}

static void testInit2(void)
{
    /* check init function */
    utf8codepoint_t code_point2 = utf8codepoint_init( "a", 1 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 97, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\0", 1 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xC3\xA4", 100 );
    TEST_ASSERT_EQUAL_INT( 2, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x00e4, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xC3\xA4", 2 );
    TEST_ASSERT_EQUAL_INT( 2, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x00e4, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT ( 0 != memcmp( &UTF8CODEPOINT_INVAL_CHAR, &code_point2, sizeof(utf8codepoint_t) ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xC3\xA4", 1 );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xC3\xA4", 0 );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT ( 0 == memcmp( &UTF8CODEPOINT_INVAL_CHAR, &code_point2, sizeof(utf8codepoint_t) ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( NULL, 2 );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xE2\x82\xAC", 3 );
    TEST_ASSERT_EQUAL_INT( 3, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x20ac, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xF0\x9D\x84\x9E", 4 );
    TEST_ASSERT_EQUAL_INT( 4, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x1d11e, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xF0\x9D\x84\x9E", 3 );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\x9D\x84\x9E", 3 );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

    code_point2 = utf8codepoint_init( "\xF0\x9D\x84\x00", 4 );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char( code_point2 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid( code_point2 ) );

}

static void testIntToUtf8ToInt(void) {
    utf8codepoint_t result;
    char dynTestArr1[7] = "";
    utf8stringbuf_t dynTestBuf1 = UTF8STRINGBUF(dynTestArr1);

    /* check to convert a character to and back from utf8 */
    /* 0 */
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    /* 1 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x01 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x01, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x7f );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x7f, utf8codepoint_get_char(result) );

    /* 2 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x80 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 2, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x80, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x07ff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 2, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x07ff, utf8codepoint_get_char(result) );

    /* 3 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x800 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 3, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x800, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0xffff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 3, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0xffff, utf8codepoint_get_char(result) );

    /* 4 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x10000 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 4, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x10000, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x10ffff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 4, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x10ffff, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x1fffff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 4, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x1fffff, utf8codepoint_get_char(result) );

    /* 5 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x200000 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 5, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x200000, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x3ffffff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 5, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x3ffffff, utf8codepoint_get_char(result) );

    /* 6 byte code points */
    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x4000000 );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 6, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x4000000, utf8codepoint_get_char(result) );

    utf8stringbuf_clear( dynTestBuf1 );
    utf8stringbuf_append_char( dynTestBuf1, 0x7fffffff );
    result = utf8stringbuf_get_char_at( dynTestBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 6, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x7fffffff, utf8codepoint_get_char(result) );
}

static void testUnicode(void)
{
    /* check valid code points */
    utf8codepoint_t code_point1 = utf8codepoint( '\0' );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xd7ff );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xe000 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfdcf );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfdf0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfffc );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x10000 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x1fffc );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x100000 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x10fffc );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_unicode( code_point1 ) );

    /* check invalid code points */
    code_point1 = utf8codepoint( 0xd800 );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xdfff );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfdd0 );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfdef );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xfffe );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xffff );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x1fffe );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x1ffff );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x10fffe );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x10ffff );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0x110000 );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

    code_point1 = utf8codepoint( 0xffffffff );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_unicode( code_point1 ) );

}


/*
 * Copyright 2012-2019 Andreas Warnke
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