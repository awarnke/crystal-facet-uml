/* File: utf8string_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "utf8stringbuf/utf8string.h"
#include "u8_test_cond.h"
#include "u8/u8_fault_inject.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>
#include <limits.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testSize( test_fixture_t *fix );
static test_case_result_t testLength( test_fixture_t *fix );
static test_case_result_t testEquals( test_fixture_t *fix );
static test_case_result_t testStartsWith( test_fixture_t *fix );
static test_case_result_t testEndsWith( test_fixture_t *fix );
static test_case_result_t testContains( test_fixture_t *fix );
static test_case_result_t testCharAt( test_fixture_t *fix );
static test_case_result_t testCharAtLoops( test_fixture_t *fix );
static test_case_result_t testParseInt( test_fixture_t *fix );
static test_case_result_t testParseFloat( test_fixture_t *fix );
#ifndef NDEBUG
static test_case_result_t testParseIntWithInjectedFault( test_fixture_t *fix );
static test_case_result_t testParseFloatWithInjectedFault( test_fixture_t *fix );
#endif

test_suite_t utf8string_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
#ifndef NDEBUG
                     "utf8StringTest (VARIANT: DEBUG)",
#else
                     "utf8StringTest (VARIANT: RELEASE)",
#endif
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "testSize", &testSize );
    test_suite_add_test_case( &result, "testLength", &testLength );
    test_suite_add_test_case( &result, "testEquals", &testEquals );
    test_suite_add_test_case( &result, "testStartsWith", &testStartsWith );
    test_suite_add_test_case( &result, "testEndsWith", &testEndsWith );
    test_suite_add_test_case( &result, "testContains", &testContains );
    test_suite_add_test_case( &result, "testCharAt", &testCharAt );
    test_suite_add_test_case( &result, "testCharAtLoops", &testCharAtLoops );
    test_suite_add_test_case( &result, "testParseInt", &testParseInt );
    test_suite_add_test_case( &result, "testParseFloat", &testParseFloat );
#ifndef NDEBUG
    /* Fault injection works not in NDEBUG mode*/
    test_suite_add_test_case( &result, "testParseIntWithInjectedFault", &testParseIntWithInjectedFault );
    test_suite_add_test_case( &result, "testParseFloatWithInjectedFault", &testParseFloatWithInjectedFault );
