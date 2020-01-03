/* File: utf8string_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "util/string/utf8string.h"
#include "util/string/utf8stringbuf.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void setUp(void);
static void tearDown(void);
static void testSize(void);
static void testLength(void);
static void testEquals(void);
static void testEqualsRegion(void);
static void testStartsWith(void);
static void testEndsWith(void);
static void testFindFirst(void);
static void testFindNext(void);
static void testFindLast(void);
static void testCharAt(void);
static void testCharAtLoops(void);

test_suite_t utf8string_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8StringTest", &setUp, &tearDown );
    test_suite_add_test_case( &result, "testSize", &testSize );
    test_suite_add_test_case( &result, "testLength", &testLength );
    test_suite_add_test_case( &result, "testEquals", &testEquals );
    test_suite_add_test_case( &result, "testEqualsRegion", &testEqualsRegion );
    test_suite_add_test_case( &result, "testStartsWith", &testStartsWith );
    test_suite_add_test_case( &result, "testEndsWith", &testEndsWith );
    test_suite_add_test_case( &result, "testFindFirst", &testFindFirst );
    test_suite_add_test_case( &result, "testFindNext", &testFindNext );
    test_suite_add_test_case( &result, "testFindLast", &testFindLast );
    test_suite_add_test_case( &result, "testCharAt", &testCharAt );
    test_suite_add_test_case( &result, "testCharAtLoops", &testCharAtLoops );
    return result;
}

static void setUp(void)
{
}

static void tearDown(void)
{
}

static void testSize(void)
{
    unsigned int size;

    /* check size */
    size = utf8string_get_size( NULL );
    TEST_ASSERT_EQUAL_INT( 0, size );

    size = utf8string_get_size( "" );
    TEST_ASSERT_EQUAL_INT( 1, size );

    size = utf8string_get_size( "123456789 123456789" );
    TEST_ASSERT_EQUAL_INT( 20, size );

    size = utf8string_get_size( "ab\xC2\xA2\0" "efg" );
    TEST_ASSERT_EQUAL_INT( 5, size );
}

