/* File: data_small_set_test.c; Copyright and License: see below */

#include "data_small_set_test.h"
#include "data_small_set.h"

static void set_up(void);
static void tear_down(void);
static void test_small_set_add_and_remove(void);
static void test_small_set_full(void);
static void test_small_set_clear(void);

TestRef data_small_set_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_small_set_add_and_remove",test_small_set_add_and_remove),
        new_TestFixture("test_small_set_full",test_small_set_full),
        new_TestFixture("test_small_set_clear",test_small_set_clear),
    };
    EMB_UNIT_TESTCALLER(result,"data_small_set_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_small_set_add_and_remove(void)
{
    data_small_set_t my_set;
    bool empty;
    bool is_in;
    data_id_t row_id;
    data_id_t row_id2;
    data_id_t row_inval;
    data_error_t d_err;

    data_small_set_init ( &my_set );

    empty = data_small_set_is_empty ( &my_set );
    TEST_ASSERT_EQUAL_INT( true, empty );

    data_id_init( &row_id, DATA_TABLE_RELATIONSHIP, 12345678900 );

    is_in = data_small_set_contains( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( false, is_in );

    is_in = data_small_set_contains_row_id ( &my_set, DATA_TABLE_RELATIONSHIP, 12345678900 );
    TEST_ASSERT_EQUAL_INT( false, is_in );

    d_err = data_small_set_add_obj ( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );

    is_in = data_small_set_contains( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( true, is_in );

    is_in = data_small_set_contains_row_id ( &my_set, DATA_TABLE_RELATIONSHIP, 12345678900 );
    TEST_ASSERT_EQUAL_INT( true, is_in );

    d_err = data_small_set_add_obj ( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_DUPLICATE_ID, d_err );

    data_id_init_void( &row_inval );

    d_err = data_small_set_add_obj ( &my_set, row_inval );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_INVALID_REQUEST, d_err );

    data_id_init( &row_id2, DATA_TABLE_CLASSIFIER, -37 );

    d_err = data_small_set_add_obj ( &my_set, row_id2 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id2 );
    TEST_ASSERT_EQUAL_INT( true, is_in );

    data_small_set_trace ( &my_set );

    d_err = data_small_set_toggle_obj ( &my_set, row_inval );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_INVALID_REQUEST, d_err );

    d_err = data_small_set_toggle_obj ( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( false, is_in );

    d_err = data_small_set_toggle_obj ( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( true, is_in );

    data_small_set_trace ( &my_set );

    d_err = data_small_set_delete_obj ( &my_set, row_id2 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id2 );
    TEST_ASSERT_EQUAL_INT( false, is_in );

    d_err = data_small_set_delete_obj ( &my_set, row_id2 );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_INVALID_REQUEST, d_err );

    d_err = data_small_set_delete_obj ( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( false, is_in );

    empty = data_small_set_is_empty ( &my_set );
    TEST_ASSERT_EQUAL_INT( true, empty );

    data_small_set_destroy ( &my_set );
}

static void test_small_set_full(void)
{

    data_small_set_t my_set;
    bool empty;
    bool is_in;
    data_id_t row_id;
    data_error_t d_err;

    data_small_set_init ( &my_set );

    for ( int idx = 0; idx < DATA_SMALL_SET_MAX_SET_SIZE; idx ++ )
    {
        data_id_init( &row_id, DATA_TABLE_DIAGRAM, idx );
        d_err = data_small_set_add_obj ( &my_set, row_id );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
        is_in = data_small_set_contains( &my_set, row_id );
        TEST_ASSERT_EQUAL_INT( true, is_in );
    }

    data_id_init( &row_id, DATA_TABLE_DIAGRAM, 57000111222 );
    d_err = data_small_set_add_obj ( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_ARRAY_BUFFER_EXCEEDED, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( false, is_in );

    d_err = data_small_set_toggle_obj ( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_ARRAY_BUFFER_EXCEEDED, d_err );
    is_in = data_small_set_contains( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( false, is_in );

    for ( int idx = 0; idx < DATA_SMALL_SET_MAX_SET_SIZE; idx ++ )
    {
        data_id_init( &row_id, DATA_TABLE_DIAGRAM, idx );
        d_err = data_small_set_delete_obj ( &my_set, row_id );
        TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );
        is_in = data_small_set_contains( &my_set, row_id );
        TEST_ASSERT_EQUAL_INT( false, is_in );
    }

    empty = data_small_set_is_empty ( &my_set );
    TEST_ASSERT_EQUAL_INT( true, empty );

    data_small_set_destroy ( &my_set );
}

static void test_small_set_clear(void)
{
    data_small_set_t my_set;
    bool empty;
    data_id_t row_id;
    data_error_t d_err;

    /* prepare */

    data_small_set_init ( &my_set );
    data_id_init( &row_id, DATA_TABLE_RELATIONSHIP, 12345678900 );
    d_err = data_small_set_add_obj ( &my_set, row_id );
    TEST_ASSERT_EQUAL_INT( DATA_ERROR_NONE, d_err );

    /* test on filled set */

    empty = data_small_set_is_empty ( &my_set );
    TEST_ASSERT_EQUAL_INT( false, empty );

    data_small_set_clear ( &my_set );

    empty = data_small_set_is_empty ( &my_set );
    TEST_ASSERT_EQUAL_INT( true, empty );

    /* test on empty set */

    data_small_set_clear ( &my_set );

    empty = data_small_set_is_empty ( &my_set );
    TEST_ASSERT_EQUAL_INT( true, empty );

    /* clean up */

    data_small_set_destroy ( &my_set );
}


/*
 * Copyright 2016-2016 Andreas Warnke
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
