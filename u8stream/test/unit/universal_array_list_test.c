/* File: universal_array_list_test.c; Copyright and License: see below */

#include "u8list/universal_array_list.h"
#include "universal_array_list_test.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <stdio.h>
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_insert_and_retrieve( test_fixture_t *fix );
static test_case_result_t test_max_size( test_fixture_t *fix );
static test_case_result_t test_element_lifecycle( test_fixture_t *fix );
static test_case_result_t test_trace( test_fixture_t *fix );

test_suite_t universal_array_list_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "universal_array_list_test_get_suite",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_insert_and_retrieve", &test_insert_and_retrieve );
    test_suite_add_test_case( &result, "test_max_size", &test_max_size );
    test_suite_add_test_case( &result, "test_element_lifecycle", &test_element_lifecycle );
    test_suite_add_test_case( &result, "test_trace", &test_trace );
    return result;
}

struct test_fixture_struct {
    unsigned int ctor_calls;  /* count constructor callbacks */
    unsigned int dtor_calls;  /* count destructor callbacks */
    unsigned int eq_calls;  /* count equal callbacks */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture.ctor_calls = 0;
    test_fixture.dtor_calls = 0;
    test_fixture.eq_calls = 0;
    return &test_fixture;
}

static void tear_down( test_fixture_t *fix )
{
}

static test_case_result_t test_insert_and_retrieve( test_fixture_t *fix )
{
    u8_error_t err;
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
    TEST_EXPECT_EQUAL_INT( true, universal_array_list_is_empty( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, universal_array_list_get_length( &testee ) );

    /* insert first */
    err = universal_array_list_append ( &testee, "123456" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 1, universal_array_list_get_length( &testee ) );

    /* insert second */
    err = universal_array_list_append ( &testee, "abcdef" );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 2, universal_array_list_get_length( &testee ) );
    TEST_EXPECT_EQUAL_INT( false, universal_array_list_is_empty( &testee ) );

    /* read element */
    TEST_EXPECT_EQUAL_PTR( NULL, universal_array_list_get_const( &testee, 2 ) );
    TEST_EXPECT_EQUAL_PTR( NULL, universal_array_list_get_ptr( &testee, 2 ) );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( "abcdef", universal_array_list_get_const( &testee, 1 ), sizeof(char[7]) ) );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( "abcdef", universal_array_list_get_ptr( &testee, 1 ), sizeof(char[7]) ) );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( "123456", universal_array_list_get_const( &testee, 0 ), sizeof(char[7]) ) );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( "123456", universal_array_list_get_ptr( &testee, 0 ), sizeof(char[7]) ) );

    /* search element */
    TEST_EXPECT_EQUAL_INT( 1, universal_array_list_get_index_of ( &testee, "abcdef" ) );

    /* clear */
    universal_array_list_clear( &testee );
    TEST_EXPECT_EQUAL_INT( true, universal_array_list_is_empty( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, universal_array_list_get_length( &testee ) );
    TEST_EXPECT_EQUAL_PTR( NULL, universal_array_list_get_const( &testee, 0 ) );
    TEST_EXPECT_EQUAL_PTR( NULL, universal_array_list_get_ptr( &testee, 0 ) );

    /* search element */
    TEST_EXPECT_EQUAL_INT( -1, universal_array_list_get_index_of ( &testee, "abcdef" ) );

    /* done */
    universal_array_list_destroy( &testee );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).dtor_calls );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_max_size( test_fixture_t *fix )
{
    u8_error_t err;
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
    TEST_EXPECT_EQUAL_INT( 0, universal_array_list_get_length( &testee ) );

    /* insert first element */
    const uint64_t ele1 = 0x8000cccc10004444;
    err = universal_array_list_append ( &testee, &ele1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 1, universal_array_list_get_length( &testee ) );

    /* insert second element */
    const uint64_t ele2 = 0xcccc100044440000;
    err = universal_array_list_append ( &testee, &ele2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 2, universal_array_list_get_length( &testee ) );

    /* insert self, first time */
    err = universal_array_list_append_all ( &testee, &testee );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 4, universal_array_list_get_length( &testee ) );

    /* insert self, second time */
    err = universal_array_list_append_all ( &testee, &testee );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, err );
    TEST_EXPECT_EQUAL_INT( 5, universal_array_list_get_length( &testee ) );
    TEST_EXPECT_EQUAL_INT( 0, memcmp( &ele1, universal_array_list_get_const( &testee, 4 ), sizeof(uint64_t) ) );

    /* insert third element */
    const uint64_t ele3 = 0x1000444400005b5b;
    err = universal_array_list_append ( &testee, &ele3 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, err );
    TEST_EXPECT_EQUAL_INT( 5, universal_array_list_get_length( &testee ) );

    return TEST_CASE_RESULT_OK;
}

void copy_ctor (double* to_instance, const double* from_instance)
{
    *to_instance = *from_instance;
    test_fixture.ctor_calls++;
}

void dtor (double* instance)
{
    *instance = 0.0;
    test_fixture.dtor_calls++;
}

bool equal (const double* instance_1, const double* instance_2)
{
    test_fixture.eq_calls++;
    return ( *instance_1 == *instance_2 );
}

static test_case_result_t test_element_lifecycle( test_fixture_t *fix )
{
    u8_error_t err;
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
    TEST_EXPECT_EQUAL_INT( 0, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).dtor_calls );

    /* insert first */
    const double ele1 = 17.125;
    err = universal_array_list_append ( &testee, &ele1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 1, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).eq_calls );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).dtor_calls );

    /* clear */
    universal_array_list_clear( &testee );
    TEST_EXPECT_EQUAL_INT( 1, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).eq_calls );
    TEST_EXPECT_EQUAL_INT( 1, (*fix).dtor_calls );

    /* insert first again */
    err = universal_array_list_append ( &testee, &ele1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 2, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).eq_calls );
    TEST_EXPECT_EQUAL_INT( 1, (*fix).dtor_calls );

    /* insert self, first time */
    err = universal_array_list_append_all ( &testee, &testee );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, err );
    TEST_EXPECT_EQUAL_INT( 3, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).eq_calls );
    TEST_EXPECT_EQUAL_INT( 1, (*fix).dtor_calls );

    /* search element */
    int idx = universal_array_list_get_index_of ( &testee, &ele1 );
    TEST_EXPECT_EQUAL_INT( 0, idx );
    TEST_EXPECT_EQUAL_INT( 3, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 1, (*fix).eq_calls );
    TEST_EXPECT_EQUAL_INT( 1, (*fix).dtor_calls );

    /* done */
    universal_array_list_destroy( &testee );
    TEST_EXPECT_EQUAL_INT( 3, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 1, (*fix).eq_calls );
    TEST_EXPECT_EQUAL_INT( 3, (*fix).dtor_calls );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_trace( test_fixture_t *fix )
{
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

    /* do trace */
    universal_array_list_trace( &testee );
    /* no results - except that program still runs */

    /* done */
    universal_array_list_destroy( &testee );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).ctor_calls );
    TEST_EXPECT_EQUAL_INT( 0, (*fix).dtor_calls );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2020-2025 Andreas Warnke
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
