/* File: universal_array_list_test.c; Copyright and License: see below */

#include "universal_array_list.h"
#include "universal_array_list_test.h"
#include "test_assert.h"
#include <stdio.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_insert_and_retrieve(void);
static void test_max_size(void);
static void test_element_lifecycle(void);

unsigned int ctor_calls;  /* count constructor callbacks */
unsigned int dtor_calls;  /* count destructor callbacks */
unsigned int eq_calls;  /* count equal callbacks */

test_suite_t universal_array_list_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_array_list_test_get_suite", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_insert_and_retrieve", &test_insert_and_retrieve );
    test_suite_add_test_case( &result, "test_max_size", &test_max_size );
    test_suite_add_test_case( &result, "test_element_lifecycle", &test_element_lifecycle );
    return result;
}

static void set_up(void)
{
    ctor_calls = 0;
    dtor_calls = 0;
    eq_calls = 0;
}

static void tear_down(void)
{
}

static void test_insert_and_retrieve(void)
{
    int err;
    char (string_buf[5])[7];
    universal_array_list_t testee;

    /* init */
    universal_array_list_init ( &testee,
                                5 /*max_elements*/,
                                &(string_buf[0]),
                                sizeof(char[7]),
                                ((char*)(&(string_buf[1])))-((char*)(&(string_buf[0]))),
                                NULL /*copy_ctor*/,
                                NULL /*equal*/,
                                NULL /*dtor*/
                              );
    TEST_ASSERT_EQUAL_INT( true, universal_array_list_is_empty( &testee ) );
    TEST_ASSERT_EQUAL_INT( 0, universal_array_list_get_length( &testee ) );

    /* insert first */
    err = universal_array_list_append ( &testee, "123456" );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 1, universal_array_list_get_length( &testee ) );

    /* insert second */
    err = universal_array_list_append ( &testee, "abcdef" );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 2, universal_array_list_get_length( &testee ) );
    TEST_ASSERT_EQUAL_INT( false, universal_array_list_is_empty( &testee ) );

    /* read element */
    TEST_ASSERT_EQUAL_PTR( NULL, universal_array_list_get_const( &testee, 2 ) );
    TEST_ASSERT_EQUAL_PTR( NULL, universal_array_list_get_ptr( &testee, 2 ) );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( "abcdef", universal_array_list_get_const( &testee, 1 ), sizeof(char[7]) ) );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( "abcdef", universal_array_list_get_ptr( &testee, 1 ), sizeof(char[7]) ) );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( "123456", universal_array_list_get_const( &testee, 0 ), sizeof(char[7]) ) );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( "123456", universal_array_list_get_ptr( &testee, 0 ), sizeof(char[7]) ) );

    /* search element */
    TEST_ASSERT_EQUAL_INT( 1, universal_array_list_get_index_of ( &testee, "abcdef" ) );

    /* clear */
    universal_array_list_clear( &testee );
    TEST_ASSERT_EQUAL_INT( true, universal_array_list_is_empty( &testee ) );
    TEST_ASSERT_EQUAL_INT( 0, universal_array_list_get_length( &testee ) );
    TEST_ASSERT_EQUAL_PTR( NULL, universal_array_list_get_const( &testee, 0 ) );
    TEST_ASSERT_EQUAL_PTR( NULL, universal_array_list_get_ptr( &testee, 0 ) );

    /* search element */
    TEST_ASSERT_EQUAL_INT( -1, universal_array_list_get_index_of ( &testee, "abcdef" ) );
    
    /* done */
    universal_array_list_destroy( &testee );
    TEST_ASSERT_EQUAL_INT( 0, ctor_calls );
    TEST_ASSERT_EQUAL_INT( 0, dtor_calls );
}

