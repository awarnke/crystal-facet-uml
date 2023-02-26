/* File: utf8stringviewtokenizer_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "utf8stringbuf/utf8stringviewtokenizer.h"
#include "utf8stringbuf/utf8stringview.h"
#include "test_expect.h"
#include <string.h>
#include <assert.h>

static void setUp(void);
static void tearDown(void);
static void testIntEmpty(void);
static void testIntSpaceSeparators(void);
static void testIntMixedCodepoints(void);
static void testIntIntegers(void);
static void testFloatEmpty(void);
static void testFloatValidFloats(void);
static void testFloatInvalidFloats(void);

test_suite_t utf8stringviewtokenizer_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8stringviewtokenizer_test", &setUp, &tearDown );
    test_suite_add_test_case( &result, "testIntEmpty", &testIntEmpty );
    test_suite_add_test_case( &result, "testIntSpaceSeparators", &testIntSpaceSeparators );
    test_suite_add_test_case( &result, "testIntMixedCodepoints", &testIntMixedCodepoints );
    test_suite_add_test_case( &result, "testIntIntegers", &testIntIntegers );
    test_suite_add_test_case( &result, "testFloatEmpty", &testFloatEmpty );
    test_suite_add_test_case( &result, "testFloatValidFloats", &testFloatValidFloats );
    test_suite_add_test_case( &result, "testFloatInvalidFloats", &testFloatInvalidFloats );
    return result;
}

static void setUp(void)
{
}

static void tearDown(void)
{
}

static void testIntEmpty(void)
{
    bool has_next;
    utf8stringview_t next;

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_NULL, UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );

    /* init */
    static const char *const my_list_2 = "";
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list_2 ), UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );

    /* init */
    static const char *const my_list_3 = " \r\n\t";
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list_3 ), UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
}

static void testIntSpaceSeparators(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "\nab 12\tab12 \r^$\x7f  ";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_PTR( (my_list+1), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 2, utf8stringview_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "ab" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_PTR( (my_list+4), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "12" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "ab12" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "^" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "$" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
}

static void testIntMixedCodepoints(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = " !\"+-./09:@AZ[`_az{~\x7f\xc2\xa0\xc2\xa1";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_INT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( true, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_PTR( (my_list+1), utf8stringview_get_start( next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "!" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "\"" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "-" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "/" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "09" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, ":" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "@" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "AZ" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "[" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "`" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "_az" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "{" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "~" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "\xc2\xa0\xc2\xa1" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
}

static void testIntIntegers(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = " int+32.23,-32e23-Infinity";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_INT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "int" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 3, utf8stringview_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+32" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "23" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "," ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "-32" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "e23" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "-" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "Infinity" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
}

static void testFloatEmpty(void)
{
    bool has_next;
    utf8stringview_t next;

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_NULL, UTF8STRINGVIEWTOKENMODE_FLOAT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );

    /* init */
    static const char *const my_list_2 = "";
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list_2 ), UTF8STRINGVIEWTOKENMODE_FLOAT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );

    /* init */
    static const char *const my_list_3 = " \r\n\t";
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list_3 ), UTF8STRINGVIEWTOKENMODE_FLOAT );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 0, utf8stringview_get_length( next ) );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
}

static void testFloatValidFloats(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = " int+3EUR+32.23e+1,-InfinityNaN5.e05INFINITY5e-20 5.sec";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_FLOAT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "int" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "EUR" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+32.23e+1" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "," ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 9, utf8stringview_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "-Infinity" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "NaN" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "5.e05" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "INFINITY" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "5e-20" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "5." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "sec" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
}

static void testFloatInvalidFloats(void)
{
    bool has_next;
    utf8stringview_t next;
    static const char *const my_list = "+nan +-.3e++4 +3.e 3.3.3e3.3E3i";

    /* init */
    utf8stringviewtokenizer_t tok;
    utf8stringviewtokenizer_init( &tok, UTF8STRINGVIEW_STR( my_list ), UTF8STRINGVIEWTOKENMODE_FLOAT );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "nan" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "-" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_get_length( next ) );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "e" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+4" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "+3." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "e" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "3.3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "3e3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "." ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "3E3" ) );

    next = utf8stringviewtokenizer_next( &tok );
    TEST_EXPECT_EQUAL_INT( 1, utf8stringview_equals_str( next, "i" ) );

    has_next = utf8stringviewtokenizer_has_next( &tok );
    TEST_EXPECT_EQUAL_INT( false, has_next );

    /* finish */
    utf8stringviewtokenizer_destroy( &tok );
}


/*
 * Copyright 2023-2023 Andreas Warnke
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
