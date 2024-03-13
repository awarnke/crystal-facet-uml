/* File: utf8codepointiterator_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "utf8stringbuf/utf8codepointiterator.h"
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
static test_case_result_t testEmptyUseCase( test_fixture_t *fix );
static test_case_result_t testIllegalUseCase( test_fixture_t *fix );

test_suite_t utf8codepointiterator_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "utf8CodePointIteratorTest",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "testStandardUseCase", &testStandardUseCase );
    test_suite_add_test_case( &result, "testEmptyUseCase", &testEmptyUseCase );
    test_suite_add_test_case( &result, "testIllegalUseCase", &testIllegalUseCase );
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
    utf8codepoint_t next;
    static const char my_string[] = "a" "\0" "\xC3\xA4" "\xE2\x82\xAC" "\xF0\x9D\x84\x9E";
    const utf8stringview_t my_string_view = UTF8STRINGVIEW( my_string, sizeof(my_string)-1 );

    /* init */
    utf8codepointiterator_t it;
    utf8codepointiterator_init( &it, &my_string_view );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 97, utf8codepoint_get_char( next ) );  /* a == 97 */
    TEST_EXPECT_EQUAL_INT( true, utf8codepoint_is_valid( next ) );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 1, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_char( next ) );  /* 0 == 0 */
    TEST_EXPECT_EQUAL_INT( true, utf8codepoint_is_valid( next ) );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 2, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 0x00e4, utf8codepoint_get_char( next ) );
    TEST_EXPECT_EQUAL_INT( true, utf8codepoint_is_valid( next ) );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 3, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 0x20ac, utf8codepoint_get_char( next ) );
    TEST_EXPECT_EQUAL_INT( true, utf8codepoint_is_valid( next ) );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 4, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 0x1d11e, utf8codepoint_get_char( next ) );  /* a == 97 */
    TEST_EXPECT_EQUAL_INT( true, utf8codepoint_is_valid( next ) );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( false, utf8codepoint_is_valid( next ) );

    /* finish */
    utf8codepointiterator_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEmptyUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8codepoint_t next;
    static const char *const my_string = "\xF0\x9D\x84\x9E";
    const utf8stringview_t my_string_view = UTF8STRINGVIEW( my_string, 0 );

    /* init */
    utf8codepointiterator_t it;
    utf8codepointiterator_init( &it, &my_string_view );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( false, utf8codepoint_is_valid( next ) );

    /* finish */
    utf8codepointiterator_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testIllegalUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8codepoint_t next;
    static const char *const illegal_string = "\xF0\x9D\x84\x9E";
    const utf8stringview_t illegal_string_view = UTF8STRINGVIEW( illegal_string, 1 );

    /* init */
    utf8codepointiterator_t it;
    utf8codepointiterator_init( &it, &illegal_string_view );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( false, utf8codepoint_is_valid( next ) );

    has_next = utf8codepointiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8codepointiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8codepoint_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( false, utf8codepoint_is_valid( next ) );

    /* finish */
    utf8codepointiterator_destroy( &it );
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
