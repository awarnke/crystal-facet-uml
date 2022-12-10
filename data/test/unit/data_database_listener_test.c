/* File: data_database_listener_test.c; Copyright and License: see below */

#include "data_database_listener_test.h"
#include "storage/data_database_listener.h"
#include "storage/data_database.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_database_listener_register_and_notify(void);
static void test_database_listener_registration_full(void);

test_suite_t data_database_listener_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result, "data_database_listener_test", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_database_listener_register_and_notify", &test_database_listener_register_and_notify );
    test_suite_add_test_case( &result, "test_database_listener_registration_full", &test_database_listener_registration_full );
    return result;
}

static int callback_counter = 0;

static void callback_notification(void* listener_instance, data_database_listener_signal_t signal_id);
static void callback_notification(void* listener_instance, data_database_listener_signal_t signal_id)
{
    (*((int*)listener_instance)) ++;
}

static void set_up(void)
{
    callback_counter = 0;
}

static void tear_down(void)
{
}

static void test_database_listener_register_and_notify(void)
{
    data_database_listener_t my_listener1;
    data_database_listener_t my_listener2;
    data_database_t db;
    u8_error_t d_err;

    data_database_listener_init( &my_listener1, &callback_counter, (void (*)(void*,data_database_listener_signal_t)) &callback_notification );
    data_database_listener_init( &my_listener2, &callback_counter, (void (*)(void*,data_database_listener_signal_t)) &callback_notification );
    data_database_init ( &db );

    d_err = data_database_add_db_listener( &db, &my_listener1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );

    d_err = data_database_add_db_listener( &db, &my_listener2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );

    d_err = data_database_add_db_listener( &db, &my_listener1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, d_err );

    d_err = data_database_private_notify_db_listeners( &db, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    TEST_EXPECT_EQUAL_INT( 2, callback_counter );

    d_err = data_database_remove_db_listener( &db, &my_listener1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );

    d_err = data_database_remove_db_listener( &db, &my_listener2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );

    d_err = data_database_remove_db_listener( &db, &my_listener1 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, d_err );

    d_err = data_database_private_notify_db_listeners( &db, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    TEST_EXPECT_EQUAL_INT( 2, callback_counter );

    data_database_destroy ( &db );
    data_database_listener_destroy ( &my_listener2 );
    data_database_listener_destroy ( &my_listener1 );
}

static void test_database_listener_registration_full(void)
{
    data_database_listener_t my_listener[DATA_DATABASE_MAX_LISTENERS+1];
    data_database_t db;
    u8_error_t d_err;

    data_database_init ( &db );

    for ( int index = 0; index < DATA_DATABASE_MAX_LISTENERS; index ++ )
    {
        data_database_listener_init( &my_listener[index], &callback_counter, (void (*)(void*,data_database_listener_signal_t)) &callback_notification );
        d_err = data_database_add_db_listener( &db, &my_listener[index] );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    }
    data_database_listener_init( &my_listener[DATA_DATABASE_MAX_LISTENERS], &callback_counter, (void (*)(void*,data_database_listener_signal_t)) &callback_notification );
    d_err = data_database_add_db_listener( &db, &my_listener[DATA_DATABASE_MAX_LISTENERS] );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, d_err );

    d_err = data_database_private_notify_db_listeners( &db, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    TEST_EXPECT_EQUAL_INT( DATA_DATABASE_MAX_LISTENERS, callback_counter );

    data_database_private_clear_db_listener_list( &db );

    d_err = data_database_private_notify_db_listeners( &db, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    TEST_EXPECT_EQUAL_INT( DATA_DATABASE_MAX_LISTENERS, callback_counter );

    for ( int index = 0; index < (DATA_DATABASE_MAX_LISTENERS+1); index ++ )
    {
        data_database_listener_destroy( &my_listener[index] );
    }
    data_database_destroy ( &db );
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
