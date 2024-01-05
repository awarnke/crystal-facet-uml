/* File: utf8stringview_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "utf8stringbuf/utf8string.h"
#include "utf8stringbuf/utf8stringview.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testInitMacros( test_fixture_t *fix );
static test_case_result_t testInitFunctionsOnRightRange( test_fixture_t *fix );
static test_case_result_t testInitFunctionsOnWrongRange( test_fixture_t *fix );
static test_case_result_t testCodepointFunctions( test_fixture_t *fix );
static test_case_result_t testFindFirst( test_fixture_t *fix );

test_suite_t utf8stringview_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8StringViewTest", &set_up, &tear_down );
    test_suite_add_test_case( &result, "testInitMacros", &testInitMacros );
    test_suite_add_test_case( &result, "testInitFunctionsOnRightRange", &testInitFunctionsOnRightRange );
    test_suite_add_test_case( &result, "testInitFunctionsOnWrongRange", &testInitFunctionsOnWrongRange );
    test_suite_add_test_case( &result, "testCodepointFunctions", &testCodepointFunctions );
    test_suite_add_test_case( &result, "testFindFirst", &testFindFirst );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t testInitMacros( test_fixture_t *fix )
{
    const char* start;
    size_t len;
    utf8stringview_t my_view;
    static const char *const my_txt = "txt";

    /* check anonymous struct usage of UTF8STRINGVIEW_NULL macro */
    start = utf8stringview_get_start( UTF8STRINGVIEW_NULL );
    TEST_EXPECT_EQUAL_PTR( NULL, start );
    len = utf8stringview_get_length( UTF8STRINGVIEW_NULL );
    TEST_EXPECT_EQUAL_INT( 0, len );

    /* check initialization by UTF8STRINGVIEW_NULL macro */
    my_view = UTF8STRINGVIEW_NULL;
    start = utf8stringview_get_start( my_view );
    TEST_EXPECT_EQUAL_PTR( NULL, start );
    len = utf8stringview_get_length( my_view );
    TEST_EXPECT_EQUAL_INT( 0, len );

    /* check anonymous struct usage of UTF8STRINGVIEW_STR macro */
    start = utf8stringview_get_start( UTF8STRINGVIEW_STR( my_txt ) );
    TEST_EXPECT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( UTF8STRINGVIEW_STR( my_txt ) );
    TEST_EXPECT_EQUAL_INT( strlen( my_txt ), len );

    /* check initialization by UTF8STRINGVIEW_STR macro */
    my_view = UTF8STRINGVIEW_STR( my_txt );
    start = utf8stringview_get_start( my_view );
    TEST_EXPECT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( my_view );
    TEST_EXPECT_EQUAL_INT( strlen( my_txt ), len );

    /* check anonymous struct usage of UTF8STRINGVIEW macro */
    start = utf8stringview_get_start( UTF8STRINGVIEW( my_txt, 2 ) );
    TEST_EXPECT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( UTF8STRINGVIEW( my_txt, 2 ) );
    TEST_EXPECT_EQUAL_INT( 2, len );

    /* check initialization by UTF8STRINGVIEW macro */
    const utf8stringview_t my_view_2 = UTF8STRINGVIEW( my_txt, 2 );
    start = utf8stringview_get_start( my_view_2 );
    TEST_EXPECT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( my_view_2 );
    TEST_EXPECT_EQUAL_INT( 2, len );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testInitFunctionsOnRightRange( test_fixture_t *fix )
{
    static const char *const my_txt = "txt";
    static const char *const my_long_txt = "1-\xc3\xb7-\xe1\xb4\x81-\xf0\x92\x80\x80";
    utf8error_t err;
    utf8stringview_t my_view;

    /* check initialization with utf8stringview_init function */
    /* sub test case: NULL, 0 */
    err = utf8stringview_init( &my_view, NULL, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, end on 1-byte char */
    err = utf8stringview_init( &my_view, my_txt, 3 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( my_txt, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, end on 2-byte char */
    err = utf8stringview_init( &my_view, my_long_txt, 4 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, end on 3-byte char */
    err = utf8stringview_init( &my_view, my_long_txt, 8 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 8, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, end on 4-byte char */
    err = utf8stringview_init( &my_view, my_long_txt, 13 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 13, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* check initialization with utf8stringview_init_str function */
    /* sub test case: NULL, 0 */
    utf8stringview_init_str( &my_view, NULL );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, end on 1-byte char */
    utf8stringview_init_str( &my_view, my_txt );
    TEST_EXPECT_EQUAL_PTR( my_txt, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* check initialization with utf8stringview_init_region function */
    /* sub test case: NULL, 0 */
    err = utf8stringview_init_region( &my_view, NULL, 0, 0 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok at 1-byte char, end exactly at string end with 1-byte char*/
    err = utf8stringview_init_region( &my_view, my_txt, 1, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( (my_txt+1), utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start at 2-byte char, end exactly at string end with 4-byte char*/
    err = utf8stringview_init_region( &my_view, my_long_txt, 2, 11 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( (my_long_txt+2), utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 11, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testInitFunctionsOnWrongRange( test_fixture_t *fix )
{
    static const char *const my_txt = "\xc3\xb7" "\xe1\xb4\x81" "t" "\0" "t";
    static const char *const my_long_txt = "1-\xc3\xb7-\xe1\xb4\x81-\xf0\x92\x80\x80";
    utf8error_t err;
    utf8stringview_t my_view;

    /* check initialization with utf8stringview_init function */
    /* sub test case: start on 1-byte cut-char, end after terminating zero */
    err = utf8stringview_init( &my_view, my_long_txt+3, 11 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+4, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 10, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start on 2-byte cut-char, end on 3-byte cut-char */
    err = utf8stringview_init( &my_view, my_long_txt+6, 6 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+8, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start on 3-byte cut-char but only 1 in range  */
    err = utf8stringview_init( &my_view, my_long_txt+10, 1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+11, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, ends on 1-byte char and a 2-byte cut-char */
    err = utf8stringview_init( &my_view, my_long_txt+5, 6 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+5, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, ends on 1-byte char and a 1-byte cut-char */
    err = utf8stringview_init( &my_view, my_long_txt+5, 5 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+5, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, ends on 2-byte char and a 1-byte cut-char */
    err = utf8stringview_init( &my_view, my_txt, 3 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_txt, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    /* no check initialization with utf8stringview_init_str function, there are no range parameters */

    /* check initialization with utf8stringview_init_region function */
    /* sub test case: region contains a termination zero */
    err = utf8stringview_init_region( &my_view, my_txt, 1, 7 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_txt+2, utf8stringview_get_start( my_view ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( my_view ) );
    utf8stringview_destroy( &my_view );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCodepointFunctions( test_fixture_t *fix )
{
    static const char *const my_txt = "1-\xc3\xb7-\xe1\xb4\x81-\xf0\x92\x80\x80";

    const utf8stringview_t valid = UTF8STRINGVIEW_STR( my_txt );
    const size_t count1 = utf8stringview_count_codepoints( valid );
    TEST_EXPECT_EQUAL_INT( 7, count1 );

    const utf8stringview_t invalidStart = UTF8STRINGVIEW_STR( &(my_txt[3]) );
    const size_t count2 = utf8stringview_count_codepoints( invalidStart );
    TEST_EXPECT_EQUAL_INT( 4, count2 );

    const utf8stringview_t invalidEnd = UTF8STRINGVIEW( my_txt, 12 );
    const size_t count3 = utf8stringview_count_codepoints( invalidEnd );
    TEST_EXPECT_EQUAL_INT( 6, count3 );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testFindFirst( test_fixture_t *fix )
{
    int pos;
    char memoryArr[] = "beforeHELLO ANANASafter";
    utf8stringview_t srchView;
    utf8error_t err = utf8stringview_init_region( &srchView, memoryArr, 6, 12 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );

    /* check search with utf8stringview_find_first_str function */
    pos = utf8stringview_find_first_str( srchView, NULL );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( UTF8STRINGVIEW_NULL, "HELLO" );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( UTF8STRINGVIEW_STR(""), "" );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( srchView, "eHELLO" );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( srchView, "HELLO" );
    TEST_EXPECT_EQUAL_INT( 0, pos );

    pos = utf8stringview_find_first_str( srchView, "ANANASa" );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( srchView, "ANAS" );
    TEST_EXPECT_EQUAL_INT( 8, pos );

    pos = utf8stringview_find_first_str( srchView, "" );
    TEST_EXPECT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( srchView, " " );
    TEST_EXPECT_EQUAL_INT( 5, pos );

    utf8stringview_destroy( &srchView );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2021-2023 Andreas Warnke
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
