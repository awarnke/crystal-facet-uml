/* File: utf8stringviewiterator_test.c; Copyright and License: see below */

#include "utf8string_test.h"
#include "util/string/utf8string.h"
#include "util/string/utf8stringbuf.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void setUp(void);
static void tearDown(void);
static void testSize(void);
static void testLength(void);

test_suite_t utf8stringviewiterator_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "utf8StringViewIteratorTest", &setUp, &tearDown );
    test_suite_add_test_case( &result, "testSize", &testSize );
    test_suite_add_test_case( &result, "testLength", &testLength );
    return result;
}

static void setUp(void)
{
}

static void tearDown(void)
{
}

static void testSize(void)
{
    unsigned int size;

    /* check size */
    size = utf8string_get_size( NULL );
    TEST_ASSERT_EQUAL_INT( 0, size );

    size = utf8string_get_size( "" );
    TEST_ASSERT_EQUAL_INT( 1, size );

    size = utf8string_get_size( "123456789 123456789" );
    TEST_ASSERT_EQUAL_INT( 20, size );

    size = utf8string_get_size( "ab\xC2\xA2\0" "efg" );
    TEST_ASSERT_EQUAL_INT( 5, size );
}

static void testLength(void)
{
    int len;

    /* check utf8string_get_length */
    len = utf8string_get_length( NULL );
    TEST_ASSERT_EQUAL_INT( 0, len );
    len = utf8string_get_length( "" );
    TEST_ASSERT_EQUAL_INT( 0, len );
    len = utf8string_get_length( "12" );
    TEST_ASSERT_EQUAL_INT( 2, len );
    len = utf8string_get_length( "123456789 123456789" );
    TEST_ASSERT_EQUAL_INT( 19, len );
}


/*
 * Copyright 2021-2021 Andreas Warnke
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
