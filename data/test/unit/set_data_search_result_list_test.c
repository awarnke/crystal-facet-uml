/* File: set_data_search_result_list_test.c; Copyright and License: see below */

#include "set_data_search_result_list_test.h"
#include "set/data_search_result_list.h"
#include "entity/data_diagram_type.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_type_convertion_wrapper( test_fixture_t *test_env );

test_suite_t set_data_search_result_list_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "set_data_search_result_list_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_type_convertion_wrapper", &test_type_convertion_wrapper );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_type_convertion_wrapper( test_fixture_t *test_env )
{
    u8_error_t d_err;

    data_search_result_t my_found_diagram;
    data_search_result_init_diagram( &my_found_diagram,
                                     (data_row_id_t) 123,
                                     (int) DATA_DIAGRAM_TYPE_UML_USE_CASE_DIAGRAM,
                                     "root diag"
                                   );

    data_search_result_t buffer[1];
    data_search_result_list_t unit_under_test;
    data_search_result_list_init( &unit_under_test, 1, &buffer );

    const bool empty1 = data_search_result_list_is_empty( &unit_under_test );
    TEST_EXPECT_EQUAL_INT( true, empty1 );

    d_err = data_search_result_list_add_all( &unit_under_test, &unit_under_test );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );

    d_err = data_search_result_list_add( &unit_under_test, &my_found_diagram );
    TEST_EXPECT_EQUAL_INT( U8_ERROR_NONE, d_err );

    data_search_result_t *first_mod = data_search_result_list_get_ptr( &unit_under_test, 0 );
    TEST_EXPECT_EQUAL_PTR( &(buffer[0]), first_mod );

    const data_search_result_t *first_const = data_search_result_list_get_const( &unit_under_test, 0 );
    TEST_EXPECT_EQUAL_PTR( &(buffer[0]), first_const );

    const unsigned int len = data_search_result_list_get_length( &unit_under_test );
    TEST_EXPECT_EQUAL_INT( 1, len );

    /* check that trace an be called and that it returns */
    data_search_result_list_trace( &unit_under_test );

    data_search_result_list_clear( &unit_under_test );
    const unsigned int len2 = data_search_result_list_get_length( &unit_under_test );
    TEST_EXPECT_EQUAL_INT( 0, len2 );

    data_search_result_list_destroy( &unit_under_test );

    data_search_result_destroy( &my_found_diagram );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2024-2024 Andreas Warnke
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
