/* File: set_data_stat_test.c; Copyright and License: see below */

#include "set_data_stat_test.h"
#include "set/data_stat.h"
#include "test_expect.h"
#include "test_environment_assert.h"

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *test_env );
static test_case_result_t test_init_add_get( test_fixture_t *test_env );

test_suite_t set_data_stat_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "set_data_stat_test",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_init_add_get", &test_init_add_get );
    return result;
}

static test_fixture_t * set_up()
{
    return NULL;
}

static void tear_down( test_fixture_t *test_env )
{
}

static test_case_result_t test_init_add_get( test_fixture_t *test_env )
{
    data_stat_t my_stat;

    data_stat_init( &my_stat );
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_count( &my_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_IGNORED ) );

    data_stat_inc_count( &my_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_IGNORED );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &my_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_IGNORED ) );

    data_stat_add_count( &my_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_IGNORED, 99 );
    TEST_EXPECT_EQUAL_INT( 100, data_stat_get_count( &my_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_IGNORED ) );

    data_stat_add( &my_stat, &my_stat );
    TEST_EXPECT_EQUAL_INT( 200, data_stat_get_count( &my_stat, DATA_TABLE_DIAGRAM, DATA_STAT_SERIES_IGNORED ) );

    TEST_EXPECT_EQUAL_INT( 200, data_stat_get_series_count( &my_stat, DATA_STAT_SERIES_IGNORED ) );
    TEST_EXPECT_EQUAL_INT( 200, data_stat_get_table_count( &my_stat, DATA_TABLE_DIAGRAM ) );
    TEST_EXPECT_EQUAL_INT( 200, data_stat_get_total_count( &my_stat ) );

    data_stat_reset_series(  &my_stat, DATA_STAT_SERIES_IGNORED );
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_series_count( &my_stat, DATA_STAT_SERIES_IGNORED ) );

    data_stat_trace( &my_stat );

    data_stat_destroy( &my_stat );

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
