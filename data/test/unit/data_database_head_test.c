/* File: data_database_head_test.c; Copyright and License: see below */

#include "data_database_head_test.h"
#include "storage/data_database_head.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <errno.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_create_head( test_fixture_t *fix );
static test_case_result_t test_read_head( test_fixture_t *fix );
static test_case_result_t test_update_head( test_fixture_t *fix );
static test_case_result_t test_delete_head( test_fixture_t *fix );

test_suite_t data_database_head_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "data_database_head_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_create_head", &test_create_head );
    test_suite_add_test_case( &result, "test_read_head", &test_read_head );
    test_suite_add_test_case( &result, "test_update_head", &test_update_head );
    test_suite_add_test_case( &result, "test_delete_head", &test_delete_head );
    return result;
}

struct test_fixture_struct {
    data_database_t database;  /*!< database instance on which the tests are performed */
};
typedef struct test_fixture_struct test_fixture_t;  /* double declaration as reminder */
static test_fixture_t test_fixture;

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    data_database_init( &((*fix).database) );
    data_database_open_in_memory( &((*fix).database) );
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
    data_database_close( &((*fix).database) );
    data_database_destroy( &((*fix).database) );
}

static const char *const TEST_KEY = "ID42";
static const char *const TEST_VAL = "VALUE 7890 2345 7890 2345 7890 2345 7890 2345 7890";

static test_case_result_t test_create_head( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_head_t value;

    data_database_head_t test_me;
    data_database_head_init( &test_me, &((*fix).database) );

    /* create/insert success */
    data_err = data_head_init_new( &value, TEST_KEY, TEST_VAL );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    data_row_t new_id = DATA_ROW_VOID;
    data_err = data_database_head_create_value( &test_me, &value, &new_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* read success */
    data_err = data_database_head_read_value_by_key( &test_me, TEST_KEY, &value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( new_id, data_head_get_row_id( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_KEY, data_head_get_key_const( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_VAL, data_head_get_value_const( &value ) );
    TEST_EXPECT_EQUAL_INT( true, data_head_has_value( &value ) );

    /* create/insert error */
    data_err = data_head_init_new( &value, TEST_KEY, TEST_VAL );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    new_id = DATA_ROW_VOID;
    data_err = data_database_head_create_value( &test_me, &value, &new_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DUPLICATE, data_err );

    data_database_head_destroy( &test_me );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_read_head( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_head_t value;

    data_database_head_t test_me;
    data_database_head_init( &test_me, &((*fix).database) );

    /* read error */
    data_err = data_database_head_read_value_by_key( &test_me, TEST_KEY, &value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NOT_FOUND, data_err );

    /* create/insert success */
    data_err = data_head_init_new( &value, TEST_KEY, TEST_VAL );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    data_row_t new_id = DATA_ROW_VOID;
    data_err = data_database_head_create_value( &test_me, &value, &new_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* read by key success */
    data_err = data_database_head_read_value_by_key( &test_me, TEST_KEY, &value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( new_id, data_head_get_row_id( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_KEY, data_head_get_key_const( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_VAL, data_head_get_value_const( &value ) );
    TEST_EXPECT_EQUAL_INT( true, data_head_has_value( &value ) );

    /* read by id success */
    data_err = data_database_head_read_value_by_id( &test_me, new_id, &value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( new_id, data_head_get_row_id( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_KEY, data_head_get_key_const( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_VAL, data_head_get_value_const( &value ) );
    TEST_EXPECT_EQUAL_INT( true, data_head_has_value( &value ) );

    data_database_head_destroy( &test_me );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_update_head( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_head_t value;

    data_database_head_t test_me;
    data_database_head_init( &test_me, &((*fix).database) );

    /* create/insert success */
    data_err = data_head_init_new( &value, TEST_KEY, TEST_VAL );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    data_row_t new_id = DATA_ROW_VOID;
    data_err = data_database_head_create_value( &test_me, &value, &new_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* update success, no result */
    data_err = data_database_head_update_value( &test_me, new_id, "OTHER", NULL );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* update success, with result */
    data_err = data_database_head_update_value( &test_me, new_id, "THIRD", &value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( new_id, data_head_get_row_id( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_KEY, data_head_get_key_const( &value ) );
    TEST_EXPECT_EQUAL_STRING( "OTHER", data_head_get_value_const( &value ) );
    TEST_EXPECT_EQUAL_INT( true, data_head_has_value( &value ) );

    /* read success */
    data_err = data_database_head_read_value_by_id( &test_me, new_id, &value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( new_id, data_head_get_row_id( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_KEY, data_head_get_key_const( &value ) );
    TEST_EXPECT_EQUAL_STRING( "THIRD", data_head_get_value_const( &value ) );
    TEST_EXPECT_EQUAL_INT( true, data_head_has_value( &value ) );

    data_database_head_destroy( &test_me );

    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_delete_head( test_fixture_t *fix )
{
    assert( fix != NULL );
    u8_error_t data_err;
    data_head_t value;

    data_database_head_t test_me;
    data_database_head_init( &test_me, &((*fix).database) );

    /* create/insert success */
    data_err = data_head_init_new( &value, TEST_KEY, TEST_VAL );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    data_row_t new_id = DATA_ROW_VOID;
    data_err = data_database_head_create_value( &test_me, &value, &new_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* delete success */
    data_err = data_database_head_delete_value( &test_me, new_id, &value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );
    TEST_EXPECT_EQUAL_INT( new_id, data_head_get_row_id( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_KEY, data_head_get_key_const( &value ) );
    TEST_EXPECT_EQUAL_STRING( TEST_VAL, data_head_get_value_const( &value ) );
    TEST_EXPECT_EQUAL_INT( true, data_head_has_value( &value ) );

    /* delete error */
    data_err = data_database_head_delete_value( &test_me, new_id, NULL );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, data_err );

    /* delete error */
    data_err = data_database_head_delete_value( &test_me, new_id, &value );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NOT_FOUND, data_err );

    data_database_head_destroy( &test_me );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2025 Andreas Warnke
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
