/* File: utf8stringviewiterator_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "utf8stringbuf/utf8stringviewiterator.h"
#include "utf8stringbuf/utf8stringview.h"
#include "test_expect.h"
#include <string.h>
#include <assert.h>

static void setUp(void);
static void tearDown(void);
static void testStandardUseCase(void);
static void testNoSeparatorUseCase(void);
static void testEmptyElementsUseCase(void);
static void testEmptyListUseCase(void);
static void testEmptySeparatorUseCase(void);
static void testNullListUseCase(void);
static void testNullSeparatorUseCase(void);

test_suite_t utf8stringviewiterator_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8StringViewIteratorTest", &setUp, &tearDown );
    test_suite_add_test_case( &result, "testStandardUseCase", &testStandardUseCase );
    test_suite_add_test_case( &result, "testNoSeparatorUseCase", &testNoSeparatorUseCase );
    test_suite_add_test_case( &result, "testEmptyElementsUseCase", &testEmptyElementsUseCase );
    test_suite_add_test_case( &result, "testEmptyListUseCase", &testEmptyListUseCase );
    test_suite_add_test_case( &result, "testEmptySeparatorUseCase", &testEmptySeparatorUseCase );
    test_suite_add_test_case( &result, "testNullListUseCase", &testNullListUseCase );
    test_suite_add_test_case( &result, "testNullSeparatorUseCase", &testNullSeparatorUseCase );
    return result;
}

static void setUp(void)
{
}

static void tearDown(void)
{
}

static void testStandardUseCase(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "23,, 24";
    
    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, UTF8STRINGVIEW_STR( my_list ), ", " );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+0), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+5), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    /* finish */
    utf8stringviewiterator_destroy( &it );
}

static void testNoSeparatorUseCase(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "23,, 24";
    
    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, UTF8STRINGVIEW_STR( my_list ), ",, 24567" );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+0), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( strlen( my_list ), utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    /* finish */
    utf8stringviewiterator_destroy( &it );
}

static void testEmptyElementsUseCase(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = ",23,, 24,";
    
    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, UTF8STRINGVIEW_STR( my_list ), "," );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+0), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+1), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+4), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+5), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( (my_list+9), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    /* finish */
    utf8stringviewiterator_destroy( &it );
}

static void testEmptyListUseCase(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "";
    
    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, UTF8STRINGVIEW_STR( my_list ), "," );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    /* finish */
    utf8stringviewiterator_destroy( &it );
}

static void testEmptySeparatorUseCase(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "1,2,3";
    
    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, UTF8STRINGVIEW_STR( my_list ), "" );  /* unspecified case */
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( my_list, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( strlen( my_list ), utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    /* finish */
    utf8stringviewiterator_destroy( &it );
}

static void testNullListUseCase(void)
{
    bool has_next;
    utf8stringview_t next;
    
    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, UTF8STRINGVIEW_NULL, "" );  /* unspecified case */
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    /* finish */
    utf8stringviewiterator_destroy( &it );
}

static void testNullSeparatorUseCase(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "1,2,3";
    
    /* init */
    utf8stringviewiterator_t it;
    utf8stringviewiterator_init( &it, UTF8STRINGVIEW_STR( my_list ), NULL );  /* unspecified case */
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( true, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( my_list, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( strlen( my_list ), utf8stringview_get_length( next ) );
    
    has_next = utf8stringviewiterator_has_next( &it );
    TEST_EXPECT_EQUAL_INT( false, has_next );
    
    next = utf8stringviewiterator_next( &it );
    TEST_EXPECT_EQUAL_PTR( NULL, utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );
    
    /* finish */
    utf8stringviewiterator_destroy( &it );
}


/*
 * Copyright 2021-2022 Andreas Warnke
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