static void test_max_size(void)
{
    int err;
    uint64_t buf[5];
    universal_array_list_t testee;

    /* init */
    universal_array_list_init ( &testee,
                                5 /*max_elements*/,
                                &buf,
                                sizeof(uint64_t),
                                ((char*)(&(buf[1])))-((char*)(&(buf[0]))),
                                NULL /*copy_ctor*/,
                                NULL /*equal*/,
                                NULL /*dtor*/
                              );
    TEST_ASSERT_EQUAL_INT( 0, universal_array_list_get_length( &testee ) );

    /* insert first element */
    const uint64_t ele1 = 0x8000cccc10004444;
    err = universal_array_list_append ( &testee, &ele1 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 1, universal_array_list_get_length( &testee ) );

    /* insert second element */
    const uint64_t ele2 = 0xcccc100044440000;
    err = universal_array_list_append ( &testee, &ele2 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 2, universal_array_list_get_length( &testee ) );

    /* insert self, first time */
    err = universal_array_list_append_all ( &testee, &testee );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 4, universal_array_list_get_length( &testee ) );

    /* insert self, second time */
    err = universal_array_list_append_all ( &testee, &testee );
    TEST_ASSERT_EQUAL_INT( -1, err );
    TEST_ASSERT_EQUAL_INT( 5, universal_array_list_get_length( &testee ) );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &ele1, universal_array_list_get_const( &testee, 4 ), sizeof(uint64_t) ) );

    /* insert third element */
    const uint64_t ele3 = 0x1000444400005b5b;
    err = universal_array_list_append ( &testee, &ele3 );
    TEST_ASSERT_EQUAL_INT( -1, err );
    TEST_ASSERT_EQUAL_INT( 5, universal_array_list_get_length( &testee ) );

}

void copy_ctor (double* to_instance, const double* from_instance)
{
    *to_instance = *from_instance;
    ctor_calls++;
}

void dtor (double* instance)
{
    *instance = 0.0;
    dtor_calls++;
}

bool equal (const double* instance_1, const double* instance_2)
{
    eq_calls++;
    return ( *instance_1 == *instance_2 );
}

static void test_element_lifecycle(void)
{
    int err;
    double buf[4];
    universal_array_list_t testee;

    /* init */
    universal_array_list_init ( &testee,
                                4 /*max_elements*/,
                                &buf,
                                sizeof(double),
                                ((char*)(&(buf[1])))-((char*)(&(buf[0]))),
                                (void (*)(void*, const void*))copy_ctor,
                                (bool (*)(const void*, const void*))equal,
                                (void (*)(void*))dtor
                              );
    TEST_ASSERT_EQUAL_INT( 0, ctor_calls );
    TEST_ASSERT_EQUAL_INT( 0, dtor_calls );

    /* insert first */
    const double ele1 = 17.125;
    err = universal_array_list_append ( &testee, &ele1 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 1, ctor_calls );
    TEST_ASSERT_EQUAL_INT( 0, eq_calls );
    TEST_ASSERT_EQUAL_INT( 0, dtor_calls );

    /* clear */
    universal_array_list_clear( &testee );
    TEST_ASSERT_EQUAL_INT( 1, ctor_calls );
    TEST_ASSERT_EQUAL_INT( 0, eq_calls );
    TEST_ASSERT_EQUAL_INT( 1, dtor_calls );

    /* insert first again */
    err = universal_array_list_append ( &testee, &ele1 );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 2, ctor_calls );
    TEST_ASSERT_EQUAL_INT( 0, eq_calls );
    TEST_ASSERT_EQUAL_INT( 1, dtor_calls );

    /* insert self, first time */
    err = universal_array_list_append_all ( &testee, &testee );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 3, ctor_calls );
    TEST_ASSERT_EQUAL_INT( 0, eq_calls );
    TEST_ASSERT_EQUAL_INT( 1, dtor_calls );

    /* search element */
    int idx = universal_array_list_get_index_of ( &testee, &ele1 );
    TEST_ASSERT_EQUAL_INT( 0, idx );
    TEST_ASSERT_EQUAL_INT( 3, ctor_calls );
    TEST_ASSERT_EQUAL_INT( 1, eq_calls );
    TEST_ASSERT_EQUAL_INT( 1, dtor_calls );

    /* done */
    universal_array_list_destroy( &testee );
    TEST_ASSERT_EQUAL_INT( 3, ctor_calls );
    TEST_ASSERT_EQUAL_INT( 1, eq_calls );
    TEST_ASSERT_EQUAL_INT( 3, dtor_calls );
}


/*
 * Copyright 2020-2021 Andreas Warnke
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
