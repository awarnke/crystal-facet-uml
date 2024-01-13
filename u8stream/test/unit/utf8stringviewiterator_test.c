/* File: utf8stringviewiterator_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "utf8stringbuf/utf8stringviewiterator.h"
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
static test_case_result_t testNoSeparatorUseCase( test_fixture_t *fix );
static test_case_result_t testEmptyElementsUseCase( test_fixture_t *fix );
static test_case_result_t testEmptyListUseCase( test_fixture_t *fix );
static test_case_result_t testEmptySeparatorUseCase( test_fixture_t *fix );
static test_case_result_t testNullListUseCase( test_fixture_t *fix );
static test_case_result_t testNullSeparatorUseCase( test_fixture_t *fix );

test_suite_t utf8stringviewiterator_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8StringViewIteratorTest", &set_up, &tear_down );
    test_suite_add_test_case( &result, "testStandardUseCase", &testStandardUseCase );
    test_suite_add_test_case( &result, "testNoSeparatorUseCase", &testNoSeparatorUseCase );
    test_suite_add_test_case( &result, "testEmptyElementsUseCase", &testEmptyElementsUseCase );
    test_suite_add_test_case( &result, "testEmptyListUseCase", &testEmptyListUseCase );
    test_suite_add_test_case( &result, "testEmptySeparatorUseCase", &testEmptySeparatorUseCase );
    test_suite_add_test_case( &result, "testNullListUseCase", &testNullListUseCase );
    test_suite_add_test_case( &result, "testNullSeparatorUseCase", &testNullSeparatorUseCase );
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
    static const char *const my_list = "23,, 24";

    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, &UTF8STRINGVIEW_STR( my_list ), ", " );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+0), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+5), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewiterator_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testNoSeparatorUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "23,, 24";

    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, &UTF8STRINGVIEW_STR( my_list ), ",, 24567" );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+0), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( strlen( my_list ), utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewiterator_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEmptyElementsUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = ",23,, 24,";

    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, &UTF8STRINGVIEW_STR( my_list ), "," );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+0), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+1), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+4), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+5), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+9), utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewiterator_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEmptyListUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "";

    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, &UTF8STRINGVIEW_STR( my_list ), "," );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewiterator_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testEmptySeparatorUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "1,2,3";

    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, &UTF8STRINGVIEW_STR( my_list ), "" );  /* unspecified case */

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( my_list, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( strlen( my_list ), utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewiterator_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testNullListUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;

    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, &UTF8STRINGVIEW_EMPTY, "" );  /* unspecified case */

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewiterator_destroy( &it );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t testNullSeparatorUseCase( test_fixture_t *fix )
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "1,2,3";

    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, &UTF8STRINGVIEW_STR( my_list ), NULL );  /* unspecified case */

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( my_list, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( strlen( my_list ), utf8stringview_get_length( &next ) );

    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( &next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( &next ) );

    /* finish */
    utf8stringviewiterator_destroy( &it );
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
