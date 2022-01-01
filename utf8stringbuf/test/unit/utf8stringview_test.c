/* File: utf8stringview_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "util/string/utf8string.h"
#include "util/string/utf8stringview.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void setUp(void);
static void tearDown(void);
static void testInitMacros(void);
static void testInitFunctions(void);
static void testFindFirst(void);

test_suite_t utf8stringview_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8StringViewTest", &setUp, &tearDown );
    test_suite_add_test_case( &result, "testInitMacros", &testInitMacros );
    test_suite_add_test_case( &result, "testInitFunctions", &testInitFunctions );
    test_suite_add_test_case( &result, "testFindFirst", &testFindFirst );
    return result;
}

static void setUp(void)
{
}

static void tearDown(void)
{
}

static void testInitMacros(void)
{
    const char* start;
    size_t len;
    utf8stringview_t my_view;
    static const char *const my_txt = "txt";

    /* check anonymous struct usage of UTF8STRINGVIEW_NULL macro */
    start = utf8stringview_get_start( UTF8STRINGVIEW_NULL );
    TEST_ASSERT_EQUAL_PTR( NULL, start );
    len = utf8stringview_get_length( UTF8STRINGVIEW_NULL );
    TEST_ASSERT_EQUAL_INT( 0, len );
    
    /* check initialization by UTF8STRINGVIEW_NULL macro */
    my_view = UTF8STRINGVIEW_NULL;
    start = utf8stringview_get_start( my_view );
    TEST_ASSERT_EQUAL_PTR( NULL, start );
    len = utf8stringview_get_length( my_view );
    TEST_ASSERT_EQUAL_INT( 0, len );
    
    /* check anonymous struct usage of UTF8STRINGVIEW_STR macro */
    start = utf8stringview_get_start( UTF8STRINGVIEW_STR( my_txt ) );
    TEST_ASSERT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( UTF8STRINGVIEW_STR( my_txt ) );
    TEST_ASSERT_EQUAL_INT( strlen( my_txt ), len );
    
    /* check initialization by UTF8STRINGVIEW_STR macro */
    my_view = UTF8STRINGVIEW_STR( my_txt );
    start = utf8stringview_get_start( my_view );
    TEST_ASSERT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( my_view );
    TEST_ASSERT_EQUAL_INT( strlen( my_txt ), len );
    
    /* check anonymous struct usage of UTF8STRINGVIEW macro */
    start = utf8stringview_get_start( UTF8STRINGVIEW( my_txt, 2 ) );
    TEST_ASSERT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( UTF8STRINGVIEW( my_txt, 2 ) );
    TEST_ASSERT_EQUAL_INT( 2, len );
    
    /* check initialization by UTF8STRINGVIEW macro */
    const utf8stringview_t my_view_2 = UTF8STRINGVIEW( my_txt, 2 );
    start = utf8stringview_get_start( my_view_2 );
    TEST_ASSERT_EQUAL_PTR( my_txt, start );
    len = utf8stringview_get_length( my_view_2 );
    TEST_ASSERT_EQUAL_INT( 2, len );
}


static void testInitFunctions(void)
{
    static const char *const my_txt = "txt";

    /* check initialization with utf8stringview_init function */
    utf8stringview_t my_view_1 = utf8stringview_init( NULL, 0 );
    TEST_ASSERT_EQUAL_PTR( NULL, utf8stringview_get_start( my_view_1 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8stringview_get_length( my_view_1 ) );

    const utf8stringview_t my_view_2 = utf8stringview_init( my_txt, 4 );
    TEST_ASSERT_EQUAL_PTR( my_txt, utf8stringview_get_start( my_view_2 ) );
    TEST_ASSERT_EQUAL_INT( 4, utf8stringview_get_length( my_view_2 ) );

    /* check initialization with utf8stringview_init_str function */
    utf8stringview_t my_view_3 = utf8stringview_init_str( NULL );
    TEST_ASSERT_EQUAL_PTR( NULL, utf8stringview_get_start( my_view_3 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8stringview_get_length( my_view_3 ) );

    const utf8stringview_t my_view_4 = utf8stringview_init_str( my_txt );
    TEST_ASSERT_EQUAL_PTR( my_txt, utf8stringview_get_start( my_view_4 ) );
    TEST_ASSERT_EQUAL_INT( strlen( my_txt ), utf8stringview_get_length( my_view_4 ) );

    /* check initialization with utf8stringview_init_region function */
    utf8stringview_t my_view_5 = utf8stringview_init_region( NULL, 0, 0 );
    TEST_ASSERT_EQUAL_PTR( NULL, utf8stringview_get_start( my_view_5 ) );
    TEST_ASSERT_EQUAL_INT( 0, utf8stringview_get_length( my_view_5 ) );

    const utf8stringview_t my_view_6 = utf8stringview_init_region( my_txt, 1, 3 );
    TEST_ASSERT_EQUAL_PTR( (my_txt+1), utf8stringview_get_start( my_view_6 ) );
    TEST_ASSERT_EQUAL_INT( 3, utf8stringview_get_length( my_view_6 ) );
}

static void testFindFirst(void)
{
    int pos;
    char memoryArr[] = "beforeHELLO ANANASafter";
    utf8stringview_t srchView = utf8stringview_init_region( memoryArr, 6, 12 );

    /* check search with utf8stringview_find_first_str function */
    pos = utf8stringview_find_first_str( srchView, NULL );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( UTF8STRINGVIEW_NULL, "HELLO" );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( UTF8STRINGVIEW_STR(""), "" );
    TEST_ASSERT_EQUAL_INT( -1, pos );
    
    pos = utf8stringview_find_first_str( srchView, "eHELLO" );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( srchView, "HELLO" );
    TEST_ASSERT_EQUAL_INT( 0, pos );

    pos = utf8stringview_find_first_str( srchView, "ANANASa" );
    TEST_ASSERT_EQUAL_INT( -1, pos );
    
    pos = utf8stringview_find_first_str( srchView, "ANAS" );
    TEST_ASSERT_EQUAL_INT( 8, pos );

    pos = utf8stringview_find_first_str( srchView, "" );
    TEST_ASSERT_EQUAL_INT( -1, pos );

    pos = utf8stringview_find_first_str( srchView, " " );
    TEST_ASSERT_EQUAL_INT( 5, pos );
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
