/* File: data_database_listener_test.c; Copyright and License: see below */

#include "data_database_listener_test.h"
#include "storage/data_database_listener.h"
#include "storage/data_database.h"

static void set_up(void);
static void tear_down(void);
static void test_database_listener_register_and_notify(void);
static void test_database_listener_registration_full(void);

TestRef data_database_listener_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_database_listener_register_and_notify",test_database_listener_register_and_notify),
        new_TestFixture("test_database_listener_registration_full",test_database_listener_registration_full),
    };
    EMB_UNIT_TESTCALLER(result,"data_database_listener_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
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
    data_error_t d_err;

    data_database_listener_init( &my_listener1, &callback_counter, (void (*)(void*,data_database_listener_signal_t)) &callback_notification );
    data_database_listener_init( &my_listener2, &callback_counter, (void (*)(void*,data_database_listener_signal_t)) &callback_notification );
    data_database_init ( &db );

    d_err = data_database_add_db_listener( &db, &my_listener1 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );

    d_err = data_database_add_db_listener( &db, &my_listener2 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );

    d_err = data_database_add_db_listener( &db, &my_listener1 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_INVALID_REQUEST, d_err );

    d_err = data_database_private_notify_db_listeners( &db, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    TEST_ASSERT_EQUAL_INT( 2, callback_counter );

    d_err = data_database_remove_db_listener( &db, &my_listener1 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );

    d_err = data_database_remove_db_listener( &db, &my_listener2 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );

    d_err = data_database_remove_db_listener( &db, &my_listener1 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_INVALID_REQUEST, d_err );

    d_err = data_database_private_notify_db_listeners( &db, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    TEST_ASSERT_EQUAL_INT( 2, callback_counter );

    data_database_destroy ( &db );
    data_database_listener_destroy ( &my_listener2 );
    data_database_listener_destroy ( &my_listener1 );
}

static void test_database_listener_registration_full(void)
{
    data_database_listener_t my_listener[GUI_DATABASE_MAX_LISTENERS+1];
    data_database_t db;
    data_error_t d_err;

    data_database_init ( &db );

    for ( int index = 0; index < GUI_DATABASE_MAX_LISTENERS; index ++ )
    {
        data_database_listener_init( &my_listener[index], &callback_counter, (void (*)(void*,data_database_listener_signal_t)) &callback_notification );
        d_err = data_database_add_db_listener( &db, &my_listener[index] );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    }
    data_database_listener_init( &my_listener[GUI_DATABASE_MAX_LISTENERS], &callback_counter, (void (*)(void*,data_database_listener_signal_t)) &callback_notification );
    d_err = data_database_add_db_listener( &db, &my_listener[GUI_DATABASE_MAX_LISTENERS] );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_ARRAY_BUFFER_EXCEEDED, d_err );

    d_err = data_database_private_notify_db_listeners( &db, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    TEST_ASSERT_EQUAL_INT( GUI_DATABASE_MAX_LISTENERS, callback_counter );

    data_database_private_clear_db_listener_list( &db );

    d_err = data_database_private_notify_db_listeners( &db, DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    TEST_ASSERT_EQUAL_INT( GUI_DATABASE_MAX_LISTENERS, callback_counter );

    for ( int index = 0; index < (GUI_DATABASE_MAX_LISTENERS+1); index ++ )
    {
        data_database_listener_destroy( &my_listener[index] );
    }
    data_database_destroy ( &db );
}


/*
 * Copyright 2016-2018 Andreas Warnke
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