static void testEquals(void)
{
    //  prepare
    int equal;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";

    //  test utf8string_equals_buf
    equal = utf8string_equals_buf( dynTestArr1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_buf( "Hello", dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_buf( dynTestArr2, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_buf( "", utf8stringbuf( "" ) );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_buf( NULL, utf8stringbuf( "" ) );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_buf( NULL, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );

    //  test utf8string_equals_str
    equal = utf8string_equals_str( NULL, NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_str( "", NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_str( "", "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_str( dynTestArr1, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_str( NULL, "Hello" );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_str( dynTestArr1, dynTestArr2 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
}

static void testEqualsRegion(void)
{
    //  prepare
    int equal;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello Hell Hello Hello";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8string_equals_region_buf
    equal = utf8string_equals_region_buf( dynTestArr1, 0, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_buf( dynTestArr1, -1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_buf( dynTestArr1, 1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_buf( dynTestArr1, 6, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_buf( dynTestArr1, 0, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_buf( dynTestArr2, 0, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_buf( dynTestArr2, 6, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_buf( dynTestArr2, 11, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_buf( dynTestArr2, 17, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_buf( NULL, 3, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );

    //  test utf8string_equals_region_str
    equal = utf8string_equals_region_str( dynTestArr1, 0, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_str( dynTestArr1, 0, "Hello" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_str( dynTestArr1, 5, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_str( dynTestArr1, 0, "" );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_str( dynTestArr2, 6, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_str( dynTestArr2, 17, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_region_str( NULL, 0, "Hi" );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_str( "", 0, NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_str( dynTestArr1, -1, NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_region_str( NULL, 0, NULL );
    TEST_ASSERT_EQUAL_INT( 0, equal );
}

static void testLength(void)
{
    int len;

    /* check utf8string_get_length */
    len = utf8string_get_length( NULL );
    TEST_ASSERT_EQUAL_INT( 0, len );
    len = utf8string_get_length( "" );
    TEST_ASSERT_EQUAL_INT( 0, len );
    len = utf8string_get_length( "12" );
    TEST_ASSERT_EQUAL_INT( 2, len );
    len = utf8string_get_length( "123456789 123456789" );
    TEST_ASSERT_EQUAL_INT( 19, len );
}
static void testStartsWith(void) {
    //  prepare
    int matches;
    char dynTestArr1[] = "Hello";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8string_starts_with_str
    matches = utf8string_starts_with_str( dynTestArr1, dynTestArr2 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_str( dynTestArr2, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_str( dynTestArr1, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_str( dynTestArr1, "" );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_str( dynTestArr1, NULL );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_str( NULL, NULL );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_str( NULL, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_str( "", "" );
    TEST_ASSERT_EQUAL_INT( 1, matches );

    //  test utf8string_starts_with_buf
    matches = utf8string_starts_with_buf( dynTestArr1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_buf( dynTestArr2, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_buf( dynTestArr1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_buf( dynTestArr1, utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_buf( NULL, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_buf( "", utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( 1, matches );

}

static void testEndsWith(void) {
    //  prepare
    int matches;
    char dynTestArr1[] = "World";
    utf8stringbuf_t dynTestBuf1 = utf8stringbuf(dynTestArr1);
    char dynTestArr2[] = "Hello World";
    utf8stringbuf_t dynTestBuf2 = utf8stringbuf(dynTestArr2);

    //  test utf8stringbuf_ends_with_str
    matches = utf8string_ends_with_str( dynTestArr1, dynTestArr2 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_str( dynTestArr2, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_str( dynTestArr1, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_str( dynTestArr1, "" );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_str( dynTestArr1, NULL );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_str( NULL, NULL );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_str( NULL, dynTestArr1 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_str( "", "" );
    TEST_ASSERT_EQUAL_INT( 1, matches );

    //  test utf8stringbuf_ends_with_buf
    matches = utf8string_ends_with_buf( dynTestArr1, dynTestBuf2 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_buf( dynTestArr2, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_buf( dynTestArr1, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_buf( dynTestArr1, utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_buf( NULL, dynTestBuf1 );
    TEST_ASSERT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_buf( "", utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( 1, matches );

}

static void testFindFirst(void)
{
    int pos;
    char srchArr3[] = "N/A";
    utf8stringbuf_t srchBuf3 = UTF8STRINGBUF(srchArr3);

    pos = utf8string_find_first_str( NULL, srchArr3);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_first_str( NULL, "");
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_first_str( NULL, NULL);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_first_str( srchArr3, srchArr3);
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8string_find_first_str( "", srchArr3);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_first_str( "_N/A_N/A", srchArr3);
    TEST_ASSERT_EQUAL_INT( 1, pos );

    pos = utf8string_find_first_str( "_n/a_n/a", srchArr3);
    TEST_ASSERT_EQUAL_INT( -1, pos );


    pos = utf8string_find_first_buf( NULL, srchBuf3);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_first_buf( NULL, utf8stringbuf( "" ) );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_first_buf( srchArr3, srchBuf3);
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8string_find_first_buf( "", srchBuf3);
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_first_buf( "_N/A_N/A", srchBuf3);
    TEST_ASSERT_EQUAL_INT( 1, pos );

    pos = utf8string_find_first_buf( "_n/a_n/a", srchBuf3);
    TEST_ASSERT_EQUAL_INT( -1, pos );
}

static void testFindNext(void)
{
    int pos;
    char srchArr1[10] = "aaaa";
    utf8stringbuf_t srchBuf1 = UTF8STRINGBUF(srchArr1);

    pos = utf8string_find_next_buf( "", srchBuf1, -17 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_next_buf( "", srchBuf1, 17 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_next_buf( "", srchBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_next_buf( NULL, srchBuf1, 0 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_next_buf( NULL, srchBuf1, 1 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_next_buf( "aaaaaa", srchBuf1, 1 );
    TEST_ASSERT_EQUAL_INT( 1, pos );

    pos = utf8string_find_next_buf( "abaaaaa", srchBuf1, 1 );
    TEST_ASSERT_EQUAL_INT( 2, pos );


    pos = utf8string_find_next_str( NULL, NULL, 1000 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_next_str( NULL, NULL, 0 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_next_str( "aaaaaa", srchArr1, 1 );
    TEST_ASSERT_EQUAL_INT( 1, pos );

    pos = utf8string_find_next_str( "aaaaaa", srchArr1, 3 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

}

static void testFindLast(void)
{
    int pos;
    char srchArr1[10] = "aaaab";
    utf8stringbuf_t srchBuf1 = UTF8STRINGBUF(srchArr1);
    char srchArr3[] = "N/A";
    utf8stringbuf_t srchBuf3 = UTF8STRINGBUF(srchArr3);

    pos = utf8string_find_last_buf( NULL, srchBuf3 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_last_buf( NULL, utf8stringbuf("") );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_last_buf( srchArr1, utf8stringbuf("aa") );
    TEST_ASSERT_EQUAL_INT( 2, pos );

    pos = utf8string_find_last_buf( "", srchBuf1 );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_last_buf( srchArr3, srchBuf3 );
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8string_find_last_buf( srchArr1, utf8stringbuf("bb") );
    TEST_ASSERT_EQUAL_INT( -1, pos );


    pos = utf8string_find_last_str( srchArr1, NULL );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_last_str( NULL, NULL );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8string_find_last_str( srchArr1, "" );
    TEST_ASSERT_EQUAL_INT( 5, pos );

    pos = utf8string_find_last_str( srchArr1, "aaa" );
    TEST_ASSERT_EQUAL_INT( 1, pos );
}


static void testCharAt(void) {
    utf8codepoint_t result;
    char dynTestArr1[6] = "He\xE2\x82\xAC";

    /* check utf8string_get_char_at */
    result = utf8string_get_char_at( dynTestArr1, 0 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 'H', utf8codepoint_get_char(result) );

    result = utf8string_get_char_at( dynTestArr1, 2 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 3, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x20ac, utf8codepoint_get_char(result) );

    result = utf8string_get_char_at( dynTestArr1, 3 );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    result = utf8string_get_char_at( dynTestArr1, 5 );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( 1, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    result = utf8string_get_char_at( dynTestArr1, 6 );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );

    result = utf8string_get_char_at( NULL, 0 );
    TEST_ASSERT_EQUAL_INT( 0, utf8codepoint_is_valid(result) );
    TEST_ASSERT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length(result) );
    TEST_ASSERT_EQUAL_INT( 0x0, utf8codepoint_get_char(result) );
}

static void testCharAtLoops(void) {
    utf8codepoint_t result;
    char dynTestArr1[6] = "He\xE2\x82\xAC";

    /* loop over all bytes, independant of code-points */
    unsigned int countCodePoints = 0;
    unsigned int byte_length = utf8string_get_length( dynTestArr1 );
    for ( int idx = 0; idx < byte_length; idx ++ ) {
        result = utf8string_get_char_at( dynTestArr1, idx );
        if ( utf8codepoint_is_valid(result) ) {
            countCodePoints ++;
        }
    }
    TEST_ASSERT_EQUAL_INT( 3, countCodePoints );

    /* loop over all code points */
    countCodePoints = 0;
    byte_length = utf8string_get_length( dynTestArr1 );
    for ( int idx = 0; idx < byte_length; ) {
        result = utf8string_get_char_at( dynTestArr1, idx );
        if ( utf8codepoint_is_valid(result) ) {
            countCodePoints ++;
            idx += utf8codepoint_get_length(result);
        }
        else {
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT( 3, countCodePoints );

    /* loop over buffer size */
    /* this is possibly the fastest way to loop over the buffer contents */
    countCodePoints = 0;
    unsigned int byteSize = utf8string_get_size( dynTestArr1 );
    for ( int idx = 0; idx < byteSize; ) {
        result = utf8string_get_char_at( dynTestArr1, idx );
        if ( utf8codepoint_is_valid(result) && ( utf8codepoint_get_char(result) != '\0' )) {
            countCodePoints ++;
            idx += utf8codepoint_get_length(result);
        }
        else {
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT( 3, countCodePoints );

    /* safely loop over buffer size, using a position-index and a loop-limit-counter */
    countCodePoints = 0;
    byteSize = utf8string_get_size( dynTestArr1 );
    unsigned int currentIndex = 0;
    for ( int loopCount = 0; loopCount < byteSize; loopCount ++ ) {
        result = utf8string_get_char_at( dynTestArr1, currentIndex );
        if ( utf8codepoint_is_valid(result) && ( utf8codepoint_get_char(result) != '\0' )) {
            countCodePoints ++;
            currentIndex += utf8codepoint_get_length(result);
        }
        else {
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT( 3, countCodePoints );
}


/*
 * Copyright 2012-2020 Andreas Warnke
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