#endif
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t testSize( test_fixture_t *fix )
{
    unsigned int size;

    /* check size */
    size = utf8string_get_size( NULL );
    TEST_EXPECT_EQUAL_INT( 0, size );

    size = utf8string_get_size( "" );
    TEST_EXPECT_EQUAL_INT( 1, size );

    size = utf8string_get_size( "123456789 123456789" );
    TEST_EXPECT_EQUAL_INT( 20, size );

    size = utf8string_get_size( "ab\xC2\xA2\0" "efg" );
    TEST_EXPECT_EQUAL_INT( 5, size );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEquals( test_fixture_t *fix )
{
    //  prepare
    int equal;
    char dynTestArr1[] = "Hello";
    char dynTestArr2[] = "Hello World";

    //  test utf8string_equals_str
    equal = utf8string_equals_str( NULL, NULL );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_str( "", NULL );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_str( "", "" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_str( dynTestArr1, "Hello" );
    TEST_EXPECT_EQUAL_INT( 1, equal );
    equal = utf8string_equals_str( NULL, "Hello" );
    TEST_EXPECT_EQUAL_INT( 0, equal );
    equal = utf8string_equals_str( dynTestArr1, dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 0, equal );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testLength( test_fixture_t *fix )
{
    int len;

    /* check utf8string_get_length */
    len = utf8string_get_length( NULL );
    TEST_EXPECT_EQUAL_INT( 0, len );
    len = utf8string_get_length( "" );
    TEST_EXPECT_EQUAL_INT( 0, len );
    len = utf8string_get_length( "12" );
    TEST_EXPECT_EQUAL_INT( 2, len );
    len = utf8string_get_length( "123456789 123456789" );
    TEST_EXPECT_EQUAL_INT( 19, len );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testStartsWith( test_fixture_t *fix )
{
    //  prepare
    int matches;
    char dynTestArr1[] = "Hello";
    char dynTestArr2[] = "Hello World";

    //  test utf8string_starts_with_str
    matches = utf8string_starts_with_str( dynTestArr1, dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_str( dynTestArr2, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_str( dynTestArr1, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_str( dynTestArr1, "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8string_starts_with_str( dynTestArr1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_str( NULL, NULL );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_str( NULL, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8string_starts_with_str( "", "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEndsWith( test_fixture_t *fix )
{
    //  prepare
    int matches;
    char dynTestArr1[] = "World";
    char dynTestArr2[] = "Hello World";

    //  test utf8stringbuf_ends_with_str
    matches = utf8string_ends_with_str( dynTestArr1, dynTestArr2 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_str( dynTestArr2, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_str( dynTestArr1, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_str( dynTestArr1, "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );
    matches = utf8string_ends_with_str( dynTestArr1, NULL );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_str( NULL, NULL );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_str( NULL, dynTestArr1 );
    TEST_EXPECT_EQUAL_INT( 0, matches );
    matches = utf8string_ends_with_str( "", "" );
    TEST_EXPECT_EQUAL_INT( 1, matches );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testContains( test_fixture_t *fix )
{
    bool pos;
    char srchArr3[] = "N/A";

    pos = utf8string_contains_str( NULL, srchArr3);
    TEST_EXPECT_EQUAL_INT( false, pos );

    pos = utf8string_contains_str( NULL, "");
    TEST_EXPECT_EQUAL_INT( false, pos );

    pos = utf8string_contains_str( NULL, NULL);
    TEST_EXPECT_EQUAL_INT( false, pos );

    pos = utf8string_contains_str( srchArr3, srchArr3);
    TEST_EXPECT_EQUAL_INT( true, pos );

    pos = utf8string_contains_str( "", srchArr3);
    TEST_EXPECT_EQUAL_INT( false, pos );

    pos = utf8string_contains_str( "_N/A_N/A", srchArr3);
    TEST_EXPECT_EQUAL_INT( true, pos );

    pos = utf8string_contains_str( "_n/a_n/a", srchArr3);
    TEST_EXPECT_EQUAL_INT( false, pos );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCharAt( test_fixture_t *fix )
{
    utf8codepoint_t result;
    char dynTestArr1[6] = "He\xE2\x82\xAC";

    /* check utf8string_get_char_at */
    result = utf8string_get_char_at( dynTestArr1, 0 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( &result ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length( &result ) );
    TEST_EXPECT_EQUAL_INT( 'H', utf8codepoint_get_char( &result ) );

    result = utf8string_get_char_at( dynTestArr1, 2 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( &result ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8codepoint_get_length( &result ) );
    TEST_EXPECT_EQUAL_INT( 0x20ac, utf8codepoint_get_char( &result ) );

    result = utf8string_get_char_at( dynTestArr1, 3 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( &result ) );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( &result ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( &result ) );

    result = utf8string_get_char_at( dynTestArr1, 5 );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_is_valid( &result ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length( &result ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( &result ) );

    result = utf8string_get_char_at( dynTestArr1, 6 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( &result ) );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( &result ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( &result ) );

    result = utf8string_get_char_at( NULL, 0 );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_is_valid( &result ) );
    TEST_EXPECT_EQUAL_INT( UTF8CODEPOINT_INVALID_LEN, utf8codepoint_get_length( &result ) );
    TEST_EXPECT_EQUAL_INT( 0x0, utf8codepoint_get_char( &result ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCharAtLoops( test_fixture_t *fix )
{
    utf8codepoint_t result;
    char dynTestArr1[6] = "He\xE2\x82\xAC";

    /* loop over all bytes, independant of code-points */
    unsigned int countCodePoints = 0;
    unsigned int byte_length = utf8string_get_length( dynTestArr1 );
    for ( int idx = 0; idx < byte_length; idx ++ ) {
        result = utf8string_get_char_at( dynTestArr1, idx );
        if ( utf8codepoint_is_valid( &result ) ) {
            countCodePoints ++;
        }
    }
    TEST_EXPECT_EQUAL_INT( 3, countCodePoints );

    /* loop over all code points */
    countCodePoints = 0;
    byte_length = utf8string_get_length( dynTestArr1 );
    for ( int idx = 0; idx < byte_length; ) {
        result = utf8string_get_char_at( dynTestArr1, idx );
        if ( utf8codepoint_is_valid( &result ) ) {
            countCodePoints ++;
            idx += utf8codepoint_get_length( &result );
        }
        else {
            break;
        }
    }
    TEST_EXPECT_EQUAL_INT( 3, countCodePoints );

    /* loop over buffer size */
    /* this is possibly the fastest way to loop over the buffer contents */
    countCodePoints = 0;
    unsigned int byteSize = utf8string_get_size( dynTestArr1 );
    for ( int idx = 0; idx < byteSize; ) {
        result = utf8string_get_char_at( dynTestArr1, idx );
        if ( utf8codepoint_is_valid( &result ) && ( utf8codepoint_get_char( &result ) != '\0' )) {
            countCodePoints ++;
            idx += utf8codepoint_get_length( &result );
        }
        else {
            break;
        }
    }
    TEST_EXPECT_EQUAL_INT( 3, countCodePoints );

    /* safely loop over buffer size, using a position-index and a loop-limit-counter */
    countCodePoints = 0;
    byteSize = utf8string_get_size( dynTestArr1 );
    unsigned int currentIndex = 0;
    for ( int loopCount = 0; loopCount < byteSize; loopCount ++ ) {
        result = utf8string_get_char_at( dynTestArr1, currentIndex );
        if ( utf8codepoint_is_valid( &result ) && ( utf8codepoint_get_char( &result ) != '\0' )) {
            countCodePoints ++;
            currentIndex += utf8codepoint_get_length( &result );
        }
        else {
            break;
        }
    }
    TEST_EXPECT_EQUAL_INT( 3, countCodePoints );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testParseInt( test_fixture_t *fix )
{
    unsigned int byte_length;
    int64_t number;
    utf8error_t u8err;

    u8err = utf8string_parse_int( NULL, &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, u8err );

    u8err = utf8string_parse_int( "", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, u8err );
    TEST_EXPECT_EQUAL_INT( 0, byte_length );
    TEST_EXPECT_EQUAL_INT( 0, number );

    u8err = utf8string_parse_int( " void", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, u8err );
    TEST_EXPECT_EQUAL_INT( 0, byte_length );
    TEST_EXPECT_EQUAL_INT( 0, number );

    u8err = utf8string_parse_int( "0", NULL, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 0, number );

    u8err = utf8string_parse_int( "-9223372036854775809", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, u8err );
    TEST_EXPECT_EQUAL_INT( 20, byte_length );
    TEST_EXPECT( (0==number)||(LLONG_MIN==number) );

    u8err = utf8string_parse_int( "+9223372036854775808", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, u8err );
    TEST_EXPECT_EQUAL_INT( 20, byte_length );
    TEST_EXPECT( (0==number)||(LLONG_MAX==number) );

    u8err = utf8string_parse_int( "15", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 2, byte_length );
    TEST_EXPECT_EQUAL_INT( 15, number );

    u8err = utf8string_parse_int( "-15", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 3, byte_length );
    TEST_EXPECT_EQUAL_INT( -15, number );

    u8err = utf8string_parse_int( "+2000111222", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 11, byte_length );
    TEST_EXPECT( +2000111222 == number );

    u8err = utf8string_parse_int( "-2000111222", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 11, byte_length );
    TEST_EXPECT( -2000111222 == number );

    u8err = utf8string_parse_int( "  15 cm", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 4, byte_length );
    TEST_EXPECT_EQUAL_INT( 15, number );

    u8err = utf8string_parse_int( " -015 cm", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 5, byte_length );
    TEST_EXPECT_EQUAL_INT( -15, number );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testParseFloat( test_fixture_t *fix )
{
    unsigned int byte_length;
    double number;
    utf8error_t u8err;

    u8err = utf8string_parse_float( NULL, &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NULL_PARAM, u8err );

    u8err = utf8string_parse_float( "", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, u8err );
    TEST_EXPECT_EQUAL_INT( 0, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, number );

    u8err = utf8string_parse_float( " void", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, u8err );
    TEST_EXPECT_EQUAL_INT( 0, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, number );

    u8err = utf8string_parse_float( "0", NULL, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, number );

    u8err = utf8string_parse_float( "-9.999e99999", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, u8err );
    TEST_EXPECT_EQUAL_INT( 12, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( -INFINITY, number );

    u8err = utf8string_parse_float( "+9.999e-99999", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, u8err );
    TEST_EXPECT_EQUAL_INT( 13, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( 0.0, number );

    u8err = utf8string_parse_float( "15.3", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 4, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( 15.3, number );

    u8err = utf8string_parse_float( "-15.e-1", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 7, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( -1.5, number );

    u8err = utf8string_parse_float( "+3.4E38", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 7, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( 3.4E38, number );

    u8err = utf8string_parse_float( "-1.2E-38", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 8, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( -1.2E-38, number );

    u8err = utf8string_parse_float( "  15.0E+1 cm", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 9, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( 150.0, number );

    u8err = utf8string_parse_float( " -015e-1 cm", &byte_length, &number );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err );
    TEST_EXPECT_EQUAL_INT( 8, byte_length );
    TEST_EXPECT_EQUAL_DOUBLE( -1.5, number );

    return TEST_CASE_RESULT_OK;
}

#ifndef NDEBUG
static test_case_result_t testParseIntWithInjectedFault( test_fixture_t *fix )
{
    unsigned int byte_length;
    int64_t number;
    utf8error_t u8err;

    U8_FAULT_INJECT_SETUP( U8_TEST_COND_STRTOLL );
    {
        u8err = utf8string_parse_int( "-9223372036854775809  24", &byte_length, &number );
        TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, u8err );
        TEST_EXPECT_EQUAL_INT( 24, byte_length );
        TEST_EXPECT( (0==number)||(LLONG_MIN==number) );
    }
    U8_FAULT_INJECT_RESET();

    return TEST_CASE_RESULT_OK;
}
#endif

#ifndef NDEBUG
static test_case_result_t testParseFloatWithInjectedFault( test_fixture_t *fix )
{
    unsigned int byte_length;
    double number;
    utf8error_t u8err;

    U8_FAULT_INJECT_SETUP( U8_TEST_COND_STRTOD );
    {
        u8err = utf8string_parse_float( "-9.999e99999  16", &byte_length, &number );
        TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, u8err );
        TEST_EXPECT_EQUAL_INT( 16, byte_length );
        TEST_EXPECT_EQUAL_DOUBLE( -INFINITY, number );
    }
    U8_FAULT_INJECT_RESET();

    return TEST_CASE_RESULT_OK;
}
#endif


/*
 * Copyright 2012-2025 Andreas Warnke
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
