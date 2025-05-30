/* File: utf8stringlines_test.c; Copyright and License: see below */

#include "utf8stringlines_test.h"
#include "utf8stringbuf/utf8stringlines.h"
#include "utf8stringbuf/utf8stringview.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <string.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t testStandardUseCase( test_fixture_t *fix );
static test_case_result_t testMultiByteUseCase( test_fixture_t *fix );
static test_case_result_t testNoSeparatorUseCase( test_fixture_t *fix );
static test_case_result_t testEmptyUseCase( test_fixture_t *fix );

test_suite_t utf8stringlines_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "utf8StringLinesTest",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "testStandardUseCase", &testStandardUseCase );
    test_suite_add_test_case( &result, "testMultiByteUseCase", &testMultiByteUseCase );
    test_suite_add_test_case( &result, "testNoSeparatorUseCase", &testNoSeparatorUseCase );
    test_suite_add_test_case( &result, "testEmptyUseCase", &testEmptyUseCase );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t testStandardUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "ab \ncd ef ghijklmn\to";

    /* init */
    utf8stringlines_t it;
    utf8stringlines_init( &it, &UTF8STRINGVIEW_STR( my_list ), 5 );

    /* test break before line length */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 'a', *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &next ) );

    /* test space before line length and space after line length */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 'c', *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &next ) );

    /* test space before line length and text after line length */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 'e', *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &next ) );

    /* test text longer than line length */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 'g', *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 9, utf8stringview_get_length( &next ) );

    /* test last character */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 'o', *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( &next ) );

    /* test no more lines */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringlines_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testMultiByteUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "\xe0\xbc\x82 \xc9\xb6 a\xe0\xbc\x82\xe0\xbc\x82\xe0\xbc\x82  ";

    /* init */
    utf8stringlines_t it;
    utf8stringlines_init( &it, &UTF8STRINGVIEW_STR( my_list ), 5 );

    /* test line length checks code points, not bytes */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 0xe0, *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 7, utf8stringview_get_length( &next ) );

    /* test line length is exact 5 code points */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 'a', *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 11, utf8stringview_get_length( &next ) );

    /* test line with only a space */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( ' ', *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( &next ) );

    /* test no more lines */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringlines_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testNoSeparatorUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "abcdefghij";

    /* init */
    utf8stringlines_t it;
    utf8stringlines_init( &it, &UTF8STRINGVIEW_STR( my_list ), 5 );

    /* test line with no space */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 'a', *(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 10, utf8stringview_get_length( &next ) );

    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringlines_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEmptyUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "";

    /* init */
    utf8stringlines_t it;
    utf8stringlines_init( &it, &UTF8STRINGVIEW_STR( my_list ), 5 );

    /* test no characters at all */
    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    has_next = utf8stringlines_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringlines_destroy( &it );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2025-2025 Andreas Warnke
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
