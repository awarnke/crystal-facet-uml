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
static test_case_result_t testAsianUseCase( test_fixture_t *fix );

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
    test_suite_add_test_case( &result, "testAsianUseCase", &testAsianUseCase );
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
    utf8stringlines_t iterator;
    utf8stringlines_init( &iterator, &UTF8STRINGVIEW_STR( my_list ), 5 );

    /* test break before line length */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 'a', (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 4, utf8stringview_get_length( &next ) );

    /* test space before line length and space after line length */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 'c', (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &next ) );

    /* test space before line length and text after line length */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 'e', (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &next ) );

    /* test text longer than line length */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 'g', (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 9, utf8stringview_get_length( &next ) );

    /* test last character */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 'o', (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( &next ) );

    /* test no more lines */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringlines_destroy( &iterator );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testMultiByteUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "\xe0\xbc\x82 \xc9\xb6 a\xe0\xbc\x82\xe0\xbc\x82\xe0\xbc\x82  ";

    /* init */
    utf8stringlines_t iterator;
    utf8stringlines_init( &iterator, &UTF8STRINGVIEW_STR( my_list ), 5 );

    /* test line length checks code points, not bytes */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 0xe0, (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 7, utf8stringview_get_length( &next ) );

    /* test line length is exact 5 code points */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 'a', (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 11, utf8stringview_get_length( &next ) );

    /* test line with only a space */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( ' ', (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( &next ) );

    /* test no more lines */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringlines_destroy( &iterator );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testNoSeparatorUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "abcdefghij";

    /* init */
    utf8stringlines_t iterator;
    utf8stringlines_init( &iterator, &UTF8STRINGVIEW_STR( my_list ), 5 );

    /* test line witeratorh no space */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 'a', (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 10, utf8stringview_get_length( &next ) );

    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringlines_destroy( &iterator );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEmptyUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "";

    /* init */
    utf8stringlines_t iterator;
    utf8stringlines_init( &iterator, &UTF8STRINGVIEW_STR( my_list ), 5 );

    /* test no characters at all */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringlines_destroy( &iterator );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testAsianUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    /* 京都は綺麗で、大阪も近くにあります。東京は遠いです。 */
    static const char *const my_list
        = "\xe4\xba\xac\xe9\x83\xbd\xe3\x81\xaf\xe7\xb6\xba\xe9\xba\x97\xe3\x81\xa7\xe3\x80\x81"
        "\xe5\xa4\xa7\xe9\x98\xaa\xe3\x82\x82\xe8\xbf\x91\xe3\x81\x8f\xe3\x81\xab\xe3\x81\x82\xe3\x82\x8a\xe3\x81\xbe\xe3\x81\x99\xe3\x80\x82"
        "\xe6\x9d\xb1\xe4\xba\xac\xe3\x81\xaf\xe9\x81\xa0\xe3\x81\x84\xe3\x81\xa7\xe3\x81\x99\xe3\x80\x82";

    /* init */
    utf8stringlines_t iterator;
    utf8stringlines_init( &iterator, &UTF8STRINGVIEW_STR( my_list ), 8 );

    /* test break at comma */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 0xe4, (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 7, utf8stringview_count_codepoints( &next ) );

    /* test break at full stop */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 0xe5, (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 11, utf8stringview_count_codepoints( &next ) );

    /* test last line */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_INT( 0xe6, (unsigned char)*(utf8stringview_get_start( &next )) );
    TEST_EXPECT_EQUAL_INT( 8, utf8stringview_count_codepoints( &next ) );

    /* test no more lines */
    has_next = utf8stringlines_has_next( &iterator );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringlines_next( &iterator );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_count_codepoints( &next ) );

    /* finish */
    utf8stringlines_destroy( &iterator );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2025-2026 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance witeratorh the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writeratoring, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, eiteratorher express or implied.
 * See the License for the specific language governing permissions and
 * limiteratorations under the License.
 */
