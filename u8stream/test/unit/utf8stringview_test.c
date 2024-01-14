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
#include <limits.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testInitMacros( test_fixture_t *fix );
static test_case_result_t testInitFunctionsOnRightRange( test_fixture_t *fix );
static test_case_result_t testInitFunctionsOnWrongRange( test_fixture_t *fix );
static test_case_result_t testCodepointFunctions( test_fixture_t *fix );
static test_case_result_t testEquals( test_fixture_t *fix );
static test_case_result_t testStartsWith( test_fixture_t *fix );
static test_case_result_t testEndsWith( test_fixture_t *fix );
static test_case_result_t testContains( test_fixture_t *fix );
static test_case_result_t testSplitAtFirst( test_fixture_t *fix );
static test_case_result_t testSplitAtLast( test_fixture_t *fix );
static test_case_result_t testParseInt( test_fixture_t *fix );
static test_case_result_t testParseFloat( test_fixture_t *fix );

test_suite_t utf8stringview_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8StringViewTest", &set_up, &tear_down );
    test_suite_add_test_case( &result, "testInitMacros", &testInitMacros );
    test_suite_add_test_case( &result, "testInitFunctionsOnRightRange", &testInitFunctionsOnRightRange );
    test_suite_add_test_case( &result, "testInitFunctionsOnWrongRange", &testInitFunctionsOnWrongRange );
    test_suite_add_test_case( &result, "testCodepointFunctions", &testCodepointFunctions );
    test_suite_add_test_case( &result, "testEquals", &testEquals );
    test_suite_add_test_case( &result, "testStartsWith", &testStartsWith );
    test_suite_add_test_case( &result, "testEndsWith", &testEndsWith );
    test_suite_add_test_case( &result, "testContains", &testContains );
    test_suite_add_test_case( &result, "testSplitAtFirst", &testSplitAtFirst );
    test_suite_add_test_case( &result, "testSplitAtLast", &testSplitAtLast );
    test_suite_add_test_case( &result, "testParseInt", &testParseInt );
    test_suite_add_test_case( &result, "testParseFloat", &testParseFloat );
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
    start = utf8stringview_get_start( &UTF8STRINGVIEW_EMPTY );
    TEST_EXPECT_EQUAL_PTR( NULL, start );
    len = utf8stringview_get_length( &UTF8STRINGVIEW_EMPTY );
    TEST_EXPECT_EQUAL_INT( 0, len );

    /* check initialization by UTF8STRINGVIEW_NULL macro */
    my_view = UTF8STRINGVIEW_EMPTY;
    start = utf8stringview_get_start( &my_view );
    TEST_EXPECT_EQUAL_PTR( NULL, start );
    len = utf8stringview_get_length( &my_view );
    TEST_EXPECT_EQUAL_INT( 0, len );

    /* check anonymous struct usage of UTF8STRINGVIEW_STR macro */
    start = utf8stringview_get_start( &UTF8STRINGVIEW_STR( my_txt ) );
    TEST_EXPECT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( &UTF8STRINGVIEW_STR( my_txt ) );
    TEST_EXPECT_EQUAL_INT( strlen( my_txt ), len );

    /* check initialization by UTF8STRINGVIEW_STR macro */
    my_view = UTF8STRINGVIEW_STR( my_txt );
    start = utf8stringview_get_start( &my_view );
    TEST_EXPECT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( &my_view );
    TEST_EXPECT_EQUAL_INT( strlen( my_txt ), len );

    /* check anonymous struct usage of UTF8STRINGVIEW macro */
    start = utf8stringview_get_start( &UTF8STRINGVIEW( my_txt, 2 ) );
    TEST_EXPECT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( &UTF8STRINGVIEW( my_txt, 2 ) );
    TEST_EXPECT_EQUAL_INT( 2, len );

    /* check initialization by UTF8STRINGVIEW macro */
    const utf8stringview_t my_view_2 = UTF8STRINGVIEW( my_txt, 2 );
    start = utf8stringview_get_start( &my_view_2 );
    TEST_EXPECT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( &my_view_2 );
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
    /* sub test case: start ok, end on 1-byte char */
    err = utf8stringview_init( &my_view, my_txt, 3 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( my_txt, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, end on 2-byte char */
    err = utf8stringview_init( &my_view, my_long_txt, 4 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, end on 3-byte char */
    err = utf8stringview_init( &my_view, my_long_txt, 8 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 8, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, end on 4-byte char */
    err = utf8stringview_init( &my_view, my_long_txt, 13 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 13, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* check initialization with utf8stringview_init_str function */
    /* sub test case: start ok, end on 1-byte char */
    utf8stringview_init_str( &my_view, my_txt );
    TEST_EXPECT_EQUAL_PTR( my_txt, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* check initialization with utf8stringview_init_region function */
    /* sub test case: start ok at 1-byte char, end exactly at string end with 1-byte char*/
    err = utf8stringview_init_region( &my_view, my_txt, 1, 2 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( (my_txt+1), utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start at 2-byte char, end exactly at string end with 4-byte char*/
    err = utf8stringview_init_region( &my_view, my_long_txt, 2, 11 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, err );
    TEST_EXPECT_EQUAL_PTR( (my_long_txt+2), utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 11, utf8stringview_get_length( &my_view ) );
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
    TEST_EXPECT_EQUAL_PTR( my_long_txt+4, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 10, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start on 2-byte cut-char, end on 3-byte cut-char */
    err = utf8stringview_init( &my_view, my_long_txt+6, 6 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+8, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start on 3-byte cut-char but only 1 in range  */
    err = utf8stringview_init( &my_view, my_long_txt+10, 1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+11, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start on 3-byte cut-char, end ok  */
    err = utf8stringview_init( &my_view, my_long_txt+10, 3 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+13, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, ends on 1-byte char and a 2-byte cut-char */
    err = utf8stringview_init( &my_view, my_long_txt+5, 6 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+5, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, ends on 1-byte char and a 1-byte cut-char */
    err = utf8stringview_init( &my_view, my_long_txt+5, 5 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_long_txt+5, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: start ok, ends on 2-byte char and a 1-byte cut-char */
    err = utf8stringview_init( &my_view, my_txt, 3 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_txt, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* no check initialization with utf8stringview_init_str function, there are no range parameters */

    /* check initialization with utf8stringview_init_region function */
    /* sub test case: region contains a terminating zero */
    err = utf8stringview_init_region( &my_view, my_txt, 1, 7 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_txt+2, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    /* sub test case: region is after a terminating zero */
    err = utf8stringview_init_region( &my_view, my_txt, 7, 1 );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, err );
    TEST_EXPECT_EQUAL_PTR( my_txt+7, utf8stringview_get_start( &my_view ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &my_view ) );
    utf8stringview_destroy( &my_view );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testCodepointFunctions( test_fixture_t *fix )
{
    static const char *const my_txt = "1-\xc3\xb7-\xe1\xb4\x81-\xf0\x92\x80\x80";

    const utf8stringview_t valid = UTF8STRINGVIEW_STR( my_txt );
    const size_t count1 = utf8stringview_count_codepoints( &valid );
    TEST_EXPECT_EQUAL_INT( 7, count1 );

    const utf8stringview_t invalidStart = UTF8STRINGVIEW_STR( &(my_txt[3]) );
    const size_t count2 = utf8stringview_count_codepoints( &invalidStart );
    TEST_EXPECT_EQUAL_INT( 4, count2 );

    const utf8stringview_t invalidEnd = UTF8STRINGVIEW( my_txt, 12 );
    const size_t count3 = utf8stringview_count_codepoints( &invalidEnd );
    TEST_EXPECT_EQUAL_INT( 6, count3 );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEquals( test_fixture_t *fix )
{
    static const char *const ananas_arr = "ANANAS";
    const utf8stringview_t ananas_view = UTF8STRINGVIEW_STR( ananas_arr );

    /* section string parameter */

    const bool result1 = utf8stringview_equals_str( &ananas_view, "" );
    TEST_EXPECT_EQUAL_INT( false, result1 );

    const bool result2 = utf8stringview_equals_str( &ananas_view, "ANANAS" );
    TEST_EXPECT_EQUAL_INT( true, result2 );

    const bool result3 = utf8stringview_equals_str( &UTF8STRINGVIEW_EMPTY, "" );
    TEST_EXPECT_EQUAL_INT( true, result3 );

    const bool result4 = utf8stringview_equals_str( &ananas_view, "ANANAS***" );
    TEST_EXPECT_EQUAL_INT( false, result4 );

    /* section stringview parameter */

    const bool result5 = utf8stringview_equals_view( &ananas_view, &UTF8STRINGVIEW_EMPTY );
    TEST_EXPECT_EQUAL_INT( false, result5 );

    const bool result6 = utf8stringview_equals_view( &ananas_view, &UTF8STRINGVIEW_STR("ANANAS") );
    TEST_EXPECT_EQUAL_INT( true, result6 );

    const bool result7 = utf8stringview_equals_view( &UTF8STRINGVIEW_EMPTY, &UTF8STRINGVIEW_STR("") );
    TEST_EXPECT_EQUAL_INT( true, result7 );

    const bool result8 = utf8stringview_equals_view( &ananas_view, &UTF8STRINGVIEW_STR("ANANAS***") );
    TEST_EXPECT_EQUAL_INT( false, result8 );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testStartsWith( test_fixture_t *fix )
{
    static const char *const ananas_arr = "ANANAS";
    const utf8stringview_t ananas_view = UTF8STRINGVIEW_STR( ananas_arr );

    /* section string parameter */

    const bool result0 = utf8stringview_starts_with_str( &ananas_view, "" );
    TEST_EXPECT_EQUAL_INT( true, result0 );

    const bool result1 = utf8stringview_starts_with_str( &ananas_view, "AN" );
    TEST_EXPECT_EQUAL_INT( true, result1 );

    const bool result2 = utf8stringview_starts_with_str( &ananas_view, "AS" );
    TEST_EXPECT_EQUAL_INT( false, result2 );

    const bool result3 = utf8stringview_starts_with_str( &UTF8STRINGVIEW_EMPTY, "" );
    TEST_EXPECT_EQUAL_INT( true, result3 );

    const bool result4 = utf8stringview_starts_with_str( &ananas_view, "ANANAS***" );
    TEST_EXPECT_EQUAL_INT( false, result4 );

    /* section stringview parameter */

    const bool result5 = utf8stringview_starts_with_view( &ananas_view, &UTF8STRINGVIEW_EMPTY );
    TEST_EXPECT_EQUAL_INT( true, result5 );

    const bool result6 = utf8stringview_starts_with_view( &ananas_view, &UTF8STRINGVIEW_STR("AN") );
    TEST_EXPECT_EQUAL_INT( true, result6 );

    const bool result7 = utf8stringview_starts_with_view( &ananas_view, &UTF8STRINGVIEW_STR("AS") );
    TEST_EXPECT_EQUAL_INT( false, result7 );

    const bool result8 = utf8stringview_starts_with_view( &UTF8STRINGVIEW_EMPTY, &UTF8STRINGVIEW_STR("") );
    TEST_EXPECT_EQUAL_INT( true, result8 );

    const bool result9 = utf8stringview_starts_with_view( &ananas_view, &UTF8STRINGVIEW_STR("ANANAS***") );
    TEST_EXPECT_EQUAL_INT( false, result9 );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEndsWith( test_fixture_t *fix )
{
    static const char *const ananas_arr = "ANANAS";
    const utf8stringview_t ananas_view = UTF8STRINGVIEW_STR( ananas_arr );

    /* section string parameter */

    const bool result0 = utf8stringview_ends_with_str( &ananas_view, "" );
    TEST_EXPECT_EQUAL_INT( true, result0 );

    const bool result1 = utf8stringview_ends_with_str( &ananas_view, "AN" );
    TEST_EXPECT_EQUAL_INT( false, result1 );

    const bool result2 = utf8stringview_ends_with_str( &ananas_view, "AS" );
    TEST_EXPECT_EQUAL_INT( true, result2 );

    const bool result3 = utf8stringview_ends_with_str( &UTF8STRINGVIEW_EMPTY, "" );
    TEST_EXPECT_EQUAL_INT( true, result3 );

    const bool result4 = utf8stringview_ends_with_str( &ananas_view, "***ANANAS" );
    TEST_EXPECT_EQUAL_INT( false, result4 );

    /* section stringview parameter */

    const bool result5 = utf8stringview_ends_with_view( &ananas_view, &UTF8STRINGVIEW_EMPTY );
    TEST_EXPECT_EQUAL_INT( true, result5 );

    const bool result6 = utf8stringview_ends_with_view( &ananas_view, &UTF8STRINGVIEW_STR("AN") );
    TEST_EXPECT_EQUAL_INT( false, result6 );

    const bool result7 = utf8stringview_ends_with_view( &ananas_view, &UTF8STRINGVIEW_STR("AS") );
    TEST_EXPECT_EQUAL_INT( true, result7 );

    const bool result8 = utf8stringview_ends_with_view( &UTF8STRINGVIEW_EMPTY, &UTF8STRINGVIEW_STR("") );
    TEST_EXPECT_EQUAL_INT( true, result8 );

    const bool result9 = utf8stringview_ends_with_view( &ananas_view, &UTF8STRINGVIEW_STR("***ANANAS") );
    TEST_EXPECT_EQUAL_INT( false, result9 );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testContains( test_fixture_t *fix )
{
    static const char *const ananas_arr = "ANANAS";
    const utf8stringview_t ananas_view = UTF8STRINGVIEW_STR( ananas_arr );

    /* section string parameter */

    const bool result0 = utf8stringview_contains_str( &ananas_view, "" );
    TEST_EXPECT_EQUAL_INT( true, result0 );

    const bool result1 = utf8stringview_contains_str( &ananas_view, "NA" );
    TEST_EXPECT_EQUAL_INT( true, result1 );

    const bool result2 = utf8stringview_contains_str( &ananas_view, "SA" );
    TEST_EXPECT_EQUAL_INT( false, result2 );

    const bool result3 = utf8stringview_contains_str( &UTF8STRINGVIEW_EMPTY, "" );
    TEST_EXPECT_EQUAL_INT( true, result3 );

    const bool result4 = utf8stringview_contains_str( &ananas_view, "***ANANAS***" );
    TEST_EXPECT_EQUAL_INT( false, result4 );

    /* section stringview parameter */

    const bool result5 = utf8stringview_contains_view( &ananas_view, &UTF8STRINGVIEW_EMPTY );
    TEST_EXPECT_EQUAL_INT( true, result5 );

    const bool result6 = utf8stringview_contains_view( &ananas_view, &UTF8STRINGVIEW_STR("NA") );
    TEST_EXPECT_EQUAL_INT( true, result6 );

    const bool result7 = utf8stringview_contains_view( &ananas_view, &UTF8STRINGVIEW_STR("SA") );
    TEST_EXPECT_EQUAL_INT( false, result7 );

    const bool result8 = utf8stringview_contains_view( &UTF8STRINGVIEW_EMPTY, &UTF8STRINGVIEW_STR("") );
    TEST_EXPECT_EQUAL_INT( true, result8 );

    const bool result9 = utf8stringview_contains_view( &ananas_view, &UTF8STRINGVIEW_STR("***ANANAS***") );
    TEST_EXPECT_EQUAL_INT( false, result9 );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testSplitAtFirst( test_fixture_t *fix )
{
    static const char *const ananas_arr = "ANANAS";
    const utf8stringview_t ananas_view = UTF8STRINGVIEW_STR( ananas_arr );
    utf8stringview_t before = UTF8STRINGVIEW_EMPTY;
    utf8stringview_t after = UTF8STRINGVIEW_EMPTY;

    /* section string parameter */

    const utf8error_t result0 = utf8stringview_split_at_first_str( &ananas_view, "", &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result0 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &after ) );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringview_get_length( &after ) );

    const utf8error_t result1 = utf8stringview_split_at_first_str( &ananas_view, "AN", &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result1 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[2]), utf8stringview_get_start( &after ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &after ) );

    const utf8error_t result2 = utf8stringview_split_at_first_str( &ananas_view, "SA", &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, result2 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[2]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &after ) );

    const utf8error_t result3 = utf8stringview_split_at_first_str( &UTF8STRINGVIEW_EMPTY, "", NULL, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result3 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[2]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &after ) );

    const utf8error_t result4 = utf8stringview_split_at_first_str( &ananas_view, "***ANANAS***", &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, result4 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[2]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &after ) );

    /* section stringview parameter */

    const utf8error_t result5 = utf8stringview_split_at_first_view( &ananas_view, &UTF8STRINGVIEW_EMPTY, &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result5 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &after ) );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringview_get_length( &after ) );

    const utf8error_t result6 = utf8stringview_split_at_first_view( &ananas_view, &UTF8STRINGVIEW_STR("AN"), &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result6 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[2]), utf8stringview_get_start( &after ) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &after ) );

    const utf8error_t result7 = utf8stringview_split_at_first_view( &ananas_view, &UTF8STRINGVIEW_STR("SA"), &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, result7 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[2]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &after ) );

    const utf8error_t result8 = utf8stringview_split_at_first_view( &UTF8STRINGVIEW_EMPTY, &UTF8STRINGVIEW_STR(""), NULL, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result8 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[2]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &after ) );

    const utf8error_t result9 = utf8stringview_split_at_first_view( &ananas_view, &UTF8STRINGVIEW_STR("***ANANAS***"), &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, result9 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[2]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &after ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testSplitAtLast( test_fixture_t *fix )
{
    static const char *const ananas_arr = "ANANAS";
    const utf8stringview_t ananas_view = UTF8STRINGVIEW_STR( ananas_arr );
    utf8stringview_t before = UTF8STRINGVIEW_EMPTY;
    utf8stringview_t after = UTF8STRINGVIEW_EMPTY;

    /* section string parameter */

    const utf8error_t result0 = utf8stringview_split_at_last_str( &ananas_view, "", &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result0 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[6]), utf8stringview_get_start( &after ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &after ) );

    const utf8error_t result1 = utf8stringview_split_at_last_str( &ananas_view, "AN", &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result1 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[4]), utf8stringview_get_start( &after ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &after ) );

    const utf8error_t result2 = utf8stringview_split_at_last_str( &ananas_view, "SA", &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, result2 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[4]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &after ) );

    const utf8error_t result3 = utf8stringview_split_at_last_str( &UTF8STRINGVIEW_EMPTY, "", NULL, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result3 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[4]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &after ) );

    const utf8error_t result4 = utf8stringview_split_at_last_str( &ananas_view, "***ANANAS***", &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, result4 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[4]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &after ) );

    /* section stringview parameter */

    const utf8error_t result5 = utf8stringview_split_at_last_view( &ananas_view, &UTF8STRINGVIEW_EMPTY, &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result5 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );
    TEST_EXPECT_EQUAL_INT( 6, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[6]), utf8stringview_get_start( &after ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &after ) );

    const utf8error_t result6 = utf8stringview_split_at_last_view( &ananas_view, &UTF8STRINGVIEW_STR("AN"), &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result6 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[4]), utf8stringview_get_start( &after ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &after ) );

    const utf8error_t result7 = utf8stringview_split_at_last_view( &ananas_view, &UTF8STRINGVIEW_STR("SA"), &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, result7 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[4]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &after ) );

    const utf8error_t result8 = utf8stringview_split_at_last_view( &UTF8STRINGVIEW_EMPTY, &UTF8STRINGVIEW_STR(""), NULL, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, result8 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[4]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &after ) );

    const utf8error_t result9 = utf8stringview_split_at_last_view( &ananas_view, &UTF8STRINGVIEW_STR("***ANANAS***"), &before, &after );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, result9 );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[0]), utf8stringview_get_start( &before ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &before ) );
    TEST_EXPECT_EQUAL_PTR( &(ananas_arr[4]), utf8stringview_get_start( &after ) );  /* unchanged */
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &after ) );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testParseInt( test_fixture_t *fix )
{
    static const char *const regular_arr = "-9223372036854775808";
    const utf8stringview_t regular_view = UTF8STRINGVIEW_STR( regular_arr );
    int64_t number;
    utf8stringview_t remainder = UTF8STRINGVIEW_EMPTY;

    const utf8error_t u8err1 = utf8stringview_parse_int( &regular_view, &number, &remainder );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err1 );
    TEST_EXPECT_EQUAL_INT( LLONG_MIN, number );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &remainder ) );

    const utf8stringview_t empty_view = UTF8STRINGVIEW( regular_arr, 0 );
    const utf8error_t u8err2 = utf8stringview_parse_int( &empty_view, &number, &remainder );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, u8err2 );
    TEST_EXPECT_EQUAL_INT( 0, number );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &remainder ) );

    static const char *const irregular_arr = "  -1009223372036854775808*  ";
    const utf8stringview_t irregular_view = UTF8STRINGVIEW_STR( irregular_arr );
    const utf8error_t u8err3 = utf8stringview_parse_int( &irregular_view, &number, &remainder );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, u8err3 );
    TEST_EXPECT( (0==number)||(LLONG_MIN==number) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &remainder ) );

    const utf8error_t u8err4 = utf8stringview_parse_int( &regular_view, &number, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err4 );
    TEST_EXPECT_EQUAL_INT( LLONG_MIN, number );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &remainder ) );  /* unchanged */

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testParseFloat( test_fixture_t *fix )
{
    static const char *const regular_arr = "-2.2250738585072014E-308";
    const utf8stringview_t regular_view = UTF8STRINGVIEW_STR( regular_arr );
    double number;
    utf8stringview_t remainder = UTF8STRINGVIEW_EMPTY;

    const utf8error_t u8err1 = utf8stringview_parse_float( &regular_view, &number, &remainder );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err1 );
    TEST_EXPECT_EQUAL_FLOAT( -2.2250738585072014E-308, number );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &remainder ) );

    const utf8stringview_t empty_view = UTF8STRINGVIEW( regular_arr, 0 );
    const utf8error_t u8err2 = utf8stringview_parse_float( &empty_view, &number, &remainder );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_NOT_FOUND, u8err2 );
    TEST_EXPECT_EQUAL_FLOAT( 0.0, number );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &remainder ) );

    static const char *const irregular_arr = "  0000001.7976931348623159e308*  ";
    const utf8stringview_t irregular_view = UTF8STRINGVIEW_STR( irregular_arr );
    const utf8error_t u8err3 = utf8stringview_parse_float( &irregular_view, &number, &remainder );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_OUT_OF_RANGE, u8err3 );
    TEST_EXPECT_EQUAL_FLOAT( INFINITY, number );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &remainder ) );

    const utf8error_t u8err4 = utf8stringview_parse_float( &regular_view, &number, NULL );
    TEST_EXPECT_EQUAL_INT( UTF8ERROR_SUCCESS, u8err4 );
    TEST_EXPECT_EQUAL_FLOAT( -2.2250738585072014E-308, number );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &remainder ) );  /* unchanged */

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2021-2024 Andreas Warnke
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
