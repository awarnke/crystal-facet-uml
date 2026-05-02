/* File: consistency_stat_test.c; Copyright and License: see below */

#include "consistency_stat_test.h"
#include "consistency/consistency_stat.h"
#include "set/data_stat.h"
#include "test_fixture.h"
#include "test_expect.h"
#include "test_environment_assert.h"
#include "test_case_result.h"
#include <assert.h>

static test_fixture_t * set_up();
static void tear_down( test_fixture_t *fix );
static test_case_result_t test_transfer_to( test_fixture_t *fix );

test_suite_t consistency_stat_test_get_suite(void)
{
    test_suite_t result;
    test_suite_init( &result,
                     "consistency_stat",
                     TEST_CATEGORY_UNIT | TEST_CATEGORY_CONTINUOUS | TEST_CATEGORY_COVERAGE,
                     &set_up,
                     &tear_down
                   );
    test_suite_add_test_case( &result, "test_transfer_to", &test_transfer_to );
    return result;
}

struct test_fixture_struct {
    int dummy;
};

typedef struct test_fixture_struct test_fixture_t;
static test_fixture_t test_fixture = { .dummy = 1 };

static test_fixture_t * set_up()
{
    test_fixture_t *fix = &test_fixture;
    return fix;
}

static void tear_down( test_fixture_t *fix )
{
    assert( fix != NULL );
}

static test_case_result_t test_transfer_to( test_fixture_t *fix )
{
    consistency_stat_t c_stat;
    consistency_stat_init( &c_stat );
    data_stat_t d_stat;
    data_stat_init( &d_stat );

    consistency_stat_transfer_to( &c_stat, &d_stat );
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_total_count( &d_stat ));

    consistency_stat_increment_classifiers ( &c_stat );
    consistency_stat_decrement_classifiers ( &c_stat );
    consistency_stat_decrement_classifiers ( &c_stat );

    consistency_stat_increment_features ( &c_stat );
    consistency_stat_decrement_features( &c_stat );
    consistency_stat_decrement_features( &c_stat );
    consistency_stat_decrement_features( &c_stat );

    consistency_stat_increment_lifelines( &c_stat );
    consistency_stat_decrement_lifelines( &c_stat );
    consistency_stat_decrement_lifelines( &c_stat );

    consistency_stat_increment_relationships( &c_stat );
    consistency_stat_decrement_relationships( &c_stat );
    consistency_stat_subtract_relationships( &c_stat, 17 );

    consistency_stat_transfer_to( &c_stat, &d_stat );
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &d_stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count( &d_stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 1, data_stat_get_count( &d_stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 17, data_stat_get_count( &d_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_count( &d_stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_count( &d_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 21, data_stat_get_total_count( &d_stat ));

    consistency_stat_transfer_to( &c_stat, &d_stat );
    TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count( &d_stat, DATA_STAT_TABLE_CLASSIFIER, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 4, data_stat_get_count( &d_stat, DATA_STAT_TABLE_FEATURE, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 2, data_stat_get_count( &d_stat, DATA_STAT_TABLE_LIFELINE, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 34, data_stat_get_count( &d_stat, DATA_STAT_TABLE_RELATIONSHIP, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_count( &d_stat, DATA_STAT_TABLE_DIAGRAMELEMENT, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 0, data_stat_get_count( &d_stat, DATA_STAT_TABLE_DIAGRAM, DATA_STAT_SERIES_DELETED ));
    TEST_EXPECT_EQUAL_INT( 42, data_stat_get_total_count( &d_stat ));

    data_stat_destroy( &d_stat );
    consistency_stat_destroy( &c_stat );

    return TEST_CASE_RESULT_OK;
}


/*
 * Copyright 2026-2026 Andreas Warnke
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
