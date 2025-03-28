/* File: set_data_small_set_test.c; Copyright and License: see below */

#include "set_data_small_set_test.h"
#include "set/data_small_set.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_small_set_add_and_remove( test_fixture_t *test_env );
static test_case_result_t test_small_set_full( test_fixture_t *test_env );
static test_case_result_t test_small_set_clear( test_fixture_t *test_env );

test_suite_t set_data_small_set_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "set_data_small_set_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_small_set_add_and_remove", &test_small_set_add_and_remove );
    test_suite_add_test_case( &result, "test_small_set_full", &test_small_set_full );
    test_suite_add_test_case( &result, "test_small_set_clear", &test_small_set_clear );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_small_set_add_and_remove( test_fixture_t *test_env )
{
    data_small_set_t my_set;
    bool empty;
    bool is_in;
    data_id_t row_id;
    data_id_t row_id2;
    data_id_t row_inval;
    u8_error_t d_err;

    data_small_set_init ( &my_set );

    empty = data_small_set_is_empty ( &my_set );
    TEST_EXPECT_EQUAL_INT( true, empty );

    data_id_init( &row_id, DATA_TABLE_RELATIONSHIP, 12345678900 );

    is_in = data_small_set_contains( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( false, is_in );

    is_in = data_small_set_contains_row_id ( &my_set, DATA_TABLE_RELATIONSHIP, 12345678900 );
    TEST_EXPECT_EQUAL_INT( false, is_in );

    d_err = data_small_set_add_obj ( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );

    is_in = data_small_set_contains( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( true, is_in );

    is_in = data_small_set_contains_row_id ( &my_set, DATA_TABLE_RELATIONSHIP, 12345678900 );
    TEST_EXPECT_EQUAL_INT( true, is_in );

    d_err = data_small_set_add_obj ( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_DUPLICATE_ID, d_err );

    data_id_init_void( &row_inval );

    d_err = data_small_set_add_obj ( &my_set, row_inval );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, d_err );

    data_id_init( &row_id2, DATA_TABLE_CLASSIFIER, -37 );

    d_err = data_small_set_add_obj ( &my_set, row_id2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id2 );
    TEST_EXPECT_EQUAL_INT( true, is_in );

    data_small_set_trace ( &my_set );

    d_err = data_small_set_toggle_obj ( &my_set, row_inval );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, d_err );

    d_err = data_small_set_toggle_obj ( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( false, is_in );

    d_err = data_small_set_toggle_obj ( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( true, is_in );

    data_small_set_trace ( &my_set );

    d_err = data_small_set_delete_obj ( &my_set, row_id2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id2 );
    TEST_EXPECT_EQUAL_INT( false, is_in );

    d_err = data_small_set_delete_obj ( &my_set, row_id2 );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_INVALID_REQUEST, d_err );

    d_err = data_small_set_delete_obj ( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( false, is_in );

    empty = data_small_set_is_empty ( &my_set );
    TEST_EXPECT_EQUAL_INT( true, empty );

    data_small_set_destroy ( &my_set );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_small_set_full( test_fixture_t *test_env )
{

    data_small_set_t my_set;
    bool empty;
    bool is_in;
    data_id_t row_id;
    u8_error_t d_err;

    data_small_set_init ( &my_set );

    for ( int idx = 0; idx < DATA_SMALL_SET_MAX_SET_SIZE; idx ++ )
    {
        data_id_init( &row_id, DATA_TABLE_DIAGRAM, idx );
        d_err = data_small_set_add_obj ( &my_set, row_id );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
        is_in = data_small_set_contains( &my_set, row_id );
        TEST_EXPECT_EQUAL_INT( true, is_in );
    }

    data_id_init( &row_id, DATA_TABLE_DIAGRAM, 57000111222 );
    d_err = data_small_set_add_obj ( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( false, is_in );

    d_err = data_small_set_toggle_obj ( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_ARRAY_BUFFER_EXCEEDED, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( false, is_in );

    for ( int idx = 0; idx < DATA_SMALL_SET_MAX_SET_SIZE; idx ++ )
    {
        data_id_init( &row_id, DATA_TABLE_DIAGRAM, idx );
        d_err = data_small_set_delete_obj ( &my_set, row_id );
        TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );
        is_in = data_small_set_contains( &my_set, row_id );
        TEST_EXPECT_EQUAL_INT( false, is_in );
    }

    empty = data_small_set_is_empty ( &my_set );
    TEST_EXPECT_EQUAL_INT( true, empty );

    data_small_set_destroy ( &my_set );
    return TEST_CASE_RESULT_OK;
}

static test_case_result_t test_small_set_clear( test_fixture_t *test_env )
{
    data_small_set_t my_set;
    bool empty;
    data_id_t row_id;
    u8_error_t d_err;

    /* prepare */

    data_small_set_init ( &my_set );
    data_id_init( &row_id, DATA_TABLE_RELATIONSHIP, 12345678900 );
    d_err = data_small_set_add_obj ( &my_set, row_id );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );

    /* test on filled set */

    empty = data_small_set_is_empty ( &my_set );
    TEST_EXPECT_EQUAL_INT( false, empty );

    data_small_set_clear ( &my_set );

    empty = data_small_set_is_empty ( &my_set );
    TEST_EXPECT_EQUAL_INT( true, empty );

    /* test on empty set */

    data_small_set_clear ( &my_set );

    empty = data_small_set_is_empty ( &my_set );
    TEST_EXPECT_EQUAL_INT( true, empty );

    /* clean up */

    data_small_set_destroy ( &my_set );
    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2016-2025 Andreas Warnke
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
