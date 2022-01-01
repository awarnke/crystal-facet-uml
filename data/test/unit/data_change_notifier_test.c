/* File: data_change_notifier_test.c; Copyright and License: see below */

#include "data_change_notifier_test.h"
#include "storage/data_change_notifier.h"
#include "test_assert.h"
#include <glib-object.h>

static void set_up(void);
static void tear_down(void);
static void test_notifier_list_insert_and_remove(void);
static void test_notifier_list_full(void);

struct test_data_struct {
    uint32_t guard_1;
    data_change_notifier_t notifier;
    uint32_t guard_2;
    int32_t max_list_len;
    int32_t max_test_len;
    GObject test_object[DATA_CHANGE_NOTIFIER_MAX_LISTENERS+1];
};
typedef struct test_data_struct test_data_t;

static test_data_t data;

test_suite_t data_change_notifier_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "data_change_notifier_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_notifier_list_insert_and_remove", &test_notifier_list_insert_and_remove );
    test_suite_add_test_case( &result, "test_notifier_list_full", &test_notifier_list_full );
    return result;
}

static void set_up(void)
{
    data_change_notifier_init( &(data.notifier) );
    data.max_list_len = DATA_CHANGE_NOTIFIER_MAX_LISTENERS;
    data.max_test_len = DATA_CHANGE_NOTIFIER_MAX_LISTENERS+1;
    data.guard_1 = 0x4343f9f5u;
    data.guard_2 = 0xf6de0043u;
}

static void tear_down(void)
{
    data_change_notifier_destroy( &(data.notifier) );
}

static void test_notifier_list_insert_and_remove(void)
{
    u8_error_t result;

    /* remove from empty list */
    result = data_change_notifier_remove_listener( &(data.notifier), &(data.test_object[0]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, result );

    /* add one to list */
    result = data_change_notifier_add_listener( &(data.notifier), &(data.test_object[0]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, result );

    /* add another to list */
    result = data_change_notifier_add_listener( &(data.notifier), &(data.test_object[1]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, result );

    /* add the same again to list */
    result = data_change_notifier_add_listener( &(data.notifier), &(data.test_object[1]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, result );

    /* remove not-contained from list */
    result = data_change_notifier_remove_listener( &(data.notifier), &(data.test_object[2]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, result );

    /* remove second from list */
    result = data_change_notifier_remove_listener( &(data.notifier), &(data.test_object[1]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, result );

    /* remove second again from list */
    result = data_change_notifier_remove_listener( &(data.notifier), &(data.test_object[1]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, result );

    /* remove first from list */
    result = data_change_notifier_remove_listener( &(data.notifier), &(data.test_object[0]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, result );
}

static void test_notifier_list_full(void)
{
    u8_error_t result;

    /* insert max_list_len */
    for ( int idx = 0; idx < data.max_list_len; idx ++ )
    {
        result = data_change_notifier_add_listener( &(data.notifier), &(data.test_object[idx]));
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, result );
    }

    /* add one more */
    result = data_change_notifier_add_listener( &(data.notifier), &(data.test_object[data.max_list_len]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, result );

    /* remove max_list_len */
    for ( int idx = 0; idx < data.max_list_len; idx ++ )
    {
        result = data_change_notifier_remove_listener( &(data.notifier), &(data.test_object[idx]));
        TEST_ASSERT_EQUAL_INT( U8_ERROR_NONE, result );
    }

    /* remove one more */
    result = data_change_notifier_remove_listener( &(data.notifier), &(data.test_object[data.max_list_len]));
    TEST_ASSERT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, result );

    /* check that memory was not overwritten */
    TEST_ASSERT_EQUAL_INT( 0x4343f9f5u, data.guard_1 );
    TEST_ASSERT_EQUAL_INT( 0xf6de0043u, data.guard_2 );
}


/*
 * Copyright 2016-2022 Andreas Warnke
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
