/* File: set_data_full_id_test.c; Copyright and License: see below */

#include "set_data_full_id_test.h"
#include "set/data_full_id.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_init_and_read( test_fixture_t *test_env );

test_suite_t set_data_full_id_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "set_data_full_id_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_init_and_read", &test_init_and_read );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_init_and_read( test_fixture_t *test_env )
{
    data_full_id_t test_me;
    data_id_t classifier;
    data_id_init( &classifier, DATA_TABLE_CLASSIFIER, 67432 );
    data_id_t feature;
    data_id_init( &feature, DATA_TABLE_FEATURE, 10000 );

    /* sub test case 1 */
    data_full_id_init_solo( &test_me, &classifier );
    const data_id_t *const read_out = data_full_id_get_primary_id_const( &test_me );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &classifier, read_out ) );

    /* sub test case 2, no expected outcome */
    data_full_id_destroy( &test_me );

    /* sub test case 3 */
    data_full_id_init( &test_me, &feature, &classifier );
    data_id_t sensor_value1 = data_full_id_get_primary_id( &test_me );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &feature, &sensor_value1 ) );
    data_id_t sensor_value2 = data_full_id_get_secondary_id( &test_me );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &classifier, &sensor_value2 ) );

    /* sub test case 4 */
    data_full_id_reinit_void( &test_me );
    TEST_EXPECT_EQUAL_INT( false, data_full_id_is_valid( &test_me ) );
    data_full_id_destroy( &test_me );

    /* sub test case 5 */
    data_full_id_init_void ( &test_me );
    TEST_EXPECT_EQUAL_INT( false, data_full_id_is_valid( &test_me ) );
    data_full_id_destroy( &test_me );

    /* sub test case 6 */
    data_full_id_init_by_table_and_id( &test_me, DATA_TABLE_FEATURE, 200, DATA_TABLE_CLASSIFIER, 689 );
    TEST_EXPECT_EQUAL_INT( true, data_full_id_is_valid( &test_me ) );

    /* sub test case 7 */
    data_full_id_t mirrored;
    data_full_id_copy( &mirrored, &test_me );
    TEST_EXPECT_EQUAL_INT( true, data_full_id_equals( &test_me, &mirrored ) );

    /* sub test case 8 */
    data_full_id_reinit_by_table_and_id( &test_me, DATA_TABLE_FEATURE, 10000, DATA_TABLE_CLASSIFIER, 67432 );
    TEST_EXPECT_EQUAL_INT( false, data_full_id_equals( &test_me, &mirrored ) );

    /* sub test case 8 */
    data_full_id_replace( &mirrored, &test_me );
    const data_id_t *const sensor_value1b = data_full_id_get_primary_id_const( &test_me );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &feature, sensor_value1b ) );
    const data_id_t *const sensor_value2b = data_full_id_get_secondary_id_const( &mirrored );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &classifier, sensor_value2b ) );

    /* sub test case 9, no expected outcome other than program continues */
    data_full_id_trace( &mirrored );

    /* sub test case 10 */
    data_full_id_reinit_solo( &test_me, &classifier );
    const data_id_t *const read_out_c = data_full_id_get_primary_id_const( &test_me );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &classifier, read_out_c ) );

    /* sub test case 11 */
    data_full_id_reinit( &test_me, &feature, &classifier );
    const data_id_t *const sensor_value1c = data_full_id_get_primary_id_const( &test_me );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &feature, sensor_value1c ) );
    const data_id_t *const sensor_value2c = data_full_id_get_secondary_id_const( &test_me );
    TEST_EXPECT_EQUAL_INT( true, data_id_equals( &classifier, sensor_value2c ) );

    data_full_id_destroy( &test_me );

    data_id_destroy( &feature );
    data_id_destroy( &classifier );

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
